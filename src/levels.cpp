const i32 world_size = 6;

const i32 wall_unit = 1000;
const i32 interactible_unit = 5;
const i32 enemy_unit = 5;
const i32 npc_unit = 5;
const i32 bgnd_unit = 50;
const i32 house_unit = 5;
const i32 liquid_unit = 10;

const i32 VILLAGE = 0;
const i32 SCRAMSEWERSENTRY = 1;

Level World[world_size] = {};

Arena *pengu_arena = NULL;

void create_levels() {

    World[0].name = S("Village");
    World[0].scale = 8;
    World[0].region = v2i(0, 0);
    World[1].name = S("SCRAM_Entrance");
    World[1].scale = 4;
    World[1].region = v2i(0, 1);
    World[2].name = S("SCRAM_Rest_One");
    World[2].scale = 2;
    World[2].region = v2i(0, 1);
    World[3].name = S("SCRAM_Interway_one");
    World[3].scale = 6;
    World[3].region = v2i(0, 1);
    World[4].name = S("SCRAM_Interway_two");
    World[4].scale = 3;
    World[4].region = v2i(0, 1);
    World[5].name = S("Boss_Room");
    World[5].scale = 3;
    World[5].region = v2i(0, 1);

    if (!pengu_arena){
        pengu_arena = arena_alloc(Gigabytes(1));
    } else {
        arena_reset(pengu_arena);
    }
    
    for (int i = 0; i < world_size; i++) {
        World[i].id = i;
        World[i].world = LoadImage(sprint("level%d.png", i));
        World[i].background = LoadImage(string_concat(sprint("region%d", World[i].region.x), sprint("area%d.png", World[i].region.y)));
        World[i].wall = make_entity_array_two(pengu_arena, wall_unit*World[i].scale);
        World[i].interactible = make_entity_array_two(pengu_arena, interactible_unit*World[i].scale);
        World[i].enemies = make_entity_array_two(pengu_arena, enemy_unit*World[i].scale);;
        World[i].npcs = make_entity_array_two(pengu_arena, npc_unit*World[i].scale);
        World[i].backgrounds = make_entity_array_two(pengu_arena, bgnd_unit*World[i].scale);
        World[i].housing = make_entity_array_two(pengu_arena, house_unit*World[i].scale);
        World[i].liquid = make_entity_array_two(pengu_arena, liquid_unit*World[i].scale);
        World[i].initialized = false;
    }

    create_level_entries();
}

const i32 LEVER = 0;
const i32 GATE = 1;
const i32 GATETOO = 2;
const i32 DOOR = 3;
const i32 FIRE = 4;
const i32 HOOK = 5;

void make_interactible(Vector2 position, i32 id, i32 level_id, i32 type) {
    switch (id)
    {
    case LEVER:
        {
            static Image image[] = {
                LoadImage(S("lever1.png")),
                LoadImage(S("lever2.png")),
                LoadImage(S("lever3.png")),
            };

            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].image = image;
            World[level_id].interactible.data[World[level_id].interactible.count].type = 0;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 0;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 0;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = true;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;

            World[level_id].interactible.count++;
        } break;
    case GATE:
        {
            static Image bot[] = {
                LoadImage(S("gate1.png")),
            };

            static Image top[] = {
                LoadImage(S("gate2.png")),
            };

            if (type == 0) {
                World[level_id].interactible.data[World[level_id].interactible.count].image = bot;
            } else {
                World[level_id].interactible.data[World[level_id].interactible.count].image = top;
            }

            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].type = type;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 1;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 1;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = false;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;
            World[level_id].interactible.data[World[level_id].interactible.count].state_time = 0;

            World[level_id].interactible.count++;
        } break;
    /*case GATETOO:
        {
            

            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].image = image;
            World[level_id].interactible.data[World[level_id].interactible.count].type = 1;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 1;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 1;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = false;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;
            World[level_id].interactible.data[World[level_id].interactible.count].state_time = 0;

            World[level_id].interactible.count++;
        } break;*/
    case DOOR:
        {
            static Image image[] = {
                LoadImage(S("pipe_door.png")),
            };

            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].image = image;
            World[level_id].interactible.data[World[level_id].interactible.count].type = 3;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 3;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 3;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = true;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;

            World[level_id].interactible.count++;
        } break;
    case FIRE:
        {
            
            static Image village_fire[] = {
                LoadImage(S("fire1.png")),
                LoadImage(S("fire2.png")),
                LoadImage(S("fire3.png")),
                LoadImage(S("fire4.png")),
                LoadImage(S("fire5.png")),
            };
          
            static Image sewer_fire[] = {
                LoadImage(S("fire_sewers1.png")),
                LoadImage(S("fire_sewers2.png")),
                LoadImage(S("fire_sewers3.png")),
                LoadImage(S("fire_sewers4.png")),
                LoadImage(S("fire_sewers5.png")),
            };

            if (level_id == 0) {
                World[level_id].interactible.data[World[level_id].interactible.count].image = village_fire;
            } else if (level_id == 2) {
                World[level_id].interactible.data[World[level_id].interactible.count].image = sewer_fire;
            }
            
            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].type = 4;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 4;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 4;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = true;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;

            World[level_id].interactible.count++;
        } break;
    case HOOK:
        {
            static Image right[] = {
                LoadImage(S("hook_right.png")),
            };

            static Image left[] = {
                LoadImage(S("hook_left.png")),
            };

            if (type == 0) {
                World[level_id].interactible.data[World[level_id].interactible.count].image = right;
                World[level_id].interactible.data[World[level_id].interactible.count].anchor = v2(position.x+48, position.y);
            } else {
                World[level_id].interactible.data[World[level_id].interactible.count].image = left;
                World[level_id].interactible.data[World[level_id].interactible.count].anchor = v2(position.x, position.y);
            }

            World[level_id].interactible.data[World[level_id].interactible.count].position = position;
            World[level_id].interactible.data[World[level_id].interactible.count].size = v2(48, 48);
            World[level_id].interactible.data[World[level_id].interactible.count].type = type;
            World[level_id].interactible.data[World[level_id].interactible.count].action_id = 5;
            World[level_id].interactible.data[World[level_id].interactible.count].id = 5;
            World[level_id].interactible.data[World[level_id].interactible.count].actable = false;
            World[level_id].interactible.data[World[level_id].interactible.count].acting = false;

            World[level_id].interactible.count++;
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
                for (int i = 0; i < World[player.player_level].interactible.count; i++) {
                    if (World[player.player_level].interactible.data[i].action_id == 1) {
                        World[player.player_level].interactible.data[i].acting = true;
                        camera_state = CAMERALOCKED;
                        camera_pos_target = World[player.player_level].interactible.data[i].position;
                    }
                }
                interactible->acting = false;
                interactible->actable = false;
            }
            player.acting = false;
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
            player.acting = false;
        } break;
    case DOOR:
        {
            DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            interactible->acting = false;
            interactible->actable = true;
            player.acting = false;
        } break;
    case FIRE:
        {
            in_menu = true;
            draw_fire(interactible, input);

            interactible->acting = false;
            player.acting = false;
            player.check_point = v2(interactible->position.x+48, interactible->position.y-4);
            player.check_point_level = player.player_level;
            player.position = player.check_point;
            player.velocity = v2_zero;
            camera_pos = player.position;
        } break;
    }
}

