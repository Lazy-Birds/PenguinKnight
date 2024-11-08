Entity wall[1000] = {};
int wall_count = 0;
Entity enemys[1000] = {};
int enemy_count = 0;

Entity npc[200] = {};
int npc_count = 0;

void move_pengu(Entity *pengu_king, f32 dt);
void draw_pengu(Entity *pengu_king, i32 frame, i32 layer);
void draw_clouds(Entity *pengu, f32 dt);
void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image);

struct Background {
    Vector2 position;
    Vector2 size;
    Image image;
    i32 frames;
};

struct Fire {
    Vector2 position;
    Vector2 size;
    Image *image;
    i32 frames;
    f32 state_time;
};

Fire fire = {};

Background bgnd[1000] = {};
i32 bgnd_count = 0;


u32 image_get_pixel(Image image, int x, int y) {
    return image.pixels[y*image.size.width+x];
}

Entity load_enemy(Vector2 pos, i32 type) {
    switch (type)
    {
    case 0:
        {
            static Image image[1] = {LoadImage(S("plant_idle1.png"))};

            Entity enemy_plant = {};
            enemy_plant.max_health = 50,
            enemy_plant.current_health = enemy_plant.max_health,
            enemy_plant.min_health = 0,
            enemy_plant.position = pos,
            enemy_plant.size = v2(48, 48),
            enemy_plant.invuln = false,
            enemy_plant.alive = true,
            enemy_plant.enemy.image = image;
            enemy_plant.facing = 1;
            enemy_plant.enemy.type = 0;
            enemy_plant.enemy.offset =v2(0,0);

            return enemy_plant;
        } break;
    case 1:
        {
            static Image image[2] = 
            {
                LoadImage(S("seal1.png")),
                LoadImage(S("seal2.png")),
            };

            Entity enemy_seal = {};
            enemy_seal.max_health = 50,
            enemy_seal.current_health = enemy_seal.max_health,
            enemy_seal.min_health = 0,
            enemy_seal.position = pos,
            enemy_seal.size = v2(32, 32),
            enemy_seal.invuln = false,
            enemy_seal.alive = true,
            enemy_seal.enemy.image = image;
            enemy_seal.enemy.frames = v2(0,1);
            enemy_seal.facing = -1;
            enemy_seal.enemy.type = 1;
            enemy_seal.enemy.offset = v2(0, 0);
            enemy_seal.enemy.enemy_state = 0;
            enemy_seal.enemy.enemy_time = 0;
            enemy_seal.enemy.exp_dropped = 50;
            enemy_seal.enemy.damage = 10;

            return enemy_seal;
        } break;
    case 2:
        {
            static Image image[12] = 
            {
                LoadImage(S("big_papa1.png")),
                LoadImage(S("big_papa2.png")),
                LoadImage(S("big_papa3.png")),
                LoadImage(S("big_papa4.png")),
                LoadImage(S("big_papa5.png")),
                LoadImage(S("big_papa6.png")),
                LoadImage(S("big_papa7.png")),
                LoadImage(S("big_papa8.png")),
                LoadImage(S("big_papa9.png")),
                LoadImage(S("big_papa10.png")),
                LoadImage(S("big_papa11.png")),
                LoadImage(S("big_papa12.png")),

            };

            static Image portrait[2] = {
                LoadImage(S("penguin_king_portrait1.png")),
                LoadImage(S("penguin_king_portrait2.png"))
            };

            Entity big_papa = {};
            big_papa.max_health = 750;
            big_papa.current_health = big_papa.max_health;
            big_papa.min_health = 0;
            big_papa.position = pos;
            big_papa.size = v2(64, 94);
            big_papa.invuln = false;
            big_papa.alive = true;
            big_papa.facing = -1;
            big_papa.enemy.image = image;
            big_papa.enemy.type = 3;
            big_papa.enemy.offset = v2(0, 0);
            big_papa.enemy.guard = 60;
            big_papa.enemy.damage = 30;
            big_papa.portrait = portrait;
            big_papa.enemy.exp_dropped = 2000;

            load_penguin_king_dialogue(&big_papa);

            return big_papa;
        } break;
    case 3:
        {
            static Image image[4] =
            {
                LoadImage(S("penguin_soldier1.png")),
                LoadImage(S("penguin_soldier2.png")),
                LoadImage(S("penguin_soldier3.png")),
                LoadImage(S("penguin_soldier4.png")),
            };

            Entity penguin_soldier = {};
            penguin_soldier.max_health = 80;
            penguin_soldier.current_health = penguin_soldier.max_health;
            penguin_soldier.min_health = 0;
            penguin_soldier.enemy.offset = v2(-27,-6);
            penguin_soldier.position = v2(pos.x + 6, pos.y);
            penguin_soldier.size = v2(42, 48);
            penguin_soldier.invuln = false;
            penguin_soldier.alive = true;
            penguin_soldier.facing = -1;
            penguin_soldier.enemy.image = image;
            penguin_soldier.enemy.type = 4;
            penguin_soldier.enemy.anchor_pos = pos;
            penguin_soldier.enemy.guard = 20;
            penguin_soldier.enemy.weapon_size = v2(56, 8);
            penguin_soldier.enemy.weapon_offset = v2(-27, 33);
            penguin_soldier.enemy.damage = 30;
            penguin_soldier.enemy.id = wall_count;
            penguin_soldier.state = NEUTRAL;
            penguin_soldier.state_prev = NEUTRAL;
            penguin_soldier.enemy.exp_dropped = 75;

            return penguin_soldier;
        }
    default:
        {
            static Image image[1] = {LoadImage(S("penguin_idle.png"))};

            Entity idle_penguin = {};
            idle_penguin.max_health = 50,
            idle_penguin.current_health = idle_penguin.max_health,
            idle_penguin.min_health = 0,
            idle_penguin.position = pos,
            idle_penguin.size = v2(48, 48),
            idle_penguin.invuln = false,
            idle_penguin.alive = true,
            idle_penguin.enemy.image = image;
            idle_penguin.facing = -1;
            idle_penguin.enemy.type = 2;
            idle_penguin.enemy.offset = v2(0,0);

            return idle_penguin;
        }
    }    
}

