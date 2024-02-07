#define NOB_IMPLEMENTATION
#include "./src/nob.h"

bool build_app(void)
{
    bool result = true;
    Nob_Cmd cmd = {0};

    if(!nob_mkdir_if_not_exists("build")) return 1;
    
    cmd.count = 0;
        nob_cmd_append(&cmd, "clang");
        nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb");
        nob_cmd_append(&cmd, "-o", "build/life");
        nob_cmd_append(&cmd,
                       "./src/main.c");
        nob_cmd_append(&cmd, "-lncurses");
    if(!nob_cmd_run_sync(cmd)) nob_return_defer(false);

defer:
    nob_cmd_free(cmd);
    return result;
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if(!build_app()) return 1;
}
