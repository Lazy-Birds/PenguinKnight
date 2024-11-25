f32 sleep = 0;
i32 state_change = 0;

f32 state_time = 0;

void set_enemy_vuln();

void player_action(Game_Input *input) {
	f32 max_speed = 600.0;
	i32 weapon_cooldown = 2;
	Controller c0 = input->controllers[0];
	f32 dt = input->dt;
	i32 offset = camera_pos.x-player.position.x;
	

	state_time+=dt;
	if (player.mp_cooldown > 0) {
		player.mp_cooldown-=dt;
	}

	if (player.shield_hit > 0) {
		player.shield_hit-=dt;
	}
	
	if (invuln_time > 0){
		invuln_time-=dt;
	}
	

	//Edge cases
	if (player.state != state_change) {
		state_change = player.state;
		state_time = 0;
	}

	//Gear Change
	if (player.state == NEUTRAL) {
		if (!player.alive) {
			player.state = DEAD;
			state_time = 0;
		}
		if (c0.right || c0.left) {
			player.state = MOVE;
			state_time = 0;
		} else if (c0.up) {
			player.state = JUMP;
			state_time = 0;

		} else if (input->mouse.left && entity_in_air(&player)) {
			player.state = STATEJUMPATTACK;
			state_time = 0;
		}
		else if (input->mouse.left) {
			player.state = ATTACK;
			state_time = 0;
		} else if (input->mouse.right) {
			player.state = CHARGING;
			state_time = 0;
		} else if (c0.b) {
			player.state = STATEDASH;
			state_time = 0;
		} else if (c0.a) {
			player.state = GUARD;
			state_time = 0;
		}
		state_change = player.state;
	}

	
	if (!(sleep<=0)) {
		sleep--;
		if (player.state == ATTACK)
		{
			draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
				player.weapon.weapon_frames.y-1, player.facing);
		} else if (player.state == STATECHARGEATTACKING)
		{
			draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
				player.weapon.charged_frames.y-1, player.facing);
		}
		return;
	}
	switch (player.state) {
	case NEUTRAL: 
		{
			player.facing = sign_i32(player.facing);
			player.velocity.x = move_f32(player.velocity.x, 0, 3000 * dt);
			draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
		} break;
	case MOVE:
		{
			
			if (c0.right)
			{
				
				player.velocity.x = move_f32(player.velocity.x, 350, 350 * dt);
				

				if (player.facing < 0) {
					player.facing = 1;
				} else {
					player.facing++;
				}

				if (i32(state_time*60)%30 < 15) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						0, player.facing);	
				} else {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						1, player.facing);
				}
				
			}
			else if (c0.left)
			{
				
				player.velocity.x = move_f32(player.velocity.x, -350, 350 * dt);
				
				if (player.facing > 0) {
					player.facing = -1;
				} else {
					player.facing--;
				}

				if (i32(state_time*60)%30 < 15) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						0, player.facing);	
				} else {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						1, player.facing);
				}
			}

			if (input->mouse.left && entity_in_air(&player)) {
				state_time = 0;
				player.state = STATEJUMPATTACK;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (input->mouse.left) {
				state_time = 0;
				player.state = ATTACK;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (input->mouse.right) {
				state_time = 0;
				player.state = CHARGING;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (c0.b) {
				state_time = 0;
				player.state = STATEDASH;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (c0.up) {
				player.state = JUMP;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (!c0.left && !c0.right) {

				player.state = NEUTRAL;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			} else if (c0.a) {
				player.state = GUARD;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
			}
			

		} break;
	case JUMP:
		{
			if (c0.up && player.current_stamina > 20) {
				if (c0.right)
				{
					player.velocity.x = move_f32(player.velocity.x, 250, 250 * dt);

					if (player.facing < 0)
					{
						player.facing = 1;
					} else
					{
						player.facing++;
					}
				} else if (c0.left)
				{
					player.velocity.x = move_f32(player.velocity.x, -250, 250 * dt);
					
					if (player.facing > 0)
					{
						player.facing = -1;
					} else
					{
						player.facing--;
					}
				}
				if (state_time*60 < 15) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.jump.x, player.facing);
				} else if (state_time*60 > 14 && entity_on_wall(&player)) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.jump.y, player.facing);
					player.velocity.y=-240;
					player.current_stamina-=20;
				} else if (player.velocity.y < 0) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.jump.y, player.facing);
				} else {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						0, player.facing);
					player.state = NEUTRAL;
				}
			} else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					0, player.facing);
				player.state = NEUTRAL;
			}
		} break;
	case ATTACK: 
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (player.current_stamina > 20 && state_time == 0)
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					state_time*12+player.weapon.weapon_frames.x, player.facing);
				player.current_stamina -=20;
			} else if (state_time*12 < player.weapon.weapon_frames.y-player.weapon.weapon_frames.x)
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);

				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					state_time*12+player.weapon.weapon_frames.x, player.facing);

			}  else if (state_time*12<player.weapon.weapon_frames.y-1)
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					player.weapon.weapon_frames.y-1, player.facing);
				sleep = weapon_cooldown*4;
			} else
			{
				set_enemy_vuln();
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					player.weapon.weapon_frames.y-1, player.facing);
				player.state = 0;
			}
			
		} break;
	case CHARGING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (input->mouse.right && state_time*9 < player.weapon.charge_time && player.current_stamina > 1)
			{
				//DrawImage(charge_meter[i32(state_time*9)], v2(player.position.x-player.position.x+out->width*.5-16, player.position.y-16));
				draw_charging(i32(state_time*9));
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					player.weapon.charged_frames.x, player.facing);

			} else if (input->mouse.right && state_time*9 > player.weapon.charge_time) 
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					player.weapon.charged_frames.x, player.facing);
				player.current_stamina-=40;
				player.state = STATECHARGEATTACKING;
			} else
			{

				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 0, player.facing);
				player.state = NEUTRAL;
			}
			
		} break;
	case STATECHARGEATTACKING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (state_time*11 + player.weapon.charged_frames.x < player.weapon.charged_frames.y) 
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					state_time*11 + player.weapon.charged_frames.x, player.facing);
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (state_time*11+ player.weapon.charged_frames.x
				> player.weapon.charged_frames.y && state_time*11 + player.weapon.charged_frames.x
				< player.weapon.charged_frames.y + 2) 
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
				sleep += 45;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					player.weapon.charged_frames.y-1, player.facing);
			} else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
					0, player.facing);
				player.state = NEUTRAL;
				set_enemy_vuln();
			}
			
			
		} break;
	case STATEJUMPATTACK: 
		{
			
			if (string_equals(player.weapon.name, S("Cleaver"))) 
			{


				if (state_time == 0 && player.current_stamina > 19) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y),
						player.weapon.jump_frame.x, player.facing);
					player.current_stamina-=20;
					player.velocity.x += 1600*dt;
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (entity_in_air(&player) && state_time > 0) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.jump_frame.x, player.facing);
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (!entity_in_air(&player) && state_time > 0) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.jump_frame.y, player.facing);
					player.state = LANDED;
					state_time = 0;
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						0, player.facing);
					player.state = NEUTRAL;
				}
			} else 
			{
				Dump("oops");
			}

			
		} break;
	case LANDED:
		{
			
			if (state_time*9 < 4) 
			{
				player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.jump_frame.y, player.facing);

			} else 
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.jump_frame.y, player.facing);
				player.state = NEUTRAL;
				player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
				set_enemy_vuln();
			}
			
			
		} break;
	case HIT:
		{
			
			if (state_time == 0)
			{
				player.velocity.y = -6000*dt;
				player.velocity.x = -6000*dt;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.frame_hit, player.facing);

			} else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.frame_hit, player.facing);
				player.state = NEUTRAL;

			} 
			
		} break;
	case EXHAUSTED:
		{

		} break;
	case STATEDASH:
		{
			if (player.current_stamina < 1) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					0, player.facing);
				player.state = NEUTRAL;

			} else if (state_time*60 <= 1) {
				player.velocity.x = 0;
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.dash_frame.x, player.facing);
				player.current_stamina -= 10;
			} else if (state_time*60 < 8) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.dash_frame.x, player.facing);
			} else if (state_time*60 < 12) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.dash_frame.x+1, player.facing);
				invuln_time+=(10*dt);
			} else if (state_time*60 < 15) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.dash_frame.x+2, player.facing);
				player.velocity.x+=600*dt*sign_f32(player.facing);
			} else if (c0.b) {
				if (i32(state_time*60)%20 < 10) {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.dash_frame.x+3, player.facing);
				} else {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
						player.weapon.dash_frame.x+2, player.facing);
				}

				if (player.facing > 0) {
					player.velocity.x += 6000*dt;
					player.velocity.x = clamp_f32(player.velocity.x, 0, 30000*dt);
				} else {
					player.velocity.x += -6000*dt;
					player.velocity.x = clamp_bot_f32(player.velocity.x, -30000*dt);
				}
				
				player.current_stamina-= 30*dt;


				Particle_Parameters min = {};
				Particle_Parameters max = {};

				if (player.facing < 0) {
					min.velocity.x = 500*dt;
					min.velocity.y = -1000*dt;

					max.velocity.x = 1000*dt;
					max.velocity.y = -500*dt;
				} else {
					min.velocity.x = -500*dt;
					min.velocity.y = -1000*dt;

					max.velocity.x = -1000*dt;
					max.velocity.y = -500*dt;
				}

				min.position.x = player.position.x;
				min.position.y = player.position.y + 40;

				max.position.x = player.position.x+30;
				max.position.y = player.position.y+51;

				min.life_time = 30*dt;
				max.life_time = 60*dt;

				Image image = LoadImage(S("snow_flake.png"));

				particle_emit(min, max , image);

				if (c0.up) {
					player.state = JUMP;
				}

			} else if (state_time*60 < 28) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.dash_frame.x+1, player.facing);
				//player.velocity.x-=8000*dt*sign_f32(player.facing);
			} else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					0, player.facing);
				player.state = NEUTRAL;
			}
		} break;
	case GUARD:
		{
			if (state_time*60 < 15) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.guard.x, player.facing);
				player.velocity.x = 0;
			} else if (c0.a && player.current_mp > 0 && player.shield_hit > 0) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.guard.y, player.facing);
				player.current_mp-=30*dt;
			} else if (c0.a && player.current_mp > 0) {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					player.weapon.guard.y-1, player.facing);
				player.current_mp-=30*dt;
			}  else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5-offset, player.position.y), 
					0, player.facing);
				player.state = NEUTRAL;
			}
		} break;
	case DEAD:
		{

		} break;
	}

	player.anchor = v2(player.position.x+15, player.position.y+25);
}

