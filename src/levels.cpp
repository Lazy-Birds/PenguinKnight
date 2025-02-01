const i32 world_size = 8;

const i32 entity_unit = 1250;
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
    World[6].name = S("Dogland");
    World[6].region = v2i(0, 1);
    World[6].scale = 6;
    World[7].name = S("BlackPowder Forest");
    World[7].region = v2i(0, 2);
    World[7].scale = 8;

    if (!pengu_arena){
        pengu_arena = arena_alloc(Gigabytes(1));
    } else {
        arena_reset(pengu_arena);
    }
    
    for (int i = 0; i < world_size; i++) {
        World[i].id = i;
        World[i].world = LoadImage(sprint("level%d.png", i));
        World[i].background = LoadImage(string_concat(sprint("region%d", World[i].region.x), sprint("area%d.png", World[i].region.y)));
        World[i].entities = make_entity_array_two(pengu_arena, entity_unit*World[i].scale);
        World[i].liquid = make_entity_array_two(pengu_arena, liquid_unit*World[i].scale);
        World[i].initialized = false;
    }

    create_level_entries();
}

Entity make_interactible(Vector2 position, i32 type, i32 sub_type) {
    Level *level = &World[player.player_level];

    switch (type)
    {
    case et_lever:
        {
            static Image image[] = {
                LoadImage(S("lever1.png")),
                LoadImage(S("lever2.png")),
                LoadImage(S("lever3.png")),
            };

            Entity ent = {};

            ent.position = position;
            ent.size = v2(48, 48);
            ent.image = image;
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = true;
            ent.acting = false;

            return ent;

        } break;
    case et_gate:
        {
            static Image bot[] = {
                LoadImage(S("gate1.png")),
            };

            static Image top[] = {
                LoadImage(S("gate2.png")),
            };

            Entity ent = {};

            if (sub_type == st_gate_bot) {
                ent.image = bot;
            } else {
                ent.image = top;
            }

            ent.position = position;
            ent.size = v2(48, 48);
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = false;
            ent.acting = false;
            ent.state_time = 0;

            return ent;

        } break;
    case et_door:
        {
            static Image image[] = {
                LoadImage(S("pipe_door.png")),
            };

            Entity ent = {};

            ent.position = position;
            ent.size = v2(48, 48);
            ent.image = image;
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = true;
            ent.acting = false;

            return ent;

        } break;
    case et_fire:
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

            static Image fire_no_bg[] = {
                LoadImage(S("fire_no_bg1.png")),
                LoadImage(S("fire_no_bg2.png")),
                LoadImage(S("fire_no_bg3.png")),
                LoadImage(S("fire_no_bg4.png")),
                LoadImage(S("fire_no_bg5.png")),
            };

            Entity ent = {};

            if (sub_type == st_village) {
                ent.image = village_fire;
            } else if (sub_type == st_sewer) {
                ent.image = sewer_fire;
            } else if (sub_type == st_no_bg) {
                ent.image = fire_no_bg;
            }
            
            ent.position = position;
            ent.size = v2(48, 48);
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = true;
            ent.acting = false;

            return ent;

        } break;
    case et_hook:
        {
            static Image right[] = {
                LoadImage(S("hook_right.png")),
            };

            static Image left[] = {
                LoadImage(S("hook_left.png")),
            };

            Entity ent = {};

            if (sub_type == st_right) {
                ent.image = right;
                ent.anchor = v2(position.x+48, position.y);
            } else {
                ent.image = left;
                ent.anchor = v2(position.x, position.y);
            }

            ent.position = position;
            ent.size = v2(48, 48);
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = false;
            ent.acting = false;

            return ent;

        } break;
    case et_ladder:
        {
            static Image laddy_daddy[] = {
                LoadImage(S("ladder.png")),
            };

            Entity ent = {};

            ent.image = laddy_daddy;

            ent.position = position;
            ent.size = v2(48, 48);
            ent.type = type;
            ent.sub_type = sub_type;
            ent.id = level->entities.count;
            ent.alive = true;
            ent.actable = true;
            ent.acting = false;

            return ent;

        } break;
    }

    return {};
}



void interact(Entity *interactible, Game_Input *input, Vector2 camera_pos) {
    switch (interactible->type)
    {
    case et_lever:
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
                for (int i = 0; i < World[player.player_level].entities.count; i++) {
                    if (World[player.player_level].entities.data[i].type == et_gate) {
                        World[player.player_level].entities.data[i].acting = true;
                        camera_state = CAMERALOCKED;
                        camera_pos_target = World[player.player_level].entities.data[i].position;
                    }
                }
                interactible->acting = false;
                interactible->actable = false;
            }
            player.acting = false;
        } break;
    case et_gate:
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
    case et_door:
        {
            DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
            interactible->acting = false;
            interactible->actable = true;
            player.acting = false;
        } break;
    case et_fire:
        {
            in_menu = true;
            at_bonfire = true;
            draw_fire(interactible, input);

            interactible->acting = false;
            player.acting = false;
            player.check_point = v2(interactible->position.x+48, interactible->position.y-4);
            player.check_point_level = player.player_level;
            player.position = player.check_point;
            player.velocity = v2_zero;
            camera_pos = player.position;
        } break;
    case et_ladder:
        {
            player.velocity.y = -200;

            player.acting = false;
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

    tele[10].entry = r2_bounds(v2(7248, -96), v2(48, 96), v2_zero, v2_one);
    tele[10].link_id = 11;
    tele[10].level_id = 0;
    tele[10].landing_pos = v2(7248, 0);
    tele[10].actable = false;
    tele_count++;

    tele[11].entry = r2_bounds(v2(4416, 720), v2(48, 96), v2_zero, v2_one);
    tele[11].link_id = 10;
    tele[11].level_id = 6;
    tele[11].landing_pos = v2(4416, 668);
    tele[11].actable = false;
    tele_count++;

    tele[12].entry = r2_bounds(v2(432, 480), v2(48, 96), v2_zero, v2_one);
    tele[12].link_id = 13;
    tele[12].level_id = 6;
    tele[12].landing_pos = v2(480, 524);
    tele[12].actable = false;
    tele_count++;

    tele[13].entry = r2_bounds(v2(9072, 480), v2(48, 96), v2_zero, v2_one);
    tele[13].link_id = 12;
    tele[13].level_id = 7;
    tele[13].landing_pos = v2(9024, 524);
    tele[13].actable = false;
    tele_count++;

    tele[14].entry = r2_bounds(v2(5424, 672), v2(1200, 96), v2_zero, v2_one);
    tele[14].link_id = 15;
    tele[14].level_id = 6;
    tele[14].landing_pos = v2(5376, 284);
    tele[14].actable = false;
    tele_count++;

    tele[15].entry = r2_bounds(v2(8026, -96), v2(1200, 96), v2_zero, v2_one);
    tele[15].link_id = 14;
    tele[15].level_id = 0;
    tele[15].landing_pos = v2(8026, 0);
    tele[15].actable = false;
    tele_count++;
}