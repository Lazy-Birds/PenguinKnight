#include <math.h>

i32 mouse_over_which_rec(Rectangle2 rec1, Rectangle2 rec2) {
	Vector2 pos = MousePosition();

	if (pos.x > rec1.x0 && pos.x < rec1.x0 + (rec1.x1-rec1.x0) && pos.y > rec1.y0 && pos.y < rec1.y0 + (rec1.y1-rec1.y0)) {
		return 1;
	} else if (pos.x > rec2.x0 && pos.x < rec2.x0 + (rec2.x1-rec2.x0) && pos.y > rec2.y0 && pos.y < rec2.y0 + (rec2.y1-rec2.y0)) {
		return 2;
	} else {
		return 0;
	}
}

i32 mouse_on_rec(Rectangle2 rec) {
	Vector2 pos = MousePosition();

	if (pos.x > rec.x0 && pos.x < rec.x0 + (rec.x1-rec.x0) && pos.y > rec.y0 && pos.y < rec.y0 + (rec.y1-rec.y0)) {
		return 1;
	} else {
		return 0;
	}
}

const i32 con = 0;
const i32 rig = 1;
const i32 str = 2;
const i32 dex = 3;
const i32 men = 4;

void level_char_up(Entity *player, i32 stat) {
	i32 exp_to_sub = player->exp_to_level;

	switch (stat)
	{
	case con:
		{
			player->constitution++;
			player->max_health = 10*player->constitution;
		} break;
	case rig:
		{
			player->rigour++;
			player->max_stamina = 10*player->rigour;
		} break;
	case str:
		{
			player->strength++;
		} break;
	case dex:
		{
			player->dexterity++;
		} break;
	case men:
		{
			player->mental++;
		} break;
	}

	switch (player->level)
	{
	case 1:
		{
			player->exp_to_level = 650;
		} break;
	case 2:
		{
			player->exp_to_level = 680;
		} break;
	case 3:
		{
			player->exp_to_level = 720;
		} break;
	case 4:
		{
			player->exp_to_level = 760;
		} break;
	case 5:
		{
			player->exp_to_level = 800;
		} break;
	case 6:
		{
			player->exp_to_level = 840;
		} break;
	case 7:
		{
			player->exp_to_level = 880;
		} break;
	case 8:
		{
			player->exp_to_level = 920;
		} break;
	case 9:
		{
			player->exp_to_level = 940;
		} break;
	default:
		{
			player->exp_to_level = i32(0.02*(powf(f32(player->level), 3.0) + 3.06*(powf(f32(player->level), 2.0) +105.6*(player->level)-800.0)));
		} break;
	}

	player->level++;
	player->exp_gained-=exp_to_sub;
}

i32 box_selected = 0;
i32 start_select = 0;
Vector2 set_select = v2(0, 16);


void item_menu() {
	for (int i = set_select.x; i < set_select.y; i++) {

		Rectangle2 it_box = r2_bounds(v2(320, 80+36*(i-start_select)), v2(96, 32), v2_zero, v2_one);

		if (box_selected == i) {
			DrawRectOutline(it_box, v4_white, 2);
		} else {
			DrawRectOutline(it_box, rgb(139, 155, 180), 2);
		}
	}

	String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
    Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

	for (int i = set_select.x; i < set_select.y; i++) {

		//Dump(player.inventory.data[i].data[0].name);
		DrawTextExt(font_hellomyoldfriend, player.inventory.data[i].data[0].name, v2(324, 88+36*(i-start_select)), v4_white, v2_zero, 1.0);

		clip_strings(player.inventory.data[i].data[0].description, 41);
	}
}

const Rectangle2 dec_con = r2_bounds(v2(65, 334), v2(49, 18), v2_zero, v2_one);
const Rectangle2 dec_rig = r2_bounds(v2(65, 406), v2(49, 18), v2_zero, v2_one);
const Rectangle2 dec_str = r2_bounds(v2(65, 476), v2(49, 18), v2_zero, v2_one);
const Rectangle2 dec_dex = r2_bounds(v2(65, 550), v2(49, 18), v2_zero, v2_one);
const Rectangle2 dec_men = r2_bounds(v2(65, 622), v2(49, 18), v2_zero, v2_one);

