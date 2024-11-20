struct Weapon {
    Image *image;
    String name;
    Image icon;

    Vector2 position;
    Vector2 size;

    Vector2 weapon_frames;
    Vector2 offset_right;
    Vector2 offset_left;

    Vector2 hit_size;
    Vector2 hit_offset_right;
    Vector2 hit_offset_left;
    Vector2 charged_frames;
    i32 charge_time;

    i32 base_damage;
    String damage_attribute;
    i32 damage_multiplier;
    i32 guard_damage;

    Vector2 jump_frame;
    Vector2 dash_frame;
    Vector2 jump;
    Vector2 guard;
    i32 frame_hit;
};

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

    static Image icon = LoadImage(S("cleaver_icon.png"));

    cleaver.image = cleaver_image;
    cleaver.size = v2(139, 87), v2(149, 92);
    cleaver.weapon_frames = v2(4, 9);
    cleaver.name = S("Cleaver");
    cleaver.offset_right = v2(-42, -40);
    cleaver.offset_left = v2(-67, -40);
    cleaver.hit_size = v2(71, 81);
    cleaver.hit_offset_right = v2(25, 31);
    cleaver.hit_offset_left = v2(57, 31);
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