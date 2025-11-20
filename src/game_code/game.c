#include "game.h"
#include "input.h"
#include "clay.h"
 
#ifdef HOT_RELOAD
# ifdef _WIN32
#  define hot __declspec(dllexport)
# else
#  define hot
# endif
#else
# define hot static
#endif

internal b32 update_game(void);
internal b32 update_input(void);
internal b32 update_ui(void);
internal void register_actions();

GameData  *Data = NULL;
global GameLevel *Level= NULL;

// NOTE  for web build
// TODO  Add option on menu to not pause game on leaving canvas focus
void pause_game(void) 
{
	Data->paused = true;
}

hot GameConfig init_pre_raylib(void **data)
{
	Data = calloc(1, sizeof(GameData));
	*data = Data;

	*Data = (GameData) {
		.canvas_size = (V2) {640, 360},
		//.canvas_size = (V2) {1280, 720},
		.particles = {0},
		.paused = false,
		.level = NULL,
		.menu_screen = false,
		.input_data = InitXIInputData()
	};

	SetXIInputData(&Data->input_data);

	return ((GameConfig) {
		.canvas_size = Data->canvas_size,
		.window_name = "20SecondsJam",
		.window_flags = FLAG_WINDOW_RESIZABLE,
		.target_fps = 60,
	});
}

hot void init_pos_raylib(void) 
{
	AssertAlways(Data);
	Image image = LoadImage("assets/monogram-bitmap.png");
	Data->assets.fonts[0] = LoadFontFromImageSheet(image, Vec2(6, 12), 32);
	UnloadImage(image);

	Data->ui.clay_mem = (Clay_Arena){0};
	ClayRendererInit(&Data->ui.clay_mem, Data->assets.fonts);

	Data->level = calloc(1, sizeof(GameLevel));
	Level = Data->level;
	register_actions();
	init_editor(Data);
}

hot void pre_reload(void)
{

}

hot void pos_reload(void *data)
{
	Data = data;
	Level = Data->level;
	SetXIInputData(&Data->input_data);
	pos_reload_editor(Data);

	ClayRendererInit(&Data->ui.clay_mem, Data->assets.fonts);
	// size offset = offset_of(GameLevel, entitys);
	// printf("offset: %ld \n", offset);
	// printf("sizeof entity: %ld \n", sizeof(Entity));
}


hot b32 update(void)
{
	assert(Data && Level);
	PoolActions();

	update_editor();
	update_ui();

	if (Data->menu_screen || Data->lost) {
		return (false);
	}

	update_input();

	if (Data->paused) return (false);

	if (update_game()) return (true);

	return (false);
}

internal b32 update_ui(void) 
{
	Clay_BeginLayout();
        //CLAY(CLAY_ID("OuterContainer"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = {250,250,255,255} }) {
	CLAY(CLAY_ID("parent"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(8) }, .backgroundColor = {100, 0, 0, 255 }}) {
		// Child element 1
		//
		CLAY_TEXT(CLAY_STRING("Hello World "), CLAY_TEXT_CONFIG({ .fontSize = 20, .textColor = {0, 255, 255, 255}}));
		// Child element 2 with red background
		// CLAY((CLAY_ID("child"), { .backgroundColor = COLOR_RED }) {
		// 	// etc
		// }
	}
	Data->ui.clay = Clay_EndLayout();
	return (false);
}

internal b32 update_input(void) 
{
	V2 input_dir = {0, 0};
	if (IsActionPressed(RIGHT)) {
		input_dir.x += 1;
	}
	if (IsActionPressed(LEFT)) {
		input_dir.x -= 1;
	}
	if (IsActionPressed(DOWN)) {
		input_dir.y += 1;
	}
	if (IsActionPressed(UP)) {
		input_dir.y -= 1;
	}

	return (false);
}

internal b32 update_game(void) 
{

	// ----------- Particles -----------
	
	//apply_func_entitys(Level, update_entity_veffects);


	// ----------- Update Entitys ----------- 
	
	//apply_func_entitys(Level, update_entity_animations);


	// ----------- Enemys ----------- 
	// {da_iterate(Level->enemys, EnemyDa) {
	// 	Enemy *e = iterate_get();
	// 	iterate_check_entity(e, EntityEnemy);
	// 	update_enemy(Level, e);
	// }}

	return (false);
}

