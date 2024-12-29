#include "pengu_def.h"

//General States
const i32 NEUTRAL = 0;
const i32 MOVE = 1;
const i32 ATTACK = 2;
const i32 CHARGING = 3;
const i32 STATECHARGEATTACKING = 4;
const i32 STATEJUMPATTACK = 5;
const i32 EXHAUSTED = 6;
const i32 LANDED = 7;
const i32 HIT = 8;
const i32 STATEDASH = 9;
const i32 TALKING = 10;
const i32 JUMP = 11;
const i32 GUARD = 12;
const i32 SHOOT = 13;
const i32 HOOKSHOTTING = 14;
const i32 DYING = 15;
const i32 DEAD = 16;

//Pengu King Phases
const i32 KINGNEUTRAL = 50;
const i32 KINGSTOMPING = 51;
const i32 KINGJUMPING = 52;
const i32 KINGLEAPING = 53;
const i32 KINGLANDING = 54;
const i32 KINGGUARDBREAK = 55;
const i32 KINGCOMMAND = 56;
const i32 KINGWAIT = 57;
const i32 KINGCHALLENGE = 58;

const i32 PHASEONE = 0;
const i32 PHASETWO = 1;

i32 king_phase = PHASEONE;

//Ooze States
const i32 SPYING = 100;
const i32 HAMMER = 101;
const i32 LAZER = 102;
const i32 CORRUPTING = 103;

i32 corruption_phase = 0;
Vector2 corrupt_pos = v2_zero;

//Camera Consts
const i32 CAMERAFOLLOW = 0;
const i32 CAMERALOCKED = 1;

String font_chars = {};
Font font_hellomyoldfriend = {};

Entity player = {};
Entry_Points tele[100] = {};
i32 tele_count = 0;

EntityArray draw_last = {};
i32 last_count = 0;

bool swing_weapon;
bool charging_weapon;
bool on_cooldown;
bool charged_attack;
bool in_menu = false;
bool in_cinematic = false;
bool bosses_killed[48] = {};
bool gate_scene;

f32 invuln_time = 0;
f32 group_time = 0;

Vector2 camera_pos;
Vector2 camera_pos_target;
f32 camera_offset = 0;
i32 camera_state = 0;
Image *charge_meter;
f32 walk_frame = 0;
f32 attack_frame = 0;
i32 charging = 0;
i32 weapon_cooldown = 0;

#include "levels.cpp"
#include "weapon.cpp"
#include "background.cpp"
#include "dialogue.cpp"
#include "entity.cpp"
#include "menu.cpp"

void set_camera_pos() {
    if (camera_state == CAMERAFOLLOW) {
        camera_pos = lerp_v2(camera_pos, player.position, 0.08);
    } else if (camera_state == CAMERALOCKED) {
        camera_pos = lerp_v2(camera_pos, camera_pos_target, 0.08);
    } else {
        camera_pos = lerp_v2(camera_pos, player.position, 0.08);
    }
}

Rectangle2 get_entity_rect(Entity *entity) {
    return r2_bounds(entity->position, entity->size, v2_zero, v2_one);
}

bool enemy_overlap(Entity *entity) {
    for (int i = 0; i < World[player.player_level].enemies.count; i++) {
        if (entity->enemy.id == World[player.player_level].enemies.data[i].enemy.id) return false;
        if (!World[player.player_level].enemies.data[i].alive) continue;

        Rectangle2 rec1 = get_entity_rect(entity);
        Rectangle2 rec2 = get_entity_rect(&World[player.player_level].enemies.data[i]);

        if (r2_intersects(rec1, rec2)) {
            return true;
        }
    }
    return false;
}

