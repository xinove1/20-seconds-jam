#define NOB_IMPLEMENTATION
#include "nob.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NO_RAYLIB
#include "./src/modules/core.h"

// Some folder paths that we use throughout the build process.
#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src_teste/"
// Paths
#define RAYLIB_PATH "./external/raylib-5.5/"
#define BOX2D_PATH "./external/box2d-3.1.1/"
// Paths for emscrippten build
#define EMDSK_PATH "~/emsdk/"
#define PYTHON_PATH "/bin/python3"
#define NODEJS_PATH "~/emsdk/node/22.16.0_64bit/bin/"

#define flag_compare(flag, ...) strcmp_many(flag, ((const char*[]){__VA_ARGS__}), (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))
#define da_append_cstrs(da, cstrs) nob_da_append_many(&(da), cstrs, count_of(cstrs)) 
b32 strcmp_many(const cstr *str, const cstr **str_many, i32 str_many_count);
internal inline i32 get_stat_time(cstr *path);

internal void build_windows_msvc();
internal void build_windows_msvc_hot();
internal void build_linux_static(){ nob_log(NOB_ERROR, "build linux static NOT IMPLEMENTED");}
internal void build_linux_hot(){ nob_log(NOB_ERROR, "build linux hot NOT IMPLEMENTED");}
internal void build_and_run_hot(){ nob_log(NOB_ERROR, "build and run hot NOT IMPLEMENTED");}
internal void build_windows_from_linux(){}
internal void build_web();
internal void build_and_run_web(){ nob_log(NOB_ERROR, "build and run web NOT IMPLEMENTED");}
internal void raylib_go_rebuild_urself(){ nob_log(NOB_ERROR, "raylib go rebuild urself NOT IMPLEMENTED");}
internal void build_raylib_web();
internal void build_box2d();
internal void build_test_file(cstr *file){ nob_log(NOB_ERROR, "build test file NOT IMPLEMENTED");}

const global cstr *ProjectOutputName = "game";
const global cstr *CC = "clang";
const global cstr *AssetsFolder = "assets";
const global cstr *WebHTMLTemplate = "./template.html";
const global cstr *IncludesJS = "lib.js";

const global cstr *DebugFlags[] = {
	"-g3",
	"-Wall",
	"-Wextra",
	"-Wno-unused-parameter",
	"-Wno-unused-function",
	"-Wno-initializer-overrides",
        //"-fsanitize=address", // BUG  remeber that this causes leak on xilib, so not good for long running the game, only for testing for leaks
	"-fsanitize=undefined",
	//"-fsanitize-trap",
	"-DBUILD_DEBUG",
        //"-Wconversion",
	//"-Wno-sign-conversion",
	//"-Werror",
	//"-Wdouble-promotion",
};
const global cstr *SharedFlags[] = { "-I./src/modules/", "-I"RAYLIB_PATH"/src", "-I./external/"};
const global cstr *HotFlags[] = {"-DHOT_RELOAD", "-DBUILD_DEBUG", "-Wl,-rpath=./build/", "-L./build/",  "-lraylib" };
const global cstr *HotFlagsWindows[] = {"-DHOT_RELOAD", "-DBUILD_DEBUG", "-I.\\external\\", "-I.\\build" };
const global cstr *WindowsLibs[] = {"gdi32.lib", "msvcrt.lib", "raylibdll.lib", "winmm.lib", "user32.lib", "shell32.lib"};
const global cstr *WindowsFlags[] = {"/Zi", "/Fe:.\\build\\", "/Fd:.\\build\\", "/Fo:.\\build\\", "-I.\\src\\modules\\", "-I.\\raylib-5.5_win64_msvc16\\include"};
const global cstr *WindowsFlagsLast[] = {"/link", "/libpath:.\\build", "/libpath:.\\raylib-5.5_win64_msvc16\\lib", "/NODEFAULTLIB:libcmt"};

const global cstr *Src_EngineLayer[] = {"./src/main.c", };
const global cstr *Dep_EngineLayer[] = {"./src/main.h"};

const global cstr *Src_Modules[] = { "./src/implementations.c", };
const global cstr *Dep_Modules[] = { 
	"./src/modules/core.h", 
	"./src/modules/clay_renderer.h", 
	"./src/modules/list.h", "./src/modules/arenas.h", 
	"./src/modules/collision.h", "./src/modules/raymath_short.h",
	"./src/modules/input.h", 
	"./src/modules/clay_renderer.h", 
	"./src/modules/xui.h",  "./src/modules/microui.h",  "./src/modules/mui.h",
	"./src/modules/sprite.h",
};

