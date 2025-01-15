void robo_pup_action(Entity *pup, Game_Input *input) {
	pup->state_time+=input->dt;

    if (pup->state_prev != pup->state) {
        pup->state_prev = pup->state;
        pup->state_time = 0;
    }

    if (pup->velocity.y < 0) {
    	pup->velocity.y+=400*input->dt;
    }

    if (pup->state != DEAD && entity_in_spike_trap(pup)) {
    	pup->state = DYING;
    }

    if (pup->alive) {
    	DrawRect(r2_bounds(v2(pup->position.x-camera_pos.x+out->width*.5-2, pup->position.y-2-12), v2(pup->size.x+4-pup->enemy.offset.x, 12),
            v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(pup->position.x-camera_pos.x+out->width*.5, pup->position.y-12), v2(pup->current_health/pup->max_health*(pup->size.x+4-pup->enemy.offset.x),
            8), v2_zero, v2_one), v4_red);
    }

    if (pup->facing > 0 && (ledge_ahead(pup) || wall_ahead(pup)) && !entity_in_air(pup))
    {
    	pup->facing = -1;
    } else if (pup->facing < 0 && (ledge_ahead(pup) || wall_ahead(pup)) && !entity_in_air(pup)) {
    	pup->facing = 1;
    }

    switch (pup->state)
    {
    case NEUTRAL:
    	{
    		pup->state = MOVE;
    		draw_enemy(pup, pup->sprite_index);
    	} break;
    case MOVE:
    	{
    		if (i32(pup->state_time*60)%10 == 0)
    		{
    			pup->sprite_index++;
    			if (pup->sprite_index > 8) pup->sprite_index = 0;
    		}
    		
    		pup->velocity.x = 4000*input->dt*sign_f32(pup->facing);
    		pup->velocity.y = 200;

    		move_enemy(pup, input->dt);

    		draw_enemy(pup, pup->sprite_index);
    	} break;
    case HIT:
    	{
    		if (i32(pup->state_time*60)%10 == 0)
    		{
    			pup->sprite_index++;
    			if (pup->sprite_index > 8) pup->sprite_index = 0;
    		}

    		if (pup->state_time*60 < 1) {
    			pup->velocity.x = 16000*input->dt*-get_entity_direction(pup);
    			pup->velocity.y = -8000*input->dt;
    			move_enemy(pup, input->dt);
    			draw_enemy(pup, pup->sprite_index);
    		} else if (!entity_on_wall(pup)) {
    			move_enemy(pup, input->dt);
    			draw_enemy(pup, pup->sprite_index);
    		} else if (entity_on_wall(pup)) {
    			draw_enemy(pup, pup->sprite_index);
    			pup->state = NEUTRAL;
    		}
   	 	} break;
   	 	case DYING:
    {
        Particle_Parameters min = {};
        Particle_Parameters max = {};

        min.velocity.x = 400*input->dt;
        min.velocity.y = 400*input->dt;

        max.velocity.x = 400*input->dt;
        max.velocity.y = 400*input->dt;

        min.position.x = pup->position.x;
        min.position.y = pup->position.y;

        max.position.x = pup->position.x + pup->size.x;
        max.position.y = pup->position.y+pup->size.y;

        min.life_time = 120*input->dt;
        max.life_time = 90*input->dt;

        min.magnet = &player;

        Image image = LoadImage(S("exp.png"));

        for (int i = 0; i < i32(pup->enemy.exp_dropped/10); i++) {
            particle_emit(min, max, image);
        }

        pup->state = DEAD;
    } break;
    case DEAD:
        {
            pup->alive = false;
        } break;
    }

    if (invuln_time <= 0) {
        if (r2_intersects(pup->hitbox, get_entity_rect(&player)))
    	{
        	player_hit(pup, input);
    	}
    }
}