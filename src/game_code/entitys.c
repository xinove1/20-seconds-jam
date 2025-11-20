#include "game.h"

void damage_entity(GameLevel *rt, GenericEntity *entity, f32 damage)
{
	assert(entity && rt);
	
	switch (entity->type) {
		case EntityEmpty: {
			TraceLog(LOG_WARNING, "damage_entity: can't damage %s", EntityTypeNames[entity->type]);
		} break;

		// case EntityEnemy: {
		// 	apply_flash_effect(entity, BLACK, 0.25f);
		// 	apply_shake_effect(entity, 0.25f);
		// 	entity->health -= damage;
		// } break ;

		default: {
			TraceLog(LOG_WARNING, "damage_entity: damaging %s not implemented yet.", EntityTypeNames[entity->type]);
		} break;
	}
}

void apply_func_entitys(GameLevel *l, void (*func)(GenericEntity *entity)) 
{
	TraceLog(LOG_WARNING, "apply_func_entitys not implemented");
}

b32 entity_in_range(GenericEntity *from, GenericEntity *to, f32 range) 
{
	//Rect from_rec = RecV2(V2Subtract(from->pos, Vec2v(range)), V2Add(from->size, Vec2v(range)));
	Rect from_rec = RecGrow(RecV2(from->pos, from->size), range);
	Rect to_rec = RecV2(to->pos, to->size);
	return (CheckCollisionRecs(from_rec, to_rec));
}
