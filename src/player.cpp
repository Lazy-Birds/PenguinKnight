i32 player_state = 0;
f32 sleep = 0;
i32 player_state_change = 0;

const i32 NEUTRAL = 0;
const i32 STATEMOVING = 1;
const i32 STATEATTACKING = 2;
const i32 STATECHARGING = 3;
const i32 STATECHARGEATTACKING = 4;
const i32 STATEJUMPATTACK = 5;
const i32 EXHAUSTED = 6;
const i32 LANDED = 7;
const i32 DEAD = 8;

f32 state_time = 0;

void player_action(Game_Input *input) {
	f32 max_speed = 600.0;
	i32 weapon_cooldown = 2;
	Controller c0 = input->controllers[0];
	f32 dt = input->dt;
	

	state_time+=dt;

	//Edge cases
	if (player_state != player_state_change) {
		player_state_change = player_state;
		state_time = 0;
	}

	//Gear Change
	if (player_state == NEUTRAL) {
		if (c0.up || c0.right || c0.left) {
			player_state = STATEMOVING;
			state_time = 0;
		} else if (input->mouse.left && entity_in_air(&player)) {
			player_state = STATEJUMPATTACK;
			state_time = 0;
		}
		 else if (input->mouse.left) {
			player_state = STATEATTACKING;
			state_time = 0;
		} else if (input->mouse.right) {
			player_state = STATECHARGING;
			state_time = 0;
		}
	}

	

	switch (player_state) {
	case NEUTRAL: 
		{
			player.facing = sign_i32(player.facing);
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 0, player.facing);
		} break;
	case STATEMOVING:
		{
			if (sleep == 0)
			{
				if (c0.right)
				{
					if (c0.b) {
						player.velocity.x = move_f32(player.velocity.x, max_speed, max_speed * dt);
					} else {
						player.velocity.x = move_f32(player.velocity.x, 300, 300 * dt);
					}

					if (player.facing < 0) {
						player.facing = 1;
					} else {
						player.facing++;
					}
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
						get_frame_walk(dt), player.facing);
				}
				else if (c0.left)
				{
					if (c0.b) {
						player.velocity.x = move_f32(player.velocity.x, -max_speed, max_speed * dt);
					} else {
						player.velocity.x = move_f32(player.velocity.x, -300, 300 * dt);
					}
					if (player.facing > 0) {
						player.facing = -1;
					} else {
						player.facing--;
					}
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
						get_frame_walk(dt), player.facing);
				}
				
				if (c0.up && player.current_stamina > 20) {
					if (player.position.y==out->height-player.size.y || entity_on_wall(&player)) {
						player.velocity.y=-248;
						player.current_stamina-=20;
					}
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
						get_frame_walk(dt), player.facing);

				}

				if (input->mouse.left) {
					state_time = 0;
					player_state = STATEATTACKING;
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 0, player.facing);
				} else if (!c0.left && !c0.right && !c0.up) {

					player_state = NEUTRAL;
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 0, player.facing);
				}

			} else 
			{
				sleep--;
			}

		} break;
	case STATEATTACKING: 
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			if (sleep == 0) {
				if (player.current_stamina > 20 && state_time == 0)
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						state_time*9+player.weapon.weapon_frames.x, player.facing);
					player.current_stamina -=20;
				} else if (state_time*9 < player.weapon.weapon_frames.y-player.weapon.weapon_frames.x)
				{
					weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr());

					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						state_time*9+player.weapon.weapon_frames.x, player.facing);

				}  else if (state_time*9<player.weapon.weapon_frames.y-1)
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						player.weapon.weapon_frames.y-1, player.facing);
					sleep = weapon_cooldown*6;
				} else
				{
					for (int i = 0; i < enemy_count; i++) {
						if (enemy[i].alive) {
							enemy[i].invuln = false;

						}
					}
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						player.weapon.weapon_frames.y-1, player.facing);
					player_state = 0;
				}
			} else 
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
					player.weapon.weapon_frames.y-1, player.facing);
				sleep--;
			}
		} break;
	case STATECHARGING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			if (sleep == 0) 
			{
				if (input->mouse.right && state_time*9 < player.weapon.charge_time && player.current_stamina > 1)
				{
					DrawImage(charge_meter[i32(state_time*9)], v2(player.position.x-player.position.x+out->width*.5-16, player.position.y-16));
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						player.weapon.charged_frames.x, player.facing);
					
				} else if (input->mouse.right && state_time*9 > player.weapon.charge_time) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						player.weapon.charged_frames.x, player.facing);
					player.current_stamina-=40;
					player_state = STATECHARGEATTACKING;
				} else
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 0, player.facing);
					player_state = NEUTRAL;
				}
			} else
			{
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), player.weapon.charged_frames.y-1
						, player.facing);
				sleep--;
			}
		} break;
	case STATECHARGEATTACKING:
		{
			player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
			if (sleep == 0)
			{
				if (state_time*9 + player.weapon.charged_frames.x < player.weapon.charged_frames.y) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
						state_time*9 + player.weapon.charged_frames.x, player.facing);
				} else if (state_time*9+ player.weapon.charged_frames.x
				 > player.weapon.charged_frames.y && state_time*9 + player.weapon.charged_frames.x
				  < player.weapon.charged_frames.y + 2) 
				{
					sleep += 45;
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
					 player.weapon.charged_frames.y-1, player.facing);
				} else {
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
					 0, player.facing);
					player_state = NEUTRAL;
				}
			}
			else {
				draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
					 player.weapon.charged_frames.y-1, player.facing);
				sleep--;
			}
		} break;
	case STATEJUMPATTACK: 
		{
			if (sleep == 0) 
			{
				if (string_equals(player.weapon.name, S("Cleaver"))) 
				{
					player.velocity.x = 0;

					if (state_time == 0 && player.current_stamina > 19) 
					{
						draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y),
							player.weapon.jump_frame.x, player.facing); player.current_stamina-=20;
					} else if (entity_in_air(&player) && state_time > 0) 
					{
						draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
							player.weapon.jump_frame.x, player.facing);
					} else if (!entity_in_air(&player) && state_time > 0) 
					{
						draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
							player.weapon.jump_frame.y, player.facing);
						player_state = LANDED;
						state_time = 0;
					} else
					{
						player_state = NEUTRAL;
					}
				} else 
				{
					dump("oops");
				}

			} else 
			{
				sleep--;
			}
		} break;
	case LANDED:
		{
			if (sleep == 0) 
			{
				if (state_time*9 < 4) 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
						player.weapon.jump_frame.y, player.facing);

				} else 
				{
					draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 
						player.weapon.jump_frame.y, player.facing);
					player_state = NEUTRAL;
				}
			}
		}
	case EXHAUSTED:
		{

		}
	}
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
		}
	}


	for (int i = 0; i < abs_f32(dx); i++) {
		player.position.x+=sign_f32(dx);
		if (wall_intersects(&player)) {
			player.position.x-=sign_f32(dx);
			player.velocity.x=0;
		}
	}
}