#define NOB_IMPLEMENTATION
#include "./src/nob.h"

#include <string.h>

#define COMMON_CFLAGS "-Wall", "-Wextra", "-pedantic", "-ggdb"

bool build_term(Nob_Cmd *cmd) {
	bool result = true;
	if(!nob_mkdir_if_not_exists("build")) return 1;
	nob_cmd_append(cmd, "clang", COMMON_CFLAGS, "-o", "build/game-of-life", "src/main.c", "-lncurses");
	if(!nob_cmd_run_sync(*cmd)) result = false;
	cmd->count = 0;
	return result;
}

bool build_wasm(Nob_Cmd *cmd) {
	bool result = true;
	if(!nob_mkdir_if_not_exists("wasm")) return 1;
	nob_cmd_append(cmd, "clang", COMMON_CFLAGS, "--target=wasm32", "-fno-builtin", "-nostdlib", "-Wl,--allow-undefined", "-Wl,--no-entry", "-Wl,--export-all", "./src/game.c", "-o", "./wasm/game.wasm");
	if(!nob_cmd_run_sync(*cmd)) result = false;
	cmd->count = 0;
	return result;
}

void usage(const char *program) {
	nob_log(NOB_INFO, "usage: %s [target]", program);
	nob_log(NOB_INFO, "available targets: term, wasm");
}

int main(int argc, char **argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = {0};
	Nob_Procs procs = {0};

	const char *program = nob_shift_args(&argc, &argv);
	if(argc > 0) {
		const char *subcmd = nob_shift_args(&argc, &argv);
		if(strcmp(subcmd, "term") == 0) {
			if(!build_term(&cmd)) return 1;
		} else if(strcmp(subcmd, "wasm") == 0) {
			if(!build_wasm(&cmd)) return 1;
		} else {
			nob_log(NOB_ERROR, "unrecognized subcommand `%s`", subcmd);
			usage(program);
			return 1;
		}
	} else {
		if(!build_term(&cmd)) return 1;
		if(!build_wasm(&cmd)) return 1;
	}

	nob_cmd_free(cmd);
	return 0;
}
