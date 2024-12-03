String lines_to_speak[1000] = {};
i32 line_count = 0;

b32 lines_generated = false;

void load_fairy_dialogue(Entity *fairy) {
	static String dialogue[4] = {
		S("I haven't seen you around here lately penguin knight. You finally gonna fight the Pengu King?"),
		S("Hey Penguin Knight Listen!"),
		S("The Penguin King seeks the great"),
		S("power, and only you can stop him!"),
	};

	fairy->dialogue = dialogue;
}

void load_penguin_king_dialogue(Entity *pengu) {
	static String dialogue[2] = {
		S(" You dare approach me, Guards! Execute him."),
		S(" Fine, I'll do it myself!"),
	};

	pengu->dialogue = dialogue;
}

void clip_strings(String words) {
	i32 char_at[10] = {};
	i32 lines = 0;

	i32 count = 0;


	if (words.count <=35) {
		lines_to_speak[0] = words;
		line_count = 0;
	} else {
		for (int i = 0; i < words.count; i++) {
			if (words.data[i] == ' ') {
				if (i > lines*35+35) {
					lines++;
				}

				if (lines >= 1 && words.count-char_at[lines-1] < 35) 
				{
					char_at[lines] = words.count;
					break;
				}

				char_at[lines] = i;
			}
		}

		for (int i = 0; i , 10; i++) {
			if (char_at[i] == 0) break;
			if (i == 0) {
				lines_to_speak[i] = string_slice(words, 0, char_at[i]);
			} else {
				lines_to_speak[i] = string_slice(words, char_at[i-1] + 1, char_at[i]);
			}
			line_count = i;
		}
	}
	lines_generated = true;
}

i32 letter_time = 0;
i32 box_count = 0;

b32 draw_dialogue_box(String words, Game_Output *out, Image *image, i32 frames) {

    Vector2 size = MeasureText(font_hellomyoldfriend, words);

    if (!lines_generated) {
    	clip_strings(words);
    }

    Rectangle2 box1 = r2_bounds(v2(96, out->height-96), v2(out->width-96, 96), v2_zero, v2_one);
    Rectangle2 box2 = r2_bounds(v2(96+4, out->height-96+4), v2(out->width-96-8, 96-8), v2_zero, v2_one);

    Rectangle2 box3 = r2_bounds(v2(0, out->height-96), v2(96, 96), v2_zero, v2_one);
    Rectangle2 box4 = r2_bounds(v2(4, out->height-92), v2(92, 88), v2_zero, v2_one);

    DrawRect(box1, v4_white);
    DrawRect(box2, v4_black);
    DrawRect(box3, v4_white);
    DrawRect(box4, v4_black);

 	if (box_count%3 == 0) {
    	String words2 = string_slice(lines_to_speak[box_count], 0, letter_time/3);
    	DrawTextExt(font_hellomyoldfriend, words2, v2(96+12, out->height-96+12), v4_white, v2_zero, 2.0);
    } else if (box_count%3 == 1) {
    	DrawTextExt(font_hellomyoldfriend, lines_to_speak[box_count-1], v2(96+12, out->height-96+12), v4_white, v2_zero, 2.0);
    	String words2 = string_slice(lines_to_speak[box_count], 0, letter_time/3);
    	DrawTextExt(font_hellomyoldfriend, words2, v2(96+12, out->height-72+12), v4_white, v2_zero, 2.0);
    } else {
    	DrawTextExt(font_hellomyoldfriend, lines_to_speak[box_count-2], v2(96+12, out->height-96+12), v4_white, v2_zero, 2.0);
    	DrawTextExt(font_hellomyoldfriend, lines_to_speak[box_count-1], v2(96+12, out->height-72+12), v4_white, v2_zero, 2.0);
    	String words2 = string_slice(lines_to_speak[box_count], 0, letter_time/3);
    	DrawTextExt(font_hellomyoldfriend, words2, v2(96+12, out->height-48+12), v4_white, v2_zero, 2.0);
    }
    
    letter_time++;
    if (letter_time >= 180) {
    	letter_time = 0;
    	box_count++;
    } else if (letter_time >= 105 && box_count+1 < line_count && box_count%3 != 2) {
    	letter_time = 0;
    	box_count++;
    }

    if (letter_time%60 < 30) {
    	DrawImage(image[0], v2(1, out->height-96));
    } else {
    	DrawImage(image[1], v2(1, out->height-96));
    }

    if (box_count > line_count) {
		for (int i = 0; i < line_count; i++) {
			lines_to_speak[i] = {};
		}
		line_count = 0;
		box_count = 0;
		lines_generated = false;
		return true;
	} else {
		return false;
	}
}