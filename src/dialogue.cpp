void load_fairy_dialogue(Entity *fairy) {
	static String dialogue[2] = {
		S("Hey Penguin Knight Listen!"),
		S("A great adventure awaits you!"),
	};

	fairy->dialogue = dialogue;
}

void load_penguin_king_dialogue(Entity *pengu) {
	static String dialogue[] = {
		S("You dare approach me!"),
		S("Guards! Execute him."),
		S("Fine, I'll do it myself!"),
	};

	pengu->dialogue = dialogue;
}

void draw_dialogue_box(String words, Game_Output *out, Image *image, i32 frames, f32 dialogue_time) {
	String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
    Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

    Vector2 size = MeasureText(font_hellomyoldfriend, words);

    String words2 = string_slice(words, 0, dialogue_time/8);

    Rectangle2 box1 = r2_bounds(v2(96, out->height-96), v2(out->width-96, 96), v2_zero, v2_one);
    Rectangle2 box2 = r2_bounds(v2(96+4, out->height-96+4), v2(out->width-96-8, 96-8), v2_zero, v2_one);

    Rectangle2 box3 = r2_bounds(v2(0, out->height-96), v2(96, 96), v2_zero, v2_one);
    Rectangle2 box4 = r2_bounds(v2(4, out->height-92), v2(92, 88), v2_zero, v2_one);

    DrawRect(box1, v4_white);
    DrawRect(box2, v4_black);
    DrawRect(box3, v4_white);
    DrawRect(box4, v4_black);

    DrawTextExt(font_hellomyoldfriend, words2, v2(96+12, out->height-96+12), v4_white, v2_zero, 2.0);

    if (i32(dialogue_time)%90 < 45) {
    	DrawImage(image[0], v2(1, out->height-96));
    } else {
    	DrawImage(image[1], v2(1, out->height-96));
    }
}