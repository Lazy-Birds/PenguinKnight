slime_action(Entity *slime) {
	 slime->state_time+=dt;

    if (slime->enemy.sleep_time > 0) {
        move_pengu(slime, input->dt);
        slime->enemy.sleep_time-=input->dt;
        if (slime->state_time*60 <= 3) {
            draw_pengu(slime, 7);
        } else {
            draw_pengu(slime, 0);
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
}