const global cstr *Src_Game[] = { 
	"./src/game_code/game.c",
	"./src/game_code/editor.c",
	"./src/game_code/utils.c",
	"./src/game_code/entitys.c",
	"./src/game_code/render.c",
};
const global cstr *Dep_Game[] = { "./src/game_code/game.h", "./src/game_code/entitys.h", "./src/game_code/render.h", };

// TODO  Be able to toggle it with command line flag
global b32 Debug = true; 
global b32 ReLaunch = true;

int main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF(argc, argv);

	if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

	nob_shift_args(&argc, &argv); // get program name out

	//raylib_go_rebuild_urself();

	cstr *flag;
	if (argc == 0) {
		flag = "hot"; // default build
	} else flag = nob_shift_args(&argc, &argv);

	if (flag_compare(flag, "r")) {
		nob_log(NOB_INFO, "Just recompiling, doing nothing");
	}
	else if (flag_compare(flag, "t")) {
		build_box2d();
	}
	else if (flag_compare(flag, "m")) {
		//introspect_files("./src/game_code/meta_generated.h", (cstr **) Dep_Game, count_of(Dep_Game));
		//parse_file("./src/game_code/entitys.h");
	} 
	else if (flag_compare(flag, "msvc")) {
		build_windows_msvc();
	} 
	else if (flag_compare(flag, "hot")) {
		#ifdef _WIN32
		build_windows_msvc_hot();
		#else
		build_linux_hot();
		#endif
	} 
	else if (flag_compare(flag, "linux_static")) {
		Debug = false;
		build_linux_static();
	}
	else if (flag_compare(flag, "web")) {
		build_web();
	}
	else if (flag_compare(flag, "web_run", "wr")) {
		build_and_run_web();
	}
	else if (flag_compare(flag, "windows")) {
		Debug = false;
		build_windows_from_linux();
	} 
	else if (flag_compare(flag, "hot_run", "rh")) {
		build_and_run_hot();
	}
	else if (flag_compare(flag, "run", "r")) {
		Nob_Cmd cmd = {0};
		nob_cmd_append(&cmd, "./build/game");
		nob_cmd_run_sync(cmd);
		return (0);
	} 
	else if (flag_compare(flag, "clean")) {
		Nob_Cmd cmd = {0};
		nob_cmd_append(&cmd, "rm -rf ./build/");
		if (!nob_cmd_run_sync(cmd)) return (1);
		nob_cmd_free(cmd);
	}
	else if (flag_compare(flag, "test")) {
		if (argc > 0) {
			build_test_file(nob_shift_args(&argc, &argv));
		} else {
			nob_log(NOB_ERROR, "Please provide file to be compiled.");
		}
	}
	else if (flag_compare(flag, "help", "-help", "--help", "-h")) {
		printf("--------------Help--------------\n");
		printf("Please fill this eventually lol\n");
	} 
	else {
		printf("\n");
		nob_log(NOB_ERROR, "flag %s unrecognized. run with '-help' for the available build options \n", flag);
	}

	if (argc != 0) {
		nob_log(NOB_INFO, "Only 1 flag alowed for now\n");
	}

	return (0);
}

internal void build_windows_msvc()
{
	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "cl",); //  "-I."
	nob_da_append_many(&cmd, Src_Game, count_of(Src_Game));
	nob_da_append_many(&cmd, Src_EngineLayer, count_of(Src_EngineLayer));
	nob_da_append_many(&cmd, Src_Modules, count_of(Src_Modules));
	nob_da_append_many(&cmd, WindowsLibs, count_of(WindowsLibs));
	nob_da_append_many(&cmd, WindowsFlags, count_of(WindowsFlags));
	if (Debug) nob_da_append(&cmd, "-DBUILD_DEBUG");
	nob_da_append_many(&cmd, WindowsFlagsLast, count_of(WindowsFlagsLast));
	if (!nob_cmd_run(&cmd)) return ;
	// nob_cmd_append(&cmd, ".\\"BUILD_FOLDER"main.exe");
	// if (!nob_cmd_run(&cmd)) return 1;
}

internal void build_windows_msvc_executable()
{
	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "cl",); //  "-I."
	nob_da_append_many(&cmd, Src_EngineLayer, count_of(Src_EngineLayer));
	nob_da_append_many(&cmd, HotFlagsWindows, count_of(HotFlagsWindows));
	nob_da_append_many(&cmd, WindowsLibs, count_of(WindowsLibs));
