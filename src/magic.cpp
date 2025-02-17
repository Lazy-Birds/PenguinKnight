Entity spells[100] = {};
i32 spell_count = 0;

SpellArray get_spell_array(Arena *arena, i32 capacity) {
	SpellArray arr = {};
	arr.data = PushArrayZero(arena, Spell, capacity);
	arr.capacity = capacity;
	arr.count = 0;

	return arr;
}

void give_player_basic_spells() {
	player.spell_list.data[0] = load_spells(sp_magic_missile);
	player.spell_list.data[1] = load_spells(sp_flame_ball);
	player.spell_list.data[2] = load_spells(sp_flame_wheel);
	player.spell_list.data[3] = load_spells(sp_flame_rain);
	player.spell_list.count = 3;
}

Spell load_spells(i32 spell_type) {
	switch (spell_type)
	{
	case sp_magic_missile:
		{
			Spell spell = {};
			spell.type = sp_magic_missile;
			spell.mp_cost = 0;

			return spell;
		} break;
	case sp_flame_wheel:
		{
			static Image icon = LoadImage(S("flame_wheel_icon.png"));

			Spell spell = {};
			spell.icon = icon;
			spell.type = sp_flame_wheel;
			spell.mp_cost = 10;

			return spell;
		} break;
	case sp_flame_rain:
		{
			static Image icon = LoadImage(S("flame_portal_icon.png"));

			Spell spell = {};
			spell.icon = icon;
			spell.type = sp_flame_rain;
			spell.mp_cost = 12;

			return spell;
		} break;
	case sp_flame_ball:
		{
			static Image icon = LoadImage(S("flame_ball_icon.png"));

			Spell spell = {};
			spell.icon = icon;
			spell.type = sp_flame_ball;
			spell.mp_cost = 8;

			return spell;
		} break;
	default:
		{
			return {};
		} break;
	}
}

i32 get_spell_slot() {

	for (int i = 0; i < 99; i++)
	{
		if (!spells[i].alive) return i;
	}

	return 100;
}