const i32 fairy = 0;

void make_npcs(Vector2 pos, i32 type) {
    switch (type)
    {
    case fairy:
        {
            static Image image[2] = {LoadImage(S("fairy_orb1.png")), LoadImage(S("fairy_orb2.png"))};
            static Image portrait[2] = {LoadImage(S("fairy_orb_portrait1.png")), LoadImage(S("fairy_orb_portrait2.png"))};

            npc[npc_count].position = pos;
            npc[npc_count].image = image;
            npc[npc_count].alive = true;
            npc[npc_count].state_time = 0;
            npc[npc_count].animation_time = 0;
            npc[npc_count].dialogue_time = 0;
            npc[npc_count].portrait = portrait;

        } break;
    }
}

void draw_player(Weapon weapon, Vector2 position, i32 frame, i32 facing) {
    int layer = 0;

    if (position.y+48 < 0) {
        layer = 720;
    } else {
        layer = 0;
    }

    if (sign_i32(facing) < 0) {
        DrawImageMirrored(weapon.image[frame], v2(position.x + weapon.offset_left.x, position.y+weapon.offset_left.y+layer), true, false);
        //DrawImage(weapon.image[i32(frame + weapon.weapon_frames.y+1)], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y));
    } else {
        DrawImage(weapon.image[frame], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y+layer));
    }
}

void make_wall(Vector2 pos, u32 pixel, String image) {
    i32 new_pos = 0;

    if (pos.y < 0) {
        new_pos = abs_i32(pos.y*48) - 720;
    } else {
        new_pos = pos.y*48;
    }

    wall[wall_count].position = v2(pos.x*48, new_pos);
    wall[wall_count].size = v2(48, 48);
    wall[wall_count].wall_type.pixel = pixel;
    wall[wall_count].wall_type.image = LoadImage(image);

    wall_count++;
}