void create_level_entries() {
    tele[0].entry = r2_bounds(v2(9264, 480), v2(48, 96), v2_zero, v2_one);
    tele[0].link_id = 1;
    tele[0].level_id = 0;
    tele[0].landing_pos = v2(9216, 524);
    tele[0].actable = false;
    tele_count++;

    tele[1].entry = r2_bounds(v2(432, 480), v2(48, 96), v2_zero, v2_one);
    tele[1].link_id = 0;
    tele[1].level_id = 1;
    tele[1].landing_pos = v2(480, 524);
    tele[1].actable = false;
    tele_count++;

    tele[2].entry = r2_bounds(v2(1104, 480), v2(48, 96), v2_zero, v2_one);
    tele[2].link_id = 3;
    tele[2].level_id = 1;
    tele[2].landing_pos = v2(1104, 524);
    tele[2].actable = true;
    tele_count++;

    tele[3].entry = r2_bounds(v2(1200, 432), v2(48, 96), v2_zero, v2_one);
    tele[3].link_id = 2;
    tele[3].level_id = 2;
    tele[3].landing_pos = v2(1200, 476);
    tele[3].actable = true;
    tele_count++;

    tele[4].entry = r2_bounds(v2(2160, 480), v2(48, 96), v2_zero, v2_one);
    tele[4].link_id = 5;
    tele[4].level_id = 1;
    tele[4].landing_pos = v2(2112, 524);
    tele[4].actable = false;
    tele_count++;

    tele[5].entry = r2_bounds(v2(480, 480), v2(48, 96), v2_zero, v2_one);
    tele[5].link_id = 4;
    tele[5].level_id = 3;
    tele[5].landing_pos = v2(528, 524);
    tele[5].actable = false;
    tele_count++;

    tele[6].entry = r2_bounds(v2(1440, 480), v2(48, 96), v2_zero, v2_one);
    tele[6].link_id = 7;
    tele[6].level_id = 3;
    tele[6].landing_pos = v2(1392, 524);
    tele[6].actable = false;
    tele_count++;

    tele[7].entry = r2_bounds(v2(672, 48), v2(48, 96), v2_zero, v2_one);
    tele[7].link_id = 6;
    tele[7].level_id = 4;
    tele[7].landing_pos = v2(720, 92);
    tele[7].actable = false;
    tele_count++;

    tele[8].entry = r2_bounds(v2(1440, 528), v2(48, 96), v2_zero, v2_one);
    tele[8].link_id = 9;
    tele[8].level_id = 4;
    tele[8].landing_pos = v2(1392, 572);
    tele[8].actable = false;
    tele_count++;

    tele[9].entry = r2_bounds(v2(1056, 48), v2(48, 96), v2_zero, v2_one);
    tele[9].link_id = 8;
    tele[9].level_id = 5;
    tele[9].landing_pos = v2(1104, 92);
    tele[9].actable = false;
    tele_count++;
}