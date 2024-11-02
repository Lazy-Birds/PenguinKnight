Entity wall[1000] = {};
int wall_count = 0;
Entity enemys[1000] = {};
int enemy_count = 0;

Entity npc[200] = {};
int npc_count = 0;

void move_pengu(Entity *pengu_king, f32 dt);
void draw_pengu(Entity *pengu_king, i32 frame);
void draw_clouds(Entity *pengu, f32 dt);
void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image);

struct Plants {
    Vector2 position;
    Vector2 size;
    Image image;
    i32 frames;
};

Plants plants[1000] = {};
i32 plant_count = 0;


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
            penguin_soldier.max_health = 200;
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

            npc[npc_count].position = pos;
            npc[npc_count].image = image;
            npc[npc_count].alive = true;
            npc[npc_count].state_time = 0;
            npc[npc_count].animation_time = 0;

        } break;
    }
}

void draw_player(Weapon weapon, Vector2 position, i32 frame, i32 facing) {
    if (sign_i32(facing) < 0) {
        DrawImageMirrored(weapon.image[frame], v2(position.x + weapon.offset_left.x, position.y+weapon.offset_left.y), true, false);
        //DrawImage(weapon.image[i32(frame + weapon.weapon_frames.y+1)], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y));
    } else {
        DrawImage(weapon.image[frame], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y));
    }
}

void make_wall(Vector2 pos, u32 pixel, String image) {
    wall[wall_count].position = v2(pos.x*48, pos.y*48);
    wall[wall_count].size = v2(48, 48);
    wall[wall_count].wall_type.pixel = pixel;
    wall[wall_count].wall_type.image = LoadImage(image);

    wall_count++;
}

void make_world() {
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

    Image world = LoadImage(S("TestWorld.png"));


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
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 0);
                        enemy_count++;
                    } break;
                case -1060381441: 
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 1);
                        enemy_count++;
                    } break;
                case -18390529:
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 2);
                        enemy_count++;
                    } break;
                case 1516865791:
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 3);
                        enemy_count++;
                    } break;
                case 370612991:
                    {
                        make_npcs(v2(i*48, k*48), 0);
                        load_fairy_dialogue(&npc[npc_count]);
                        npc_count++;
                    } break;
                case 753464831: 
                    {
                        plants[plant_count].position = v2(i*48, k*48);
                        plants[plant_count].size = v2(48, 48);
                        plants[plant_count].image = LoadImage(S("ice_grass1.png"));
                        plant_count++;
                    } break;
                case 842807807: 
                    {
                        make_wall(v2(i, k), pixel, S("grass_bot.png"));
                    } break;
                case -1713024769:
                    {
                        make_wall(v2(i, k), pixel, S("grass_top.png"));
                    } break;
                case 1437261311: 
                    {
                        make_wall(v2(i, k), pixel, S("dirt_covered_snow.png"));
                    } break;
                case 1715024383:
                    {
                        make_wall(v2(i, k), pixel, S("dirt.png"));
                    } break;
                default:
                    {
                        Dump(pixel);
                        make_wall(v2(i, k), pixel, S("penguin_idle.png"));
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
                    player_hit(seal->enemy.damage);
                }
            }
        }
    } 
}

const i32 KINGNEUTRAL = 0;
const i32 KINGSTOMPING = 1;
const i32 KINGJUMPING = 2;
const i32 KINGLEAPING = 3;
const i32 KINGLANDING = 4;
const i32 KINGGUARDBREAK = 5;
const i32 KINGCOMMAND = 6;

i32 king_state = KINGNEUTRAL;
i32 king_state_prev = KINGNEUTRAL;
f32 king_time = 0;


Vector2 stage_size = v2(3840, 4656);

