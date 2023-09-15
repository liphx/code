#include "liph/file.h"
#include "liph/flags.h"
#include "liph/singleton.h"
#include "online/server.h"

int main(int argc, char **argv) {
    auto flags = liph::singleton<liph::flags>::instance();
    flags.register_string_flag("host", "0.0.0.0", "server host");
    flags.register_int32_flag("port", 9000, "server port");
    flags.register_string_flag("saved_files", "/tmp/saved_files", "default path for saved files");
    flags.register_string_flag("db_path", "/tmp/user.db", "path for user db");
    flags.register_bool_flag("test", false, "for test");
    flags.parse_flags(argc, &argv);

    liph::mkdirp(flags.string_ref("saved_files"));

    /* liph::timer timer; */
    /* int clear_session_interval = 10 * 60;    // 10 mins */
    /* int clear_file_interval = 24 * 60 * 60;  // 1 day */
    /* timer.add(clear_session_interval, clear_session); */
    /* timer.add(clear_file_interval, clear_file); */
    /* timer.start(); */

    online::Server svr;
    svr.start(flags.string_ref("host"), flags.int32_ref("port"));
}
