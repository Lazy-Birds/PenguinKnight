bool boss_walls_active = false;

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
            enemy_plant.image = image;
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
            enemy_seal.max_health = 50;
            enemy_seal.current_health = enemy_seal.max_health;
            enemy_seal.min_health = 0;
            enemy_seal.position = pos;
            enemy_seal.check_point = pos;
            enemy_seal.size = v2(32, 32);
            enemy_seal.hitbox = r2_bounds(pos, enemy_seal.size, v2_zero, v2_one);
            enemy_seal.invuln = false;
            enemy_seal.attackable = true;
            enemy_seal.alive = true;
            enemy_seal.image = image;
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
            big_papa.check_point = pos;
            big_papa.size = v2(64, 94);
            big_papa.hitbox = r2_bounds(big_papa.position, big_papa.size, v2_zero, v2_one);
            big_papa.invuln = false;
            big_papa.attackable = true;
            big_papa.alive = true;
            big_papa.facing = -1;
            big_papa.image = image;
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
            penguin_soldier.check_point = penguin_soldier.position;
            penguin_soldier.size = v2(42, 48);
            penguin_soldier.hitbox = r2_bounds(pos, v2(48, 48), v2_zero, v2_one);
            penguin_soldier.invuln = false;
            penguin_soldier.attackable = true;
            penguin_soldier.alive = true;
            penguin_soldier.facing = -1;
            penguin_soldier.image = image;
            penguin_soldier.enemy.type = 4;
            penguin_soldier.enemy.anchor_pos = pos;
            penguin_soldier.enemy.guard = 20;
            penguin_soldier.enemy.weapon_size = v2(56, 8);
            penguin_soldier.enemy.weapon_offset = v2(-27, 33);
            penguin_soldier.enemy.damage = 30;
            penguin_soldier.enemy.id = World[player.player_level].enemies.count;
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
            pup.check_point = pos;
            pup.size = v2(39, 96);
            pup.hitbox = r2_bounds(pos, v2(48, 96), v2_zero, v2_one);
            pup.invuln = false;
            pup.attackable = true;
            pup.alive = true;
            pup.facing = 1;
            pup.image = image;
            pup.type = 5;
            pup.enemy.anchor_pos = pos;
            pup.enemy.damage = 40;
            pup.enemy.id = World[player.player_level].enemies.count;
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
                LoadImage(S("slime5.png")),
                LoadImage(S("slime6.png")),
            };

            static Image dart[] = {
                LoadImage(S("slime_dart.png")),
            };

            Entity slim = {};
            slim.max_health = 100;
            slim.current_health = slim.max_health;
            slim.enemy.offset = v2(0, -11);
            slim.position = pos;
            slim.check_point = pos;
            slim.size = v2(48, 37);
            slim.hitbox = get_entity_rect(&slim);
            slim.invuln = false;
            slim.attackable = true;
            slim.facing = 1;
            slim.image = image;
            slim.type = 6;
            slim.enemy.anchor_pos = pos;
            slim.enemy.damage =  30;
            slim.enemy.id = World[player.player_level].enemies.count;
            slim.state = NEUTRAL;
            slim.enemy.exp_dropped = 100;
            slim.has_hit = true;
            slim.alive = true;
            slim.projectile = dart;
            slim.velocity_prev = v2_zero;

            return slim;
        } break;
    case 6:
        {
            static Image image[] {
                LoadImage(S("eye_monster1.png")),
                LoadImage(S("eye_monster2.png")),
                LoadImage(S("eye_monster3.png")),
            };

            static Image lazer[] = {
                LoadImage(S("lazer1.png")),
                LoadImage(S("lazer2.png")),
            };

            Entity stalker = {};
            stalker.max_health = 80;
            stalker.current_health = stalker.max_health;
            stalker.enemy.offset = v2_zero;
            stalker.position = pos;
            stalker.check_point = pos;
            stalker.size = v2(48, 48);
            stalker.hitbox = r2_bounds(stalker.position, stalker.size, v2_zero, v2_one);
            stalker.invuln = false;
            stalker.attackable = true;
            stalker.facing = 1;
            stalker.image = image;
            stalker.type = 7;
            stalker.enemy.anchor_pos = v2(stalker.position.x + 21, stalker.position.y + 18);
            stalker.enemy.damage = 80;
            stalker.enemy.id = World[player.player_level].enemies.count;
            stalker.state = NEUTRAL;
            stalker.enemy.exp_dropped = 150;
            stalker.has_hit = true;
            stalker.alive = true;
            stalker.projectile = lazer;
            stalker.velocity_prev = v2_zero;

            return stalker;

        } break;
    case 7:
        {
            Entity ooze = {};

            static Image image[] = {
                LoadImage(S("the_ooze1.png")),
                LoadImage(S("the_ooze2.png")),
                LoadImage(S("the_ooze3.png")),
                LoadImage(S("the_ooze4.png")),
                LoadImage(S("the_ooze5.png")),
                LoadImage(S("the_ooze6.png")),
                LoadImage(S("the_ooze7.png")),
                LoadImage(S("the_ooze8.png")),
                LoadImage(S("the_ooze9.png")),
                LoadImage(S("the_ooze10.png")),
                LoadImage(S("the_ooze11.png")),
                LoadImage(S("the_ooze12.png")),
                LoadImage(S("the_ooze13.png")),
                LoadImage(S("the_ooze14.png")),
                LoadImage(S("the_ooze15.png")),
                LoadImage(S("the_ooze16.png")),
                LoadImage(S("the_ooze17.png")),
                LoadImage(S("the_ooze18.png")),
                LoadImage(S("the_ooze19.png")),
                LoadImage(S("the_ooze20.png")),
                LoadImage(S("the_ooze21.png")),
                LoadImage(S("the_ooze22.png")),
                LoadImage(S("the_ooze23.png")),
                LoadImage(S("the_ooze24.png")),
                LoadImage(S("the_ooze25.png")),
                LoadImage(S("the_ooze26.png")),
                LoadImage(S("the_ooze27.png")),
                LoadImage(S("the_ooze28.png")),
            };

            ooze.image = image;
            ooze.max_health = 500;
            ooze.current_health = ooze.max_health;
            ooze.enemy.offset = v2_zero;
            ooze.position = pos;
            ooze.check_point = pos;
            ooze.size = v2(48, 144);
            ooze.hitbox = r2_bounds(v2(pos.x+144, pos.y+48), v2(48, 96), v2_zero, v2_one);
            ooze.invuln = false;
            ooze.attackable = false;
            ooze.facing = 1;
            ooze.type = 8;
            ooze.enemy.anchor_pos = pos;
            ooze.enemy.id = World[player.player_level].enemies.count;
            ooze.enemy.damage = 80;
            ooze.state = NEUTRAL;
            ooze.enemy.exp_dropped = 3000;
            ooze.has_hit = false;
            ooze.alive = true;
            ooze.assymetric = true;
            ooze.offset = v2(144, 0);

            return ooze;
        } break;
    case 8:
        {
            Entity nick = {};

            static Image snick[] = {
                LoadImage(S("nick1.png")),
                LoadImage(S("nick2.png")),
                LoadImage(S("nick3.png")),
                LoadImage(S("nick4.png")),
                LoadImage(S("nick5.png")),
                LoadImage(S("nick6.png")),
                LoadImage(S("nick7.png")),
                LoadImage(S("nick8.png")),
                LoadImage(S("nick9.png")),
            };

            nick.image = snick;
            nick.max_health = 800;
            nick.current_health = nick.max_health;
            nick.enemy.offset = v2_zero;
            nick.position = pos;
            nick.check_point = pos;
            nick.size = v2(72, 81);
            nick.hitbox = r2_bounds(pos, v2(54, 81), v2_zero, v2_one);
            nick.invuln = false;
            nick.attackable = true;
            nick.facing = 1;
            nick.type = 9;
            nick.enemy.anchor_pos = pos;
            nick.enemy.id = World[player.player_level].enemies.count;
            nick.enemy.damage = 40;
            nick.state = NEUTRAL;
            nick.enemy.exp_dropped = 2000;
            nick.has_hit = false;
            nick.alive = true;
            nick.assymetric = false;

            return nick;
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
            idle_penguin.image = image;
            idle_penguin.facing = -1;
            idle_penguin.enemy.type = 2;
            idle_penguin.enemy.offset = v2(0,0);

            return idle_penguin;
        }
    }    
}

