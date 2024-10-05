struct texture {
    u32 pixel;
    Image image;
};

#include "weapon.cpp"

struct Entity
{
    u64 id;
    u64 type;

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
    Image *player_spr_left;
    Image *player_spr_right;

    i32 sprite_index;
    f32 animation_time;

    Weapon weapon;

    texture wall_type;
};

#include "entity.cpp"

Entity player = {};

Entity enemy_plant = {};

Image spr_guy;

bool swing_weapon;

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

bool wall_ahead(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x+1, entity_one->position.y), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
        }
    }

    return false;
}

bool wall_behind(Entity *entity_one) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity_one->position.x-1, entity_one->position.y), entity_one->size, v2_zero, v2_one),
            r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
        } 
    }

    return false;
}

bool wall_intersects(Entity *entity) {
    for (int i = 0; i < wall_count; i++) {
        if (r2_intersects(r2_bounds(v2(entity->position.x, entity->position.y), entity->size, v2_zero, v2_one),
        r2_bounds(v2(wall[i].position.x, wall[i].position.y), wall[i].size, v2_zero, v2_one))) {
            return true;
        }
    }
    return false;
}

void load_enemy() {
    enemy_plant.wall_type.image = LoadImage(S("plant_idle1.png"));
    enemy_plant.max_health = 5;
    enemy_plant.current_health = 5;
    enemy_plant.min_health = 0;
    enemy_plant.position = v2(528, 480);
    enemy_plant.size = v2(48, 48);
    enemy_plant.invuln = false;
    enemy_plant.alive = true;
}

void GameStart(Game_Input *input, Game_Output *out)
{
    player.position = v2(out->width*.5, out->height*.5);
    player.size = v2(48, 48);
    player.anchor = v2(0.5, 0.5);
    player.facing = -1;
    player.max_health = 5;
    player.current_health = 4;
    player.min_health = 0;
    player.max_stamina = 100;
    player.current_stamina = player.max_stamina;
    player.invuln = false;
    player.alive = true;
    static Image penguin_knight_left[] = 
    {
        LoadImage(S("penguin_walking_left1.png")),
        LoadImage(S("penguin_walking_left2.png")),
        LoadImage(S("penguin_walking_left3.png")),
        LoadImage(S("penguin_walking_left4.png")),
    };
    static Image penguin_knight_right[] = 
    {
        LoadImage(S("penguin_walking_right1.png")),
        LoadImage(S("penguin_walking_right2.png")),
        LoadImage(S("penguin_walking_right3.png")),
        LoadImage(S("penguin_walking_right4.png")),
    };
    player.player_spr_left = penguin_knight_left;
    player.player_spr_right = penguin_knight_right;
    static Image excalibrrr[] = 
    {
        LoadImage(S("Excalibrrr_right1.png")),
        LoadImage(S("Excalibrrr_right2.png")),
        LoadImage(S("Excalibrrr_right3.png")),
        LoadImage(S("Excalibrrr_right4.png")),
        LoadImage(S("Excalibrrr_right5.png")),
        LoadImage(S("Excalibrrr_left1.png")),
        LoadImage(S("Excalibrrr_left2.png")),
        LoadImage(S("Excalibrrr_left3.png")),
        LoadImage(S("Excalibrrr_left4.png")),
        LoadImage(S("Excalibrrr_left5.png")),
    };
    player.weapon.image = excalibrrr;
    player.weapon.name = S("Excalibrrr");
    player.weapon.position = player.position;
    player.weapon.size = v2(96, 48);
    player.weapon.weapon_swing = 0;

    swing_weapon = false;

    make_wall();

    load_enemy();
}

void GameUpdate(Game_Input *input, Game_Output *out)
{   
    f32 max_speed = 600.0;
    Controller c0 = input->controllers[0];
    f32 dt = input->dt;

    if (c0.right && !swing_weapon)
    {
        if (c0.b) {
            player.velocity.x = move_f32(player.velocity.x, max_speed, max_speed * dt);
        } else {
            player.velocity.x = move_f32(player.velocity.x, 300, 300 * dt);
        }
        
        if (player.facing > 0) {
            player.facing = -1;
        } else {
            player.facing--;
        }
    }
    else if (c0.left && !swing_weapon)
    {
        if (c0.b) {
            player.velocity.x = move_f32(player.velocity.x, -max_speed, max_speed * dt);
        } else {
            player.velocity.x = move_f32(player.velocity.x, -300, 300 * dt);
        }
        if (player.facing < 0) {
            player.facing = 1;
        } else {
            player.facing++;
        }
    }
    else
    {
        player.facing = sign_i32(player.facing);
        player.velocity.x = move_f32(player.velocity.x, 0, 1200 * dt);
    }

    if (c0.a && player.current_stamina > 20) {
        if (player.position.y==out->height-player.size.y || entity_on_wall(&player)) {
            player.velocity.y=-248;
            player.current_stamina-=20;
        }
    }

    if ((input->mouse.left) && player.current_stamina > 20 && !swing_weapon) {
        swing_weapon = true;
        player.current_stamina-=20;
    }

    if (player.current_stamina < player.max_stamina) {
        player.current_stamina+=20*dt;
    }

    player.velocity.y+=496*input->dt;

    f32 dy = player.velocity.y*input->dt;
    f32 dx = player.velocity.x*input->dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        player.position.y+=sign_f32(dy);
        if (wall_intersects(&player)) {
            player.position.y-=sign_f32(dy);
            player.velocity.y=0;
        }
    }

    for (int i = 0; i < abs_f32(dx); i++) {
        player.position.x+=sign_f32(dx);
        if (wall_intersects(&player)) {
            player.position.x-=sign_f32(dx);
            player.velocity.x=0;
        }
    }
}

