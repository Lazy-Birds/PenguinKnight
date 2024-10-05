Entity wall[1000] = {};
int wall_count = 0;

u32 image_get_pixel(Image image, int x, int y) {
    return image.pixels[y*image.size.width+x];
}

Image set_texture(Entity entity) {
    if (entity.wall_type.pixel == 842807807) {
        Image image = LoadImage(S("grass_bot.png"));

        return image;

        dump(entity.wall_type.image.size);
    } else if (entity.wall_type.pixel == -1713024769) {
        Image image = LoadImage(S("grass_top.png"));

        return image;
    } else if (entity.wall_type.pixel == -1405996289) {
        Image image = LoadImage(S("plant_idle1.png"));

        return image;
    } else if (entity.wall_type.pixel == 1437261311) {
        Image image = LoadImage(S("dirt_covered_snow.png"));

        return image;
    } else if (entity.wall_type.pixel == -521737985) {
        Image image = LoadImage(S("ice_brick.png"));

        return image;
    } else if (entity.wall_type.pixel == 1715024383) {
        Image image = LoadImage(S("dirt.png"));

        return image;
    }
    else {
        Image image = LoadImage(S("penguin_idle.png"));
        dump(entity.wall_type.pixel);

        return image;
    }
}


void make_wall() {
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
                wall[wall_count].position = v2(i*48, k*48);
                wall[wall_count].size = v2(48, 48);
                wall[wall_count].wall_type.pixel = pixel;
                wall[wall_count].wall_type.image = set_texture(wall[wall_count]);

                wall_count++;
            }
        }
    }
}