Weapon sword = {};
Weapon cleaver = {};

void load_weapon() {

    static Image sword_image[] = {
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

    cleaver.size = v2(139, 87), v2(149, 92);
    cleaver.weapon_frames = v2(4, 9);
    cleaver.name = S("Cleaver");
    cleaver.offset_right = v2(-42, -40);
    cleaver.offset_left = v2(-67, -40);
    cleaver.hit_size = v2(76, 81);
    cleaver.hit_offset_right = v2(20, 31);
    cleaver.hit_offset_left = v2(62, 31);
    cleaver.charged_frames = v2(9, 14);
    cleaver.base_damage = 30;
    cleaver.damage_attribute = S("Strength");
    cleaver.damage_multiplier = 1.5;
    cleaver.charge_time = 9;
    cleaver.jump_frame = v2(14, 15);
    cleaver.frame_hit = 2;
    cleaver.dash_frame = v2(16, 19);
    cleaver.icon = icon;
    cleaver.jump = v2(20,21);
    cleaver.guard = v2(22, 24);
}