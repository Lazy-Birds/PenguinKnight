Arena *animation_arena = NULL;

FrameArray make_frame_array(Arena *arena, i32 capacity) {
    FrameArray arr = {};
    arr.data = PushArrayZero(arena, Frame, capacity);
    arr.capacity = capacity;
    arr.count = 0;

    return arr;
}

FrameArray* load_weapon_frames(i32 weapon) {
    if (animation_arena == NULL) {
        animation_arena = arena_alloc(Gigabytes(1));
    }

    static FrameArray frames[16] = {};

    switch (weapon)
    {
    case pw_cleaver:
        {
            static Frame move_arr[2] =
            {
                { .image = LoadImage(S("cleaver1.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver2.png")), .frame_length = 8},
            };

            FrameArray move =
            {
                .data = move_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame hurt_arr[1] =
            {
                { .image = LoadImage(S("cleaver3.png")), .frame_length = 4},
            };

            FrameArray hurt =
            {
                .data = hurt_arr,
                .capacity = 1,
                .count = 1,
            };

            static Frame sprint_arr[4] =
            {
                { .image = LoadImage(S("cleaver4.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver5.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver6.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver7.png")), .frame_length = 8},
            };

            static FrameArray sprint =
            {
                .data = sprint_arr,
                .capacity = 10,
                .count = 4,
            };

            static Frame jump_arr[2] =
            {

                { .image = LoadImage(S("cleaver8.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver9.png")), .frame_length = 45},
            };

            FrameArray jump =
            {
                .data = jump_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame guard_arr[3] =
            {
                { .image = LoadImage(S("cleaver10.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver11.png")), .frame_length = 30},
                { .image = LoadImage(S("cleaver12.png")), .frame_length = 8},
            };

            FrameArray guard =
            {
                .data = guard_arr,
                .capacity = 3,
                .count = 3,
            };

            static Frame dodge_arr[6] =
            {
                { .image = LoadImage(S("cleaver13.png")), .frame_length = 4},
                { .image = LoadImage(S("cleaver14.png")), .frame_length = 4},
                { .image = LoadImage(S("cleaver14.png")), .frame_length = 4},
                { .image = LoadImage(S("cleaver16.png")), .frame_length = 4},
                { .image = LoadImage(S("cleaver17.png")), .frame_length = 4},
                { .image = LoadImage(S("cleaver18.png")), .frame_length = 4},
            };

            FrameArray dodge =
            {
                .data = dodge_arr,
                .capacity = 6,
                .count = 6,
            };

            static Frame attack_one_arr[6] =
            {
                { .image = LoadImage(S("cleaver19.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver20.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver21.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver22.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver23.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver24.png")), .frame_length = 16},
            };

            FrameArray attack_one =
            {
                .data = attack_one_arr,
                .capacity = 6,
                .count = 6,
            };

            static Frame attack_two_arr[5] =
            {
                { .image = LoadImage(S("cleaver25.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver26.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver27.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver28.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver29.png")), .frame_length = 16},
            };

            FrameArray attack_two =
            {
                .data = attack_two_arr,
                .capacity = 5,
                .count = 5,
            };

            static Frame attack_three_arr[5] =
            {
                { .image = LoadImage(S("cleaver30.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver31.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver32.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver33.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver34.png")), .frame_length = 16},
            };

            FrameArray attack_three =
            {
                .data = attack_three_arr,
                .capacity = 5,
                .count = 5,
            };

            static Frame charge_attack_arr[5] =
            {
                { .image = LoadImage(S("cleaver35.png")), .frame_length = 45},
                { .image = LoadImage(S("cleaver36.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver37.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver38.png")), .frame_length = 8},
                { .image = LoadImage(S("cleaver39.png")), .frame_length = 16},
            };

            FrameArray charge_attack =
            {
                .data = charge_attack_arr,
                .capacity = 5,
                .count = 5,
            };

            static Frame jump_attack_arr[2] =
            {
                { .image = LoadImage(S("cleaver40.png")), .frame_length = 90},
                { .image = LoadImage(S("cleaver41.png")), .frame_length = 16},
            };

            FrameArray jump_attack =
            {
                .data = jump_attack_arr,
                .capacity = 2,
                .count = 2,
            };

            frames[0] = move;
            frames[1] = hurt;
            frames[2] = sprint;
            frames[3] = jump;
            frames[4] = guard;
            frames[5] = dodge;
            frames[6] = attack_one;
            frames[7] = attack_two;
            frames[8] = attack_three;
            frames[9] = charge_attack;
            frames[10] = jump_attack;

            return frames;
        } break;
    case pw_staff:
        {
            static Frame move_arr[2] =
            {
                { .image = LoadImage(S("staff1.png")), .frame_length = 8},
                { .image = LoadImage(S("staff2.png")), .frame_length = 8},
            };

            FrameArray move =
            {
                .data = move_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame hurt_arr[1] =
            {
                { .image = LoadImage(S("staff3.png")), .frame_length = 4},
            };

            FrameArray hurt =
            {
                .data = hurt_arr,
                .capacity = 1,
                .count = 1,
            };

            static Frame sprint_arr[4] =
            {
                { .image = LoadImage(S("staff4.png")), .frame_length = 8},
                { .image = LoadImage(S("staff5.png")), .frame_length = 8},
                { .image = LoadImage(S("staff6.png")), .frame_length = 8},
                { .image = LoadImage(S("staff7.png")), .frame_length = 8},
            };

            FrameArray sprint =
            {
                .data = sprint_arr,
                .capacity = 10,
                .count = 4,
            };

            static Frame jump_arr[2] =
            {

                { .image = LoadImage(S("staff8.png")), .frame_length = 8},
                { .image = LoadImage(S("staff9.png")), .frame_length = 45},
            };

            FrameArray jump =
            {
                .data = jump_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame guard_arr[3] =
            {
                { .image = LoadImage(S("staff10.png")), .frame_length = 8},
                { .image = LoadImage(S("staff11.png")), .frame_length = 30},
                { .image = LoadImage(S("staff12.png")), .frame_length = 8},
            };

            FrameArray guard =
            {
                .data = guard_arr,
                .capacity = 3,
                .count = 3,
            };

            static Frame dodge_arr[6] =
            {
                { .image = LoadImage(S("staff13.png")), .frame_length = 4},
                { .image = LoadImage(S("staff14.png")), .frame_length = 4},
                { .image = LoadImage(S("staff14.png")), .frame_length = 4},
                { .image = LoadImage(S("staff16.png")), .frame_length = 4},
                { .image = LoadImage(S("staff17.png")), .frame_length = 4},
                { .image = LoadImage(S("staff18.png")), .frame_length = 4},
            };

            FrameArray dodge =
            {
                .data = dodge_arr,
                .capacity = 6,
                .count = 6,
            };

            static Frame attack_one_arr[2] =
            {
                { .image = LoadImage(S("staff19.png")), .frame_length = 8},
                { .image = LoadImage(S("staff20.png")), .frame_length = 16},
            };

            FrameArray attack_one =
            {
                .data = attack_one_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame charge_attack_arr[2] =
            {
                { .image = LoadImage(S("staff21.png")), .frame_length = 45},
                { .image = LoadImage(S("staff22.png")), .frame_length = 16},
            };

            FrameArray charge_attack = 
            {
                .data = charge_attack_arr,
                .capacity = 2,
                .count = 2,
            };

            frames[0] = move;
            frames[1] = hurt;
            frames[2] = sprint;
            frames[3] = jump;
            frames[4] = guard;
            frames[5] = dodge;
            frames[6] = attack_one;
            frames[7] = charge_attack;

            return frames;
        } break;
    case pw_katana:
        {
            static Frame move_arr[2] =
            {
                { .image = LoadImage(S("katana1.png")), .frame_length = 8},
                { .image = LoadImage(S("katana2.png")), .frame_length = 8},
            };

            FrameArray move =
            {
                .data = move_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame hurt_arr[1] =
            {
                { .image = LoadImage(S("katana3.png")), .frame_length = 4},
            };

            FrameArray hurt =
            {
                .data = hurt_arr,
                .capacity = 1,
                .count = 1,
            };

            static Frame sprint_arr[4] =
            {
                { .image = LoadImage(S("katana4.png")), .frame_length = 8},
                { .image = LoadImage(S("katana5.png")), .frame_length = 8},
                { .image = LoadImage(S("katana6.png")), .frame_length = 8},
                { .image = LoadImage(S("katana7.png")), .frame_length = 8},
            };

            FrameArray sprint =
            {
                .data = sprint_arr,
                .capacity = 10,
                .count = 4,
            };

            static Frame jump_arr[2] =
            {

                { .image = LoadImage(S("katana8.png")), .frame_length = 8},
                { .image = LoadImage(S("katana9.png")), .frame_length = 45},
            };

            FrameArray jump =
            {
                .data = jump_arr,
                .capacity = 2,
                .count = 2,
            };

            static Frame guard_arr[3] =
            {
                { .image = LoadImage(S("katana10.png")), .frame_length = 8},
                { .image = LoadImage(S("katana11.png")), .frame_length = 30},
                { .image = LoadImage(S("katana12.png")), .frame_length = 8},
            };

            FrameArray guard =
            {
                .data = guard_arr,
                .capacity = 3,
                .count = 3,
            };

            static Frame dodge_arr[6] =
            {
                { .image = LoadImage(S("katana13.png")), .frame_length = 4},
                { .image = LoadImage(S("katana14.png")), .frame_length = 4},
                { .image = LoadImage(S("katana14.png")), .frame_length = 4},
                { .image = LoadImage(S("katana16.png")), .frame_length = 4},
                { .image = LoadImage(S("katana17.png")), .frame_length = 4},
                { .image = LoadImage(S("katana18.png")), .frame_length = 4},
            };

            FrameArray dodge =
            {
                .data = dodge_arr,
                .capacity = 6,
                .count = 6,
            };

            static Frame attack_one_arr[4] =
            {
                { .image = LoadImage(S("katana19.png")), .frame_length = 24},
                { .image = LoadImage(S("katana20.png")), .frame_length = 24},
                { .image = LoadImage(S("katana21.png")), .frame_length = 8},
                { .image = LoadImage(S("katana22.png")), .frame_length = 16},
            };

            FrameArray attack_one =
            {
                .data = attack_one_arr,
                .capacity = 4,
                .count = 4,
            };

            static Frame jump_attack_arr[2] =
            {
                { .image = LoadImage(S("katana23.png")), .frame_length = 90},
                { .image = LoadImage(S("katana24.png")), .frame_length = 8},
            };

            FrameArray jump_attack =
            {
                .data = jump_attack_arr,
                .capacity = 2,
                .count = 2,
            };

            frames[0] = move;
            frames[1] = hurt;
            frames[2] = sprint;
            frames[3] = jump;
            frames[4] = guard;
            frames[5] = dodge;
            frames[6] = attack_one;
            frames[7] = jump_attack;

            return frames;
        } break;
    }

    Dump("Returned No frames");
    return {};
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

void draw_player(i32 action) {
    Weapon weapon = player.weapon;
    Vector2 position = player.position;
    i32 facing = player.facing;

    if (sign_i32(facing) < 0) {
        DrawImageMirrored(weapon.frames[action].data[player.animation.frame].image,
            v2(position.x + weapon.offset_left.x+camera_offset, position.y+weapon.offset_left.y), true, false);
        //DrawImage(weapon.image[i32(frame + weapon.weapon_frames.y+1)], v2(position.x + weapon.offset_right.x, position.y+weapon.offset_right.y));
    } else {
        DrawImage(weapon.frames[action].data[player.animation.frame].image, 
            v2(position.x + weapon.offset_right.x+camera_offset, position.y+weapon.offset_right.y));
    }

    player.animation.index++;

    if (player.animation.index == player.weapon.frames->data[player.animation.frame].frame_length)
    {
        player.animation.frame++;
        player.animation.index = 0;
    }
}