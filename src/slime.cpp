void slime_action(Entity *slime, Entity *player, Game_Input *input) {
  slime->state_time+=input->dt;

  if (slime->enemy.sleep_time > 0) {
    move_enemy(slime, input->dt);
    slime->enemy.sleep_time-=input->dt;
    if (slime->state_time*60 <= 20) {
        draw_enemy(slime, 0);
        if (slime->state_time*60 >= 10) {
            slime->velocity.x = 0;
        }
    } else if (slime->state_time*60 <= 40) {
        draw_enemy(slime, 1);
    } else {
       draw_enemy(slime, 2);
   }

   if (slime->enemy.sleep_time*60 <= 1) {
    slime->state_time = 0;
}

return;
}

if (player->position.x < slime->position.x) {
    slime->facing = -1;
} else {
    slime->facing = 1;
}

if (slime->state_prev != slime->state) {
    slime->state_prev = slime->state;
    slime->state_time = 0;
}

switch (slime->state)
{
case NEUTRAL:
   {
      if (abs_i32(slime->position.x - player->position.x) < 800)
      {
         slime->state = MOVE;
         draw_enemy(slime, 0);
     } else if (slime->state_time*60 < 20) 
     {
         draw_enemy(slime, 0);
     } else if (slime->state_time*60 < 40) 
     {
         draw_enemy(slime, 1);
     } else 
     {
         draw_enemy(slime, 2);
     }
 } break;
case MOVE:
   {
      if (slime->state_time*60 < 30)
      {
         draw_enemy(slime, 0);
     } else if (slime->state_time*60 < 60)
     {
         draw_enemy(slime, 1);
     } else if (slime->state_time*60 < 62)
     {
        if (slime->velocity_prev.x == 0) {
            slime->velocity_prev.x = random_i32_between(1500, 2500)*input->dt*sign_f32(player->position.x-slime->position.x);
            slime->velocity_prev.y = random_i32_between(6000, 10000)*input->dt;

        }
        slime->velocity.x+=slime->velocity_prev.x;
        slime->velocity.y-=slime->velocity_prev.y;
        draw_enemy(slime, 2);
    } else if (slime->velocity.y < 0)
    {
     draw_enemy(slime, 2);
 } else if (!entity_on_wall(slime) && !enemy_on_enemy(slime))
 {
     draw_enemy(slime, 1);
 } else 
 {
     draw_enemy(slime, 0);
     slime->enemy.sleep_time+=60*input->dt;
     slime->velocity.x = 0;
     slime->state_time = 0;

     slime->velocity_prev = v2_zero;

     if (random_f32_between(0, 1) < .25) {
        slime->state = SHOOT;
    } else {
        slime->state = MOVE;
    }
}
} break;
case SHOOT:
    {
        if (slime->state_time*60 < 20) {
            slime->projectile_launched = false;
            draw_enemy(slime, 4);
        } else if (slime->state_time*60 < 40) {
            Image dart = LoadImage(S("slime_dart.png"));
            draw_enemy(slime, 5);
            DrawImage(dart, v2(slime->position.x-camera_pos.x+out->width*.5, slime->position.y + 17));
        } else if (slime->state_time*60 < 60) {
            if (!slime->projectile_launched) {
                make_projectile(slime->projectile, v2(slime->position.x, slime->position.y+17), v2(8000*input->dt*sign_f32(player->position.x-slime->position.x), 0));
                slime->projectile_launched = true;
            }

            draw_enemy(slime, 4);
        } else {
            draw_enemy(slime, 0);
            slime->enemy.sleep_time+=60*input->dt;
            slime->velocity.x = 0;
            slime->state_time = 0;
            slime->projectile_launched = false;

                if (random_f32_between(0, 1) < .25) {
                    slime->state = SHOOT;
                } else {
                    slime->state = MOVE;
                }
            }
        } break;
    case HIT:
        {
            if (slime->state_time*60 <= 6) {
                draw_enemy(slime, 4);
                if (slime->position.x > player->position.x)
                {
                    slime->velocity.x = 8000*input->dt;
                    slime->velocity.y -= 6000*input->dt;
                } else
                {
                    slime->velocity.x = -8000*input->dt;
                     slime->velocity.y -= 6000*input->dt;
                 }
            }

            slime->enemy.sleep_time=20*input->dt;

            slime->state = MOVE;
        } break;
    case DYING:
    {
        Particle_Parameters min = {};
        Particle_Parameters max = {};

        min.velocity.x = 400*input->dt;
        min.velocity.y = 400*input->dt;

        max.velocity.x = 400*input->dt;
        max.velocity.y = 400*input->dt;

        min.position.x = slime->position.x;
        min.position.y = slime->position.y;

        max.position.x = slime->position.x + slime->size.x;
        max.position.y = slime->position.y+slime->size.y;

        min.life_time = 120*input->dt;
        max.life_time = 90*input->dt;

        min.magnet = player;

        Image image = LoadImage(S("exp.png"));

        for (int i = 0; i < i32(slime->enemy.exp_dropped/10); i++) {
            particle_emit(min, max, image);
        }

        slime->state = DEAD;

            /*if (i32(slime->state_time*60) < 2) {
                draw_enemy(slime, 4);
            } else if (i32(slime->state_time*60) < 4) {
                draw_enemy(slime, 5);
            } else if (i32(slime->state_time*60) < 6) {
                draw_enemy(slime, 6);
            } else {
                slime->alive = false;
            }*/
    } break;
    case DEAD:
        {
            slime->alive = false;
        } break;
    }

    if (invuln_time <= 0){
        if (r2_intersects(r2_bounds(player->position, player->size, v2_zero, v2_one), r2_bounds(slime->position, slime->size, v2_zero, v2_one)))
        {
            player_hit(slime, input);
        }   
    }

    move_enemy(slime, input->dt);
}

