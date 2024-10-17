Entity wall[1000] = {};
int wall_count = 0;
Entity enemys[1000] = {};
int enemy_count = 0;

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

            return enemy_seal;
        }
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

