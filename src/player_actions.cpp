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

Entity spells[100] = {};
i32 spell_count = 0;

i32 get_spell_slot() {

	for (int i = 0; i < 99; i++)
	{
		if (!spells[i].alive) return i;
	}

	return 100;
}

void magic_emit(i32 spell_type) {
	i32 spell_slot = get_spell_slot();

	assert(spell_slot < 100);

	Entity *spell = &spells[spell_slot];

	spell->alive = true;
	spell->state_time = 0;
	spell->sprite_index = 0;
	spell->type = spell_type;
	spell->id = spell_count;
	spell->facing = player.facing;

	switch (spell_type)
	{
	case sp_magic_missile:
		{
			static Image image[] = {
				LoadImage(S("magic_missile1.png")),
				LoadImage(S("magic_missile2.png")),
			};

			spell->image = image;
			spell->position = v2(player.position.x + 47*sign_f32( player.facing), player.position.y+36);
			spell->size = v2(16, 9);
			spell->hitbox = hitbox_simple(spell);
			spell->velocity = v2(20000*input->dt*sign_f32(player.facing), 0);
			spell->damage = 15;
			draw_entity(spell, spell->sprite_index);
			spell_count++;
		} break;
	}
}

void update_spells() {
	for (int i = 0; i < spell_count; i++)
	{
		Entity *spell = &spells[i];

		if (!spell->alive) continue;

		switch (spell->type)
		{
		case sp_magic_missile:
			{
				spell->state_time+=input->dt;

				if (mod_f32(spell->state_time*fps, 6) == 0)
				{
					spell->sprite_index == 0 ? spell->sprite_index = 1 : spell->sprite_index = 0;
				}

				move_entity(spell, input->dt);
				spell->hitbox = hitbox_simple(spell);

				draw_entity(spell, spell->sprite_index);

				Entity *enemy = enemy_hit(spell->hitbox);

				if (enemy!=nullptr)
				{
					entity_take_damage(enemy, spell);
					enemy->invuln = false;
					spell->alive = false;
				}

				if (wall_ahead(spell) || spell->state_time*fps > 80) spell->alive = false;
			} break;
		}
	}
}