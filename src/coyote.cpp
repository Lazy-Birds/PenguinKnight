void coyote_action(Entity *coyote, Game_Input *input) {
	draw_entity(coyote, 0);
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
		move_entity(coyote, input->dt);
		draw_entity(coyote, 0);

	} break;
	case SHOOT:
	    {
	        
	} break;
	case HIT:
	{
	    if (coyote->state_time*60 <= 6) {
	        draw_entity(coyote, 0);	        
	    }

	        coyote->enemy.sleep_time=20*input->dt;

	        coyote->state = NEUTRAL;
	} break;
	    case DYING:
	    {
	        emit_death_particles(coyote, input->dt);

	        coyote->state = DEAD;

	            /*if (i32(coyote->state_time*60) < 2) {
	                draw_entity(coyote, 4);
	            } else if (i32(coyote->state_time*60) < 4) {
	                draw_entity(coyote, 5);
	            } else if (i32(coyote->state_time*60) < 6) {
	                draw_entity(coyote, 6);
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

void marmoset_action(Entity *marm) {
	marm->velocity.y+=480*input->dt;

	marm->state_time+=input->dt;

	if (marm->alive) draw_normal_enemy_health(marm);

	if (player.position.x < marm->position.x) {
	    marm->facing = -1;
	} else {
	    marm->facing = 1;
	}

	if (marm->state_prev != marm->state) {
	    marm->state_prev = marm->state;
	    marm->state_time = 0;
	}

	switch (marm->state)
	{
	case NEUTRAL:
		{
			draw_entity(marm, 0);

			marm->velocity = v2_zero;

			if (marm->state_time*60 > 10) marm->state = SHOOT;
		} break;
	case SHOOT:
		{
			if (marm->state_time*60 < 20) {
				draw_entity(marm, 0);
			} else if (marm->state_time*60 < 45) {
				draw_entity(marm, 1);
			} else if (marm->state_time*60 < 85) {
				draw_entity(marm, 2);
			} else if (marm->state_time*60 <= 86) {
				draw_entity(marm, 2);
				make_projectile(marm->projectile, v2(marm->position.x+52, marm->position.y+42), v2(12000*input->dt*sign_f32(marm->facing), 0), 0, 0);
			} else {
				marm->state = NEUTRAL;
				draw_entity(marm, 0);
			}
		} break;
	case HIT:
		{
			if (marm->state_time*60 < 2) {
				draw_entity(marm, 0);
				marm->velocity.x-=8000*input->dt*sign_f32(marm->facing);
				marm->velocity.y-=6000*input->dt;
			} else if (entity_in_air(marm)) {
				draw_entity(marm, 0);
			} else {
				draw_entity(marm, 0);
				marm->state = NEUTRAL;
			}
		} break;
	case DYING:
		{
			emit_death_particles(marm, input->dt);

	        marm->state = DEAD;
		} break;
	case DEAD:
		{
			marm->alive = false;
		} break;
	}

	move_entity(marm, input->dt);

	if (marm->facing > 0) {
		marm->hitbox = r2_bounds(marm->position, marm->size, v2_zero, v2_one);
	} else {
		marm->hitbox = r2_bounds(v2(marm->position.x + 24, marm->position.y), marm->size, v2_zero, v2_one);
	}
}

void hedgehog_actions(Entity *hedge) {
	hedge->velocity.y+=480*input->dt;

	hedge->state_time+=input->dt;

	if (hedge->alive) draw_normal_enemy_health(hedge);

	if (player.position.x < hedge->position.x) {
	    if (entity_get_distance_x(&player, hedge) > 50) hedge->facing = -1;
	} else {
	    if (entity_get_distance_x(&player, hedge) > 50) hedge->facing = 1;
	}

	if (hedge->state_prev != hedge->state) {
	    hedge->state_prev = hedge->state;
	    hedge->state_time = 0;
	} 

	switch (hedge->state)
	{
	case NEUTRAL:
		{
			draw_entity(hedge, 0);

			if (entity_get_distance_x(&player, hedge) < 700) hedge->state = ATTACK;
		} break;
	case ATTACK:
		{
			if (hedge->state_time*fps < 10) {
				hedge->sprite_index = 0;
			} else if (hedge->state_time*fps < 20) {
				hedge->sprite_index = 1;
			} else if (hedge->state_time*fps < 30) {
				hedge->sprite_index = 2;
				hedge->velocity.x+=400*input->dt*sign_f32(hedge->facing);
				
			} else {
				if (i32(hedge->state_time*30)%10 == 0) hedge->sprite_index++;
				if (hedge->sprite_index < 3 || hedge->sprite_index > 7) hedge->sprite_index = 3;
				
				hedge->velocity.x+=600*input->dt*sign_f32(hedge->facing);
			}

		draw_entity(hedge, hedge->sprite_index);
		} break;
	case HIT:
		{
			if (hedge->state_time*fps <= 1) hedge->velocity = v2_zero;

			if (hedge->state_time*fps < 2) {
				draw_entity(hedge, 0);
				hedge->velocity.x-=8000*input->dt*sign_f32(hedge->facing);
				hedge->velocity.y-=6000*input->dt;
			} else if (entity_in_air(hedge)) {
				draw_entity(hedge, hedge->sprite_index);
			} else {
				draw_entity(hedge, hedge->sprite_index);
				hedge->state = NEUTRAL;
			}
		} break;
	case DYING:
		{
			emit_death_particles(hedge, input->dt);

	        hedge->state = DEAD;
		} break;
	case DEAD:
		{
			hedge->alive = false;
		} break;
	}

	if (invuln_time <= 0){
        if (r2_intersects(r2_bounds(player.position, player.size, v2_zero, v2_one), r2_bounds(hedge->position, hedge->size, v2_zero, v2_one)))
        {
            player_hit(hedge, input);
        }   
    }

	move_entity(hedge, input->dt);

	//Hitbox
	hedge->hitbox = r2_bounds(v2(hedge->position.x+12, hedge->position.y), hedge->size, v2_zero, v2_one);

	Rectangle2 dRect = r2_shift(hedge->hitbox, v2(camera_offset, 0));

	DrawRectOutline(dRect, v4_red, 2);
}