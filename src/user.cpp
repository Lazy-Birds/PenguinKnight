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
const i32 DYING = 14;
const i32 DEAD = 15;

#include "interactable.cpp"
#include "levels.cpp"

bool in_menu = false;
bool bosses_killed[48] = {};

struct texture {
    u32 pixel;
    Image image;
};

#include "weapon.cpp"

struct Enemy {
    Image *image;
    Vector2 frames;
    i32 damage;
    i32 type;
    Vector2 offset;
    i32 id;

    Vector2 weapon_size;
    Vector2 weapon_offset;

    f32 enemy_time ;
    i32 enemy_state;

    f32 sleep_time;
    i32 guard;

    Vector2 anchor_pos;
    i32 exp_dropped;
};

struct Entity
{
    i32 constitution;
    i32 rigour;
    i32 strength;
    i32 dexterity;
    i32 mental;

    i32 max_health;
    f32 current_health;
    f32 min_health;

    bool invuln;
    bool alive;
    bool has_hit;

    f32 max_stamina;
    f32 current_stamina;

    f32 max_mp;
    f32 current_mp;
    f32 mp_cooldown;
    f32 shield_hit;

    i32 fairy_uses;
    i32 current_fairy_uses;

    Vector2 check_point;
    Vector2 position;
    Vector2 velocity;
    Vector2 velocity_prev;

    Vector2 size;
    Vector2 anchor;

    i32 facing;
    i32 type;

    i32 sprite_index;
    f32 state_time;
    f32 animation_time;

    Weapon weapon;
    Image *projectile;
    b32 projectile_launched;

    texture wall_type;
    Enemy enemy;

    i32 state;
    i32 state_prev;

    Image *image;
    Image *portrait;

    String *dialogue;
    f32 dialogue_time;
    b32 talking;

    i32 exp_gained;
    i32 exp_to_level;
    i32 level;

    Level player_level;
};

#include "background.cpp"


bool enemy_overlap(Entity *entity);
Rectangle2 get_entity_rect(Entity *entity);
void draw_bound_box(Entity *entity);

f32 invuln_time = 0;

const i32 CAMERAFOLLOW = 0;
const i32 CAMERALOCKED = 1;

Vector2 camera_pos;
Vector2 camera_pos_target;
i32 camera_state = 0;

void player_hit(Entity *entity, Game_Input *input);

#include "dialogue.cpp"
#include "entity.cpp"
#include "menu.cpp"



Entity player = {};

bool swing_weapon;
bool charging_weapon;
bool on_cooldown;
bool charged_attack;

Image *charge_meter;

