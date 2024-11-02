const i32 TYPESNOW = 0;

struct Particle {
	Image image;
	Vector2 velocity;
	Vector2 position;
	bool is_alive;
	f32 depth;
	i32 direction;
	f32 life_time;
	i32 type;
};

struct Particle_Parameters {
	Vector2 velocity;
	Vector2 position;
	Vector4 color;
	f32 life_time;
	Vector2 accel;
	f32 friction;
};

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

void spawn_snowflakes(Game_Output *out, Game_Input *input) {
	/*for (int i = 0; i < (out->width+3000)/12; i++) {
		
			if (random_f32_between(0, 1) < .005) {

				i32 snowflake = get_open_snowflake();
				if (snowflake != 5001) {
					snow[snowflake].is_alive = true;
					snow[snowflake].position = v2(i*12, -6);
					snow[snowflake].velocity = v2(40*input->dt, 40*input->dt);
					snow[snowflake].image = LoadImage(S("snow_flake.png"));
					snow[snowflake].depth = random_f32_between(.6, .8);
				}

			
		}
	}*/

	for (int i = 0; i < 2; i++) {
		f32 x = random_f32_between(0, 1)*(out->width+4000);
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