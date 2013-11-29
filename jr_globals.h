const int MAX_THROWN_OBJECTS=3;

const int PLAYER_STATE_IDLE=0;
const int PLAYER_STATE_HIT=1;
const int PLAYER_STATE_JUMPING=2;
const int PLAYER_STATE_WALKING=3;
const int PLAYER_STATE_FALLING=4;
const int PLAYER_STATE_ATTACK=5;
const int PLAYER_STATE_PUNSHING=6;
const int PLAYER_STATE_HEALING=7;
const int PLAYER_STATE_ROLLING=8;
const int PLAYER_STATE_SPECIAL=9;

const int PLAYER_DIRECTION_LEFT=0;
const int PLAYER_DIRECTION_RIGHT=1;
const int PLAYER_DIRECTION_DOWN=2;

const int PLAYER_WALKING_STATE_NORMAL=0;
const int PLAYER_WALKING_STATE_RUNNING=1;

const int LAYER_BG=0;
const int LAYER_PLAYER=1;
const int LAYER_FG=2;

const int ENEMY_STATE_WALKING=0;
const int ENEMY_STATE_ATTACKING=1;
const int ENEMY_STATE_VANISHING=2;
const int ENEMY_STATE_THROWN_OBJECT=3;
const int ENEMY_STATE_SPECIAL1=4;
const int ENEMY_STATE_SPECIAL2=5;
const int ENEMY_STATE_SPECIAL3=6;
const int ENEMY_STATE_SPECIAL4=7;

const int ENEMY_DIRECTION_LEFT=0;
const int ENEMY_DIRECTION_RIGHT=1;
const int ENEMY_DIRECTION_DOWN=2;
const int ENEMY_DIRECTION_UP=3;

const bool ENEMY_WALKING_LEFT=false;
const bool ENEMY_WALKING_RIGHT=true;

const int ATTACKING_ON_TOUCH=1;
const int ATTACKING_REGULARY=2;

const bool DIRECTION_LEFT=false;
const bool DIRECTION_RIGHT=true;

const int ITEM_ACTION_TALK=-1;
const int ITEM_ACTION_ONE_UP=0;
const int ITEM_ACTION_HEAL=1;
const int ITEM_ACTION_ELEMENT_EARTH=2;
const int ITEM_ACTION_ELEMENT_ICE=3;
const int ITEM_ACTION_ELEMENT_WIND=4;
const int ITEM_ACTION_ELEMENT_FIRE=5;

const bool ITEM_ACTIVATE_INSTANT=true;
const bool ITEM_ACTIVATE_INVENTORY=false;

const int NO_ELEMENT=0;
const int ELEMENT_EARTH=1;
const int ELEMENT_ICE=2;
const int ELEMENT_WIND=3;
const int ELEMENT_FIRE=4;

const int SCROLL_DIRECTION_LEFT=0;
const int SCROLL_DIRECTION_RIGHT=1;
const int SCROLL_DIRECTION_UP=2;
const int SCROLL_DIRECTION_DOWN=3;

const int UPDATE_GAME_OVER=0;
const int UPDATE_CONTINUE=1;
const int UPDATE_FINISHED=2;
const int UPDATE_CONTINUE_PREVIOUS=3;

const int OBJECT_TYPE_MOVING_PLATFORM=0;
const int OBJECT_TYPE_FLOATING_PLATFORM=1;
const int OBJECT_TYPE_VANISHING=2;
const int OBJECT_TYPE_LIANA=3;
const int OBJECT_TYPE_FOUNTAIN_ROCK=4;
const int OBJECT_TYPE_CHEST=5;
const int OBJECT_TYPE_LADDER=6;
/*this object is really fucked up, i use the pos_y to determine both, the pos as well as the size, the pos is (pos_y/10)*10,
the size is pos_y%10 ;-) fucked up, aint it ;-)*/
const int OBJECT_TYPE_RISING_PLATFORM=7;

const int CHEST_CLOSED_NICE=0;
const int CHEST_CLOSED_RANDOM=1;
const int CHEST_OPEND=2;

const int FLOATING_DIRECTION_LEFT=0;
const int FLOATING_DIRECTION_RIGHT=1;

const bool MODE_LOAD=true;
const bool MODE_SAVE=false;

Mix_Music *menu_music;
Mix_Music *stage_music;

Mix_Chunk *jump_sound;
Mix_Chunk *hit_sound;
Mix_Chunk *got_item_sound;
Mix_Chunk *one_up_sound;
Mix_Chunk *enemy_death_sound;
Mix_Chunk *heal_sound;
Mix_Chunk *punsh_sound;
Mix_Chunk *attack_sound;
Mix_Chunk *thunder_sound;
