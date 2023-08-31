#include "liph/liph.h"
using namespace std;
using namespace liph;

multimap<int, string, greater<int>> conf;
const int max_size = 15;
string cmd;
string conf_file;

void get_conf() {
    string str;
    if (!read_file(conf_file, str)) return;
    auto lines = split(str, "\n");
    for (auto& line : lines) {
        auto s = trim(line);
        if (s.empty()) continue;
        stringstream ss(s);
        int n;
        string path;
        ss >> n;
        ss >> path;
        conf.insert(make_pair(n, path));
    }
}

void save_conf() {
    stringstream ss;
    int i = 0;
    for (const auto& [n, s] : conf) {
        ss << n << " " << s << "\n";
        i++;
        if (i == max_size) break;
    }
    (void)write_file(conf_file, ss.str());
}

void open(const string& args) {
    auto it = conf.begin();
    for (; it != conf.end(); ++it) {
        if (it->second == args) {
            break;
        }
    }
    if (it == conf.end()) {
        conf.insert(make_pair(1, args));
    } else {
        pair<int, string> item = *it;
        conf.erase(it);
        item.first++;
        conf.insert(item);
    }
    system((cmd + " " + args).c_str());
    save_conf();
}

int main(int argc, char **argv) {
    if (argc == 1) return 1;
    cmd = argv[1];
    for (char ch : cmd) {
        if (!isalnum(ch)) return 1;
    }
    conf_file = "/tmp/" + cmd + ".conf";
    get_conf();
    if (argc == 2) {
        int i = 1;
        for (const auto& [n, s] : conf) {
            cout << i++ << " " << s << "\n";
        }
        cout << "\n\n\ninput: ";
        size_t n = 0;
        cin >> n;
        if (n > 0 && n <= conf.size()) {
            auto it = conf.begin();
            advance(it, n - 1);
            open(it->second);
        }

        return 0;
    }
    open(argv[2]);
}
