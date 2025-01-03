const i32 TYPESNOW = 0;

i32 snow_count = 5000;
Particle snow[5000] = {};

i32 get_open_snowflake() {
	for (int i = 0; i < snow_count; i++) {
		if (!snow[i].is_alive) {
			return i;
		}
	}
	return 5001;
}

void spawn_initial_snowflakes(Game_Output *out, Game_Input *input) {
	for (int i = 0; i < 1600; i++) {
		f32 x = random_f32_between(0, 1)*(out->width+9000);
		f32 y = random_f32_between(0, 1)*(out->height-200);
		i32 snowflake = get_open_snowflake();
		if (snowflake != 5001) {
			snow[snowflake].is_alive = true;
			snow[snowflake].position = v2(x, y);
			snow[snowflake].velocity = v2(40*input->dt, 40*input->dt);
			snow[snowflake].image = LoadImage(S("snow_flake.png"));
			snow[snowflake].depth = random_f32_between(.6, .8);
			snow[snowflake].type = TYPESNOW;
		}
	}
}

void spawn_snowflakes(Game_Output *out, Game_Input *input) {
	
	for (int i = 0; i < 2; i++) {
		f32 x = random_f32_between(0, 1)*(out->width+9000);
		i32 snowflake = get_open_snowflake();
		if (snowflake != 5001) {
			snow[snowflake].is_alive = true;
			snow[snowflake].position = v2(x, -6);
			snow[snowflake].velocity = v2(40*input->dt, 40*input->dt);
			snow[snowflake].image = LoadImage(S("snow_flake.png"));
			snow[snowflake].depth = random_f32_between(.6, .8);
			snow[snowflake].type = TYPESNOW;
		}
	}
	
	
}

void update_snowflakes(Game_Output *out, i32 offset) {
	for (int i = 0; i < snow_count; i++) {
		if (snow[i].is_alive) {
			snow[i].position.x-=snow[i].velocity.x;
			snow[i].position.y+=snow[i].velocity.y;
		}

		if (snow[i].position.y > out->height || snow[i].position.x < -20) {
			snow[i].is_alive = false;
		} else {
			DrawImage(snow[i].image, v2((snow[i].position.x+Sin(snow[i].position.y/100.0+i/8.0)*25)-(offset+out->width*.5)*snow[i].depth, snow[i].position.y));
		}
	}
}

void generate_drips(Vector2 pos, Image drip, Game_Input *input) {
	Particle_Parameters min = {};
	Particle_Parameters max = {};

	min.position.x = pos.x;
	min.position.y = pos.y;

	max.position.x = pos.x+12;
	max.position.y = pos.y+12;

	min.velocity.x = 0;
	min.velocity.y = 150;

	max.velocity.x = 0;
	max.velocity.y = 200;

	min.life_time = 40*input->dt;
	max.life_time = 50*input->dt;

	particle_emit(min, max, drip);
}