#include <mutex>

//ESP
inline std::mutex          entity_mutex;
inline std::vector<BasePlayer> otherplayers;
inline std::vector<Entity> corpse;
inline std::vector<Entity> vehicles;
inline std::vector<Entity> Stash;
inline std::vector<Entity> DroppedItem;
inline std::vector<Entity> Animal;
inline std::vector<Entity> SulfurNodes;
inline std::vector<Entity> StoneNodes;
inline std::vector<Entity> MetalNodes;
inline std::vector<Entity> Airdrop;
inline std::vector<Entity> patrol_heli;
inline std::vector<Entity> tool_cupboard;
inline std::vector<Entity> hackable_crate;
inline std::vector<Entity> hemp;
inline std::vector<Entity> food;
inline std::vector<Entity> cargo_ship;
inline std::vector<Entity> high_tier_crates;
inline std::vector<Entity> low_tier_crates;


//main settings
struct Settings{
	bool crosshair = true;
	bool ismenuopen = true;
	int crosshair_size1 = 50, crosshair_thickness = 1;
	bool setadmin, no_animations, doublejump ,day_changer, snapline_esp , sky_changer, omni_sprint, spiderman, water, node_esp, sulfur_node, stone_node, metal_node, hackable_crate, patrol_heli, hemp, food_esp, high_tier_crate, low_tier_crate, airdrop, cargo_esp, tool_cupboard, dropped_esp, stash_esp, heli_esp, corpse_esp, tank_esp;
	float gravity_multiplier = 0.975f, day_time = 12;
	int  crosshair_size = 50, max_dist = 300, max_dist_ore = 300, max_dist_vehicle = 300, max_dist_collectable = 300, max_dist_high_crate = 300, max_dist_low_crate = 300, max_dist_corpse = 300, max_dist_airdrop = 300, max_dist_tool_cupboard = 300, max_dist_dropped_items = 300, max_dist_stash = 300, max_dist_hackable = 300;
	bool test_esp = true, skeleton_esp = true, fov_changer_on_key, rainbow_esp, DrawPlayers = true, distance_esp = true, name_esp = true, held_weapon = true, hotbar_esp, filled_box_esp = true, box_esp = true, head_esp = true, flag_esp, cornered_box_esp, oldbox_esp = true, sleeper, scientist, recoil_control_system, standalone_recoil_control_system, health_esp = true, team_esp = true, fast_swim, speedhack;
	bool recoil, sway, spread, jesus_walk, automatic, reload, speed, aimbot, fast_shoot, deploy, penetration, thickness, distancehack, fov_changer, walkonwater, walltest;
	bool weaponstuff, test, bowshit, slidetest;
	int  freeze_key, aimbot_key, water_key, swim_key, fov_changer_key, jesus_walk_key, speed_key, walkonwater_key;
	int fov = 10, recoil_multiplier = 100;
	bool mountable, compound_bow, bow, extended_melee, swim, freeze, melee, med, eoka, cloth, heal;
	float spread_multiplier = 0.f, jesus_speed = 0.5f, reload_time, speed_time, swimspeed = 0.5f, fastshoot_modifier, thickness_multiplier = 1.6, distance_multiplier, sky_color = 1, night_sky_color = 1, moon_color = 1, cloud_color = 1, cloud_brightness = 1, speedhack_time = 1, aimbot_smooth = 0.6, fov_multiplier = 9;
};

inline Settings config;