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

	if (player.weapon.sprite_index > 0) {
		charged_projectile(&player.weapon);
	}

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
		switch (state_change)
		{
		case ATTACK:
		case STATECHARGEATTACKING:
		case STATEJUMPATTACK:
			{
				set_enemy_vuln();
			} break;
		}

		state_change = player.state;
		player.state_time = 0;
	}

	//Gear Change
	if (player.state == NEUTRAL) {
		if (!player.alive) {
			player.state = DEAD;
			player.state_time = 0;
		}
		if (c0.right || c0.left) {
			player.state = MOVE;
			player.state_time = 0;
		} else if (c0.up) {
			player.state = JUMP;
			player.state_time = 0;
			player.can_climb = true;

		} else if (c0.bumper && entity_in_air(&player)) {
			player.state = STATEJUMPATTACK;
			player.state_time = 0;
		}
		else if (c0.bumper) {
			player.state = ATTACK;
			player.state_time = 0;
		} else if (c0.trigger) {
			player.state = CHARGING;
			player.state_time = 0;
		} else if (c0.b) {
			player.state = STATEDODGEDASH;
			player.state_time = 0;
		} else if (c0.a) {
			player.state = GUARD;
			player.state_time = 0;
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
			draw_player(fr_atk_six);
		} else if (player.state == STATECHARGEATTACKING)
		{
			draw_player(fr_chatk_four);
		}
		return;
	}
	switch (player.state) {
	case NEUTRAL: 
		{
			player.facing = sign_i32(player.facing);
			player.velocity.x = move_f32(player.velocity.x, 0, 3000 * dt);
			draw_player(fr_neutral);
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
					draw_player(fr_neutral);	
				} else {
					draw_player(fr_walk);
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
					draw_player(fr_neutral);	
				} else {
					draw_player(fr_walk);
				}
			}

			if (c0.bumper && entity_in_air(&player)) {
				state_time = 0;
				player.state = STATEJUMPATTACK;
				draw_player(fr_neutral);
			} else if (c0.bumper) {
				state_time = 0;
				player.state = ATTACK;
				draw_player(fr_neutral);
			} else if (c0.trigger) {
				state_time = 0;
				player.state = CHARGING;
				draw_player(fr_neutral);
			} else if (c0.b) {
				state_time = 0;
				player.state = STATEDODGEDASH;
				draw_player(fr_neutral);
			} else if (c0.up) {
				player.state = JUMP;
				draw_player(fr_neutral);
			} else if (!c0.left && !c0.right) {

				player.state = NEUTRAL;
				draw_player(fr_neutral);
			} else if (c0.a) {
				player.state = GUARD;
				draw_player(fr_neutral);
			}
			

		} break;
	case JUMP:
		{
			if (!entity_on_wall(&player)) {
				draw_player(fr_neutral);
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
				if (player.state_time*60 < 7) {
					draw_player(fr_jmp_one);
				} else if (player.state_time*60 < 14 && entity_on_wall(&player)) {
					draw_player(fr_jmp_two);
					player.velocity.y-=240;
					player.current_stamina-=20;

					MixerPlaySound(player.jumpies, 1.0);

				} else if (player.velocity.y < 0) {
					draw_player(fr_jmp_two);
				} else {
					draw_player(fr_neutral);
					player.state = NEUTRAL;
				}
			} else {
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case ATTACK: 
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			
			player_attack_one();
		} break;
	case ATTACKTWO:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);

			player_attack_two(); 
		} break;
	case ATTACKTHREE:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);

			player_attack_three();
		} break;
	case CHARGING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);


			if (c0.trigger && player.state_time*60 < 30 && player.current_stamina > 1) 
			{
				draw_player(fr_chatk_one);
			} else if (c0.trigger && player.state_time*60 >= 30 && player.current_stamina > 1) {
				draw_player(fr_chatk_one);
				player.current_stamina-=40;

				player.current_stamina = clamp_i32(player.current_stamina, 0, player.max_stamina);

				player.state = STATECHARGEATTACKING;
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
			
			/*if (c0.trigger && state_time*9 < player.weapon.charge_time && player.current_stamina > 1)
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
			}*/
			
		} break;
	case STATECHARGEATTACKING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);

			if (player.state_time*fps < 6)
			{
				draw_player(fr_chatk_two);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (player.state_time*fps < 12)
			{
				draw_player(fr_chatk_three);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (player.state_time*fps < 18)
			{
				draw_player(fr_chatk_four);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (player.state_time*fps < 36)
			{
				if (player.weapon.sprite_index == 0 && player.current_mp > 20)
				{
					player.current_mp-= 20;
					player.weapon.sprite_index = 1;
				}
				draw_player(fr_chatk_five);
			} else {
				draw_player(fr_neutral);
				player.state = NEUTRAL;
				set_enemy_vuln();
			}
			
			/*if (state_time*11 + player.weapon.charged_frames.x < player.weapon.charged_frames.y) 
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
			}*/
			
			
		} break;
	case STATEJUMPATTACK: 
		{
			
			if (player.weapon.type == pw_cleaver) 
			{


				if (player.state_time == 0 && player.current_stamina > 19) 
				{
					draw_player(fr_jmp_atk_one);
					player.current_stamina-=20;
					player.velocity.x += 1600*dt*sign_f32(player.facing);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (entity_in_air(&player) && player.state_time*fps > 0) 
				{
					draw_player(fr_jmp_atk_one);
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else if (!entity_in_air(&player) && player.state_time*fps > 0) 
				{
					draw_player(fr_jmp_atk_two);
					player.state = LANDED;
					player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 1);
				} else
				{
					draw_player(fr_neutral);
					player.state = NEUTRAL;
				}
			} else 
			{
				Dump("oops");
			}

			
		} break;
	case LANDED:
		{
			if (player.state_time*fps < 30) 
			{
				player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
				draw_player(fr_jmp_atk_two);

			} else 
			{
				draw_player(fr_jmp_atk_two);
				player.state = NEUTRAL;
				player.velocity.x = move_f32(player.velocity.x, 0, 400 * dt);
				set_enemy_vuln();
			}
			
			
		} break;
	case HIT:
		{
			
			if (player.state_time == 0)
			{
				player.velocity.y = -6000*dt;
				player.velocity.x = -6000*dt;

				draw_player(fr_damage);

				MixerPlaySound(player.hit, 1.0);
			} else {
				draw_player(fr_damage);
				player.state = NEUTRAL;

			} 
			
		} break;
	case EXHAUSTED:
		{

		} break;
	case STATEDODGEDASH:
		Dump(ControllerDown(0, Button_B));

		{
			if (player.state_time*fps < 12 && ControllerReleased(0, Button_B))
			{
				draw_player(fr_neutral);
				player.state = STATEDODGE;
			} else if (player.state_time*fps < 12 && ControllerDown(0, Button_B))
			{
				Dump("Sitting Here");
				draw_player(fr_neutral);
			} else if (player.state_time*fps >= 12 && ControllerDown(0, Button_B))
			{
				Dump("Got Here");
				draw_player(fr_neutral);
				player.state = STATEDASH;
			} else
			{
				Dump("Why are we here?");
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case STATEDODGE:
		{
			f32 dodgespeed = 600*input->dt*sign_f32(player.facing);



			if (player.current_stamina > 20 && player.state_time*fps < 1)
			{
				draw_player(fr_dodge_one);
				player.current_stamina-=20;
				invuln_time+=24*input->dt;

				if (c0.right)
				{
					player.facing = 1;
				} else if (c0.left)
				{
					player.facing = -1;
				}
			} else if (player.state_time*fps < 4)
			{
				player.velocity.x+=dodgespeed;
				draw_player(fr_dodge_one);
			} else if (player.state_time*fps < 8)
			{
				
				player.velocity.x+=dodgespeed;
				draw_player(fr_dodge_two);
			} else if (player.state_time*fps < 12)
			{
				player.velocity.x+=dodgespeed;
				draw_player(fr_dodge_three);
			} else if (player.state_time*fps < 16)
			{
				player.velocity.x+=dodgespeed;
				draw_player(fr_dodge_four);
			} else if (player.state_time*fps < 20)
			{
				player.velocity.x+=dodgespeed;
				draw_player(fr_dodge_five);
			} else if (player.state_time*fps < 24)
			{
				player.velocity.x-=dodgespeed;
				draw_player(fr_dodge_six);
			} else
			{
				player.state = NEUTRAL;
				draw_player(fr_neutral);
			}
		} break;
	case STATEDASH:
		{
			if (player.current_stamina < 1) {
				draw_player(fr_neutral);
				player.state = NEUTRAL;

			} else if (state_time*60 <= 1) {
				player.velocity.x = 0;
				draw_player(fr_sprint_one);
				player.current_stamina -= 10;
			} else if (state_time*60 < 8) {
				draw_player(fr_sprint_one);
			} else if (state_time*60 < 12) {
				draw_player(fr_sprint_two);
				invuln_time+=(10*dt);
			} else if (state_time*60 < 15) {
				draw_player(fr_sprint_three);
				player.velocity.x+=600*dt*sign_f32(player.facing);
			} else if (c0.b) {
				if (i32(state_time*60)%20 < 10) {
					draw_player(fr_sprint_four);
				} else {
					draw_player(fr_sprint_three);
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

				min.life_time = 20*dt;
				max.life_time = 40*dt;

				Image image = LoadImage(S("snow_flake.png"));

				particle_emit(min, max, image);

				if (c0.up) {
					player.state = JUMP;
				}

			} else if (state_time*60 < 28) {
				draw_player(fr_sprint_two);
				//player.velocity.x-=8000*dt*sign_f32(player.facing);
			} else {
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case GUARD:
		{
			if (state_time*60 < 15) {
				draw_player(fr_grd_one);
				player.velocity.x = 0;
			} else if (c0.a && player.current_mp > 0 && player.shield_hit > 0) {
				draw_player(fr_grd_three);
				player.current_mp-=30*dt;
			} else if (c0.a && player.current_mp > 0) {
				draw_player(fr_grd_two);
				player.current_mp-=30*dt;
			}  else {
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case HOOKSHOTTING:
		{
			if (player.acting) {

				for (int i = 0; i < level->entities.count; i++) {
					if (level->entities.data[i].id == 5 && entity_get_anchor_distance_direct(&player, &level->entities.data[i]) < 300) {
						draw_hook_shot(&level->entities.data[i]);
					}

				}
				draw_player(fr_jmp_two);
			} else {
				draw_player(fr_jmp_two);
				player.state = NEUTRAL;
			}
			
		} break;
	case DEAD:
		{

		} break;
	}
	
	player.hitbox = r2_bounds(player.position, player.size, v2_zero, v2_one);
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

/*void change_player_velocity(i32 left, i32 up, i32 right) {
	if (left > 0)
	{
		player.velocity.x-=
	}
}*/

void set_enemy_vuln() {
	for (int i = 0; i < World[player.player_level].entities.count; i++) {
		if (World[player.player_level].entities.data[i].alive) {
			World[player.player_level].entities.data[i].invuln = false;

		}
	}
}

void player_hit(Entity *entity, Game_Input *input) {
	if (player.state == GUARD && player.current_mp > 10) {
		player.current_mp-=10;
		invuln_time = 60*input->dt;
		player.shield_hit = 60*input->dt;

		if (player.position.x < entity->position.x && r2_intersects(get_entity_rect(&player), entity->hitbox)) {
			player.velocity.x-=6000*input->dt;
		} else if (player.position.x > entity->position.x && r2_intersects(get_entity_rect(&player), entity->hitbox)) {
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

void charged_projectile(Weapon *wep) {
	switch (player.weapon.type)
	{
	case pw_cleaver:
		{	
			wep->state_time+=input->dt;

			i32 ind = wep->sprite_index;

			switch (ind)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				{
					if (player.facing < 0)
					{
						DrawImage(wep->projectile[wep->sprite_index], 
						v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
							player.position.y-128));
					} else
					{
						DrawImageMirrored(wep->projectile[wep->sprite_index], 
							v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
								player.position.y-128), true, false);
					}
					
				} break;
			case 4:
			case 5:
			case 6:
				{
					if (player.facing < 0)
					{
						DrawImage(wep->projectile[6-wep->sprite_index], 
							v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
								player.position.y-128));
					} else
					{
						DrawImageMirrored(wep->projectile[6-wep->sprite_index], 
							v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
								player.position.y-128), true, false);
					}
					
				} break;
			case 7:
				{
					if (player.facing < 0)
					{
						DrawImage(wep->projectile[5], 
							v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
								player.position.y-128));
					} else
					{
						DrawImageMirrored(wep->projectile[5], 
							v2(player.position.x+camera_offset+48*sign_f32(player.facing)+32*ind*sign_f32(player.facing),
								player.position.y-128), true, false);
					}
					
				} break;
			}

			Vector2 size_one = v2(42, 52);
			Vector2 size_two = v2(56, 99);
			Vector2 size_three = v2(64, 144);
			Vector2 size_four = v2(70, 180);
			Vector2 size_five = v2(36, 10);

			i32 pos_one = player.position.x+sign_f32(player.facing)*(48+32*ind-28);
			i32 pos_two = player.position.x+sign_f32(player.facing)*(48+32*ind-14);
			i32 pos_three = player.position.x+sign_f32(player.facing)*(48+32*ind-6);
			i32 pos_four = player.position.x+sign_f32(player.facing)*(48+32*ind);
			i32 pos_five = player.position.x+sign_f32(player.facing)*(48+32*ind-34);

			Rectangle2 hit_one = r2_bounds(v2(pos_one, player.position.y), size_one, v2_zero, v2_one);
			Rectangle2 hit_two = r2_bounds(v2(pos_two, player.position.y-29), size_two, v2_zero, v2_one);
			Rectangle2 hit_three = r2_bounds(v2(pos_three,	player.position.y-92), size_three, v2_zero, v2_one);
			Rectangle2 hit_four = r2_bounds(v2(pos_four, player.position.y-128), size_four, v2_zero, v2_one);
			Rectangle2 hit_five = r2_bounds(v2(pos_five, player.position.y+42), size_five, v2_zero, v2_one);

			switch (wep->sprite_index)
			{
			case 0:
			case 4:
				{
					weapon_charged_attack(wep, hit_one);

					hit_one = r2_shift(hit_one, v2(camera_offset, 0));
					DrawRectOutline(hit_one, v4_red, 2);
				} break;
			case 1:
			case 5:
				{
					weapon_charged_attack(wep, hit_two);

					hit_two = r2_shift(hit_two, v2(camera_offset, 0));
					DrawRectOutline(hit_two, v4_red, 2);
				} break;
			case 2:
			case 6:
				{
					weapon_charged_attack(wep, hit_three);

					hit_three = r2_shift(hit_three, v2(camera_offset, 0));
					DrawRectOutline(hit_three, v4_red, 2);
				} break;
			case 3:
				{
					weapon_charged_attack(wep, hit_four);

					hit_four = r2_shift(hit_four, v2(camera_offset, 0));
					DrawRectOutline(hit_four, v4_red, 2);
				} break;
			case 7:
				{
					weapon_charged_attack(wep, hit_five);

					hit_five = r2_shift(hit_five, v2(camera_offset, 0));
					DrawRectOutline(hit_five, v4_red, 2);
				} break;
			}

			if (i32(wep->state_time*fps)%6 == 0 && wep->sprite_index < 8)
			{
				wep->sprite_index++;
			}

			if (wep->sprite_index == 8) wep->sprite_index = 0;
		} break;
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

void draw_player_stats() {
	DrawRect(r2_bounds(v2(70, 20), v2(20+player.max_health, 12), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(72, 22), v2(16+player.current_health, 8), v2_zero, v2_one), v4_red);

    DrawRect(r2_bounds(v2(70, 34), v2(20+player.max_stamina, 12), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(72, 36), v2(16+player.current_stamina, 8), v2_zero, v2_one), v4_green);

    DrawRect(r2_bounds(v2(70, 48), v2(20+player.max_mp, 12), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(72, 50), v2(16+player.current_mp, 8), v2_zero, v2_one), v4_blue);
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