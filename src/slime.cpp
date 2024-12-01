void slime_action(Entity *slime, Entity *player, Game_Input *input) {
	 slime->state_time+=input->dt;

    if (slime->enemy.sleep_time > 0) {
        move_enemy(slime, input->dt);
        slime->enemy.sleep_time-=input->dt;
        if (slime->state_time*60 <= 20) {
            draw_enemy(slime, 0);
            if (slime->state_time*60 >= 10) slime->velocity.x = 0;
        } else if (slime->state_time*60 <= 40) {
            draw_enemy(slime, 1);
        } else {
        	draw_enemy(slime, 2);
        }
        
        if (slime->enemy.sleep_time*60 <= 1) slime->state_time = 0;

        return;
    }

    if (player->position.x < slime->position.x) {
        slime->facing = -1;
    } else {
        slime->facing = 1;
    }

    if (slime->state_prev != slime->state) {
        slime->state_prev = slime->state;
        slime->state_time = 0;
    }

    switch (slime->state)
    {
    case NEUTRAL:
    	{
    		if (abs_i32(slime->position.x - player->position.x) < 800)
    		{
    			slime->state = MOVE;
    			draw_enemy(slime, 0);
    		} else if (slime->state_time*60 < 20) 
    		{
    			draw_enemy(slime, 0);
    		} else if (slime->state_time*60 < 40) 
    		{
    			draw_enemy(slime, 1);
    		} else 
    		{
    			draw_enemy(slime, 2);
    		}
    	} break;
    case MOVE:
    	{
    		if (slime->state_time*60 < 30)
    		{
    			draw_enemy(slime, 0);
    		} else if (slime->state_time*60 < 60)
    		{
    			draw_enemy(slime, 1);
    		} else if (slime->state_time*60 < 62)
    		{
                if (slime->velocity_prev.x == 0) {
                    slime->velocity_prev.x = random_i32_between(1500, 2500)*input->dt*sign_f32(player->position.x-slime->position.x);
                    slime->velocity_prev.y = random_i32_between(6000, 10000)*input->dt;

                }
    			slime->velocity.x+=slime->velocity_prev.x;
    			slime->velocity.y-=slime->velocity_prev.y;
    			draw_enemy(slime, 2);
    		} else if (slime->velocity.y < 0)
    		{
    			draw_enemy(slime, 2);
    		} else if (!entity_on_wall(slime) && !enemy_on_enemy(slime))
    		{
    			draw_enemy(slime, 1);
    		} else 
    		{
    			draw_enemy(slime, 0);
    			slime->enemy.sleep_time+=60*input->dt;
                slime->velocity.x = 0;
                slime->state_time = 0;

                slime->velocity_prev = v2_zero;

                if (random_f32_between(0, 1) < .25) {
                    slime->state = SHOOT;
                } else {
                    slime->state = MOVE;
                }
    		}
    	} break;
    case SHOOT:
        {
            if (slime->state_time*60 < 20) {
                slime->projectile_launched = false;
                draw_enemy(slime, 4);
            } else if (slime->state_time*60 < 40) {
                Image dart = LoadImage(S("slime_dart.png"));
                draw_enemy(slime, 5);
                DrawImage(dart, v2(slime->position.x-camera_pos.x+out->width*.5, slime->position.y + 17));
            } else if (slime->state_time*60 < 60) {
                if (!slime->projectile_launched) {
                    make_projectile(slime->projectile, v2(slime->position.x, slime->position.y+17), v2(8000*input->dt*sign_f32(player->position.x-slime->position.x), 0));
                    slime->projectile_launched = true;
                }

                draw_enemy(slime, 4);
            } else {
                draw_enemy(slime, 0);
                slime->enemy.sleep_time+=60*input->dt;
                slime->velocity.x = 0;
                slime->state_time = 0;
                slime->projectile_launched = false;

                if (random_f32_between(0, 1) < .25) {
                    slime->state = SHOOT;
                } else {
                    slime->state = MOVE;
                }
            }
        } break;
    case HIT:
        {
            if (slime->state_time*60 <= 6) {
                draw_enemy(slime, 4);
                if (slime->position.x > player->position.x)
            	{
                	slime->velocity.x = 8000*input->dt;
                	slime->velocity.y -= 6000*input->dt;
            	} else
            	{
                	slime->velocity.x = -8000*input->dt;
                	slime->velocity.y -= 6000*input->dt;
            	}
            }

            slime->enemy.sleep_time=20*input->dt;

            slime->state = MOVE;
        } break;
    case DYING:
        {
            Particle_Parameters min = {};
            Particle_Parameters max = {};

            min.velocity.x = 400*input->dt;
            min.velocity.y = 400*input->dt;

            max.velocity.x = 400*input->dt;
            max.velocity.y = 400*input->dt;

            min.position.x = slime->position.x;
            min.position.y = slime->position.y;

            max.position.x = slime->position.x + slime->size.x;
            max.position.y = slime->position.y+slime->size.y;

            min.life_time = 120*input->dt;
            max.life_time = 90*input->dt;

            min.magnet = player;

            Image image = LoadImage(S("exp.png"));

            for (int i = 0; i < i32(slime->enemy.exp_dropped/10); i++) {
                particle_emit(min, max, image);
            }

            slime->state = DEAD;

            /*if (i32(slime->state_time*60) < 2) {
                draw_enemy(slime, 4);
            } else if (i32(slime->state_time*60) < 4) {
                draw_enemy(slime, 5);
            } else if (i32(slime->state_time*60) < 6) {
                draw_enemy(slime, 6);
            } else {
                slime->alive = false;
            }*/
        } break;
    case DEAD:
        {
            slime->alive = false;
        } break;
    }
    
    if (invuln_time <= 0){
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(slime->position, slime->size, v2_zero, v2_one)))
        {
            player_hit(slime, input);
        }
    }

    move_enemy(slime, input->dt);
}