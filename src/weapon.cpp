WeaponArray w_list = {NULL};

//Enum for General frames, giv
enum general_frames {
    gf_move,
    gf_hurt,
    gf_sprint,
    gf_jump,
    gf_guard,
    gf_dodge,
};

//Enums for the other weapons, weapons using gen frames start at 6

//Cleaver Frames
enum fr_cleaver {
    cf_attack_one = 6,
    cf_attack_two,
    cf_attack_three,
    cf_charge_attack,
    cf_jump_attack,
};

//Staff
enum fr_staff {
    sf_attack = 6,
    sf_charged_spell,
};

//Katana
enum fr_katana {
    kf_attack = 6,
    kf_jump_attack,
};

WeaponArray make_weapon_array(Arena *arena, i32 capacity) {
    WeaponArray arr = {};

    arr.data = PushArrayZero(arena, Weapon, capacity);
    arr.capacity = capacity;
    arr.count = 0;

    return arr;
}

void load_weapon(i32 weapon_type) {
    switch (weapon_type)
    {
    case pw_cleaver:
        {
            Weapon *cleaver = &w_list.data[pw_cleaver];

            static Image icon = LoadImage(S("cleaver_icon.png"));
            static Image proj[] = {
                LoadImage(S("cleaver_charged1.png")),
                LoadImage(S("cleaver_charged2.png")),
                LoadImage(S("cleaver_charged3.png")),
                LoadImage(S("cleaver_charged4.png")),
                LoadImage(S("cleaver_charged5.png")),
            };

            cleaver->frames = load_weapon_frames(pw_cleaver);
            cleaver->size = v2(142, 88);
            cleaver->weapon_frames = v2(4, 9);
            cleaver->type = pw_cleaver;
            cleaver->offset_right = v2(-42, -36);
            cleaver->offset_left = v2(-60, -36);
            cleaver->hit_size = v2(76, 81);
            cleaver->hit_offset_right = v2(20, 30);
            cleaver->hit_offset_left = v2(60, 30);
            cleaver->charged_frames = v2(9, 14);
            cleaver->base_damage = 30;
            cleaver->damage_attribute = at_strength;
            cleaver->damage_multiplier = 1.5;
            cleaver->charge_time = 9;
            cleaver->jump_frame = v2(14, 15);
            cleaver->frame_hit = 2;
            cleaver->dash_frame = v2(16, 19);
            cleaver->icon = icon;
            cleaver->jump = v2(20,21);
            cleaver->guard = v2(22, 24);
            cleaver->item_pickup = v2(25, 25);
            cleaver->sprite_index = 0;
            cleaver->projectile = proj;
        } break;
    case pw_staff:
        {
            Image icon = LoadImage(S("staff_icon.png"));


            Weapon *staff = &w_list.data[pw_staff];

            staff->frames = load_weapon_frames(pw_staff);
            staff->icon = icon;
            staff->size = v2(106, 88);
            staff->weapon_frames = v2(4, 9);
            staff->type = pw_staff;
            staff->offset_right = v2(-6, -36);
            staff->offset_left = v2(-60, -36);
            staff->hit_size = v2(76, 81);
            staff->hit_offset_right = v2(20, 30);
            staff->hit_offset_left = v2(60, 30);
            staff->base_damage = 30;
            staff->damage_attribute = at_strength;
            staff->damage_multiplier = 1.5;
            staff->charge_time = 9;
            staff->icon = icon;
            staff->sprite_index = 0;
        } break;
    case pw_katana:
        {
            Image icon = LoadImage(S("katana_icon.png"));


            Weapon *katana = &w_list.data[pw_katana];

            katana->frames = load_weapon_frames(pw_katana);
            katana->icon = icon;
            katana->size = v2(106, 88);
            katana->weapon_frames = v2(4, 9);
            katana->type = pw_staff;
            katana->offset_right = v2(-18, -36);
            katana->offset_left = v2(-60, -36);
            katana->hit_size = v2(76, 81);
            katana->hit_offset_right = v2(20, 30);
            katana->hit_offset_left = v2(60, 30);
            katana->base_damage = 25;
            katana->damage_attribute = at_dexterity;
            katana->damage_multiplier = 1.3;
            katana->charge_time = 9;
            katana->icon = icon;
            katana->sprite_index = 0;
        } break;
    }
}

