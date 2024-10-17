#include "background.cpp"

struct texture {
    u32 pixel;
    Image image;
};

struct time {
    int seconds;
    int partial_seconds;
};

#include "weapon.cpp"

struct Enemy {
    Image *image;
    Vector2 frames;
    i32 damage;
    i32 type;
    Vector2 offset;
};

struct Entity
{
    i32 constitution;
    i32 rigour;
    i32 strength;
    i32 dexterity;
    i32 mental;

    i32 max_health;
    i32 current_health;
    i32 min_health;

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
    f32 animation_time;

    Weapon weapon;

    texture wall_type;
    Enemy enemy;
};

#include "entity.cpp"

time game_time = {0, 0};

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

Rectangle2 get_entity_rect(Entity *entity) {
    return r2_bounds(entity->position, entity->size, v2_zero, v2_one);
}

bool entity_on_wall(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+1), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
    } 
}

return false;
}

bool wall_intersects(Entity *entity) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(entity->position, entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
        }
    }
    return false;
}

bool wall_ahead(Entity *entity, i32 dir) {
    for (int i = 0; i < wall_count; i++) {
        if (dir > 0)
        {
            if (r2_intersects(r2_bounds(v2(entity->position.x+1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one)))
            {
                return true;
            }
        } else if (r2_intersects(r2_bounds(v2(entity->position.x-1, entity->position.y-1), entity->size, v2_zero, v2_one), r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one)))
        {
            return true;
        }
    }

    return false;
}

