Weapon sword = {};
Weapon cleaver = {};

enum fr_cleaver {
    fr_neutral,
    fr_walk,
    fr_damage,
    fr_atk_one,
    fr_atk_two,
    fr_atk_three,
    fr_atk_four,
    fr_atk_five,
    fr_atk_six,
    fr_atk_seven,
    fr_atk_eight,
    fr_atk_nine,
    fr_atk_ten,
    fr_atk_eleven,
    fr_atk_twelve,
    fr_atk_thirteen,
    fr_atk_fourteen,
    fr_atk_fifteen,
    fr_atk_sixteen,
    fr_chatk_one,
    fr_chatk_two,
    fr_chatk_three,
    fr_chatk_four,
    fr_chatk_five,
    fr_jmp_atk_one,
    fr_jmp_atk_two,
    fr_sprint_one,
    fr_sprint_two,
    fr_sprint_three,
    fr_sprint_four,
    fr_jmp_one,
    fr_jmp_two,
    fr_grd_one,
    fr_grd_two,
    fr_grd_three,
    fr_grd_four,
};

void load_weapon() {

    /*static Image sword_image[] = {
        LoadImage(S("Excalibrrr_left1.png")),
        LoadImage(S("Excalibrrr_left2.png")),
        LoadImage(S("Excalibrrr_left3.png")),
        LoadImage(S("Excalibrrr_left4.png")),
        LoadImage(S("Excalibrrr_left5.png")),
        LoadImage(S("Excalibrrr_left6.png")),
        LoadImage(S("Excalibrrr_right1.png")),
        LoadImage(S("Excalibrrr_right2.png")),
        LoadImage(S("Excalibrrr_right3.png")),
        LoadImage(S("Excalibrrr_right4.png")),
        LoadImage(S("Excalibrrr_right5.png")),
        LoadImage(S("Excalibrrr_right6.png")),
    };
    sword.image = sword_image;
    sword.size = v2(48, 96);
    sword.weapon_frames = v2(2, 6);
    String sname = S("Excalibrrr");
    sword.name = sname;
    */
    static Image cleaver_image[] = {
        LoadImage(S("cleaver1.png")),
        LoadImage(S("cleaver2.png")),
        LoadImage(S("cleaver3.png")),
        LoadImage(S("cleaver4.png")),
        LoadImage(S("cleaver5.png")),
        LoadImage(S("cleaver6.png")),
        LoadImage(S("cleaver7.png")),
        LoadImage(S("cleaver8.png")),
        LoadImage(S("cleaver9.png")),
        LoadImage(S("cleaver10.png")),
        LoadImage(S("cleaver11.png")),
        LoadImage(S("cleaver12.png")),
        LoadImage(S("cleaver13.png")),
        LoadImage(S("cleaver14.png")),
        LoadImage(S("cleaver15.png")),
        LoadImage(S("cleaver16.png")),
        LoadImage(S("cleaver17.png")),
        LoadImage(S("cleaver18.png")),
        LoadImage(S("cleaver19.png")),
        LoadImage(S("cleaver20.png")),
        LoadImage(S("cleaver21.png")),
        LoadImage(S("cleaver22.png")),
        LoadImage(S("cleaver23.png")),
        LoadImage(S("cleaver24.png")),
        LoadImage(S("cleaver25.png")),
        LoadImage(S("cleaver26.png")),
        LoadImage(S("cleaver27.png")),
        LoadImage(S("cleaver28.png")),
        LoadImage(S("cleaver29.png")),
        LoadImage(S("cleaver30.png")),
        LoadImage(S("cleaver31.png")),
        LoadImage(S("cleaver32.png")),
        LoadImage(S("cleaver33.png")),
        LoadImage(S("cleaver34.png")),
        LoadImage(S("cleaver35.png")),
    };

    cleaver.image = cleaver_image;

    static Image psn_img[] = {
        LoadImage(S("cleaver_poisoned1.png")),
        LoadImage(S("cleaver_poisoned2.png")),
        LoadImage(S("cleaver_poisoned3.png")),
        LoadImage(S("cleaver_poisoned4.png")),
    };

    cleaver.poisoned_image = psn_img;

    static Image icon = LoadImage(S("cleaver_icon.png"));
    static Image proj[] = {
        LoadImage(S("cleaver_charged1.png")),
        LoadImage(S("cleaver_charged2.png")),
        LoadImage(S("cleaver_charged3.png")),
        LoadImage(S("cleaver_charged4.png")),
        LoadImage(S("cleaver_charged5.png")),
    };

    cleaver.size = v2(139, 87), v2(149, 92);
    cleaver.weapon_frames = v2(4, 9);
    cleaver.type = pw_cleaver;
    cleaver.offset_right = v2(-42, -40);
    cleaver.offset_left = v2(-67, -40);
    cleaver.hit_size = v2(76, 81);
    cleaver.hit_offset_right = v2(20, 31);
    cleaver.hit_offset_left = v2(62, 31);
    cleaver.charged_frames = v2(9, 14);
    cleaver.base_damage = 30;
    cleaver.damage_attribute = at_strength;
    cleaver.damage_multiplier = 1.5;
    cleaver.charge_time = 9;
    cleaver.jump_frame = v2(14, 15);
    cleaver.frame_hit = 2;
    cleaver.dash_frame = v2(16, 19);
    cleaver.icon = icon;
    cleaver.jump = v2(20,21);
    cleaver.guard = v2(22, 24);
    cleaver.item_pickup = v2(25, 25);
    cleaver.sprite_index = 0;
    cleaver.projectile = proj;
}