void draw_bound_box(Entity *entity) {
    Rectangle2 rec = get_entity_rect(entity);
    rec = r2_shift(rec, v2(-camera_pos.x+out->width*.5, 0));

    DrawRectOutline(rec, v4_red, 1);
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


    for (int i = 0; i < World[player.player_level].enemies.count; i++) {
        if (!World[player.player_level].enemies.data[i].invuln && World[player.player_level].enemies.data[i].attackable)
        {
            if (facing > 0) {
                if (r2_intersects(sword_box_right, World[player.player_level].enemies.data[i].hitbox) 
                    && World[player.player_level].enemies.data[i].alive) {

                    World[player.player_level].enemies.data[i].invuln = true;

                if (World[player.player_level].enemies.data[i].has_hit) {
                    World[player.player_level].enemies.data[i].state = HIT;
                }
                
                if (World[player.player_level].enemies.data[i].current_health <= damage) {
                    World[player.player_level].enemies.data[i].current_health = 0;
                    World[player.player_level].enemies.data[i].state = DYING;
                    World[player.player_level].enemies.data[i].state_time = 0;
                    player.exp_gained += World[player.player_level].enemies.data[i].enemy.exp_dropped;
                    if (player.mp_cooldown <= 0) {
                        player.current_mp+=10;
                        player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                        player.mp_cooldown+=.5;
                    }
                    
                } else {
                    World[player.player_level].enemies.data[i].current_health-=damage;
                    World[player.player_level].enemies.data[i].invuln = true;
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
            if (r2_intersects(sword_box_left, World[player.player_level].enemies.data[i].hitbox) 
                && World[player.player_level].enemies.data[i].alive) {

                World[player.player_level].enemies.data[i].invuln = true;

            if (World[player.player_level].enemies.data[i].has_hit) {
                World[player.player_level].enemies.data[i].state = HIT;
            }
            
            if (World[player.player_level].enemies.data[i].current_health <= damage) {
                World[player.player_level].enemies.data[i].current_health = 0;
                World[player.player_level].enemies.data[i].state = DYING;
                World[player.player_level].enemies.data[i].state_time = 0;
                player.exp_gained += World[player.player_level].enemies.data[i].enemy.exp_dropped;
                if (player.mp_cooldown <= 0) {
                    player.current_mp+=10;
                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                    player.mp_cooldown+=.5;
                }
                
            } else {
                World[player.player_level].enemies.data[i].current_health-=damage;
                World[player.player_level].enemies.data[i].invuln = true;
                if (player.mp_cooldown <= 0) {
                    player.current_mp+=10;
                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                    player.mp_cooldown+=.5;
                }
            }
        }

        Rectangle2 draw_box = r2_shift(sword_box_left, v2(camera_offset, 0));

        DrawRectOutline(draw_box, v4_red, 2);
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

void bosses_alive() {
    for (int i = 0; i < 48; i++) {
        bosses_killed[i] = false;
    }
}

void GameStart(Game_Input *input, Game_Output *out)
{
    load_weapon();

    player.constitution = 10;
    player.rigour = 10;
    player.strength = 10;
    player.dexterity = 10;
    player.mental = 10;

    player.check_point = /*v2(1298, 524);v2(480, 524);//v2(8449, 476);*/v2(672, out->height - 244);
    player.check_point_level = 0;
    player.position = player.check_point;
    player.anchor = v2(player.position.x+15, player.position.y+25);
    player.facing = -1;
    player.max_health = 10*player.constitution;
    player.current_health = player.max_health;
    player.min_health = 0;
    player.max_stamina = 10*player.rigour;
    player.current_stamina = player.max_stamina;
    player.max_mp = 10*player.mental;
    player.current_mp = player.max_mp;
    player.mp_cooldown = 0;
    player.shield_hit = 0;
    player.fairy_uses = 0;
    player.current_fairy_uses = player.fairy_uses;
    player.invuln = false;
    player.alive = true;
    player.weapon = cleaver;
    player.weapon.position = player.position;
    player.size = v2(40, 52);
    player.exp_gained = 0;
    player.exp_to_level = 600;
    player.level = 1;
    player.type = 0;
    player.player_level = player.check_point_level;
    player.acting_cd = 0;
    player.jumpies = LoadSound(S("Jump.wav"));
    player.hit = LoadSound(S("pengu_hurt.wav"));

    static Image img[] = {
        LoadImage(S("charge_left1.png")),
        LoadImage(S("charge_left2.png")),
        LoadImage(S("charge_left3.png")),
        LoadImage(S("charge_left4.png")),
        LoadImage(S("charge_left5.png")),
        LoadImage(S("charge_left6.png")),
        LoadImage(S("charge_left7.png")),
        LoadImage(S("charge_left8.png")),
        LoadImage(S("charge_left9.png")),
        LoadImage(S("charge_right1.png")),
        LoadImage(S("charge_right2.png")),
        LoadImage(S("charge_right3.png")),
        LoadImage(S("charge_right4.png")),
        LoadImage(S("charge_right5.png")),
        LoadImage(S("charge_right6.png")),
        LoadImage(S("charge_right7.png")),
        LoadImage(S("charge_right8.png")),
        LoadImage(S("charge_right9.png")),
    };

    camera_pos = player.position;
    camera_offset = -camera_pos.x+out->width*.5;

    charge_meter = img;

    swing_weapon = false;
    charging_weapon = false;
    charged_attack = false;
    gate_scene = false;

    bosses_alive();

    spawn_initial_snowflakes(out, input);

    font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
    font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));
}

void reset_vars() {
    TimeFunction;
    player.position = player.check_point;
    player.player_level = player.check_point_level;
    player.current_health = player.max_health;
    player.current_stamina = player.max_stamina;
    player.current_mp = player.max_mp;
    player.alive = true;
    player.exp_gained = 0;
    player.velocity = v2(0, 0);
    player.facing = -1;
    player.state = NEUTRAL;
    player.current_fairy_uses = player.fairy_uses;

    camera_pos = player.position;

    boss_walls_active = false;

    camera_state = CAMERAFOLLOW;
}

void set_world(b32 reset, Level level) {
    TimeFunction;
    if (reset){
        reset_vars();
    } else {
        camera_state = CAMERAFOLLOW;
        camera_pos = player.position;
    }

    if (!World[player.player_level].initialized) {
        make_world(World[player.player_level]);
    }

    reset_enemies();
    reset_particles();
    reset_projectiles();
}

i32 get_frame_walk(f32 dt) {
    return floor_f32(walk_frame*dt*3);
}

i32 get_frame_attack(Weapon weapon, f32 dt) {
    return floor_f32(weapon.weapon_frames.x+attack_frame*dt*10);
}

i32 get_frame_charged(f32 dt, i32 ch_multi) {
    return floor_f32(charging*dt*ch_multi);
}

i32 get_dmg_attr() {
    i32 dmg_attr = 0;

    if (string_equals(player.weapon.damage_attribute, S("Strength"))) {
        return player.strength;
    } else if (string_equals(player.weapon.damage_attribute, S("Dexterity"))) {
        return player.dexterity;
    } else if (string_equals(player.weapon.damage_attribute, S("Mental"))) {
        return player.mental;
    } else {
        Dump(S("We have a problem"));
        return NULL;
    }
}

#include "player.cpp"

void GameUpdate(Game_Input *input, Game_Output *out)
{   

    Controller c0 = input->controllers[0];
    f32 dt = input->dt;

    /*if (game_time.seconds > 2) {
        


        if (!charged_attack)
        {   
            if (input->mouse.right) {
                Dump(S("Right Click!"));
            }
            if ((input->mouse.right) && player.current_stamina > 40 && !swing_weapon && !on_cooldown)
            {
                charging_weapon = true;
            } else if (charging > 0 && !input->mouse.right && !on_cooldown) {
                on_cooldown = true;
            } else if (on_cooldown && weapon_cooldown*dt*7 == 9) {
                on_cooldown = false;
                weapon_cooldown = 0;
            } else if (!on_cooldown && get_frame_charged(dt, player.weapon.charge_time_multiplier) > 12) {
                charging = 0;   \
                
            }
        }
    }*/
    if (player.current_stamina < player.max_stamina) {
        player.current_stamina+=20*dt;
    }
}

void draw_background()
{
   i32 new_pos_x = -camera_pos.x*.02;
   i32 new_pos_y = 0;

   new_pos_x = Clamp(new_pos_x, -280, 0);
   new_pos_y = Clamp(new_pos_y, -200, 0);

   DrawImage(World[player.player_level].background, v2(new_pos_x, new_pos_y));
}

void GameRender(Game_Input *input, Game_Output *out)
{

}

void GameUpdateAndRender(Game_Input *input, Game_Output *out)
{
    static b32 initted = false;
    static b32 menu_open = false;
    static Image screen = {};



    if (!initted)
    {
        create_levels();
        GameStart(input, out);
        set_world(false, World[0]);
        draw_last = make_entity_array_two(pengu_arena, 100);
        initted = true;
    }

    Controller CC = input->controllers[0];

    if (in_menu) {
        if (ControllerReleased(0, Button_Start)) {
            in_menu = false;
            set_world(true, World[player.player_level]);
        }

        draw_menu(out, &player, screen);
    } else if (player.alive) {
        MixerOutputPlayingSounds();
        //DrawClear(v4(0.2, 0.2, 0.2, 1));

        Level *level = &World[player.player_level];
        group_time+=input->dt;

        draw_background();

        GameUpdate(input, out);
        GameRender(input, out);

        if (player.player_level == 0) {
            spawn_snowflakes(out, input);
            update_snowflakes(out, camera_pos.x);
        }

        check_level();
        set_camera_pos();
        camera_offset = -camera_pos.x+out->width*.5;

        update_projectiles(input, &player);

        for (int i = 0; i < World[player.player_level].backgrounds.count; i++) {

            if (level->backgrounds.data[i].id == 7) {
                draw_last.data[last_count] = level->backgrounds.data[i];
                if (group_time*60 < 45) {
                    draw_last.data[last_count].sprite_index = 0;
                } else {
                    draw_last.data[last_count].sprite_index = 1;
                }
                
                last_count++;
            } else {
                DrawImage(World[player.player_level].backgrounds.data[i].image[0], v2(World[player.player_level].backgrounds.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].backgrounds.data[i].position.y));
            }
            

            if (level->backgrounds.data[i].id == 6) {
                level->backgrounds.data[i].state_time+=input->dt;
                if (level->backgrounds.data[i].state_time*60 > 60 && random_f32_between(0, 1) < .01) {
                    generate_drips(v2(level->backgrounds.data[i].position.x+15, level->backgrounds.data[i].position.y+36), level->backgrounds.data[i].projectile[0], input);
                    level->backgrounds.data[i].state_time = 0;
                }
                
            }
        }

        for (int i = 0; i < World[player.player_level].liquid.count; i++) {
            liquid_do_liquid(&World[player.player_level].liquid.data[i], input, i, out);
        }

        

        /*if (World[player.player_level].fire_count != 0 && abs_i32(World[player.player_level].fire[0].position.x - player.position.x) < 1200) {
            draw_fire(input->dt);
        }*/

        for (int i = 0; i < World[player.player_level].interactible.count; i++)
        {
            if (r2_intersects(get_entity_rect(&player), get_entity_rect(&World[player.player_level].interactible.data[i])) && !World[player.player_level].interactible.data[i].acting 
                && World[player.player_level].interactible.data[i].actable)
            {
                Rectangle2 text_box = r2_bounds(v2(World[player.player_level].interactible.data[i].position.x-28-camera_pos.x+out->width*.5, World[player.player_level].interactible.data[i].position.y-28),
                    v2(104, 20), v2_zero, v2_one);

                DrawRect(text_box, v4_black);
                DrawTextExt(font_hellomyoldfriend, S("Press U."), v2(World[player.player_level].interactible.data[i].position.x-24-camera_pos.x+out->width*.5,
                    World[player.player_level].interactible.data[i].position.y-24), v4_white, v2_zero, 1.0);

                if ((player.state == NEUTRAL || player.state == MOVE) && player.acting)
                {
                    World[player.player_level].interactible.data[i].acting = true;
                }
            } else if (World[player.player_level].interactible.data[i].action_id == 4 && r2_intersects(get_entity_rect(&player), get_entity_rect(&World[player.player_level].interactible.data[i])) && !World[player.player_level].interactible.data[i].acting 
                && World[player.player_level].interactible.data[i].actable) {

            } else if (level->interactible.data[i].id == 5 && player.acting && player.acting_cd <= 0 && clear_line_of_sight(&player, &level->interactible.data[i])) {
                if (abs_f32(player.anchor.x - level->interactible.data[i].anchor.x) < 10 && abs_f32(player.anchor.y - level->interactible.data[i].anchor.y) < 10) {
                    player.acting_cd = 60*input->dt;
                    player.state = NEUTRAL;
                } else {
                    player.state = HOOKSHOTTING;

                    f32 velocity = 6000*input->dt;

                    f32 radians = angle_between(player.anchor, level->interactible.data[i].anchor);
                    Vector2 step = v2_arm(radians);

                    player.velocity.x += velocity*step.x;
                    player.velocity.y += velocity*step.y;

                    player.velocity.x = clamp_f32(player.velocity.x, -25000*input->dt, 25000*input->dt);
                    player.velocity.y = clamp_f32(player.velocity.y, -25000*input->dt, 25000*input->dt);
                    //draw_hook_shot(&level->interactible.data[i]);
                }
            }
            
            if (World[player.player_level].interactible.data[i].acting)
            {
                interact(&World[player.player_level].interactible.data[i], input, camera_pos);
            } else if (World[player.player_level].interactible.data[i].action_id == 4)
            {
                draw_fire(&World[player.player_level].interactible.data[i], input);
            } else
            {
                DrawImage(level->interactible.data[i].image[0], v2(level->interactible.data[i].position.x+camera_offset, level->interactible.data[i].position.y));
                if (level->interactible.data[i].id == 5) {
                }
            }
        }

        for (int i = 0; i < World[player.player_level].wall.count; i++) {
            DrawImage(World[player.player_level].wall.data[i].image[0], v2(World[player.player_level].wall.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].wall.data[i].position.y));
        }

        for (int i = 0; i < World[player.player_level].housing.count; i++) {
            if (abs_i32(World[player.player_level].housing.data[i].position.x - player.position.x) < 1200 && abs_i32(World[player.player_level].housing.data[i].position.y - player.position.y) < 500) {
                World[player.player_level].housing.data[i].state_time+=input->dt;
                draw_house(&World[player.player_level].housing.data[i]);
            }
        }

        for (int i = 0; i < World[player.player_level].npcs.count; i++) {
            if (entity_get_distance_x(&player, &level->npcs.data[i]) < 1200) {
                npc_action(&World[player.player_level].npcs.data[i], &player);
            }
        }

        for (int i = 0; i < World[player.player_level].enemies.count; i++) {
            if (World[player.player_level].enemies.data[i].enemy.type == 1 && World[player.player_level].enemies.data[i].alive) seal_action(&World[player.player_level].enemies.data[i], input, &player);
        }

        f32 dt = input->dt;

        Image icon = LoadImage(S("icon.png"));
        Image weapon_icon = LoadImage(S("Excalibrrr_icon.png"));
        Image fairy_icon[] = {LoadImage(S("fairy_icon1.png")), LoadImage(S("fairy_icon2.png"))};

        DrawImage(icon, v2(16, 16));
        DrawImage(player.weapon.icon, v2(25, 25));

        if (player.fairy_uses > 0) {
            DrawRect(r2_bounds(v2(70, 64), v2(30*player.fairy_uses+2, 12), v2_zero, v2_one), v4_black);
            for (int i = 0; i < player.current_fairy_uses; i++) {
                DrawRect(r2_bounds(v2(72+(14+16)*i, 66), v2(16+12, 8), v2_zero, v2_one), rgb(162, 221, 225));
            }

            if (i32(player.state_time*60)%60 < 30) {
                DrawImage(fairy_icon[0], v2(24, 66));
            } else {
                DrawImage(fairy_icon[1], v2(24, 66));

            }
        }

        DrawRect(r2_bounds(v2(70, 20), v2(20+player.max_health, 12), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(72, 22), v2(16+player.current_health, 8), v2_zero, v2_one), v4_red);

        DrawRect(r2_bounds(v2(70, 34), v2(20+player.max_stamina, 12), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(72, 36), v2(16+player.current_stamina, 8), v2_zero, v2_one), v4_green);

        DrawRect(r2_bounds(v2(70, 48), v2(20+player.max_mp, 12), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(72, 50), v2(16+player.current_mp, 8), v2_zero, v2_one), v4_blue);
        
        for (int i = 0; i < World[player.player_level].enemies.count; i++) {
            if (World[player.player_level].enemies.data[i].alive) {

                if (World[player.player_level].enemies.data[i].enemy.type == 1) {
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5-2, World[player.player_level].enemies.data[i].position.y-2-12), v2(World[player.player_level].enemies.data[i].size.x+4+World[player.player_level].enemies.data[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].enemies.data[i].position.y-12), v2(World[player.player_level].enemies.data[i].current_health/World[player.player_level].enemies.data[i].max_health*(World[player.player_level].enemies.data[i].size.x+4+World[player.player_level].enemies.data[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                    if (World[player.player_level].enemies.data[i].facing > 0) 
                    {
                        DrawImageMirrored(World[player.player_level].enemies.data[i].image[0], v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5 + World[player.player_level].enemies.data[i].enemy.offset.x,
                           World[player.player_level].enemies.data[i].position.y+World[player.player_level].enemies.data[i].enemy.offset.y), true, false);
                    } else 
                    {
                        DrawImage(World[player.player_level].enemies.data[i].image[0], v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5 + World[player.player_level].enemies.data[i].enemy.offset.x,
                           World[player.player_level].enemies.data[i].position.y+World[player.player_level].enemies.data[i].enemy.offset.y));
                    }
                } else if (World[player.player_level].enemies.data[i].enemy.type == 4 && abs_f32(player.position.x - World[player.player_level].enemies.data[i].position.x) < 900 && abs_i32(player.position.y - World[player.player_level].enemies.data[i].position.y) < 700) {
                    p_soldier_action(&World[player.player_level].enemies.data[i], input->dt, &player, invuln_time, input);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5-2, World[player.player_level].enemies.data[i].position.y-2-12), v2(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].enemies.data[i].position.y-12), v2(World[player.player_level].enemies.data[i].current_health/World[player.player_level].enemies.data[i].max_health*(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                } else if (!bosses_killed[0] && World[player.player_level].enemies.data[i].enemy.type == 3 && player.position.x > 8400 && player.position.x < 9504) {
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


                    

                    penguin_king_action(&World[player.player_level].enemies.data[i], &player, input->dt, out);

                } else if (World[player.player_level].enemies.data[i].type == 6 && abs_i32(World[player.player_level].enemies.data[i].position.x - player.position.x) < 1200) {
                    slime_action(&World[player.player_level].enemies.data[i], &player, input);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5-2, World[player.player_level].enemies.data[i].position.y-2-12), v2(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].enemies.data[i].position.y-12), v2(World[player.player_level].enemies.data[i].current_health/World[player.player_level].enemies.data[i].max_health*(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x),
                        8), v2_zero, v2_one), v4_red);
                } else if (World[player.player_level].enemies.data[i].type == 7 && entity_get_distance_x(&level->enemies.data[i], &player) < 1200 && entity_get_distance_y(&level->enemies.data[i], &player) < 300) {
                    eye_monster_action(&World[player.player_level].enemies.data[i], input);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5-2, World[player.player_level].enemies.data[i].position.y-2-12), v2(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(World[player.player_level].enemies.data[i].position.x-camera_pos.x+out->width*.5, World[player.player_level].enemies.data[i].position.y-12), v2(World[player.player_level].enemies.data[i].current_health/World[player.player_level].enemies.data[i].max_health*(World[player.player_level].enemies.data[i].size.x+4-World[player.player_level].enemies.data[i].enemy.offset.x),
                        8), v2_zero, v2_one), v4_red);
                } else if (level->enemies.data[i].type == 8 && entity_get_distance_x(&player, &level->enemies.data[i]) < 700) {
                    ooze_action(&level->enemies.data[i], input);
                }

            }
        }

        for (int i = 0; i < last_count; i++) {
            DrawImage(draw_last.data[i].image[draw_last.data[i].sprite_index], v2(draw_last.data[i].position.x+camera_offset, draw_last.data[i].position.y));
        }
        last_count = 0;


        player_action(input);
        player_move(input);
        
        Rectangle2 player_rec = get_entity_rect(&player);
        player_rec = r2_shift(player_rec, v2(camera_offset, 0));

    DrawRectOutline(player_rec, v4_red, 2);


        /*if (ControllerReleased(0, Button_Start)) {
            if (menu_open) {
                menu_open = false;
            } else {
                menu_open = true;
            }
        }*/



           
        /*
        for (int i = 0; i < World[player.player_level].enemies.count; i++) {

            draw_bound_box(&World[player.player_level].enemies.data[i]);
        }

        draw_bound_box(&player);
        */
            

        particle_update(input->dt);

        if (in_menu) {
            screen = MakeScreenImage(out);
            draw_menu(out, &player, screen);
        }
        
    } else {
        String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
        Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

        DrawClear(v4(0.2, 0.2, 0.2, 1));

        DrawTextExt(font_hellomyoldfriend, S("You Died"), v2(out->width/2 - 144, out->height/2-18), v4_red, v2_zero, 3.0);
        DrawTextExt(font_hellomyoldfriend, S("Press I to Respawn"), v2(out->width/2 - 324, out->height/2+54), v4_red, v2_zero, 3.0);

        if (ControllerReleased(0, Button_A)) {
            set_world(true, World[player.check_point_level]);
        }

        /*Image you_died = LoadImage(S("you_died.png"));

        DrawImage(you_died, v2(out->width/2-196, out->height/2-48));*/
    }

}