bool entity_in_air(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x, entity_one->position.y+2), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return false;
    }
}
return true;
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
                    if (enemys[i].current_health < damage) {
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
                    if (enemys[i].current_health < damage) {
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
    DrawImage(charge_meter[frame], v2(player.position.x-player.position.x+out->width*.5-16, player.position.y-16));
}

void GameStart(Game_Input *input, Game_Output *out)
{
    load_weapon();

    player.constitution = 10;
    player.rigour = 10;
    player.strength = 10;
    player.dexterity = 10;
    player.mental = 10;

    player.position = v2(out->width*.5, out->height*.5);
    player.anchor = v2(0.5, 0.5);
    player.facing = 1;
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

    charge_meter = img;

    swing_weapon = false;
    charging_weapon = false;
    charged_attack = false;

    make_world();

    for (int i = 0; i < 800; i++) {
        spawn_snowflakes(out, input);
        update_snowflakes(out, player.position.x);
    }
}

void increment_time() {
    game_time.partial_seconds++;
    if (game_time.partial_seconds == 60) {
        game_time.seconds++;
        game_time.partial_seconds = 0;
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

void frame_updates() {
    if (walk_frame == 29) {
        walk_frame = 0;
    } else {
        walk_frame++;
    }
    charging++;

    if (on_cooldown) {
        weapon_cooldown++;
        charging = 0;
    }

    attack_frame++;
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
        dump(S("We have a problem"));
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
                dump(S("Right Click!"));
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

void seal_walk() {
    for (int i = 0; i < enemy_count; i++) {
        if (!enemys[i].alive) {continue;}
        if (enemys[i].enemy.type == 1) {
            if (wall_ahead(&enemys[i], enemys[i].facing))
            {
                if (enemys[i].facing < 0) {
                    enemys[i].facing = 1;
                    enemys[i].velocity.x = 0;
                } else {
                    enemys[i].facing = -1;
                    enemys[i].velocity.x = 0;
                }
            }

            enemys[i].velocity.y+=496*input->dt;
            switch (enemys[i].facing)
            {
            case -1: 
                {
                    enemys[i].velocity.x-=50*input->dt;
                } break;
            case 1:
                {
                    enemys[i].velocity.x+=50*input->dt;
                } break;
            }

            f32 dy = enemys[i].velocity.y*input->dt;
            f32 dx = enemys[i].velocity.x*input->dt;

            for (int i = 0; i < abs_f32(dy); i++) {
                enemys[i].position.y+=sign_f32(dy);
                if (wall_intersects(&enemys[i])) {
                    enemys[i].position.y-=sign_f32(dy);
                    enemys[i].velocity.y=0;
                }
            }


            for (int i = 0; i < abs_f32(dx); i++) {
                enemys[i].position.x+=sign_f32(dx);
                if (wall_intersects(&enemys[i])) {
                    enemys[i].position.x-=sign_f32(dx);
                    enemys[i].velocity.x=0;
                }
            }

            if (invuln_time <= 0){
                if (r2_intersects(r2_bounds(player.position, player.size, v2_zero, v2_one), r2_bounds(enemys[i].position, enemys[i].size, v2_zero, v2_one)))
                {
                    player_hit();
                }
            }
        }
    }
}

void GameRender(Game_Input *input, Game_Output *out)
{

}

void GameUpdateAndRender(Game_Input *input, Game_Output *out)
{
    increment_time();

    frame_updates();

    DrawClear(v4(0.2, 0.2, 0.2, 1));

    static b32 initted = false;
    if (!initted)
    {
        GameStart(input, out);
        initted = true;
    }

    GameUpdate(input, out);
    GameRender(input, out);

    spawn_snowflakes(out, input);
    update_snowflakes(out, player.position.x);

    for (int i = 0; i < plant_count; i++) {
        DrawImage(plants[i].image, v2(plants[i].position.x-player.position.x+out->width*.5, plants[i].position.y));
    }

    player_action(input);
    player_move(input);

    seal_walk();

    f32 dt = input->dt;

    Image icon = LoadImage(S("icon.png"));
    Image weapon_icon = LoadImage(S("Excalibrrr_icon.png"));

    DrawImage(icon, v2(16, 16));
    DrawImage(weapon_icon, v2(21, 18));

    /*if (player.alive)
    {
        else if (charging_weapon && !charged_attack) {
            dump(S("Drawing Charge!"));

            draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), player.weapon.weapon_frames.x, player.facing);
            draw_charging(get_frame_charged(dt, player.weapon.charge_time_multiplier));

            charging_weapon = false;
            
            player.velocity.x = move_f32(player.velocity.x, 0, 600 * dt);
            

        } else if (charged_attack){
            if (get_frame_charged(dt, player.weapon.charge_time_multiplier) < 12) {
                draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), get_frame_charged(dt, player.weapon.charge_time_multiplier), player.facing);
            } else {
                charged_attack = false;
                weapon_cooldown = true;
            }
        }  else 
        {
            draw_player(player.weapon, v2(player.position.x-player.position.x+out->width*.5, player.position.y), 0, player.facing);
        }
        
    }*/

    DrawRect(r2_bounds(v2(72, 24), v2(16+player.max_health, 8), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(72, 24), v2(16+player.current_health, 8), v2_zero, v2_one), v4_red);

    DrawRect(r2_bounds(v2(71, 36), v2(16+player.max_stamina, 8), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(71, 36), v2(16+player.current_stamina, 8), v2_zero, v2_one), v4_green);

    for (int i = 0; i < plant_count; i++) {
        DrawImage(plants[i].image, v2(plants[i].position.x-player.position.x+out->width*.5, plants[i].position.y));
    }
    
    for (int i = 0; i < enemy_count; i++) {
        if (enemys[i].alive) {
            DrawRect(r2_bounds(v2(enemys[i].position.x-player.position.x+out->width*.5, enemys[i].position.y), v2(enemys[i].max_health, 8), v2_zero, v2_one), v4_black);
            DrawRect(r2_bounds(v2(enemys[i].position.x-player.position.x+out->width*.5, enemys[i].position.y), v2(enemys[i].current_health, 8), v2_zero, v2_one), v4_red);
            if (enemys[i].facing > 0) 
            {
                DrawImageMirroredX(enemys[i].enemy.image[0], v2(enemys[i].position.x-player.position.x+out->width*.5 + enemys[i].enemy.offset.x,
                   enemys[i].position.y+enemys[i].enemy.offset.y));
            } else 
            {
                DrawImage(enemys[i].enemy.image[0], v2(enemys[i].position.x-player.position.x+out->width*.5 + enemys[i].enemy.offset.x,
                   enemys[i].position.y+enemys[i].enemy.offset.y));
            }
            
        }
    }


    for (int i = 0; i < wall_count; i++) {
        DrawImage(wall[i].wall_type.image, v2(wall[i].position.x-player.position.x+out->width*.5, wall[i].position.y));
    }
}