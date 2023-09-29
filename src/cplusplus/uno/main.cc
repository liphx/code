#include "gflags/gflags.h"
#include "glog/logging.h"
#include "uno/server.h"

DEFINE_bool(server, true, "");
DEFINE_int32(port, 8000, "");
DEFINE_string(host, "0.0.0.0", "");

DEFINE_bool(client, false, "");

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_server) {
        uno::Server server;
        LOG(INFO) << "Server start at port: " << FLAGS_port;
        server.Start(FLAGS_host, FLAGS_port);
    }
}
