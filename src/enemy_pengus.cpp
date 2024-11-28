void make_guards(Vector2 positions) {
    enemys[enemy_count] = load_enemy(v2(positions.x, 0), 3);
    enemy_count++;

    enemys[enemy_count] = load_enemy(v2(positions.y, 0), 3);
    enemy_count++;
}

const i32 KINGNEUTRAL = 0;
const i32 KINGSTOMPING = 1;
const i32 KINGJUMPING = 2;
const i32 KINGLEAPING = 3;
const i32 KINGLANDING = 4;
const i32 KINGGUARDBREAK = 5;
const i32 KINGCOMMAND = 6;
const i32 KINGWAIT = 7;
const i32 KINGCHALLENGE = 20;

const i32 PHASEONE = 0;
const i32 PHASETWO = 1;

i32 king_phase = PHASEONE;


Vector2 stage_size = v2(8640, 9504);

void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt, Game_Output *out) {


    pengu_king->state_time+=dt;
    pengu_king->dialogue_time+=dt;

    if (pengu_king->state != HIT && pengu_king->state != pengu_king->state_prev) {
        pengu_king->state_prev = pengu_king->state;
        pengu_king->state_time = 0;
    }

    if (pengu_king->state == KINGNEUTRAL){
        if (!wall_ahead(pengu_king)) {
            pengu_king->state_time = 0;
            pengu_king->state = KINGSTOMPING;
            pengu_king->state_prev = pengu_king->state;
        } else if(wall_ahead(pengu_king)) {
            pengu_king->state_time = 0;
            pengu_king->state = KINGJUMPING;
            pengu_king->state_prev = pengu_king->state;
        } else if (pengu_king->enemy.guard <=0) {
            pengu_king->state_time = 0;
            pengu_king->state = KINGGUARDBREAK;
            pengu_king->state_prev = pengu_king->state;
        }
    }

    if (wall_ahead(pengu_king) && pengu_king->state_time == 0) {
        if (pengu_king->facing < 0) {
            pengu_king->facing = 1;
        } else {
            pengu_king->facing = -1;
        }
    }

    if (king_phase == PHASEONE) {
        if (pengu_king->state != KINGCOMMAND && pengu_king->state != KINGWAIT) {
            pengu_king->state = KINGCOMMAND;
            pengu_king->state_time = 0;
            pengu_king->state_prev = pengu_king->state;
        }
    } else {
        DrawRect(r2_bounds(v2(out->width*.5-302, out->height-50), v2(604, 12), v2_zero, v2_one), v4_black);
        DrawRect(r2_bounds(v2(out->width*.5-300, out->height-48), v2((pengu_king->current_health/pengu_king->max_health)*600, 8), v2_zero, v2_one), v4_red);
    }


    switch (pengu_king->state)
    {
    case KINGNEUTRAL:
        {
            if (pengu_king->facing > 0)
            {
                DrawImageMirrored(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                 pengu_king->position.y+pengu_king->enemy.offset.y), true, false);
            } else 
            {
                DrawImage(pengu_king->enemy.image[0], v2(pengu_king->position.x-player->position.x+out->width*.5 + pengu_king->enemy.offset.x,
                    pengu_king->position.y+pengu_king->enemy.offset.y));
            }

        } break;
    case KINGSTOMPING:
        {
            if (pengu_king->state_time*60 < 5) {
                draw_enemy(pengu_king, 2);
                
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 10) {
                draw_enemy(pengu_king, 3);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 15) {
                draw_enemy(pengu_king, 4);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 20) {
                draw_enemy(pengu_king, 5);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 25) {
                draw_enemy(pengu_king, 6);
                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 30) {
                draw_enemy(pengu_king, 7);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else if (pengu_king->state_time*60 < 35) {
                draw_enemy(pengu_king, 4);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
            } else {
                draw_enemy(pengu_king, 2);

                pengu_king->velocity.x = 4000*dt*sign_f32(pengu_king->facing);
                pengu_king->state = KINGNEUTRAL;
            }
            for (int i = 0; i < 3; i++) {
                Particle_Parameters param1 = {};
                Particle_Parameters param2 = {};

                if (pengu_king->facing < 0) {
                    param1.position.x = pengu_king->position.x-5;
                    param2.position.x = pengu_king->position.x+pengu_king->size.x;

                    param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                    param2.position.y = pengu_king->position.y + pengu_king->size.y;
                } else {
                    param1.position.x = pengu_king->position.x-5;
                    param2.position.x = pengu_king->position.x+pengu_king->size.x;

                    param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                    param2.position.y = pengu_king->position.y + pengu_king->size.y;

                }

                param1.velocity.x = 300*dt*sign_f32(pengu_king->facing);
                param2.velocity.x = 600*dt*sign_f32(pengu_king->facing);

                param1.velocity.y = -600*dt;
                param2.velocity.y = -100*dt;

                param1.life_time = 60*dt;
                param2.life_time = 120*dt;

                Image image = LoadImage(S("snow_flake_chunky.png"));

                particle_emit(param1, param2, image);
            }
        } break;
    case KINGJUMPING:
        {
            if (pengu_king->facing > 0) {
                if (pengu_king->position.x < stage_size.y-65) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                        for (int i = 0; i < 80; i++) {
                            Particle_Parameters param1 = {};
                            Particle_Parameters param2 = {};

                            if (pengu_king->facing < 0) {
                                param1.position.x = pengu_king->position.x;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;
                            } else {
                                param1.position.x = pengu_king->position.x-5;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;

                            }
                            if (i%2 == 0) {
                                param1.velocity.x = 800*dt;
                                param2.velocity.x = 1600*dt;
                            } else {
                                param1.velocity.x = -800*dt;
                                param2.velocity.x = -1600*dt;
                            }
                            

                            param1.velocity.y = -1600*dt;
                            param2.velocity.y = -600*dt;

                            param1.life_time = 60*dt;
                            param2.life_time = 120*dt;
                            
                            Image image = LoadImage(S("snow_flake_chunky.png"));

                            particle_emit(param1, param2, image);
                        }

                    }
                    pengu_king->velocity.x = 6000*dt;

                    draw_enemy(pengu_king, 0);
                } else {
                    pengu_king->state = KINGLEAPING;
                    draw_enemy(pengu_king, 0);
                }
            } else {
                if (pengu_king->position.x > stage_size.x) {
                    if (entity_on_wall(pengu_king)) {
                        pengu_king->velocity.y = -15000*dt;
                        for (int i = 0; i < 80; i++) {
                            Particle_Parameters param1 = {};
                            Particle_Parameters param2 = {};

                            if (pengu_king->facing < 0) {
                                param1.position.x = pengu_king->position.x;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;
                            } else {
                                param1.position.x = pengu_king->position.x-5;
                                param2.position.x = pengu_king->position.x+pengu_king->size.x;

                                param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                                param2.position.y = pengu_king->position.y + pengu_king->size.y;

                            }
                            if (i%2 == 0) {
                                param1.velocity.x = 800*dt;
                                param2.velocity.x = 1600*dt;
                            } else {
                                param1.velocity.x = -800*dt;
                                param2.velocity.x = -1600*dt;
                            }
                            

                            param1.velocity.y = -1600*dt;
                            param2.velocity.y = -600*dt;

                            param1.life_time = 60*dt;
                            param2.life_time = 120*dt;
                            
                            Image image = LoadImage(S("snow_flake_chunky.png"));

                            particle_emit(param1, param2, image);
                        }
                    }
                    pengu_king->velocity.x = -6000*dt;

                    draw_enemy(pengu_king, 0);
                } else {
                    pengu_king->state = KINGLEAPING;
                    draw_enemy(pengu_king, 0);
                }
            }   
        } break;
    case KINGLEAPING:
        {
            Rectangle2 land_aoe = r2_bounds(v2(pengu_king->position.x-50, i32(pengu_king->position.y+pengu_king->size.y/2)), v2(pengu_king->size.x+100, i32(pengu_king->size.y/2)), v2_one, v2_zero);

            if (entity_on_wall(pengu_king) && pengu_king->state_time*60 < 10) {
                draw_enemy(pengu_king, 8);
            } else if (pengu_king->position.y > -pengu_king->size.y && pengu_king->state_time*60 < 180) {
                pengu_king->velocity.x = 0;
                draw_enemy(pengu_king, 9);
                pengu_king->velocity.y = -45000*dt;
            } else if (pengu_king->state_time*60 < 180) {
                pengu_king->velocity.y = 0;
                pengu_king->position = v2(player->position.x, -300);
                pengu_king->facing = player->facing;
            } else if (!entity_on_wall(pengu_king)) {
                if (pengu_king->position.x > stage_size.y-49) pengu_king->position = v2(stage_size.y-49, pengu_king->position.y);
                if (pengu_king->position.x < stage_size.x) pengu_king->position = v2(stage_size.x, pengu_king->position.y);
                draw_enemy(pengu_king, 8);
                pengu_king->velocity.y += 2000*dt;
            } else {
                for (int i = 0; i < 80; i++) {
                    Particle_Parameters param1 = {};
                    Particle_Parameters param2 = {};

                    if (pengu_king->facing < 0) {
                        param1.position.x = pengu_king->position.x;
                        param2.position.x = pengu_king->position.x+pengu_king->size.x;

                        param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                        param2.position.y = pengu_king->position.y + pengu_king->size.y;
                    } else {
                        param1.position.x = pengu_king->position.x-5;
                        param2.position.x = pengu_king->position.x+pengu_king->size.x;

                        param1.position.y = pengu_king->position.y + pengu_king->size.y-8;
                        param2.position.y = pengu_king->position.y + pengu_king->size.y;

                    }
                    if (i%2 == 0) {
                        param1.velocity.x = 1200*dt;
                        param2.velocity.x = 3600*dt;
                    } else {
                        param1.velocity.x = -1200*dt;
                        param2.velocity.x = -3600*dt;
                    }


                    param1.velocity.y = -2200*dt;
                    param2.velocity.y = -1000*dt;

                    param1.life_time = 80*dt;
                    param2.life_time = 160*dt;

                    Image image = LoadImage(S("snow_flake_chunky.png"));

                    particle_emit(param1, param2, image);
                }
                Rectangle2 land_aoe = r2_bounds(v2(pengu_king->position.x-50, i32(pengu_king->position.y+pengu_king->size.y/2)), v2(pengu_king->size.x+100, i32(pengu_king->size.y/2)), v2_one, v2_zero);

                if (r2_intersects(land_aoe, get_entity_rect(player))) {
                    player_hit(pengu_king, input);
                }

                

                pengu_king->state = KINGLANDING;
                draw_enemy(pengu_king, 10);
            }

            land_aoe = r2_shift(land_aoe, v2(-camera_pos.x+out->width*.5, 0));

            DrawRectOutline(land_aoe, v4_red, 1);
        } break;
    case KINGLANDING:
        {
            if (pengu_king->state_time*60 < 180) {
                if (i32(pengu_king->state_time*60)%20< 10) {
                    draw_enemy(pengu_king, 10);
                } else {
                    draw_enemy(pengu_king, 11);
                }
            } else {
                pengu_king->state = KINGNEUTRAL;
                draw_enemy(pengu_king, 0);
            }
        } break;
    case KINGCOMMAND:
        {
            draw_enemy(pengu_king, 0);

            if (!pengu_king->talking) {
                pengu_king->talking = draw_dialogue_box(pengu_king->dialogue[0], out, pengu_king->portrait, 2);
            }

            /*if (pengu_king->state_time*60 < 220) {
                draw_dialogue_box(pengu_king->dialogue[0], out, pengu_king->portrait, 2);
            } 

            if (pengu_king->state_time*60 > 220 && pengu_king->state_time*60 < 222) {
                pengu_king->dialogue_time = 0;
            }*/

            if (pengu_king->talking) {
                pengu_king->state = KINGWAIT;
                make_guards(stage_size);
                pengu_king->talking = false;
            }

            /*if (pengu_king->state_time*60 > 220 && pengu_king->state_time*60 < 440) {
                draw_dialogue_box(pengu_king->dialogue[1], out, pengu_king->portrait, 2);
            } else if (pengu_king->state_time*60 > 440) {
                make_guards(stage_size);
                pengu_king->state = KINGWAIT;
            }*/
            
        } break;
    case KINGWAIT:
        {
            if (enemys[enemy_count-1].alive || enemys[enemy_count-2].alive) {
                draw_enemy(pengu_king, 0);
            } else {
                draw_enemy(pengu_king, 0);
                pengu_king->state = KINGCHALLENGE;
                pengu_king->dialogue_time = 0;

                king_phase = PHASETWO;
            }
        } break;
    case KINGCHALLENGE:
        {   
            draw_enemy(pengu_king, 0);

            if (!pengu_king->talking) {
                pengu_king->talking = draw_dialogue_box(pengu_king->dialogue[1], out, pengu_king->portrait, 2);
            }

            if (pengu_king->talking) {
                pengu_king->talking = false;
                pengu_king->state = KINGLEAPING;
            }

            /*if (pengu_king->state_time*60 < 200) {
                draw_enemy(pengu_king, 0);
                draw_dialogue_box(pengu_king->dialogue[2], out, pengu_king->portrait, 2);
            } else {
                draw_enemy(pengu_king, 0);
                pengu_king->state = KINGLEAPING;
            }*/
        } break;
    case HIT:
        {
            if (pengu_king->state_time*60 < 4) {
                draw_enemy(pengu_king, 12);
            } else {
                draw_enemy(pengu_king, 12);
                pengu_king->state = pengu_king->state_prev;
            }
        } break;
    case DYING:
        {
            Particle_Parameters min = {};
            Particle_Parameters max = {};

            min.velocity.x = 400*dt;
            min.velocity.y = 400*dt;

            max.velocity.x = 400*dt;
            max.velocity.y = 400*dt;

            min.position.x = pengu_king->position.x;
            min.position.y = pengu_king->position.y;

            max.position.x = pengu_king->position.x + pengu_king->size.x;
            max.position.y = pengu_king->position.y+pengu_king->size.y;

            min.life_time = 120*dt;
            max.life_time = 90*dt;

            min.magnet = player;

            Image image = LoadImage(S("exp.png"));

            for (int i = 0; i < i32(pengu_king->enemy.exp_dropped/10); i++) {
                particle_emit(min, max, image);
            }

            pengu_king->state = DEAD;
        } break;
    case DEAD:
        {
            destroy_boss_walls();

            camera_state = CAMERAFOLLOW;

            bosses_killed[0] = true;

            pengu_king->alive = false;
        } break;
    }

    if (invuln_time <= 0 && pengu_king->state == KINGSTOMPING) {
        Rectangle2 peng_rec;
        if (pengu_king->facing < 0)
        {
            peng_rec = r2_bounds(pengu_king->position, v2(pengu_king->size.x/2, pengu_king->size.y), v2_zero, v2_one);
        } else
        {
            peng_rec = r2_bounds(v2(pengu_king->position.x + pengu_king->size.x/2, pengu_king->position.y), v2(pengu_king->size.x/2, pengu_king->size.y), v2_zero, v2_one);
        }

        if (r2_intersects(peng_rec, get_entity_rect(player))) {
            player_hit(pengu_king, input);
        }
    } else if (invuln_time <= 0) {
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(pengu_king->position, pengu_king->size, v2_zero, v2_one)))
        {
            player_hit(pengu_king, input);
        }
    }

    move_enemy(pengu_king, dt);
    
}

