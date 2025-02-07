#include "pengu_def.h"

//General States
enum general_states {
    NEUTRAL,
    MOVE,
    ATTACK,
    ATTACKTWO,
    ATTACKTHREE,
    CHARGING,
    STATECHARGEATTACKING,
    STATECHARGEATTACK,
    STATEJUMPATTACK,
    EXHAUSTED,
    LANDED,
    HIT,
    STATEDODGEDASH,
    STATEDODGE,
    STATEDASH,
    TALKING,
    JUMP,
    GUARD,
    SHOOT,
    HOOKSHOTTING,
    DYING,
    DEAD,
};

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
EntityArrayP temp_arr = {};
CampfireArray camps = {};
i32 last_count = 0;

bool swing_weapon;
bool charging_weapon;
bool on_cooldown;
bool charged_attack;
bool in_menu = false;
bool at_bonfire = false;
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
i32 fps = 60;

Arena *inv_arena = NULL;

#include "levels.cpp"
#include "weapon.cpp"
#include "background.cpp"
#include "dialogue.cpp"
#include "entity.cpp"
#include "menu.cpp"
#include "character.cpp"
#include "player_actions.cpp"

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

/*bool enemy_overlap(Entity *entity) {
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
}*/

void draw_bound_box(Entity *entity) {
    Rectangle2 rec = get_entity_rect(entity);
    rec = r2_shift(rec, v2(-camera_pos.x+out->width*.5, 0));

    DrawRectOutline(rec, v4_red, 1);
}

void bosses_alive() {
    for (int i = 0; i < 48; i++) {
        bosses_killed[i] = false;
    }
}

void GameStart(Game_Input *input, Game_Output *out)
{
    load_weapon();
    static Image chargey_attack[] = {
        LoadImage(S("charged_attack1.png")),
        LoadImage(S("charged_attack2.png")),
    };

    camps = make_campfire_array(pengu_arena, 100);
    make_camps();

    player.constitution = 10;
    player.rigour = 10;
    player.strength = 10;
    player.dexterity = 10;
    player.mental = 10;
    //player.check_point = /*v2(1298, 524);*/v2(480, 524);//v2(8449, 476);v2(6000, out->height - 244);
    //player.check_point_level = 1;
    player.check_point = camps.data[cf_village].position;
    player.check_point_level = camps.data[cf_village].level;

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
    player.weapon = w_list.data[pw_staff];
    player.weapon.position = player.position;
    player.inventory = make_inventory(100, pengu_arena);
    player.size = v2(40, 52);
    player.exp_gained = 0;
    player.exp_to_level = 600;
    player.level = 1;
    player.type = et_player;
    player.player_level = player.check_point_level;
    player.acting_cd = 0;
    player.jumpies = LoadSound(S("Jump.wav"));
    player.hit = LoadSound(S("pengu_hurt.wav"));
    player.projectile = chargey_attack;
    player.entities_met = make_npc_list(100, pengu_arena);

    player.hitbox = r2_bounds(player.position, player.size, v2_zero, v2_one);

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

    gen_test_items(&player, pengu_arena);

    font_chars = S(" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,;:?!-_~#\"'&()[]|`\\/@°+=*$");
    font_hellomyoldfriend = LoadFont(S("Lou-Bagel-Caps.png"), font_chars, v2i(7, 9));
}

void reset_vars() {
    TimeFunction;
    player.position = player.check_point;
    player.player_level = player.check_point_level;
    player.current_health = player.max_health;
    player.current_stamina = player.max_stamina;
    player.current_mp = player.max_mp;
    player.alive = true;
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
        make_world();
        reset_particles();
    } else {
        destroy_boss_walls();
        reset_enemies();
        reset_particles();
        reset_projectiles();
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

    if (player.weapon.damage_attribute == at_strength) {
        return player.strength;
    } else if (player.weapon.damage_attribute == at_dexterity) {
        return player.dexterity;
    } else if (player.weapon.damage_attribute == at_mental) {
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
        temp_arr = make_entity_array_three(pengu_arena, 500);
        initted = true;
    }

    Controller CC = input->controllers[0];

    if (in_menu) {
        if (ControllerReleased(0, Button_Start)) {
            in_menu = false;
            if (at_bonfire) {
                set_world(true, World[player.player_level]);
                at_bonfire = false;
            }
            
        }

        draw_menu(out, &player, screen, at_bonfire);
    } else if (player.alive) {
        MixerOutputPlayingSounds();

        check_level();
        set_camera_pos();
        camera_offset = -camera_pos.x+out->width*.5;

        Level *level = &World[player.player_level];
        group_time+=input->dt;

        draw_background();

        GameUpdate(input, out);
        GameRender(input, out);

        if (player.player_level == 0) {
            spawn_snowflakes(out, input);
            update_snowflakes(out, camera_pos.x);
        }

        entity_processing(input, out);

        f32 dt = input->dt;

        Image icon = LoadImage(S("icon.png"));
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

        draw_player_stats();

        update_projectiles(input, &player);

        for (int i = 0; i < last_count; i++) {
            DrawImage(draw_last.data[i].image[draw_last.data[i].sprite_index], v2(draw_last.data[i].position.x+camera_offset, draw_last.data[i].position.y));
        }
        last_count = 0;


        player_action(input);
        player_move(input);
        
        
        Rectangle2 player_rec = r2_shift(player.hitbox, v2(camera_offset, 0));

        DrawRectOutline(player_rec, v4_red, 2);


        if (ControllerReleased(0, Button_Start) && !in_menu) {
            in_menu = true;
        }

        particle_update(input->dt);

        if (in_menu) {
            screen = MakeScreenImage(out);
            draw_menu(out, &player, screen, at_bonfire);
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
    }

}