void make_world(i32 offset) {
    /*wall[0].position.y = out->height-32;
    wall[0].position.x = 32;
    wall[0].size = v2(32, 32);
    wall[0].color = v4_yellow;

    wall[1].position.y = out->height-32;
    wall[1].position.x = out->width-64;
    wall[1].size = v2(32, 32);
    wall[1].color = v4_yellow;

    wall[2].position.y = out->height-64;
    wall[2].position.x = out->width-64;
    wall[2].size = v2(32, 32);
    wall[2].color = v4_yellow;*/

    Image world = {};

    i32 layer = 0;

    if (offset == 0) {
        layer = 1;
    } else {
        layer = -1;
    }

    if (offset == 0) {
        world = LoadImage(S("TestWorld.png"));
    } else {
        world = LoadImage(S("TestWorld2.png"));
    }


    for (int i = 0; i < world.size.x; i++) {
        for (int k = 0; k < world.size.y; k++) {
            u32 pixel = image_get_pixel(world, i, k);
            if (ARCH_LITTLE_ENDIAN) {
                pixel = endian_swap_u32(pixel);
            }
            
            if (image_get_pixel(world, i, k)!=0) {
                switch (pixel)
                {
                case -1405996289: 
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48-offset), 0);
                        enemy_count++;
                    } break;
                case -1060381441: 
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48-offset), 1);
                        enemy_count++;
                    } break;
                case -18390529:
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48-offset), 2);
                        enemy_count++;
                    } break;
                case 1516865791:
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48-offset), 3);
                        enemy_count++;
                    } break;
                case -1562516993:
                    {
                        make_npcs(v2(i*48, k*48-offset), 0);
                        load_fairy_dialogue(&npc[npc_count]);
                        npc_count++;
                    } break;
                case 753464831: 
                    {
                        bgnd[bgnd_count].position = v2(i*48, k*48-offset);
                        bgnd[bgnd_count].size = v2(48, 48);
                        bgnd[bgnd_count].image = LoadImage(S("ice_grass1.png"));
                        bgnd_count++;
                    } break;
                case 423378687: 
                    {
                        bgnd[bgnd_count].position = v2(i*48, k*48-offset);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = LoadImage(S("pointed_fir.png"));
                        bgnd_count++;
                    } break;
                case -465877761:
                {
                    static Image image[5] = {
                        LoadImage(S("fire1.png")),
                        LoadImage(S("fire2.png")),
                        LoadImage(S("fire3.png")),
                        LoadImage(S("fire4.png")),
                        LoadImage(S("fire5.png")),
                    };
                    fire.position = v2(i*48, k*48-offset);
                    fire.size = v2(48, 48);
                    fire.image = image;
                    fire.frames = 4;
                    fire.state_time = 0;
                } break;
                case 403973631:
                    {
                        static Image image = LoadImage(S("cave_wall.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48-offset);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
                    } break;
                case 842807807: 
                    {
                        make_wall(v2(i, k*layer), pixel, S("grass_bot.png"));
                    } break;
                case -1713024769:
                    {
                        make_wall(v2(i, k*layer), pixel, S("grass_top.png"));
                    } break;
                case 1437261311: 
                    {
                        make_wall(v2(i, k*layer), pixel, S("dirt_covered_snow.png"));
                    } break;
                case 1715024383:
                    {
                        make_wall(v2(i, k*layer), pixel, S("dirt.png"));
                    } break;
                case -521737985:
                    {
                        make_wall(v2(i, k*layer), pixel, S("ice_brick.png"));
                    } break;
                case 7903743:
                    {
                        make_wall(v2(i, k*layer), pixel, S("rocks.png"));
                    } break;
                case 629118975:
                    {
                        make_wall(v2(i, k*layer), pixel, S("stone_brick.png"));
                    } break;
                case 530563839:
                    {
                        make_wall(v2(i, k*layer), pixel, S("rocks_crack.png"));
                    } break;
                case -2003194113:
                    {
                        make_wall(v2(i, k*layer), pixel, S("cloud_brick.png"));
                    } break;
                default:
                    {
                        Dump(pixel);
                        make_wall(v2(i, k*layer), pixel, S("penguin_idle.png"));
                    } break;
                }; 
                
            }
        }
    }
}

bool entity_on_wall(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+1), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
    } 
}

return false;
}

bool wall_intersects(Entity *entity) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(entity->position, entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
        }
    }
    return false;
}

