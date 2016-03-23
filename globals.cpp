#include "sdl/include/SDL_mixer.h"
#include "player.h"
#include "enemies.h"
#include "globals.h"

unsigned char main_proc=0;				//$18
unsigned char simple_f=0;				//$19
unsigned char framecount=0;				//$1A
unsigned char blank_screens=0;			//$20
unsigned short msg700_ptr;				//$21
bool in_game=false;						//$23
char lives_setting;
bool pause=false;						//$25
unsigned char demo_stage=0;				//$27
unsigned char title_delay=0;			//$28
unsigned char delay_before_go=0;			//$29
unsigned short long_delay=0;			//$2A, 2B
unsigned char game_proc=0;				//$2C
unsigned char sclear_proc=0;			//$2D
unsigned char diff_extra;				//$2F
unsigned char stage=0;						//$30
unsigned char num_of_allclears=0;			//$31
unsigned char colors_to_update=0;		//$36
unsigned char room_clear;				//$37
unsigned char stage_clear;				//$3B
unsigned char konamicode_idx=0;			//$3F
unsigned char zstage_f=0;				//$40
unsigned char vstage_f=0;				//$41
unsigned char *screens_blocks_table;	//$42,43
unsigned char *screens_bg_table;
unsigned char *tiles_of_blocks;			//$44,45
unsigned char *attribs_of_blocks;		//$46,47
unsigned char page_of_bgswitch;			//$48		
unsigned char tjuge[3];					//$49		
unsigned char bgcg3_spec[4];			//$4C-4F
unsigned char palcgs_spec[8];			//$50-57
unsigned char screens_of_stage;			//$58
unsigned char through_solid;			//$59
unsigned char cr_no_to_add;				//$60
unsigned char cols_to_add;				//$61
unsigned char rows_to_add;
unsigned short v_addr_to_add;			//$62,63
unsigned char screen_page;				//$64
unsigned char screen_off;				//$65
unsigned short a_addr_to_add;			//$66,67
int scroll_to_do;						//$68
unsigned char ntable_off600;			//$69
bool show_status=false;					//$6A
int go_option=0;
unsigned char bgswitch_reached;			//$71
unsigned char cg23_idx;					//$72
unsigned char mag_screen;				//$73
unsigned char bright_count;				//$74
unsigned char boss_ascroll;				//$75
unsigned char demon_ascroll;			//$76
unsigned char tank_ascroll;				//$77
unsigned char disable_cg23;				//$78
unsigned char random_e_gen_state;		//$79
short random_e_delay;					//$7A
int random_e_x;							//$7B
int random_e_y;							//$7C
unsigned char flash_cnt;				//$7D
unsigned char btank_active;				//$7F
SUB** stage_emy_jtables;				//$80,81
unsigned char emydata_ptr;				//$82
bool boss_ascroll_done;					//$84
unsigned char arms_clred;				//$85
unsigned char s_robject_left;			//$86
unsigned char b_robject_clred;			//$87
unsigned char enemy_rounds;				//$88
unsigned char bjumper_appeared;			//$89
unsigned char grenadier_active;			//$8A
unsigned char homing_delaytbl_idx;		//$8B
unsigned char player_normal;			//$8E
short hscroll=0, vscroll=0;				//$FC, $FD
unsigned char vcontol_v=0x18;			//$FE & $FF
unsigned char random_e_crt_page;		//$0195
unsigned char random_e_cnt;				//$0196
unsigned char disp_forms[0x1A];			//$0300
int chr_ys[0x1A];			//$031A
int chr_xs[0x1A];			//$0334
unsigned char disp_attr[0x1A];			//$034E
unsigned char vbuf600[0x80];			//$0600
unsigned char bgbuf680[0x80];			//$0680
unsigned char msg700[0x200];			//$0700
unsigned char pal_buf[0x20];			//$07C0
unsigned short hi_score;					//$07E0

FILE* fherr=0;

unsigned char *contra_gfx=0;

struct PLAYER player;
struct P_BULLET p_bullet[0x10];
struct ENEMY enemy[0x10];

int bossbgm;

unsigned char bg_val;
unsigned char bg_toff;
unsigned char bg_bitno;
unsigned char bg_plat_f;
unsigned char bg_solid_f;
unsigned char bg_empty_f;

int bgobj_addr;

Mix_Chunk* g_sound[MAX_SOUNDS];