//	nob_cmd_append(&cmd, "xilib.lib");
	nob_da_append_many(&cmd, WindowsFlags, count_of(WindowsFlags));
	nob_da_append_many(&cmd, WindowsFlagsLast, count_of(WindowsFlagsLast));
	if (!nob_cmd_run(&cmd)) return ;
}

internal void build_windows_msvc_dll()
{
	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "cl",); //  "-I."
	nob_cmd_append(&cmd, "/LD",); //  "-I."
	nob_da_append_many(&cmd, Src_Game, count_of(Src_Game));
	nob_da_append_many(&cmd, Src_Modules, count_of(Src_Modules));
	nob_da_append_many(&cmd, HotFlagsWindows, count_of(HotFlagsWindows));
	nob_da_append_many(&cmd, WindowsLibs, count_of(WindowsLibs));
	nob_da_append_many(&cmd, WindowsFlags, count_of(WindowsFlags));
	nob_da_append_many(&cmd, WindowsFlagsLast, count_of(WindowsFlagsLast));
	if (!nob_cmd_run(&cmd)) return ;
}

internal void build_windows_msvc_hot()
{
	Nob_Cmd cmd = {0};

	printf("---------------------- BUILDING HOT ------------------------------\n");
	if (!nob_file_exists(".\\build\\raylib.dll")) {
		nob_copy_file(".\\raylib-5.5_win64_msvc16\\lib\\raylib.dll", ".\\build\\raylib.dll");
	}
	build_windows_msvc_executable();
	build_windows_msvc_dll();
	
	i32 libgame_time = get_stat_time(".\\build\\game.dll");
	// nob_cmd_append(&cmd, ".\\build\\main.exe");
	// Nob_Proc proc;
	// proc = nob_cmd_run_async(cmd);
	//if (!nob_cmd_run(&cmd, &procs)) return;

	Nob_Cmd dep_engine = {0}; da_append_cstrs(dep_engine, Src_EngineLayer); da_append_cstrs(dep_engine, Dep_EngineLayer);
	Nob_Cmd dep_game = {0}; 
	da_append_cstrs(dep_game, Src_Game); da_append_cstrs(dep_game, Dep_Game);
	da_append_cstrs(dep_game, Src_Modules); da_append_cstrs(dep_game, Dep_Modules);

	while (1) {
		if (nob_needs_rebuild("./build/game.dll", dep_game.items, dep_game.count) > 0) {
			printf("------------ REBUILDING GAME LIB --------------- \n");
			build_windows_msvc_dll();
			printf("------------------------------------------------ \n");
		}
	}

	// printf("df neee\n");
	// while (nob__proc_wait_async(proc,  100)) {
	// 	printf("oi neee\n");
	// }
}

internal void build_web()
{
	build_raylib_web();
	Nob_Cmd cmd = {0};

	nob_cmd_append(&cmd, "emcc");

	// Flags
	nob_cmd_append(&cmd, "-DPLATFORM_WEB");
	nob_cmd_append(&cmd, "--preload-file", AssetsFolder);
	nob_cmd_append(&cmd, "--shell-file", WebHTMLTemplate);
	nob_cmd_append(&cmd, "--js-library", IncludesJS);
	nob_cmd_append(&cmd, "-sEXPORTED_FUNCTIONS=_main, _pause_game");
	nob_cmd_append(&cmd, "-s", "ALLOW_MEMORY_GROWTH=1", "-s", "EXPORTED_RUNTIME_METHODS=ccall,cwrap");
	nob_cmd_append(&cmd, "-s", "STACK_SIZE=1mb", "-Os", "-s", "USE_GLFW=3", "-sGL_ENABLE_GET_PROC_ADDRESS");

	nob_cmd_append(&cmd, "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11",); // raylib
	nob_da_append_many(&cmd, SharedFlags, count_of(SharedFlags));
	if (Debug) nob_da_append_many(&cmd, DebugFlags, count_of(DebugFlags));
	
	// Sources
	nob_da_append_many(&cmd, Src_Game, count_of(Src_Game));
	nob_da_append_many(&cmd, Src_EngineLayer, count_of(Src_EngineLayer));
	nob_da_append_many(&cmd, Src_Modules, count_of(Src_Modules));
	
	nob_cmd_append(&cmd, "./build/libraylib_web.a");

	Nob_String_Builder name = {0};
	nob_sb_append_cstr(&name, "./build/");
	nob_sb_append_cstr(&name, ProjectOutputName);
	nob_sb_append_cstr(&name, ".html");
	nob_sb_append_null(&name);
	nob_cmd_append(&cmd, "-o", name.items, );

	if (!nob_cmd_run_sync(cmd)) exit(1);
}