bool wall_ahead(Entity *entity) {
    for (int i = 0; i < wall_count; i++) {
        if (entity->facing > 0)
        {
            if (r2_intersects(r2_bounds(v2(entity->position.x+1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one)))
            {
                return true;
            }
        } else if (r2_intersects(r2_bounds(v2(entity->position.x-1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    return false;
}

bool entity_in_air(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+2), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return false;
    }
}
return true;
}

const i32 SEALNEUTRAL = 0;
const i32 SEALMOVING = 1;
const i32 SEALATTACKING = 2;

void seal_action(Entity *seal, Game_Input *input, Entity *player) {
    if (seal->enemy.enemy_state == SEALNEUTRAL) {
        if (seal->alive) {
            seal->enemy.enemy_state = SEALMOVING;
        }
    }

    seal->enemy.enemy_time++;

    if (seal->enemy.sleep_time > 0) {
        seal->enemy.sleep_time-=input->dt;
        return;
    }

    if (wall_ahead(seal)) {
        if (seal->facing > 0) {
            seal->facing = -1;
        } else {
            seal->facing = 1;
        }
    }

    switch (seal->enemy.enemy_state) 
    {
    case SEALNEUTRAL:
        {
        } break;
    case SEALMOVING:
        {
            if (seal->facing > 0) {
                seal->velocity.x = 200;
            } else {
                seal->velocity.x = -200;
            }

            seal->velocity.y += 496*input->dt;

            f32 dy = seal->velocity.y*input->dt;
            f32 dx = seal->velocity.x*input->dt;

            for (int i = 0; i < abs_f32(dy); i++) {
                seal->position.y+=sign_f32(dy);
                if (wall_intersects(seal)) {
                    seal->position.y-=sign_f32(dy);
                    seal->velocity.y=0;
                }
            }


            for (int i = 0; i < abs_f32(dx); i++) {
                seal->position.x+=sign_f32(dx);
                if (wall_intersects(seal)) {
                    seal->position.x-=sign_f32(dx);
                    seal->velocity.x=0;
                }
            }

            if (invuln_time <= 0){
                if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(seal->position, seal->size, v2_zero, v2_one)))
                {
                    player_hit(seal->enemy.damage, input);
                }
            }
        }
    } 
}

void make_guards(Vector2 positions) {
    enemys[enemy_count] = load_enemy(v2(positions.x, 0), 3);
    enemy_count++;

    enemys[enemy_count] = load_enemy(v2(positions.y, 0), 3);
    enemy_count++;
}

const i32 KINGNEUTRAL = 0;
const i32 KINGSTOMPING = 1;
const i32 KINGJUMPING = 2;
const i32 KINGLEAPING = 3;
const i32 KINGLANDING = 4;
const i32 KINGGUARDBREAK = 5;
const i32 KINGCOMMAND = 6;
const i32 KINGWAIT = 7;
const i32 KINGCHALLENGE = 8;

i32 king_state = KINGNEUTRAL;
i32 king_state_prev = KINGNEUTRAL;
f32 king_time = 0;


const i32 PHASEONE = 0;
const i32 PHASETWO = 1;

i32 king_phase = PHASEONE;


Vector2 stage_size = v2(8640, 9504);

void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt, Game_Output *out, i32 layer) {


    king_time+=dt;
    pengu_king->dialogue_time+=dt;

    if (king_state != king_state_prev) {
        king_state_prev = king_state;
        king_time = 0;
    }

    if (king_state == KINGNEUTRAL){
        if (!wall_ahead(pengu_king)) {
            king_time = 0;
            king_state = KINGSTOMPING;
            king_state_prev = king_state;
        } else if(wall_ahead(pengu_king)) {
            king_time = 0;
            king_state = KINGJUMPING;
            king_state_prev = king_state;
        } else if (pengu_king->enemy.guard <=0) {
            king_time = 0;
            king_state = KINGGUARDBREAK;
            king_state_prev = king_state;
        }
    }

    if (wall_ahead(pengu_king) && king_time == 0) {
        if (pengu_king->facing < 0) {
            pengu_king->facing = 1;
        } else {
            pengu_king->facing = -1;
        }
    }

    if (king_phase == PHASEONE) {
        if (king_state != KINGCOMMAND && king_state != KINGWAIT) {
            king_state = KINGCOMMAND;
            king_time = 0;
            king_state_prev = king_state;
        }
    } else {
        DrawRect(r2_bounds(v2(out->width*.5-302, out->height-50 + layer), v2(604, 12), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(out->width*.5-300, out->height-48 + layer), v2((pengu_king->current_health/pengu_king->max_health)*600, 8), v2_zero, v2_one), v4_red);
    }


    switch (king_state)
    {
    case KINGNEUTRAL:
        {
            if (pengu_king->facing > 0)
            {
                DrawImageMirrored(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                   pengu_king->position.y+pengu_king->enemy.offset.y), true, false);
            } else 
            {
                DrawImage(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                    pengu_king->position.y+pengu_king->enemy.offset.y));
            }

        } break;
    case KINGSTOMPING:
        {
            if (king_time*60 < 5) {
                draw_pengu(pengu_king, 2, layer);
                
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 10) {
                draw_pengu(pengu_king, 3, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 15) {
                draw_pengu(pengu_king, 4, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 20) {
                draw_pengu(pengu_king, 5, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 25) {
                draw_pengu(pengu_king, 6, layer);
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 30) {
                draw_pengu(pengu_king, 7, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 35) {
                draw_pengu(pengu_king, 4, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else {
                draw_pengu(pengu_king, 2, layer);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
                king_state = KINGNEUTRAL;
            }
            for (int i = 0; i < 3; i++) {
                Particle_Parameters param1 = {};
                Particle_Parameters param2 = {};

                if (pengu_king->facing < 0) {
                    param1.position.x = pengu_king->position.x-5;
                    param2.position.x = pengu_king->position.x+pengu_king->size.x;

                    param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                    param2.position.y = pengu_king->position.y + pengu_king->size.y;
                } else {
                    param1.position.x = pengu_king->position.x-5;
                    param2.position.x = pengu_king->position.x+pengu_king->size.x;

                    param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                    param2.position.y = pengu_king->position.y + pengu_king->size.y;

                }

                param1.velocity.x = 300*dt*sign_f32(pengu_king->facing);
                param2.velocity.x = 600*dt*sign_f32(pengu_king->facing);

                param1.velocity.y = -600*dt;
                param2.velocity.y = -100*dt;

                param1.life_time = 60*dt;
                param2.life_time = 120*dt;

                Image image = LoadImage(S("snow_flake_chunky.png"));

                particle_emit(param1, param2, image);
            }
        } break;
    case KINGJUMPING:
        {
            if (pengu_king->facing > 0) {
                if (pengu_king->position.x < stage_size.y-65) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                        for (int i = 0; i < 80; i++) {
                            Particle_Parameters param1 = {};
                            Particle_Parameters param2 = {};

                            if (pengu_king->facing < 0) {
                                param1.position.x = pengu_king->position.x;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;
                            } else {
                                param1.position.x = pengu_king->position.x-5;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;

                            }
                            if (i%2 == 0) {
                                param1.velocity.x = 800*dt;
                                param2.velocity.x = 1600*dt;
                            } else {
                                param1.velocity.x = -800*dt;
                                param2.velocity.x = -1600*dt;
                            }
                            

                            param1.velocity.y = -1600*dt;
                            param2.velocity.y = -600*dt;

                            param1.life_time = 60*dt;
                            param2.life_time = 120*dt;
                            
                            Image image = LoadImage(S("snow_flake_chunky.png"));

                            particle_emit(param1, param2, image);
                        }

                    }
                    pengu_king->velocity.x = 6000*dt;

                    draw_pengu(pengu_king, 0, layer);
                } else {
                    king_state = KINGLEAPING;
                    draw_pengu(pengu_king, 0, layer);
                }
            } else {
                if (pengu_king->position.x > stage_size.x) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                        for (int i = 0; i < 80; i++) {
                            Particle_Parameters param1 = {};
                            Particle_Parameters param2 = {};

                            if (pengu_king->facing < 0) {
                                param1.position.x = pengu_king->position.x;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;
                            } else {
                                param1.position.x = pengu_king->position.x-5;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;

                            }
                            if (i%2 == 0) {
                                param1.velocity.x = 800*dt;
                                param2.velocity.x = 1600*dt;
                            } else {
                                param1.velocity.x = -800*dt;
                                param2.velocity.x = -1600*dt;
                            }
                            

                            param1.velocity.y = -1600*dt;
                            param2.velocity.y = -600*dt;

                            param1.life_time = 60*dt;
                            param2.life_time = 120*dt;
                            
                            Image image = LoadImage(S("snow_flake_chunky.png"));

                            particle_emit(param1, param2, image);
                        }
                    }
                    pengu_king->velocity.x = -6000*dt;

                    draw_pengu(pengu_king, 0, layer);
                } else {
                    king_state = KINGLEAPING;
                    draw_pengu(pengu_king, 0, layer);
                }
            }   
        } break;
    case KINGLEAPING:
        {
            Rectangle2 land_aoe = r2_bounds(v2(pengu_king->position.x-50, i32(pengu_king->position.y+pengu_king->size.y/2)), v2(pengu_king->size.x+100, i32(pengu_king->size.y/2)), v2_one, v2_zero);

            if (entity_on_wall(pengu_king) && king_time*60 < 10) {
                draw_pengu(pengu_king, 8, layer);
            } else if (pengu_king->position.y > -pengu_king->size.y && king_time*60 < 180) {
                pengu_king->velocity.x = 0;
                draw_pengu(pengu_king, 9, layer);
                pengu_king->velocity.y = -45000*dt;
            } else if (king_time*60 < 180) {
                pengu_king->velocity.y = 0;
                pengu_king->position = v2(player->position.x, -300);
                pengu_king->facing = player->facing;
            } else if (!entity_on_wall(pengu_king)) {
                if (pengu_king->position.x > stage_size.y-49) pengu_king->position = v2(stage_size.y-49, pengu_king->position.y);
                if (pengu_king->position.x < stage_size.x) pengu_king->position = v2(stage_size.x, pengu_king->position.y);
                draw_pengu(pengu_king, 8, layer);
                pengu_king->velocity.y += 2000*dt;
            } else {
                for (int i = 0; i < 80; i++) {
                    Particle_Parameters param1 = {};
                    Particle_Parameters param2 = {};

                    if (pengu_king->facing < 0) {
                        param1.position.x = pengu_king->position.x;
                        param2.position.x = pengu_king->position.x+pengu_king->size.x;

                        param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                        param2.position.y = pengu_king->position.y + pengu_king->size.y;
                    } else {
                        param1.position.x = pengu_king->position.x-5;
                        param2.position.x = pengu_king->position.x+pengu_king->size.x;

                        param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                        param2.position.y = pengu_king->position.y + pengu_king->size.y;

                    }
                    if (i%2 == 0) {
                        param1.velocity.x = 1200*dt;
                        param2.velocity.x = 3600*dt;
                    } else {
                        param1.velocity.x = -1200*dt;
                        param2.velocity.x = -3600*dt;
                    }


                    param1.velocity.y = -2200*dt;
                    param2.velocity.y = -1000*dt;

                    param1.life_time = 80*dt;
                    param2.life_time = 160*dt;

                    Image image = LoadImage(S("snow_flake_chunky.png"));

                    particle_emit(param1, param2, image);
                }
                Rectangle2 land_aoe = r2_bounds(v2(pengu_king->position.x-50, i32(pengu_king->position.y+pengu_king->size.y/2)), v2(pengu_king->size.x+100, i32(pengu_king->size.y/2)), v2_one, v2_zero);

                if (r2_intersects(land_aoe, get_entity_rect(player))) {
                    player_hit(200, input);
                }

                

                king_state = KINGLANDING;
                draw_pengu(pengu_king, 10, layer);
            }

            land_aoe = r2_shift(land_aoe, v2(-camera_pos.x+out->width*.5, 0));

            DrawRectOutline(land_aoe, v4_red, 1);
        } break;
    case KINGLANDING:
        {
            if (king_time*60 < 180) {
                if (i32(king_time*60)%20< 10) {
                    draw_pengu(pengu_king, 10, layer);
                } else {
                    draw_pengu(pengu_king, 11, layer);
                }
            } else {
                king_state = KINGNEUTRAL;
                draw_pengu(pengu_king, 0, layer);
            }
        } break;
    case KINGCOMMAND:
        {
            draw_pengu(pengu_king, 0, layer);

            if (king_time*60 < 220) {
                draw_dialogue_box(pengu_king->dialogue[0], out, pengu_king->portrait, 2, 120*pengu_king->dialogue_time);
            } 

            if (king_time*60 > 220 && king_time*60 < 222) {
                pengu_king->dialogue_time = 0;
            }

            if (king_time*60 > 220 && king_time*60 < 440) {
                draw_dialogue_box(pengu_king->dialogue[1], out, pengu_king->portrait, 2, 120*pengu_king->dialogue_time);
            } else if (king_time*60 > 440) {
                make_guards(stage_size);
                king_state = KINGWAIT;
            }
            
        } break;
    case KINGWAIT:
        {
            if (enemys[enemy_count-1].alive || enemys[enemy_count-2].alive) {
                draw_pengu(pengu_king, 0, layer);
            } else {
                draw_pengu(pengu_king, 0, layer);
                king_state = KINGCHALLENGE;
                pengu_king->dialogue_time = 0;

                king_phase = PHASETWO;
            }
        } break;
    case KINGCHALLENGE:
        {
            if (king_time*60 < 200) {
                draw_pengu(pengu_king, 0, layer);
                draw_dialogue_box(pengu_king->dialogue[2], out, pengu_king->portrait, 2, 60*pengu_king->dialogue_time);
            } else {
                draw_pengu(pengu_king, 0, layer);
                king_state = KINGLEAPING;
            }
        } break;
    }

    if (invuln_time <= 0) {
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(pengu_king->position, pengu_king->size, v2_zero, v2_one)))
        {
            player_hit(pengu_king->enemy.damage, input);
        }
    }

    move_pengu(pengu_king, dt);
    
}