void magic_emit(i32 spell_type) {
	switch (spell_type)
	{
	case sp_magic_missile:
		{
			static Image image[] = {
				LoadImage(S("magic_missile1.png")),
				LoadImage(S("magic_missile2.png")),
			};

			i32 spell_slot = get_spell_slot();

			assert(spell_slot < 100);

			Entity *spell = &spells[spell_slot];

			spell->alive = true;
			spell->state_time = 0;
			spell->sprite_index = 0;
			spell->type = spell_type;
			spell->id = spell_count;
			spell->facing = player.facing;
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

			for (int i = 0; i < 4; i++) {

				i32 spell_slot = get_spell_slot();

				assert(spell_slot < 100);

				Entity *spell = &spells[spell_slot];

				spell->alive = true;
				spell->state_time = 0;
				spell->sprite_index = 0;
				spell->type = spell_type;
				spell->id = spell_count;
				spell->facing = player.facing;
				spell->image = image;
				spell->position = v2(player.position.x+28*sign_f32(player.facing), player.position.y+15);
				spell->size = v2(30, 30);
				spell->hitbox = hitbox_simple(spell);
				spell->velocity = v2(10000*input->dt*sign_f32(player.facing), 0);
				spell->sprite_index = 0;
				spell->state = st_ball_one;
				spell->damage = 35;
				spell->state = i;
				spell_count++;
			}
		} break;
	case sp_flame_rain:
		{
			static Image image[] = {
				LoadImage(S("flame_ball1.png")),
				LoadImage(S("flame_ball2.png")),
				LoadImage(S("flame_ball3.png")),
			};

			i32 spell_slot = get_spell_slot();

			assert(spell_slot < 100);

			Entity *spell = &spells[spell_slot];

			spell->alive = true;
			spell->state_time = 0;
			spell->sprite_index = 0;
			spell->type = spell_type;
			spell->id = spell_count;
			spell->facing = player.facing;
			spell->image = image;
			spell->position = v2(player.position.x, player.position.y - 65);
			spell->velocity = v2(10000*input->dt*sign_f32(player.facing),
				random_f32_between(1000, 8000)*input->dt);
			spell->size = v2(30, 30);
			spell->damage = 35;
			spell->hitbox = hitbox_simple(spell);
			spell->sprite_index = 0;
			spell_count++; 

		} break;
	case sp_flame_ball:
		{
			static Image image[] = {
				LoadImage(S("flame_ball1.png")),
				LoadImage(S("flame_ball2.png")),
				LoadImage(S("flame_ball3.png")),
			};

			i32 spell_slot = get_spell_slot();

			assert(spell_slot < 100);

			Entity *spell = &spells[spell_slot];

			spell->alive = true;
			spell->state_time = 0;
			spell->sprite_index = 0;
			spell->type = spell_type;
			spell->id = spell_count;
			spell->facing = player.facing;
			spell->image = image;
			spell->position = v2(player.position.x+28*sign_f32(player.facing), player.position.y+15);
			spell->size = v2(30, 30);
			spell->hitbox = hitbox_simple(spell);
			spell->velocity = v2(10000*input->dt*sign_f32(player.facing), 0);
			spell->damage = 35;
			spell->sprite_index = 0;
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
		case sp_flame_wheel:
			{
				spell->state_time+=input->dt;

				f32 angle = degrees_to_radians(45+90*spell->state+i32(spell->state_time*fps));

				f32 distance_x = 12+(.5*spell->sprite_index*12);
				f32 distance_y = 20+(.5*spell->sprite_index*20);

				move_entity(spell, input->dt);
				spell->hitbox = hitbox_simple(spell);
				spell->hitbox = r2_shift(spell->hitbox, v2(Cos(angle)*distance_x, Sin(angle)*distance_y));

				DrawImage(spell->image[spell->sprite_index], 
					v2(spell->hitbox.x0+camera_offset, spell->hitbox.y0));

				if (i32(spell->state_time*fps)%8 == 0) {
					spell->sprite_index == 0 ? spell->sprite_index = 1 : spell->sprite_index = 0;
				}

				Entity *enemy = enemy_hit(spell->hitbox);

				if (enemy!=nullptr)
				{
					entity_take_damage(enemy, spell);
					enemy->invuln = false;
					spell->alive = false;
				}

				if (wall_ahead(spell) || spell->state_time*fps > 160) spell->alive = false;
			} break;
		case sp_flame_rain:
			{
				spell->state_time+=input->dt;

				move_entity(spell, input->dt);

				spell->hitbox = hitbox_simple(spell);

				if (i32(spell->state_time*fps)%8 == 0)
				{
					spell->sprite_index == 0 ? spell->sprite_index = 1 : spell->sprite_index = 0;
				}
				 

				draw_entity(spell, spell->sprite_index);

				Entity *enemy = enemy_hit(spell->hitbox);

				if (enemy!=nullptr)
				{
					entity_take_damage(enemy, spell);
					enemy->invuln = false;
					spell->alive = false;
				}

				if (wall_ahead(spell) || spell->state_time*fps > 160 || entity_on_wall(spell))
					spell->alive = false;
			} break;
		case sp_flame_ball:
			{
				spell->state_time+=input->dt;

				move_entity(spell, input->dt);

				spell->hitbox = hitbox_simple(spell);

				if (i32(spell->state_time*fps)%8 == 0)
				{
					spell->sprite_index == 0 ? spell->sprite_index = 1 : spell->sprite_index = 0;
				}

				draw_entity(spell, spell->sprite_index);

				Entity *enemy = enemy_hit(spell->hitbox);

				if (enemy!=nullptr)
				{
					entity_take_damage(enemy, spell);
					enemy->invuln = false;
					spell->alive = false;
				}

				if (wall_ahead(spell) || spell->state_time*fps > 240)
					spell->alive = false;
			} break;
		}
	}
}