/*void draw_enemy(Entity *pengu, i32 frame) {
    if (pengu->facing > 0) {
        DrawImageMirrored(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5,
            pengu->position.y + pengu->enemy.offset.y), true, false);
    } else {
        DrawImage(pengu->enemy.image[frame], v2(pengu->position.x-camera_pos.x+out->width*.5 + pengu->enemy.offset.x,
            pengu->position.y+pengu->enemy.offset.y));
    }
}

void move_enemy(Entity *pengu, f32 dt) {
    pengu->velocity.y+=496*input->dt;

    f32 dy = pengu->velocity.y*dt;
    f32 dx = pengu->velocity.x*dt;

    for (int i = 0; i < abs_f32(dy); i++) {
        pengu->position.y+=sign_f32(dy);
        if (wall_intersects(pengu) || enemy_overlap(pengu)) {
            pengu->position.y-=sign_f32(dy);
            pengu->velocity.y=0;
            break;
        }
    }


    for (int i = 0; i < abs_f32(dx); i++) {
        pengu->position.x+=sign_f32(dx);
        if (wall_intersects(pengu) || enemy_overlap(pengu)) {
            pengu->position.x-=sign_f32(dx);
            pengu->velocity.x=0;
            break;
        }
    }
}*/

void pengu_attack(Entity *pengu, Entity *player, f32 invuln_time, Game_Input *input) {
    if (invuln_time > 0) return;

    Rectangle2 weapon = r2_bounds(v2(pengu->position.x+pengu->enemy.weapon_offset.x, pengu->position.y+pengu->enemy.weapon_offset.y), pengu->enemy.weapon_size, v2_zero, v2_one);
    Rectangle2 rec_player = get_entity_rect(player);

    if (pengu->facing > 0) {
        weapon = r2_shift(weapon, v2(42, 0));
    }

    if (r2_intersects(weapon, rec_player)) {
        player_hit(pengu, input);
    }
}