void set_enemy_vuln() {
	for (int i = 0; i < enemy_count; i++) {
		if (enemys[i].alive) {
			enemys[i].invuln = false;

		}
	}
}

void player_hit(Entity *entity, Game_Input *input) {
	if (player.state == GUARD && player.current_mp > 10) {
		player.current_mp-=10;
		invuln_time = 60*input->dt;
		player.shield_hit = 60*input->dt;

		if (player.position.x < entity->position.x && r2_intersects(get_entity_rect(&player), get_entity_rect(entity))) {
			player.velocity.x-=6000*input->dt;
		} else if (player.position.x > entity->position.x && r2_intersects(get_entity_rect(&player), get_entity_rect(entity))) {
			player.velocity.x+=6000*input->dt;
		}

		return;
	}

	if (player.current_health <= entity->enemy.damage) {
		player.current_health = 0;
		player.alive = false;
	}
	player.current_health-=entity->enemy.damage;
	player.state = HIT;
	
	invuln_time = 80*input->dt;
}

void player_move(Game_Input *input) {
	player.velocity.y+=496*input->dt;

	f32 dy = player.velocity.y*input->dt;
	f32 dx = player.velocity.x*input->dt;

	for (int i = 0; i < abs_f32(dy); i++) {
		player.position.y+=sign_f32(dy);
		if (wall_intersects(&player)) {
			player.position.y-=sign_f32(dy);
			player.velocity.y=0;
			break;
		}
	}


	for (int i = 0; i < abs_f32(dx); i++) {
		player.position.x+=sign_f32(dx);
		if (wall_intersects(&player)) {
			player.position.x-=sign_f32(dx);
			player.velocity.x=0;
			break;
		}
	}
}