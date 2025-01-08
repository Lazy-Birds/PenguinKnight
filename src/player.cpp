f32 sleep = 0;
i32 state_change = 0;

f32 state_time = 0;
f32 heal_cd = 0;

void player_action(Game_Input *input) {
	f32 max_speed = 600.0;
	i32 weapon_cooldown = 2;
	f32 dt = input->dt;
	Controller c0 = input->controllers[0];
	i32 offset = camera_pos.x-player.position.x;
	Level *level = &World[player.player_level];
	player.can_climb = false;
	
	player.state_time+=dt;
	state_time+=dt;
	if (player.mp_cooldown > 0) {
		player.mp_cooldown-=dt;
	}

	if (player.shield_hit > 0) {
		player.shield_hit-=dt;
	}

	if (player_in_poison()) {
		player.current_health = 0;
		player.alive = false;
	} 

	if (invuln_time > 0){
		invuln_time-=dt;
	}

	if (heal_cd > 0) {
		heal_cd-=dt;
	}

	if (player.acting_cd > 0) {
		player.acting_cd-=input->dt;
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
			player.can_climb = true;

		} else if (c0.bumper && entity_in_air(&player)) {
			player.state = STATEJUMPATTACK;
			state_time = 0;
		}
		else if (c0.bumper) {
			player.state = ATTACK;
			state_time = 0;
		} else if (c0.trigger) {
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

	//Healing
	if (c0.h && heal_cd <= 0 && player.current_fairy_uses > 0) {
		player.current_health += 50;
		player.current_health = clamp_i32(player.current_health, 0, player.max_health);
		player.current_fairy_uses-=1;
		heal_cd+=120*dt;
	}

	//Toggles player as interacting
	if (c0.u && !in_menu) {
		player.acting = true;
	} else {
		player.acting = false;
	}
	
	if (!(sleep<=0)) {
		sleep--;
		if (player.state == ATTACK)
		{
			draw_player(
				player.weapon.weapon_frames.y-1);
		} else if (player.state == STATECHARGEATTACKING)
		{
			draw_player(
				player.weapon.charged_frames.y-1);
		}
		return;
	}
	switch (player.state) {
	case NEUTRAL: 
		{
			player.facing = sign_i32(player.facing);
			player.velocity.x = move_f32(player.velocity.x, 0, 3000 * dt);
			draw_player( 0);
		} break;
	case MOVE:
		{
			
			if (c0.right)
			{
				
				player.velocity.x = move_f32(player.velocity.x, 350, 1000 * dt);
				

				if (player.facing < 0) {
					player.facing = 1;
				} else {
					player.facing++;
				}

				if (i32(state_time*60)%30 < 15) {
					draw_player(0);	
				} else {
					draw_player(1);
				}
				
			}
			else if (c0.left)
			{
				
				player.velocity.x = move_f32(player.velocity.x, -350, 1000 * dt);
				
				if (player.facing > 0) {
					player.facing = -1;
				} else {
					player.facing--;
				}

				if (i32(state_time*60)%30 < 15) {
					draw_player(0);	
				} else {
					draw_player( 
						1);
				}
			}

			if (c0.bumper && entity_in_air(&player)) {
				state_time = 0;
				player.state = STATEJUMPATTACK;
				draw_player( 0);
			} else if (c0.bumper) {
				state_time = 0;
				player.state = ATTACK;
				draw_player( 0);
			} else if (c0.trigger) {
				state_time = 0;
				player.state = CHARGING;
				draw_player( 0);
			} else if (c0.b) {
				state_time = 0;
				player.state = STATEDASH;
				draw_player( 0);
			} else if (c0.up) {
				player.state = JUMP;
				draw_player( 0);
			} else if (!c0.left && !c0.right) {

				player.state = NEUTRAL;
				draw_player( 0);
			} else if (c0.a) {
				player.state = GUARD;
				draw_player( 0);
			}
			

		} break;
	case JUMP:
		{
			if (!entity_on_wall(&player)) {
				draw_player(0);
				player.state = NEUTRAL;
			} else if (player.current_stamina > 20) {
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
				if (state_time*60 < 7) {
					draw_player(player.weapon.jump.x);
				} else if (state_time*60 < 14 && entity_on_wall(&player)) {
					draw_player(player.weapon.jump.y);
					player.velocity.y-=240;
					player.current_stamina-=20;

					MixerPlaySound(player.jumpies, 1.0);

				} else if (player.velocity.y < 0) {
					draw_player(player.weapon.jump.y);
				} else {
					draw_player(0);
					player.state = NEUTRAL;
				}
			} else {
				draw_player(0);
				player.state = NEUTRAL;
			}
		} break;
	case ATTACK: 
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (player.current_stamina > 20 && state_time == 0)
			{
				draw_player(
					state_time*12+player.weapon.weapon_frames.x);
				player.current_stamina -=20;
			} else if (state_time*12 < player.weapon.weapon_frames.y-player.weapon.weapon_frames.x)
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);

				draw_player(state_time*12+player.weapon.weapon_frames.x);

			}  else if (state_time*12<player.weapon.weapon_frames.y-1)
			{
				draw_player(
					player.weapon.weapon_frames.y-1);
				sleep = weapon_cooldown*4;
			} else
			{
				set_enemy_vuln();
				draw_player(player.weapon.weapon_frames.y-1);
				player.state = 0;
			}
			
		} break;
	case CHARGING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (c0.trigger && state_time*9 < player.weapon.charge_time && player.current_stamina > 1)
			{
				//DrawImage(charge_meter[i32(state_time*9)], v2(player.position.x-player.position.x+out->width*.5-16, player.position.y-16));
				draw_charging(i32(state_time*9));
				draw_player(player.weapon.charged_frames.x);

			} else if (c0.trigger && state_time*9 > player.weapon.charge_time) 
			{
				draw_player(player.weapon.charged_frames.x);
				player.current_stamina-=40;
				player.state = STATECHARGEATTACKING;
			} else
			{

				draw_player( 0);
				player.state = NEUTRAL;
			}
			
		} break;
	case STATECHARGEATTACKING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			if (state_time*11 + player.weapon.charged_frames.x < player.weapon.charged_frames.y) 
			{
				draw_player(
					state_time*11 + player.weapon.charged_frames.x);
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (state_time*11+ player.weapon.charged_frames.x
				> player.weapon.charged_frames.y && state_time*11 + player.weapon.charged_frames.x
				< player.weapon.charged_frames.y + 2) 
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
				sleep += 45;
				draw_player(player.weapon.charged_frames.y-1);
				//make_projectile(player.projectile, v2(player.position.x+100*sign_f32(player.facing), player.position.y-14), v2(player.facing*400, 0), 1, 1);
			} else {
				draw_player(0);
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
					draw_player(
						player.weapon.jump_frame.x);
					player.current_stamina-=20;
					player.velocity.x += 1600*dt;
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (entity_in_air(&player) && state_time > 0) 
				{
					draw_player( 
						player.weapon.jump_frame.x);
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (!entity_in_air(&player) && state_time > 0) 
				{
					draw_player( 
						player.weapon.jump_frame.y);
					player.state = LANDED;
					state_time = 0;
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else
				{
					draw_player( 
						0);
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
				draw_player( 
					player.weapon.jump_frame.y);

			} else 
			{
				draw_player( 
					player.weapon.jump_frame.y);
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

				draw_player(player.weapon.frame_hit);

				MixerPlaySound(player.hit, 1.0);
			} else {
				draw_player(player.weapon.frame_hit);
				player.state = NEUTRAL;

			} 
			
		} break;
	case EXHAUSTED:
		{

		} break;
	case STATEDASH:
		{
			if (player.current_stamina < 1) {
				draw_player( 
					0);
				player.state = NEUTRAL;

			} else if (state_time*60 <= 1) {
				player.velocity.x = 0;
				draw_player( 
					player.weapon.dash_frame.x);
				player.current_stamina -= 10;
			} else if (state_time*60 < 8) {
				draw_player( 
					player.weapon.dash_frame.x);
			} else if (state_time*60 < 12) {
				draw_player( 
					player.weapon.dash_frame.x+1);
				invuln_time+=(10*dt);
			} else if (state_time*60 < 15) {
				draw_player( 
					player.weapon.dash_frame.x+2);
				player.velocity.x+=600*dt*sign_f32(player.facing);
			} else if (c0.b) {
				if (i32(state_time*60)%20 < 10) {
					draw_player( 
						player.weapon.dash_frame.x+3);
				} else {
					draw_player( 
						player.weapon.dash_frame.x+2);
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
				draw_player( 
					player.weapon.dash_frame.x+1);
				//player.velocity.x-=8000*dt*sign_f32(player.facing);
			} else {
				draw_player( 
					0);
				player.state = NEUTRAL;
			}
		} break;
	case GUARD:
		{
			if (state_time*60 < 15) {
				draw_player( 
					player.weapon.guard.x);
				player.velocity.x = 0;
			} else if (c0.a && player.current_mp > 0 && player.shield_hit > 0) {
				draw_player( 
					player.weapon.guard.y);
				player.current_mp-=30*dt;
			} else if (c0.a && player.current_mp > 0) {
				draw_player( 
					player.weapon.guard.y-1);
				player.current_mp-=30*dt;
			}  else {
				draw_player( 
					0);
				player.state = NEUTRAL;
			}
		} break;
	case HOOKSHOTTING:
		{
			if (player.acting) {

				for (int i = 0; i < level->interactible.count; i++) {
					if (level->interactible.data[i].id == 5 && entity_get_anchor_distance_direct(&player, &level->interactible.data[i]) < 300) {
						draw_hook_shot(&level->interactible.data[i]);
					}

				}
				draw_player(21);
			} else {
				draw_player(21);
				player.state = NEUTRAL;
			}
			
		} break;
	case DEAD:
		{

		} break;
	}

	player.anchor = v2(player.position.x+15, player.position.y+25);
}

void check_level() {
	for (int i = 0; i < tele_count; i++) {
		if (player.player_level == tele[i].level_id && tele[i].actable && player.acting_cd <=0 && player.acting && r2_intersects(get_entity_rect(&player), tele[i].entry)) {
			player.acting_cd+=1.0;
			player.position = tele[tele[i].link_id].landing_pos;
			player.player_level = tele[tele[i].link_id].level_id;
			set_world(false, World[player.player_level]);
		} else if (!tele[i].actable && player.player_level == tele[i].level_id && r2_intersects(get_entity_rect(&player), tele[i].entry)) {
			player.position = tele[tele[i].link_id].landing_pos;
			player.player_level = tele[tele[i].link_id].level_id;
			set_world(false, World[player.player_level]);
		}
	}
}

void set_enemy_vuln() {
	for (int i = 0; i < World[player.player_level].enemies.count; i++) {
		if (World[player.player_level].enemies.data[i].alive) {
			World[player.player_level].enemies.data[i].invuln = false;

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

b32 player_in_poison() {
	for (i32 i = 0; i < World[player.player_level].liquid.count; i++) {
		if (r2_intersects(get_entity_rect(&player), get_entity_rect(&World[player.player_level].liquid.data[i]))) {
			return true;
		}
	}

	return false;
}

void draw_hook_shot(Entity *target) {
	f32 radians = angle_between(player.anchor, target->anchor);

	Vector2 step = v2_arm(radians);

	f32 distance_x = abs_f32(player.anchor.x - target->anchor.x);
	f32 distance_y = abs_f32(player.anchor.y-target->anchor.y);

	f32 distance = Sqrt(Pow(distance_x, 2) + Pow(distance_y, 2));

	step.x = step.x*7;
	step.y = step.y*7;

	Image rope_outer = LoadImage(S("rope_outer.png"));

	for (int i = 0; i < distance/7; i++) {
		DrawImage(rope_outer, v2(player.anchor.x+(i*step.x)*-get_entity_direction(target)+camera_offset,
			player.anchor.y+(i*step.y)));
	}
}

void draw_player(i32 frame) {
	Weapon weapon = player.weapon;
	Vector2 position = player.position;
	i32 facing = player.facing;

    if (sign_i32(facing) < 0) {
        DrawImageMirrored(weapon.image[frame], v2(position.x + weapon.offset_left.x+camera_offset, position.y+weapon.offset_left.y), true, false);
        //DrawImage(weapon.image[i32(frame + weapon.weapon_frames.y+1)], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y));
    } else {
        DrawImage(weapon.image[frame], v2(position.x + weapon.offset_right.x+camera_offset, position.y+weapon.offset_right.y));
    }
}

void player_move(Game_Input *input) {
	player.velocity.y+=496*input->dt;

	f32 dy = player.velocity.y*input->dt;
	f32 dx = player.velocity.x*input->dt;

	for (int i = 0; i < abs_f32(dy); i++) {
		player.position.y+=sign_f32(dy);
		if (wall_intersects(&player)) {
			if (player.state == HOOKSHOTTING) player.state = NEUTRAL;
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

	player.anchor = v2(player.position.x+15, player.position.y+25);
}