void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt, Game_Output *out) {


    king_time+=dt;

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
                draw_pengu(pengu_king, 2);
                
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 10) {
                draw_pengu(pengu_king, 3);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 15) {
                draw_pengu(pengu_king, 4);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 20) {
                draw_pengu(pengu_king, 5);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 25) {
                draw_pengu(pengu_king, 6);
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 30) {
                draw_pengu(pengu_king, 7);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 35) {
                draw_pengu(pengu_king, 4);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else {
                draw_pengu(pengu_king, 2);

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
                if (pengu_king->position.x < stage_size.y-49) {
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

                    draw_pengu(pengu_king, 0);
                } else {
                    king_state = KINGLEAPING;
                    draw_pengu(pengu_king, 0);
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

                    draw_pengu(pengu_king, 0);
                } else {
                    king_state = KINGLEAPING;
                    draw_pengu(pengu_king, 0);
                }
            }   
        } break;
    case KINGLEAPING:
        {
            Rectangle2 land_aoe = r2_bounds(v2(pengu_king->position.x-50, i32(pengu_king->position.y+pengu_king->size.y/2)), v2(pengu_king->size.x+100, i32(pengu_king->size.y/2)), v2_one, v2_zero);

            if (entity_on_wall(pengu_king) && king_time*60 < 10) {
                draw_pengu(pengu_king, 8);
            } else if (pengu_king->position.y > -pengu_king->size.y && king_time*60 < 180) {
                pengu_king->velocity.x = 0;
                draw_pengu(pengu_king, 9);
                pengu_king->velocity.y = -45000*dt;
            } else if (king_time*60 < 180) {
                pengu_king->velocity.y = 0;
                pengu_king->position = v2(player->position.x, -300);
                pengu_king->facing = player->facing;
            } else if (!entity_on_wall(pengu_king)) {
                if (pengu_king->position.x > stage_size.y-49) pengu_king->position = v2(stage_size.y-49, pengu_king->position.y);
                if (pengu_king->position.x < stage_size.x) pengu_king->position = v2(stage_size.x, pengu_king->position.y);
                draw_pengu(pengu_king, 8);
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
                    player_hit(200);
                }

                

                king_state = KINGLANDING;
                draw_pengu(pengu_king, 10);
            }

                land_aoe = r2_shift(land_aoe, v2(-camera_pos.x+out->width*.5, 0));

                DrawRectOutline(land_aoe, v4_red, 1);
        } break;
    case KINGLANDING:
        {
            if (king_time*60 < 180) {
                if (i32(king_time*60)%20< 10) {
                    draw_pengu(pengu_king, 10);
                } else {
                    draw_pengu(pengu_king, 11);
                }
            } else {
                king_state = KINGNEUTRAL;
                draw_pengu(pengu_king, 0);
            }
        } break;
    }

    if (invuln_time <= 0) {
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(pengu_king->position, pengu_king->size, v2_zero, v2_one)))
        {
            player_hit(pengu_king->enemy.damage);
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

void draw_pengu(Entity *pengu, i32 frame) {
    if (pengu->facing > 0) {
        DrawImageMirrored(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5,
            pengu->position.y + pengu->enemy.offset.y), true, false);
    } else {
        DrawImage(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5 + pengu->enemy.offset.x,
            pengu->position.y+pengu->enemy.offset.y));
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

void pengu_attack(Entity *pengu, Entity *player, i32 invuln_time) {
    if (invuln_time > 0) return;

    Rectangle2 weapon = r2_bounds(v2(pengu->position.x+pengu->enemy.weapon_offset.x, pengu->position.y+pengu->enemy.weapon_offset.y), pengu->enemy.weapon_size, v2_zero, v2_one);
    Rectangle2 rec_player = get_entity_rect(player);

    if (r2_intersects(weapon, rec_player)) {
        player_hit(pengu->enemy.damage);
    }
}

const i32 SOLDIERNEUTRAL = 0;
const i32 SOLDIERMOVE = 1;
const i32 SOLDIERATTACK = 2;
const i32 SOLDIERHURT = 3;

i32 soldier_state = SOLDIERNEUTRAL;
i32 soldier_state_prev = soldier_state;
f32 soldier_time = 0;

void p_soldier_action(Entity *soldier, f32 dt, Entity *player, i32 invuln_time) {
    soldier_time += dt;

    if (player->position.x < soldier->position.x) {
        soldier->facing = -1;
    } else {
        soldier->facing = 1;
    }

    if (soldier_state_prev != soldier_state) {
        soldier_state_prev = soldier_state;
        soldier_time = 0;
    }

    if (soldier_state == SOLDIERNEUTRAL) {
        if (abs_i32(soldier->position.x - player->position.x) < 800 && abs_i32(soldier->position.x - player->position.x) > 200)
        {
            soldier_state = SOLDIERMOVE;
            soldier_state_prev = soldier_state;
            soldier_time = 0;
        } else if (abs_i32(soldier->position.x - player->position.x) < 200)
        {
            soldier_state = SOLDIERATTACK;
            soldier_state_prev = soldier_state;
            soldier_time = 0;
        }
    }

    switch (soldier_state)
    {
    case SOLDIERMOVE:
        {
            soldier->velocity.x = 120*sign_f32(soldier->facing);

            move_pengu(soldier, dt);
            if (abs_i32(soldier->position.x - player->position.x > 200)) {
                if (i32(soldier_time*60)%120 < 60) {
                    draw_pengu(soldier, 0);
                } else {
                    draw_pengu(soldier, 1);
                }
            }else {
                draw_pengu(soldier, 0);
                soldier_state = SOLDIERATTACK;
            }
        } break;
    case SOLDIERATTACK:
        {
            soldier->velocity.x = 120*sign_f32(soldier->facing);

            move_pengu(soldier, dt);
            if (abs_i32(soldier->position.x - player->position.x < 200)) {
                if (i32(soldier_time*60)%120 < 60) {
                    draw_pengu(soldier, 2);
                } else {
                    draw_pengu(soldier, 3);
                }
            }else {
                draw_pengu(soldier, 0);
                soldier_state = SOLDIERMOVE;
            }
            pengu_attack(soldier, player, invuln_time);
        } break;
    case SOLDIERHURT:
        {

        } break;
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