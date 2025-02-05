Weapon *w_list = {};

enum fr_cleaver {
    fr_neutral,
    fr_walk,
    fr_damage,
    fr_atk_one,
    fr_atk_two,
    fr_atk_three,
    fr_atk_four,
    fr_atk_five,
    fr_atk_six,
    fr_atk_seven,
    fr_atk_eight,
    fr_atk_nine,
    fr_atk_ten,
    fr_atk_eleven,
    fr_atk_twelve,
    fr_atk_thirteen,
    fr_atk_fourteen,
    fr_atk_fifteen,
    fr_atk_sixteen,
    fr_chatk_one,
    fr_chatk_two,
    fr_chatk_three,
    fr_chatk_four,
    fr_chatk_five,
    fr_jmp_atk_one,
    fr_jmp_atk_two,
    fr_sprint_one,
    fr_sprint_two,
    fr_sprint_three,
    fr_sprint_four,
    fr_jmp_one,
    fr_jmp_two,
    fr_grd_one,
    fr_grd_two,
    fr_grd_three,
    fr_dodge_one,
    fr_dodge_two,
    fr_dodge_three,
    fr_dodge_four,
    fr_dodge_five,
    fr_dodge_six,
};

void load_weapon(i32 weapon) {

    /*static Image sword_image[] = {
        LoadImage(S("Excalibrrr_left1.png")),
        LoadImage(S("Excalibrrr_left2.png")),
        LoadImage(S("Excalibrrr_left3.png")),
        LoadImage(S("Excalibrrr_left4.png")),
        LoadImage(S("Excalibrrr_left5.png")),
        LoadImage(S("Excalibrrr_left6.png")),
        LoadImage(S("Excalibrrr_right1.png")),
        LoadImage(S("Excalibrrr_right2.png")),
        LoadImage(S("Excalibrrr_right3.png")),
        LoadImage(S("Excalibrrr_right4.png")),
        LoadImage(S("Excalibrrr_right5.png")),
        LoadImage(S("Excalibrrr_right6.png")),
    };
    sword.image = sword_image;
    sword.size = v2(48, 96);
    sword.weapon_frames = v2(2, 6);
    String sname = S("Excalibrrr");
    sword.name = sname;
    */
    switch (weapon)
    {
    case pw_cleaver:
        {
            load_cleaver();
        } break;
    case pw_staff:
        {
            load_staff();
        } break;
   }
}

void load_cleaver() {
    Weapon *cleaver = &w_list[pw_cleaver];

     static Image cleaver_image[] = {
        LoadImage(S("cleaver1.png")),
        LoadImage(S("cleaver2.png")),
        LoadImage(S("cleaver3.png")),
        LoadImage(S("cleaver4.png")),
        LoadImage(S("cleaver5.png")),
        LoadImage(S("cleaver6.png")),
        LoadImage(S("cleaver7.png")),
        LoadImage(S("cleaver8.png")),
        LoadImage(S("cleaver9.png")),
        LoadImage(S("cleaver10.png")),
        LoadImage(S("cleaver11.png")),
        LoadImage(S("cleaver12.png")),
        LoadImage(S("cleaver13.png")),
        LoadImage(S("cleaver14.png")),
        LoadImage(S("cleaver15.png")),
        LoadImage(S("cleaver16.png")),
        LoadImage(S("cleaver17.png")),
        LoadImage(S("cleaver18.png")),
        LoadImage(S("cleaver19.png")),
        LoadImage(S("cleaver20.png")),
        LoadImage(S("cleaver21.png")),
        LoadImage(S("cleaver22.png")),
        LoadImage(S("cleaver23.png")),
        LoadImage(S("cleaver24.png")),
        LoadImage(S("cleaver25.png")),
        LoadImage(S("cleaver26.png")),
        LoadImage(S("cleaver27.png")),
        LoadImage(S("cleaver28.png")),
        LoadImage(S("cleaver29.png")),
        LoadImage(S("cleaver30.png")),
        LoadImage(S("cleaver31.png")),
        LoadImage(S("cleaver32.png")),
        LoadImage(S("cleaver33.png")),
        LoadImage(S("cleaver34.png")),
        LoadImage(S("cleaver35.png")),
        LoadImage(S("cleaver36.png")),
        LoadImage(S("cleaver37.png")),
        LoadImage(S("cleaver38.png")),
        LoadImage(S("cleaver39.png")),
        LoadImage(S("cleaver40.png")),
        LoadImage(S("cleaver41.png")),
    };

    cleaver->image = cleaver_image;

    static Image psn_img[] = {
        LoadImage(S("cleaver_poisoned1.png")),
        LoadImage(S("cleaver_poisoned2.png")),
        LoadImage(S("cleaver_poisoned3.png")),
        LoadImage(S("cleaver_poisoned4.png")),
    };

    cleaver->poisoned_image = psn_img;

    static Image icon = LoadImage(S("cleaver_icon.png"));
    static Image proj[] = {
        LoadImage(S("cleaver_charged1.png")),
        LoadImage(S("cleaver_charged2.png")),
        LoadImage(S("cleaver_charged3.png")),
        LoadImage(S("cleaver_charged4.png")),
        LoadImage(S("cleaver_charged5.png")),
    };

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
}

void load_staff() {
    static Image staff_image[] = {
        LoadImage(S("staff1.png")),
        LoadImage(S("staff2.png")),
        LoadImage(S("staff3.png")),
        LoadImage(S("staff4.png")),
        LoadImage(S("staff5.png")),
        LoadImage(S("staff6.png")),
        LoadImage(S("staff7.png")),
        LoadImage(S("staff8.png")),
        LoadImage(S("staff9.png")),
        LoadImage(S("staff10.png")),
        LoadImage(S("staff11.png")),
        LoadImage(S("staff12.png")),
        LoadImage(S("staff13.png")),
        LoadImage(S("staff14.png")),
        LoadImage(S("staff15.png")),
        LoadImage(S("staff16.png")),
        LoadImage(S("staff17.png")),
        LoadImage(S("staff18.png")),
        LoadImage(S("staff19.png")),
        LoadImage(S("staff20.png")),
        LoadImage(S("staff21.png")),
        LoadImage(S("staff22.png")),
        LoadImage(S("staff23.png")),
        LoadImage(S("staff24.png")),
        LoadImage(S("staff25.png")),
        LoadImage(S("staff26.png")),
        LoadImage(S("staff27.png")),
        LoadImage(S("staff28.png")),
        LoadImage(S("staff29.png")),
        LoadImage(S("staff30.png")),
        LoadImage(S("staff31.png")),
        LoadImage(S("staff32.png")),
        LoadImage(S("staff33.png")),
        LoadImage(S("staff34.png")),
        LoadImage(S("staff35.png")),
        LoadImage(S("staff36.png")),
        LoadImage(S("staff37.png")),
        LoadImage(S("staff38.png")),
        LoadImage(S("staff39.png")),
        LoadImage(S("staff40.png")),
        LoadImage(S("staff41.png")),
    };

    Image icon = LoadImage(S("staff_icon.png"));


    Weapon *staff = &w_list[pw_staff];

    staff->image = staff_image;
    staff->icon = icon;
    staff->size = v2(142, 88);
    staff->weapon_frames = v2(4, 9);
    staff->type = pw_staff;
    staff->offset_right = v2(-42, -36);
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