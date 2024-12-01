struct Level {
    Image world;
    Image background;
    Rectangle2 entry_points;
    i32 id;
    Vector2 landing_pos;
};

Level village = {};
Level scram_sewers_entry = {};

void create_levels() {
    static Image village_world = LoadImage(S("village.png"));
    static Image village_bg = LoadImage(S("factory_background.png"));
    static Image scram_sewers_world = LoadImage(S("scrap_metal_cave.png"));
    static Image scram_sewers_bg = LoadImage(S("scram_sewers_bg.png"));

    village.world = village_world;
    village.background = village_bg;
    village.entry_points = r2_bounds(v2(9552, 436), v2(48, 96), v2_zero, v2_one);
    village.id = 0;
    village.landing_pos = v2(9504, 476);

    scram_sewers_entry.world = scram_sewers_world;
    scram_sewers_entry.background = scram_sewers_bg;
    scram_sewers_entry.entry_points = r2_bounds(v2(480, 480), v2(48, 96), v2_zero, v2_one);
    scram_sewers_entry.id = 1;
    scram_sewers_entry.landing_pos = v2(528, 524);
}