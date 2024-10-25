Entity wall[1000] = {};
int wall_count = 0;
Entity enemys[1000] = {};
int enemy_count = 0;

void move_king(Entity *pengu_king, f32 dt);
void draw_king(Entity *pengu_king, i32 frame, Entity *player);

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

            return big_papa;
        } break;
    default:
        {
            static Image image[1] = {LoadImage(S("penguin_idle1.png"))};

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

void draw_player(Weapon weapon, Vector2 position, i32 frame, i32 facing) {
    if (sign_i32(facing) < 0) {
        DrawImageMirroredX(weapon.image[frame], v2(position.x + weapon.offset_left.x, position.y+weapon.offset_left.y));
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
                        dump(pixel);
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
                    player_hit();
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

i32 king_state = KINGNEUTRAL;
i32 king_state_prev = KINGNEUTRAL;
f32 king_time = 0;


Vector2 stage_size = v2(3840, 4656);

void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt) {


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
                DrawImageMirroredX(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                   pengu_king->position.y+pengu_king->enemy.offset.y));
            } else 
            {
                DrawImage(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                    pengu_king->position.y+pengu_king->enemy.offset.y));
            }

        } break;
    case KINGSTOMPING:
        {
            if (king_time*60 < 5) {
                draw_king(pengu_king, 2, player);
                
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 10) {
                draw_king(pengu_king, 3, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 15) {
                draw_king(pengu_king, 4, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 20) {
                draw_king(pengu_king, 5, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 25) {
                draw_king(pengu_king, 6, player);
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 30) {
                draw_king(pengu_king, 7, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (king_time*60 < 35) {
                draw_king(pengu_king, 4, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else {
                draw_king(pengu_king, 2, player);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
                king_state = KINGNEUTRAL;
            }
        } break;
    case KINGJUMPING:
        {
            if (pengu_king->facing > 0) {
                if (pengu_king->position.x < stage_size.y-49) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                    }
                    pengu_king->velocity.x = 6000*dt;

                    draw_king(pengu_king, 0, player);
                } else {
                    king_state = KINGLEAPING;
                    draw_king(pengu_king, 0, player);
                }
            } else {
                if (pengu_king->position.x > stage_size.x) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                    }
                    pengu_king->velocity.x = -6000*dt;

                    draw_king(pengu_king, 0, player);
                } else {
                    king_state = KINGLEAPING;
                    draw_king(pengu_king, 0, player);
                }
            }   
        } break;
    case KINGLEAPING:
        {
            if (entity_on_wall(pengu_king) && king_time*60 < 10) {
                draw_king(pengu_king, 8, player);
            } else if (pengu_king->position.y > -pengu_king->size.y && king_time*60 < 180) {
                pengu_king->velocity.x = 0;
                draw_king(pengu_king, 9, player);
                pengu_king->velocity.y = -30000*dt;
            } else if (king_time*60 < 180) {
                pengu_king->velocity.y = 0;
                pengu_king->position = v2(player->position.x, -300);
                pengu_king->facing = player->facing;
            } else if (!entity_on_wall(pengu_king)) {
                if (pengu_king->position.x > stage_size.y-49) pengu_king->position = v2(stage_size.y-49, pengu_king->position.y);
                if (pengu_king->position.x < stage_size.x) pengu_king->position = v2(stage_size.x, pengu_king->position.y);
                draw_king(pengu_king, 8, player);
                pengu_king->velocity.y += 2000*dt;
            } else {
                king_state = KINGLANDING;
                draw_king(pengu_king, 10, player);
            }
        } break;
    case KINGLANDING:
        {
            if (king_time*60 < 180) {
                if (i32(king_time*60)%20 < 10) {
                    draw_king(pengu_king, 10, player);
                } else {
                    draw_king(pengu_king, 11, player);
                }
            } else {
                king_state = KINGNEUTRAL;
                draw_king(pengu_king, 0, player);
            }
        } break;
    }

    if (invuln_time <= 0) {
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(pengu_king->position, pengu_king->size, v2_zero, v2_one)))
        {
            player_hit();
        }
    }

    move_king(pengu_king, dt);
    
}

void draw_king(Entity *pengu_king, i32 frame, Entity *player) {
    if (pengu_king->facing > 0) {
        DrawImageMirroredX(pengu_king->enemy.image[frame], v2(pengu_king->position.x-camera_pos.x+out->width*.5 + pengu_king->enemy.offset.x,
            pengu_king->position.y+pengu_king->enemy.offset.y));
    } else {
        DrawImage(pengu_king->enemy.image[frame], v2(pengu_king->position.x-camera_pos.x+out->width*.5 + pengu_king->enemy.offset.x,
            pengu_king->position.y+pengu_king->enemy.offset.y));
    }
}

void move_king(Entity *pengu_king, f32 dt) {
    pengu_king->velocity.y+=496*input->dt;

    f32 dy = pengu_king->velocity.y*dt;
    f32 dx = pengu_king->velocity.x*dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        pengu_king->position.y+=sign_f32(dy);
        if (wall_intersects(pengu_king)) {
            pengu_king->position.y-=sign_f32(dy);
            pengu_king->velocity.y=0;
        }
    }


    for (int i = 0; i < abs_f32(dx); i++) {
        pengu_king->position.x+=sign_f32(dx);
        if (wall_intersects(pengu_king)) {
            pengu_king->position.x-=sign_f32(dx);
            pengu_king->velocity.x=0;
        }
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

            dump(i);
            dump(dx);

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