const i32 fairy = 0;
const i32 lil_pengu = 1;
const i32 mayor_snoresly = 2;
const i32 pengu_pullers = 3;

void make_npcs(Vector2 pos, i32 type) {
    Level *level = &World[player.player_level];

    switch (type)
    {
    case fairy:
        {
            static Image image[2] = {LoadImage(S("fairy_orb1.png")), LoadImage(S("fairy_orb2.png"))};
            static Image portrait[2] = {LoadImage(S("fairy_orb_portrait1.png")), LoadImage(S("fairy_orb_portrait2.png"))};

            World[player.player_level].npcs.data[World[player.player_level].npcs.count].position = pos;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].image = image;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].alive = true;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].state_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].animation_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].dialogue_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].portrait = portrait;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].type = fairy;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].talking = false;

        } break;
    case lil_pengu:
        {
            static Image image[3] = {LoadImage(S("lil_pengu1.png")),
            LoadImage(S("lil_pengu2.png")),
            LoadImage(S("lil_pengu3.png")),
            };

            World[player.player_level].npcs.data[World[player.player_level].npcs.count].position = pos;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].image = image;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].alive = true;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].state_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].animation_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].dialogue_time = 0;
            World[player.player_level].npcs.data[World[player.player_level].npcs.count].type = lil_pengu;
        } break;
    case mayor_snoresly:
        {
            static Image image[] = {
                LoadImage(S("mayor_snoresly1.png")),
                LoadImage(S("mayor_snoresly2.png")),
            };

            level->npcs.data[level->npcs.count].position = pos;
            level->npcs.data[level->npcs.count].image = image;
            level->npcs.data[level->npcs.count].alive = true;
            level->npcs.data[level->npcs.count].state_time = 0;
            level->npcs.data[level->npcs.count].animation_time = 0;
            level->npcs.data[level->npcs.count].dialogue_time = 0;
            level->npcs.data[level->npcs.count].type = mayor_snoresly;
        } break;
    case pengu_pullers:
        {
            static Image image[] = {
                LoadImage(S("pengu_puller1.png")),
                LoadImage(S("pengu_puller2.png")),
            };

            level->npcs.data[level->npcs.count].position = pos;
            level->npcs.data[level->npcs.count].image = image;
            level->npcs.data[level->npcs.count].alive = true;
            level->npcs.data[level->npcs.count].state_time = 0;
            level->npcs.data[level->npcs.count].animation_time = 0;
            level->npcs.data[level->npcs.count].dialogue_time = 0;
            level->npcs.data[level->npcs.count].type = pengu_pullers;
        } break;
    }
}

void make_wall(Vector2 pos, u32 pixel, Image *image) {
    World[player.player_level].wall.data[World[player.player_level].wall.count].position = pos;
    World[player.player_level].wall.data[World[player.player_level].wall.count].size = v2(48, 48);
    World[player.player_level].wall.data[World[player.player_level].wall.count].wall_type.pixel = pixel;
    World[player.player_level].wall.data[World[player.player_level].wall.count].image = image;

    World[player.player_level].wall.count++;
}

void make_custom_wall(Vector2 pos, u32 pixel, Image *image, Vector2 size) {
    Level *level = &World[player.player_level];
    level->wall.data[level->wall.count].position = pos;
    level->wall.data[level->wall.count].size = size;
    level->wall.data[level->wall.count].wall_type.pixel = pixel;
    level->wall.data[level->wall.count].image = image;

    level->wall.count++;
}