void ooze_action(Entity *ooze, Game_Input *input) {
    Level *level = &World[player.player_level];

    Rectangle2 kill_box = r2_bounds(v2(ooze->position.x+144, ooze->position.y+146), v2(48, 45), v2_zero, v2_one);
    Rectangle2 kill_box_out = r2_shift(kill_box, v2(-camera_pos.x+out->width*.5, 0));
    //DrawRectOutline(kill_box_out, v4_red, 2);

    if (r2_intersects(get_entity_rect(&player), kill_box)) {
        player.alive = false;
    }

    if (ooze->sprite_index == 1) {
        ooze->state_time-=input->dt;
    } else {
        ooze->state_time+=input->dt;
    }

    if (ooze->enemy.sleep_time > 0) {
        move_enemy(ooze, input->dt);
        ooze->enemy.sleep_time-=input->dt;
        if (ooze->state_time*60 <= 20) {
            draw_enemy(ooze, 0);
            if (ooze->state_time*60 >= 10) ooze->velocity.x = 0;
        } else {
            draw_enemy(ooze, 1);
        }
        
        if (ooze->enemy.sleep_time*60 <= 1) ooze->state_time = 0;

        return;
    }

    if (ooze->sprite_index > 0 && level->id == 5) {
        draw_boss_health_bar(ooze);
        ooze->attackable = true;
    }

    if (player.position.x < ooze->position.x+144) {
        ooze->facing = -1;
        ooze->enemy.anchor_pos = v2(ooze->position.x+15, ooze->position.y+18);
    } else {
        ooze->facing = 1;
        ooze->enemy.anchor_pos = v2(ooze->position.x+21, ooze->position.y+18);
    }

    if (ooze->state_prev != ooze->state) {
        ooze->state_prev = ooze->state;
        ooze->state_time = 0;
    }

    switch (ooze->state)
    {
    case NEUTRAL:
        {
            draw_enemy(ooze, 0);
            if (ooze->state_time*60 > 20 && ooze->sprite_index == 0 && entity_get_distance_x(ooze, &player) < 450) 
            {
                ooze->state = SPYING;
            } else if (ooze->sprite_index == 2) 
            {
                if (ooze->current_health < ooze->max_health*.5 && corruption_phase == 0) {
                    ooze->state = CORRUPTING;
                } else {
                    ooze->state = HAMMER;
                }
                
            }
        } break;
    case SPYING:
        {
            if (ooze->state_time*60 < -1) {

                ooze->state = NEUTRAL;
                
                ooze->sprite_index = 2;
            }

            if (ooze->state_time*60 < 10) {
                draw_enemy(ooze, 1);
            } else if (ooze->state_time*60 < 20) {
                draw_enemy(ooze, 2);
            } else if (ooze->state_time*60 < 30) {
                draw_enemy(ooze, 3);
            } else if (ooze->state_time*60 < 40) {
                draw_enemy(ooze, 4);
            } else if (ooze->state_time*60 < 50) {
                draw_enemy(ooze, 5);
            } else if (ooze->state_time*60 < 60) {
                draw_enemy(ooze, 6);
            } else if (ooze->state_time*60 < 70) {
                draw_enemy(ooze, 7);
            } else if (ooze->state_time*60 < 80) {
                draw_enemy(ooze, 8);
            } else if (ooze->state_time*60 < 140) {
                draw_enemy(ooze, 9);
            } else if (ooze->state_time*60 < 160) {
                draw_enemy(ooze, 10);
            } else if (ooze->state_time*60 < 180) {
                draw_enemy(ooze, 10);
                ooze->state_time = 59*input->dt;
                ooze->sprite_index = 1;
            }
        } break;
    case HAMMER:
        {
            if (ooze->state_time*60 < 5) {
                draw_enemy(ooze, 11);
            } else if (ooze->state_time*60 < 10) {
                draw_enemy(ooze, 12);
            } else if (ooze->state_time*60 < 15) {
                draw_enemy(ooze, 13);
            } else if (ooze->state_time*60 < 20) {
                draw_enemy(ooze, 14);
            } else if (ooze->state_time*60 < 25) {
                draw_enemy(ooze, 15);
            } else if (ooze->state_time*60 < 30) {
                draw_enemy(ooze, 16);
            } else if (ooze->state_time*60 < 35) {
                draw_enemy(ooze, 17);
            } else if (ooze->state_time*60 < 40) {
                draw_enemy(ooze, 18);
            } else if (ooze->state_time*60 < 45) {
                draw_enemy(ooze, 19);
            } else if (ooze->state_time*60 < 50) {
                draw_enemy(ooze, 20);
            } else if (ooze->state_time*60 < 60) {
                draw_enemy(ooze, 21);
            } else if (ooze->state_time*60 < 65) {
                draw_enemy(ooze, 22);
            } else if (ooze->state_time*60 < 75) {
                draw_enemy(ooze, 23);
            } else if (ooze->state_time*60 < 80) {
                draw_enemy(ooze, 24);
            }  else if (ooze->state_time*60 < 95) {
                draw_enemy(ooze, 25);
            }  else if (ooze->state_time*60 < 100) {
                draw_enemy(ooze, 26);
            } else {
                draw_enemy(ooze, 0);
                ooze->state = LAZER;
            }
        } break;
    case LAZER:
        {
            if (ooze->state_time*60 < -1) {

                ooze->state = NEUTRAL;
                ooze->sprite_index = 2;
            }

            if (ooze->state_time*60 < 10) {
                draw_enemy(ooze, 1);
            } else if (ooze->state_time*60 < 20) {
                draw_enemy(ooze, 2);
            } else if (ooze->state_time*60 < 30) {
                draw_enemy(ooze, 3);
            } else if (ooze->state_time*60 < 40) {
                draw_enemy(ooze, 4);
            } else if (ooze->state_time*60 < 50) {
                draw_enemy(ooze, 5);
            } else if (ooze->state_time*60 < 60) {
                draw_enemy(ooze, 6);
            } else if (ooze->state_time*60 < 70) {
                draw_enemy(ooze, 7);
            } else if (ooze->state_time*60 < 80) {
                draw_enemy(ooze, 8);
            } else if (ooze->state_time*60 < 140) {
                draw_enemy(ooze, 9);
            } else if (ooze->state_time*60 < 160) {
                draw_enemy(ooze, 10);
                draw_super_lazer(ooze->position, player.position);
            } else if (ooze->state_time*60 < 180) {
                draw_enemy(ooze, 10);
                ooze->state_time = 59*input->dt;
                ooze->sprite_index = 1;
            }        
        } break;
    case CORRUPTING:
        {
            if (ooze->state_time*60 < 20) {
                if (corruption_phase == 0) {
                  
                    Vector2 pos = v2(1248, 576);

                    for (int i = 0; i < 17; i++) {
                        World[player.player_level].liquid.data[World[player.player_level].liquid.count] = make_liquid(v2(pos.x + i*48, pos.y), 1);
                        World[player.player_level].liquid.count++;
                    }
                    corruption_phase = 1;
                    draw_enemy(ooze, 0);
                } 
                
                ooze->position = v2(1440, 192);
                ooze->hitbox = r2_bounds(v2(ooze->position.x+144, ooze->position.y+48), v2(48, 96), v2_zero, v2_one);
            } else if (ooze->state_time*60 < 100) {
                raise_water();
            } else {
                ooze->state = NEUTRAL;
            }
        } break;
    }
}

