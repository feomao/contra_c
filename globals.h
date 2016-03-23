#ifndef GLOBALS_H
#define GLOBALS_H

#include "sdl/include/sdl.h"
#include "sdl/include/SDL_mixer.h"

extern SDL_Surface *g_screen;
extern int g_keys[256];

//contra
#define EXT(v) (v<0x80)? v:((0x100-v)*-1)

typedef void (*SUB) ();

#define PLAYER_B SDLK_z
#define PLAYER_A SDLK_x
#define PLAYER_SEL SDLK_SPACE
#define PLAYER_ST SDLK_RETURN

#define PLAYER_RAPIDB SDLK_a
#define PLAYER_RAPIDA SDLK_s
#define RAPID_VAL 4

extern unsigned char main_proc;
extern unsigned char simple_f;
extern unsigned char framecount;
extern unsigned char blank_screens;
extern unsigned short msg700_ptr;				//$21
extern char lives_setting;
extern bool pause;						//$25
extern bool in_game;
extern unsigned char title_delay;
extern unsigned char delay_before_go;			//$29
extern unsigned short long_delay;
extern unsigned char game_proc;
extern unsigned char sclear_proc;			//$2C
extern unsigned char diff_extra;				//$2F
extern unsigned char stage;
extern unsigned char num_of_allclears;			//$31
extern unsigned char colors_to_update;
extern unsigned char room_clear;				//$37
extern unsigned char stage_clear;				//$3B
extern unsigned char konamicode_idx;
extern unsigned char zstage_f;				//$40
extern unsigned char vstage_f;				//$41
extern unsigned char *screens_blocks_table;	//$42,43
extern unsigned char *screens_bg_table;
extern unsigned char *tiles_of_blocks;			//$44,45
extern unsigned char *attribs_of_blocks;		//$46,47
extern unsigned char page_of_bgswitch;			//$48		
extern unsigned char tjuge[3];					//$49		
extern unsigned char bgcg3_spec[4];			//$4C-4F
extern unsigned char palcgs_spec[8];			//$50-57
extern unsigned char screens_of_stage;			//$58
extern unsigned char through_solid;			//$59
extern unsigned char cr_no_to_add;				//$60
extern unsigned char cols_to_add;				//$61
extern unsigned char rows_to_add;
extern unsigned short v_addr_to_add;			//$62,63
extern unsigned char screen_page;				//$64
extern unsigned char screen_off;				//$65
extern unsigned short a_addr_to_add;			//$66,67
extern int scroll_to_do;						//$68
extern unsigned char ntable_off600;			//$69
extern bool show_status;
extern int go_option;
extern unsigned char bgswitch_reached;					//$71
extern unsigned char cg23_idx;					//$72
extern unsigned char mag_screen;				//$73
extern unsigned char bright_count;			//$74
extern unsigned char boss_ascroll;				//$75
extern unsigned char demon_ascroll;			//$76
extern unsigned char tank_ascroll;				//$77
extern unsigned char disable_cg23;				//$78
extern unsigned char random_e_gen_state;		//$79
extern short random_e_delay;					//$7A
extern int random_e_x;							//$7B
extern int random_e_y;							//$7C
extern unsigned char flash_cnt;				//$7D
extern unsigned char btank_active;				//$7F
extern SUB** stage_emy_jtables;					//$80,81
extern unsigned char emydata_ptr;			//$82
extern bool boss_ascroll_done;					//$84
extern unsigned char arms_clred;				//$85
extern unsigned char s_robject_left;			//$86
extern unsigned char b_robject_clred;			//$87
extern unsigned char enemy_rounds;				//$88
extern unsigned char bjumper_appeared;			//$89
extern unsigned char grenadier_active;			//$8A
extern unsigned char homing_delaytbl_idx;		//$8B
extern unsigned char player_normal;			//$8E
extern short hscroll, vscroll;	//$FC, $FD
extern unsigned char vcontol_v;				//$FE & $FF
extern unsigned char random_e_crt_page;		//$0195
extern unsigned char random_e_cnt;			//$0196
extern unsigned char disp_forms[0x1A];
extern int chr_ys[0x1A];			//$031A
extern int chr_xs[0x1A];			//$0334
extern unsigned char disp_attr[0x1A];			//$034E
extern unsigned char vbuf600[0x80];			//$0600
extern unsigned char bgbuf680[0x80];			//$0680
extern unsigned char msg700[0x200];
extern unsigned char pal_buf[0x20];
extern unsigned short hi_score;					//$07E0