void make_world(Level level) {
    TimeFunction;
    /*wall.data[0].position.y = out->height-32;
    wall.data[0].position.x = 32;
    wall.data[0].size = v2(32, 32);
    wall.data[0].color = v4_yellow;

    wall.data[1].position.y = out->height-32;
    wall.data[1].position.x = out->width-64;
    wall.data[1].size = v2(32, 32);
    wall.data[1].color = v4_yellow;

    wall.data[2].position.y = out->height-64;
    wall.data[2].position.x = out->width-64;
    wall.data[2].size = v2(32, 32);
    wall.data[2].color = v4_yellow;*/

    Image world = level.world;

    World[player.player_level].initialized = true;


    for (int i = 0; i < world.size.x; i+=48) {
        for (int k = 0; k < world.size.y; k+=48) {
            u32 pixel = image_get_pixel(world, i, k);
            if (ARCH_LITTLE_ENDIAN) {
                pixel = endian_swap_u32(pixel);
            }
            
            if (image_get_pixel(world, i, k)!=0) {
                switch (pixel)
                {
                case -1405996289: 
                    {
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 0);
                        World[player.player_level].enemies.count++;
                    } break;
                case -1060381441: 
                    {
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 1);
                        World[player.player_level].enemies.count++;
                    } break;
                case -18390529:
                    {
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 2);
                        World[player.player_level].enemies.count++;
                    } break;
                case 1516865791:
                    {
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 3);
                        World[player.player_level].enemies.count++;
                    } break;
                case 1821034495:
                    {
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 4);
                        World[player.player_level].enemies.count++;
                    } break;
                case 643580671:
                    {
                        //265c42
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 5);
                        World[player.player_level].enemies.count++;
                    } break;
                case -1253013249:
                    {
                        //b55088
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 6);
                        World[player.player_level].enemies.count++;
                    } break;
                case 595415807:
                    {
                        //237d52
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 7);
                        World[player.player_level].enemies.count++;
                    } break;
                case 1748528383:
                    {
                        //68386c
                        World[player.player_level].enemies.data[World[player.player_level].enemies.count] = load_enemy(v2(i, k), 8);
                        World[player.player_level].enemies.count++;
                    } break;
                case -1562516993:
                    {
                        make_npcs(v2(i, k), fairy);
                        load_fairy_dialogue(&World[player.player_level].npcs.data[World[player.player_level].npcs.count]);
                        World[player.player_level].npcs.count++;
                    } break;
                case 1969959423:
                    {
                        make_npcs(v2(i, k), lil_pengu);
                        World[player.player_level].npcs.count++;
                    } break;
                case 1803064575:
                    {
                        make_npcs(v2(i, k), mayor_snoresly);
                        World[player.player_level].npcs.count++;
                    } break;
                case -327140609:
                    {
                        make_npcs(v2(i, k), pengu_pullers);
                        World[player.player_level].npcs.count++;
                    } break; 
                case 1059599103:
                    {
                        static Image image[3] = {
                            LoadImage(S("peasant_hovel1.png")),
                            LoadImage(S("peasant_hovel2.png")),
                            LoadImage(S("peasant_hovel3.png")),
                        };

                        World[player.player_level].housing.data[World[player.player_level].housing.count].position = v2(i, k);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].size = v2(192, 202);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].image = image;
                        World[player.player_level].housing.data[World[player.player_level].housing.count].type = 0;
                        World[player.player_level].housing.count++;
                    } break;
                case 659922687:
                    {
                        static Image image[] = {
                            LoadImage(S("clock_tower.png")),
                        };

                        World[player.player_level].housing.data[World[player.player_level].housing.count].position = v2(i, k);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].size = v2(64, 240);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].image = image;
                        World[player.player_level].housing.data[World[player.player_level].housing.count].type = 1;
                        World[player.player_level].housing.count++;
                    } break;
                case -1017357057:
                    {
                        //c35c5c
                        make_interactible(v2(i, k), 4, level.id, 0);
                    } break;
                case -465877761:
                    {
                        //e43b44
                        make_interactible(v2(i, k), 4, level.id, 0);
                    } break;
                case 403973631:
                    {
                        static Image image[] = {LoadImage(S("cave_wall.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 0;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 2139260671:
                    {
                        static Image image[] = {LoadImage(S("cloud_brick_background.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 1;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 640369919:
                    {
                        //262b44
                        static Image image[] = {LoadImage(S("bolted_plates14.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 2;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 572998143:
                    {
                        //222741
                        static Image image[] = {LoadImage(S("bolted_plates15.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 3;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 741561343:
                    {
                        //2c3353
                        static Image image[] = {LoadImage(S("bolted_plates16.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 4;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 1049184511:
                    {
                        //2c3353
                        static Image image[] = {LoadImage(S("bolted_plates16.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 5;
                        World[player.player_level].backgrounds.count++;
                    } break;
                    case 977561343:
                    {
                        //3a4466
                        static Image image[] = {LoadImage(S("pipe_opening.png"))};
                        static Image drip[] = {LoadImage(S("sewer_drip.png"))};
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 6;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].projectile = drip;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 2102019327:
                    {
                        //7d4a44
                        static Image image[] = {
                            LoadImage(S("work_rope1.png")),
                            LoadImage(S("work_rope2.png")),
                    };
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(288, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 7;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case -1229075457:
                    {
                        //b6bdcb
                        static Image image[] = {
                            LoadImage(S("spikes.png")),
                    };
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(48, 48);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 8;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case 2033857535:
                    {
                        //
                        static Image image[] = {
                            LoadImage(S("badger_tent.png")),
                    };
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].position = v2(i, k);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].size = v2(96, 96);
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].image = image;
                        World[player.player_level].backgrounds.data[World[player.player_level].backgrounds.count].id = 9;
                        World[player.player_level].backgrounds.count++;
                    } break;
                case -372160769:
                    {
                        static Image image[] = {
                            LoadImage(S("street_lamp1.png")),
                            LoadImage(S("street_lamp2.png")),
                        };

                        //e9d146

                        World[player.player_level].housing.data[World[player.player_level].housing.count].position = v2(i, k);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].size = v2(48, 96);
                        World[player.player_level].housing.data[World[player.player_level].housing.count].image = image;
                        World[player.player_level].housing.data[World[player.player_level].housing.count].type = 2;
                        World[player.player_level].housing.count++;
                    } break;
                case 1050692607:
                    {
                        //3ea04b
                        World[player.player_level].liquid.data[World[player.player_level].liquid.count] = make_liquid(v2(i, k), 0);
                        World[player.player_level].liquid.count++;
                    } break;
                case 423378687:
                    {
                        //193c3e
                        World[player.player_level].liquid.data[World[player.player_level].liquid.count] = make_liquid(v2(i, k), 1);
                        World[player.player_level].liquid.count++;
                    } break;
                case -16759553:
                    {
                        //ff0044
                        make_interactible(v2(i, k), 0, level.id, 0);
                    } break;
                case -1958396417:
                    {
                        //8b453d
                        make_interactible(v2(i, k), 1, level.id, 0);
                    } break;
                case 1950037503:
                    {
                        //743b35
                        make_interactible(v2(i, k), 1, level.id, 1);
                    } break;
                case 776045311:
                    {
                        //2e4182
                        make_interactible(v2(i, k), 3, level.id, 0);
                    } break;
                case 1837220863:
                    {
                        //6d81c3
                        make_interactible(v2(i, k), 5, level.id, 0);
                    } break;
                case 1432922111:
                    {
                        //
                        make_interactible(v2(i, k), 5, level.id, 2);
                    } break;
                case -982528769:
                    {
                        //
                        make_interactible(v2(i, k), 6, level.id, 0);
                    } break;
                case 842807807:
                    {
                        static Image image[] = {LoadImage(S("grass_bot.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1713024769:
                    {
                        static Image image[] = {LoadImage(S("grass_top.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1437261311: 
                    {
                        static Image image[] = {LoadImage(S("dirt_covered_snow.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1715024383:
                    {
                        static Image image[] = {LoadImage(S("dirt.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -521737985:
                    {
                        static Image image[] = {LoadImage(S("ice_brick.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 7903743:
                    {
                        static Image image[] = {LoadImage(S("rocks.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 629118975:
                    {
                        static Image image[] = {LoadImage(S("stone_brick.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 530563839:
                    {
                        static Image image[] = {LoadImage(S("rocks_crack.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -2003194113:
                    {
                        static Image image[] = {LoadImage(S("cloud_brick.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -513925889:
                    {
                        static Image image[] = {LoadImage(S("pipe6.png"))};
                        //e15e1c
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -663337729:
                    {
                        static Image image[] = {LoadImage(S("pipe1.png"))};
                        //d87644
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -595899905:
                    {
                        static Image image[] = {LoadImage(S("pipe5.png"))};
                        //dc7b49
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -227990785:
                    {
                        static Image image[] = {LoadImage(S("pipe4.png"))};
                        //f26922
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -814463489:
                    {
                        static Image image[] = {LoadImage(S("pipe2.png"))};
                        //f26922 
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1102434305:
                    {
                        static Image image[] = {LoadImage(S("pipe7.png"))};
                        //f26922
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1824115457:
                    {
                        static Image image[] = {LoadImage(S("pipe8.png"))};
                        //f26922
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -661491969:
                    {
                        static Image image[] = {LoadImage(S("pipe9.png"))};
                        //d8926e
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1937938943:
                    {
                        static Image image[] = {LoadImage(S("factory_walls1.png"))};
                        //738299
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1028485119:
                    {
                        static Image image[] = {LoadImage(S("factory_walls2.png"))};
                        //3d4d6f
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1519048191:
                    {
                        static Image image[] = {LoadImage(S("factory_walls3.png"))};
                        //5a8ad5
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 811442943:
                    {
                        static Image image[] = {LoadImage(S("factory_walls4.png"))};
                        //305da2
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 658863359:
                    {
                        static Image image[] = {LoadImage(S("factory_walls5.png"))};
                        //274574
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1315674623:
                    {
                        static Image image[] = {LoadImage(S("factory_walls6.png"))};
                        //4e6b99
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1834428929:
                    {
                        static Image image[] = {LoadImage(S("factory_walls7.png"))};
                        //223c66d
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1951998721:
                    {
                        static Image image[] = {LoadImage(S("factory_walls8.png"))};
                        //8ba6dc
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -2103254529:
                    {
                        static Image image[] = {LoadImage(S("factory_walls9.png"))};
                        //82a2e1
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1047046655:
                    {
                        static Image image[] = {LoadImage(S("factory_walls10.png"))};
                        //3e68a9
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 744335871:
                    {
                        static Image image[] = {LoadImage(S("factory_walls11.png"))};
                        //2c5da9
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 593013247:
                    {
                        //2358a9
                        static Image image[] = {LoadImage(S("factory_walls12.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 408005119:
                    {
                        //1851a9
                        static Image image[] = {LoadImage(S("factory_walls13.png"))};
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1128283137:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates1.png"))};
                        //bcbfc3
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1448432385:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates2.png"))};
                        //a9aaac
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1734697217:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates3.png"))};
                        //989a9e
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -2037871617:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates4.png"))};
                        //86888b
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 2088930303:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates5.png"))};
                        //7c828b
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1902730497:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates6.png"))};
                        //8e96a2
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1987408897:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates7.png"))};
                        //898a8b
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -2121954305:
                    {
                        static Image image[] ={LoadImage(S("bolted_plates8.png"))};
                        //81858b
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 2021624831:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates9.png"))};
                        //787f8b
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1718185983:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates10.png"))};
                        //66696f
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1532978943:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates11.png"))};
                        //5b5f66
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1448896255:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates12.png"))};
                        //565c66
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case 1381259519:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates13.png"))};
                        //525458
                        make_wall(v2(i, k), pixel, image);
                    } break;
                case -1751011073:
                    {
                        static Image image[] = {LoadImage(S("broken_beam.png"))};
                        //525458
                        make_custom_wall(v2(i, k), pixel, image, v2(96, 192));

                    } break;
                case 979781887:
                    {
                        static Image image[] = {LoadImage(S("factory_grate.png"))};
                        //3a6648
                        make_wall(v2(i, k), pixel, image);
                    } break;
                default:
                    {
                        static Image image[] = {LoadImage(S("penguin_idle.png"))};
                        make_wall(v2(i, k), pixel, image);
                        Dump((u32)pixel);
                    } break;
                }; 
                
            }
        }
    }
}

EntityArray make_entity_array(i32 size) {
    EntityArray arr = {};
    arr.data = (Entity*) malloc(size*sizeof(Entity));
    arr.count = 0;
    arr.capacity = size;

    return arr;
}

EntityArray make_entity_array_two(Arena *arena, i32 size) {
    EntityArray arr = {};

    arr.data = PushArrayZero(arena, Entity, size);
    arr.count = 0;
    arr.capacity = size;

    return arr;
}

/*EntityArray add_to_pos(EntityArray arr, Entity entity, i32 pos) {
    EntityArray arr_ret = {};
    Entity ar_data[arr.capacity] = {};

    arr_ret.data = ar_data;
    arr_ret.count = 0;
    arr_ret.capacity = arr.capacity;

    i32 count;

    for (int i = 0; i < arr.count+1; i++) {
        if (i == pos) {
            arr_ret.data[i] = entity;
            arr_ret.count++;
        } else {
            arr_ret.data[i] = arr.data[count];
            arr_ret.count++;
            count++;
        }
    }

    return arr;
}

EntityArray* get_array_with_id(EntityArray arr, i32 id) {
    EntityArray* arr_ret = {};

    Enitty data[arr.count] = {};
    arr_ret.count = 0;
    arr_ret.capacity = arr.count;

    for (int i = 0; i < arr.count; i++) {
        if (arr.data[i].id == id) {
            arr_ret.data[arr_ret.count] = arr.data[i];
            arr_ret.count++;
        }
    }

    return arr_ret;
}

i32 compare_entity_position(const void *one, const void *two) {
    Entity *a0 = (Entity *)one;
    Entity *b0 = (Entity *)two;

    return a0->position.x - b0->position.x;
}

/*void return_gate(EntityArray arr, Entity *pos) {
    EntityArray gates = {};
    Entity data[arr.capacity] = {};
    gates.data = data;
    gates.capacity = arr.capacity;
    gates.count = 0;

    for (int i = 0; i < arr.count; i++) {
        if (gates.count == 0) {
            gates.data[0] = arr.data[i];
        }
        b32 added = false;
        for (int k = 0; k < gates.count; k++) {
            if (entity_get_distance_x(&arr.data[i], pos) < entity_get_distance_x(gates.data[k], pos)) {
                gates = add_to_pos(gates, arr.data[i], k);
                added = true;
            }
        }

        if (!added) {
            gates.data[gates.count] = arr.data[i];
            gates.count++;
        }
    }

    return gates;
}*/

const i32 SEWERWATER = 0;
const i32 SEWERWATERSTILL = 1;
const i32 RAWSEWERWATERSTILL = 2;

Entity make_liquid(Vector2 pos, i32 id) {
    Entity entity = {};

    switch (id)
    {
    case SEWERWATER:
        {
            static Image image[] = {
                LoadImage(S("sewer_water1.png")),
                LoadImage(S("sewer_water2.png")),
                LoadImage(S("sewer_water3.png")),
            };

            entity.image = image;
            entity.position = pos;
            entity.size = v2(48, 48);
            entity.id = id;
            entity.state_time = 0;
            entity.sprite_index = 14;
        } break;
    case SEWERWATERSTILL:
        {
            static Image image[] = {
                LoadImage(S("sewer_water_still.png")),
            };

            entity.image = image;
            entity.position = pos;
            entity.size = v2(48, 48);
            entity.id = id;
            entity.state_time = 0;
            entity.sprite_index = 14;
        } break;
    case RAWSEWERWATERSTILL:
        {
            static Image image[] = {
                LoadImage(S("raw_sewer_water.png")),
            };

            entity.image = image;
            entity.position = pos;
            entity.size = v2(48, 48);
            entity.id = id;
            entity.state_time = 0;
            entity.sprite_index = 15;
        } break;
    default:
        {
            Dump(S("Something went wrong."));
        } break;
    }

    return entity;
}

void liquid_do_liquid(Entity *liquid, Game_Input *input, i32 count, Game_Output *out) {
    
    Vector2 top_pos = v2(liquid->position.x+camera_offset, liquid->position.y+48-liquid->sprite_index*3);
    Vector2 slice_size = v2(48, 3);

    Rectangle2 top_rec = r2_bounds(top_pos, slice_size, v2_zero, v2_one);

    DrawRectOutline(top_rec, v4_red, 1);

    DrawImageRow(liquid->image[0], top_pos, slice_size, 0);

    for (int i = 0; i < liquid->sprite_index; i++) {
        DrawImageRow(liquid->image[0], v2(top_pos.x, top_pos.y+3+3*i), slice_size, 3);
    }
}

Entity get_wall_at(Vector2 pos) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (World[player.player_level].wall.data[i].position.x == pos.x && World[player.player_level].wall.data[i].position.y == pos.y) {
            return World[player.player_level].wall.data[i];
        }
    }

    Entity blank = {};

    return blank;
}

void reset_enemies() {
    Level *level = &
    World[player.player_level];

    for (int i = 0; i < level->enemies.count; i++) {
        level->enemies.data[i].alive = true;
        level->enemies.data[i].position = level->enemies.data[i].check_point;
        level->enemies.data[i].current_health = level->enemies.data[i].max_health;
        level->enemies.data[i].velocity = v2_zero;
        level->enemies.data[i].state = NEUTRAL;
        level->enemies.data[i].sprite_index = 0;
    }
}

bool enemy_on_enemy(Entity *entity_one) {
    for (int i = 0; i < World[player.player_level].enemies.count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+1), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(World[player.player_level].enemies.data[i].position.x, World[player.player_level].enemies.data[i].position.y), World[player.player_level].enemies.data[i].size, v2_zero, v2_one))) {

            if (entity_one->enemy.id != World[player.player_level].enemies.data[i].enemy.id) {
                return true;
            }
        } 
    }

    return false;
}

bool entity_on_wall(Entity *entity_one) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+1), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(World[player.player_level].wall.data[i].position.x, World[player.player_level].wall.data[i].position.y), World[player.player_level].wall.data[i].size, v2_zero, v2_one))) {
            return true;
        } 
    }

    return false;
}

bool entity_against_wall(Entity *ent) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->wall.count; i++)
    {
        if (r2_intersects(get_entity_rect(&level->wall.data[i]), r2_bounds(v2(ent->position.x+sign_f32(ent->facing), ent->position.y), ent->size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    return false;
}

bool wall_intersects(Entity *entity) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (r2_intersects(r2_bounds(entity->position, entity->size, v2_zero, v2_one), r2_bounds(v2(World[player.player_level].wall.data[i].position.x, World[player.player_level].wall.data[i].position.y), World[player.player_level].wall.data[i].size, v2_zero, v2_one))) {
            return true;
        }
    }

    for (int i = 0; i < World[player.player_level].interactible.count; i++) {
        if (World[player.player_level].interactible.data[i].type == 1 && r2_intersects(get_entity_rect(entity), get_entity_rect(&World[player.player_level].interactible.data[i])))
        {
            return true;
        }
    }
    return false;
}

bool wall_intersects_rec(Rectangle2 rec) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (r2_intersects(get_entity_rect(&World[player.player_level].wall.data[i]), rec)) {
            return true;
        }
    }

    return false;
}

bool wall_ahead(Entity *entity) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (entity->facing > 0)
        {
            if (r2_intersects(r2_bounds(v2(entity->position.x+1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(World[player.player_level].wall.data[i].position.x, World[player.player_level].wall.data[i].position.y), World[player.player_level].wall.data[i].size, v2_zero, v2_one)))
            {
                return true;
            }
        } else if (r2_intersects(r2_bounds(v2(entity->position.x-1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(World[player.player_level].wall.data[i].position.x, World[player.player_level].wall.data[i].position.y), World[player.player_level].wall.data[i].size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    for (int i = 0; i < World[player.player_level].interactible.count; i++) {
        if (entity->facing > 0)
        {
            if (r2_intersects(r2_bounds(v2(entity->position.x+1, entity->position.y-1), entity->size, v2_zero, v2_one), 
                r2_bounds(v2(World[player.player_level].interactible.data[i].position.x, World[player.player_level].interactible.data[i].position.y), World[player.player_level].interactible.data[i].size, v2_zero, v2_one)))
            {
                return true;
            }
        } else if (r2_intersects(r2_bounds(v2(entity->position.x-1, entity->position.y-1), entity->size, v2_zero, v2_one),
            r2_bounds(v2(World[player.player_level].interactible.data[i].position.x, World[player.player_level].interactible.data[i].position.y), World[player.player_level].interactible.data[i].size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    return false;
}

bool entity_in_air(Entity *entity_one) {
    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+2), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(World[player.player_level].wall.data[i].position.x, World[player.player_level].wall.data[i].position.y), World[player.player_level].wall.data[i].size, v2_zero, v2_one))) {
            return false;
    }
}
return true;
}

f32 entity_get_distance_x(Entity *one, Entity *two) {
    return abs_i32(one->position.x - two->position.x);
}

f32 entity_get_distance_y(Entity *one, Entity *two) {
    return (abs_i32(one->position.y - two->position.y));
}

f32 entity_get_anchor_distance_direct(Entity *one, Entity *two) {
    f32 distance_x = abs_f32(one->anchor.x - two->anchor.x);
    f32 distance_y = abs_f32(one->anchor.y - two->anchor.y);

    f32 distance = Sqrt(Pow(distance_x, 2) + Pow(distance_y, 2));

    return distance;
}

i32 get_entity_direction(Entity *entity) {
    return sign_i32(player.position.x - entity->position.x);
}

b32 clear_line_of_sight(Entity *one, Entity *two) {
    TimeFunction;
    Level *level = &World[player.player_level];

    f32 radians = angle_between(one->position, two->position);

    Vector2 step = v2_arm(radians);

    f32 distance = entity_get_anchor_distance_direct(one, two);

    for (int i = 0; i < distance; i++) {
        for (int k = 0; k < level->wall.count; k++) {
            if (r2_contains(get_entity_rect(&level->wall.data[k]), v2(one->position.x+i*step.x, one->position.y+i*step.y))) {
                return false;
            }
        }
    }

    return true;
}

const i32 particle_count = 10000;
i32 living_particles = 0;

Particle particles[particle_count] = {};

void reset_particles() {
    for (int i = 0; i < particle_count; i++) {
        particles[i].is_alive = false;
    }
}

void particle_create(Vector2 pos, Vector2 velocity, f32 lifetime, /*Vector4 color, Vector2 accel,*/ Image image) {
    for (int i = 0; i < particle_count; i++) 
    {
        if (!particles[i].is_alive) 
        {
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
    static Image boss_walls[] = {LoadImage(S("wall_flowers_white.png"))};

    for (int i = 0; i < count; i++) {
        World[player.player_level].wall.data[World[player.player_level].wall.count].position = pos[i];
        World[player.player_level].wall.data[World[player.player_level].wall.count].size = v2(48, 48);
        World[player.player_level].wall.data[World[player.player_level].wall.count].image = boss_walls;
        World[player.player_level].wall.data[World[player.player_level].wall.count].type = 2;

        World[player.player_level].wall.count++;
    }
}

void destroy_boss_walls() {
    i32 walls_destroyed = 0;

    for (int i = 0; i < World[player.player_level].wall.count; i++) {
        if (World[player.player_level].wall.data[i].type == 2) {
            World[player.player_level].wall.data[i] = {};
            walls_destroyed++;
        }
    }

    World[player.player_level].wall.count-=walls_destroyed;
}

void draw_boss_health_bar(Entity *boss) {
    DrawRect(r2_bounds(v2(out->width*.5-302, out->height-50), v2(604, 12), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(out->width*.5-300, out->height-48), v2((boss->current_health/boss->max_health)*600, 8), v2_zero, v2_one), v4_red);
}

void draw_enemy(Entity *nme, i32 frame) {
    if (nme->assymetric){
        if (nme->facing > 0) {
            DrawImageMirrored(nme->image[frame], v2(nme->position.x-camera_pos.x+out->width*.5+nme->offset.x,
            nme->position.y+nme->offset.y), true, false);
        } else {
            DrawImage(nme->image[frame], v2(nme->position.x-camera_pos.x+out->width*.5,
            nme->position.y));
        }
    } else {
        if (nme->facing > 0) {
            DrawImageMirrored(nme->image[frame], v2(nme->position.x-camera_pos.x+out->width*.5,
            nme->position.y + nme->enemy.offset.y), true, false);
        } else {
            DrawImage(nme->image[frame], v2(nme->position.x-camera_pos.x+out->width*.5 + nme->enemy.offset.x,
            nme->position.y+nme->enemy.offset.y));
        }
    }
}

void move_enemy(Entity *nme, f32 dt) {
    f32 y_moved = 0;
    f32 x_moved = 0;

    if (nme->enemy.type != 7) {
        nme->velocity.y+=496*input->dt;
    } else {
        nme->velocity.y = 0;
    }

    f32 dy = nme->velocity.y*dt;
    f32 dx = nme->velocity.x*dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        nme->position.y+=sign_f32(dy);
        y_moved +=sign_f32(dy);
        if (wall_intersects(nme) /*|| enemy_overlap(nme)*/) {
        nme->position.y-=sign_f32(dy);
        y_moved-=sign_f32(dy);
        nme->velocity.y=0;
        break;
    }
}


    for (int i = 0; i < abs_f32(dx); i++) {
        nme->position.x+=sign_f32(dx);
        x_moved += sign_f32(dx);
        if (wall_intersects(nme) /*|| enemy_overlap(nme)*/) {
            nme->position.x-=sign_f32(dx);
            x_moved -= sign_f32(dx);
            nme->velocity.x=0;
            break;
        }
    }

    nme->hitbox = r2_shift(nme->hitbox, v2(x_moved, y_moved));

    Rectangle2 draw_box = r2_shift(nme->hitbox, v2(camera_offset, 0));

    DrawRectOutline(draw_box, v4_red, 2);
}

Entity projectiles[1000] = {};
i32 proj_count = 0;

void reset_projectiles() {
    for (int i = 0; i < 1000; i++) {
        projectiles[i].alive = false;
    }
}

i32 get_prejectile_slot() {
    assert(proj_count != 1000);

    for (int i = 0; i < 1000; i++) {
        if (!projectiles[i].alive)
        {
            return i;
        }
    }

    return proj_count;
}

void make_projectile(Image *image, Vector2 pos, Vector2 vel, i32 sprite_index, i32 id) {

    i32 slot = get_prejectile_slot();

    projectiles[slot].id = id;
    projectiles[slot].image = image;
    projectiles[slot].sprite_index = sprite_index;
    projectiles[slot].state_time = 0;
    projectiles[slot].position = pos;
    projectiles[slot].size = v2(image[0].size.x, image[0].size.y);
    projectiles[slot].velocity = vel;
    projectiles[slot].alive = true;
    projectiles[slot].enemy.offset = v2_zero;
    projectiles[slot].enemy.type = 7;
    projectiles[slot].enemy.damage = 20;
    if (projectiles[slot].velocity.x > 0) {
        projectiles[slot].facing = 1;
    } else {
        projectiles[slot].facing = -1;
    }
    proj_count++;
    draw_enemy(projectiles, 0);
}

void update_projectiles(Game_Input *input, Entity *player) {
    Level *level = &World[player->player_level];

    for (int i = 0; i < 1000; i++) {
        if (projectiles[i].alive) {
            draw_bound_box(&projectiles[i]);
            
            f32 dy = projectiles[i].velocity.y*input->dt;
            f32 dx = projectiles[i].velocity.x*input->dt;

            projectiles[i].state_time+=input->dt;
            if (projectiles[i].state_time*60 >= 200)
            {
                projectiles[i].alive = false;
                proj_count--;
            }

            move_enemy(&projectiles[i], input->dt);

            //Draw based on sprite frames
            if (projectiles[i].sprite_index == 0) {
                draw_enemy(&projectiles[i], 0);
            } else {
                emit_backwards_particles(&projectiles[i], input);
                draw_enemy(&projectiles[i], 1);
            }

            if (entity_against_wall(&projectiles[i])) {
                projectiles[i].alive = false;
                proj_count--;
            }

            if (projectiles[i].id == 1){ 
                    for (int k = 0; k < level->enemies.count; k++) {
                        if (r2_intersects(get_entity_rect(&level->enemies.data[k]), get_entity_rect(&projectiles[i])))
                        {
                            projectiles[i].alive = false;
                            proj_count--;

                            i32 damage = 40;

                            level->enemies.data[k].current_health-=damage;

                            if (World[player->player_level].enemies.data[k].has_hit) {
                                World[player->player_level].enemies.data[k].state = HIT;
                            }
                            
                            if (level->enemies.data[k].current_health <= damage) {
                                level->enemies.data[k].current_health = 0;
                                level->enemies.data[k].state = DYING;
                                level->enemies.data[k].state_time = 0;
                                player->exp_gained += level->enemies.data[k].enemy.exp_dropped;
                                if (player->mp_cooldown <= 0) {
                                    player->current_mp+=10;
                                    player->current_mp = clamp_f32(player->current_mp, 0, player->max_mp);
                                    player->mp_cooldown+=.5;
                                }
                                
                            }
                        }
                    }
                }

            if (invuln_time <= 0){
                if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(projectiles[i].position, projectiles[i].size, v2_zero, v2_one)))
                {
                    projectiles[i].alive = false;
                    proj_count--;
                    player_hit(&projectiles[i], input);
                }
            }
        }
    }
}

void emit_backwards_particles(Entity *ent, Game_Input *input) {
    /*Particle_Parameters min = {};
    Particle_Parameters max = {};

    min.position = v2(ent->position.x+10, ent->position.y+10);
    min.velocity = v2(-ent->velocity.x, ent->velocity.y);
    min.life_time = 30*input->dt;

    max.position = v2(ent->position.x+15, ent->position.y+30);
    max.velocity = v2(-(ent->velocity.x+sign_f32(ent->velocity.x)*200*input->dt), ent->velocity.y);
    max.life_time = 45*input->dt;

    Image snow = LoadImage(S("snow_flake.png"));

    particle_emit(min, max, snow);*/

    static Image lazer_ball_stripped = LoadImage(S("trail.png"));

    particle_create(v2(ent->position.x+ent->size.x/2+random_f32_between(0, 10), ent->position.y+ent->size.y/2), v2(400, 0), 15*input->dt, lazer_ball_stripped);
}

#include "enemy_pengus.cpp"
#include "misc_enemies.cpp"
#include "slime.cpp"
#include "coyote.cpp"

void draw_fire(Entity *interactible, Game_Input *input) {
    interactible->state_time+=input->dt;

    if (i32(interactible->state_time*60)%50 < 10) {
        DrawImage(interactible->image[0], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
    } else if (i32(interactible->state_time*60)%50 < 20) {
        DrawImage(interactible->image[1], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
    } else if (i32(interactible->state_time*60)%50 < 30) {
        DrawImage(interactible->image[2], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
    } else if (i32(interactible->state_time*60)%50 < 40) {
        DrawImage(interactible->image[3], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
    } else {
        DrawImage(interactible->image[4], v2(interactible->position.x-camera_pos.x+out->width*.5, interactible->position.y));
    }

    Particle_Parameters min = {};
    Particle_Parameters max = {};

    min.position.x = interactible->position.x+12;
    max.position.x = interactible->position.x + 36;

    min.position.y = interactible->position.y;
    max.position.y = interactible->position.y + 18;

    min.velocity.x = 0;
    max.velocity.x = 0;

    min.velocity.y = -1200*input->dt;
    max.velocity.y = -1700*input->dt;

    min.life_time = 100.0*input->dt;
    max.life_time = 200.0*input->dt;

    static Image smoke = LoadImage(S("smoke.png"));


    if (random_f32_between(0, 1) < .1) {
        particle_emit(min, max, smoke);
    }
}

void draw_house(Entity *house) {

    switch (house->type)
    {
    case 0:
        {
            if (house->state_time*60 < 60) {
                DrawImage(house->image[0], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
            } else if (house->state_time*60 < 120) {
                DrawImage(house->image[1], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
            } else {
                DrawImage(house->image[2], v2(house->position.x - camera_pos.x+out->width*.5, house->position.y));
            }
            
            if (house->state_time*60 > 180) {
                    house->state_time = 0;
            }
        } break;
    case 1:
        {
            DrawImage(house->image[0], v2(house->position.x+camera_offset, house->position.y));
        } break;
    case 2:
        {
            if (house->sprite_index == 1 && house->state_time*60 < 20) {
                DrawImage(house->image[1], v2(house->position.x+camera_offset, house->position.y));
            } else if (house->sprite_index == 1 && house->state_time*60 > 20) {
                DrawImage(house->image[1], v2(house->position.x+camera_offset, house->position.y));
                house->sprite_index = 0;
                house->state_time = 0;
            } else if (house->state_time*60 > 120) {
                if (random_f32_between(0, 1) < .01) {
                    DrawImage(house->image[1], v2(house->position.x+camera_offset, house->position.y));
                    house->state_time = 0;
                    house->sprite_index = 1;
                } else {
                    DrawImage(house->image[0], v2(house->position.x+camera_offset, house->position.y));
                }

                
            } else {
                DrawImage(house->image[0], v2(house->position.x+camera_offset, house->position.y));
            }
        } break;
    }
}

void npc_action(Entity *npc, Entity *player) {
    npc->animation_time+=input->dt;
    npc->state_time+=input->dt;
    npc->dialogue_time+=input->dt;

    switch (npc->type) {
    case fairy:
        {
            if (player->fairy_uses == 0) {
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
            }
        } break;
    case lil_pengu:
        {
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
        } break;
    case mayor_snoresly:
        {
            if (group_time*60 < 45) {
                DrawImage(npc->image[0], v2(npc->position.x+camera_offset, npc->position.y));
            } else {
                DrawImage(npc->image[1], v2(npc->position.x+camera_offset, npc->position.y));

                String heave = S("HEAVE!!!");
                
                String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
                Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

                /*Rectangle2 text_rec = r2_bounds(v2(npc->position.x-2+camera_offset, npc->position.y-28), v2(186, 28), v2_zero, v2_one);

                DrawRect(text_rec, v4_black);*/

                DrawTextExt(font_hellomyoldfriend, heave, v2(npc->position.x+camera_offset-48, npc->position.y-26), v4_white, v2_zero, 2.0);
            }

            if (group_time*60 >= 90) {
                group_time = 0;
            }
        } break;
    case pengu_pullers:
        {
            draw_last.data[last_count] = *npc;

            if (group_time*60 < 45) {
                draw_last.data[last_count].sprite_index = 0;
            } else if (group_time*60 < 90) {
                draw_last.data[last_count].sprite_index = 1;
            } else {
                draw_last.data[last_count].sprite_index = 0;
            }

            last_count++;
        } break;
    }
}