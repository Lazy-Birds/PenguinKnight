struct Weapon {
    Image *image;
    String name;

    Vector2 position;
    Vector2 size;

    i32 weapon_frames;
};

Weapon sword = {};

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
    sword.weapon_frames = 6;
    static String sname = S("Excalibrrr");
    sword.name = sname;
}

void draw_player(Weapon weapon, Vector2 position, i32 frame, i32 facing) {
    if (sign_i32(facing) > 0) {
        DrawImage(weapon.image[frame + weapon.weapon_frames], position);
    } else {
        DrawImage(weapon.image[frame], position);
    }
}

void weapon_attack() {

}