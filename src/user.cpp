const i32 NEUTRAL = 0;
const i32 MOVE = 1;
const i32 ATTACK = 2;
const i32 CHARGING = 3;
const i32 STATECHARGEATTACKING = 4;
const i32 STATEJUMPATTACK = 5;
const i32 EXHAUSTED = 6;
const i32 LANDED = 7;
const i32 STATEHIT = 8;
const i32 STATEDASH = 9;
const i32 TALKING = 10;
const i32 JUMP = 11;
const i32 DEAD = 12;

Image background;

#include "background.cpp"

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

    f32 max_stamina;
    f32 current_stamina;

    Vector2 position;
    Vector2 velocity;

    Vector2 size;
    Vector2 anchor;

    i32 facing;

    i32 sprite_index;
    f32 state_time;
    f32 animation_time;

    Weapon weapon;

    texture wall_type;
    Enemy enemy;

    i32 state;
    i32 state_prev;

    Image *image;
    Image *portrait;

    String *dialogue;
    f32 dialogue_time;
};


bool enemy_overlap(Entity *entity);
Rectangle2 get_entity_rect(Entity *entity);

f32 invuln_time = 0;

const i32 CAMERAFOLLOW = 0;
const i32 CAMERALOCKED = 1;

Vector2 camera_pos;
Vector2 camera_pos_target;
i32 camera_state = 0;

void player_hit(i32 damage, Game_Input *input);

