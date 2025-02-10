void player_attack_one() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (player.current_stamina > 20 && player.state_time*60 < 1)
			{

				draw_player(fr_atk_one);
				player.current_stamina -=20;
			} else if (player.state_time*60 < 6)
			{
				draw_player(fr_atk_one);
			} else if (player.state_time*60 < 12)
			{
				draw_player(fr_atk_two);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*60 < 18)
			{
				draw_player(fr_atk_three);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			}  else if (player.state_time*60 < 24)
			{
				draw_player(fr_atk_four);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*60 < 30)
			{
				draw_player(fr_atk_five);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*60 < 42)
			{
				set_enemy_vuln();
				draw_player(fr_atk_six);

				if (player.state_time*60 > 34 && c0.bumper) {
					player.state = ATTACKTWO;
				}

			} else
			{
				
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case pw_staff:
		{
			if (player.state_time*fps < 1 && player.current_stamina > 20)
			{
				draw_player(fr_atk_one);
				player.current_stamina-=20;
			} else if (player.state_time*fps < 10)
			{
				draw_player(fr_atk_one);
			} else if (player.state_time*fps < 11)
			{
				draw_player(fr_atk_two);
				magic_emit(sp_magic_missile);
				player.current_mp-=5;
			} else if (player.state_time*fps < 20)
			{
				draw_player(fr_atk_two);
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	}
}

void player_attack_two() {
	Controller c0 = input->controllers[0];

	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
			if (player.current_stamina > 20 && player.state_time*fps < 1)
			{
				player.velocity.x+=2000*input->dt*sign_f32(player.facing);
				player.velocity.y-=800*input->dt;

				draw_player(fr_atk_seven);
				player.current_stamina -=20;
			} else if (player.state_time*fps < 6)
			{
				player.velocity.x+=2000*input->dt*sign_f32(player.facing);
				player.velocity.y-=800*input->dt;

				draw_player(fr_atk_seven);
			} else if (player.state_time*fps < 12)
			{
				draw_player(fr_atk_eight);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 18)
			{
				draw_player(fr_atk_nine);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 24)
			{
				draw_player(fr_atk_ten);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 36)
			{
				draw_player(fr_atk_eleven);

				set_enemy_vuln();

				if (player.state_time*fps > 30 && c0.bumper)
				{
					player.state = ATTACKTHREE;
				}
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
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
			if (player.current_stamina > 20 && player.state_time*fps < 1)
			{
				player.velocity.x+=1000*input->dt*sign_f32(player.facing);

				draw_player(fr_atk_eleven);
				player.current_stamina -=20;
			} else if (player.state_time*fps < 6)
			{
				player.velocity.x+=1000*input->dt*sign_f32(player.facing);

				draw_player(fr_atk_eleven);
			} else if (player.state_time*fps < 12)
			{
				draw_player(fr_atk_twelve);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 18)
			{
				draw_player(fr_atk_thirteen);
			
				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 24)
			{
				draw_player(fr_atk_fourteen);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 30)
			{
				draw_player(fr_atk_fifteen);

				weapon_attack(player.position, player.weapon, player.facing, get_dmg_attr(), 0);
			} else if (player.state_time*fps < 42)
			{
				draw_player(fr_atk_sixteen);

				set_enemy_vuln();
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			} 
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
			if (c0.trigger && player.state_time*60 < 30 && player.current_stamina > 20) 
			{
				draw_player(fr_chatk_one);
			} else if (c0.trigger && player.state_time*60 >= 30 && player.current_stamina > 20) {
				draw_player(fr_chatk_one);
				player.current_stamina-=40;

				player.current_stamina = clamp_i32(player.current_stamina, 0, player.max_stamina);

				player.state = STATECHARGEATTACKING;
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	case pw_staff:
		{
			if (c0.trigger && player.state_time*fps < 45 && player.current_stamina > 20)
			{
				draw_player(fr_chatk_one);
			}
			else if (c0.trigger && player.state_time*fps >= 45 && player.current_stamina > 20)
			{
				draw_player(fr_chatk_one);
				player.current_stamina-=40;

				player.current_stamina = clamp_i32(player.current_stamina, 0, player.max_stamina);

				player.state = STATECHARGEATTACKING;
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	}
}

void player_charge_attacking() {
	switch (player.weapon.type)
	{
	case pw_cleaver:
		{
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
		} break;
	case pw_staff:
		{
			if (player.state_time*fps < 1 && player.current_mp > 40) {
				player.current_mp-=40;
				magic_emit(sp_flame_wheel);
				draw_player(fr_chatk_two);
			} else if (player.state_time*fps < 18)
			{
				draw_player(fr_chatk_two);
			} else
			{
				draw_player(fr_neutral);
				player.state = NEUTRAL;
			}
		} break;
	}
}