f32 walk_frame = 0;
f32 attack_frame = 0;
i32 charging = 0;
i32 weapon_cooldown = 0;

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
    for (int i = 0; i < enemy_count; i++) {
        if (entity->enemy.id == enemys[i].enemy.id) return false;
        if (!enemys[i].alive) continue;

        Rectangle2 rec1 = get_entity_rect(entity);
        Rectangle2 rec2 = get_entity_rect(&enemys[i]);

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


    for (int i = 0; i < enemy_count; i++) {
        if (!enemys[i].invuln)
        {
            if (facing > 0) {
                if (r2_intersects(r2_bounds(v2(pos.x+weapon.hit_offset_right.x, pos.y-weapon.hit_offset_right.y), weapon.hit_size, v2_zero, v2_one),
                    get_entity_rect(&enemys[i])) && enemys[i].alive) {

                    enemys[i].invuln = true;

                if (enemys[i].has_hit) {
                    enemys[i].state = HIT;
                }
                
                if (enemys[i].current_health <= damage) {
                    enemys[i].current_health = 0;
                    enemys[i].state = DYING;
                    enemys[i].state_time = 0;
                    player.exp_gained += enemys[i].enemy.exp_dropped;
                    if (player.mp_cooldown <= 0) {
                        player.current_mp+=10;
                        player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                        player.mp_cooldown+=.5;
                    }
                    
                } else {
                    enemys[i].current_health-=damage;
                    enemys[i].invuln = true;
                    if (player.mp_cooldown <= 0) {
                        player.current_mp+=10;
                        player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                        player.mp_cooldown+=.5;
                    }
                } 
            }
        } else {
            if (r2_intersects(r2_bounds(v2(pos.x-weapon.hit_offset_left.x, pos.y-weapon.hit_offset_left.y), weapon.hit_size, v2_zero, v2_one),
                get_entity_rect(&enemys[i])) && enemys[i].alive) {

                enemys[i].invuln = true;

            if (enemys[i].has_hit) {
                enemys[i].state = HIT;
            }
            
            if (enemys[i].current_health <= damage) {
                enemys[i].current_health = 0;
                enemys[i].state = DYING;
                enemys[i].state_time = 0;
                player.exp_gained += enemys[i].enemy.exp_dropped;
                if (player.mp_cooldown <= 0) {
                    player.current_mp+=10;
                    player.current_mp = clamp_f32(player.current_mp, 0, player.max_mp);
                    player.mp_cooldown+=.5;
                }
                
            } else {
                enemys[i].current_health-=damage;
                enemys[i].invuln = true;
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
        bosses_killed[i] = true;
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

    player.check_point = v2(9456, 476);//v2(624, out->height - 244);
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
    player.player_level = village;

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

    charge_meter = img;

    swing_weapon = false;
    charging_weapon = false;
    charged_attack = false;

    bosses_alive();

    spawn_initial_snowflakes(out, input);
}

void reset_vars() {
    TimeFunction;
    player.position = player.check_point;
    player.current_health = player.max_health;
    player.current_stamina = player.max_stamina;
    player.current_mp = player.max_mp;
    player.alive = true;
    player.exp_gained = 0;
    player.velocity = v2(0, 0);
    player.facing = -1;
    player.state = NEUTRAL;

    camera_pos = player.position;



    wall[10000] = {};
    wall_count = 0;
    enemys[1000] = {};
    enemy_count = 0;
    boss_walls_active = false;

    npc[200] = {};
    npc_count = 0;

    fire = {};

    bgnd[10000] = {};
    bgnd_count = 0;
    camera_state = CAMERAFOLLOW;
}

void set_world(b32 reset, Level level) {
    TimeFunction;
    if (reset){
        reset_vars();
    } else {
        wall[10000] = {};
        wall_count = 0;
        enemys[1000] = {};
        enemy_count = 0;

        camera_pos = player.position;

        npc[200] = {};
        npc_count = 0;

        fire = {};

        bgnd[10000] = {};
        bgnd_count = 0;

        housing[100] = {};
        house_count = 0;
        camera_state = CAMERAFOLLOW;
    }

    make_world(level);
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

   DrawImage(player.player_level.background, v2(new_pos_x, new_pos_y));
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
        set_world(false, village);
        initted = true;
    }

    Controller CC = input->controllers[0];

    if (menu_open) {
        if (ControllerReleased(0, Button_Start)) {
            if (menu_open) {
                menu_open = false;
            } else {
                menu_open = true;
            }
        }

        draw_menu(out, &player, screen);
    } else if (player.alive) {
        //DrawClear(v4(0.2, 0.2, 0.2, 1));

        draw_background();

        GameUpdate(input, out);
        GameRender(input, out);

        if (player.player_level.id == 0) {
            spawn_snowflakes(out, input);
            update_snowflakes(out, camera_pos.x);
        }

        check_level();
        set_camera_pos();

        update_projectiles(input, &player);

        for (int i = 0; i < bgnd_count; i++) {
            DrawImage(bgnd[i].image, v2(bgnd[i].position.x-camera_pos.x+out->width*.5, bgnd[i].position.y));
        }

        for (int i = 0; i < wall_count; i++) {
            DrawImage(wall[i].wall_type.image, v2(wall[i].position.x-camera_pos.x+out->width*.5, wall[i].position.y));
        }

        if (player.player_level.id == 0 && abs_i32(fire.position.x - player.position.x) < 1200) {
            draw_fire(input->dt);
        }

        for (int i = 0; i < house_count; i++) {
            if (abs_i32(housing[i].position.x - player.position.x) < 1200 && abs_i32(housing[i].position.y - player.position.y) < 500) {
                housing[i].state_time+=input->dt;
                draw_house(&housing[i]);
                if (housing[i].state_time*60 > 180) {
                    housing[i].state_time = 0;
                }
            }
        }

        for (int i = 0; i < npc_count; i++) {
            if (abs_i32(npc[i].position.x - player.position.x) < 1200) {
                npc_action(&npc[i], &player);
            }
        }

        for (int i = 0; i < enemy_count; i++) {
            if (enemys[i].enemy.type == 1 && enemys[i].alive) seal_action(&enemys[i], input, &player);
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
        
        for (int i = 0; i < enemy_count; i++) {
            if (enemys[i].alive) {

                if (enemys[i].enemy.type == 1) {
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5-2, enemys[i].position.y-2-12), v2(enemys[i].size.x+4+enemys[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5, enemys[i].position.y-12), v2(enemys[i].current_health/enemys[i].max_health*(enemys[i].size.x+4+enemys[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                    if (enemys[i].facing > 0) 
                    {
                        DrawImageMirrored(enemys[i].enemy.image[0], v2(enemys[i].position.x-camera_pos.x+out->width*.5 + enemys[i].enemy.offset.x,
                           enemys[i].position.y+enemys[i].enemy.offset.y), true, false);
                    } else 
                    {
                        DrawImage(enemys[i].enemy.image[0], v2(enemys[i].position.x-camera_pos.x+out->width*.5 + enemys[i].enemy.offset.x,
                           enemys[i].position.y+enemys[i].enemy.offset.y));
                    }
                } else if (enemys[i].enemy.type == 4 && abs_f32(player.position.x - enemys[i].position.x) < 900 && abs_i32(player.position.y - enemys[i].position.y) < 700) {
                    p_soldier_action(&enemys[i], input->dt, &player, invuln_time, input);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5-2, enemys[i].position.y-2-12), v2(enemys[i].size.x+4-enemys[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5, enemys[i].position.y-12), v2(enemys[i].current_health/enemys[i].max_health*(enemys[i].size.x+4-enemys[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                } else if (!bosses_killed[0] && enemys[i].enemy.type == 3 && player.position.x > 8640 && player.position.x < 9504) {
                    camera_state = CAMERALOCKED;
                    camera_pos_target = v2(9072, out->height);

                    if (!boss_walls_active)
                    {
                        Vector2 boss_walls[] = {
                            v2(8592, 480),
                            v2(8592, 432),
                            v2(8592, 384),
                            v2(8592, 336),
                            v2(8592, 288),
                            v2(8592, 240),
                            v2(8592, 192),
                            v2(8592, 144),
                            v2(9504, 480),
                            v2(9504, 432),
                        };

                        make_boss_walls(boss_walls, 10);

                        boss_walls_active = true;
                    }

                    

                    penguin_king_action(&enemys[i], &player, input->dt, out);

                } else if (enemys[i].type == 6 && abs_i32(enemys[i].position.x - player.position.x) < 1200) {
                    slime_action(&enemys[i], &player, input);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5-2, enemys[i].position.y-2-12), v2(enemys[i].size.x+4-enemys[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5, enemys[i].position.y-12), v2(enemys[i].current_health/enemys[i].max_health*(enemys[i].size.x+4-enemys[i].enemy.offset.x),
                        8), v2_zero, v2_one), v4_red);
                }

            }
        }


        player_action(input);
        player_move(input);



        if (ControllerReleased(0, Button_Start)) {
            if (menu_open) {
                menu_open = false;
            } else {
                menu_open = true;
            }
        }



            /*if (CC.pause) draw_bounding_boxes = true;

            if (draw_bounding_boxes) {
                for (int i = 0; i < enemy_count; i++) {

                    draw_bound_box(&enemys[i]);
                }
                draw_bound_box(&player);

            }*/

        particle_update(input->dt);

        if (menu_open) {
            screen = MakeScreenImage(out);
            draw_menu(out, &player, screen);
        }
        
    } else {
        String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
        Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

        DrawClear(v4(0.2, 0.2, 0.2, 1));

        DrawTextExt(font_hellomyoldfriend, S("You Died"), v2(out->width/2 - 144, out->height/2-18), v4_red, v2_zero, 3.0);
        DrawTextExt(font_hellomyoldfriend, S("Press Q to Respawn"), v2(out->width/2 - 324, out->height/2+54), v4_red, v2_zero, 3.0);

        if (ControllerReleased(0, Button_A)) {
            set_world(true, village);
        }

        /*Image you_died = LoadImage(S("you_died.png"));

        DrawImage(you_died, v2(out->width/2-196, out->height/2-48));*/
    }

}