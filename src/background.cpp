struct Snowflake {
	Image image;
	Vector2 velocity;
	Vector2 position;
	bool is_alive;
	f32 depth;
};

i32 snow_count = 5000;
Snowflake snow[5000] = {};

i32 get_open_snowflake() {
	for (int i = 0; i < snow_count; i++) {
		if (!snow[i].is_alive) {
			return i;
		}
	}
	return 5001;
}

void spawn_snowflakes(Game_Output *out, Game_Input *input) {
			for (int i = 0; i < out->width+3000; i++) {
			if (i%12 == 0) {
				if (random_f32_between(0, 1) < .005) {
					i32 snowflake = get_open_snowflake();
					if (snowflake != 5001) {
						snow[snowflake].is_alive = true;
						snow[snowflake].position = v2(i, -6);
						snow[snowflake].velocity = v2(40*input->dt, 40*input->dt);
						snow[snowflake].image = LoadImage(S("snow_flake.png"));
						snow[snowflake].depth = random_f32_between(.6, .8);
					}

				}
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