void weapon_attack(Vector2 pos, Weapon weapon, i32 facing, i32 dmg_attr, i32 attack_type) {
    i32 damage = 0;

    Rectangle2 sword_box_right = r2_bounds(v2(pos.x+weapon.hit_offset_right.x, pos.y-weapon.hit_offset_right.y), weapon.hit_size, v2_zero, v2_one);
    Rectangle2 sword_box_left = r2_bounds(v2(pos.x-weapon.hit_offset_left.x, pos.y-weapon.hit_offset_left.y), weapon.hit_size, v2_zero, v2_one);

    switch (attack_type)
    {
    case 0: 
        {
            damage = weapon.base_damage + (weapon.damage_multiplier*dmg_attr);
        } break;
    case 1: 
        {
            damage = 1.4*(weapon.base_damage + (weapon.damage_multiplier*dmg_attr));
        } break;
    case 2: 
        {
            damage = 2.5*(weapon.base_damage + (weapon.damage_multiplier*dmg_attr));
        } break;
    }  


    for (int i = 0; i < World[player.player_level].entities.count; i++) {
        Entity *ent = &World[player.player_level].entities.data[i];

        switch (ent->type)
        {
        case et_default:
            {

            } break;
        case et_player:
            {

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
                if (!ent->invuln && ent->attackable)
                {
                    if (facing > 0) {
                        if (r2_intersects(sword_box_right, ent->hitbox) 
                            && ent->alive) {

                            ent->invuln = true;

                            if (ent->has_hit) {
                                ent->state = HIT;
                            }
                            
                            if (ent->current_health <= damage) {
                                ent->current_health = 0;
                                ent->state = DYING;
                                ent->state_time = 0;
                                player.exp_gained += ent->enemy.exp_dropped;
                                if (player.mp_cooldown <= 0) {
                                    player.current_mp+=10;
                                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                                    player.mp_cooldown+=.5;
                                }
                                
                            } else {
                                ent->current_health-=damage;
                                ent->invuln = true;
                                if (player.mp_cooldown <= 0) {
                                    player.current_mp+=10;
                                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                                    player.mp_cooldown+=.5;
                                }
                            } 
                        }

                        Rectangle2 draw_box = r2_shift(sword_box_right, v2(camera_offset, 0));

                        DrawRectOutline(draw_box, v4_red, 2);
                        } else {
                            if (r2_intersects(sword_box_left, ent->hitbox) && ent->alive) {

                                ent->invuln = true;

                            if (ent->has_hit) {
                                ent->state = HIT;
                            }
                            
                            if (ent->current_health <= damage) {
                                ent->current_health = 0;
                                ent->state = DYING;
                                ent->state_time = 0;
                                player.exp_gained += ent->enemy.exp_dropped;
                                if (player.mp_cooldown <= 0) {
                                    player.current_mp+=10;
                                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                                    player.mp_cooldown+=.5;
                                }
                                
                            } else {
                                ent->current_health-=damage;
                                ent->invuln = true;
                                if (player.mp_cooldown <= 0) {
                                    player.current_mp+=10;
                                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                                    player.mp_cooldown+=.5;
                                }
                            }
                        }

                        Rectangle2 draw_box = r2_shift(sword_box_left, v2(camera_offset, 0));

                        DrawRectOutline(draw_box, v4_red, 2);

                    } break;
                }    
            }
        }
    }
}

void weapon_charged_attack(Weapon *wep, Rectangle2 hit_box) {
    i32 damage = 0;

    switch (wep->damage_attribute)
    {
    case at_strength:
        {
            damage = wep->base_damage+(wep->damage_multiplier*player.strength);
        } break;
    case at_dexterity:
        {
            damage = wep->base_damage+(wep->damage_multiplier*player.dexterity);
        } break;
    case at_mental:
        {
            damage = wep->base_damage+(wep->damage_multiplier*player.dexterity);
        } break;
    }

    Level *level = &World[player.player_level];

    for (int i = 0; i < level->entities.count; i++) {
        Entity *ent = &level->entities.data[i];

        if (!ent->invuln)
        {
            if (r2_intersects(hit_box, ent->hitbox))
            {
                ent->invuln = true;

                if (ent->has_hit) {
                    ent->state = HIT;
                }
                
                if (ent->current_health <= damage) {
                    ent->current_health = 0;
                    ent->state = DYING;
                    ent->state_time = 0;
                    player.exp_gained += ent->enemy.exp_dropped;
                    if (player.mp_cooldown <= 0) {
                        player.current_mp+=10;
                        player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                        player.mp_cooldown+=.5;
                    }
                    
                } else {
                    ent->current_health-=damage;
                    ent->invuln = true;
                    if (player.mp_cooldown <= 0) {
                        player.current_mp+=10;
                        player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                        player.mp_cooldown+=.5;
                    }
                }
            }
        }
    }
}

void draw_charging(i32 frame) {
    if (camera_pos.x != player.position.x) {
        DrawImage(charge_meter[frame], v2(player.position.x-camera_pos.x+out->width*.5-16, player.position.y-16));
    } else {
        DrawImage(charge_meter[frame], v2(camera_pos.x-player.position.x+out->width*.5-16, player.position.y-16));
    }
}