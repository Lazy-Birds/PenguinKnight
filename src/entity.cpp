bool boss_walls_active = false;

u32 image_get_pixel(Image image, int x, int y) {
    return image.pixels[y*image.size.width+x];
}

Entity load_npc(Vector2 pos, i32 type) {
    switch (type)
    {
    case et_plant:
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
            enemy_plant.enemy.offset =v2(0,0);
            enemy_plant.type = et_plant;

            return enemy_plant;
        } break;
    case et_seal:
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
            enemy_seal.enemy.offset = v2(0, 0);
            enemy_seal.enemy.enemy_state = 0;
            enemy_seal.enemy.enemy_time = 0;
            enemy_seal.enemy.exp_dropped = 50;
            enemy_seal.enemy.damage = 10;
            enemy_seal.has_hit = true;
            enemy_seal.type = et_seal;

            return enemy_seal;
        } break;
    case et_penguin_king:
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
            big_papa.enemy.offset = v2(0, 0);
            big_papa.enemy.guard = 60;
            big_papa.enemy.damage = 30;
            big_papa.portrait = portrait;
            big_papa.enemy.exp_dropped = 2000;
            big_papa.has_hit = true;
            big_papa.state = NEUTRAL;
            big_papa.state_prev = NEUTRAL;
            big_papa.talking = false;
            big_papa.type = et_penguin_king;
            big_papa.name = S("Penguin Prince");

            big_papa.dialogue = load_dialogue(&big_papa);

            return big_papa;
        } break;
    case et_penguin_soldier:
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
            penguin_soldier.hitbox = r2_bounds(pos, v2(42, 48), v2_zero, v2_one);
            penguin_soldier.invuln = false;
            penguin_soldier.attackable = true;
            penguin_soldier.alive = true;
            penguin_soldier.facing = -1;
            penguin_soldier.image = image;
            penguin_soldier.type = et_penguin_soldier;
            penguin_soldier.enemy.anchor_pos = pos;
            penguin_soldier.enemy.guard = 20;
            penguin_soldier.enemy.weapon_size = v2(56, 8);
            penguin_soldier.enemy.weapon_offset = v2(-27, 33);
            penguin_soldier.enemy.damage = 30;
            penguin_soldier.enemy.id = World[player.player_level].entities.count;
            penguin_soldier.state = NEUTRAL;
            penguin_soldier.state_prev = NEUTRAL;
            penguin_soldier.enemy.exp_dropped = 75;
            penguin_soldier.has_hit = true;

            return penguin_soldier;
        } break;
    case et_robo_pup:
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
            pup.current_health = pup.max_health;
            pup.min_health = 0;
            pup.enemy.offset = v2(0, 0);
            pup.position = pos;
            pup.check_point = pos;
            pup.size = v2(39, 96);
            pup.hitbox = r2_bounds(pos, v2(39, 96), v2_zero, v2_one);
            pup.invuln = false;
            pup.attackable = true;
            pup.alive = true;
            pup.facing = 1;
            pup.image = image;
            pup.type = et_robo_pup;
            pup.enemy.anchor_pos = pos;
            pup.enemy.damage = 40;
            pup.enemy.id = World[player.player_level].entities.count;
            pup.state = NEUTRAL;
            pup.enemy.exp_dropped = 100;
            pup.has_hit = true;
            pup.movement_chks = chk_ldg;

            return pup;
        } break;
    case et_slime:
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
            slim.type = et_slime;
            slim.enemy.anchor_pos = pos;
            slim.enemy.damage =  30;
            slim.enemy.id = World[player.player_level].entities.count;
            slim.state = NEUTRAL;
            slim.enemy.exp_dropped = 100;
            slim.has_hit = true;
            slim.alive = true;
            slim.projectile = dart;
            slim.velocity_prev = v2_zero;

            return slim;
        } break;
    case et_eye_monster:
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
            stalker.type = et_eye_monster;
            stalker.enemy.anchor_pos = v2(stalker.position.x + 21, stalker.position.y + 18);
            stalker.enemy.damage = 80;
            stalker.enemy.id = World[player.player_level].entities.count;
            stalker.state = NEUTRAL;
            stalker.enemy.exp_dropped = 150;
            stalker.has_hit = true;
            stalker.alive = true;
            stalker.projectile = lazer;
            stalker.velocity_prev = v2_zero;

            return stalker;

        } break;
    case et_ooze:
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
            ooze.type = et_ooze;
            ooze.enemy.anchor_pos = pos;
            ooze.enemy.id = World[player.player_level].entities.count;
            ooze.enemy.damage = 80;
            ooze.state = NEUTRAL;
            ooze.enemy.exp_dropped = 3000;
            ooze.has_hit = false;
            ooze.alive = true;
            ooze.assymetric = true;
            ooze.offset = v2(144, 0);
            ooze.name = S("The Ooze");

            return ooze;
        } break;
    case et_coyote_nick:
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
            nick.enemy.offset = v2(18, 0);
            nick.position = pos;
            nick.check_point = pos;
            nick.size = v2(72, 81);
            nick.hitbox = r2_bounds(pos, v2(54, 81), v2_zero, v2_one);
            nick.invuln = false;
            nick.attackable = true;
            nick.facing = 1;
            nick.type = et_coyote_nick;
            nick.enemy.anchor_pos = pos;
            nick.enemy.id = World[player.player_level].entities.count;
            nick.enemy.damage = 40;
            nick.state = NEUTRAL;
            nick.enemy.exp_dropped = 2000;
            nick.has_hit = false;
            nick.alive = true;
            nick.assymetric = false;

            return nick;
        } break;
    case et_marmoset:
        {
            Entity marm = {};

            static Image marmy[] = {
                LoadImage(S("marmoset1.png")),
                LoadImage(S("marmoset2.png")),
                LoadImage(S("marmoset3.png")),
            };

            static Image bullet[] = {
                LoadImage(S("coyote_bullet.png")),
            };

            marm.image = marmy;
            marm.max_health = 150;
            marm.current_health = marm.max_health;
            marm.enemy.offset = v2_zero;
            marm.position = pos;
            marm.check_point = pos;
            marm.size = v2(30, 90);
            marm.hitbox = r2_bounds(pos, v2(30, 81), v2_zero, v2_one);
            marm.invuln = false;
            marm.attackable = true;
            marm.facing = 1;
            marm.type = et_marmoset;
            marm.enemy.anchor_pos = pos;
            marm.enemy.id = World[player.player_level].entities.count;
            marm.enemy.damage = 40;
            marm.state = NEUTRAL;
            marm.enemy.exp_dropped = 200;
            marm.has_hit = true;
            marm.alive = true;
            marm.assymetric = false;
            marm.projectile = bullet;

            return marm;

        } break;
    case et_hedgehog:
        {
            Entity hed = {};

            static Image heddy[] {
                LoadImage(S("hedgehog1.png")),
                LoadImage(S("hedgehog2.png")),
                LoadImage(S("hedgehog3.png")),
                LoadImage(S("hedgehog4.png")),
                LoadImage(S("hedgehog5.png")),
                LoadImage(S("hedgehog6.png")),
                LoadImage(S("hedgehog7.png")),
                LoadImage(S("hedgehog8.png")),
            };

            hed.image = heddy;
            hed.max_health = 250;
            hed.current_health = hed.max_health;
            hed.enemy.offset = v2_zero;
            hed.position = v2(pos.x, pos.y-3);
            hed.check_point = hed.position;
            hed.velocity = v2_zero;
            hed.size = v2(54, 99);
            hed.hitbox = r2_bounds(v2(hed.position.x+12, hed.position.y), hed.size, v2_zero, v2_one);
            hed.invuln = false;
            hed.attackable = true;
            hed.facing = 1;
            hed.type = et_hedgehog;
            hed.enemy.anchor_pos = pos;
            hed.enemy.id = World[player.player_level].entities.count;
            hed.enemy.damage = 20;
            hed.state = NEUTRAL;
            hed.enemy.exp_dropped = 200;
            hed.has_hit = true;
            hed.alive = true;
            hed.assymetric = false;

            return hed;

        } break;
    case et_fairy:
        {
            static Image image[2] = {LoadImage(S("fairy_orb1.png")), LoadImage(S("fairy_orb2.png"))};
            static Image portrait[2] = {LoadImage(S("fairy_orb_portrait1.png")), LoadImage(S("fairy_orb_portrait2.png"))};

            Entity fairy = {};

            fairy.position = pos;
            fairy.image = image;
            fairy.alive = true;
            fairy.state_time = 0;
            fairy.animation_time = 0;
            fairy.dialogue_time = 0;
            fairy.portrait = portrait;
            fairy.type = et_fairy;
            fairy.talking = false;
            fairy.dialogue = load_dialogue(&fairy);

            return fairy;

        } break;
    case et_lil_pengu:
        {
            static Image image[3] = {
                LoadImage(S("lil_pengu1.png")),
                LoadImage(S("lil_pengu2.png")),
                LoadImage(S("lil_pengu3.png")),
            };

            Entity lil_pengu = {};

            lil_pengu.position = pos;
            lil_pengu.image = image;
            lil_pengu.alive = true;
            lil_pengu.state_time = 0;
            lil_pengu.animation_time = 0;
            lil_pengu.dialogue_time = 0;
            lil_pengu.type = et_lil_pengu;

            return lil_pengu;
        } break;
    case et_mayor_snoresly:
        {
            static Image image[] = {
                LoadImage(S("mayor_snoresly1.png")),
                LoadImage(S("mayor_snoresly2.png")),
            };

            Entity snores = {};

            snores.position = pos;
            snores.image = image;
            snores.alive = true;
            snores.state_time = 0;
            snores.animation_time = 0;
            snores.dialogue_time = 0;
            snores.type = et_mayor_snoresly;

            return snores;
        } break;
    case et_pengu_puller:
        {
            static Image image[] = {
                LoadImage(S("pengu_puller1.png")),
                LoadImage(S("pengu_puller2.png")),
            };

            Entity puller = {};

            puller.position = pos;
            puller.image = image;
            puller.alive = true;
            puller.state_time = 0;
            puller.animation_time = 0;
            puller.dialogue_time = 0;
            puller.type = et_pengu_puller;

            return puller;
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
            idle_penguin.enemy.offset = v2(0,0);
            idle_penguin.type = et_default;

            return idle_penguin;
        } break;
    }    
}