#include "dialogue.cpp"
#include "entity.cpp"



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
        camera_pos = player.position;
    } else if (camera_state == CAMERALOCKED) {
        camera_pos = lerp_v2(camera_pos, camera_pos_target, 0.08);
    } else {
        camera_pos = player.position;
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
            damage = 1.25*(weapon.base_damage + (weapon.damage_multiplier*dmg_attr));
        } break;
    case 2: 
        {
            damage = 1.5*(weapon.base_damage + (weapon.damage_multiplier*dmg_attr));
        } break;
    }  


    for (int i = 0; i < enemy_count; i++) {
        if (!enemys[i].invuln)
        {
            if (facing > 0) {
                if (r2_intersects(r2_bounds(v2(pos.x+weapon.hit_offset_right.x, pos.y-weapon.hit_offset_right.y), weapon.hit_size, v2_zero, v2_one),
                    get_entity_rect(&enemys[i]))) {
                    if (enemys[i].current_health <= damage) {
                        enemys[i].current_health = 0;
                        enemys[i].alive = false;
                    } else {
                        enemys[i].current_health-=damage;
                        enemys[i].invuln = true;
                    } 
                }
            } else {
                if (r2_intersects(r2_bounds(v2(pos.x-weapon.hit_offset_left.x, pos.y-weapon.hit_offset_left.y), weapon.hit_size, v2_zero, v2_one),
                    get_entity_rect(&enemys[i]))) {
                    if (enemys[i].current_health <= damage) {
                        enemys[i].current_health = 0;
                        enemys[i].alive = false;
                    } else {
                        enemys[i].current_health-=damage;
                        enemys[i].invuln = true;
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

void GameStart(Game_Input *input, Game_Output *out)
{
    load_weapon();

    player.constitution = 10;
    player.rigour = 10;
    player.strength = 10;
    player.dexterity = 10;
    player.mental = 10;

    player.position = v2(624, out->height - 244);
    player.anchor = v2(0.5, 0.5);
    player.facing = -1;
    player.max_health = 10*player.constitution;
    player.current_health = player.max_health;
    player.min_health = 0;
    player.max_stamina = 10*player.rigour;
    player.current_stamina = player.max_stamina;
    player.invuln = false;
    player.alive = true;
    player.weapon = cleaver;
    player.weapon.position = player.position;
    player.size = v2(40, 52);

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

    static Image image = LoadImage(S("snowy_hills.png"));

    background = image;

    camera_pos = player.position;

    charge_meter = img;

    swing_weapon = false;
    charging_weapon = false;
    charged_attack = false;

    make_world(720);
    make_world(0);

    for (int i = 0; i < 800; i++) {
        spawn_snowflakes(out, input);
        update_snowflakes(out, player.position.x);
    }
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
   i32 new_pos = -camera_pos.x*.02;

   Clamp(new_pos, 0, 280);

   DrawImage(background, v2(new_pos, 0));
}

void GameRender(Game_Input *input, Game_Output *out)
{

}

void GameUpdateAndRender(Game_Input *input, Game_Output *out)
{
    static b32 initted = false;
    if (!initted)
    {
        GameStart(input, out);
        initted = true;
    }

    if (player.alive) {
        DrawClear(v4(0.2, 0.2, 0.2, 1));

        int layer = 0;

        if (player.position.y < 0) {
            layer = 720;
        }

        draw_background();

        GameUpdate(input, out);
        GameRender(input, out);

        spawn_snowflakes(out, input);
        update_snowflakes(out, camera_pos.x);

        for (int i = 0; i < bgnd_count; i++) {
            DrawImage(bgnd[i].image, v2(bgnd[i].position.x-camera_pos.x+out->width*.5, bgnd[i].position.y+layer));
        }

        for (int i = 0; i < wall_count; i++) {
            DrawImage(wall[i].wall_type.image, v2(wall[i].position.x-camera_pos.x+out->width*.5, wall[i].position.y+layer));
        }

        if (abs_i32(fire.position.x - player.position.x) < 1200) {
            draw_fire(input->dt);
        }
        

        player_action(input);
        player_move(input);

        set_camera_pos();

        for (int i = 0; i < enemy_count; i++) {
            if (enemys[i].enemy.type == 1 && enemys[i].alive) seal_action(&enemys[i], input, &player);
        }

        f32 dt = input->dt;

        Image icon = LoadImage(S("icon.png"));
        Image weapon_icon = LoadImage(S("Excalibrrr_icon.png"));

        DrawImage(icon, v2(16, 16));
        DrawImage(player.weapon.icon, v2(25, 25));

        DrawRect(r2_bounds(v2(72, 24), v2(16+player.max_health, 8), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(72, 24), v2(16+player.current_health, 8), v2_zero, v2_one), v4_red);

        DrawRect(r2_bounds(v2(71, 36), v2(16+player.max_stamina, 8), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(71, 36), v2(16+player.current_stamina, 8), v2_zero, v2_one), v4_green);

        for (int i = 0; i < enemy_count; i++) {
            if (enemys[i].alive) {
                if (enemys[i].enemy.type == 1) {
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5-2, enemys[i].position.y-2-12-layer), v2(enemys[i].size.x+4+enemys[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5, enemys[i].position.y-12-layer), v2(enemys[i].current_health/enemys[i].max_health*(enemys[i].size.x+4+enemys[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                    if (enemys[i].facing > 0) 
                    {
                        DrawImageMirrored(enemys[i].enemy.image[0], v2(enemys[i].position.x-camera_pos.x+out->width*.5 + enemys[i].enemy.offset.x,
                           enemys[i].position.y+enemys[i].enemy.offset.y+layer), true, false);
                    } else 
                    {
                        DrawImage(enemys[i].enemy.image[0], v2(enemys[i].position.x-camera_pos.x+out->width*.5 + enemys[i].enemy.offset.x,
                           enemys[i].position.y+enemys[i].enemy.offset.y+layer));
                    }
                } else if (enemys[i].enemy.type == 4 && abs_f32(player.position.x - enemys[i].position.x) < 800) {
                    p_soldier_action(&enemys[i], input->dt, &player, invuln_time, input);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5-2, enemys[i].position.y-2-12), v2(enemys[i].size.x+4-enemys[i].enemy.offset.x, 12),
                     v2_zero, v2_one), v4_black);
                    DrawRect(r2_bounds(v2(enemys[i].position.x-camera_pos.x+out->width*.5, enemys[i].position.y-12), v2(enemys[i].current_health/enemys[i].max_health*(enemys[i].size.x+4-enemys[i].enemy.offset.x),
                     8), v2_zero, v2_one), v4_red);
                } else if (enemys[i].enemy.type == 3 && player.position.x > 8640 && player.position.x < 9504) {
                    camera_state = CAMERALOCKED;
                    camera_pos_target = v2(9072, out->height);

                    penguin_king_action(&enemys[i], &player, input->dt, out);

                }

            }

            for (int i = 0; i < npc_count; i++) {
                npc[i].animation_time+=input->dt;
                npc[i].state_time+=input->dt;
                npc[i].dialogue_time+=input->dt;

                if (i32(npc[i].animation_time*30)%90 < 45) {
                    DrawImage(npc[i].image[0], v2(npc[i].position.x-camera_pos.x+out->width*.5, npc[i].position.y-layer));
                } else {
                    DrawImage(npc[i].image[1], v2(npc[i].position.x-camera_pos.x+out->width*.5, npc[i].position.y-layer));
                }



                if (abs_i32(npc[i].position.x-player.position.x) < 100) {
                    if (npc[i].state_time*30.0 < 400) {
                        draw_dialogue_box(npc[i].dialogue[0], out, npc[i].portrait, 2, npc[i].dialogue_time*30.0);
                    }

                    if (npc[i].state_time*30.0 > 400 && npc[i].state_time*30.0 < 402 ) {
                        npc[i].dialogue_time = 0;
                    }


                    if (npc[i].state_time*30.0 > 400 && npc[i].state_time*30.0 < 800) {
                        draw_dialogue_box(npc[i].dialogue[1], out, npc[i].portrait, 2, npc[i].dialogue_time*30.0);
                    }
                    
                } else {
                    npc[i].dialogue_time = 0;
                    npc[i].state_time = 0;
                }
            }

            static b32 draw_bounding_boxes = false;
            Controller CC = input->controllers[0];


            if (CC.pause) draw_bounding_boxes = true;

            if (draw_bounding_boxes) {
                for (int i = 0; i < enemy_count; i++) {

                    draw_bound_box(&enemys[i]);
                }
                draw_bound_box(&player);

            }

            particle_update(input->dt);


        }
    } else {
        String font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$ €£¥¤+-*/÷=%‰\"'#@&_(),.;:¿?¡!\\|{}<>[]§¶µ`^~©®™");
        Font font_hellomyoldfriend = LoadFont(S("spr_font_hellomyoldfriend_12x12_by_lotovik_strip110.png"), font_chars, v2i(12, 12));

        DrawClear(v4(0.2, 0.2, 0.2, 1));

        DrawTextExt(font_hellomyoldfriend, S("You Died"), v2(out->width/2 - 144, out->height/2-18), v4_red, v2_zero, 3.0);

        /*Image you_died = LoadImage(S("you_died.png"));

        DrawImage(you_died, v2(out->width/2-196, out->height/2-48));*/
    }

}