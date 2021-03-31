#include "tcp.h"
#include "common.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netdb.h>
using namespace std;


const int BUFFSIZE = 1024;

set<string> sites = {
    "liph.ink",
    "www.liph.ink",
    "resource.liph.ink",
    "www.freeday.top",
    "freeday.top",
    "chat.freeday.top",
    "jasmine.freeday.top"
};

int get_host(const string& req, string& host, int& port)
{
    string::size_type pos = req.find("Host:");
    if (pos != string::npos) {
        pos += 5;
        while (pos != string::npos && req[pos] != '\r' && req[pos] != '\n' && req[pos] != ':') {
            if (req[pos] != ' ')
                host += req[pos];
            pos++;
        }
        if (req[pos] == ':') { // with port
            pos++;
            string tmp;
            while (pos != string::npos && req[pos] != '\r' && req[pos] != '\n') {
                tmp += req[pos++];
            }
            port = stoi(tmp);
        } else {
            port = 80;
        }
    }
    return 0;
}

void change_http_version(string& req)
{
    string str = "HTTP/1.1";
    string str2 = "HTTP/1.0";
    auto pos = req.find(str);
    if (pos != string::npos)
        req = req.replace(pos, str.length(), str2);
}

void change_header(string& req, const string& host, const string& ip)
{
    auto pos = req.find("Host:");
    if (pos == string::npos)
        return;
    pos = req.find(host, pos);
    if (pos == string::npos)
        return;
    
    req = req.replace(pos, host.length(), ip);
}

bool check_request_header(string req)
{
    if (req.find("\r\n\r\n") != string::npos)
        return true;
    return false;
}

int host2ip(const string& host, string& ip)
{
    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL)
        return -1;
    
    char **addr = server->h_addr_list;
    if (addr && *addr) {
        for (int i = 0; i < 4; i++) {
            ip += to_string((unsigned char)(*addr)[i]);
            if (i < 3)
                ip += ".";
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int port = 0;
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
        case 'p':
            port = atoi(optarg);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-p port]" << std::endl;
            exit(1);
        }
    }

    tcp_server server(port);

    server.start([](const tcp_connection& client) {
        print("--- accept ---");
        string req;
        char buf[BUFFSIZE];
        int size = client.read(buf, BUFFSIZE);
        req += string(buf, size);
        while (size > 0 && !check_request_header(req)) {
            size = client.read(buf, BUFFSIZE);
            req += string(buf, size);
        }

        string host_;
        int port_;
        int ret = get_host(req, host_, port_);
        if (ret < 0)
           return;

        string ip_ = "";
        if (host2ip(host_, ip_) < 0)
            return;

        print(host_, ip_, port_);
        change_http_version(req);

        if (sites.find(host_) != sites.end())
            change_header(req, host_, ip_);

        tcp_connection *remote = nullptr;
        try {
            remote = new tcp_connection(ip_.c_str(), port_);
        } catch (exception& e) {
            cerr << e.what() << endl;
            return;
        }

        size = remote->writen(req.data(), req.length());
        if (size_t(size) != req.length()) {
            cerr << "write error: " << strerror(errno) << endl;
            delete remote;
            return;
        }

        string res;
        size = remote->read(buf, BUFFSIZE);
        res += string(buf, size);
        while (size > 0) {
            size = remote->read(buf, BUFFSIZE);
            res += string(buf, size);
        }
        delete remote;
        
        client.write(res.data(), res.length());
    });

    exit(0);
}