const i32 particle_count = 10000;
i32 living_particles = 0;

Particle particles[particle_count] = {};

void particle_create(Vector2 pos, Vector2 velocity, f32 lifetime, /*Vector4 color, Vector2 accel,*/ Image image) {
for (int i = 0; i < particle_count; i++) {
    if (!particles[i].is_alive) {
        particles[i].is_alive = true;
        particles[i].position = pos;
        particles[i].velocity = velocity;
        particles[i].life_time = lifetime;
        particles[i].image = image;
        living_particles++;
        break;
    }
}
}

void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image) {
    for (int i = 0; i < particle_count; i++) {
        if (!particles[i].is_alive) {
            particles[i].is_alive = true;
            particles[i].position.x = random_i32_between(min.position.x, max.position.x);
            particles[i].position.y = random_i32_between(min.position.y, max.position.y);
            particles[i].velocity.x = random_f32_between(min.velocity.x, max.velocity.x);
            particles[i].velocity.y = random_f32_between(min.velocity.y, max.velocity.y);
            particles[i].life_time = random_f32_between(min.life_time, max.life_time);
            particles[i].image = image;
            living_particles++;
            break;
        }
    }
}

//void particle_emit_rect()

void particle_update(f32 dt) {
    i32 particles_checked = 0;

    for (int i = 0; i < particle_count; i++) {
        if (particles[i].is_alive) {
            particles_checked++;
            particles[i].position.x += particles[i].velocity.x*dt;
            particles[i].position.y += particles[i].velocity.y*dt;
            particles[i].life_time-=dt;
            DrawImage(particles[i].image, v2(particles[i].position.x-camera_pos.x+out->width*.5, particles[i].position.y));
            if (particles[i].life_time <= 0) {
                particles[i].is_alive = false;
                living_particles--;
            }

            if (particles_checked == living_particles) break;
        }
    }
}