void raise_water() {
    Level *level = &World[player.player_level];
    for (int i = 0; i < level->liquid.count; i++) {
        level->liquid.data[i].sprite_index++;
    }  
}

void draw_super_lazer(Vector2 point_one, Vector2 point_two) {
    Vector2 eye_pos = v2(point_one.x+116, point_one.y+64);
    f32 radians = angle_between(eye_pos, point_two);

    Vector2 inc = v2_arm(radians);

    Image lazer_red = LoadImage(S("lazer_outline.png"));
    Image lazer_white = LoadImage(S("lazer_inline.png"));

    for (f32 i = 0; i < 600; i+=.5) {

        Vector2 pos = v2(point_one.x+116+inc.x*(i*random_i32_between(2, 4))-camera_pos.x+out->width*.5, point_one.y+64+inc.y*(i*random_i32_between(2, 4)));
        //Vector2 pos_two = v2(pos.x+8, pos.y+8);

        DrawImage(lazer_red, pos);
        //DrawImage(lazer_white, pos_two);
    }

    for (f32 i = 0; i < 500; i+=.5) {
        Vector2 pos = v2(point_one.x-16+136+inc.x*(i*random_i32_between(2, 4))-camera_pos.x+out->width*.5, point_one.y+8+64+inc.y*(i*random_i32_between(2, 4)));

        DrawImage(lazer_white, pos);
    }
}