hot void draw(void)
{
	//DrawRectangle(0, 0, Data->canvas_size.x, Data->canvas_size.y, BLACK);
	ClearBackground((Color){130,200,229, 255});

	//apply_func_entitys(Level, render_entity);

	//render_env_sprites(Level->enviromnent_sprites, length_of(Level->enviromnent_sprites));

	// for (i32 i = 0; i < count_of(Data->particles); i++) {
	// 	if (Data->particles[i].type == ParticleEmpty) continue;
	// 	Particle *p = &Data->particles[i];
	// 	p->pos = V2Add(p->pos, V2Scale(p->dir, p->velocity * GetFrameTime()));
	// 	p->duration_count += GetFrameTime();
	// 	if (p->duration_count >= p->duration) {
	// 		*p = (Particle) { 0 };
	// 		continue;
	// 	}
	// 	render_particle(Data->particles[i]);
	// }

	// -------------

	// ---- Ui -----

	ClayRendererRender(Data->ui.clay, Data->assets.fonts);
	draw_editor();
}

GameFunctions game_init_functions()
{
	return (GameFunctions) {
		.init_pre_raylib = &init_pre_raylib,
		.init_pos_raylib = &init_pos_raylib,
		.update = &update,
		.draw = &draw,
		.pre_reload = &pre_reload,
		.pos_reload = &pos_reload,
	};
}

internal void register_actions()
{
	SetGamePadId(0);

	RegisterActionName(RIGHT, "right");
	RegisterInputKeyAction(RIGHT, KEY_D);
	RegisterInputKeyAction(RIGHT, KEY_RIGHT);
	RegisterGamePadButtonAction(RIGHT, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
	RegisterGamePadAxisAction(RIGHT, GAMEPAD_AXIS_LEFT_X, 0.5f);


	RegisterActionName(LEFT, "left");
	RegisterInputKeyAction(LEFT, KEY_A);
	RegisterInputKeyAction(LEFT, KEY_LEFT);
	RegisterGamePadButtonAction(LEFT, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
	RegisterGamePadAxisAction(LEFT, GAMEPAD_AXIS_LEFT_X, -0.5f);


	RegisterActionName(UP, "up");
	RegisterInputKeyAction(UP, KEY_W);
	RegisterInputKeyAction(UP, KEY_UP);
	RegisterGamePadButtonAction(UP, GAMEPAD_BUTTON_LEFT_FACE_UP);
	RegisterGamePadAxisAction(UP, GAMEPAD_AXIS_LEFT_Y, -0.5f);


	RegisterActionName(DOWN, "down");
	RegisterInputKeyAction(DOWN, KEY_S);
	RegisterInputKeyAction(DOWN, KEY_DOWN);
	RegisterGamePadButtonAction(DOWN, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
	RegisterGamePadAxisAction(DOWN, GAMEPAD_AXIS_LEFT_Y, 0.5f);


	RegisterActionName(ACTION_1, "action_1");
	RegisterInputKeyAction(ACTION_1, KEY_J);
	RegisterInputKeyAction(ACTION_1, KEY_X);
	RegisterGamePadButtonAction(ACTION_1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
	RegisterGamePadAxisAction(ACTION_1, GAMEPAD_AXIS_RIGHT_TRIGGER, 0.7f);


	RegisterActionName(ACTION_2, "action_2");
	RegisterInputKeyAction(ACTION_2, KEY_K);
	RegisterInputKeyAction(ACTION_2, KEY_Z);
	RegisterGamePadButtonAction(ACTION_2, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);


	RegisterActionName(ACTION_3, "action_3");
	RegisterInputKeyAction(ACTION_3, KEY_SPACE);
	RegisterGamePadButtonAction(ACTION_3, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);


	RegisterActionName(OPEN_MENU, "open_menu");
	RegisterInputKeyAction(OPEN_MENU, KEY_ESCAPE);
	RegisterInputKeyAction(OPEN_MENU, KEY_E);
	RegisterGamePadButtonAction(OPEN_MENU, GAMEPAD_BUTTON_MIDDLE_RIGHT);
}