void draw_pengu(Entity *pengu, i32 frame, i32 layer) {
    if (pengu->facing > 0) {
        DrawImageMirrored(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5,
            pengu->position.y + pengu->enemy.offset.y+layer), true, false);
    } else {
        DrawImage(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5 + pengu->enemy.offset.x,
            pengu->position.y+pengu->enemy.offset.y+layer));
    }
}

void move_pengu(Entity *pengu, f32 dt) {
    pengu->velocity.y+=496*input->dt;

    f32 dy = pengu->velocity.y*dt;
    f32 dx = pengu->velocity.x*dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        pengu->position.y+=sign_f32(dy);
        if (wall_intersects(pengu) || enemy_overlap(pengu)) {
            pengu->position.y-=sign_f32(dy);
            pengu->velocity.y=0;
        }
    }


    for (int i = 0; i < abs_f32(dx); i++) {
        pengu->position.x+=sign_f32(dx);
        if (wall_intersects(pengu) || enemy_overlap(pengu)) {
            pengu->position.x-=sign_f32(dx);
            pengu->velocity.x=0;
        }
    }
}

void pengu_attack(Entity *pengu, Entity *player, f32 invuln_time, Game_Input *input) {
    if (invuln_time > 0) return;

    Rectangle2 weapon = r2_bounds(v2(pengu->position.x+pengu->enemy.weapon_offset.x, pengu->position.y+pengu->enemy.weapon_offset.y), pengu->enemy.weapon_size, v2_zero, v2_one);
    Rectangle2 rec_player = get_entity_rect(player);

    if (pengu->facing > 0) {
        weapon = r2_shift(weapon, v2(42, 0));
    }

    if (r2_intersects(weapon, rec_player)) {
        player_hit(pengu->enemy.damage, input);
    }
}