void p_soldier_action(Entity *soldier, f32 dt, Entity *player, f32 invuln_time, Game_Input *input) {
    soldier->state_time+=dt;

    if (soldier->enemy.sleep_time > 0) {
        move_enemy(soldier, input->dt);
        soldier->enemy.sleep_time-=input->dt;
        if (soldier->state_time*60 <= 3) {
            draw_enemy(soldier, 7);
        } else {
            draw_enemy(soldier, 0);
        }
        
        return;
    }

    if (player->position.x < soldier->position.x) {
        soldier->facing = -1;
    } else {
        soldier->facing = 1;
    }

    if (soldier->state_prev != soldier->state) {
        soldier->state_prev = soldier->state;
        soldier->state_time = 0;
    }

    if (soldier->state == NEUTRAL) {
        if (abs_i32(soldier->position.x - player->position.x) < 800 && abs_i32(soldier->position.x - player->position.x) > 200)
        {
            soldier->state = MOVE;
            soldier->state_prev = soldier->state;
            soldier->state_time = 0;
        } else if (abs_i32(soldier->position.x - player->position.x) < 200)
        {
            soldier->state = ATTACK;
            soldier->state_prev = soldier->state;
            soldier->state_time = 0;
        }
    }

    if (r2_intersects(get_entity_rect(soldier), get_entity_rect(player))) {
        soldier->state = ATTACK;

        if (soldier->state != soldier->state_prev) {
            soldier->state_time = 0;
            soldier->state_prev = ATTACK;
        }
    }


    switch (soldier->state)
    {
    case MOVE:
        {
            soldier->velocity.x = 120*sign_f32(soldier->facing);

            move_enemy(soldier, dt);
            if (soldier->facing > 0) {
                if (abs_i32(soldier->position.x - player->position.x) > 5) {
                    if (i32(soldier->state_time*60) < 60) {
                        draw_enemy(soldier, 0);
                    } else {
                        draw_enemy(soldier, 1);
                    }
                } else {
                    draw_enemy(soldier, 0);
                    soldier->state = ATTACK;
                } 
            } else {
                if (abs_i32(soldier->position.x - player->position.x) > player->size.x+5) {
                    if (i32(soldier->state_time*60) < 60) {
                        draw_enemy(soldier, 0);
                    } else {
                        draw_enemy(soldier, 1);
                    }
                } else {
                    draw_enemy(soldier, 0);
                    soldier->state = ATTACK;
                } 
            }
        } break;
    case ATTACK:
        {

            if (i32(soldier->state_time*60) < 30)
            {
                draw_enemy(soldier, 2);
            } else if (i32(soldier->state_time*60) < 60)
            {
                draw_enemy(soldier, 3);

                if (invuln_time <= 0)
                {
                    pengu_attack(soldier, player, invuln_time, input);
                }
            } else {
                draw_enemy(soldier, 0);
                soldier->state = MOVE;
            }
        } break;
    case HIT:
        {
            if (soldier->state_time*60 <= 2) {
                draw_enemy(soldier, 7);
            }

            if (soldier->position.x > player->position.x)
            {
                soldier->velocity.x = 8000*dt;
                soldier->velocity.y -= 6000*dt;
            } else
            {
                soldier->velocity.x = -8000*dt;
                soldier->velocity.y -= 6000*dt;
            }

            soldier->enemy.sleep_time=25*input->dt;

            soldier->state = MOVE;
        } break;
    case DYING:
        {
            Particle_Parameters min = {};
            Particle_Parameters max = {};

            min.velocity.x = 400*dt;
            min.velocity.y = 400*dt;

            max.velocity.x = 400*dt;
            max.velocity.y = 400*dt;

            min.position.x = soldier->position.x;
            min.position.y = soldier->position.y;

            max.position.x = soldier->position.x + soldier->size.x;
            max.position.y = soldier->position.y+soldier->size.y;

            min.life_time = 120*dt;
            max.life_time = 90*dt;

            min.magnet = player;

            Image image = LoadImage(S("exp.png"));

            for (int i = 0; i < i32(soldier->enemy.exp_dropped/10); i++) {
                particle_emit(min, max, image);
            }

            soldier->state = DEAD;

            /*if (i32(soldier->state_time*60) < 2) {
                draw_enemy(soldier, 4);
            } else if (i32(soldier->state_time*60) < 4) {
                draw_enemy(soldier, 5);
            } else if (i32(soldier->state_time*60) < 6) {
                draw_enemy(soldier, 6);
            } else {
                soldier->alive = false;
            }*/
        } break;
    case DEAD:
        {
            soldier->alive = false;
        }
    }
}