Level World[2] = {};

Level village = {};
Level scram_sewers_entry = {};

Entity village_int[100] = {};
Entity scram_sewers_int[100] = {};

Entity village_walls[10000] = {};
Entity scram_sewers_walls[1000] = {};

Entity village_nme[100] = {};
Entity scram_sewers_nme[20] {};

Entity village_npc[50] = {};
Entity scram_sewers_npc[5] = {};

Entity village_bgs[1000] = {};
Entity scram_sewers_bgs[100] = {};

Entity village_hs[100] = {};

Entity village_fs[10] = {};


const i32 VILLAGE = 0;
const i32 SCRAMSEWERSENTRY = 1;

void create_levels() {
    static Image village_world = LoadImage(S("village_area2.png"));
    static Image village_bg = LoadImage(S("factory_background.png"));
    static Image scram_sewers_world = LoadImage(S("scram_entry2.png"));
    static Image scram_sewers_bg = LoadImage(S("scram_sewers_bg.png"));

    village.world = village_world;
    village.background = village_bg;
    village.entry_points = r2_bounds(v2(9264, 480), v2(48, 96), v2_zero, v2_one);
    village.id = 0;
    village.landing_pos = v2(9216, 524);
    village.interactible = village_int;
    village.interactible_count = 0;
    village.wall = village_walls;
    village.wall_count = 0;
    village.enemies = village_nme;
    village.enemy_count = 0;
    village.npcs = village_npc;
    village.npc_count = 0;
    village.backgrounds = village_bgs;
    village.bgnd_count = 0;
    village.housing = village_hs;
    village.fire = village_fs;
    village.initialized = false;

    scram_sewers_entry.world = scram_sewers_world;
    scram_sewers_entry.background = scram_sewers_bg;
    scram_sewers_entry.entry_points = r2_bounds(v2(432, 480), v2(48, 96), v2_zero, v2_one);
    scram_sewers_entry.id = 1;
    scram_sewers_entry.landing_pos = v2(480, 524);
    scram_sewers_entry.interactible = scram_sewers_int;
    scram_sewers_entry.interactible_count = 0;
    scram_sewers_entry.wall = scram_sewers_walls;
    scram_sewers_entry.wall_count = 0;
    scram_sewers_entry.enemies = scram_sewers_nme;
    scram_sewers_entry.enemy_count = 0;
    scram_sewers_entry.npcs = scram_sewers_npc;
    scram_sewers_entry.npc_count = 0;
    scram_sewers_entry.backgrounds = scram_sewers_bgs;
    scram_sewers_entry.bgnd_count = 0;
    scram_sewers_entry.initialized = false;

    World[0] = village;
    World[1] = scram_sewers_entry;
}

const i32 LEVER = 0;
const i32 GATE = 1;
const i32 GATETOO = 2;

void make_interactible(Vector2 position, i32 id, i32 level_id) {
    switch (id)
    {
    case LEVER:
        {
            static Image image[] = {
                LoadImage(S("lever1.png")),
                LoadImage(S("lever2.png")),
                LoadImage(S("lever3.png")),
            };

            World[level_id].interactible[World[level_id].interactible_count].position = position;
            World[level_id].interactible[World[level_id].interactible_count].size = v2(48, 48);
            World[level_id].interactible[World[level_id].interactible_count].image = image;
            World[level_id].interactible[World[level_id].interactible_count].type = 0;
            World[level_id].interactible[World[level_id].interactible_count].action_id = 0;
            World[level_id].interactible[World[level_id].interactible_count].actable = true;
            World[level_id].interactible[World[level_id].interactible_count].acting = false;

            World[level_id].interactible_count++;
        } break;
    case GATE:
        {
            static Image image[] = {
                LoadImage(S("gate1.png")),
            };

            World[level_id].interactible[World[level_id].interactible_count].position = position;
            World[level_id].interactible[World[level_id].interactible_count].size = v2(48, 48);
            World[level_id].interactible[World[level_id].interactible_count].image = image;
            World[level_id].interactible[World[level_id].interactible_count].type = 1;
            World[level_id].interactible[World[level_id].interactible_count].action_id = 1;
            World[level_id].interactible[World[level_id].interactible_count].actable = false;
            World[level_id].interactible[World[level_id].interactible_count].acting = false;

            World[level_id].interactible_count++;
        } break;
    case GATETOO:
        {
            static Image image[] = {
                LoadImage(S("gate2.png")),
            };

            World[level_id].interactible[World[level_id].interactible_count].position = position;
            World[level_id].interactible[World[level_id].interactible_count].size = v2(48, 48);
            World[level_id].interactible[World[level_id].interactible_count].image = image;
            World[level_id].interactible[World[level_id].interactible_count].type = 1;
            World[level_id].interactible[World[level_id].interactible_count].action_id = 1;
            World[level_id].interactible[World[level_id].interactible_count].actable = false;
            World[level_id].interactible[World[level_id].interactible_count].acting = false;

            World[level_id].interactible_count++;
        } break;
    }
}



void interact(Entity *interactible, Game_Input *input, Vector2 camera_pos) {
    switch (interactible->action_id)
    {
    case LEVER:
        {
            interactible->state_time+=input->dt;

            if (interactible->state_time*60 < 10)
            {
                DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            } else if (interactible->state_time*60 < 20)
            {
                DrawImage(interactible->image[1], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            } else if (interactible->state_time*60 < 30)
            {
                DrawImage(interactible->image[2], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            } else if (interactible->state_time*60 < 40) {
                DrawImage(interactible->image[1], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            } else {
                DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
                interactible->acting = false;
                interactible->state_time = 0;
                for (int i = 0; i < World[player.player_level].interactible_count; i++) {
                    if (World[player.player_level].interactible[i].action_id == 1) {
                        World[player.player_level].interactible[i].acting = true;
                        camera_state = CAMERALOCKED;
                        camera_pos_target = World[player.player_level].interactible[i].position;
                    }
                }
                interactible->acting = false;
                interactible->actable = false;
            }
        } break;
    case GATE:
        {
            interactible->state_time+=input->dt;
            if (i32(interactible->state_time*60)%4 == 0) {
                interactible->position = v2(interactible->position.x, interactible->position.y-2);
                DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
                Entity wall_at = get_wall_at(interactible->position);
                if (wall_at.position.x != 0) {
                    DrawImage(wall_at.image[0], v2(wall_at.position.x-camera_pos.x+out->width*.5, wall_at.position.y));
                }

                if (interactible->state_time*60 >= 182) {
                    interactible->acting = false;
                    camera_state = CAMERAFOLLOW;
                }
            } else {
                DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            }
        } break;
    }
}