internal void build_raylib_web()
{
	if (nob_file_exists("./build/libraylib_web.a"))
		return ;
	nob_log(NOB_INFO, "Building raylib web -----\n");
	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "make", "-C", RAYLIB_PATH"/src/");
	// NOTE  hard coded paths
	nob_cmd_append(&cmd, "PLATFORM=PLATFORM_WEB", "-B", "EMSDK_PATH="EMDSK_PATH,  "PYTHON_PATH="PYTHON_PATH, "NODE_PATH="NODEJS_PATH);
	if (!nob_cmd_run_sync(cmd)) {
		// exit(1);
	}

	if (!nob_copy_file(RAYLIB_PATH"/src/libraylib.a", "./build/libraylib_web.a")) {
		// exit(1);
	}

	//nob_cmd_free(cmd);
	cmd.count = 0;
	nob_cmd_append(&cmd, "make", "-C", RAYLIB_PATH"/src/");
	nob_cmd_append(&cmd, "clean");
	if (!nob_cmd_run_sync(cmd)) exit(1);

	nob_cmd_free(cmd);

	nob_log(NOB_INFO, "---------------------------------\n");
}

internal void build_box2d()
{
	if (!nob_mkdir_if_not_exists(BOX2D_PATH"build")) return exit(1);

	#ifdef _WIN32
		if (nob_file_exists("./build/box2d.lib")) return ;
		Nob_Cmd cmd = {0};
		nob_cmd_append(&cmd, "cmake", "..");
		if (!nob_cmd_run(&cmd, .pwd = BOX2D_PATH"build")) exit(1);
		nob_cmd_append(&cmd, "cmake", "--build", ".", "--config", "release");
		if (!nob_cmd_run(&cmd, .pwd = BOX2D_PATH"build")) exit(1);
		if (!nob_copy_file(BOX2D_PATH"build/src/Release/box2d.lib", "./build/box2d.lib")) {
			//nob_log(NOB_ERROR, "Coulndt "):
		}
	#else
		if (nob_file_exists("./build/libbox2d.a")) return ;
		Nob_Cmd cmd = {0};
		nob_cmd_append(&cmd, "rm", "-rf", BOX2D_PATH"build");
		if (!nob_cmd_run(&cmd)) exit(1);
		if (!nob_mkdir_if_not_exists(BOX2D_PATH"build")) return exit(1);

		nob_cmd_append(&cmd, "emcmake", "cmake", "-DBOX2D_VALIDATE=OFF", "-DBOX2D_UNIT_TESTS=OFF", "-DBOX2D_SAMPLES=OFF", "-DCMAKE_BUILD_TYPE=Release", "..");
		if (!nob_cmd_run(&cmd, .pwd = BOX2D_PATH"build")) exit(1);
		nob_cmd_append(&cmd, "cmake", "--build", ".");
		if (!nob_cmd_run(&cmd, .pwd = BOX2D_PATH"build")) exit(1);
		if (!nob_copy_file(BOX2D_PATH"build/src/libbox2d.a", "./build/libbox2d.a")) {
			//nob_log(NOB_ERROR, "Coulndt "):
		}
// 
		nob_cmd_append(&cmd, "rm", "-rf", BOX2D_PATH"build");
		if (!nob_cmd_run(&cmd)) exit(1);
	#endif
}


b32 strcmp_many(const cstr *str, const cstr **str_many, i32 str_many_count)
{
	for (i32 i = 0; i < str_many_count; i++) {
		if (!strcmp(str, str_many[i])) return (true);
	}
	return (false);
}

internal inline i32 get_stat_time(cstr *path) 
{
	#ifdef _WIN32
		WIN32_FILE_ATTRIBUTE_DATA attr;
		GetFileAttributesExA(path, GetFileExInfoStandard, &attr);
		ULARGE_INTEGER ull;
		ull.LowPart = attr.ftLastWriteTime.dwLowDateTime;
		ull.HighPart = attr.ftLastWriteTime.dwHighDateTime;
		return (ull.QuadPart);
	#else
		struct stat statbuf = {0};
		if (stat(path, &statbuf) < 0) {
			// NOTE: if output does not exist it 100% must be rebuilt
			//if (errno == ENOENT) return 1;
			nob_log(NOB_ERROR, "could not stat %s: %s\n", path, strerror(errno));
			return (0);
		}
		int path_time = statbuf.st_mtime;
		//printf("path_time: %d \n", path_time);
		return (path_time);
	#endif
}
