#include "game.h"

global GameEditor *Ed = NULL;

internal void edit_entity(GenericEntity *e);
internal void is_entity_under_mouse(GenericEntity *e);

void init_editor(GameData *data)
{
	TraceLog(LOG_INFO, "Initting Editor");
	Data = data;
	Ed = &data->editor;
}

void pos_reload_editor(GameData *data) 
{
	Ed = &data->editor;
}

void update_editor()
{
	V2 mouse_pos = GetMousePosition();

	if (IsKeyPressed(KEY_D)) {
		Ed->debug_window = Ed->debug_window ? false : true;
	}

	// ----- Ui -----

	// ----- Editor ----- 
	if (Ed->debug_select) {
		Ed->hovered = NULL;
		apply_func_entitys(Data->level, is_entity_under_mouse);

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (Ed->hovered) {
				Ed->selected = Ed->hovered;
				Ed->hovered = NULL;

			} else Ed->selected = NULL;
		}
	} 

}

void draw_editor()
{
	Color selected_highlight = BLACK;
	Color hover_highlight = PURPLE;
	if (Ed->selected) {
		DrawRectangleLinesEx(RecV2(Ed->selected->pos, Ed->selected->size), 1, selected_highlight);
	}
	if (Ed->hovered) {
		DrawRectangleLinesEx(RecV2(Ed->hovered->pos, Ed->hovered->size), 1, hover_highlight);
	}

}

// internal void edit_entity(GenericEntity *e) 
// {

// 	//mu_label(ctx, EntityTypeNames[e->type]);
// 	i32 body_width = mu_get_current_container(ctx)->body.w;
// 	i32 width = body_width * 0.30;
// 	i32 label_w = 50;

// 	mu_layout_row(ctx, 2, (int[]) {label_w, -1}, 0);
// 	if (mu_header(ctx, "Effects")) {
// 		mu_layout_row(ctx, 2, (int[]) {label_w, -1}, 0);
// 		mu_label(ctx, "Duration");
// 		mu_slider(ctx, &Ed->effect_duration, 0.01, 0.99);
// 		i32 width_ = body_width * 0.15;
// 		mu_layout_row(ctx, 6, (int[]) {label_w, width_, width_, width_, width_, -1}, 0);
// 		mu_label(ctx, "Color");
// 		u8_slider(ctx, &Ed->effect_color.r, 0, 255);
// 		u8_slider(ctx, &Ed->effect_color.g, 0, 255);
// 		u8_slider(ctx, &Ed->effect_color.b, 0, 255);
// 		u8_slider(ctx, &Ed->effect_color.a, 0, 255);
// 		mu_draw_rect(ctx, mu_layout_next(ctx), ColorToMu(Ed->effect_color));
// 	}
// 	if (mu_button(ctx, "apply")) {
// 		apply_flash_effect(e, Ed->effect_color, Ed->effect_duration);
// 		//apply_shake_effect(entity, 0.25f);
// 	}


// 	if (e->type == EntityEnemy || e->type == EntityTurret) {
// 		mu_layout_row(ctx, 2, (int[]) {label_w, -1}, 0);
// 		mu_label(ctx, "Floor");
// 		mu_label(ctx, TextFormat("%d", ((Enemy*) e)->floor));
// 	}

// 	mu_layout_row(ctx, 3, (int[]) {label_w, width, -1}, 0);
// 	mu_label(ctx, "Position");
// 	mu_number_ex(ctx, &e->pos.x, 3, "%.1f", MU_OPT_ALIGNCENTER);
// 	mu_number_ex(ctx, &e->pos.y, 3, "%.1f", MU_OPT_ALIGNCENTER);
// 	mu_label(ctx, "Size");
// 	mu_number_ex(ctx, &e->size.x, 3, "%.1f", MU_OPT_ALIGNCENTER);
// 	mu_number_ex(ctx, &e->size.y, 3, "%.1f", MU_OPT_ALIGNCENTER);

// 	mu_layout_row(ctx, 2, (int[]) {label_w, -1}, 0);
// 	mu_label(ctx, "Health");
// 	mu_number_ex(ctx, &e->health, 3, "%.1f", MU_OPT_ALIGNCENTER);
// 	mu_label(ctx, "Health Max");
// 	mu_number_ex(ctx, &e->health_max, 3, "%.1f", MU_OPT_ALIGNCENTER);

// 	width = body_width * 0.14;
// 	mu_layout_row(ctx, 6, (int[]) {label_w, width, width, width, width, -1}, 0);
// 	mu_label(ctx, "Color");
// 	u8_slider(ctx, &e->render.tint.r, 0, 255);
// 	u8_slider(ctx, &e->render.tint.g, 0, 255);
// 	u8_slider(ctx, &e->render.tint.b, 0, 255);
// 	u8_slider(ctx, &e->render.tint.a, 0, 255);
// 	mu_draw_rect(ctx, mu_layout_next(ctx), ColorToMu(e->render.tint));
// }

internal void is_entity_under_mouse(GenericEntity *e) 
{
	V2 mouse_pos = GetMousePosition();
	Rect e_rec = RecV2(e->pos, e->size);
	if (CheckCollisionPointRec(mouse_pos, e_rec)) {
		Ed->hovered = e;
		return ;
	}
}