void p_soldier_action(Entity *soldier, f32 dt, Entity *player, f32 invuln_time, Game_Input *input, i32 layer) {
    soldier->state_time+=dt;

    if (player->position.x < soldier->position.x) {
        soldier->facing = -1;
    } else {
        soldier->facing = 1;
    }

    if (soldier->state_prev != soldier->state) {
        soldier->state_prev = soldier->state;
        soldier->state_time = 0;
    }

    if (soldier->state == NEUTRAL) {
        if (abs_i32(soldier->position.x - player->position.x) < 800 && abs_i32(soldier->position.x - player->position.x) > 200)
        {
            soldier->state = MOVE;
            soldier->state_prev = soldier->state;
            soldier->state_time = 0;
        } else if (abs_i32(soldier->position.x - player->position.x) < 200)
        {
            soldier->state = ATTACK;
            soldier->state_prev = soldier->state;
            soldier->state_time = 0;
        }
    }

    if (r2_intersects(get_entity_rect(soldier), get_entity_rect(player))) {
        soldier->state = ATTACK;

        if (soldier->state != soldier->state_prev) {
            soldier->state_time = 0;
            soldier->state_prev = ATTACK;
        }
    }


    switch (soldier->state)
    {
    case MOVE:
        {
            soldier->velocity.x = 120*sign_f32(soldier->facing);

            move_pengu(soldier, dt);
            if (soldier->facing > 0) {
                if (abs_i32(soldier->position.x - player->position.x) > 5) {
                    if (i32(soldier->state_time*60) < 60) {
                        draw_pengu(soldier, 0, layer);
                    } else {
                        draw_pengu(soldier, 1, layer);
                    }
                } else {
                    draw_pengu(soldier, 0, layer);
                    soldier->state = ATTACK;
                } 
            } else {
                if (abs_i32(soldier->position.x - player->position.x) > player->size.x+5) {
                    if (i32(soldier->state_time*60) < 60) {
                        draw_pengu(soldier, 0, layer);
                    } else {
                        draw_pengu(soldier, 1, layer);
                    }
                } else {
                    draw_pengu(soldier, 0, layer);
                    soldier->state = ATTACK;
                } 
            }
        } break;
    case ATTACK:
        {

            if (i32(soldier->state_time*60) < 30)
            {
                draw_pengu(soldier, 2, layer);
            } else if (i32(soldier->state_time*60) < 60)
            {
                draw_pengu(soldier, 3, layer);

                if (invuln_time <= 0)
                {
                    pengu_attack(soldier, player, invuln_time, input);
                }
            } else {
                draw_pengu(soldier, 0, layer);
                soldier->state = MOVE;
            }
        } break;
    }
}