Entity make_background(Vector2 pos, Image *img, Image *projectile, i32 sub_type) {
    Level *level = &World[player.player_level];

    Entity ent = {};

    ent.image = img;
    ent.projectile = projectile;
    ent.position = pos;
    ent.size = v2(48, 48);
    ent.type = et_background;
    ent.sub_type = sub_type;
    ent.id = level->entities.count;

    return ent;
}

Entity make_custom_background(Vector2 pos, Vector2 size, Image *img, Image *projectile, i32 sub_type) {
    Level *level = &World[player.player_level];

    Entity ent = {};

    ent.image = img;
    ent.projectile = projectile;
    ent.position = pos;
    ent.size = size;
    ent.type = et_background;
    ent.sub_type = sub_type;
    ent.id = level->entities.count;

    return ent;
}

Entity make_wall(Vector2 pos, u32 pixel, Image *image) {
    Entity ent = {};
    
    ent.position = pos;
    ent.size = v2(48, 48);
    ent.wall_type.pixel = pixel;
    ent.image = image;
    ent.type = et_wall;

    return ent;
}

Entity make_custom_wall(Vector2 pos, u32 pixel, Image *image, Vector2 size) {
    Entity ent = {};

    ent.position = pos;
    ent.size = size;
    ent.wall_type.pixel = pixel;
    ent.image = image;
    ent.type = et_wall;

    return ent;
}

