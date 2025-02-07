//Enums
enum entity_types {
    et_default,
    et_wall,
    et_background,
    et_liquid,
    et_player,
    et_plant,
    et_seal,
    et_penguin_king,
    et_penguin_soldier,
    et_robo_pup,
    et_slime,
    et_eye_monster,
    et_ooze,
    et_coyote_nick,
    et_marmoset,
    et_hedgehog,
    et_fairy,
    et_lil_pengu,
    et_mayor_snoresly,
    et_pengu_puller,
    et_lever,
    et_gate,
    et_door,
    et_fire,
    et_hook,
    et_ladder,
};

enum entity_subtypes {
    st_default,
    st_boss,
    st_gate_top,
    st_gate_bot,
    st_village,
    st_sewer,
    st_no_bg,
    st_right,
    st_left,
    st_cv_wall,
    st_cld_brk_wall,
    st_bolt_plates_14,
    st_bolt_plates_15,
    st_bolt_plates_16,
    st_pipe_opening,
    st_work_rope,
    st_spikes,
    st_badger_tent,
    st_factory_walls_14,
    st_factory_walls_15,
    st_pointed_fir,
    st_factory_walls_16,
    st_street_lamps,
    st_peasant_hovel,
    st_clock_tower,
    st_sewerwater,
    st_sewerwater_still,
    st_raw_sewerwater_still,
};

enum player_weapons {
    pw_cleaver,
    pw_staff,
};

enum magic_spell {
    sp_magic_missile,
};

enum campfire_ids {
    cf_village,
    cf_sewer_entrance,
    cf_blackpowder_forest,
};

enum movement_checks {
    no_chks,
    chk_ldg,
    chk_jmp,
    chk_ldg_jmp,
};

enum player_stats {
    at_vigor,
    at_rigour,
    at_strength,
    at_dexterity,
    at_mental,
};

//Structs
//Weapon
struct Weapon {
    Image *image;
    Image *poisoned_image;
    i32 type;
    Image icon;
    Image *projectile;

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
    i32 damage_attribute;
    i32 damage_multiplier;
    i32 guard_damage;

    Vector2 jump_frame;
    Vector2 dash_frame;
    Vector2 jump;
    Vector2 guard;
    Vector2 item_pickup;

    i32 sprite_index;
    f32 state_time;
    i32 frame_hit;
};

struct WeaponArray {
    Weapon *data;
    i32 count;
    i32 capacity;
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
    Vector2 anchor_pos_two;
    i32 exp_dropped;
};

//Items
struct Item {
    Image *icon;
    String name;
    String description;
    bool consumable;
    i32 type;
};

//Array of Items
struct ItemArray {
    Item *data;
    i32 count;
    i32 capacity;
};

//Array of Item arrays, aka an inventory
struct InventoryArray {
    ItemArray *data;
    i32 count;
    i32 capacity;
};

//NPC List
struct NPC {
    String name;
    Image icon;
    String Description;
    String_Array unlocked_dialogue;
};

struct NPCArray {
    NPC *data;
    i32 capacity;
    i32 count;
};

struct Campfire {
    Vector2 position;
    i32 level;
    i32 id;
};

struct CampfireArray {
    Campfire *data;
    i32 capacity;
    i32 count;
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

    //Bools
    bool invuln;
    bool attackable;
    bool alive;
    bool has_hit;
    bool poisoned;
    bool assymetric;

    //Sounds
    Sound jumpies;
    Sound hit;

    f32 max_stamina;
    f32 current_stamina;

    f32 max_mp;
    f32 current_mp;
    f32 mp_cooldown;
    f32 shield_hit;

    i32 fairy_uses;
    i32 current_fairy_uses;

    Vector2 check_point;
    i32 check_point_level;
    Vector2 position;
    Vector2 extra_position;
    Vector2 velocity;
    Vector2 velocity_prev;

    Vector2 size;
    Vector2 anchor;
    Vector2 offset;
    Rectangle2 hitbox;

    i32 facing;
    i32 type;
    i32 sub_type;
    i32 id;
    i32 movement_chks;
    i32 damage;

    bool acting;
    f32 acting_cd;
    bool actable;
    bool can_climb;
    bool affected_by_gravity;

    i32 sprite_index;
    f32 state_time;
    f32 animation_time;

    Weapon weapon;
    InventoryArray inventory;
    NPCArray entities_met;

    Image *projectile;
    b32 projectile_launched;

    texture wall_type;
    Enemy enemy;

    i32 state;
    i32 state_prev;

    Image *image;
    Image *portrait;

    String name;
    String_Array dialogue;
    f32 dialogue_time;
    b32 talking;

    i32 exp_gained;
    i32 exp_to_level;
    i32 level;

    i32 player_level;
};

//Generic
struct EntityArray {
    Entity *data;
    i64 count;
    i64 capacity;
};

//EntityArray of pointers
struct EntityArrayP {
    Entity **data;
    i64 count;
    i64 capacity;
};

//Levels
struct Entry_Points {
    Rectangle2 entry;
    i32 link_id;
    i32 level_id;
    Vector2 landing_pos;
    bool actable;
};