void GameRender(Game_Input *input, Game_Output *out)
{

}

void GameUpdateAndRender(Game_Input *input, Game_Output *out)
{
    DrawClear(v4(0.2, 0.2, 0.2, 1));

    static b32 initted = false;
    if (!initted)
    {
        GameStart(input, out);
        initted = true;
    }

    GameUpdate(input, out);
    GameRender(input, out);

    

    DrawRect(r2_bounds(v2(enemy_plant.position.x-player.position.x+out->width*.5, enemy_plant.position.y), v2(8*enemy_plant.max_health, 8), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(enemy_plant.position.x-player.position.x+out->width*.5, enemy_plant.position.y), v2(8*enemy_plant.current_health, 8), v2_zero, v2_one), v4_red);

    DrawImage(enemy_plant.wall_type.image, v2(enemy_plant.position.x-player.position.x+out->width*.5, enemy_plant.position.y));

    Image icon = LoadImage(S("icon.png"));
    Image weapon_icon = LoadImage(S("Excalibrrr_icon.png"));

    DrawImage(icon, v2(16, 16));
    DrawImage(weapon_icon, v2(21, 18));

    if (player.alive)
    {
        if (r2_intersects(r2_bounds(player.weapon.position, player.weapon.size, v2_zero, v2_one), get_entity_rect(&enemy_plant))
                && enemy_plant.current_health > enemy_plant.min_health && !enemy_plant.invuln && player.facing < 0)
            {
                enemy_plant.current_health = enemy_plant.current_health-1;
                enemy_plant.invuln = true;
            } else if (r2_intersects(r2_bounds(player.weapon.position, v2(player.weapon.size.x, player.weapon.size.y), v2_zero, v2_one), get_entity_rect(&enemy_plant))
                && enemy_plant.current_health > enemy_plant.min_health && !enemy_plant.invuln && player.facing > 0) {

            }
        if (!swing_weapon && player.facing < 0) {
            DrawImage(player.weapon.image[0], v2(player.position.x-player.position.x+out->width*.5-15, player.position.y-18));
        } else if (!swing_weapon && player.facing > 0) {
            DrawImage(player.weapon.image[5], v2(player.position.x-player.position.x+out->width*.5-30, player.position.y-18));
        }
        if (swing_weapon) 
        {
            if (player.weapon.weapon_swing%18/3 == 5)
            {
                swing_weapon = false;
                player.weapon.weapon_swing = 0;
                enemy_plant.invuln = false;
            } 
            else if (player.facing < 0)
            {
                player.weapon.position = player.position;
                DrawImage(player.weapon.image[player.weapon.weapon_swing%18/3], v2(player.position.x-player.position.x+out->width*.5-15, player.position.y-18));
            } else {
                player.weapon.position = player.position;
                DrawImage(player.weapon.image[player.weapon.weapon_swing%18/3+5], v2(player.position.x-player.position.x+out->width*.5-30, player.position.y-18));
            }
            
            player.weapon.weapon_swing++;
        }
        if (player.facing > 0)
        {
            DrawImage(player.player_spr_left[player.facing%32/8], v2(player.position.x-player.position.x+out->width*.5, player.position.y));
        } 
        else
        {
            DrawImage(player.player_spr_right[abs_i32(player.facing%32/8)], v2(player.position.x-player.position.x+out->width*.5, player.position.y));
        }
    }

    DrawRect(r2_bounds(v2(72, 24), v2(16+8*player.max_health, 8), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(72, 24), v2(16+8*player.current_health, 8), v2_zero, v2_one), v4_red);

    DrawRect(r2_bounds(v2(71, 36), v2(16+player.max_stamina, 8), v2_zero, v2_one), v4_black);
    DrawRect(r2_bounds(v2(71, 36), v2(16+player.current_stamina, 8), v2_zero, v2_one), v4_green);
    


    for (int i = 0; i < wall_count; i++) {
        DrawImage(wall[i].wall_type.image, v2(wall[i].position.x-player.position.x+out->width*.5, wall[i].position.y));
    }
}