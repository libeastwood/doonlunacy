structures = {
    'barracks' :
    {
        'health' : 800,
        'power_requirement' : 20,
        'view_range' : 4,
    },

    'construction_yard' : 
    {
        'health' : 1500,
        'power_requirement' : 0,
        'size' : [2,2],
        'view_range' : 6,
        'upgrade_levels' : 1,
        'upgrade_cost' : 500,    
    },

    'gun_turret' : {
        'damage' : 100,
        'health' : 700,
        'power_requirement' : 10,
        'primary_weapon_reload_time' : 100,
        'view_range' : 5,
        'weapon_range' : 5,
        'size' : [1,1],
        'turn_Speed' : 0.01,
        'attackSound' : "Sound_gunTurret",
        'bulletType' : "Bullet_Shell",
    },
    
    'heavy_factory' : {
        'health' : 1000,
        'power_requirement' : 35,
        'size' : [3,2],
        'upgrade_levels' : 2,
        'upgrade_cost' : 500,    
        'view_range' : 5,        
    },

    'high_tech_factory' : {
        'health' : 1000,
        'power_requirement' : 35,
        'size' : [3,2],
        'view_range' : 5,
    },

    'ix' : {
        'size' : [2,2],
        'health' : 1000,
        'power_requirement' : 40,
        'view_range' : 3,
    },
    
    'light_factory' : {
        'size' : [2,2],
        'health' : 1000,
        'power_requirement' : 20,
        'upgrade_Levels' : 1,
        'upgrade_cost' : 500,
        'view_range' : 5,
    },
    
    'palace' : {
        'health' : 2000,
        'power_requirement' : 200,
        'view_range' : 8,
    },
    
    'rocket_turret' : {
        'damage' : 100,
        'health' : 700,
        'power_requirement' : 15,
        'size' : [1,1],
        'view_range' : 7,
        'weapon_range' : 9,
        'attackSound' : "Sound_rocket",
        'bulletType' : "Bullet_Rocket",
    },
    
    'refinery' : {
        'capacity' : 2000,
        'health' : 1500,
        'power_requirement' : 30,
        'size' : [3,2],
        'view_ange' : 6,
    },
    
    'radar' : {
        'health' : 1000,
        'power_requirement' : 20,
        'view_range' : 8,
        'size' : [2,2],
    },

    'repair_yard' : {
        'size' : [3,2],
        'health' : 1000,
        'power_requirement' : 30,
        'view_range' : 5,
    },

    'silo' : 
    {
        'capacity' : 1000,
        'health' : 750,
        'picture_array' : 25,
        'power_req' : 5,
        'view_range' : 3,
        'size' : [2,2],
    },
    
    'starport' : {
        'health' : 3000,
        'power_requirement' : 80,
        'size' : [3,3],
        'view_range' : 6,
        # Some starport specific variables
        'price_modifier' : 1.0,
        #'modifierTimer' : STARPORT_CHANGE_PRICETIME,
        #define STARPORT_ARRIVETIME			5000
        #define STARPORT_CHANGE_PRICETIME	5000
        #define STARPORT_MIN_MOD			65
        #define STARPORT_MAX_MOD			125
  
    },
    
    'wall' : {
        'health' : 500,
        'size' : [1,1],
        'view_range' : 2,
    },
    
    'windtrap' : {
        'health' : 1000,
        'power_requirement' : 0,
        'size' : [2,2],
        'view_range' : 3,
    },
    
    'wor' : {
        'health' : 1500,
        'power_requirement' : 20,
        'size' : [2,2],
        'view_range' : 5,
    },
}
