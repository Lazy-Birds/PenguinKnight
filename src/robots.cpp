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
    	draw_normal_enemy_health(pup);
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
        emit_death_particles(pup, input->dt);

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