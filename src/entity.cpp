Entity wall[10000] = {};
int wall_count = 0;
bool boss_walls_active = false;

Entity enemys[1000] = {};
int enemy_count = 0;

Entity npc[200] = {};
int npc_count = 0;

void move_pengu(Entity *pengu_king, f32 dt);
void draw_pengu(Entity *pengu_king, i32 frame);
void draw_clouds(Entity *pengu, f32 dt);
void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image);

struct Background {
    Vector2 position;
    Vector2 size;
    Image image;
    i32 frames;
};

struct Housing {
    Vector2 position;
    Vector2 size;
    Image *image;
    i32 frames;
    f32 state_time;
};

struct Fire {
    Vector2 position;
    Vector2 size;
    Image *image;
    i32 frames;
    f32 state_time;
};

Fire fire = {};

Background bgnd[10000] = {};
i32 bgnd_count = 0;

Housing housing[100] = {};
i32 house_count = 0;

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
            enemy_seal.has_hit = true;

            return enemy_seal;
        } break;
    case 2:
        {
            static Image image[] = 
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
                LoadImage(S("big_papa13.png")),

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
            big_papa.has_hit = true;
            big_papa.state = NEUTRAL;
            big_papa.state_prev = NEUTRAL;
            big_papa.talking = false;

            load_penguin_king_dialogue(&big_papa);

            return big_papa;
        } break;
    case 3:
        {
            static Image image[] =
            {
                LoadImage(S("penguin_soldier1.png")),
                LoadImage(S("penguin_soldier2.png")),
                LoadImage(S("penguin_soldier3.png")),
                LoadImage(S("penguin_soldier4.png")),
                LoadImage(S("penguin_soldier5.png")),
                LoadImage(S("penguin_soldier6.png")),
                LoadImage(S("penguin_soldier7.png")),
                LoadImage(S("penguin_soldier8.png")),

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
            penguin_soldier.enemy.id = enemy_count;
            penguin_soldier.state = NEUTRAL;
            penguin_soldier.state_prev = NEUTRAL;
            penguin_soldier.enemy.exp_dropped = 75;
            penguin_soldier.has_hit = true;

            return penguin_soldier;
        } break;
    case 4:
        {
            static Image image[9] = {
                LoadImage(S("robo_pup1.png")),
                LoadImage(S("robo_pup2.png")),
                LoadImage(S("robo_pup3.png")),
                LoadImage(S("robo_pup4.png")),
                LoadImage(S("robo_pup5.png")),
                LoadImage(S("robo_pup6.png")),
                LoadImage(S("robo_pup7.png")),
                LoadImage(S("robo_pup8.png")),
                LoadImage(S("robo_pup9.png")),
            };

            Entity pup = {};
            pup.max_health = 120;
            pup.current_health = pup.current_health;
            pup.min_health = 0;
            pup.enemy.offset = v2(0, 0);
            pup.position = pos;
            pup.size = v2(39, 96);
            pup.invuln = false;
            pup.alive = true;
            pup.facing = 1;
            pup.image = image;
            pup.type = 5;
            pup.enemy.anchor_pos = pos;
            pup.enemy.damage = 40;
            pup.enemy.id = enemy_count;
            pup.state = NEUTRAL;
            pup.enemy.exp_dropped = 100;
            pup.has_hit = true;

            return pup;
        } break;
    case 5:
        {
            static Image image[] {
                LoadImage(S("slime1.png")),
                LoadImage(S("slime2.png")),
                LoadImage(S("slime3.png")),
                LoadImage(S("slime4.png")),
            };

            Entity slim = {};
            slim.max_health = 220;
            slim.current_health = slim.max_health;
            slim.enemy.offset = v2_zero;
            slim.position = pos;
            slim.size = v2(48, 37);
            slim.invuln = false;
            slim.facing = 1;
            slim.enemy.image = image;
            slim.type = 6;
            slim.enemy.anchor_pos = pos;
            slim.enemy.damage =  30;
            slim.enemy.id = enemy_count;
            slim.state = NEUTRAL;
            slim.enemy.exp_dropped = 100;
            slim.has_hit = true;
            slim.alive = true;

            return slim;
        } break;
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
const i32 lil_pengu = 1;

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
            npc[npc_count].type = fairy;
            npc[npc_count].talking = false;

        } break;
    case lil_pengu:
        {
            static Image image[3] = {LoadImage(S("lil_pengu1.png")),
            LoadImage(S("lil_pengu2.png")),
            LoadImage(S("lil_pengu3.png")),
        };

        npc[npc_count].position = pos;
        npc[npc_count].image = image;
        npc[npc_count].alive = true;
        npc[npc_count].state_time = 0;
        npc[npc_count].animation_time = 0;
        npc[npc_count].dialogue_time = 0;
        npc[npc_count].type = lil_pengu;
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

void make_world(Level level) {
    TimeFunction;
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

    Image world = level.world;


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
                case 1821034495:
                    {
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 4);
                        enemy_count++;
                    } break;
                case 643580671:
                    {
                        //265c42
                        enemys[enemy_count] = load_enemy(v2(i*48, k*48), 5);
                        enemy_count++;
                    } break;
                case -1562516993:
                    {
                        make_npcs(v2(i*48, k*48), fairy);
                        load_fairy_dialogue(&npc[npc_count]);
                        npc_count++;
                    } break;
                case 1969959423:
                    {
                        make_npcs(v2(i*48, k*48), lil_pengu);
                        npc_count++;
                    } break;
                case 753464831: 
                    {
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 48);
                        bgnd[bgnd_count].image = LoadImage(S("ice_grass1.png"));
                        bgnd_count++;
                    } break;
                case 423378687: 
                    {
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = LoadImage(S("pointed_fir.png"));
                        bgnd_count++;
                    } break;
                case 1059599103:
                    {
                        static Image image[3] = {
                            LoadImage(S("peasant_hovel1.png")),
                            LoadImage(S("peasant_hovel2.png")),
                            LoadImage(S("peasant_hovel3.png")),
                        };
                        housing[house_count].position = v2(i*48, k*48);
                        housing[house_count].size = v2(192, 202);
                        housing[house_count].image = image;
                        house_count++;
                    } break;
                case -465877761:
                    {
                        static Image image[] = {
                            LoadImage(S("fire1.png")),
                            LoadImage(S("fire2.png")),
                            LoadImage(S("fire3.png")),
                            LoadImage(S("fire4.png")),
                            LoadImage(S("fire5.png")),
                        };
                        fire.position = v2(i*48, k*48);
                        fire.size = v2(48, 48);
                        fire.image = image;
                        fire.frames = 4;
                        fire.state_time = 0;
                    } break;
                case 403973631:
                    {
                        static Image image = LoadImage(S("cave_wall.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
                    } break;
                case 2139260671:
                    {
                        static Image image = LoadImage(S("cloud_brick_background.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
                    } break;
                    case 640369919:
                    {
                        //262b44
                        static Image image = LoadImage(S("bolted_plates14.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
                    } break;
                case 572998143:
                    {
                        //222741
                        static Image image = LoadImage(S("bolted_plates15.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
                    } break;
                case 741561343:
                    {
                        //2c3353
                        static Image image = LoadImage(S("bolted_plates16.png"));
                        bgnd[bgnd_count].position = v2(i*48, k*48);
                        bgnd[bgnd_count].size = v2(48, 96);
                        bgnd[bgnd_count].image = image;
                        bgnd_count++;
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
                case -521737985:
                    {
                        make_wall(v2(i, k), pixel, S("ice_brick.png"));
                    } break;
                case 7903743:
                    {
                        make_wall(v2(i, k), pixel, S("rocks.png"));
                    } break;
                case 629118975:
                    {
                        make_wall(v2(i, k), pixel, S("stone_brick.png"));
                    } break;
                case 530563839:
                    {
                        make_wall(v2(i, k), pixel, S("rocks_crack.png"));
                    } break;
                case -2003194113:
                    {
                        make_wall(v2(i, k), pixel, S("cloud_brick.png"));
                    } break;
                case -513925889:
                    {
                        //e15e1c
                        make_wall(v2(i, k), pixel, S("pipe6.png"));
                    } break;
                case -663337729:
                    {
                        //d87644
                        make_wall(v2(i, k), pixel, S("pipe1.png"));
                    } break;
                case -595899905:
                    {
                        //dc7b49
                        make_wall(v2(i, k), pixel, S("pipe5.png"));
                    } break;
                case -227990785:
                    {
                        //f26922
                        make_wall(v2(i, k), pixel, S("pipe4.png"));
                    } break;
                case -814463489:
                    {
                        //f26922
                        make_wall(v2(i, k), pixel, S("pipe2.png"));
                    } break;
                case -1102434305:
                    {
                        //f26922
                        make_wall(v2(i, k), pixel, S("pipe7.png"));
                    } break;
                case -1824115457:
                    {
                        //f26922
                        make_wall(v2(i, k), pixel, S("pipe8.png"));
                    } break;
                case -661491969:
                    {
                        //d8926e
                        make_wall(v2(i, k), pixel, S("pipe9.png"));
                    } break;
                case 1937938943:
                    {
                        //738299
                        make_wall(v2(i, k), pixel, S("factory_walls1.png"));
                    } break;
                case 1028485119:
                    {
                        //3d4d6f
                        make_wall(v2(i, k), pixel, S("factory_walls2.png"));
                    } break;
                case 1519048191:
                    {
                        //5a8ad5
                        make_wall(v2(i, k), pixel, S("factory_walls3.png"));
                    } break;
                case 811442943:
                    {
                        //305da2
                        make_wall(v2(i, k), pixel, S("factory_walls4.png"));
                    } break;
                case 658863359:
                    {
                        //274574
                        make_wall(v2(i, k), pixel, S("factory_walls5.png"));
                    } break;
                case 1315674623:
                    {
                        //4e6b99
                        make_wall(v2(i, k), pixel, S("factory_walls6.png"));
                    } break;
                case 574383871:
                    {
                        //223c66d
                        make_wall(v2(i, k), pixel, S("factory_walls7.png"));
                    } break;
                case 557010943:
                    {
                        //21334f
                        make_wall(v2(i, k), pixel, S("factory_walls8.png"));
                    } break;
                case 523457535:
                    {
                        //1f3353
                        make_wall(v2(i, k), pixel, S("factory_walls9.png"));
                    } break;
                case -1128283137:
                    {
                        //bcbfc3
                        make_wall(v2(i, k), pixel, S("bolted_plates1.png"));
                    } break;
                case -1448432385:
                    {
                        //a9aaac
                        make_wall(v2(i, k), pixel, S("bolted_plates2.png"));
                    } break;
                case -1734697217:
                    {
                        //989a9e
                        make_wall(v2(i, k), pixel, S("bolted_plates3.png"));
                    } break;
                case -2037871617:
                    {
                        //86888b
                        make_wall(v2(i, k), pixel, S("bolted_plates4.png"));
                    } break;
                case 2088930303:
                    {
                        //7c828b
                        make_wall(v2(i, k), pixel, S("bolted_plates5.png"));
                    } break;
                case -1902730497:
                    {
                        //8e96a2
                        make_wall(v2(i, k), pixel, S("bolted_plates6.png"));
                    } break;
                case -1987408897:
                    {
                        //898a8b
                        make_wall(v2(i, k), pixel, S("bolted_plates7.png"));
                    } break;
                case -2121954305:
                    {
                        //81858b
                        make_wall(v2(i, k), pixel, S("bolted_plates8.png"));
                    } break;
                case 2021624831:
                    {
                        //787f8b
                        make_wall(v2(i, k), pixel, S("bolted_plates9.png"));
                    } break;
                case 1718185983:
                    {
                        //66696f
                        make_wall(v2(i, k), pixel, S("bolted_plates10.png"));
                    } break;
                case 1532978943:
                    {
                        //5b5f66
                        make_wall(v2(i, k), pixel, S("bolted_plates11.png"));
                    } break;
                case 1448896255:
                    {
                        //565c66
                        make_wall(v2(i, k), pixel, S("bolted_plates12.png"));
                    } break;
                case 1381259519:
                    {
                        //525458
                        make_wall(v2(i, k), pixel, S("bolted_plates13.png"));
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
            if (min.magnet != NULL && min.magnet->type == 0) {
                particles[i].magnet = min.magnet;
                particles[i].sleep_time = .5;
            }
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
            if (particles[i].magnet != NULL && particles[i].magnet->type == 0) {
                particles[i].velocity.x+=300*dt*sign_f32(particles[i].magnet->anchor.x-particles[i].position.x)/*+random_f32_between(-2000, 4000)*dt*/;
                particles[i].velocity.y+=300*dt*sign_f32(particles[i].magnet->anchor.y-particles[i].position.y)/*+random_f32_between(-1200, 2000)*dt*/;

                particles[i].velocity.x = clamp_f32(particles[i].velocity.x, -150, 150);
                particles[i].velocity.y = clamp_f32(particles[i].velocity.y, -150, 150);
            }

            particles_checked++;
            particles[i].position.x += particles[i].velocity.x*dt;
            particles[i].position.y += particles[i].velocity.y*dt;
            particles[i].life_time-=dt;
            DrawImage(particles[i].image, v2(particles[i].position.x-camera_pos.x+out->width*.5, particles[i].position.y));
            if (particles[i].life_time <= 0) {
                particles[i].is_alive = false;
                particles[i].magnet = NULL;
                living_particles--;
            } else if (particles[i].magnet != NULL && abs_f32(particles[i].magnet->anchor.x - particles[i].position.x) < 4
             && abs_f32(particles[i].magnet->anchor.y - particles[i].position.y) < 4 ) {
                particles[i].is_alive = false;
                particles[i].life_time = 0;
                particles[i].magnet = NULL;
                living_particles--;
            }

            if (particles_checked == living_particles) break;
        }
    }
}

void make_boss_walls(Vector2 *pos, i32 count) {
    static Image boss_walls = LoadImage(S("wall_flowers_white.png"));

    for (int i = 0; i < count; i++) {
        wall[wall_count].position = pos[i];
        wall[wall_count].size = v2(48, 48);
        wall[wall_count].wall_type.image = boss_walls;
        wall[wall_count].type = 2;

        wall_count++;
    }
}

void destroy_boss_walls() {
    i32 walls_destroyed = 0;

    for (int i = 0; i < wall_count; i++) {
        if (wall[i].type == 2) {
            wall[i] = {};
            walls_destroyed++;
        }
    }

    wall_count-=walls_destroyed;
}

void draw_enemy(Entity *nme, i32 frame) {
    if (nme->facing > 0) {
        DrawImageMirrored(nme->enemy.image[frame], v2(nme->position.x-camera_pos.x+out->width*.5,
            nme->position.y + nme->enemy.offset.y), true, false);
    } else {
        DrawImage(nme->enemy.image[frame], v2(nme->position.x-camera_pos.x+out->width*.5 + nme->enemy.offset.x,
            nme->position.y+nme->enemy.offset.y));
    }
}

void move_enemy(Entity *nme, f32 dt) {
    nme->velocity.y+=496*input->dt;

    f32 dy = nme->velocity.y*dt;
    f32 dx = nme->velocity.x*dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        nme->position.y+=sign_f32(dy);
        if (wall_intersects(nme) || enemy_overlap(nme)) {
            nme->position.y-=sign_f32(dy);
            nme->velocity.y=0;
            break;
        }
    }


    for (int i = 0; i < abs_f32(dx); i++) {
        nme->position.x+=sign_f32(dx);
        if (wall_intersects(nme) || enemy_overlap(nme)) {
            nme->position.x-=sign_f32(dx);
            nme->velocity.x=0;
            break;
        }
    }
}

#include "enemy_pengus.cpp"
#include "seals.cpp"
#include "slime.cpp"

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

void draw_house(Housing *house) {
    if (house->state_time*60 < 60) {
        DrawImage(house->image[0], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
    } else if (house->state_time*60 < 120) {
        DrawImage(house->image[1], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
    } else {
        DrawImage(house->image[2], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
    }
}

void npc_action(Entity *npc, Entity *player) {
    npc->animation_time+=input->dt;
    npc->state_time+=input->dt;
    npc->dialogue_time+=input->dt;
    
    if (npc->type == fairy && player->fairy_uses == 0) {
        if (i32(npc->animation_time*60)%60 < 30) {
            DrawImage(npc->image[0], v2(npc->position.x-camera_pos.x+out->width*.5, npc->position.y));
        } else {
            DrawImage(npc->image[1], v2(npc->position.x-camera_pos.x+out->width*.5, npc->position.y));
        }

        if (abs_i32(npc->position.x-player->position.x) < 200 && abs_i32(npc->position.y-player->position.y) < 300) {
            if (!npc->talking) {
                npc->talking = draw_dialogue_box(npc->dialogue[0], out, npc->portrait, 2);
            } else {
                player->fairy_uses = 3;
                player->current_fairy_uses = player->fairy_uses;

            }

            /*if (npc->state_time*60.0 < 400) {
                draw_dialogue_box(npc->dialogue[0], out, npc->portrait, 2, npc->dialogue_time*60.0);
            }

            if (npc->state_time*60.0 > 400 && npc->state_time*60.0 < 402 ) {
                npc->dialogue_time = 0;
            }


            if (npc->state_time*60.0 > 400 && npc->state_time*60.0 < 800) {
                draw_dialogue_box(npc->dialogue[1], out, npc->portrait, 2, npc->dialogue_time*60.0);
            }

            if (npc->state_time*60.0 > 800 && npc->state_time*60.0 < 802 ) {
                npc->dialogue_time = 0;
            }


            if (npc->state_time*60.0 > 800) {
                draw_dialogue_box(npc->dialogue[2], out, npc->portrait, 2, npc->dialogue_time*60.0);
            }*/
        } else {
            npc->dialogue_time = 0;
            npc->state_time = 0;
            npc->talking = false;
            for (int i = 0; i < line_count; i++) {
                lines_to_speak[i] = {};
            }
            line_count = 0;
            box_count = 0;
            lines_generated = false;
        }
    } else if (npc->type == lil_pengu) {
        if (npc->state_time*60 < 60) {
            DrawImage(npc->image[0], v2(npc->position.x - camera_pos.x+out->width*.5, npc->position.y));
        } else if (npc->state_time*60 < 120) {
            DrawImage(npc->image[1], v2(npc->position.x - camera_pos.x+out->width*.5, npc->position.y));
        } else {
            DrawImage(npc->image[2], v2(npc->position.x - camera_pos.x+out->width*.5, npc->position.y));
        }

        if (npc->state_time*60 > 180) {
            npc->state_time = 0;
        }
    }
}