extern FILE* fherr;

extern unsigned char *contra_gfx;

extern int bossbgm;

extern unsigned char bg_val;
extern unsigned char bg_toff;
extern unsigned char bg_bitno;
extern unsigned char bg_plat_f;
extern unsigned char bg_solid_f;
extern unsigned char bg_empty_f;

extern int bgobj_addr;

void update_pals(void);
void proc_msg700(void);
void read_keys();
void disp_objs();
void gfx_setting(int setidx);
void gfx_setop(int op, int clr_f);
void clear_vars();
void pre_title_init();
void pre_game_init(bool full);
void write_msg(int msg);
void init_stage();
void add_blks600(int page);
void add_bgs680(int page);
void update_pal_buf(int colors);
int initial_add_screen();
void add_1col();
void add_1col_attr();
void add_1lin();
void add_1lin_attr();
void check_pause();
void show_p_bullets();
unsigned char check_bg(int x, int y, int xovr);
int solid_or_solidunderplat(int x, int y);
unsigned char read_bg(int y, unsigned char off);
int solid_or_solidunderplat_cont();
int solidunderplat();
int can_scroll();
void show_player();
void switch_bgvars();
void scroll_addcolrow();
void color_op();
void chang_boss_tiles();
void p_bullet_vanish(int no);
void show_bgobj_4x4(int x, int y, int bno);
void show_bgobj_2xn(int x, int y, int bno);
void set_bgval_4x4(int v20, int v31);
void clr_bgval_blks2();
void stageclear_init(int sndno);
void stageclear_proc();

//data
extern unsigned char* spdata[];
extern unsigned char gfx_settbl[];
extern unsigned char gfx_optbl[];
extern int gfx_movtbl[];
extern unsigned char* msgdata[];
extern unsigned char stage_vars[];
extern unsigned char screens_blocks_data[];
extern unsigned char screens_bg_data[];
extern unsigned char stage_tiles_data[];
extern unsigned char stage_attrs_data[];
extern unsigned char colors_table[];
extern unsigned char bgswitch_tbl[];
extern unsigned char *enemy_cmm_idata[];
extern unsigned char **enemy_gdatas[];
extern unsigned char col_detect_data[];
extern unsigned char cd_dyn_data0[];
extern unsigned char cd_dyn_data1[];
extern unsigned char *bgobj_4x4_tiles[];
extern unsigned char *bgobj_4x4_attrs[];
extern unsigned char *bgobj_2xn_tbls[];
extern unsigned char *s1_t1518_gdatas[];
extern unsigned char *s3_t1518_gdatas[];
extern unsigned char **t1518_gdatas[];
extern unsigned char *emsgs[];
extern unsigned char *re_gdatas[];

#define MAX_SOUNDS 30

extern Mix_Chunk* g_sound[];
#define PLAYSOUND0(n) Mix_PlayChannel(0,g_sound[n],0)
#define PLAYSOUND1(n) Mix_PlayChannel(1,g_sound[n],0)
#define PLAYSOUND2(n) Mix_PlayChannel(2,g_sound[n],0)
#define PLAYSOUND3(n) Mix_PlayChannel(3,g_sound[n],0)
#define PLAYSOUND4(n) Mix_PlayChannel(4,g_sound[n],0)

#define STOPSOUND(n) Mix_HaltChannel(n)


#define TITLE_SND		0
#define PAUSE_SND		1
#define P_LANDING		2
#define P_SHOCK			3
#define P_DEATH			4
#define N_GUN			5
#define M_GUN			6
#define F_GUN			7
#define S_GUN			8
#define L_GUN			9
#define P_1UP			10
#define BONUS			11
#define HITSND0			12
#define HITSND1			13
#define HITSND2			14
#define BOMBING0		15
#define BOMBING1		16
#define STAGE_CLEAR		17
#define BOMBING2		18
#define STONE_LANDING	19
#define PIPEBOMB		20
#define FLAME			21
#define GAMEOVER		22
#define ALARM			23
#define BOMBING3		24
#define MOTOR			25
#define BOMBING4		26
#define ROBOT_LANDING	27
#define AIRPLANE_MOTOR	28
#define ENDING			29
#endif
