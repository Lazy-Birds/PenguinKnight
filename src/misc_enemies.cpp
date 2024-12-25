void seal_action(Entity *seal, Game_Input *input, Entity *player) {
    if (seal->state == NEUTRAL) {
        if (seal->alive) {
            seal->state = MOVE;
        }
    }

    seal->enemy.enemy_time++;

    if (seal->enemy.sleep_time > 0) {
        seal->enemy.sleep_time-=input->dt;
        move_enemy(seal, input->dt);
        return;
    }

    if (wall_ahead(seal)) {
        if (seal->facing > 0) {
            seal->facing = -1;
        } else {
            seal->facing = 1;
        }
    }

    switch (seal->state) 
    {
    case NEUTRAL:
        {
        } break;
    case MOVE:
        {
            if (seal->facing > 0) {
                seal->velocity.x = 200;
            } else {
                seal->velocity.x = -200;
            }

            seal->velocity.y += 496*input->dt;

            
        } break;
    case HIT:
        {
            if (seal->position.x > player->position.x)
            {
                seal->velocity.x = 8000*input->dt;
                seal->velocity.y -= 6000*input->dt;
            } else
            {
                seal->velocity.x = -8000*input->dt;
                seal->velocity.y -= 6000*input->dt;
            }
            seal->state = MOVE;

            seal->enemy.sleep_time = 25*input->dt;

        } break;
    case DYING:
        {
            Particle_Parameters min = {};
            Particle_Parameters max = {};

            min.velocity.x = 400*input->dt;
            min.velocity.y = 400*input->dt;

            max.velocity.x = 400*input->dt;
            max.velocity.y = 400*input->dt;

            min.position.x = seal->position.x;
            min.position.y = seal->position.y;

            max.position.x = seal->position.x + seal->size.x;
            max.position.y = seal->position.y + seal->size.y;

            min.life_time = 120*input->dt;
            max.life_time = 90*input->dt;

            min.magnet = player;

            Image image = LoadImage(S("exp.png"));

            for (int i = 0; i < i32(seal->enemy.exp_dropped/10); i++) {
                particle_emit(min, max, image);
            }

            seal->state = DEAD;
        } break;
    case DEAD:
        {
            seal->alive = false;
        } break;
    }

    if (invuln_time <= 0){
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(seal->position, seal->size, v2_zero, v2_one)))
        {
            player_hit(seal, input);
        }
    }

    move_enemy(seal, input->dt);
}

void eye_monster_action(Entity *monster, Game_Input *input) {
    monster->state_time+=input->dt;

    if (monster->enemy.sleep_time > 0) {
        move_enemy(monster, input->dt);
        monster->enemy.sleep_time-=input->dt;
        if (monster->state_time*60 <= 20) {
            draw_enemy(monster, 0);
            if (monster->state_time*60 >= 10) monster->velocity.x = 0;
        } else {
            draw_enemy(monster, 1);
        }
        
        if (monster->enemy.sleep_time*60 <= 1) monster->state_time = 0;

        return;
    }

    if (player.position.x < monster->position.x) {
        monster->facing = -1;
        monster->enemy.anchor_pos = v2(monster->position.x+15, monster->position.y+18);
    } else {
        monster->facing = 1;
        monster->enemy.anchor_pos = v2(monster->position.x+21, monster->position.y+18);
    }

    if (monster->state_prev != monster->state) {
        monster->state_prev = monster->state;
        monster->state_time = 0;
    }

    switch (monster->state)
    {
    case NEUTRAL:
        {
            if (entity_get_distance_x(monster, &player) < 800)
            {
                monster->state = ATTACK;
                draw_enemy(monster, 0);
            } else 
            {
                if (monster->state_time*60 < 30) 
                {
                    draw_enemy(monster, 0);
                } else if (monster->state_time*60 < 60) 
                {
                    draw_enemy(monster, 1);
                } else
                {
                    draw_enemy(monster, 0);
                    monster->state_time = 0;
                }
            }
        } break;
    case ATTACK:
        {
            if (monster->state_time*60 < 30)
            {
                draw_enemy(monster, 0);
            } else if (monster->state_time*60 < 60)
            {
                draw_enemy(monster, 1);
            } else if (monster->state_time*60 < 90)
            {
                draw_enemy(monster, 2);
                monster->projectile_launched = true;
            } else if(monster->projectile_launched)
            {
                draw_enemy(monster, 2);
                monster->sprite_index++;
                lazer_attack(monster);
            } else
            {
                draw_enemy(monster, 0);
                monster->state = NEUTRAL;
            }
        } break;
        case HIT:
        {
            if (monster->state_time*60 <= 6) {
                draw_enemy(monster, 0);
                if (monster->position.x > player.position.x)
                {
                    monster->velocity.x = 8000*input->dt;
                    monster->velocity.y -= 6000*input->dt;
                } else
                {
                    monster->velocity.x = -8000*input->dt;
                    monster->velocity.y -= 6000*input->dt;
                }
            }

            monster->enemy.sleep_time=20*input->dt;

            monster->state = ATTACK;
        } break;
    case DYING:
        {
            Particle_Parameters min = {};
            Particle_Parameters max = {};

            min.velocity.x = 400*input->dt;
            min.velocity.y = 400*input->dt;

            max.velocity.x = 400*input->dt;
            max.velocity.y = 400*input->dt;

            min.position.x = monster->position.x;
            min.position.y = monster->position.y;

            max.position.x = monster->position.x + monster->size.x;
            max.position.y = monster->position.y+monster->size.y;

            min.life_time = 120*input->dt;
            max.life_time = 90*input->dt;

            min.magnet = &player;

            Image image = LoadImage(S("exp.png"));

            for (int i = 0; i < i32(monster->enemy.exp_dropped/10); i++) {
                particle_emit(min, max, image);
            }

            monster->state = DEAD;

            /*if (i32(monster->state_time*60) < 2) {
                draw_enemy(monster, 4);
            } else if (i32(monster->state_time*60) < 4) {
                draw_enemy(monster, 5);
            } else if (i32(monster->state_time*60) < 6) {
                draw_enemy(monster, 6);
            } else {
                monster->alive = false;
            }*/
        } break;
    case DEAD:
        {
            monster->alive = false;
        } break;
    }
}

void lazer_attack(Entity *monster) {

    Vector2 pos = v2(monster->enemy.anchor_pos.x + (get_entity_direction(monster)*monster->sprite_index*monster->projectile[0].size.width), monster->enemy.anchor_pos.y);
    Vector2 size = v2(monster->sprite_index*monster->projectile[0].size.width, monster->projectile[0].size.height);

    Rectangle2 rec = r2_bounds(pos, size, v2_zero, v2_one);
    Rectangle2 rec_out = r2_shift(rec, v2(-camera_pos.x+out->width*.5, 0));

    DrawRectOutline(rec_out, v4_red, 2);

    for (int i = 0; i < monster->sprite_index; i++)
    {
        i32 width = monster->projectile[0].size.width;
        Vector2 pos = v2(monster->enemy.anchor_pos.x+(get_entity_direction(monster)*width*i)-camera_pos.x+out->width*.5, monster->enemy.anchor_pos.y);
        DrawImage(monster->projectile[monster->sprite_index%2], pos);
    }

    if (r2_intersects(get_entity_rect(&player), rec))
    {
        if (invuln_time <= 0)
        {
            player_hit(monster, input);
        }
        
        monster->projectile_launched = false;
        monster->sprite_index = 0;
    } else if (wall_intersects_rec(rec))
    {
        monster->projectile_launched = false;
        monster->sprite_index = 0;
    }
}