struct Level {
    String name;
    Image world;
    Image background;
    Vector2i region;
    i32 id;
    i32 scale;
    EntityArray entities;
    EntityArray liquid;
    bool initialized;
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
Entity make_interactible(Vector2 position, i32 type, i32 sub_type);
void interact(Entity *interactible, Game_Input *input, Vector2 camera_pos);
void create_level_entries();

//Weapon
WeaponArray make_weapon_array(i32 capacity);
void load_weapon();
void charged_projectile(Weapon *wep);
void load_cleaver();
void load_staff();

//Background
i32 get_open_snowflake();
void spawn_initial_snowflakes(Game_Output *out, Game_Input *input);
void spawn_snowflakes(Game_Output *out, Game_Input *input);
void update_snowflakes(Game_Output *out, i32 offset);
void generate_drips(Vector2 pos, Image drip, Game_Input *input);

//Dialogue
void clip_strings(String words);
b32 draw_dialogue_box(String words, Game_Output *out, Image *image, i32 frames);
String_Array load_dialogue(Entity *ent);

//Entity
u32 image_get_pixel(Image image, int x, int y);
Entity load_npc(Vector2 pos, i32 type);
void make_entities(Vector2 pos, i32 type);
void make_wall(Vector2 pos, u32 pixel, String image);
void make_world();
EntityArray make_entity_array(i32 size);
EntityArray make_entity_array_two(Arena *arena, i32 size);
Entity make_liquid(Vector2 pos, i32 id);
void liquid_do_liquid(Entity *liquid, Game_Input *input, Game_Output *out);
Entity get_wall_at(Vector2 pos);
void reset_enemies();
//bool enemy_on_enemy(Entity *entity_one);
bool entity_on_wall(Entity *entity_one);
bool entity_against_wall(Entity *ent);
bool wall_intersects(Entity *entity);
bool wall_intersects_rec(Rectangle2 rec);
bool wall_ahead(Entity *entity);
bool point_in_wall(Vector2 point);
bool ledge_ahead(Entity *ent);
bool entity_in_air(Entity *entity_one);
f32 entity_get_distance_x(Entity *one, Entity *two);
f32 entity_get_distance_y(Entity *one, Entity *two);
f32 entity_get_anchor_distance_direct(Entity *one, Entity *two);
i32 get_entity_direction(Entity *entity);
void reset_particles();
void particle_create(Vector2 pos, Vector2 velocity, f32 lifetime, /*Vector4 color, Vector2 accel,*/ Image image);
void particle_emit(Particle_Parameters min, Particle_Parameters max, Image image);
void particle_update(f32 dt);
void emit_death_particles(Entity *ent, f32 dt);
void make_boss_walls(Vector2 *pos, i32 count);
void destroy_boss_walls();
void draw_boss_health_bar(Entity *boss);
void draw_normal_enemy_health(Entity ent);
void draw_entity(Entity *nme, i32 frame);
void move_entity(Entity *nme, f32 dt);
i32 get_prejectile_slot();
void make_projectile(Image *image, Vector2 pos, Vector2 vel, i32 sprite_index);
void update_projectiles(Game_Input *input, Entity *player);
void emit_backwards_particles(Entity *ent, Game_Input *input);
void draw_fire(Entity *interactible, Game_Input *input);
void draw_house(Housing *house);
void enemy_action(Entity *enemy, Game_Input *input);
void npc_action(Entity *npc, Entity *player);
void interactible_actions(Entity *ent, Game_Input *input);
void entity_action(Game_Input *input);
void entity_processing(Game_Input *input, Game_Output *out);
//void enemy_turn(Game_Input *input);

//Enemy_Pengus
void make_guards(Vector2 positions);
void penguin_king_action(Entity *pengu_king, Entity *player, f32 dt, Game_Output *out);
void pengu_attack(Entity *pengu, Entity *player, f32 invuln_time, Game_Input *input);
void p_soldier_action(Entity *soldier, f32 dt, Entity *player, f32 invuln_time, Game_Input *input);

//Misc Enemies
void seal_action(Entity *seal, Game_Input *input, Entity *player);
void eye_monster_action(Entity *monster, Game_Input *input);
void lazer_attack(Entity *monster);

//Slime
void slime_action(Entity *slime, Entity *player, Game_Input *input);
void ooze_action(Entity *ooze, Game_Input *input);
void raise_water();
void draw_super_lazer(Vector2 point_one, Vector2 point_two);

//Menu
i32 mouse_over_rec(Rectangle2 rec1, Rectangle2 rec2);
void level_char_up(Entity *player, i32 stat);
void item_menu();
void draw_menu(Game_Output *out, Entity *player, Image screen, bool at_bonfire);

//Inventory
ItemArray stack_maker(i32 capacity);
InventoryArray make_inventory(i32 capacity);

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
void draw_player(i32 frame);
void player_move(Game_Input *input);
b32 player_in_poison();
void draw_hook_shot(Entity *target);

//Player actions
i32 get_spell_slot();
void magic_emit(i32 spell_type);
void update_spells();

//Coyote
void coyote_action(Entity *coyote);
void marmoset_action(Entity *marm);
void hedgehog_actions(Entity *hedge);