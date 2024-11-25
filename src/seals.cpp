void seal_move(Entity *seal, Game_Input *input);

void seal_action(Entity *seal, Game_Input *input, Entity *player) {
    if (seal->state == NEUTRAL) {
        if (seal->alive) {
            seal->state = MOVE;
        }
    }

    seal->enemy.enemy_time++;

    if (seal->enemy.sleep_time > 0) {
        seal->enemy.sleep_time-=input->dt;
        seal_move(seal, input);
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

            min.life_time = 90*input->dt;
            max.life_time = 60*input->dt;

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

    seal_move(seal, input);
}

void seal_move(Entity *seal, Game_Input *input) {
    seal->velocity.y+=496*input->dt;

    f32 dy = seal->velocity.y*input->dt;
    f32 dx = seal->velocity.x*input->dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        seal->position.y+=sign_f32(dy);
        if (wall_intersects(seal)) {
            seal->position.y-=sign_f32(dy);
            seal->velocity.y=0;
        }
    }


    for (int i = 0; i < abs_f32(dx); i++) {
        seal->position.x+=sign_f32(dx);
        if (wall_intersects(seal)) {
            seal->position.x-=sign_f32(dx);
            seal->velocity.x=0;
        }
    }
} 