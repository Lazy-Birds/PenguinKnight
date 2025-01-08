void coyote_action(Entity *coyote, Game_Input *input) {
	draw_enemy(coyote, 0);
	  coyote->state_time+=input->dt;

	if (coyote->enemy.sleep_time > 0) {
	    
	}

	if (player.position.x < coyote->position.x) {
	    coyote->facing = -1;
	} else {
	    coyote->facing = 1;
	}

	if (coyote->state_prev != coyote->state) {
	    coyote->state_prev = coyote->state;
	    coyote->state_time = 0;
	}

	switch (coyote->state)
	{
	case NEUTRAL:
	{
		coyote->velocity.y = 3000;
		move_enemy(coyote, input->dt);
		draw_enemy(coyote, 0);

	} break;
	case SHOOT:
	    {
	        
	} break;
	case HIT:
	{
	    if (coyote->state_time*60 <= 6) {
	        draw_enemy(coyote, 0);	        
	    }

	        coyote->enemy.sleep_time=20*input->dt;

	        coyote->state = NEUTRAL;
	} break;
	    case DYING:
	    {
	        Particle_Parameters min = {};
	        Particle_Parameters max = {};

	        min.velocity.x = 400*input->dt;
	        min.velocity.y = 400*input->dt;

	        max.velocity.x = 400*input->dt;
	        max.velocity.y = 400*input->dt;

	        min.position.x = coyote->position.x;
	        min.position.y = coyote->position.y;

	        max.position.x = coyote->position.x + coyote->size.x;
	        max.position.y = coyote->position.y+coyote->size.y;

	        min.life_time = 120*input->dt;
	        max.life_time = 90*input->dt;

	        min.magnet = &player;

	        Image image = LoadImage(S("exp.png"));

	        for (int i = 0; i < i32(coyote->enemy.exp_dropped/10); i++) {
	            particle_emit(min, max, image);
	        }

	        coyote->state = DEAD;

	            /*if (i32(coyote->state_time*60) < 2) {
	                draw_enemy(coyote, 4);
	            } else if (i32(coyote->state_time*60) < 4) {
	                draw_enemy(coyote, 5);
	            } else if (i32(coyote->state_time*60) < 6) {
	                draw_enemy(coyote, 6);
	            } else {
	                coyote->alive = false;
	            }*/
	    } break;
	    case DEAD:
	        {
	            coyote->alive = false;
	        } break;
	}

	    if (invuln_time <= 0){
	        if (r2_intersects(r2_bounds(player.position, player.size, v2_zero, v2_one), r2_bounds(coyote->position, coyote->size, v2_zero, v2_one)))
	        {
	            player_hit(coyote, input);
	        }   
	    }
}