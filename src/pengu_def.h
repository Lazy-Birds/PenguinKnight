//Structs
//Weapon
struct Weapon {
    Image *image;
    String name;
    Image icon;

    Vector2 position;
    Vector2 size;

    Vector2 weapon_frames;
    Vector2 offset_right;
    Vector2 offset_left;

    Vector2 hit_size;
    Vector2 hit_offset_right;
    Vector2 hit_offset_left;
    Vector2 charged_frames;
    i32 charge_time;

    i32 base_damage;
    String damage_attribute;
    i32 damage_multiplier;
    i32 guard_damage;

    Vector2 jump_frame;
    Vector2 dash_frame;
    Vector2 jump;
    Vector2 guard;
    i32 frame_hit;
};

//User
struct texture {
    u32 pixel;
    Image image;
};

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
    i32 id;

    i32 action_id;
    bool acting;
    bool actable;

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

    i32 player_level;
};

//Levels
struct Level {
    Image world;
    Image background;
    Rectangle2 entry_points;
    i32 id;
    Vector2 landing_pos;
    Entity *interactible;
    i32 interactible_count;
};

//Background
struct Particle {
	Image image;
	Vector2 velocity;
	Vector2 position;
	bool is_alive;
	f32 depth;
	i32 direction;
	f32 life_time;
	i32 type;
	Entity *magnet;
	f32 sleep_time;
};

struct Particle_Parameters {
	Vector2 velocity;
	Vector2 position;
	Vector4 color;
	f32 life_time;
	Vector2 accel;
	f32 friction;
	Entity *magnet;
};

//Entity
struct Background {
    Vector2 position;
    Vector2 size;
    Image image;
    i32 frames;
};

struct Housing {
    Vector2 position;
    Vector2 size;
    Image *image;
    i32 frames;
    f32 state_time;
};

struct Fire {
    Vector2 position;
    Vector2 size;
    Image *image;
    i32 frames;
    f32 state_time;
};

//Functions
//Levels
void create_levels();
void make_interactible(Vector2 position, i32 id, i32 level_id);
void interact(Entity *interactible, Game_Input *input, Vector2 camera_pos);

//Weapon
void load_weapon();

//Background
i32 get_open_snowflake();
void spawn_initial_snowflakes(Game_Output *out, Game_Input *input);
void spawn_snowflakes(Game_Output *out, Game_Input *input);
void update_snowflakes(Game_Output *out, i32 offset);

//Dialogue
void load_fairy_dialogue(Entity *fairy);
void load_penguin_king_dialogue(Entity *pengu);
void clip_strings(String words);
b32 draw_dialogue_box(String words, Game_Output *out, Image *image, i32 frames);

//Entity
u32 image_get_pixel(Image image, int x, int y);
Entity load_enemy(Vector2 pos, i32 type);
void make_npcs(Vector2 pos, i32 type);
void make_wall(Vector2 pos, u32 pixel, String image);
void make_world(Level level);
Entity get_wall_at(Vector2 pos);
bool enemy_on_enemy(Entity *entity_one);
bool entity_on_wall(Entity *entity_one);
bool wall_intersects(Entity *entity);
bool wall_ahead(Entity *entity);
bool entity_in_air(Entity *entity_one);
void particle_create(Vector2 pos, Vector2 velocity, f32 lifetime, /*Vector4 color, Vector2 accel,*/ Image image);
void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image);
void particle_update(f32 dt);
void make_boss_walls(Vector2 *pos, i32 count);
void destroy_boss_walls();
void draw_enemy(Entity *nme, i32 frame);
void move_enemy(Entity *nme, f32 dt);
i32 get_prejectile_slot();
void make_projectile(Image *image, Vector2 pos, Vector2 vel);
void update_projectiles(Game_Input *input, Entity *player);
void draw_fire(f32 dt);
void draw_house(Housing *house);
void npc_action(Entity *npc, Entity *player);

//Enemy_Pengus
void make_guards(Vector2 positions);
void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt, Game_Output *out);
void pengu_attack(Entity *pengu, Entity *player, f32 invuln_time, Game_Input *input);
void p_soldier_action(Entity *soldier, f32 dt, Entity *player, f32 invuln_time, Game_Input *input);

//Seals
void seal_action(Entity *seal, Game_Input *input, Entity *player);
void seal_move(Entity *seal, Game_Input *input);

//Slime
void slime_action(Entity *slime, Entity *player, Game_Input *input);

//Menu
i32 mouse_over_rec(Rectangle2 rec1, Rectangle2 rec2);
void level_char_up(Entity *player, i32 stat);
void draw_menu(Game_Output *out, Entity *player, Image screen);

//User
void set_camera_pos();
Rectangle2 get_entity_rect(Entity *entity);
bool enemy_overlap(Entity *entity);
void draw_bound_box(Entity *entity);
void weapon_attack(Vector2 pos, Weapon weapon, i32 facing, i32 dmg_attr, i32 attack_type);
void draw_charging(i32 frame);
void bosses_alive();
void GameStart(Game_Input *input, Game_Output *out);
void reset_vars();
void set_world(b32 reset, Level level);
i32 get_frame_walk(f32 dt);
i32 get_frame_attack(Weapon weapon, f32 dt);
i32 get_frame_charged(f32 dt, i32 ch_multi);
i32 get_dmg_attr();

//Player
void player_action(Game_Input *input);
void check_level();
void set_enemy_vuln();
void player_hit(Entity *entity, Game_Input *input);
void player_move(Game_Input *input);