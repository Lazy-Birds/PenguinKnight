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
	case sp_flame_wheel:
		{
			static Image image[] = {
				LoadImage(S("flame_ball1.png")),
				LoadImage(S("flame_ball2.png")),
				LoadImage(S("flame_ball3.png")),
			};

			spell->image = image;
			spell->position = v2(player.position.x+28*sign_f32(player.facing), player.position.y+15);
			spell->size = v2(1, 1);
			spell->hitbox = hitbox_simple(spell);
			spell->velocity = v2(5000*input->dt*sign_f32(player.facing), 0);
			spell->sprite_index = 0;
			spell->state = st_increasing;
			spell->damage = 45;
			draw_flame_wheel(spell);
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
		case sp_flame_wheel:
			{
				spell->state_time+=input->dt;

				move_entity(spell, input->dt);

				if (i32(spell->state_time*fps)%8 == 0) {
					spell->state == st_increasing ? spell->sprite_index+=1 : spell->sprite_index-=1;
				}

				draw_flame_wheel(spell);

				if (spell->sprite_index == 2)
				{
					spell->state = st_decreasing;
				} else if (spell->sprite_index == 0)
				{
					spell->state = st_increasing;
				}
			} break;
		}
	}
}

void draw_flame_wheel(Entity *ent) {
	f32 angle_one = degrees_to_radians(45.0+2*i32(ent->state_time*fps));
	f32 angle_two = degrees_to_radians(135.0+2*i32(ent->state_time*fps));
	f32 angle_three = degrees_to_radians(225.0+2*i32(ent->state_time*fps));
	f32 angle_four = degrees_to_radians(315.0+2*i32(ent->state_time*fps));

	f32 distance_x = 12+(.5*ent->sprite_index*12);
	f32 distance_y = 20+(.5*ent->sprite_index*20);

	//cos = x
	//sin = y

	Vector2 center = ent->position;

	DrawImage(ent->image[0], v2(ent->position.x+distance_x*Cos(angle_one)+camera_offset,
		ent->position.y+Sin(angle_one)*distance_y));

	DrawImage(ent->image[0], v2(ent->position.x+distance_x*Cos(angle_two)+camera_offset,
		ent->position.y+Sin(angle_two)*distance_y));

	DrawImage(ent->image[0], v2(ent->position.x+distance_x*Cos(angle_three)+camera_offset,
		ent->position.y+Sin(angle_three)*distance_y));

	DrawImage(ent->image[0], v2(ent->position.x+distance_x*Cos(angle_four)+camera_offset,
		ent->position.y+Sin(angle_four)*distance_y));
}