const Rectangle2 inc_con = r2_bounds(v2(217, 334), v2(49, 18), v2_zero, v2_one);
const Rectangle2 inc_rig = r2_bounds(v2(217, 406), v2(49, 18), v2_zero, v2_one);
const Rectangle2 inc_str = r2_bounds(v2(217, 476), v2(49, 18), v2_zero, v2_one);
const Rectangle2 inc_dex = r2_bounds(v2(217, 550), v2(49, 18), v2_zero, v2_one);
const Rectangle2 inc_men = r2_bounds(v2(217, 622), v2(49, 18), v2_zero, v2_one);

i32 menu_at = 0;

void draw_menu(Game_Output *out, Entity *player, Image screen, bool at_bonfire) {
	TimeFunction;
	Rectangle2 outline = r2_bounds(v2(16, 16), v2(264, out->height-32), v2_zero, v2_one);
	Rectangle2 pengu = r2_bounds(v2(20, 20), v2(256, 256), v2_zero, v2_one);
	Rectangle2 middle = r2_bounds(v2(20, 276), v2(256, 4), v2_zero, v2_one);
	Rectangle2 stats_block = r2_bounds(v2(20, 280), v2(256, 420), v2_zero, v2_one);

	//Menu Items
	Rectangle2 first = r2_bounds(v2(332, 44), v2(80, 16), v2_zero, v2_one);
	Rectangle2 second = r2_bounds(v2(420, 44), v2(80, 16), v2_zero, v2_one);
	Rectangle2 third = r2_bounds(v2(508, 44), v2(80, 16), v2_zero, v2_one);
	Rectangle2 fourth = r2_bounds(v2(596, 44), v2(80, 16), v2_zero, v2_one);

	String items = S("Inventory");
	String equipment = S("Equipment");
	String characters = S("Characters");
	String settings = S("Settings");

	i32 xp = i32(240*(f32(player->exp_gained)/f32(player->exp_to_level)));
	xp = Clamp(xp, 0, 240);

	Rectangle2 level_bar = r2_bounds(v2(44, 258), v2(240, 18), v2_zero, v2_one);
	Rectangle2 level_act = r2_bounds(v2(44, 258), v2(xp, 18), v2_zero, v2_one);

	String exp = string_concat(sprint("%d", player->exp_gained), S("/"));
	exp = string_concat(exp, sprint("%d", player->exp_to_level));

	static Image big_icon_bonfire = LoadImage(S("sleeping_bag_pengu.png"));
	static Image big_icon_out = LoadImage(S("menu_sprite.png"));

	String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
    Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

    String constitution = S("Constitution");
    String rigour = S("Rigour");
    String strength = S("Strength");
    String dexterity = S("Dexterity");
    String mental = S("Mental");

    String player_lvl = sprint("%d", player->level);
    
    Image level_up[] = {
    	LoadImage(S("level_up1.png")),
    	LoadImage(S("level_up2.png")),
    	LoadImage(S("level_up3.png")),
    };

    //Load Menu image
    Image menu_screen[] = {
    	LoadImage(S("character_menu1.png")),
    	LoadImage(S("character_menu2.png")),
    	LoadImage(S("character_menu3.png")),
    	LoadImage(S("character_menu4.png")),
    };

    Image menu_icons[] = {
    	LoadImage(S("menu_icons1.png")),
    	LoadImage(S("menu_icons2.png")),
    	LoadImage(S("menu_icons3.png")),
    	LoadImage(S("menu_icons4.png")),
    };

    //Draw Screen outline
    DrawImage(menu_screen[menu_at], v2(0, 0));

    //Change box select with directional input
    if (ControllerReleased(0, Button_Up) && box_selected > 0) 
	{
		box_selected--;
	} else if (ControllerReleased(0, Button_Down) && box_selected < 99)
	{
		box_selected++;
	}

	//change set select
	if (box_selected == set_select.y && box_selected < 100)
	{
		set_select.x++;
		set_select.y++;
		start_select++;
	} else if (box_selected == set_select.x && box_selected > 0) 
	{
		set_select.x--;
		set_select.y--;
		start_select--;
	}

    if (menu_at == 0) {
    	item_menu();
    }

    //Draw menu icons
    DrawImage(menu_icons[0], v2(364, 44));
    DrawImage(menu_icons[1], v2(452, 44));
    DrawImage(menu_icons[2], v2(540, 44));
    DrawImage(menu_icons[3], v2(628, 44));


	//DrawRect(outline, v4_white);
	//DrawRectOutline(outline, v4_white, 4);
	//DrawRect(middle, v4_white);
	//DrawRect(pengu, v4_black);
	//DrawRect(stats_block, v4_black);

    //Draw Menu Icon
    if (at_bonfire) {
    	DrawImage(big_icon_bonfire, v2(44, 28));
    } else {
    	DrawImage(big_icon_out, v2(44, 28));
    }
	
	DrawTextExt(font_hellomyoldfriend, player_lvl, v2(44, 234), v4_white, v2_zero, 2.0);
	DrawRect(level_bar, v4_white);
	DrawRect(level_act, v4_green);

	DrawTextExt(font_hellomyoldfriend, exp, v2(46, 258), v4_red, v2_zero, 1.2);

	DrawTextExt(font_hellomyoldfriend, constitution, v2(40, 284), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, rigour, v2(40, 356), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, strength, v2(40, 428), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, dexterity, v2(40, 500), v4_white, v2_zero, 1.5);
	DrawTextExt(font_hellomyoldfriend, mental, v2(40, 572), v4_white, v2_zero, 1.5);

	Rectangle2 con_bar = r2_bounds(v2(44+240*(f32(player->constitution)/100.0), 308), v2(240-240*(f32(player->constitution)/100.0), 18), v2_zero, v2_one);
	Rectangle2 rig_bar = r2_bounds(v2(44+240*(f32(player->rigour)/100.0), 380), v2(240-240*(f32(player->rigour)/100.0), 18), v2_zero, v2_one);
	Rectangle2 str_bar = r2_bounds(v2(44+240*(f32(player->strength)/100.0), 452), v2(240-240*(f32(player->strength)/100.0), 18), v2_zero, v2_one);
	Rectangle2 dex_bar = r2_bounds(v2(44+240*(f32(player->dexterity)/100.0), 524), v2(240-240*(f32(player->dexterity)/100.0), 18), v2_zero, v2_one);
	Rectangle2 men_bar = r2_bounds(v2(44+240*(f32(player->mental)/100.0), 596), v2(240-240*(f32(player->mental)/100.0), 18), v2_zero, v2_one);

	Rectangle2 act_con_bar = r2_bounds(v2(44, 308), v2(240*(f32(player->constitution)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_rig_bar = r2_bounds(v2(44, 380), v2(240*(f32(player->rigour)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_str_bar = r2_bounds(v2(44, 452), v2(240*(f32(player->strength)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_dex_bar = r2_bounds(v2(44, 524), v2(240*(f32(player->dexterity)/100.0), 18), v2_zero, v2_one);
	Rectangle2 act_men_bar = r2_bounds(v2(44, 596), v2(240*(f32(player->mental)/100.0), 18), v2_zero, v2_one);

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

	if (mouse_on_rec(first) == 1 && MouseReleased(Mouse_Left)) {
		menu_at = 0;
	} else if (mouse_on_rec(second) == 1 && MouseReleased(Mouse_Left)) {
		menu_at = 1;
	} else if (mouse_on_rec(third) == 1 && MouseReleased(Mouse_Left)) {
		menu_at = 2;
	} else if (mouse_on_rec(fourth) == 1 && MouseReleased(Mouse_Left)) {
		menu_at = 3;
	}

	if (at_bonfire) {
		DrawImage(level_up[mouse_over_which_rec(dec_con, inc_con)], v2(65, 332));
		DrawImage(level_up[mouse_over_which_rec(dec_rig, inc_rig)], v2(65, 404));
		DrawImage(level_up[mouse_over_which_rec(dec_str, inc_str)], v2(65, 476));
		DrawImage(level_up[mouse_over_which_rec(dec_dex, inc_dex)], v2(65, 548));
		DrawImage(level_up[mouse_over_which_rec(dec_men, inc_men)], v2(65, 620));

		if (mouse_over_which_rec(dec_con, inc_con) == 2 && MouseReleased(Mouse_Left) && player->exp_to_level <= player->exp_gained) {
			level_char_up(player, 0); 
		} else if (mouse_over_which_rec(dec_rig, inc_rig) == 2 && MouseReleased(Mouse_Left) && player->exp_to_level <= player->exp_gained) {
			level_char_up(player, 1);
		} else if (mouse_over_which_rec(dec_str, inc_str) == 2 && MouseReleased(Mouse_Left) && player->exp_to_level <= player->exp_gained) {
			level_char_up(player, 2);
		} else if (mouse_over_which_rec(dec_dex, inc_dex) == 2 && MouseReleased(Mouse_Left) && player->exp_to_level <= player->exp_gained) {
			level_char_up(player, 3);
		} else if (mouse_over_which_rec(dec_men, inc_men) == 2 && MouseReleased(Mouse_Left) && player->exp_to_level <= player->exp_gained) {
			level_char_up(player, 4);
		}
	}
}