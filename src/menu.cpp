void draw_menu(Game_Output *out, Entity *player, Image screen) {
	Rectangle2 outline = r2_bounds(v2(16, 16), v2(264, out->height-32), v2_zero, v2_one);
	Rectangle2 pengu = r2_bounds(v2(20, 20), v2(256, 256), v2_zero, v2_one);
	Rectangle2 middle = r2_bounds(v2(20, 276), v2(256, 4), v2_zero, v2_one);
	Rectangle2 stats_block = r2_bounds(v2(20, 280), v2(256, 420), v2_zero, v2_one);

	i32 xp = i32(240*(f32(player->exp_gained)/f32(player->exp_to_level)));
	xp = Clamp(xp, 0, 240);

	Rectangle2 level_bar = r2_bounds(v2(24, 254), v2(240, 18), v2_zero, v2_one);
	Rectangle2 level_act = r2_bounds(v2(24, 254), v2(xp, 18), v2_zero, v2_one);

	static Image image = LoadImage(S("menu_sprite.png"));

	String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
    Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

    String constitution = S("Constitution");
    String rigour = S("Rigour");
    String strength = S("Strength");
    String dexterity = S("Dexterity");
    String mental = S("Mental");
    String level = S("< - * + >");

    String player_lvl = S("LV.1");

    //Draw Screen outline
    DrawImage(screen, v2(0, 0));

	//DrawRect(outline, v4_white);
	DrawRectOutline(outline, v4_white, 4);
	DrawRect(middle, v4_white);
	DrawRect(pengu, v4_black);
	DrawRect(stats_block, v4_black);

	DrawImage(image, v2(20, 20));
	DrawTextExt(font_hellomyoldfriend, player_lvl, v2(24, 226), v4_white, v2_zero, 2.0);
	DrawRect(level_bar, v4_white);
	DrawRect(level_act, v4_green);

	DrawTextExt(font_hellomyoldfriend, constitution, v2(24, 284), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, rigour, v2(24, 356), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, strength, v2(24, 428), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, dexterity, v2(24, 500), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, mental, v2(24, 572), v4_white, v2_zero, 1.5);

	Rectangle2 con_bar = r2_bounds(v2(28+240*(f32(player->constitution)/100.0), 308), v2(240-240*(f32(player->constitution)/100.0), 18), v2_zero, v2_one);
	Rectangle2 rig_bar = r2_bounds(v2(28+240*(f32(player->rigour)/100.0), 380), v2(240-240*(f32(player->rigour)/100.0), 18), v2_zero, v2_one);
	Rectangle2 str_bar = r2_bounds(v2(28+240*(f32(player->strength)/100.0), 452), v2(240-240*(f32(player->strength)/100.0), 18), v2_zero, v2_one);
	Rectangle2 dex_bar = r2_bounds(v2(28+240*(f32(player->dexterity)/100.0), 524), v2(240-240*(f32(player->dexterity)/100.0), 18), v2_zero, v2_one);
	Rectangle2 men_bar = r2_bounds(v2(28+240*(f32(player->mental)/100.0), 596), v2(240-240*(f32(player->mental)/100.0), 18), v2_zero, v2_one);

	Rectangle2 act_con_bar = r2_bounds(v2(28, 308), v2(240*(f32(player->constitution)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_rig_bar = r2_bounds(v2(28, 380), v2(240*(f32(player->rigour)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_str_bar = r2_bounds(v2(28, 452), v2(240*(f32(player->strength)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_dex_bar = r2_bounds(v2(28, 524), v2(240*(f32(player->dexterity)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_men_bar = r2_bounds(v2(28, 596), v2(240*(f32(player->mental)/100.0), 18), v2_zero, v2_one);

	DrawTextExt(font_hellomyoldfriend, level, v2(36, 332), v4_white, v2_zero, 2.0);
	DrawTextExt(font_hellomyoldfriend, level, v2(36, 404), v4_white, v2_zero, 2.0);
	DrawTextExt(font_hellomyoldfriend, level, v2(36, 476), v4_white, v2_zero, 2.0);
	DrawTextExt(font_hellomyoldfriend, level, v2(36, 548), v4_white, v2_zero, 2.0);
	DrawTextExt(font_hellomyoldfriend, level, v2(36, 620), v4_white, v2_zero, 2.0);

	DrawRect(con_bar, v4_white);
	DrawRect(rig_bar, v4_white);
	DrawRect(str_bar, v4_white);
	DrawRect(dex_bar, v4_white);
	DrawRect(men_bar, v4_white);

	DrawRect(act_con_bar, v4_red);
	DrawRect(act_rig_bar, v4_red);
	DrawRect(act_str_bar, v4_red);
	DrawRect(act_dex_bar, v4_red);
	DrawRect(act_men_bar, v4_red);
}