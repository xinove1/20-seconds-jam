#ifndef ENTITYS_H_
# define ENTITYS_H_
# include "raylib.h"
# include "core.h"
# include "render.h"

# define iterate_check_entity(e, expected_type)        \
	assert(e);                             \
	if (e->type == EntityEmpty) {continue;} \
	if (e->type != expected_type) { continue; TraceLog(LOG_WARNING, "iterate_check_entity: expected entity of type %s but got type %s. continuing", EntityTypeNames[expected_type], EntityTypeNames[e->type]); continue;}

introspect() typedef enum {
	EntityEmpty,
	EntityStatic,
	EntityTypeCount
} EntityType;

introspect() typedef struct {
	EntityType type;
	Sprite render;
	VEffect veffects[VEffectTypeCount]; // TODO  Rename to render effects
	V2 pos;
	V2 size;
	f32 health;
	f32 health_max;
} GenericEntity;


#endif