void make_world() {
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

    assert(&World[player.player_level]);

    Level *level = &World[player.player_level];

    level->initialized = true;

    for (int i = 0; i < level->world.size.x; i+=48) {
        for (int k = 0; k < level->world.size.y; k+=48) {
            u32 pixel = image_get_pixel(level->world, i, k);
            if (ARCH_LITTLE_ENDIAN) {
                pixel = endian_swap_u32(pixel);
            }
            
            if (image_get_pixel(level->world, i, k)!=0) {
                switch (pixel)
                {
                case -1405996289: 
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_plant);
                        level->entities.count++;
                    } break;
                case -1060381441: 
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_seal);
                        level->entities.count++;
                    } break;
                case -18390529:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_penguin_king);
                        level->entities.count++;
                    } break;
                case 1516865791:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_penguin_soldier);
                        level->entities.count++;
                    } break;
                case 1752933631:
                    {
                        //687ba4
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_robo_pup);
                        level->entities.count++;
                    } break;
                case 643580671:
                    {
                        //265c42
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_slime);
                        level->entities.count++;
                    } break;
                case -1253013249:
                    {
                        //b55088
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_eye_monster);
                        level->entities.count++;
                    } break;
                case 595415807:
                    {
                        //237d52
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_ooze);
                        level->entities.count++;
                    } break;
                case 1748528383:
                    {
                        //68386c
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_coyote_nick);
                        level->entities.count++;
                    } break;
                case -1228546817:
                    {
                        //b6c5dc
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_marmoset);
                        level->entities.count++;
                    } break;
                case -355161345:
                    {
                        //ead4aa
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_hedgehog);
                        level->entities.count++;
                    } break;
                case -1562516993:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_fairy);
                        level->entities.count++;
                    } break;
                case 1969959423:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_lil_pengu);
                        level->entities.count++;
                    } break;
                case 1803064575:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_mayor_snoresly);
                        level->entities.count++;
                    } break;
                case -327140609:
                    {
                        level->entities.data[level->entities.count] =  load_npc(v2(i, k), et_pengu_puller);
                        level->entities.count++;
                    } break; 
                case 1059599103:
                    {
                        static Image image[3] = {
                            LoadImage(S("peasant_hovel1.png")),
                            LoadImage(S("peasant_hovel2.png")),
                            LoadImage(S("peasant_hovel3.png")),
                        };

                        Image proj[1] = {};

                        level->entities.data[level->entities.count] = make_custom_background(v2(i, k), v2(192, 202), image, proj, st_peasant_hovel);
                        level->entities.count++;
                    } break;
                case 659922687:
                    {
                        static Image image[] = {
                            LoadImage(S("clock_tower.png")),
                        };

                        Image proj[1] = {};

                        level->entities.data[level->entities.count] = make_custom_background(v2(i, k), v2(64, 240), image, proj, st_clock_tower);
                        level->entities.count++;

                    } break;
                case 403973631:
                    {
                        static Image image[] = {LoadImage(S("cave_wall.png"))};
                        static Image proj[1] = {};

                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_cv_wall);
                        level->entities.count++;
                    } break;
                case 2139260671:
                    {
                        static Image image[] = {LoadImage(S("cloud_brick_background.png"))};
                        static Image proj[1] = {};

                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_cld_brk_wall);
                        level->entities.count++;
                    } break;
                case 640369919:
                    {
                        //262b44
                        static Image image[] = {LoadImage(S("bolted_plates14.png"))};
                        static Image proj[1] = {};

                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_bolt_plates_14);
                        level->entities.count++;
                    } break;
                case 572998143:
                    {
                        //222741
                        static Image image[] = {LoadImage(S("bolted_plates15.png"))};
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_bolt_plates_15);
                        level->entities.count++;
                    } break;
                case 741561343:
                    {
                        //2c3353
                        static Image image[] = {LoadImage(S("bolted_plates16.png"))};
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_bolt_plates_16);
                        level->entities.count++;
                    } break;
                case 1049184511:
                    {
                        //2c3353
                        static Image image[] = {LoadImage(S("bolted_plates16.png"))};
                        static Image proj[1] = {};

                        Dump("The second one");
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_bolt_plates_16);
                        level->entities.count++;
                    } break;
                    case 977561343:
                    {
                        //3a4466
                        static Image image[] = {LoadImage(S("pipe_opening.png"))};
                        static Image proj[1] = {LoadImage(S("sewer_drip.png"))};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_pipe_opening);
                        level->entities.count++;
                    } break;
                case 2102019327:
                    {
                        //7d4a44
                        static Image image[] = {
                            LoadImage(S("work_rope1.png")),
                            LoadImage(S("work_rope2.png")),
                        };
                        
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_work_rope);
                        level->entities.count++;
                    } break;
                case -1229075457:
                    {
                        //b6bdcb
                        static Image image[] = {
                            LoadImage(S("spikes.png")),
                    };
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_spikes);
                        level->entities.count++;
                    } break;
                case 2033857535:
                    {
                        //
                        static Image image[] = {
                            LoadImage(S("badger_tent.png")),
                        };
                        
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_badger_tent);
                        level->entities.count++;
                    } break;
                case 439775487:
                    {
                        //1a3670
                        static Image image[] = {
                            LoadImage(S("factory_walls14.png")),
                        };
                        
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_factory_walls_14);
                        level->entities.count++;
                    } break;
                case 389047295:
                    {
                        //173063
                    static Image image[] = {
                        LoadImage(S("factory_walls15.png")),
                        };
                        
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_factory_walls_15);
                        level->entities.count++;
                    } break;
                case 542069503:
                    {
                        //173063
                        static Image image[] = {
                            LoadImage(S("large_pointed_fir.png")),
                        };
                        
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_pointed_fir);
                        level->entities.count++;
                    } break;
                case 455097855:
                    {
                    
                    //173063
                    static Image image[] = {
                        LoadImage(S("factory_walls16.png")),
                        };
                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_factory_walls_16);
                        level->entities.count++;
                    } break;
                case -372160769:
                    {
                        static Image image[] = {
                            LoadImage(S("street_lamp1.png")),
                            LoadImage(S("street_lamp2.png")),
                        };

                        //e9d146

                        static Image proj[1] = {};
                        
                        level->entities.data[level->entities.count] = make_background(v2(i, k), image, proj, st_street_lamps);
                        level->entities.count++;
                    } break;
                case 1050692607:
                    {
                        //3ea04b
                        level->entities.data[level->entities.count] = make_liquid(v2(i, k), 0);
                        level->entities.count++;
                    } break;
                case 423378687:
                    {
                        //193c3e
                        level->entities.data[level->entities.count] = make_liquid(v2(i, k), 1);
                        level->entities.count++;
                    } break;
                case -16759553:
                    {
                        //ff0044
                        level->entities.data[level->entities.count] =  make_interactible(v2(i, k), et_lever, st_default);
                        level->entities.count++;
                    } break;
                case -1958396417:
                    {
                        //8b453d
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_gate, st_gate_bot);
                        level->entities.count++;
                    } break;
                case 1950037503:
                    {
                        //743b35
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_gate, st_gate_top);
                        level->entities.count++;
                    } break;
                case 776045311:
                    {
                        //2e4182
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_door, st_default);
                        level->entities.count++;
                    } break;
                case -465877761:
                    {
                        //e43b44
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_fire, st_village);
                        level->entities.count++;
                    } break;
                case -1017357057:
                    {
                        //c35c5c
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_fire, st_sewer);
                        level->entities.count++;
                    } break;
                case -332843777:
                    {
                        //e43b44
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_fire, st_no_bg);
                        level->entities.count++;
                    } break;
                case 1837220863:
                    {
                        //6d81c3
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_hook, st_right);
                        level->entities.count++;
                    } break;
                case 1432922111:
                    {
                        //
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_hook, st_right);
                        level->entities.count++;
                    } break;
                case -982528769:
                    {
                        //
                        level->entities.data[level->entities.count] = make_interactible(v2(i, k), et_ladder, st_default);
                        level->entities.count++;
                    } break;
                case 842807807:
                    {
                        static Image image[] = {LoadImage(S("grass_bot.png"))};

                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1713024769:
                    {
                        static Image image[] = {LoadImage(S("grass_top.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1437261311: 
                    {
                        static Image image[] = {LoadImage(S("dirt_covered_snow.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1715024383:
                    {
                        static Image image[] = {LoadImage(S("dirt.png"))};
                       level->entities.data[level->entities.count] =  make_wall(v2(i, k), pixel, image);
                       level->entities.count++;
                    } break;
                case -521737985:
                    {
                        static Image image[] = {LoadImage(S("ice_brick.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 7903743:
                    {
                        static Image image[] = {LoadImage(S("rocks.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 629118975:
                    {
                        static Image image[] = {LoadImage(S("stone_brick.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 530563839:
                    {
                        static Image image[] = {LoadImage(S("rocks_crack.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -2003194113:
                    {
                        static Image image[] = {LoadImage(S("cloud_brick.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -513925889:
                    {
                        static Image image[] = {LoadImage(S("pipe6.png"))};
                        //e15e1c
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -663337729:
                    {
                        static Image image[] = {LoadImage(S("pipe1.png"))};
                        //d87644
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -595899905:
                    {
                        static Image image[] = {LoadImage(S("pipe5.png"))};
                        //dc7b49
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -227990785:
                    {
                        static Image image[] = {LoadImage(S("pipe4.png"))};
                        //f26922
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -814463489:
                    {
                        static Image image[] = {LoadImage(S("pipe2.png"))};
                        //f26922 
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1102434305:
                    {
                        static Image image[] = {LoadImage(S("pipe7.png"))};
                        //f26922
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1824115457:
                    {
                        static Image image[] = {LoadImage(S("pipe8.png"))};
                        //f26922
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -661491969:
                    {
                        static Image image[] = {LoadImage(S("pipe9.png"))};
                        //d8926e
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1937938943:
                    {
                        static Image image[] = {LoadImage(S("factory_walls1.png"))};
                        //738299
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1028485119:
                    {
                        static Image image[] = {LoadImage(S("factory_walls2.png"))};
                        //3d4d6f
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1519048191:
                    {
                        static Image image[] = {LoadImage(S("factory_walls3.png"))};
                        //5a8ad5
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 811442943:
                    {
                        static Image image[] = {LoadImage(S("factory_walls4.png"))};
                        //305da2
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 658863359:
                    {
                        static Image image[] = {LoadImage(S("factory_walls5.png"))};
                        //274574
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1315674623:
                    {
                        static Image image[] = {LoadImage(S("factory_walls6.png"))};
                        //4e6b99
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1834428929:
                    {
                        static Image image[] = {LoadImage(S("factory_walls7.png"))};
                        //223c66d
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1951998721:
                    {
                        static Image image[] = {LoadImage(S("factory_walls8.png"))};
                        //8ba6dc
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -2103254529:
                    {
                        static Image image[] = {LoadImage(S("factory_walls9.png"))};
                        //82a2e1
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1047046655:
                    {
                        static Image image[] = {LoadImage(S("factory_walls10.png"))};
                        //3e68a9
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 744335871:
                    {
                        static Image image[] = {LoadImage(S("factory_walls11.png"))};
                        //2c5da9
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 593013247:
                    {
                        //2358a9
                        static Image image[] = {LoadImage(S("factory_walls12.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 408005119:
                    {
                        //1851a9
                        static Image image[] = {LoadImage(S("factory_walls13.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1128283137:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates1.png"))};
                        //bcbfc3
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1448432385:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates2.png"))};
                        //a9aaac
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1734697217:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates3.png"))};
                        //989a9e
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -2037871617:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates4.png"))};
                        //86888b
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 2088930303:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates5.png"))};
                        //7c828b
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1902730497:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates6.png"))};
                        //8e96a2
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1987408897:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates7.png"))};
                        //898a8b
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -2121954305:
                    {
                        static Image image[] ={LoadImage(S("bolted_plates8.png"))};
                        //81858b
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 2021624831:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates9.png"))};
                        //787f8b
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1718185983:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates10.png"))};
                        //66696f
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1532978943:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates11.png"))};
                        //5b5f66
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1448896255:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates12.png"))};
                        //565c66
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case 1381259519:
                    {
                        static Image image[] = {LoadImage(S("bolted_plates13.png"))};
                        //525458
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                case -1751011073:
                    {
                        static Image image[] = {LoadImage(S("broken_beam.png"))};
                        //525458
                        level->entities.data[level->entities.count] = make_custom_wall(v2(i, k), pixel, image, v2(96, 192));
                        level->entities.count++;
                    } break;
                case 979781887:
                    {
                        static Image image[] = {LoadImage(S("factory_grate.png"))};
                        //3a6648
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
                    } break;
                default:
                    {
                        static Image image[] = {LoadImage(S("penguin_idle.png"))};
                        level->entities.data[level->entities.count] = make_wall(v2(i, k), pixel, image);
                        level->entities.count++;
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

EntityArrayP make_entity_array_three(Arena *arena, i32 size) {
    EntityArrayP arr = {};
    arr.data = PushArrayZero(arena, Entity*, size);
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

Entity make_liquid(Vector2 pos, i32 sub_type) {
    Entity entity = {};

    switch (sub_type)
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
            entity.type = et_liquid;
            entity.sub_type = sub_type;
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
            entity.type = et_liquid;
            entity.sub_type = sub_type;
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
            entity.type = et_liquid;
            entity.sub_type = sub_type;
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

void liquid_do_liquid(Entity *liquid, Game_Input *input, Game_Output *out) {
    
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
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];
        if (level->entities.data[i].type == et_wall &&
            ent->position.x <= pos.x &&
            ent->position.x + 48 >= pos.x &&
            ent->position.y <= pos.y &&
            ent->position.y+ 48 >= pos.y) {
            
            return *ent;
        }
    }

    return {};
}

void set_entity_hit_box(Entity *ent) {
    switch (ent->type)
    {
    case et_default:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_plant:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_seal:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_penguin_king:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_penguin_soldier:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_robo_pup:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_slime:
        {
            ent->hitbox = get_entity_rect(ent);
        } break;
    case et_ooze:
        {
            ent->hitbox = r2_bounds(v2(ent->position.x+144, ent->position.y+48), v2(48, 96), v2_zero, v2_one);
        } break;
    case et_coyote_nick:
        {
            ent->hitbox = r2_bounds(ent->position, v2(54, 81), v2_zero, v2_one);
        } break;
    case et_marmoset:
        {
            ent->hitbox = r2_bounds(ent->position, v2(30, 81), v2_zero, v2_one);
        } break;
    }
}

void reset_enemies() {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *npc = &level->entities.data[i];

        switch (npc->type)
        {
        case et_default:
            {
                Dump("Reset Enemies Mistake: default", npc->id);
            } break;
        case et_wall:
            {
                
            } break;
        case et_player:
            {
                Dump("Reset Enemies Mistake: player", npc->id);
            } break;
        case et_plant:
        case et_seal:
        case et_penguin_king:
        case et_penguin_soldier:
        case et_robo_pup:
        case et_slime:
        case et_eye_monster:
        case et_ooze:
        case et_coyote_nick:
        case et_marmoset:
        case et_hedgehog:
            {
                npc->alive = true;
                npc->position = npc->check_point;
                npc->current_health = npc->max_health;
                npc->velocity = v2_zero;
                npc->state = NEUTRAL;
                npc->sprite_index = 0;
                npc->invuln = false;

                set_entity_hit_box(npc);
            } break;
        case et_fairy:
        case et_lil_pengu:
        case et_mayor_snoresly:
        case et_pengu_puller:
            {
                
            } break;
        case et_lever:
        case et_gate:
        case et_door:
        case et_fire:
        case et_hook:
        case et_ladder:
            {

            } break;
        }
    }
}

/*bool enemy_on_enemy(Entity *entity_one) {
    for (int i = 0; i < World[player.player_level].enemies.count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+1), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(World[player.player_level].enemies.data[i].position.x, World[player.player_level].enemies.data[i].position.y), World[player.player_level].enemies.data[i].size, v2_zero, v2_one))) {

            if (entity_one->enemy.id != World[player.player_level].enemies.data[i].enemy.id) {
                return true;
            }
        } 
    }

    return false;
}*/

bool entity_on_wall(Entity *ent) {
    for (int i = 0; i < World[player.player_level].entities.count; i++) {
        Entity *ent_2 = &World[player.player_level].entities.data[i];

        if (ent_2->type == et_wall &&
            r2_intersects(r2_bounds(v2(ent->position.x, ent->position.y+1), ent->size, v2_zero, v2_one),
            get_entity_rect(ent_2))) {
            return true;
        } 
    }

    return false;
}

bool entity_against_wall(Entity *ent) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++)
    {
        if (level->entities.data[i].type == et_wall &&
            r2_intersects(get_entity_rect(&level->entities.data[i]), r2_bounds(v2(ent->position.x+sign_f32(ent->facing), ent->position.y), ent->size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    return false;
}

bool wall_intersects(Entity *ent) {
    Level *level = &World[player.player_level];
    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent_2 = &World[player.player_level].entities.data[i];

        if ((ent_2->type == et_wall || ent_2->type == et_gate) &&
            r2_intersects(get_entity_rect(ent), get_entity_rect(ent_2))) {
            return true;
        }
    }

    return false;
}

bool wall_intersects_rec(Rectangle2 rec) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];
        if (ent->type == et_wall && r2_intersects(get_entity_rect(ent), rec)) {
            return true;
        }
    }

    return false;
}

bool wall_ahead(Entity *ent) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent_2 = &level->entities.data[i];
        if (ent->facing > 0)
        {
            if ((ent_2->type == et_wall || ent_2->type == et_gate) &&
                r2_intersects(r2_shift(get_entity_rect(ent), v2(1, -1)), get_entity_rect(ent_2)))
            {
                return true;
            }
        } else if ((ent_2->type == et_wall || ent_2->type == et_gate) &&
            r2_intersects(r2_shift(get_entity_rect(ent), v2(-1, -1)), get_entity_rect(ent_2)))
        {
            return true;
        }
    }

    return false;
}

bool point_in_wall(Vector2 point) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];
        if (point.x >= ent->position.x &&
            point.x <= ent->position.x + ent->size.x &&
            point.y >= ent->position.y &&
            point.y <= ent->position.y + ent->size.y)
        {
            return false;
        }
    }

    return true;
}

bool ledge_ahead(Entity *ent) {
    Level *level = &World[player.player_level];

    Vector2 chk_pos;

    if (ent->facing < 0) {
        chk_pos = v2(ent->position.x-1, ent->position.y+ent->size.y+1);
    } else {
        chk_pos = v2(ent->position.x+ent->size.x+1, ent->position.y+ent->size.y+1);
    }

    return point_in_wall(chk_pos);
}

bool entity_in_air(Entity *ent) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {

        Entity *ent_2 = &level->entities.data[i];
        
        if (ent_2->type == et_wall &&
            r2_intersects(r2_shift(get_entity_rect(ent), v2(0, 2)), get_entity_rect(ent_2))) {
            return false;
        }
    }
    return true;
}

bool entity_in_spike_trap(Entity *ent) {
    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {

        if (level->entities.data[i].sub_type == st_spikes && r2_intersects(ent->hitbox, get_entity_rect(&level->entities.data[i])))
        {
            return true;
        }
    }

    return false;
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
        for (int k = 0; k < level->entities.count; k++) {
            Entity *ent = &level->entities.data[k];
            if (r2_contains(get_entity_rect(ent), v2(one->position.x+i*step.x, one->position.y+i*step.y))) {
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
            if (min.magnet != NULL && min.magnet->type == et_player) {
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
            if (particles[i].magnet != NULL && particles[i].magnet->type == et_player) {
                particles[i].velocity.x+=300*dt*sign_f32(particles[i].magnet->anchor.x-particles[i].position.x);
                particles[i].velocity.y+=300*dt*sign_f32(particles[i].magnet->anchor.y-particles[i].position.y);

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

void emit_death_particles(Entity *ent, f32 dt) {
    Particle_Parameters min = {};
    Particle_Parameters max = {};

    min.velocity.x = 4000*dt;
    min.velocity.y = 4000*dt;

    max.velocity.x = 4000*dt;
    max.velocity.y = 4000*dt;

    min.position.x = ent->position.x;
    min.position.y = ent->position.y;

    max.position.x = ent->position.x + ent->size.x;
    max.position.y = ent->position.y + ent->size.y;

    min.life_time = 120*dt;
    max.life_time = 90*dt;

    min.magnet = &player;

    Image image = LoadImage(S("exp.png"));

    for (int i = 0; i < i32(ent->enemy.exp_dropped/10); i++) {
        particle_emit(min, max, image);
    }
}

void make_boss_walls(Vector2 *pos, i32 count) {
    static Image boss_walls[] = {LoadImage(S("wall_flowers_white.png"))};

    Level *level = &World[player.player_level];

    for (int i = 0; i < count; i++) {
        Entity ent = {};
        
        ent.position = pos[i];
        ent.size = v2(48, 48);
        ent.image = boss_walls;
        ent.type = et_wall;
        ent.sub_type = st_boss;

        level->entities.data[level->entities.count] = ent;

        level->entities.count++;
    }
}

void destroy_boss_walls() {
    i32 walls_destroyed = 0;

    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];

        if (ent->type == et_wall && ent->sub_type == st_boss) {
            level->entities.data[i] = {};
            walls_destroyed++;
        }
    }

    level->entities.count-=walls_destroyed;
}

void draw_boss_health_bar(Entity *boss) {
    DrawRect(r2_bounds(v2(out->width*.5-302, out->height-50), v2(604, 12), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(out->width*.5-300, out->height-48), v2((boss->current_health/boss->max_health)*600, 8), v2_zero, v2_one), v4_red);
}

void draw_normal_enemy_health(Entity *ent) {
    DrawRect(r2_bounds(v2(ent->hitbox.x0-camera_pos.x+out->width*.5-2, ent->hitbox.y0-14), v2(ent->size.x+4-ent->offset.x, 12),
        v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(ent->hitbox.x0-camera_pos.x+out->width*.5, ent->hitbox.y0-12),
        v2(ent->current_health/ent->max_health*(ent->size.x+4)-ent->offset.x, 8), v2_zero, v2_one), v4_red);
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
        if (wall_intersects(nme)) {
        nme->position.y-=sign_f32(dy);
        y_moved-=sign_f32(dy);
        nme->velocity.y=0;
        break;
    }
}


    for (int i = 0; i < abs_f32(dx); i++) {
        nme->position.x+=sign_f32(dx);
        x_moved += sign_f32(dx);
        
        if (nme->movement_chks == chk_ldg && !entity_in_air(nme) && ledge_ahead(nme))
        {
            nme->velocity.x=0;

            break;
        }

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
#include "robots.cpp"

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

void draw_ent(Entity *ent) {
    switch (ent->type)
    {
    case 0:
        {
            if (ent->state_time*60 < 60) {
                DrawImage(ent->image[0], v2(ent->position.x - camera_pos.x+out->width*.5, ent->position.y));
            } else if (ent->state_time*60 < 120) {
                DrawImage(ent->image[1], v2(ent->position.x - camera_pos.x+out->width*.5, ent->position.y));
            } else {
                DrawImage(ent->image[2], v2(ent->position.x - camera_pos.x+out->width*.5, ent->position.y));
            }
            
            if (ent->state_time*60 > 180) {
                    ent->state_time = 0;
            }
        } break;
    case 1:
        {
            DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));
        } break;
    case 2:
        {
            
        } break;
    }
}

void enemy_action(Entity *enemy, Game_Input *input) {
    switch (enemy->type)
    {
    case et_plant:
        {

        } break;
    case et_seal:
        {
            seal_action(enemy, input, &player);

            if (enemy->facing > 0)
            {
                DrawImageMirrored(enemy->image[0], v2(enemy->position.x-camera_pos.x+out->width*.5 + enemy->enemy.offset.x,
                   enemy->position.y+enemy->enemy.offset.y), true, false);
            } else 
            {
                DrawImage(enemy->image[0], v2(enemy->position.x-camera_pos.x+out->width*.5 + enemy->enemy.offset.x,
                   enemy->position.y+enemy->enemy.offset.y));
            }
        } break;
    case et_penguin_king:
        {
            if (!bosses_killed[0] && player.position.x > 8400 && player.position.x < 9504) {
                camera_state = CAMERALOCKED;
                camera_pos_target = v2(8880, out->height);

                if (!boss_walls_active && player.position.x > 8448)
                {
                    Vector2 boss_walls[] = {
                        v2(8400, 528),
                        v2(8400, 480),
                        v2(8400, 432),
                        v2(8400, 384),
                        v2(8400, 336),
                        v2(8400, 288),
                        v2(8400, 240),
                        v2(8400, 192),
                        v2(8400, 144),
                        v2(8400, 96),
                        v2(8400, 48),
                        v2(8400, 0),
                    };

                    make_boss_walls(boss_walls, 12);

                    boss_walls_active = true;
                }


                

                penguin_king_action(enemy, &player, input->dt, out);

            }
        } break;
    case et_penguin_soldier:
        {
            p_soldier_action(enemy, input->dt, &player, invuln_time, input);   
        } break;
    case et_robo_pup:
        {
            if (entity_get_distance_x(&player, enemy) < 900) {
                robo_pup_action(enemy, input);
            }
        } break;
    case et_slime:
        {
            if (entity_get_distance_x(&player, enemy) < 900) {
                slime_action(enemy, &player, input);
            }
        } break;
    case et_eye_monster:
        {
            if (entity_get_distance_x(enemy, &player) < 1000 && entity_get_distance_y(enemy, &player) < 300) {
                eye_monster_action(enemy, input);
            }
        } break;
    case et_ooze:
        {
            if (entity_get_distance_x(&player, enemy) < 700) {
                ooze_action(enemy, input);
            }
        } break;
    case et_coyote_nick:
        {
            coyote_action(enemy, input);
        } break;
    case et_marmoset:
        {
            if (entity_get_distance_x(&player, enemy) < 700) {
                marmoset_action(enemy);
            }
        } break;
    case et_hedgehog:
        {
            if (entity_get_distance_x(&player, enemy) < 1200) {
                hedgehog_actions(enemy);
            }
        } break;
    }
    
}

void npc_action(Entity *npc, Entity *player) {
    npc->animation_time+=input->dt;
    npc->state_time+=input->dt;
    npc->dialogue_time+=input->dt;

    switch (npc->type) {
    case et_fairy:
        {
            if (player->fairy_uses == 0) {
                if (i32(npc->animation_time*60)%60 < 30) {
                    DrawImage(npc->image[0], v2(npc->position.x-camera_pos.x+out->width*.5, npc->position.y));
                } else {
                    DrawImage(npc->image[1], v2(npc->position.x-camera_pos.x+out->width*.5, npc->position.y));
                }

                if (abs_i32(npc->position.x-player->position.x) < 200 && abs_i32(npc->position.y-player->position.y) < 300) {
                    if (!npc->talking) {
                        npc->talking = draw_dialogue_box(npc->dialogue.data[0], out, npc->portrait, 2);
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
    case et_lil_pengu:
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
    case et_mayor_snoresly:
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
    case et_pengu_puller:
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

void interactible_actions(Entity *ent, Game_Input *input) {
    if (entity_get_distance_x(ent, &player) < 1000)
    { 
        switch (ent->type)
        {
        case et_lever:
        case et_gate:
        case et_door:
        case et_fire:
            {
                if (ent->type == et_fire)
                {
                    draw_fire(ent, input);
                } else
                {
                    DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));

                    if (ent->type == et_gate) {
                        DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));

                        Entity ent_2 = get_wall_at(ent->position);

                        DrawImage(ent_2.image[0], v2(ent_2.position.x + camera_offset, ent_2.position.y));
                    }
                }
                

                if (r2_intersects(get_entity_rect(&player), get_entity_rect(ent)) && !ent->acting 
                    && ent->actable)
                {
                    Rectangle2 text_box = r2_bounds(v2(ent->position.x-28-camera_pos.x+out->width*.5, ent->position.y-28),
                        v2(104, 20), v2_zero, v2_one);

                    DrawRect(text_box, v4_black);
                    DrawTextExt(font_hellomyoldfriend, S("Press U."), v2(ent->position.x-24-camera_pos.x+out->width*.5,
                        ent->position.y-24), v4_white, v2_zero, 1.0);

                    if ((player.state == NEUTRAL || player.state == MOVE) && player.acting)
                    {
                        ent->acting = true;
                    }
                }

                if (ent->acting)
                {
                    interact(ent, input, camera_pos);
                }
            } break;
        case et_hook:
            {
                DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));

                if (ent->type == et_hook && player.acting && player.acting_cd <= 0 && clear_line_of_sight(&player, ent)) {
                    if (abs_f32(player.anchor.x - ent->anchor.x) < 10 && abs_f32(player.anchor.y - ent->anchor.y) < 10) {
                        player.acting_cd = 60*input->dt;
                        player.state = NEUTRAL;
                    } else {
                        player.state = HOOKSHOTTING;

                        f32 velocity = 6000*input->dt;

                        f32 radians = angle_between(player.anchor, ent->anchor);
                        Vector2 step = v2_arm(radians);

                        player.velocity.x += velocity*step.x;
                        player.velocity.y += velocity*step.y;

                        player.velocity.x = clamp_f32(player.velocity.x, -25000*input->dt, 25000*input->dt);
                        player.velocity.y = clamp_f32(player.velocity.y, -25000*input->dt, 25000*input->dt);
                        //draw_hook_shot(&level->interactible.data[i]);
                    }
                }

            } break;
        case et_ladder:
            {
                if (r2_intersects(get_entity_rect(&player), get_entity_rect(ent))
                    && player.can_climb) {

                    interact(ent, input, camera_pos);
                }

                DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));
            } break;
        }
    }
}

void background_action(Entity *ent, Game_Input *input) {
    Level *level = &World[player.player_level];

    if (entity_get_distance_x(ent, &player) < 1000)
    {
        switch (ent->sub_type)
        {
        case st_cv_wall:
        case st_cld_brk_wall:
        case st_bolt_plates_14:
        case st_bolt_plates_15:
        case st_bolt_plates_16:
        case st_badger_tent:
        case st_factory_walls_14:
        case st_factory_walls_15:
        case st_factory_walls_16:
        case st_pointed_fir:
        case st_clock_tower:
            {
                DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));
            } break;
        case st_pipe_opening:
            {
                ent->state_time+=input->dt;
                if (ent->state_time*60 > 60 && random_f32_between(0, 1) < .01) {
                    generate_drips(v2(ent->position.x+15, ent->position.y+36), ent->projectile[0], input);
                    ent->state_time = 0;
                }
            } break;
        case st_work_rope:
            {
                draw_last.data[last_count] = *ent;
                if (group_time*60 < 45) {
                    draw_last.data[last_count].sprite_index = 0;
                } else {
                    draw_last.data[last_count].sprite_index = 1;
                }
                
                last_count++;
            } break;
        case st_spikes:
            {
                if (r2_intersects(get_entity_rect(&player), get_entity_rect(ent))) {
                    player.current_health = 0;
                    player.alive = false;
                }

                DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));
            } break;
        case st_peasant_hovel:
            {
                if (entity_get_distance_x(ent, &player) < 900) {
                    ent->state_time+=input->dt;
                    
                    if (ent->sprite_index == 1 && ent->state_time*fps > 20) {
                        ent->sprite_index = 0;
                        ent->state_time = 0;
                    } else if (ent->sprite_index == 2 && ent->state_time*fps > 20) {
                        ent->sprite_index = 0;
                        ent->state_time = 0;
                    } else if (ent->state_time*fps > 120) {
                        if (random_f32_between(0, 1) < .01) {
                            ent->sprite_index = 1;
                            ent->state_time = 0;
                        } else if (random_f32_between(0, 1) < .01) {
                            ent->sprite_index = 2;
                            ent->state_time = 0;
                        }
                    }

                    DrawImage(ent->image[ent->sprite_index], v2(ent->position.x+camera_offset, ent->position.y));
                }
            } break;

        }
    }
}

void entity_action(Game_Input *input) {
    for (int i = 0; i < temp_arr.count; i++) {
        if (temp_arr.data[i]->alive) {
            switch (temp_arr.data[i]->type)
            {
            case et_default:
                {
                    Dump("Oopsies");
                } break;
            case et_player:
                {
                    Dump("Player stuff.");
                } break;
            case et_plant:
            case et_seal:
            case et_penguin_king:
            case et_penguin_soldier:
            case et_robo_pup:
            case et_slime:
            case et_eye_monster:
            case et_ooze:
            case et_coyote_nick:
            case et_marmoset:
            case et_hedgehog:
            {
                enemy_action(temp_arr.data[i], input);
            }
            case et_fairy:
            case et_lil_pengu:
            case et_mayor_snoresly:
            case et_pengu_puller:
                {
                    npc_action(temp_arr.data[i], &player);
                } break;
            case et_lever:
            case et_gate:
            case et_door:
            case et_fire:
            case et_hook:
            case et_ladder:
                {
                    interactible_actions(temp_arr.data[i], input);
                } break;
            }
        }
    }
}



void entity_processing(Game_Input *input, Game_Output *out) {
    temp_arr.count = 0;

    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];

        switch (ent->type)
        {
        case et_default:
            {

            } break;
        case et_wall:
            {
                DrawImage(ent->image[0], v2(ent->position.x+camera_offset, ent->position.y));
            } break;
        case et_background:
            {
                background_action(ent, input);
            } break;
        case et_liquid:
            {
                liquid_do_liquid(ent, input, out);
            } break;
        case et_player:
        case et_plant:
        case et_seal:
        case et_penguin_king:
        case et_penguin_soldier:
        case et_robo_pup:
        case et_slime:
        case et_eye_monster:
        case et_ooze:
        case et_coyote_nick:
        case et_marmoset:
        case et_hedgehog:
        case et_fairy:
        case et_lil_pengu:
        case et_mayor_snoresly:
        case et_pengu_puller:
        case et_lever:
        case et_gate:
        case et_door:
        case et_fire:
        case et_hook:
        case et_ladder:
            {
                temp_arr.data[temp_arr.count] = &level->entities.data[i];
                temp_arr.count++;
            } break;
        }
    }

    entity_action(input);
}