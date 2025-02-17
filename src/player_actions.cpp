void player_attack_one() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{

			if (player.current_stamina > 20 && player.animation.frame == 0 && player.animation.index == 0)
			{
				player.current_stamina -=20;
			} else if (player.animation.frame >= 3 && player.animation.frame <= 4)
			{

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.animation.frame == 5)
			{
				set_enemy_vuln();

				if (player.animation.index > 7 && c0.bumper) {
					player.state = ATTACKTWO;
				}

			} else
			{
				player.state = NEUTRAL;
			}

			draw_player(cf_attack_one);
		} break;
	case pw_staff:
		{
			if (player.animation.index == 0 && player.animation.frame == 0 && player.current_stamina >= 20)
			{
				player.current_stamina-=20;
			} else if (player.animation.frame == 1 && player.animation.index == 0)
			{
				magic_emit(sp_magic_missile);
			} else if (player.animation.frame == 1 && player.animation.index == 15)
			{
				player.state = NEUTRAL;
			}

			draw_player(sf_attack);
		} break;
	}
}

void player_attack_two() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (player.animation.index == 0 && player.animation.frame == 0 && player.current_stamina >= 20)
			{
				player.velocity.x+=2000*input->dt*sign_f32(player.facing);
				player.velocity.y-=800*input->dt;

				player.current_stamina -=20;
			} else if (player.animation.frame == 0 && player.animation.index > 0)
			{
				player.velocity.x+=2000*input->dt*sign_f32(player.facing);
				player.velocity.y-=800*input->dt;
			} else if (player.animation.frame >= 1 && player.animation.frame <= 3)
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.animation.frame == 4)
			{
				set_enemy_vuln();

				if (player.animation.index > 7 && c0.bumper)
				{
					player.state = ATTACKTHREE;
				}
			} else
			{
				player.state = NEUTRAL;

			}

			draw_player(cf_attack_two);
		} break;
	case pw_staff:
		{

		} break;
	}
}

void player_attack_three() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (player.animation.index == 0 && player.animation.frame == 0 && player.current_stamina > 20)
			{
				player.velocity.x+=1000*input->dt*sign_f32(player.facing);

				player.current_stamina -=20;
			} else if (player.animation.frame == 0 && player.animation.index > 0)
			{
				player.velocity.x+=1000*input->dt*sign_f32(player.facing);

			} else if (player.animation.frame >= 1 && player.animation.frame <= 3)
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.animation.frame == 4)
			{
				set_enemy_vuln();
			} else
			{
				player.state = NEUTRAL;
			}

			draw_player(cf_attack_three);
		} break;
	case pw_staff:
		{

		} break;
	}
}

void player_charging() {
	Controller c0 = input->controllers[0];
	
	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (!c0.trigger || player.current_stamina < 20) 
			{
				player.state = NEUTRAL;
			} else if (c0.trigger && player.animation.index == 44 && player.current_stamina > 20) {
				player.current_stamina-=40;

				player.current_stamina = clamp_i32(player.current_stamina, 0, player.max_stamina);

				player.state = STATECHARGEATTACKING;
			} else
			{
				player.state = NEUTRAL;
			}

			draw_player(cf_charge_attack);
		} break;
	case pw_staff:
		{
			switch (player.spell_selected)

			{
			case sp_flame_wheel:
			case sp_flame_rain:
				{

					if (c0.trigger && player.animation.index == 44)
					{
						player.current_stamina-=40;
		
						player.current_stamina = clamp_i32(player.current_stamina, 0, player.max_stamina);
		
						player.state = STATECHARGEATTACKING;
					} else if (!c0.trigger || player.current_stamina < 20)
					{
						player.state = NEUTRAL;
					}

					draw_player(sf_charged_spell);
				} break;
			case sp_flame_ball:
				{

					if (c0.trigger && player.animation.index == 18)
					{
						player.state = STATECHARGEATTACKING;
					} else if (!c0.trigger || player.current_stamina < 20)
					{
						player.state = NEUTRAL;
					}

					draw_player(sf_charged_spell);
				} break;
			}
		} break;
	}
}

void player_charge_attacking() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (player.animation.frame == 0)
			{
				player.animation.frame = 1;
			}

			if (player.animation.frame >= 1 && player.animation.frame <= 3)
			{
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 2);
			} else if (player.animation.frame == 4)
			{
				set_enemy_vuln();
			} else {
				player.state = NEUTRAL;
				set_enemy_vuln();
			}

			draw_player(cf_charge_attack);
		} break;
	case pw_staff:
		{
			if (player.animation.frame == 0)
			{
				player.animation.frame = 1;
			}
			switch (player.spell_list.data[player.spell_selected].type)
			{
			case sp_flame_wheel:
				{
					
					if (player.animation.index == 0 && player.current_mp > 40) {
						player.current_mp-=40;
						magic_emit(sp_flame_wheel);
					} else if (player.animation.index == 15)
					{
						player.state = NEUTRAL;
					}

					draw_player(sf_charged_spell);
				} break;
			case sp_flame_rain:
				{
					Image portal[] = {
						LoadImage(S("flame_portal1.png")),
						LoadImage(S("flame_portal2.png")),
					};

					if (c0.trigger && player.current_mp >= 10 && player.current_stamina >= 10)
					{

						i32 index = 0;

						i32(player.state_time*fps)%8 < 4 ? index = 0 : index = 1; 

						DrawImage(portal[index], v2(player.position.x+camera_offset,
							player.position.y-80));

						if (player.animation.index == 9)
						{
							magic_emit(sp_flame_rain);
							player.current_mp-=10;
							player.current_stamina-=10;
							player.animation.index = 0;
						}
					} else
					{
						player.state = NEUTRAL;
					}

					draw_player(sf_charged_spell);
				} break;
			case sp_flame_ball:
				{
					if (player.animation.index == 0 && player.current_mp >= 8 && player.current_stamina >= 20)
					{
						magic_emit(sp_flame_ball);
						player.current_stamina-=10;
						player.current_mp-=8;
					}  else if (player.animation.index == 15)
					{
						player.state = NEUTRAL;
					}

					draw_player(sf_charged_spell);
				} break;
			}
		} break;
	}
}