void draw_fire(f32 dt) {
    fire.state_time+=dt;

    if (i32(fire.state_time*60)%50 < 10) {
        DrawImage(fire.image[0], v2(fire.position.x-camera_pos.x+out->width*.5, fire.position.y));
    } else if (i32(fire.state_time*60)%50 < 20) {
        DrawImage(fire.image[1], v2(fire.position.x-camera_pos.x+out->width*.5, fire.position.y));
    } else if (i32(fire.state_time*60)%50 < 30) {
        DrawImage(fire.image[2], v2(fire.position.x-camera_pos.x+out->width*.5, fire.position.y));
    } else if (i32(fire.state_time*60)%50 < 40) {
        DrawImage(fire.image[3], v2(fire.position.x-camera_pos.x+out->width*.5, fire.position.y));
    } else {
        DrawImage(fire.image[4], v2(fire.position.x-camera_pos.x+out->width*.5, fire.position.y));
    }

    Particle_Parameters min = {};
    Particle_Parameters max = {};

    min.position.x = fire.position.x+12;
    max.position.x = fire.position.x + 36;

    min.position.y = fire.position.y;
    max.position.y = fire.position.y + 18;

    min.velocity.x = 0;
    max.velocity.x = 0;

    min.velocity.y = -1200*dt;
    max.velocity.y = -1700*dt;

    min.life_time = 100.0*dt;
    max.life_time = 200.0*dt;

    static Image smoke = LoadImage(S("smoke.png"));


    if (random_f32_between(0, 1) < .1) {
        particle_emit(min, max, smoke);
    }
}


/*void seal_walk(Entity *entity, Game_Input *input) {
    for (int i = 0; i < enemy_count; i++) {
        if (!enemys[i].alive) {continue;}
        if (enemys[i].enemy.type == 1) {
            if (wall_ahead(&enemys[i], enemys[i].facing))
            {
                if (enemys[i].facing < 0) {
                    enemys[i].facing = 1;
                    enemys[i].velocity.x = 0;
                } else if (enemys[i].facing > 0) {
                    enemys[i].facing = -1;
                    enemys[i].velocity.x = 0;
                }
            }

            enemys[i].velocity.y+=496*input->dt;

            if (enemys[i].facing < 0) {
                enemys[i].velocity.x = -300;
            } else {
                enemys[i].velocity.x = 300;
            }

            f32 dy = enemys[i].velocity.y*input->dt;
            f32 dx = enemys[i].velocity.x*input->dt;

            Dump(i);
            Dump(dx);

            for (int i = 0; i < abs_f32(dy); i++) {
                enemys[i].position.y+=sign_f32(dy);
                if (wall_intersects(&enemys[i])) {
                    enemys[i].position.y-=sign_f32(dy);
                    enemys[i].velocity.y=0;
                }
            }


            for (int i = 0; i < abs_f32(dx); i++) {
                enemys[i].position.x+=sign_f32(dx);
                if (wall_intersects(&enemys[i])) {
                    enemys[i].position.x-=sign_f32(dx);
                    enemys[i].velocity.x=0;
                }
            }

            if (invuln_time <= 0){
                if (r2_intersects(r2_bounds(entity->position, entity->size, v2_zero, v2_one), r2_bounds(enemys[i].position, enemys[i].size, v2_zero, v2_one)))
                {
                    player_hit();
                }
            }
        }
    }
}*/