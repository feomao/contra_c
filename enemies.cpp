#include "globals.h"
#include "timing.h"
#include "sound.h"
#include "player.h"
#include "enemies.h"

static int				t_enemy_no;
static struct ENEMY		*t_penemy=0;
static unsigned char	*t_penemy_dform=0;
static int				*t_penemy_x=0;
static int				*t_penemy_y=0;
static unsigned char	*t_penemy_dattr=0;

static int				t_col_xcheck=0;
static int				t_col_ycheck=0;
static unsigned int		t_col_xdist=0;
static unsigned int		t_col_ydist=0;

void enemy_exec(int state);
void e_bonus_form();
int check_bonus_y8(int skip);
int check_bonus_xdelta(int delta);
int calc_xlevel7(int type, int x);
int check_emy_bounds(int xbound, int ybound);
void e_soldier_xspd();
void e_soldier_turnback();
unsigned char get_soldier_bullets();
void e_soldier_form();
void e_sniper_form();
int e_tank_form();
void e_cannon_form(int idx);
void set_r4p_stop_delay();
void zenemy_form();
void set_ze_pos_xspd(int idx);
void rfsh_ze_x_chk_bound();
void gen_rolling_g(int xlevel, int x, int y);
void gen_grenade();
void gen_zbullet();
void w_soldier_form();
void w_soldier_attr();
void e_stone_form();
int s2boss_mouth();
void e_seg_proc();
void seg_fire_proc();
void aim_segs();
void rotete_segs();
char rotete_segs_sub(int no, char sum_rforce);
void set_segs_pos();
void refresh_btankx();
int btank_deep_left();
void btank_color_by_hp();
void s4boss_lowhalf_form();
void s4boss_uphalf_form();
void s_ship_form();
void flameend_form();
int draw_flame(int idx);
void robot_motion_reset();
void robot_color();
int draw_m_arm();
void e_3wcannon_form();
int door_form();
void show_blks2(int x, int y, int blk1, int blk2);
void show_mouth_upperblks();
void show_mouth_lowerblk();
void show_blks_by_no(int no);
void s7emy_allclear();
void get_alien_delay_btwn_homing();
void get_spds_of_dir(int dir);
void flower_scorpion_form(int baseform);
void set_scorpion_hp_delay_yflip();
void scorpiongen_form();
void gen_scorpion();
void refresh_enemyxy();
void refresh_bonusxy();
int refresh_bonusy(int spd);
void enemy_selfclear();
void move_enemy_scroll();
void refresh_enemyactx();
void refresh_enemyx();
void refresh_enemyacty();
void refresh_enemyy();
void combine_line_oval_y();
unsigned char check_e_bg(int xdelta, int ydelta);
void enemy_y_regulate(int delta);
void enemy_common_init(int no, int type);
int find_free_emy();
int find_first6_free_emy();
int find_emy_sac();
void enemy_player_collide();
void enemy_pb_collide();
void col_detect(int mode);
void p_addscore_e_death();
void e_death_state(struct ENEMY *penemy);
void enemy_allclear();
void bombing_gen(int x, int y);
void bombing_init(int type);
void bombing_proc(int type);
void enemy_bgobj_4x4(int bno);
void enemy_bgobj_4x4xv2(int bno0, int bno1, int keep_bgval);
void enemy_bgobj_2xn(int bno);
void new_e_pos(int xdelta, int ydelta, int *x, int *y);
void rfsh_saucer_osc_xspd();
void rfsh_saucer_osc_yspd();
int get_p_e_xdist();
int get_p_e_ydist();
int fire_eb(int dirtype, int x,int y,int dir,int speedadj);
void fire_eb_aimed_dir18(int x, int y, int btype, int speedadj);
void get_eb_spd(int dir, int speedadj, int yxflag, short *xspd, short *yspd);
short adjust_eb_spd(short spd, int speedadj);
int get_dir_aimed(int x, int y, int dirmode, int *newdir);
int get_dir_aimed_adjust(int x, int y, int dirmode, int *newdir);
int get_dir_aimed_rotate(int x, int y, int dirmode, int *newdir);
int get_segdir_aimed_adjust(int segno, int *newdir);
int gen_new_enemy (int xdelta, int ydelta, int type);

void e_bonus_st1();
void e_bonus_st2();
void e_bonus_st3();
SUB e_bonus_jtbls[]={
	e_bonus_st1, e_bonus_st2, e_bonus_st3
};

void e_bullet_st1();
void e_bullet_st2();
void e_bullet_st3();
void e_bullet_st4();
SUB e_bullet_jtbls[]={e_bullet_st1, e_bullet_st2, e_bullet_st3, e_bullet_st4};

void e_bbox_st1();
void e_bbox_st2();
void e_bbox_st3();
void e_bbox_st4();
void e_bbox_st5();
void e_bbox_st6();
void e_bbox_st7();
void e_bbox_st8();
SUB e_bbox_jtbls[]={
	e_bbox_st1, e_bbox_st2, e_bbox_st3, e_bbox_st4, e_bbox_st5,
	e_bbox_st6, e_bbox_st7, e_bbox_st8
};

void e_saucer_st1();
void e_saucer_st2();
void e_saucer_st3();
SUB e_saucer_jtbls[]={e_saucer_st1, e_saucer_st2, e_saucer_st3};

void e_turret_st1();
void e_turret_st2();
void e_turret_st3();
void e_turret_st4();
void e_turret_st5();
void e_turret_st6();
void e_turret_st7();
void e_turret_st8();
void e_turret_st9();
void e_turret_sta();
SUB e_turret_jtbls[]={e_turret_st1, e_turret_st2, e_turret_st3, e_turret_st4,
					e_turret_st5, e_turret_st6, e_turret_st7, e_turret_st8,
					e_turret_st9, e_turret_sta
};

void e_soldier_st1();
void e_soldier_st2();
void e_soldier_st3();
void e_soldier_st4();
void e_soldier_st5();
void e_soldier_st6();
void e_soldier_st7();
void e_soldier_st8();
void e_soldier_st9();
void e_soldier_sta();
void e_soldier_stb();
SUB e_soldier_jtbls[]={e_soldier_st1, e_soldier_st2, e_soldier_st3, e_soldier_st4,
					e_soldier_st5, e_soldier_st6, e_soldier_st7, e_soldier_st8,
					e_soldier_st9, e_soldier_sta, e_soldier_stb
};

void e_sniper_st1();
void e_sniper_st2();
void e_sniper_st3();
void e_sniper_st4();
void e_sniper_st5();
void e_sniper_st6();
void e_sniper_st7();
void e_sniper_st8();
void e_sniper_st9();
SUB e_sniper_jtbls[]={e_sniper_st1, e_sniper_st2, e_sniper_st3, e_sniper_st4,
					e_sniper_st5, e_sniper_st6, e_sniper_st7, e_sniper_st8,
					e_sniper_st9
};

void e_tank_st1();
void e_tank_st2();
void e_tank_st3();
void e_tank_st4();
void e_tank_st5();
void e_tank_st6();
void e_tank_st7();
void e_tank_st8();
void e_tank_st9();
SUB e_tank_jtbls[]={e_tank_st1, e_tank_st2, e_tank_st3, e_tank_st4,
					e_tank_st5, e_tank_st6, e_tank_st7, e_tank_st8,
					e_tank_st9
};

void e_3wturret_st1();
void e_3wturret_st2();
void e_3wturret_st3();
void e_3wturret_st4();
void e_3wturret_st5();
void e_3wturret_st6();
void e_3wturret_st7();
void e_3wturret_st8();
SUB e_3wturret_jtbls[]={
	e_3wturret_st1, e_3wturret_st2, e_3wturret_st3, e_3wturret_st4, e_3wturret_st5,
	e_3wturret_st6, e_3wturret_st7, e_3wturret_st8
};

void e_b_robject_st1();
void e_b_robject_st2();
void e_b_robject_st3();
void e_b_robject_st4();
void e_b_robject_st5();
void e_b_robject_st6();
void e_b_robject_st7();
SUB e_b_robject_jtbls[]={
	e_b_robject_st1, e_b_robject_st2, e_b_robject_st3, e_b_robject_st4, e_b_robject_st5,
	e_b_robject_st6, e_b_robject_st7
};

void e_3wshell_st1();
void e_3wshell_st2();
void e_3wshell_st3();
void e_3wshell_st4();
void e_3wshell_st5();
void e_3wshell_st6();
void e_3wshell_st7();
void e_3wshell_st8();
void e_3wshell_st9();
SUB e_3wshell_jtbls[]={e_3wshell_st1, e_3wshell_st2, e_3wshell_st3, e_3wshell_st4,
					e_3wshell_st5, e_3wshell_st6, e_3wshell_st7, e_3wshell_st8,
					e_3wshell_st9
};

void e_scuba_st1();
void e_scuba_st2();
void e_scuba_st3();
void e_scuba_st4();
void e_scuba_st5();
void e_scuba_st6();
SUB e_scuba_jtbls[]={
	e_scuba_st1, e_scuba_st2, e_scuba_st3, e_scuba_st4, e_scuba_st5,
	e_scuba_st6
};

void e_gunner_st1();
void e_gunner_st2();
void e_gunner_st3();
void e_gunner_st4();
void e_gunner_st5();
void e_gunner_st6();
SUB e_gunner_jtbls[]={
	e_gunner_st1, e_gunner_st2, e_gunner_st3, e_gunner_st4, e_gunner_st5,
	e_gunner_st6
};

void e_shell_st1();
void e_shell_st2();
void e_shell_st3();
SUB e_shell_jtbls[]={
	e_shell_st1, e_shell_st2, e_shell_st3
};

void e_dummy_st1(){
	enemy_selfclear();
}
SUB e_dummy_jtbls[]={
	e_dummy_st1
};

SUB* common_emy_jtables[]={
	e_bonus_jtbls,
	e_bullet_jtbls,
	e_bbox_jtbls,
	e_saucer_jtbls,
	e_turret_jtbls,
	e_soldier_jtbls,
	e_sniper_jtbls,
	e_tank_jtbls,
	e_3wturret_jtbls,
	e_dummy_jtbls,
	e_b_robject_jtbls,
	e_3wshell_jtbls,
	e_scuba_jtbls,
	e_dummy_jtbls, 
	e_gunner_jtbls,
	e_shell_jtbls
};

void e_cannon_st1();
void e_cannon_st2();
void e_cannon_st3();
void e_cannon_st4();
void e_cannon_st5();
void e_cannon_st6();
SUB e_cannon_jtbls[]={
	e_cannon_st1, e_cannon_st2, e_cannon_st3, e_cannon_st4, e_cannon_st5,
	e_cannon_st6
};

void e_bridge_st1();
void e_bridge_st2();
void e_bridge_st3();
void e_bridge_st4();
void e_bridge_st5();
SUB e_bridge_jtbls[]={
	e_bridge_st1, e_bridge_st2, e_bridge_st3, e_bridge_st4, e_bridge_st5
};

void e_s0boss_st1();
void e_s0boss_st2();
void e_s0boss_st3();
void e_s0boss_st4();
void e_s0boss_st5();
void e_s0boss_st6();
void e_s0boss_st7();
SUB e_s0boss_jtbls[]={
	e_s0boss_st1, e_s0boss_st2, e_s0boss_st3, e_s0boss_st4, e_s0boss_st5,
	e_s0boss_st6, e_s0boss_st7
};

SUB* stage0_emy_jtables[]={
	e_cannon_jtbls,
	e_s0boss_jtbls,
	e_bridge_jtbls
};


void e_s1boss_st1();
void e_s1boss_st2();
void e_s1boss_st3();
void e_s1boss_st4();
void e_s1boss_st5();
void e_s1boss_st6();
void e_s1boss_st7();
SUB e_s1boss_jtbls[]={
	e_s1boss_st1, e_s1boss_st2, e_s1boss_st3, e_s1boss_st4, e_s1boss_st5,
	e_s1boss_st6, e_s1boss_st7
};

void e_rolling_g_st1();
void e_rolling_g_st2();
void e_rolling_g_st3();
void e_rolling_g_st4();
void e_rolling_g_st5();
SUB e_rolling_g_jtbls[]={
	e_rolling_g_st1, e_rolling_g_st2, e_rolling_g_st3, e_rolling_g_st4,
	e_rolling_g_st5
};

void e_grenade_st1();
void e_grenade_st2();
void e_grenade_st3();
void e_grenade_st4();
void e_grenade_st5();
void e_grenade_st6();
SUB e_grenade_jtbls[]={
	e_grenade_st1, e_grenade_st2, e_grenade_st3, e_grenade_st4, e_grenade_st5,
	e_grenade_st6
};

void e_zturret_st1();
void e_zturret_st2();
void e_zturret_st3();
void e_zturret_st4();
void e_zturret_st5();
void e_zturret_st6();
void e_zturret_st7();
void e_zturret_st8();
SUB e_zturret_jtbls[]={
	e_zturret_st1, e_zturret_st2, e_zturret_st3, e_zturret_st4, e_zturret_st5,
	e_zturret_st6, e_zturret_st7, e_zturret_st8
};

void e_s_robject_st1();
void e_s_robject_st2();
void e_s_robject_st3();
void e_s_robject_st4();
void e_s_robject_st5();
void e_s_robject_st6();
void e_s_robject_st7();
void e_s_robject_st8();
void e_s_robject_st9();
void e_s_robject_sta();
SUB e_s_robject_jtbls[]={e_s_robject_st1, e_s_robject_st2, e_s_robject_st3, e_s_robject_st4,
					e_s_robject_st5, e_s_robject_st6, e_s_robject_st7, e_s_robject_st8,
					e_s_robject_st9, e_s_robject_sta
};

void e_runner_st1();
void e_runner_st2();
void e_runner_st3();
void e_runner_st4();
void e_runner_st5();
void e_runner_st6();
void e_runner_st7();
SUB e_runner_jtbls[]={
	e_runner_st1, e_runner_st2, e_runner_st3, e_runner_st4, e_runner_st5,
	e_runner_st6, e_runner_st7
};

void e_jumper_st1();
void e_jumper_st2();
void e_jumper_st3();
void e_jumper_st4();
void e_jumper_st5();
void e_jumper_st6();
void e_jumper_st7();
void e_jumper_st8();
SUB e_jumper_jtbls[]={
	e_jumper_st1, e_jumper_st2, e_jumper_st3, e_jumper_st4, e_jumper_st5,
	e_jumper_st6, e_jumper_st7, e_jumper_st8
};

void e_grenadier_st1();
void e_grenadier_st2();
void e_grenadier_st3();
void e_grenadier_st4();
void e_grenadier_st5();
void e_grenadier_st6();
void e_grenadier_st7();
SUB e_grenadier_jtbls[]={
	e_grenadier_st1, e_grenadier_st2, e_grenadier_st3, e_grenadier_st4, e_grenadier_st5,
	e_grenadier_st6, e_grenadier_st7
};

void e_runner4p_st1();
void e_runner4p_st2();
void e_runner4p_st3();
void e_runner4p_st4();
void e_runner4p_st5();
void e_runner4p_st6();
void e_runner4p_st7();
void e_runner4p_st8();
SUB e_runner4p_jtbls[]={
	e_runner4p_st1, e_runner4p_st2, e_runner4p_st3, e_runner4p_st4, e_runner4p_st5,
	e_runner4p_st6, e_runner4p_st7, e_runner4p_st8
};

void e_type1518gen_st1();
void e_type1518gen_st2();
void e_type1518gen_st3();
SUB e_type1518gen_jtbls[]={
	e_type1518gen_st1, e_type1518gen_st2, e_type1518gen_st3
};

void e_rolling_ggen_st1();
void e_rolling_ggen_st2();
void e_rolling_ggen_st3();
SUB e_rolling_ggen_jtbls[]={
	e_rolling_ggen_st1, e_rolling_ggen_st2, e_rolling_ggen_st3
};

void e_big_orb_st1();
void e_big_orb_st2();
void e_big_orb_st3();
void e_big_orb_st4();
void e_big_orb_st5();
SUB e_big_orb_jtbls[]={
	e_big_orb_st1, e_big_orb_st2, e_big_orb_st3, e_big_orb_st4, e_big_orb_st5
};

void e_s3boss_st1();
void e_s3boss_st2();
void e_s3boss_st3();
void e_s3boss_st4();
void e_s3boss_st5();
void e_s3boss_st6();
void e_s3boss_st7();
SUB e_s3boss_jtbls[]={
	e_s3boss_st1, e_s3boss_st2, e_s3boss_st3, e_s3boss_st4, e_s3boss_st5,
	e_s3boss_st6, e_s3boss_st7
};

void e_homingorb_st1();
void e_homingorb_st2();
void e_homingorb_st3();
void e_homingorb_st4();
void e_homingorb_st5();
SUB e_homingorb_jtbls[]={
	e_homingorb_st1, e_homingorb_st2, e_homingorb_st3, e_homingorb_st4, e_homingorb_st5
};

void e_w_soldier_st1();
void e_w_soldier_st2();
void e_w_soldier_st3();
void e_w_soldier_st4();
void e_w_soldier_st5();
void e_w_soldier_st6();
void e_w_soldier_st7();
SUB e_w_soldier_jtbls[]={
	e_w_soldier_st1, e_w_soldier_st2, e_w_soldier_st3, e_w_soldier_st4, e_w_soldier_st5,
	e_w_soldier_st6, e_w_soldier_st7
};

void e_r_sniper_st1();
void e_r_sniper_st2();
void e_r_sniper_st3();
void e_r_sniper_st4();
void e_r_sniper_st5();
void e_r_sniper_st6();
SUB e_r_sniper_jtbls[]={
	e_r_sniper_st1, e_r_sniper_st2, e_r_sniper_st3, e_r_sniper_st4, e_r_sniper_st5,
	e_r_sniper_st6
};

void e_type1e1fgen_st1();
void e_type1e1fgen_st2();
void e_type1e1fgen_st3();
SUB e_type1e1fgen_jtbls[]={
	e_type1e1fgen_st1, e_type1e1fgen_st2, e_type1e1fgen_st3
};

SUB* stage1_emy_jtables[]={
	e_s1boss_jtbls,
	e_rolling_g_jtbls,
	e_grenade_jtbls,
	e_zturret_jtbls,
	e_s_robject_jtbls,
	e_runner_jtbls,
	e_jumper_jtbls,
	e_grenadier_jtbls,
	e_runner4p_jtbls,
	e_type1518gen_jtbls,
	e_rolling_ggen_jtbls,
	e_big_orb_jtbls,
	e_s3boss_jtbls,
	e_homingorb_jtbls,
	e_w_soldier_jtbls,
	e_r_sniper_jtbls,
	e_type1e1fgen_jtbls,
};


void e_aircar_st1();
void e_aircar_st2();
SUB e_aircar_jtbls[]={e_aircar_st1, e_aircar_st2};

void e_fire_st1();
void e_fire_st2();
SUB e_fire_jtbls[]={
	e_fire_st1, e_fire_st2
};

void e_stonegen_st1();
void e_stonegen_st2();
void e_stonegen_st3();
SUB e_stonegen_jtbls[]={e_stonegen_st1, e_stonegen_st2, e_stonegen_st3};

void e_stone_st1();
void e_stone_st2();
void e_stone_st3();
void e_stone_st4();
void e_stone_st5();
void e_stone_st6();
SUB e_stone_jtbls[]={
	e_stone_st1, e_stone_st2, e_stone_st3, e_stone_st4, e_stone_st5,
	e_stone_st6
};

void e_s2boss_st1();
void e_s2boss_st2();
void e_s2boss_st3();
void e_s2boss_st4();
void e_s2boss_st5();
void e_s2boss_st6();
void e_s2boss_st7();
void e_s2boss_st8();
void e_s2boss_st9();
SUB e_s2boss_jtbls[]={e_s2boss_st1, e_s2boss_st2, e_s2boss_st3, e_s2boss_st4,
					e_s2boss_st5, e_s2boss_st6, e_s2boss_st7, e_s2boss_st8,
					e_s2boss_st9
};

void e_armseg_st1();
void e_armseg_st2();
void e_armseg_st3();
void e_armseg_st4();
void e_armseg_st5();
void e_armseg_st6();
void e_armseg_st7();
void e_armseg_st8();
SUB e_armseg_jtbls[]={
	e_armseg_st1, e_armseg_st2, e_armseg_st3, e_armseg_st4, e_armseg_st5,
	e_armseg_st6, e_armseg_st7, e_armseg_st8
};

SUB* stage2_emy_jtables[]={
	e_aircar_jtbls,
	e_fire_jtbls,
	e_stonegen_jtbls,
	e_stone_jtbls,
	e_s2boss_jtbls,
	e_armseg_jtbls
};

void e_pipebombgen_st1();
void e_pipebombgen_st2();
void e_pipebombgen_st3();
SUB e_pipebombgen_jtbls[]={e_pipebombgen_st1, e_pipebombgen_st2, e_pipebombgen_st3};

void e_pipebomb_st1();
void e_pipebomb_st2();
void e_pipebomb_st3();
void e_pipebomb_st4();
void e_pipebomb_st5();
SUB e_pipebomb_jtbls[]={
	e_pipebomb_st1, e_pipebomb_st2, e_pipebomb_st3, e_pipebomb_st4, e_pipebomb_st5
};

void e_btank_st1();
void e_btank_st2();
void e_btank_st3();
void e_btank_st4();
void e_btank_st5();
void e_btank_st6();
SUB e_btank_jtbls[]={
	e_btank_st1, e_btank_st2, e_btank_st3, e_btank_st4, e_btank_st5,
	e_btank_st6
};

void e_arc_st1();

SUB e_arc_jtbls[]={
	e_arc_st1
};


void e_s4boss_st1();
void e_s4boss_st2();
void e_s4boss_st3();
void e_s4boss_st4();
void e_s4boss_st5();
void e_s4boss_st6();
void e_s4boss_st7();
void e_s4boss_st8();
void e_s4boss_st9();
void e_s4boss_sta();
void e_s4boss_stb();
void e_s4boss_stc();
SUB e_s4boss_jtbls[]={e_s4boss_st1, e_s4boss_st2, e_s4boss_st3, e_s4boss_st4,
					e_s4boss_st5, e_s4boss_st6, e_s4boss_st7, e_s4boss_st8,
					e_s4boss_st9, e_s4boss_sta, e_s4boss_stb, e_s4boss_stc
};

void e_s_ship_st1();
void e_s_ship_st2();
void e_s_ship_st3();
void e_s_ship_st4();
void e_s_ship_st5();
void e_s_ship_st6();
void e_s_ship_st7();
SUB e_s_ship_jtbls[]={
	e_s_ship_st1, e_s_ship_st2, e_s_ship_st3, e_s_ship_st4, e_s_ship_st5,
	e_s_ship_st6, e_s_ship_st7
};

void e_fireball_st1();
void e_fireball_st2();
void e_fireball_st3();
void e_fireball_st4();
SUB e_fireball_jtbls[]={e_fireball_st1, e_fireball_st2, e_fireball_st3, e_fireball_st4};

SUB* stage4_emy_jtables[]={
	e_pipebombgen_jtbls,
	e_pipebomb_jtbls,
	e_btank_jtbls,
	e_arc_jtbls,
	e_s4boss_jtbls,
	e_s_ship_jtbls,
	e_fireball_jtbls
};

void e_flameud_st1();
void e_flameud_st2();
void e_flameud_st3();
void e_flameud_st4();
SUB e_flameud_jtbls[]={e_flameud_st1, e_flameud_st2, e_flameud_st3, e_flameud_st4};

void e_flamerl_st1();
void e_flamerl_st2();
void e_flamerl_st3();
void e_flamerl_st4();
SUB e_flamerl_jtbls[]={e_flamerl_st1, e_flamerl_st2, e_flamerl_st3, e_flamerl_st4};

void e_flamelr_st1();
void e_flamelr_st2();
void e_flamelr_st3();
void e_flamelr_st4();
SUB e_flamelr_jtbls[]={e_flamelr_st1, e_flamelr_st2, e_flamelr_st3, e_flamelr_st4};

void e_s5boss_st1();
void e_s5boss_st2();
void e_s5boss_st3();
void e_s5boss_st4();
void e_s5boss_st5();
void e_s5boss_st6();
void e_s5boss_st7();
void e_s5boss_st8();
void e_s5boss_st9();
void e_s5boss_sta();
SUB e_s5boss_jtbls[]={e_s5boss_st1, e_s5boss_st2, e_s5boss_st3, e_s5boss_st4,
					e_s5boss_st5, e_s5boss_st6, e_s5boss_st7, e_s5boss_st8,
					e_s5boss_st9, e_s5boss_sta
};

void e_discus_st1();
void e_discus_st2();
void e_discus_st3();
SUB e_discus_jtbls[]={
	e_discus_st1, e_discus_st2, e_discus_st3
};

SUB* stage5_emy_jtables[]={
	e_flameud_jtbls,
	e_flamerl_jtbls,
	e_flamelr_jtbls,
	e_s5boss_jtbls,
	e_discus_jtbls
};

void e_m_arm_st1();
void e_m_arm_st2();
void e_m_arm_st3();
void e_m_arm_st4();
SUB e_m_arm_jtbls[]={e_m_arm_st1, e_m_arm_st2, e_m_arm_st3, e_m_arm_st4};

void e_s_wall_st1();
void e_s_wall_st2();
void e_s_wall_st3();
void e_s_wall_st4();
void e_s_wall_st5();
void e_s_wall_st6();
SUB e_s_wall_jtbls[]={
	e_s_wall_st1, e_s_wall_st2, e_s_wall_st3, e_s_wall_st4, e_s_wall_st5,
	e_s_wall_st6
};

void e_s_twall_st1();
void e_s_twall_st2();
void e_s_twall_st3();
void e_s_twall_st4();
SUB e_s_twall_jtbls[]={e_s_twall_st1, e_s_twall_st2, e_s_twall_st3, e_s_twall_st4};

void e_m_cargen_st1();
void e_m_cargen_st2();
SUB e_m_cargen_jtbls[]={e_m_cargen_st1, e_m_cargen_st2};

void e_m_car_st1();
void e_m_car_st4();
void e_m_car_st5();
void e_m_car_st6();
SUB e_m_car_jtbls[]={
	e_m_car_st1, e_m_car_st1, e_m_car_st1, e_m_car_st4, e_m_car_st5,
	e_m_car_st6
};

void e_car_st1();
void e_car_st2();
void e_car_st3();
void e_car_st4();
void e_car_st5();
void e_car_st6();
SUB e_car_jtbls[]={
	e_car_st1, e_car_st2, e_car_st3, e_car_st4, e_car_st5,
	e_car_st6
};

void e_s6boss_st1();
void e_s6boss_st2();
void e_s6boss_st3();
void e_s6boss_st4();
void e_s6boss_st5();
void e_s6boss_st6();
void e_s6boss_st7();
SUB e_s6boss_jtbls[]={
	e_s6boss_st1, e_s6boss_st2, e_s6boss_st3, e_s6boss_st4, e_s6boss_st5,
	e_s6boss_st6, e_s6boss_st7
};

void e_3wcannon_st1();
void e_3wcannon_st2();
void e_3wcannon_st3();
void e_3wcannon_st4();
void e_3wcannon_st5();
void e_3wcannon_st6();
void e_3wcannon_st7();
void e_3wcannon_st8();
SUB e_3wcannon_jtbls[]={
	e_3wcannon_st1, e_3wcannon_st2, e_3wcannon_st3, e_3wcannon_st4, e_3wcannon_st5,
	e_3wcannon_st6, e_3wcannon_st7, e_3wcannon_st8
};

void e_soldiergen_st1();
void e_soldiergen_st2();
void e_soldiergen_st3();
void e_soldiergen_st4();
void e_soldiergen_st5();
SUB e_soldiergen_jtbls[]={
	e_soldiergen_st1, e_soldiergen_st2, e_soldiergen_st3, e_soldiergen_st4, e_soldiergen_st5
};

SUB* stage6_emy_jtables[]={
	e_m_arm_jtbls,
	e_s_wall_jtbls,
	e_s_twall_jtbls,
	e_m_cargen_jtbls,
	e_m_car_jtbls,
	e_car_jtbls,
	e_s6boss_jtbls,
	e_3wcannon_jtbls,
	e_soldiergen_jtbls
};


void e_demon_st1();
void e_demon_st2();
void e_demon_st3();
void e_demon_st4();
void e_demon_st5();
void e_demon_st6();
void e_demon_st7();
void e_demon_st8();
void e_demon_st9();
void e_demon_sta();
void e_demon_stb();
void e_demon_stc();
void e_demon_std();
SUB e_demon_jtbls[]={e_demon_st1, e_demon_st2, e_demon_st3, e_demon_st4,
					e_demon_st5, e_demon_st6, e_demon_st7, e_demon_st8,
					e_demon_st9, e_demon_sta, e_demon_stb, e_demon_stc,
					e_demon_std
};

void e_alien_st1();
void e_alien_st2();
void e_alien_st3();
void e_alien_st4();
void e_alien_st5();
SUB e_alien_jtbls[]={
	e_alien_st1, e_alien_st2, e_alien_st3, e_alien_st4, e_alien_st5
};

void e_flowergen_st1();
void e_flowergen_st2();
void e_flowergen_st3();
void e_flowergen_st4();
void e_flowergen_st5();
void e_flowergen_st6();
SUB e_flowergen_jtbls[]={
	e_flowergen_st1, e_flowergen_st2, e_flowergen_st3, e_flowergen_st4, e_flowergen_st5,
	e_flowergen_st6
};

void e_flower_st1();
void e_flower_st2();
void e_flower_st3();
void e_flower_st4();
void e_flower_st5();
void e_flower_st6();
SUB e_flower_jtbls[]={
	e_flower_st1, e_flower_st2, e_flower_st3, e_flower_st4, e_flower_st5,
	e_flower_st6
};

void e_scorpion_st1();
void e_scorpion_st2();
void e_scorpion_st3();
void e_scorpion_st4();
void e_scorpion_st5();
void e_scorpion_st6();
void e_scorpion_st7();
void e_scorpion_st8();
SUB e_scorpion_jtbls[]={
	e_scorpion_st1, e_scorpion_st2, e_scorpion_st3, e_scorpion_st4, e_scorpion_st5,
	e_scorpion_st6, e_scorpion_st7, e_scorpion_st8
};

void e_scorpiongen_st1();
void e_scorpiongen_st2();
void e_scorpiongen_st3();
void e_scorpiongen_st4();
void e_scorpiongen_st5();
void e_scorpiongen_st6();
SUB e_scorpiongen_jtbls[]={
	e_scorpiongen_st1, e_scorpiongen_st2, e_scorpiongen_st3, e_scorpiongen_st4, e_scorpiongen_st5,
	e_scorpiongen_st6
};

void e_s7boss_st1();
void e_s7boss_st2();
void e_s7boss_st3();
void e_s7boss_st4();
void e_s7boss_st5();
void e_s7boss_st6();
void e_s7boss_st7();
void e_s7boss_st8();
SUB e_s7boss_jtbls[]={
	e_s7boss_st1, e_s7boss_st2, e_s7boss_st3, e_s7boss_st4, e_s7boss_st5,
	e_s7boss_st6, e_s7boss_st7, e_s7boss_st8
};

SUB* stage7_emy_jtables[]={
	e_demon_jtbls,
	e_alien_jtbls,
	e_flowergen_jtbls,
	e_flower_jtbls,
	e_scorpion_jtbls,
	e_scorpiongen_jtbls,
	e_s7boss_jtbls
};

SUB** stages_emy_jtables[]={
	stage0_emy_jtables, stage1_emy_jtables, stage2_emy_jtables, stage1_emy_jtables, 
	stage4_emy_jtables, stage5_emy_jtables, stage6_emy_jtables, stage7_emy_jtables, 
};

void init_stage_emy_jtables(){
	stage_emy_jtables =stages_emy_jtables[stage];
}

void enemy_proc(){
	int i;
	unsigned char c, idx;

	player.on_car=0;
	for (i=0xf; i>=0; i--){
		t_penemy=&enemy[i];
		c=t_penemy->state;
		if (c){
			t_enemy_no=i;
			idx =0xa+i;
			t_penemy_dform =&disp_forms[idx];
			t_penemy_x =&chr_xs[idx];
			t_penemy_y =&chr_ys[idx];
			t_penemy_dattr=&disp_attr[idx];
			enemy_exec(--c);
			if (*t_penemy_dform){
				if (!(zstage_f&1) ||*t_penemy_y>=0x9c){
					if (!(t_penemy->flag1 & ENEMY_NONCOL))
						enemy_player_collide();
				}
			}
			if (!(t_penemy->flag1 & ENEMY_NONSHOOTABLE))
				enemy_pb_collide();
		}
	}
}

void enemy_exec(int state_m1){
	SUB* pe_jtbls;

	if (t_penemy->type<0x10){
		pe_jtbls =common_emy_jtables[t_penemy->type];
	}else{
		pe_jtbls =stage_emy_jtables[t_penemy->type-0x10];
	}
	pe_jtbls[state_m1]();
}


//	00
void e_bonus_st1(){
	static short z_xspds[]={0xaa, 0x71, 0x38, 0x0, -0x38, -0x71, -0xaa};
	int idx;

	t_penemy->flag1 =ENEMY_NONSHOOTABLE;
	t_penemy->form_idx=0;
	t_penemy->score_idx=2;
	t_penemy->col_type=2;
	*t_penemy_dattr =5;
	if (zstage_f &1){
		t_penemy->line_ylocus =*t_penemy_y;
		idx =calc_xlevel7(0, *t_penemy_x);
		t_penemy->xspeed =z_xspds[idx];
		t_penemy->yspeed =0x0100;
		t_penemy->oval_yspd =-0x0280;
	}else{
		if (vstage_f)
			t_penemy->xspeed =(*t_penemy_x<0x80) ?0x0040 : -0x0040;
		else
			t_penemy->xspeed =0x0080;
		t_penemy->yspeed =-0x0300;
	}
	t_penemy->state++;
}

void e_bonus_st2(){
	e_bonus_form();
	if (zstage_f){
		t_penemy->oval_yspd +=0x12;
		combine_line_oval_y();
		if (t_penemy->oval_ylocus>=0){
			*t_penemy_y=0xa4;
			t_penemy->state++;
		}
	}else{
		refresh_bonusxy();
		if (check_bonus_y8(0)){
			enemy_y_regulate(0xa);
			t_penemy->xspeed =0;  t_penemy->yspeed =0;
			t_penemy->state++;
		}else{
			if (t_penemy->xspeed>=0){
				if (*t_penemy_x>=0xe8 || check_bonus_xdelta(0xa))
					t_penemy->xspeed =-t_penemy->xspeed;
			}else{
				if (*t_penemy_x<0x18 || check_bonus_xdelta(-0xa))
					t_penemy->xspeed =-t_penemy->xspeed;
			}
			t_penemy->yspeed +=0x10;
		}
	}
}

void e_bonus_st3(){
	e_bonus_form();
	if (zstage_f){
		if (screen_off)
			enemy_selfclear();
	}else{
		move_enemy_scroll();
		if (!check_bonus_y8(1)){
			if (t_penemy->state)
				t_penemy->state=2;
		}
	}
}

void e_bonus_form(){
	static unsigned char forms[]={0x33,0x34,0x31,0x2f,0x32,0x30,0x4e};
	int type;

	if (t_penemy->form_idx)
		*t_penemy_dform=0;
	else{
		type =t_penemy->flag2 & ENEMY_BONUSTYPE;
		if (type==6)
			*t_penemy_dattr= ((framecount>>3)&3) | 4;
		*t_penemy_dform =forms[type];
	}
}

int check_bonus_y8(int skip){
	if (!skip && *t_penemy_y<0x20)
		return 0;
	if (t_penemy->form_idx || (t_penemy->yspeed<0))
		return 0;
	check_e_bg(0,8);
	if (bg_empty_f)
		return 0;
	return 1;
}

int check_bonus_xdelta(int delta){
	int x =*t_penemy_x+delta, y=*t_penemy_y;
	if (through_solid==0)
		return 0;
	if (t_penemy->form_idx || y<0x10)
		y=0x10;
	return solid_or_solidunderplat(x,y);
}

int calc_xlevel7(int type, int x){
	static unsigned char xjuges0[]={0xff,0x94,0x8c,0x84,0x7c,0x74,0x6c};
	static unsigned char xjuges1[]={0xff,0xbc,0xa4,0x8c,0x74,0x5c,0x44};
	int i;
	unsigned char *pxjuges =(type) ?xjuges1 :xjuges0;

	for (i=6; i>=0; i--){
		if (x<pxjuges[i])
			break;
	}
	if (i<0)
		return 0;
	else
		return i;
}

//	01
void e_bullet_st1(){
	static unsigned char ctypes[]={
		1,5,5,1,2,0
	};

	t_penemy->col_type =ctypes[t_penemy->bullet_type];
	t_penemy->state++;
}

void e_bullet_st2(){
	static unsigned char forms[]={0x1e,0x21,0x21,0x1e,0x79,0x07};
	static unsigned char attrs[]={0x01,0x02,0x02,0x01,0x01,0x02};
	static unsigned char fb_attrs[]={0x01,0x41,0xc1,0x81};
	int idx=t_penemy->bullet_type;

	if (!idx && stage==4)
		idx=5;
	*t_penemy_dform=forms[idx];
	*t_penemy_dattr=attrs[idx];
	refresh_enemyxy();
	if (!t_penemy->bullet_type && (through_solid & 0x80)){
		check_e_bg(0,0);
		solidunderplat();
		if (bg_solid_f)
			enemy_selfclear();
	}
	switch (t_penemy->bullet_type){
	case 0:
	case 2:
		break;
	case 1:
		t_penemy->yspeed +=0x14;
		if (*t_penemy_y>=0xd0){
			t_penemy->form_idx=0;
			t_penemy->delay=1;
			if (t_penemy->state)
				t_penemy->state++;
		}
		break;
	case 3:
		if (*t_penemy_y>=0xb4 || *t_penemy_x<0x20 || *t_penemy_x>=0xe0)
			enemy_selfclear();
		break;
	case 4:
		*t_penemy_dattr =fb_attrs[(framecount>>2) &3];
		break;
	default:
		break;
	}
}

void e_bullet_st3(){
	static unsigned char forms[]={0x37,0x36,0x37};
	move_enemy_scroll();
	if (--t_penemy->delay)
		return;
	t_penemy->delay=8;
	t_penemy->form_idx++;
	if (t_penemy->form_idx<3)
		*t_penemy_dform=forms[t_penemy->form_idx];
	else
		if (t_penemy->state)
			t_penemy->state++;
}

void e_bullet_st4(){
	enemy_selfclear();
}

//	02
void e_bbox_st1(){
	t_penemy->form_idx=1;
	t_penemy->delay=0x20;
	t_penemy->state++;
}

void e_bbox_st2(){
	move_enemy_scroll();
	if (!check_emy_bounds(0xf0, 0x30))
		return;
	if (check_emy_bounds(0x18, 0xc8)){
		if (t_penemy->state)
			t_penemy->state=4;
		return;
	}
	if (--t_penemy->delay)
		return;
	t_penemy->delay=8;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_bbox_st3(){
	move_enemy_scroll();
	if (check_emy_bounds(0x18, 0xc8)){
		if (t_penemy->state)
			t_penemy->state=4;
		return;
	}
	if (--t_penemy->delay)
		return;
	t_penemy->delay=8;
	enemy_bgobj_4x4(t_penemy->form_idx);
	if (t_penemy->motion_state){
		if (t_penemy->form_idx){
			t_penemy->form_idx--;
			return;
		}else{
			++t_penemy->form_idx;
			t_penemy->hp=0xf0;
			t_penemy->motion_state--;
		}
	}else{
		if (t_penemy->form_idx<2){
			t_penemy->form_idx++;
			return;
		}else{
			--t_penemy->form_idx;
			t_penemy->hp=1;
			t_penemy->motion_state++;
		}
	}
	t_penemy->delay=0x40;
	if (t_penemy->state)
		t_penemy->state=2;
}

void e_bbox_st4(){
	move_enemy_scroll();
	enemy_bgobj_4x4(0);
	enemy_selfclear();
}

void e_bbox_st5(){
	static unsigned char rforms[]={
		0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x19,0x1a,0x03,0x04,0x09,0x09,0x16,0x16,
	};
	int idx =stage<<1;
	int nx,ny;

	move_enemy_scroll();
	if (t_penemy->flag2 & ENEMY_BBOX_BGTYPE1)
		idx++;
	enemy_bgobj_4x4( rforms[idx]);
	PLAYSOUND3(BOMBING0);
	new_e_pos (0,0, &nx, &ny);
	bombing_gen(nx,ny);
	t_penemy->flag2 &=7;
	t_penemy->oval_ylocus=0;
	t_penemy->state=1;
	t_penemy->type=0;
}

void e_bbox_st6(){
	bombing_init(0);
}

void e_bbox_st7(){
	bombing_proc(0);
}

void e_bbox_st8(){
	move_enemy_scroll();
	enemy_selfclear();
}

int check_emy_bounds(int xbound, int ybound){
	if (vstage_f){
		if (*t_penemy_y>=ybound)
			return 1;
	}else{
		if (*t_penemy_x<xbound)
			return 1;
	}
	return 0;
}

//	03
void e_saucer_st1(){
	static short spds[]={0, 0x180, -0x180, 0};
	int idx;

	*t_penemy_dattr=3;
	t_penemy->basey =*t_penemy_y;
	t_penemy->basex =*t_penemy_x;
	if (vstage_f){
		*t_penemy_x +=0x20;
		*t_penemy_y =0xe0;
		idx=2;
	}else{
		*t_penemy_y +=0x20;
		*t_penemy_x =0x10;
		idx=0;
	}
	t_penemy->yspeed =spds[idx++];
	t_penemy->xspeed =spds[idx];
	t_penemy->state++;
}

void e_saucer_st2(){
	*t_penemy_dform =0x4d;
	if (vstage_f){
		rfsh_saucer_osc_xspd();
	}else{
		rfsh_saucer_osc_yspd();
	}
	refresh_enemyxy();
}

void e_saucer_st3(){
	int nx,ny;

	PLAYSOUND3(BOMBING0);
	new_e_pos (0,0, &nx, &ny);
	bombing_gen(nx,ny);
	t_penemy->flag2 &=7;
	t_penemy->oval_ylocus=0;
	t_penemy->state=1;
	t_penemy->type=0;
}

void e_turret_st1(){
	move_enemy_scroll();
	t_penemy->aimed_dir=6;
	t_penemy->state++;
}

void e_turret_st2(){
	move_enemy_scroll();
	if (!check_emy_bounds(0xf0, 0x30))
		return;
	t_penemy->delay=8;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_turret_st3(){
	move_enemy_scroll();
	if (--t_penemy->delay)
		return;
	t_penemy->delay=8;
	enemy_bgobj_4x4(t_penemy->form_idx+3);
	if (++t_penemy->form_idx==3){
		t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
		t_penemy->delay=8;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_turret_st4(){
	static unsigned char delays[]={0x30,0x28,0x20,0x18};
	static char bullets[]={
		1,2,3,3
	};
	int x, y, dir_ok, dummy, bno;

	move_enemy_scroll();
	if (check_emy_bounds(0x18, 0xc8)){
		if (t_penemy->state){
			t_penemy->state=6;
			return;
		}
	}
	if (--t_penemy->delay)
		return;
	t_penemy->delay=delays[diff_extra];
	new_e_pos (0,0, &x, &y);
	dir_ok=get_dir_aimed_rotate(x, y, 0, &dummy);
	bno=t_penemy->aimed_dir-6;
	if (bno<0)
		bno+=0xc;
	bno+=5;
	enemy_bgobj_4x4(bno);
	if (dir_ok){
		t_penemy->bullets=bullets[t_penemy->flag2 & ENEMY_BULLETS_TYPE];
		t_penemy->delay=8;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_turret_st5(){
	static unsigned char delays[]={0x80,0x60,0x40,0x30};
	static int deltas[]={
		0x00,0x07,0x0c,0x0d,0x0c,0x07,0x00,-0x7,-0xc,-0xd,-0xc,-0x7,0x00,0x07,0x0c
	};
	int bx,by;

	move_enemy_scroll();
	if (check_emy_bounds(0x18, 0xc8)){
		if (t_penemy->state){
			t_penemy->state=6;
			return;
		}
	}
	if (--t_penemy->delay)
		return;
	by=*t_penemy_y+deltas[t_penemy->aimed_dir];
	bx=*t_penemy_x+deltas[t_penemy->aimed_dir+3];
	fire_eb(0, bx,by,t_penemy->aimed_dir,4);
	t_penemy->delay=0x10;
	if (--t_penemy->bullets==0){
		t_penemy->delay=delays[diff_extra];
		if (t_penemy->state){
			t_penemy->state=4;
			return;
		}
	}
}

void e_turret_st6(){
	move_enemy_scroll();
	enemy_bgobj_4x4(3);
	enemy_selfclear();
}

void e_turret_st7(){
	move_enemy_scroll();
	enemy_bgobj_4x4(0x16);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_turret_st8(){
	bombing_init(0);
}

void e_turret_st9(){
	bombing_proc(0);
}

void e_turret_sta(){
	move_enemy_scroll();
	enemy_selfclear();
}


//	05
void e_soldier_st1(){
	static unsigned char dtbl[]={0x01,0x10,0x20,0x30};
	move_enemy_scroll();
	enemy_y_regulate(4);
	t_penemy->delay =dtbl[(t_penemy->flag2>>4)&3];
	if (t_penemy->state)
		t_penemy->state++;
}

void e_soldier_st2(){
	unsigned char c;

	if (vstage_f){
		move_enemy_scroll();
		if (--t_penemy->delay)
			return;
	}else{
		if (scroll_to_do){
			if(t_penemy->flag2 & ENEMY_LEFT_SOLDIER){
				if (!(framecount & 1) || --t_penemy->delay)
					return;
			}else{
				if (--t_penemy->delay && --t_penemy->delay)
					return;
			}
		}else{
			if (--t_penemy->delay)
				return;
		}
	}
	check_e_bg(0,0x10);
	if (bg_empty_f){
		enemy_selfclear();
		return;
	}
	t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	c=t_penemy->flag2 & ENEMY_LEFT_SOLDIER;
	t_penemy->left =c;
	if (c)
		*t_penemy_x=0xa;
	e_soldier_xspd(); t_penemy->yspeed =0;
	t_penemy->delay =0x10;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_soldier_st3(){
	static int spdidx[]={0,0,2,0, 2,0,2,2};
	static short spds[]={-0x200, -0xb8, -0x100, -0xa0};
	int delta, idx, xbound;
	short *pspds;

	if (t_penemy->air_jump_f){
		t_penemy->form_idx=0xa;
		if (t_penemy->yspeed<0){
			t_penemy->yspeed +=0x10;
		}else{
			check_e_bg(0, 0x10);
			if (bg_solid_f || bg_plat_f){
				t_penemy->air_jump_f=0;
				t_penemy->form_idx=0;
				enemy_y_regulate(4);
				e_soldier_xspd(); t_penemy->yspeed =0;
			}else{
				if (bg_val==2)
					t_penemy->state=0xa;
				t_penemy->yspeed +=0x10;
			}
		}
	}else if (t_penemy->flag2 & (ENEMY_SOLDIER_FIRETYPE0 |ENEMY_SOLDIER_FIRETYPE1) &&
				player_normal && --t_penemy->delay==0){
		t_penemy->delay=0x80;
		t_penemy->delay_btwn_fire=8;
		t_penemy->bullets =get_soldier_bullets();
		if (t_penemy->state)
			t_penemy->state++;
		e_soldier_form();
		refresh_enemyxy();
		return;
	}else{
		if (!(++t_penemy->form_cnt &7)){
			if (++t_penemy->form_idx>=6)
				t_penemy->form_idx=0;
		}
		check_e_bg(t_penemy->xspeed>>8, 0x10);
		if (bg_empty_f || bg_val==2){
			if (t_penemy->turnback_cnt<2 && (t_penemy->flag2 &ENEMY_CAN_TURNBACK)){
				e_soldier_turnback();
			}else{
				t_penemy->air_jump_f++;
				delta =chr_ys[0]-*t_penemy_y;
				idx=4;
				if (delta<0){
					delta=-delta;
					idx=0;
				}
				if (delta<0x10)
					idx=0;
				idx +=get_rand8() &3;
				pspds =&spds[spdidx[idx]];
				t_penemy->yspeed =*pspds++;
				t_penemy->xspeed =*pspds;
				if (t_penemy->left)
					t_penemy->xspeed =-t_penemy->xspeed;
				e_soldier_form();
				refresh_enemyxy();
				return;
			}
		}
	}
	check_e_bg(0,0);
	solidunderplat();
	if (bg_solid_f){
		if (t_penemy->state)
			t_penemy->state=7;
			return;
	}
	xbound =*t_penemy_x + (t_penemy->left ?8:-8);
	if (t_penemy->turnback_cnt<2 && xbound<0xf0 && xbound>=0x10){
		solid_or_solidunderplat(xbound, *t_penemy_y);
		if (bg_solid_f)
			e_soldier_turnback();
	}
	e_soldier_form();
	refresh_enemyxy();
}

void e_soldier_st4(){
	static int xdeltas[]={-0x10,0x10,-0x10,0x10};
	static int ydeltas[]={-0x09,-0x09,0x0a,0x0a};
	static unsigned char dirs[]={0x06,0x00};
	int idx, bx,by;
	unsigned char formidx;

	if (t_penemy->flag2 &ENEMY_SOLDIER_FIRETYPE0){
		idx=0;
		formidx=6;
	}else{
		idx=2;
		formidx=7;
		t_penemy->col_type=0x1b;
	}
	t_penemy->form_idx=formidx;
	if (--t_penemy->delay_btwn_fire){
		e_soldier_form();
		move_enemy_scroll();
		return;
	}
	if ((--t_penemy->bullets)<0){
		t_penemy->col_type=0;
		t_penemy->bullets=0;
		t_penemy->form_idx=0;
		e_soldier_form();
		move_enemy_scroll();
		if (t_penemy->state)
			t_penemy->state=3;
		return;
	}
	t_penemy->delay_btwn_fire=0x10;
	if (t_penemy->left)
		idx++;
	bx =*t_penemy_x+xdeltas[idx];
	by =*t_penemy_y+ydeltas[idx];
	if (bx<0x100 && bx>=8 && fire_eb(0, bx, by, dirs[idx &1], 6) )
		t_penemy->fire_cnt=6;
	e_soldier_form();
	move_enemy_scroll();
}

void e_soldier_st5(){
	t_penemy->form_idx=0xb;
	e_soldier_form();
	t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	t_penemy->yspeed =-0x380;
	t_penemy->xspeed =0x060;
	if (*t_penemy_x<0x10 || *t_penemy_x>=0xf0)
		t_penemy->xspeed =0;
	if (t_penemy->left)
		t_penemy->xspeed =-t_penemy->xspeed;
	move_enemy_scroll();
	t_penemy->delay =0x10;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_soldier_st6(){
	e_soldier_form();
	t_penemy->yspeed +=0x30;
	if (*t_penemy_y<8)
		t_penemy->state++;
	else{
		refresh_enemyxy();
		if (!(--t_penemy->delay))
			t_penemy->state++;
	}
}

void e_soldier_st7(){
	bombing_init(0);
}

void e_soldier_st8(){
	bombing_proc(0);
}

void e_soldier_st9(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_soldier_sta(){
	t_penemy->form_idx=8;
	*t_penemy_y+=0x10;
	e_soldier_form();
	move_enemy_scroll();
	t_penemy->delay=8;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_soldier_stb(){
	if (--t_penemy->delay==0){
		t_penemy->delay=8;
		t_penemy->form_idx++;
		if (t_penemy->form_idx==0xa){
			enemy_selfclear();
			return;
		}
		*t_penemy_y+=0x8;
	}
	e_soldier_form();
	move_enemy_scroll();
}

void e_soldier_xspd(){
	static short xspds[]={-0x100, 0x140, -0x100, 0x100};
	int idx;

	if (zstage_f)
		idx=2;
	else
		idx=0;
	idx +=t_penemy->left;
	t_penemy->xspeed=xspds[idx];
}

void e_soldier_turnback(){
	t_penemy->turnback_cnt++;
	t_penemy->left ^=1;
	e_soldier_xspd();
}

unsigned char get_soldier_bullets(){
	static unsigned char bullets[]={
		0x01,0x01,0x02,0x01,0x02,0x01,0x02,0x02
	};
	int idx;

	idx =((diff_extra &2)<<1) +(get_rand8() &3);
	return bullets[idx];
}

void e_soldier_form(){
	static char forms[]={
		0x3b,0x3c,0x3d,0x3f,0x3c,0x3e,0x40,0x26,0x73,0x18,0x28,0x27
	};

	*t_penemy_dform =forms[t_penemy->form_idx];
	*t_penemy_dattr =(t_penemy->left) ?0:0x40;
	if (t_penemy->fire_cnt){
		t_penemy->fire_cnt--;
		*t_penemy_dattr |=8;
	}
}

void e_sniper_st1(){
	static unsigned char delays[]={1,0x30,0x80};
	static unsigned findexes[]={3,0,0};

	t_penemy->delay=delays[t_penemy->flag2];
	t_penemy->form_idx=findexes[t_penemy->flag2];
	enemy_y_regulate(4);
	t_penemy->left=1;
	if (t_penemy->flag2==1)
		*t_penemy_y+=5;
	t_penemy->state++;
}

void e_sniper_st2(){
	static char delays[]={0x40, 4, 0x10};
	static unsigned char bullets[]={3, 1, 3};

	e_sniper_form();
	move_enemy_scroll();
	if (--t_penemy->delay)
		return;
	if (t_penemy->flag2){
		t_penemy->delay=8;
		if (++t_penemy->form_idx<3)
			return;
		if (t_penemy->flag2==1)
			t_penemy->form_idx--;
		else{
			*t_penemy_y-=0xe;
			(*t_penemy_x)++;
		}
	}
	t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	t_penemy->col_type=0;
	t_penemy->delay_btwn_fire=delays[t_penemy->flag2];
	t_penemy->bullets=bullets[t_penemy->flag2];
	if (t_penemy->state)
		t_penemy->state++;
}

void e_sniper_st3(){
	static unsigned char forms[]={4,3,5};
	static int xdeltas[]={-0xd, -0xf, -0xf};
	static int ydeltas[]={-0x12, -0xb, -0x6};
	static unsigned char adjs[]={3,5,3};

	int dir, formv, formidx;
	int sx,sy, bx,by;

	e_sniper_form();
	move_enemy_scroll();
	if (--t_penemy->delay_btwn_fire)
		return;
	if (--t_penemy->bullets>=0){
		t_penemy->delay_btwn_fire=0x18;
		t_penemy->left =(chr_xs[0]<*t_penemy_x)?1:0;
		if (t_penemy->flag2==1){
			dir =(t_penemy->left) ?0xc:0;
		}else{
			if (t_penemy->flag2==2)
				new_e_pos(0,-0x10, &sx, &sy);
			else
				new_e_pos(0,0, &sx, &sy);
			get_dir_aimed_adjust(sx, sy, 1, &dir);
		}
		formv=dir+6;
		if (formv>=0x18)
			formv -=0x18;
		if (formv>=0xc)
			formv =0x18-formv;
		formidx =(formv<5) ?0:(formv<8)?1:2;
		if (t_penemy->flag2!=1)
			t_penemy->form_idx=forms[formidx];
		bx =(t_penemy->left) ?*t_penemy_x+xdeltas[formidx] :*t_penemy_x-xdeltas[formidx];
		by =*t_penemy_y+ydeltas[formidx];
		if (fire_eb(1, bx,by, dir,adjs[t_penemy->flag2]))
			t_penemy->fire_cnt=6;
	}else{
		if (!t_penemy->flag2){
			t_penemy->bullets=3;
			t_penemy->delay_btwn_fire=0x80;
		}else{
			t_penemy->form_idx =(t_penemy->flag2==1) ?2:3;
			t_penemy->delay=0x80;
			if (t_penemy->state)
				t_penemy->state++;
		}
	}
}

void e_sniper_st4(){
	static unsigned char delays[]={1,0x60,0x80};

	if (--t_penemy->delay==0){
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		t_penemy->delay=8;
		if (--t_penemy->form_idx==0){
			t_penemy->delay=delays[t_penemy->flag2];
			if (t_penemy->state)
				t_penemy->state=2;
		}
		if (t_penemy->flag2==2 && t_penemy->form_idx==2){
			*t_penemy_y+=0xe;
			(*t_penemy_x)--;
		}
	}
	e_sniper_form();
	move_enemy_scroll();
}

void e_sniper_st5(){
	t_penemy->form_idx=6;
	e_sniper_form();
	t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	t_penemy->yspeed =-0x380;
	t_penemy->xspeed =0x060;
	if (*t_penemy_x<0x10 || *t_penemy_x>=0xf0)
		t_penemy->xspeed =0;
	if (t_penemy->left)
		t_penemy->xspeed =-t_penemy->xspeed;
	move_enemy_scroll();
	t_penemy->delay =0x10;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_sniper_st6(){
	e_sniper_form();
	t_penemy->yspeed +=0x30;
	if (*t_penemy_y<8)
		t_penemy->state++;
	else{
		refresh_enemyxy();
		if (!(--t_penemy->delay))
			t_penemy->state++;
	}
}

void e_sniper_st7(){
	bombing_init(0);
}

void e_sniper_st8(){
	bombing_proc(0);
}

void e_sniper_st9(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_sniper_form(){
	static unsigned char forms[]={
		0x44,0x45,0x46,0x43,0x42,0x41,0x29,0x44,0x45,0x46,0x2c,0x42,0x2d,0x29
	};
	unsigned char *pforms=(t_penemy->flag2==2) ?&forms[7]:&forms[0];
	unsigned char dattr;

	*t_penemy_dform=pforms[t_penemy->form_idx];
	dattr=(t_penemy->left) ? 0x40: 0;
	if (t_penemy->fire_cnt){
		t_penemy->fire_cnt--;
		dattr |=8;
	}
	*t_penemy_dattr=dattr;
}

void e_tank_st1(){
	move_enemy_scroll();
	t_penemy->aimed_dir=6;
	t_penemy->state++;
}

void e_tank_st2(){
	move_enemy_scroll();
	if (!check_emy_bounds(0xf0, 0x40))
		return;
	t_penemy->delay=1;
	t_penemy->state++;
}

void e_tank_st3(){
	move_enemy_scroll();
	if (!e_tank_form())
		return;
	if (++t_penemy->form_idx<4)
		return;
	t_penemy->bullets=2;
	t_penemy->delay_btwn_fire=(num_of_allclears) ?8:0x28;
	t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
	t_penemy->delay =0x10;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_tank_st4(){
	static unsigned char forms[]={
		0x11,0x12,0x13
	};
	static int deltas[]={0,-8, -0x10, -0xe, -0xe, -8};
	int *pdeltas;

	int dummy, rotatef, bx,by;

	move_enemy_scroll();
	if (check_emy_bounds(0x30, 0xc0)){
		t_penemy->form_idx=2;
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		t_penemy->delay =1;
		if (t_penemy->state)
			t_penemy->state++;
		return;
	}
	rotatef=get_dir_aimed_adjust(*t_penemy_x, *t_penemy_y, 0, &dummy);
	if (--t_penemy->delay==0){
		t_penemy->delay=0x10;
		if (rotatef!=2){
			if (!rotatef){
				if (t_penemy->aimed_dir!=8){
					t_penemy->aimed_dir++;
					enemy_bgobj_4x4(forms[t_penemy->aimed_dir-6]);
				}
			}else{
				if (t_penemy->aimed_dir!=6){
					t_penemy->aimed_dir--;
					enemy_bgobj_4x4(forms[t_penemy->aimed_dir-6]);
				}
			}
		}//if (rotatef!=2)
	}//if (--t_penemy->delay==0)
	if (--t_penemy->delay_btwn_fire)
		return;
	if (--t_penemy->bullets<0){
		t_penemy->bullets=2;
		t_penemy->delay_btwn_fire=0x50;
	}else
		t_penemy->delay_btwn_fire=0x10;
	if ((*t_penemy_y+0x20)<chr_ys[0] || *t_penemy_x<chr_xs[0])
		return;
	pdeltas=&deltas[(t_penemy->aimed_dir-6)<<1];
	by =*t_penemy_y + *pdeltas++;
	bx =*t_penemy_x + *pdeltas++;
	fire_eb(0, bx,by, t_penemy->aimed_dir,5);
}

void e_tank_st5(){
	move_enemy_scroll();
	if (!e_tank_form())
		return;
	if (--t_penemy->form_idx>=0x80)
		enemy_selfclear();
}

void e_tank_st6(){
	int bno=(t_penemy->flag2 &ENEMY_TANK_BGTYPE1) ? 0x17: 0x16;

	move_enemy_scroll();
	enemy_bgobj_4x4(bno);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_tank_st7(){
	bombing_init(0);
}

void e_tank_st8(){
	bombing_proc(0);
}

void e_tank_st9(){
	move_enemy_scroll();
	enemy_selfclear();
}

int e_tank_form(){
	static unsigned char forms[]={
		0x16,0x14,0x18,0x11,0x17,0x15,0x18,0x11,0x11,0x12,0x13
	};
	int idx;

	if (--t_penemy->delay)
		return 0;
	t_penemy->delay=4;
	idx=(t_penemy->flag2 &ENEMY_TANK_BGTYPE1)
			? t_penemy->form_idx+4 :t_penemy->form_idx;
	enemy_bgobj_4x4(forms[idx]);
	return 1;
}

void e_3wturret_st1(){
	t_penemy->utemp =8;
	t_penemy->delay=0x50;
	t_penemy->state++;
}

void e_3wturret_st2(){
	if (!player_normal || --t_penemy->delay)
		return;
	t_penemy->delay =6;
	enemy_bgobj_4x4(t_penemy->form_idx);
	if (t_penemy->form_idx>=2){
		t_penemy->hp =t_penemy->utemp;
		t_penemy->delay_btwn_fire =4;
		t_penemy->delay=0x40;
		t_penemy->state++;
	}else
		t_penemy->form_idx++;
}

void e_3wturret_st3(){
	static int xdeltas[]={-8, 0, 8};
	static unsigned char dirs[]={0x48, 0x46, 0x44};
	int i;
	int bx,by;

	if (t_penemy->delay_btwn_fire){
		if (--t_penemy->delay_btwn_fire==0){
			for (i=2; i>=0; i--){
				new_e_pos(xdeltas[i], 8, &bx,&by);
				fire_eb(1, bx,by,dirs[i],7);
			}
		}
	}
	if (--t_penemy->delay==0){
		t_penemy->utemp =t_penemy->hp;
		t_penemy->hp =0xf1;
		t_penemy->delay=0x6;
		t_penemy->state++;
	}
}

void e_3wturret_st4(){
	if (--t_penemy->delay)
		return;
	t_penemy->delay =6;
	enemy_bgobj_4x4(t_penemy->form_idx);
	if (t_penemy->form_idx==0){
		t_penemy->delay=(diff_extra>=2) ?0x60:0xc0;
		t_penemy->state=2;
	}else
		t_penemy->form_idx--;
}

void e_3wturret_st5(){
	enemy_bgobj_4x4(5);
	t_penemy->state++;
}

void e_3wturret_st6(){
	bombing_init(0);
}

void e_3wturret_st7(){
	bombing_proc(0);
}

void e_3wturret_st8(){
	enemy_selfclear();
}

void e_b_robject_st1(){
	t_penemy->delay=0x80;
	t_penemy->state++;
}

void e_b_robject_st2(){
	if (--t_penemy->delay)
		return;
	t_penemy->delay=4;
	enemy_bgobj_4x4(t_penemy->form_idx+3);
	if (++t_penemy->form_idx>=2){
		t_penemy->hp=0xa;
		t_penemy->state++;
	}
}

void e_b_robject_st3(){
	return;
}

void e_b_robject_st4(){
	enemy_bgobj_4x4(5);
	b_robject_clred++;
	t_penemy->state++;
}

void e_b_robject_st5(){
	bombing_init(0);
}

void e_b_robject_st6(){
	bombing_proc(0);
}

void e_b_robject_st7(){
	enemy_selfclear();
}

void e_scuba_st1(){
	t_penemy->delay =0x80;
	t_penemy->state++;
}

void e_scuba_st2(){
	*t_penemy_dform=0x4b;
	*t_penemy_dattr =(t_penemy->delay&0x10) ?0:8;
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		if (*t_penemy_y<0xb8)
			t_penemy->delay=0x10;
		else{
			t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
			t_penemy->delay_btwn_fire =0x10;
			t_penemy->delay=0x30;
			if (t_penemy->state)
				t_penemy->state++;
		}
	}
}

void e_scuba_st3(){
	*t_penemy_dform=0x4c;
	if (t_penemy->fire_cnt){
		t_penemy->fire_cnt--;
		*t_penemy_dattr =8;
	}else
		*t_penemy_dattr =0;
	if (--t_penemy->delay){
		if (--t_penemy->delay_btwn_fire==0){
			t_penemy->fire_cnt=7;
			gen_new_enemy (5,-18, 0xb);
		}
		move_enemy_scroll();
	}else{
		move_enemy_scroll();
		t_penemy->delay =0xc0;
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_scuba_st4(){
	bombing_init(0);
}

void e_scuba_st5(){
	bombing_proc(0);
}

void e_scuba_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_3wshell_st1(){
	static short yxspds[]={
		-0x0500,0x0000, -0x0200,0x0000, -0x01c0,0x0090, -0x01c0,-0x0090,
		-0x0500,-0x0040,-0x0500,-0x0080,-0x0500,-0x00c0,-0x0500,-0x0100,

	};
	int idx;
	short *pyxspds;

	t_penemy->flag1 =(t_penemy->flag2)
		? ENEMY_NONSHOOTABLE
		: ENEMY_NONSHOOTABLE |ENEMY_BOMBING_TYPE1 |ENEMY_BOMBING_SND;
	*t_penemy_dform =0x20;
	*t_penemy_dattr =6;
	if (t_penemy->flag2)
		idx =t_penemy->flag2;
	else if ((idx=t_penemy->spd_idx)!=0)
		idx +=3;
	pyxspds =&yxspds[idx<<1];
	t_penemy->yspeed =*pyxspds++;
	t_penemy->xspeed =*pyxspds;
	t_penemy->state++;
}

void e_3wshell_st2(){
	t_penemy->yspeed +=0x10;
	refresh_enemyxy();
	if (t_penemy->flag2){
		if (t_penemy->yspeed>=0 && *t_penemy_y>=chr_ys[0]){
			check_e_bg(0,0);
			if (!bg_empty_f){
				PLAYSOUND3(BOMBING1);
				if (t_penemy->state)
					t_penemy->state=7;
			}
		}
	}else{
		if (t_penemy->yspeed>=0 || *t_penemy_y<0x30)
			if (t_penemy->state)
				t_penemy->state++;
	}

}

void e_3wshell_st3(){
	int i,no,idx;

	refresh_enemyxy();
	for (i=3; i>0; i--){
		if ((no=find_free_emy())<0)
			break;
		idx =0xa+no;
		enemy_common_init(no, 0xb);
		enemy[no].flag2 =i;
		chr_xs[idx] =*t_penemy_x;
		chr_ys[idx] =*t_penemy_y;
	}
	if (t_penemy->state)
		t_penemy->state++;
}

void e_3wshell_st4(){
	bombing_init(0);
}

void e_3wshell_st5(){
	bombing_proc(0);
}

void e_3wshell_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_3wshell_st7(){
	bombing_init(1);
}

void e_3wshell_st8(){
	bombing_proc(0);
}

void e_3wshell_st9(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_gunner_st1(){
	*t_penemy_dform=0xbd;
	t_penemy->delay =(t_penemy->flag2<<4) +1;
	t_penemy->state++;
}

void e_gunner_st2(){
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		(*t_penemy_dform)++;
		t_penemy->delay =5;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_gunner_st3(){
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		PLAYSOUND3(M_GUN);
		gen_new_enemy (-0x10,-4, 0xf);
		t_penemy->delay =(t_penemy->flag2<<4) +0x30;
		(*t_penemy_dform)--;
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_gunner_st4(){
	bombing_init(0);
}

void e_gunner_st5(){
	bombing_proc(0);
}

void e_gunner_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_shell_st1(){
	t_penemy->xspeed =-0x280;
	*t_penemy_dform=0x1f;
	t_penemy->state++;
}

void e_shell_st2(){
	refresh_enemyxy();
}

void e_shell_st3(){
	enemy_selfclear();
}



void e_bridge_st1(){
	move_enemy_scroll();
	if (get_p_e_xdist()<0x18){
		t_penemy->delay=1;
		t_penemy->bombobj_idx =0;
		t_penemy->bombing_idx =0;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_bridge_st2(){
	static int bnos[]={0x00,0x1a,0x1b,0x1c,0x19,0x1c,0x19,0x1d};
	static int ydeltas[]={0,0x00,-0x10,0x00,0x10};
	static int xdeltas[]={0,-0x10,0x00,0x10,0x00,};
	int idx, x, y;
	unsigned char bno;

	move_enemy_scroll();
	if (--t_penemy->delay)
		return;
	idx=(t_penemy->bombobj_idx<<1) +t_penemy->bombing_idx;
	if (t_penemy->bombing_idx<2 && (bno=bnos[idx])){
		y=*t_penemy_y-0xc;
		x=(t_penemy->bombing_idx &1) ?*t_penemy_x-0xc :*t_penemy_x-0x2c;
		show_bgobj_4x4(x,y,bno);
		set_bgval_4x4(0,0);
	}
	if (++t_penemy->bombing_idx>=4){
		if (t_penemy->state)
			t_penemy->state++;
		return;
	}
	PLAYSOUND3(BOMBING1);
	t_penemy->delay=4;
	new_e_pos(xdeltas[t_penemy->bombing_idx], ydeltas[t_penemy->bombing_idx], &x, &y);
	bombing_gen(x,y);
}

void e_bridge_st3(){
	bombing_init(0);
}

void e_bridge_st4(){
	bombing_proc(0);
}

void e_bridge_st5(){
	move_enemy_scroll();
	if (++t_penemy->bombobj_idx>=4){
		enemy_selfclear();
	}else{
		*t_penemy_x +=0x20;
		if (*t_penemy_x>=0x100){
			enemy_selfclear();
			return;
		}
		*t_penemy_dform=1;
		t_penemy->delay=1;
		t_penemy->bombing_idx =0;
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_s0boss_st1(){
	t_penemy->state++;
}

void e_s0boss_st2(){
	move_enemy_scroll();
}

void e_s0boss_st3(){
	stop_bgm();
	stageclear_init(BOMBING3);
	enemy_allclear();
	bombing_init(0);
}

void e_s0boss_st4(){
	bombing_proc(0);
}

void e_s0boss_st5(){
	*t_penemy_dform=0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_s0boss_st6(){
	t_penemy->bombobj_idx=0;
	t_penemy->delay=8;
	t_penemy->state++;
}

void e_s0boss_st7(){
	static int yxdeltas[]={
		-0x10,-0x10, 0x20,0, -0x20,0x20, 0x20,0, -0x20,0x20, 0x20,0, -0x20,0x20, 0x20,0,
		-1
	};
	static unsigned char bnos[]={0x1e,0x22,0x1f,0x23,0x20,0x24,0x21,0x25};
	static unsigned char bgvals[]={0x00,0x00,0x00,0x04,0x00,0x04,0x00,0x04};
	unsigned char val;
	int *pdeltas;

	if (--t_penemy->delay==0){
		t_penemy->delay=8;
		enemy_bgobj_4x4(bnos[t_penemy->bombobj_idx]);
		val =bgvals[t_penemy->bombobj_idx];
		set_bgval_4x4(val,val);
		t_penemy->bombobj_idx++;
		bombing_gen(*t_penemy_x, *t_penemy_y);
	}else if (t_penemy->delay==1){
		pdeltas =&yxdeltas[t_penemy->bombobj_idx<<1];
		if (*pdeltas==-1){
			long_delay=0x30;
			enemy_selfclear();
		}else{
			*t_penemy_y +=*pdeltas++;
			*t_penemy_x +=*pdeltas;
		}
	}
}

void e_cannon_st1(){
	t_penemy->delay_btwn_fire=0x60;
	t_penemy->state++;
}

void e_cannon_st2(){
	static int adjs[]={1,3,5,7};
	int bx,by, spdadj;

	move_enemy_scroll();
	if (--t_penemy->delay_btwn_fire)
		return;
	e_cannon_form(t_penemy->form_idx);
	t_penemy->delay_btwn_fire =(t_penemy->form_idx) ?8:0x28;
	t_penemy->form_idx ^=2;
	if (t_penemy->form_idx){
		new_e_pos(-8,0, &bx, &by);
		spdadj =adjs[get_rand8() &3];
		fire_eb(0, bx,by,0x17,spdadj);
	}
}

void e_cannon_st3(){
	e_cannon_form(4);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_cannon_st4(){
	bombing_init(0);
}

void e_cannon_st5(){
	bombing_proc(0);
}

void e_cannon_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_cannon_form(int idx){
	static unsigned char forms[]={0x29,0x26,0x2a,0x27,0x2b,0x28};

	if (t_penemy->flag2){
		idx++;
		*t_penemy_x -=8;
	}
	enemy_bgobj_4x4(forms[idx]);
	if (t_penemy->flag2){
		*t_penemy_x +=8;
	}
}

void e_rolling_g_st1(){
	*t_penemy_y=0x72;
	t_penemy->state++;
}

void e_rolling_g_st2(){
	static int yjudges[]={0x7c,0x8c,0x9c};
	int i=2;

	while(i>=0){
		if (*t_penemy_y>=yjudges[i])
			break;
		i--;
	}
	*t_penemy_dform=0x99+i+1;
	if (i==1)
		t_penemy->col_type=0xe;
	refresh_enemyxy();
	if (*t_penemy_y>=0xac){
		if (*t_penemy_y>=0xbc)
			enemy_selfclear();
		else
			t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	}
}

void e_rolling_g_st3(){
	bombing_init(0);
}

void e_rolling_g_st4(){
	bombing_proc(3);
}

void e_rolling_g_st5(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_grenade_st1(){
	t_penemy->line_ylocus =*t_penemy_y;
	t_penemy->oval_yspd =(short)-0x0300;
	t_penemy->state++;
}

void e_grenade_st2(){
	static unsigned char yjudges[]={0x80,0x90};
	static unsigned char form_cnts[]={0x04,0x08,0x08};
	static unsigned char forms0[]={0xa8,0xa9,0xa6,0xa9,0x00,0x00,0x00,0xc0};
	static unsigned char forms1[]={
		0xa4,0xa5,0xa6,0xa5,0xa4,0xa7,0xa6,0xa7,0x00,0x00,0x00,0xc0,0xc0,0x00,
		0x00,0xc0
	};
	static unsigned char forms2[]={
		0xa0,0xa1,0xa2,0xa1,0xa0,0xa3,0xa2,0xa3,0x00,0x00,0x00,0xc0,0xc0,0x00,
		0x00,0xc0
	};
	static unsigned char *formtbls[]={forms0, forms1, forms2};
	unsigned char *pforms;
	int ylevel;
	unsigned char cnt;

	for (ylevel=1; ylevel>=0; ylevel--){
		if (t_penemy->line_ylocus>=yjudges[ylevel])
			break;
	}
	ylevel++;
	if (!(framecount&7))
		t_penemy->form_idx++;
	cnt =form_cnts[ylevel];
	if (t_penemy->form_idx>=cnt)
		t_penemy->form_idx =0;
	pforms =formtbls[ylevel];
	*t_penemy_dform =pforms[t_penemy->form_idx];
	*t_penemy_dattr =pforms[t_penemy->form_idx+cnt];
	t_penemy->oval_yspd +=0x000c;
	combine_line_oval_y();
	if (t_penemy->oval_ylocus>=0)
		t_penemy->state++;
}

void e_grenade_st3(){
	PLAYSOUND3(BOMBING1);
	*t_penemy_y =0xac;
	bombing_init(1);
	t_penemy->state++;
}

void e_grenade_st4(){
	bombing_init(0);
}

void e_grenade_st5(){
	bombing_proc(0);
}

void e_grenade_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_zturret_st1(){
	static unsigned char delays[]={0x50,0x80,0xb0,0xf0};
	t_penemy->delay =delays[t_penemy->flag2 & ENEMY_DELAY_TYPE];
	t_penemy->state++;
}

void e_zturret_st2(){
	if (!t_penemy->utemp){
		t_penemy->utemp++;
		enemy_bgobj_2xn(0x84);
	}
	if (--t_penemy->delay==0){
		t_penemy->delay=1;
		t_penemy->state++;
	}
}

void e_zturret_st3(){
	static unsigned char bnos[]={0x85, 0x88, 0x89};
	if (--t_penemy->delay)
		return;
	t_penemy->delay=8;
	enemy_bgobj_2xn(bnos[t_penemy->form_idx]);
	if (++t_penemy->form_idx>=3){
		t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
		t_penemy->delay_btwn_fire=0x80;
		t_penemy->state++;
	}
}

void e_zturret_st4(){
	if (--t_penemy->delay_btwn_fire==0){
		t_penemy->delay_btwn_fire=0x50;
		fire_eb_aimed_dir18(*t_penemy_x, *t_penemy_y, 3, 4);
	}
}

void e_zturret_st5(){
	enemy_bgobj_2xn(0x83);
	t_penemy->state++;
}

void e_zturret_st6(){
	bombing_init(0);
	t_penemy->form_idx=0;
}

void e_zturret_st7(){
	bombing_proc(0);
}

void e_zturret_st8(){
	enemy_selfclear();
}

void e_s_robject_st1(){
	static delays[]={0x20,0x80,0xb0,0xf0};
	static hps[]={0x08,0x05,0x10,0x05};
	static sh_hps[]={0x00,0x03,0x00,0x03};
	int idx;

	idx=(t_penemy->flag2 & (ENEMY_BIG_S_ROBJECT |ENEMY_HAS_SHIELD)) >>2;
	t_penemy->hp=hps[idx];
	t_penemy->shield_hp=sh_hps[idx];
	t_penemy->score_idx=2;
	if (t_penemy->flag2 & ENEMY_HAS_SHIELD){
		t_penemy->hitsfx_idx=4;
		t_penemy->col_type=2;
		t_penemy->delay=0x20;
	}else{
		t_penemy->col_type=5;
		t_penemy->delay=delays[t_penemy->flag2 & ENEMY_DELAY_TYPE];
	}
	t_penemy->state++;
}

void e_s_robject_st2(){
	if (!t_penemy->utemp && !(t_penemy->flag2 & ENEMY_BIG_S_ROBJECT)){
		t_penemy->utemp++;
		if (t_penemy->flag2 & ENEMY_HAS_SHIELD)
			enemy_bgobj_2xn(0x80);
		else
			enemy_bgobj_2xn(0x84);
	}
	if (--t_penemy->delay)
		return;
	if (t_penemy->flag2 & ENEMY_HAS_SHIELD){
		t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
		t_penemy->state++;
	}
	t_penemy->delay_btwn_fire=1;
	t_penemy->delay=1;
	t_penemy->state++;
}

void e_s_robject_st3(){
	static unsigned char bnos[]={0x85,0x86,0x87};

	if (--t_penemy->delay)
		return;
	if (!(t_penemy->flag2 & ENEMY_BIG_S_ROBJECT)){
		t_penemy->delay=8;
		enemy_bgobj_2xn(bnos[t_penemy->form_idx]);
		if (++t_penemy->form_idx<3)
			return;
	}
	t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
	t_penemy->state++;
}

void e_s_robject_st4(){
	if (enemy_rounds<7 || t_penemy->shield_hp || *t_penemy_y>=0x70)
		return;
	if (--t_penemy->delay_btwn_fire==0){
		t_penemy->delay_btwn_fire=0x28;
		fire_eb_aimed_dir18(*t_penemy_x,*t_penemy_y,3,5);
	}
}

void e_s_robject_st5(){
	static unsigned char bnos[]={0x83,0x87,0x82,0x81,0x83,0x8a,0x82,0x81};
	int idx;
	unsigned char hp_reset;

	idx=t_penemy->shield_hp;
	if (t_penemy->flag2 & ENEMY_BIG_S_ROBJECT)
		idx+=4;
	enemy_bgobj_2xn(bnos[idx]);
	hp_reset=5;
	if (--t_penemy->shield_hp<0){
		t_penemy->state++;
		return;
	}else if (t_penemy->shield_hp==0){
		t_penemy->hitsfx_idx=0;
		t_penemy->col_type=5;
		hp_reset=8;
	}
	t_penemy->hp=hp_reset;
	t_penemy->state=4;
}

void e_s_robject_st6(){
	bombing_init(0);
	t_penemy->form_idx=0;
}

void e_s_robject_st7(){
	bombing_proc(0);
}

void e_s_robject_st8(){
	if (--s_robject_left){
		enemy_selfclear();
		return;
	}
	*t_penemy_dform=0;
	enemy_allclear();
	t_penemy->bombing_idx=3;
	t_penemy->delay=4;
	t_penemy->state++;
}

void e_s_robject_st9(){
	static unsigned char bnos[]={0x02,0x03,0x01,0x00};
	static int xs[]={0x70,0x90,0x90,0x70};
	static int ys[]={0x78,0x78,0x58,0x58};
	int by;

	if (--t_penemy->delay){
		if (t_penemy->delay==1)
			PLAYSOUND4(BOMBING2);
		return;
	}
	*t_penemy_x =xs[t_penemy->bombing_idx];
	*t_penemy_y =ys[t_penemy->bombing_idx];
	enemy_bgobj_4x4(bnos[t_penemy->bombing_idx]);
	by=*t_penemy_y+((t_penemy->bombing_idx & 2) ? -4:-12);
	bombing_gen(*t_penemy_x, by);
	if ((--t_penemy->bombing_idx) & 0x80){
		t_penemy->delay=0x10;
		t_penemy->state++;
	}else
		t_penemy->delay=0x1;
}

void e_s_robject_sta(){
	if (--t_penemy->delay==0){
		room_clear=1;
		enemy_selfclear();
	}
}

void e_runner_st1(){
	set_ze_pos_xspd(0);
	t_penemy->delay_btwn_fire =8;
	t_penemy->state++;
}

void e_runner_st2(){
	int subtype, xlevel;

	zenemy_form();
	rfsh_ze_x_chk_bound();
	if (--t_penemy->delay_btwn_fire)
		return;
	t_penemy->delay_btwn_fire =0x10;
	if (*t_penemy_x<0x68 || *t_penemy_x>0x98)
		return;
	subtype=(t_penemy->flag2>>1) & 3;
	switch (subtype){
	case 0:
		gen_zbullet();
		break;
	case 1:
		if (++t_penemy->utemp & 1)
			gen_grenade();
		break;
	case 2:
	case 3:
		xlevel =calc_xlevel7(0, *t_penemy_x);
		gen_rolling_g(xlevel, *t_penemy_x, *t_penemy_y+8);
		break;
	}
}

void e_runner_st3(){
	t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	*t_penemy_dform =0x96;
	t_penemy->yspeed = -0x0280;
	t_penemy->xspeed =0;
	t_penemy->delay =0x10;
	t_penemy->state++;
}

void e_runner_st4(){
	refresh_enemyxy();
	t_penemy->yspeed +=0x38;
	if (--t_penemy->delay==0)
		t_penemy->state++;
}

void e_runner_st5(){
	bombing_init(0);
}

void e_runner_st6(){
	bombing_proc(2);
}

void e_runner_st7(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_jumper_st1(){
	int is_bjumper;

	is_bjumper =t_penemy->flag2 & 2;
	if (is_bjumper){
		if (!bjumper_appeared && enemy_rounds)
			bjumper_appeared++;
		else
			t_penemy->flag2 &=0xfd;
	}
	set_ze_pos_xspd(1);
	t_penemy->state++;
}

void e_jumper_st2(){
	static int ydeltas[]={
		-3,-3,-2,-2,-2,-1,-1,-1,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x02,
		0x02,0x02,0x03,0x03	
	};
	int is_bjumper =t_penemy->flag2 & 2;
	unsigned char cg =(is_bjumper) ?5:0;

	if (t_penemy->delay==0)
		*t_penemy_dform =0x97;
	else if (t_penemy->delay>=4)
		*t_penemy_dform =0x98;
	else
		*t_penemy_dform =0x93;
	if (t_penemy->xspeed<0)
		*t_penemy_dattr |=0x40;
	else
		*t_penemy_dattr &=0xbf;
	*t_penemy_dattr =(*t_penemy_dattr & 0xf8) | cg;
	if (t_penemy->delay){
		t_penemy->delay--;
		if (!is_bjumper && t_penemy->delay==8){
			fire_eb_aimed_dir18(*t_penemy_x, *t_penemy_y, 3, 4);
		}
	}else{
		rfsh_ze_x_chk_bound();
		*t_penemy_y +=ydeltas[t_penemy->data_ptr++];
		if (t_penemy->data_ptr>=0x14){
			t_penemy->data_ptr=0;
			t_penemy->delay =0x10;
		}
	}
}

void e_jumper_st3(){
	e_runner_st3();
}

void e_jumper_st4(){
	e_runner_st4();
}

void e_jumper_st5(){
	int is_bjumper =t_penemy->flag2 & 2;
	int nx,ny;

	if (!is_bjumper || *t_penemy_x<0x64 || *t_penemy_x>0x9c ||
		(t_penemy->flag2 &ENEMY_CLEARED) )
		t_penemy->state++;
	else{
		t_penemy->flag2>>=2;
		PLAYSOUND3(BOMBING0);
		new_e_pos (0,0, &nx, &ny);
		bombing_gen(nx,ny);
		t_penemy->flag2 &=7;
		t_penemy->oval_ylocus=0;
		t_penemy->state=1;
		t_penemy->type=0;
	}
}

void e_jumper_st6(){
	bombing_init(0);
}

void e_jumper_st7(){
	bombing_proc(2);
}

void e_jumper_st8(){
	enemy_selfclear();
}

void e_grenadier_st1(){
	grenadier_active =1;
	set_ze_pos_xspd(3);
	t_penemy->delay =0x20;
	t_penemy->state++;
}

void e_grenadier_st2(){
	int xlevel, p_xlevel;

	if (t_penemy->still){
		*t_penemy_dform =0x96;
		if (--t_penemy->delay){
			if (t_penemy->bullets && (--t_penemy->delay_btwn_fire==0)){
				t_penemy->delay_btwn_fire=0x14;
				t_penemy->bullets--;
				gen_grenade();
			}
		}else{
			t_penemy->delay=8;
			t_penemy->still=0;
			xlevel=calc_xlevel7(0, *t_penemy_x);
			p_xlevel=calc_xlevel7(1, chr_xs[0]);
			if ((p_xlevel<xlevel && t_penemy->xspeed<0) ||
				(p_xlevel>=xlevel && t_penemy->xspeed>=0))
				t_penemy->xspeed =-t_penemy->xspeed;
		}
	}else{
		zenemy_form();
		if ((t_penemy->xspeed<0 && *t_penemy_x<0x60) ||
			(t_penemy->xspeed>=0 && *t_penemy_x>=0xa0))
			t_penemy->delay=1;
		else
			rfsh_ze_x_chk_bound();
		if (--t_penemy->delay==0){
			xlevel=calc_xlevel7(0, *t_penemy_x);
			p_xlevel=calc_xlevel7(1, chr_xs[0]);
			t_penemy->delay =(p_xlevel==xlevel) ?0x38:0x18;
			t_penemy->still++;
			t_penemy->delay_btwn_fire =4;
			t_penemy->bullets =(t_penemy->flag2>>1) &3;
			if (p_xlevel!=xlevel)
				t_penemy->bullets =0;
		}
	}
}

void e_grenadier_st3(){
	e_runner_st3();
}

void e_grenadier_st4(){
	e_runner_st4();
}

void e_grenadier_st5(){
	bombing_init(0);
}

void e_grenadier_st6(){
	bombing_proc(2);
}

void e_grenadier_st7(){
	enemy_selfclear();
	grenadier_active=0;
}

void e_runner4p_st1(){
	set_ze_pos_xspd(2);
	set_r4p_stop_delay();
	t_penemy->state++;
}

void e_runner4p_st2(){
	static unsigned char delays[]={
		0x3f,0x39,0x33,0x2d,0x18,0x10,0x10,0x18,0xff,0xff,0xff,0xff
	};
	int idx;

	if (--t_penemy->delay){
		if (t_penemy->delay==4)
			gen_zbullet();
	}else{
		if (t_penemy->action_no==1 && t_penemy->sub_no>=2)
			t_penemy->xspeed =-t_penemy->xspeed;
		idx =(t_penemy->action_no<<2) +t_penemy->sub_no;
		t_penemy->delay =delays[idx];
		t_penemy->state++;
	}
}

void e_runner4p_st3(){
	zenemy_form();
	rfsh_ze_x_chk_bound();
	if (--t_penemy->delay)
		return;
	*t_penemy_dform =0x96;
	t_penemy->action_no++;
	set_r4p_stop_delay();
	if (t_penemy->state)
		t_penemy->state=2;
}

void e_runner4p_st4(){
	e_runner_st3();
}

void e_runner4p_st5(){
	e_runner_st4();
}

void e_runner4p_st6(){
	bombing_init(0);
}

void e_runner4p_st7(){
	bombing_proc(2);
}

void e_runner4p_st8(){
	enemy_selfclear();
}

void set_r4p_stop_delay(){
	static unsigned char delays[]={
		0x01,0x07,0x0d,0x13,0x18,0x18,0x18,0x18,0x10,0x18,0x18,0x10
	};
	int idx;

	idx =(t_penemy->action_no<<2) +t_penemy->sub_no;
	t_penemy->delay =delays[idx];
}

void zenemy_form(){
	if (!(framecount&3)){
		t_penemy->form_idx++;
		if (t_penemy->form_idx>=3)
			t_penemy->form_idx=0;
	}
	*t_penemy_dform =0x93+t_penemy->form_idx;
	if (t_penemy->xspeed<0)
		*t_penemy_dattr |=0x40;
	else
		*t_penemy_dattr &=0xbf;
}

void set_ze_pos_xspd(int idx){
	static short xspds[]={-0xe0, -0xc0, -0xc0, -0xc0};

	if (t_penemy->flag2 & ENEMY_LEFT_SOLDIER){
		t_penemy->xspeed =-xspds[idx];
		*t_penemy_x =0x58;
	}else{
		t_penemy->xspeed =xspds[idx];
		*t_penemy_x =0xa8;
	}
	*t_penemy_y =0x6d;
}

void rfsh_ze_x_chk_bound(){
	t_penemy->x_acum &=0xff;
	t_penemy->x_acum +=t_penemy->xspeed;
	*t_penemy_x +=t_penemy->x_acum>>8;
	if ((t_penemy->xspeed>0 && *t_penemy_x>=0xb0) ||
		(t_penemy->xspeed<0 && *t_penemy_x<0x50)){
		enemy_selfclear();
		return;
	}
	if (*t_penemy_x<0x60 || *t_penemy_x>=0xa0)
		*t_penemy_dattr |=0x20;
	else
		*t_penemy_dattr &=0xdf;
}

void e_type1518gen_st1(){
	t_penemy->delay=0x40;
	t_penemy->state++;
}

void e_type1518gen_st2(){
	static int newtypes[]={0x15, 0x16, 0x18, 0x17};
	unsigned char **pgendatas, *pgendata, c;
	int flag2, type, gentype;
	int no, i;
	struct ENEMY *pnewenemy;

	if (framecount&1)
		return;
	if (grenadier_active)
		return;
	if (--t_penemy->delay)
		return;
	pgendatas =t1518_gdatas[t_penemy->flag2];
	pgendata =pgendatas[screen_page];
	c=pgendata[t_penemy->data_ptr++];
	flag2 =c&0x3f;
	gentype =c>>6;
	c=pgendata[t_penemy->data_ptr++];
	if (c&0x80){
		t_penemy->data_ptr=0;
		if (++enemy_rounds==7){
			enemy_selfclear();
			return;
		}
	}
	t_penemy->delay =c&0x7f;
	type =newtypes[gentype];
	if (gentype !=2){
		if ((no=find_free_emy())<0)
			return;
		pnewenemy =&enemy[no];
		enemy_common_init(no, type);
		pnewenemy->flag2=flag2;
	}else{
		for (i=3; i>=0; i--){
			if ((no=find_free_emy())<0)
				return;
			pnewenemy =&enemy[no];
			enemy_common_init(no, type);
			pnewenemy->flag2=flag2;
			pnewenemy->sub_no =i;
		}
	}
}

void e_type1518gen_st3(){
	enemy_selfclear();
}

void e_rolling_ggen_st1(){
	t_penemy->delay=0x60;
	t_penemy->state++;
}

void e_rolling_ggen_st2(){
	static unsigned char gendata0[]={
		0x00,0x00,0x10,0x00,0x20,0x00,0x30,0x00,0x40,0x00,0x50,0x00,0x60,0xf0,0x01,0x00,
		0x11,0x00,0x21,0x00,0x31,0x00,0x41,0x00,0x51,0x00,0x61,0xf0,0x30,0x10,0x20,0x00,
		0x40,0x10,0x10,0x00,0x50,0x10,0x00,0x00,0x60,0xf0,0x00,0x00,0x60,0x10,0x10,0x00,
		0x50,0x10,0x20,0x00,0x40,0x10,0x30,0xf0,0xff
	};
	static unsigned char gendata1[]={
		0x00,0x00,0x20,0x00,0x40,0x00,0x60,0xf0,0x10,0x00,0x30,0x00,0x50,0xf0,0xff
	};
	static unsigned char *gendatas[]={gendata0, gendata1};
	static int gen_xs[]={0x98,0x90,0x88,0x80,0x78,0x70,0x68};
	unsigned char *pgendata, c, xlevel;
	int gx,gy;

	if (enemy_rounds>=7){
		t_penemy->state++;
		return;
	}
	if ((framecount&1) || --t_penemy->delay)
		return;
	pgendata =gendatas[t_penemy->flag2];
	while (t_penemy->delay==0){
		c=pgendata[t_penemy->data_ptr++];
		if (c==0xff){
			t_penemy->data_ptr=0;
			c=pgendata[t_penemy->data_ptr++];
		}
		t_penemy->delay =pgendata[t_penemy->data_ptr++];
		xlevel =c>>4;
		gx =gen_xs[xlevel];	gy=0x70;
		gen_rolling_g(xlevel,gx,gy);
	}
}

void e_rolling_ggen_st3(){
	enemy_selfclear();
}

void gen_rolling_g(int xlevel, int x, int y){
	static short xspds[]={0x55, 0x38, 0x1c, 0x00, -0x1c, -0x38, -0x55};
	struct ENEMY *pnewenemy;
	int no, idx;

	if (!player_normal)
		return;
	if ((no=find_free_emy())<0)
		return;
	idx=0xa+no;
	pnewenemy =&enemy[no];
	enemy_common_init(no, 0x11);
	chr_xs[idx]=x;	chr_ys[idx]=y;
	pnewenemy->xspeed=xspds[xlevel];
	pnewenemy->yspeed=0x0080;
}

void gen_grenade(){
	static short xspds[]={0x55, 0x38, 0x1c, 0x00, -0x1c, -0x38, -0x55};
	struct ENEMY *pnewenemy;
	int no, idx;
	int xlevel;

	if (!player_normal)
		return;
	xlevel =calc_xlevel7(0, *t_penemy_x);
	if ((no=find_free_emy())<0)
		return;
	idx=0xa+no;
	pnewenemy =&enemy[no];
	enemy_common_init(no, 0x12);
	chr_xs[idx]=*t_penemy_x;	chr_ys[idx]=*t_penemy_y;
	pnewenemy->xspeed=xspds[xlevel];
	pnewenemy->yspeed=0x0080;
}

void gen_zbullet(){
	static short xspds[]={0xd4, 0x8d, 0x46, 0x00, -0x46, -0x8d, -0xd4};
	struct ENEMY *pnewenemy;
	int no, idx;
	int xlevel;

	if (!player_normal)
		return;
	if (*t_penemy_x<0x60 || *t_penemy_x>=0xa0)
		return;
	xlevel =calc_xlevel7(0, *t_penemy_x);
	if ((no=find_free_emy())<0)
		return;
	idx=0xa+no;
	pnewenemy =&enemy[no];
	enemy_common_init(no, 0x01);
	chr_xs[idx]=*t_penemy_x;	chr_ys[idx]=*t_penemy_y;
	pnewenemy->xspeed=xspds[xlevel];
	pnewenemy->yspeed=0x0140;
}

void e_s1boss_st1(){
	t_penemy->delay=0x40;
	t_penemy->state++;
}

void e_s1boss_st2(){
	if (b_robject_clred<4)
		return;
	if (--t_penemy->delay)
		return;
	t_penemy->xspeed =0x0140;
	t_penemy->utemp =0x10;
	t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	t_penemy->delay_btwn_fire =0x20;
	t_penemy->delay=0xc0;
	t_penemy->state++;
}

void e_s1boss_st3(){
	static unsigned char forms[]={0x5d,0x5e,0x5f,0x5e,0x60,0x61,0x62,0x61};
	static unsigned char delays[]={0x70,0x50,0x40,0x28};
	int idx;

	if (t_penemy->delay){
		t_penemy->delay--;
		idx =framecount&4;
	}else
		idx=0;
	t_penemy->form_idx++;
	*t_penemy_dform =forms[((t_penemy->form_idx>>3)&3) +idx];
	refresh_enemyxy();
	if ((t_penemy->xspeed<0 && *t_penemy_x<0x50) ||
		(t_penemy->xspeed>=0 && *t_penemy_x>=0xb0))
		t_penemy->xspeed =-t_penemy->xspeed;
	if (player_normal && --t_penemy->delay_btwn_fire==0){
		t_penemy->delay_btwn_fire =delays[diff_extra];
		gen_new_enemy (0,0, 0x1b);
	}
}

void e_s1boss_st4(){
	if (--t_penemy->utemp){
		if (t_penemy->utemp==1)
			t_penemy->score_idx=5;
		PLAYSOUND3(HITSND1);
		t_penemy->hp=1;
		t_penemy->delay=0x20;
		t_penemy->state=3;
	}else{
		t_penemy->state++;
	}
}

void e_s1boss_st5(){
	stop_bgm();
	stageclear_init(BOMBING3);
	enemy_allclear();
	bombing_init(0);
}

void e_s1boss_st6(){
	bombing_proc(0);
}

void e_s1boss_st7(){
	*t_penemy_dform=0;
	t_penemy->state++;
	long_delay=0x60;
	enemy_selfclear();
}

void e_big_orb_st1(){
	int dir, yxflag;
	yxflag=get_dir_aimed(*t_penemy_x, *t_penemy_y, 1, &dir);
	short xs, ys;

	get_eb_spd(dir, 6, yxflag, &xs, &ys);
	t_penemy->xspeed =xs;
	t_penemy->yspeed =ys;
	t_penemy->state++;
}

void e_big_orb_st2(){
	static unsigned char xyflips[]={0, 0x40, 0xc0, 0x80};
	if (*t_penemy_y<0x48)
		*t_penemy_dform =0x63;
	else{
		*t_penemy_dform =0x64;
		t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	}
	*t_penemy_dattr = (*t_penemy_dattr&0x3f) | xyflips[(framecount>>2)&3];
	refresh_enemyxy();
}

void e_big_orb_st3(){
	bombing_init(0);
}

void e_big_orb_st4(){
	bombing_proc(0);
}

void e_big_orb_st5(){
	enemy_selfclear();
}

void e_s3boss_st1(){
	t_penemy->utemp =0xa;
	t_penemy->basex =*t_penemy_x;
	t_penemy->xspeed =0x0080;
	t_penemy->delay =0x40;
	t_penemy->state++;
}

void e_s3boss_st2(){
	if (b_robject_clred>=3 && --t_penemy->delay==0){
		t_penemy->delay_btwn_fire =0xa0;
		t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
		t_penemy->delay =0x20;
		t_penemy->state++;
	}
}

void e_s3boss_st3(){
	static unsigned char forms[]={0x68,0x69,0x6a,0x68,0x6b,0x6c};
	static unsigned char delays[]={0x8a,0xa9,0x63,0xd7};
	int idx, delta;
	int skip_refresh;

	if (!(framecount&7)){
		t_penemy->form_idx++;
		if (t_penemy->form_idx>=3)
			t_penemy->form_idx=0;
	}
	idx =t_penemy->low_hp;
	if (t_penemy->break_cnt){
		t_penemy->break_cnt--;
		if (t_penemy->break_cnt &1)
			idx ^=1;
	}
	idx =(idx) ?(t_penemy->form_idx+3):t_penemy->form_idx;
	*t_penemy_dform =forms[idx];
	if (player_normal && --t_penemy->delay_btwn_fire==0){
		t_penemy->delay_btwn_fire =
			delays[((framecount+get_rand8())>>1)&3]-(diff_extra<<3);
		gen_new_enemy (0,0, 0x1d);
	}
	skip_refresh=0;
	if (t_penemy->delay){
		if (--t_penemy->delay)
			skip_refresh=1;
		else
			t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	}
	if (!skip_refresh){
		t_penemy->yspeed +=t_penemy->xspeed;
		if (t_penemy->xspeed>0){
			if (t_penemy->yspeed>=0x3000){
				t_penemy->delay=0x20;
				t_penemy->xspeed =-t_penemy->xspeed;
			}
		}else{
			if (t_penemy->yspeed<0){
				t_penemy->yspeed =0;
				t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
				t_penemy->delay=0x30;
				t_penemy->xspeed =-t_penemy->xspeed;
			}
		}
	}
	delta =t_penemy->yspeed>>8;
	if (framecount &1)
		*t_penemy_x =t_penemy->basex -delta;
	else
		*t_penemy_x =t_penemy->basex +delta;
}

void e_s3boss_st4(){
	if (t_penemy->delay)
		t_penemy->delay--;
	if (--t_penemy->utemp==0){
		t_penemy->state++;
		return;
	}
	if (t_penemy->utemp<7){
		if (t_penemy->utemp==1)
			t_penemy->score_idx =5;
		t_penemy->low_hp=1;
	}
	t_penemy->hp=1;
	t_penemy->break_cnt =0x10;
	PLAYSOUND3(HITSND1);
	if (t_penemy->state)
		t_penemy->state=3;
}

void e_s3boss_st5(){
	if (--s_robject_left)
		bombing_init(0);
	else{
		stop_bgm();
		stageclear_init(BOMBING3);
		enemy_allclear();
		bombing_init(0);
	}
}

void e_s3boss_st6(){
	bombing_proc(0);
}

void e_s3boss_st7(){
	if (s_robject_left)
		enemy_selfclear();
	else{
		*t_penemy_dform=0;
		long_delay=0x60;
		enemy_selfclear();
	}
}

void e_homingorb_st1(){
	static int spdadjs[]={1,3,4,5};
	int dir, spdadj, yxflag;
	short xs, ys;

	t_penemy->spd_idx =framecount &3;
	spdadj =spdadjs[t_penemy->spd_idx];
	yxflag=get_dir_aimed(*t_penemy_x, *t_penemy_x, 1, &dir);
	get_eb_spd(dir, spdadj, yxflag, &xs, &ys);
	t_penemy->xspeed =xs;
	t_penemy->yspeed =ys;
	get_dir_aimed_adjust(*t_penemy_x, *t_penemy_x, 1, &dir);
	t_penemy->aimed_dir =dir;
	t_penemy->delay_btwn_fire=0x20;	//借用变量，实际意思是delay_btwn_homing
	t_penemy->state++;
}

void e_homingorb_st2(){
	static unsigned char delays[]={8,6,4,2};
	static short yxspds[]={
		0x0000, 0x0063, 0x00c0, 0x010f, 0x014b, 0x0172, 0x017e, 0x0172,
		0x014b, 0x010f, 0x00c0, 0x0063, 0x0000, -0x0063,-0x00c0,-0x010f,
		-0x014b,-0x0172,-0x017e,-0x0172,-0x014b,-0x010f,-0x00c0,-0x0063,
		0x0000, 0x0063, 0x00c0, 0x010f, 0x014b, 0x0172, 
	};
	int dummy;

	if (++t_penemy->delay>=delays[t_penemy->spd_idx]){
		t_penemy->delay=0;
		t_penemy->form_idx++;
		if (t_penemy->form_idx>=6)
			t_penemy->form_idx=0;
	}
	*t_penemy_dform =t_penemy->form_idx+0x6d;
	refresh_enemyxy();
	if (t_penemy->utemp>=0x14 || --t_penemy->delay_btwn_fire)
		return;
	t_penemy->delay_btwn_fire =8;
	t_penemy->utemp++;
	if (get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy)==0){
		t_penemy->spd_idx |=3;
		t_penemy->yspeed =yxspds[t_penemy->aimed_dir];
		t_penemy->xspeed =yxspds[6+t_penemy->aimed_dir];
	}
}

void e_homingorb_st3(){
	bombing_init(0);
}

void e_homingorb_st4(){
	bombing_proc(0);
}

void e_homingorb_st5(){
	enemy_selfclear();
}

void e_w_soldier_st1(){
	static int xys[]={0x9c,0xf0,0x9c,0x10,0x61,0xf0,0x61,0x10};
	static short xspds[]={-0x0100, 0x0100};
	int *pxys =&xys[t_penemy->flag2<<1];

	*t_penemy_y =*pxys++;
	*t_penemy_x =*pxys;
	t_penemy->xspeed =xspds[t_penemy->flag2&1];
	t_penemy->state++;
}

void e_w_soldier_st2(){
	w_soldier_form();
	*t_penemy_dform =0x85 +t_penemy->form_idx;
	*t_penemy_dattr = (t_penemy->flag2&1) ?0x47:7;
	w_soldier_attr();
	refresh_enemyxy();
	if (*t_penemy_x>=0x28 && *t_penemy_x<0xd8){
		if (get_p_e_xdist()<0x10){
			t_penemy->form_idx=0;
			t_penemy->delay=1;
			if (t_penemy->state)
				t_penemy->state++;
		}
	}
}

void e_w_soldier_st3(){
	static short xspds[]={-0x0040, 0x0040};

	if (--t_penemy->delay)
		return;
	t_penemy->delay =8;
	*t_penemy_dform =0x88 +t_penemy->form_idx;
	t_penemy->form_idx++;
	if (t_penemy->form_idx>=3){
		*t_penemy_dattr &=0xdf;
		t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
		t_penemy->xspeed =xspds[t_penemy->flag2&1];
		t_penemy->yspeed =-0x0100;
		t_penemy->delay=0x10;
		t_penemy->state++;
	}
}

void e_w_soldier_st4(){
	if (t_penemy->delay){
		t_penemy->delay--;
		*t_penemy_dform =0x8a;
	}else
		*t_penemy_dform =0x8b;
	t_penemy->yspeed +=0x10;
	refresh_enemyxy();
}

void e_w_soldier_st5(){
	bombing_init(0);
}

void e_w_soldier_st6(){
	bombing_proc(0);
}

void e_w_soldier_st7(){
	enemy_selfclear();
}

void w_soldier_form(){
	if (!(framecount&3)){
		t_penemy->form_idx++;
		if (t_penemy->form_idx>=3)
			t_penemy->form_idx=0;
	}
}

void w_soldier_attr(){
	unsigned char prio =(*t_penemy_x>=0xdc || *t_penemy_x<0x24) ?0x20:0;

	*t_penemy_dattr = (*t_penemy_dattr & 0xdf) | prio;
}

void e_r_sniper_st1(){
	e_w_soldier_st1();
	t_penemy->utemp =0;
}

void e_r_sniper_st2(){
	int xjudge;
	w_soldier_form();
	*t_penemy_dform =0x8c +t_penemy->form_idx;
	*t_penemy_dattr = (t_penemy->flag2&1) ?0x46:6;
	w_soldier_attr();
	refresh_enemyxy();
	if (t_penemy->utemp)
		return;
	if (*t_penemy_x<0x28 || *t_penemy_x>=0xd8)
		return;
	xjudge =(t_penemy->flag2&2) ?0x30:0x10;
	if (get_p_e_xdist()<xjudge){
		*t_penemy_dform =0x8f;
		t_penemy->bullets=3;
		t_penemy->delay_btwn_fire =0x10;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_r_sniper_st3(){
	if (--t_penemy->delay_btwn_fire){
		if (t_penemy->delay_btwn_fire==0x2c)
			*t_penemy_dattr &=0xf7;
		return;
	}
	*t_penemy_dform =0x90;
	if (--t_penemy->bullets>=0){
		t_penemy->delay_btwn_fire =0x30;
		*t_penemy_dattr |=8;
		fire_eb_aimed_dir18(*t_penemy_x, *t_penemy_y, 0, 4);
	}else{
		t_penemy->utemp++;
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_r_sniper_st4(){
	bombing_init(0);
}

void e_r_sniper_st5(){
	bombing_proc(0);
}

void e_r_sniper_st6(){
	enemy_selfclear();
}

void e_type1e1fgen_st1(){
	t_penemy->delay =0x80;
	t_penemy->data_ptr=0;
	t_penemy->state++;
}

void e_type1e1fgen_st2(){
	static unsigned char gendata[]={
		0x00,0x01,0x02,0x03,0xd0,0x06,0x07,0xa0,0x04,0x05,0xc0,0x00,0x01,0xb0,0x02,0x03,
		0xd0,0x04,0x05,0x06,0x07,0xd0,0x00,0x01,0x02,0x03,0xfe,0xff,
	};
	unsigned char c, type, flag2;
	int no;

	if (b_robject_clred==3){
		enemy_selfclear();
		return;
	}
	if ((framecount&1) || --t_penemy->delay)
		return;
	c=gendata[t_penemy->data_ptr++];
	if (c==0xff){
		t_penemy->data_ptr=0;
		c=gendata[t_penemy->data_ptr++];
	}
	while (!(c&0x80)){
		flag2 =c&3;
		type =(c>>2) ?0x1e:0x1f;
		if ((no=find_free_emy())>=0){
			enemy_common_init(no, type);
			enemy[no].flag2 =flag2;
		}
		c=gendata[t_penemy->data_ptr++];
	}
	t_penemy->delay =(c&0x7f)<<1;
}

void e_type1e1fgen_st3(){
	enemy_selfclear();
}

void e_stonegen_st1(){
	move_enemy_scroll();
	if (t_penemy->state)
		t_penemy->state++;
}

void e_stonegen_st2(){
	move_enemy_scroll();
	t_penemy->delay=8;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_stonegen_st3(){
	refresh_enemyxy();
	if (--t_penemy->delay==0){
		t_penemy->delay=0xe0;
		gen_new_enemy (0,0, 0x13);
	}
}

void e_stone_st1(){
	t_penemy->delay=0x40;
	t_penemy->state++;
}

void e_stone_st2(){
	*t_penemy_dform=0x4a;
	refresh_enemyxy();
	if (!(framecount &3)){
		*t_penemy_x +=(framecount &4) ?1 :-1;
	}
	if (--t_penemy->delay==0){
		t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
		t_penemy->delay=1;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_stone_st3(){
	e_stone_form();
	if (*t_penemy_y>=t_penemy->free_dist && check_e_bg(0, 8)!=0){
		PLAYSOUND3(STONE_LANDING);
		t_penemy->free_dist =*t_penemy_y+0x10;
		if (t_penemy->free_dist>0xff)
			t_penemy->free_dist=0xff;
		t_penemy->yspeed =-0x140;
	}
	t_penemy->yspeed +=0x10;
	t_penemy->free_dist +=scroll_to_do;
	if (t_penemy->free_dist>0xff)
		t_penemy->free_dist=0xff;
	refresh_enemyxy();
}

void e_stone_st4(){
	bombing_init(0);
}

void e_stone_st5(){
	bombing_proc(0);
}

void e_stone_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_stone_form(){
	static unsigned char attrs[]={0x00,0x40,0xc0,0x80};

	 *t_penemy_dattr=(*t_penemy_dattr & 0x3f) | attrs[(framecount>>2)&3];
	 *t_penemy_dform =0x4a;
}

void e_aircar_st1(){
	static short spds[]={-0x80, 0xc0, -0x80, 0x80};
	static int limits[]={0x50,0xb0,0x70,0xc0,0x48,0xb8,0x48,0xb8};

	t_penemy->xspeed =spds[t_penemy->flag2];
	t_penemy->left_limit =limits[t_penemy->flag2<<1];
	t_penemy->right_limit =limits[(t_penemy->flag2<<1)+1];
	move_enemy_scroll();
	if (t_penemy->state)
		t_penemy->state++;
}

void e_aircar_st2(){
	*t_penemy_dform=0x48;
	refresh_enemyxy();
	if ((t_penemy->xspeed<0 && *t_penemy_x<t_penemy->left_limit) ||
		(t_penemy->xspeed>0 && *t_penemy_x>=t_penemy->right_limit)
		)
		t_penemy->xspeed =-t_penemy->xspeed;
}

void e_fire_st1(){
	e_aircar_st1();
}

void e_fire_st2(){
	*t_penemy_dform=0x49;
	*t_penemy_dattr =(framecount&8) ?0x40:0;
	refresh_enemyxy();
	if ((t_penemy->xspeed<0 && *t_penemy_x<t_penemy->left_limit) ||
		(t_penemy->xspeed>0 && *t_penemy_x>=t_penemy->right_limit)
		)
		t_penemy->xspeed =-t_penemy->xspeed;
}

void e_s2boss_st1(){
	t_penemy->utemp =0x20;
	t_penemy->break_cnt =2;
	t_penemy->form_idx =1;
	t_penemy->delay =0xff;
	t_penemy->state++;
}

void e_s2boss_st2(){
	move_enemy_scroll();
	if (boss_ascroll_done && bright_count==0 && --t_penemy->delay==0){
		t_penemy->delay=1;
		t_penemy->state++;
	}
}

void e_s2boss_st3(){
	if (!s2boss_mouth())
		return;
	if (t_penemy->form_idx<2)
		t_penemy->form_idx++;
	else{
		t_penemy->flag1 &=~ENEMY_NONSHOOTABLE;
		t_penemy->hp =t_penemy->utemp;
		t_penemy->delay_btwn_fire =6;
		t_penemy->delay =0x70;
		t_penemy->state++;
	}
}

void e_s2boss_st4(){
	static unsigned char dirs[]={0x88, 0x86, 0x84};
	int i;
	int bx,by, spdadj;

	move_enemy_scroll();
	if (t_penemy->delay_btwn_fire && --t_penemy->delay_btwn_fire==0){
		for (i=2; i>=0; i--){
			new_e_pos(0,8, &bx,&by);
			spdadj =(arms_clred<2) ?6:7;
			fire_eb(1, bx,by,dirs[i],spdadj);
		}
	}
	if (--t_penemy->delay==0){
		t_penemy->flag1 |=ENEMY_NONSHOOTABLE;
		t_penemy->utemp =t_penemy->hp;
		t_penemy->hp =0xf1;
		t_penemy->delay =6;
		t_penemy->state++;
	}
}

void e_s2boss_st5(){
	static unsigned char delays[]={0xc0, 0x70, 0x20};
	int idx;

	if (!s2boss_mouth())
		return;
	if (t_penemy->form_idx!=0)
		t_penemy->form_idx--;
	else{
		idx =arms_clred;
		if (idx>2)
			idx=2;
		t_penemy->delay =delays[idx];
		t_penemy->state=3;
	}
}

void e_s2boss_st6(){
	stop_bgm();
	stageclear_init(BOMBING3);
	enemy_allclear();
	bombing_init(0);
}

void e_s2boss_st7(){
	bombing_proc(0);
}

void e_s2boss_st8(){
	*t_penemy_dform=0;
	t_penemy->state++;
}

void e_s2boss_st9(){
	static int bxs[]={
		0x50,0xb0,0x70,0x90,0x70,0x90,0x70,0x90,0x70,0x90,0x70,0x90,0x70,0x90	
	};
	static int bys[]={
		0x20,0x20,0x20,0x20,0x40,0x40,0x60,0x60,0x80,0x80,0xa0,0xa0,0xc0,0xc0
	};
	static unsigned char bnos[]={
		0x19,0x19,0x19,0x19,0x1a,0x1b,0x29,0x2a,0x1c,0x1d,0x1e,0x1f,0x26,0x27
	};
	if (--t_penemy->break_cnt==0){
		t_penemy->break_cnt=1;
		*t_penemy_x =bxs[t_penemy->bombing_idx];
		*t_penemy_y =bys[t_penemy->bombing_idx];
		enemy_bgobj_4x4(bnos[t_penemy->bombing_idx]);
		bombing_gen(*t_penemy_x, *t_penemy_y);
		if (++t_penemy->bombing_idx>=0xe){
			long_delay=0x60;
			enemy_selfclear();
		}
	}
}

int s2boss_mouth(){
	static unsigned char bnos[]={0xa0,0xa1,0xa2,0xa3,0xa4,0xa5};
	unsigned char *pbnos;

	move_enemy_scroll();
	if (--t_penemy->delay)
		return 0;
	pbnos =&bnos[t_penemy->form_idx<<1];
	enemy_bgobj_4x4xv2(pbnos[0], pbnos[1], 1);
	t_penemy->delay=6;
	return 1;
}

void e_armseg_st1(){
	int left =t_penemy->flag2 & ENEMY_LEFT_SOLDIER;
	if (left){
		t_penemy->seg_rotno_local =0x28;
		t_penemy->delay_btwn_fire =0x28;
		*t_penemy_x -=8;
	}else{
		t_penemy->seg_rotno_local =0x38;
		t_penemy->delay_btwn_fire =0x38;
		*t_penemy_x +=8;
	}
	t_penemy->utemp =4;
	t_penemy->seg_prev =0xff;
	t_penemy->seg_last =t_enemy_no;
	t_penemy->state++;
}

void e_armseg_st2(){
	int no, idx;
	struct ENEMY* penemy;

	move_enemy_scroll();
	if (boss_ascroll_done && !bright_count && t_penemy->seg_prev ==0xff){
		if ((no=find_free_emy())<0)
			return;
		idx =0xa+no;
		penemy =&enemy[no];
		enemy_common_init(no, 0x15);

		penemy->state =2;
		penemy->flag1 =ENEMY_NONSHOOTABLE | ENEMY_BOMBING_TYPE1 | ENEMY_HITSND;
		penemy->score_idx =5;
		penemy->col_type =2;
		penemy->hp =0xf1;
		penemy->flag2 =t_penemy->flag2;
		chr_xs[idx] =*t_penemy_x;
		chr_ys[idx] =*t_penemy_y;
		penemy->seg_prev =t_penemy->seg_last;
		enemy[t_penemy->seg_last].seg_next =no;
		t_penemy->seg_last =no;

		if (--t_penemy->utemp==0){
			penemy->seg_next =0xff;
			penemy->hp =0x10;
			penemy->flag1 &=~ENEMY_NONSHOOTABLE;
			penemy->seg_active_cnt =1;
			penemy->delay =0x20;
			t_penemy->endseg_no =no;
			no=t_enemy_no;
			do{
				enemy[no].state++;
				no=enemy[no].seg_next;
			}while (no !=0xff);
			t_penemy->seg_active_cnt=0;
		}
	}//if (boss_ascroll_done && !bright_count && t_penemy->seg_prev ==0xff)
}

void e_armseg_st3(){
	static short yxspds[]={-0x00b5, 0x00b5, -0x00b5, -0x00b5};
	short *pspds;
	int no;
	struct ENEMY *penemy;

	move_enemy_scroll();
	if (t_penemy->delay){
		if (framecount)
			t_penemy->delay--;
		return;
	}
	if (!t_penemy->seg_active_cnt)
		return;
	*t_penemy_dform =(t_penemy->seg_next ==0xff) ?0x7b:0x7a;
	pspds =&yxspds[(t_penemy->flag2 & ENEMY_LEFT_SOLDIER)<<1];
	t_penemy->y_acum = (t_penemy->y_acum&0xff) + *pspds++;
	*t_penemy_y +=t_penemy->y_acum>>8;
	t_penemy->x_acum = (t_penemy->x_acum&0xff) + *pspds;
	*t_penemy_x +=t_penemy->x_acum>>8;
	if (t_penemy->seg_active_cnt==0xff)
		return;
	if (++t_penemy->seg_active_cnt<0x10)
		return;
	t_penemy->seg_active_cnt=0xff;
	no=t_penemy->seg_prev;
	penemy =&enemy[no];
	penemy->seg_active_cnt =1;
	penemy->delay=0;
	if (penemy->seg_prev==0xff){
		do{
			enemy[no].state++;
			no=enemy[no].seg_next;
		}while (no !=0xff);
	}
}

void e_armseg_st4(){
	*t_penemy_dform =(t_penemy->seg_next ==0xff) ?0x7b:0x7a;
	if (t_penemy->seg_prev !=0xff)
		return;
	e_seg_proc();
	if (t_penemy->seg_rot_state!=4)
		rotete_segs();
	set_segs_pos();
}

void e_armseg_st5(){
	unsigned char no, prevno;

	if (t_penemy->seg_next==0xff){
		arms_clred++;
		no=t_enemy_no;
		while ((prevno=enemy[no].seg_prev)!=0xff){
			e_death_state(&enemy[prevno]);
			no=prevno;
		}
	}
	t_penemy->state++;
}

void e_armseg_st6(){
	bombing_init(0);
}

void e_armseg_st7(){
	bombing_proc(0);
}

void e_armseg_st8(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_seg_proc(){
	static unsigned char rlimits[]={0x14,0x0c,0x2c,0x34};
	static char momentums[]={0x40, -0x40, 0x40};
	static unsigned char rlimits2[]={0x08,0x38};
	static unsigned char delays[]={0x40,0x60,0x30,0x70};
	int left, no;
	struct ENEMY *penemy;

	switch (t_penemy->seg_rot_state){
	case 0:
		seg_fire_proc();
		left =t_penemy->flag2 & ENEMY_LEFT_SOLDIER;
		if (!t_penemy->seg_up){
			if (t_penemy->seg_rotno_local==rlimits[left]){
				t_penemy->delay =3;
				t_penemy->seg_up ^=1;
			}else
				t_penemy->seg_momentum =momentums[left];
		}else{
			if (t_penemy->seg_rotno_local==rlimits[left+2]){
				if (++t_penemy->seg_swing_cnt==3)
					t_penemy->seg_rot_state++;
				t_penemy->delay =3;
				t_penemy->seg_up ^=1;
			}else
				t_penemy->seg_momentum =momentums[left+1];
		}
		break;
	case 1:
		seg_fire_proc();
		if (!t_penemy->seg_momentum_ored)
			t_penemy->seg_rot_state++;
		break;
	case 2:
		seg_fire_proc();
		left =t_penemy->flag2 & ENEMY_LEFT_SOLDIER;
		no =t_penemy->seg_next;
		penemy =&enemy[no];
		do{
			if (penemy->seg_rotno_local!=rlimits2[left]){
				penemy->seg_momentum =momentums[left];
				return;
			}
			no =penemy->seg_next;
			if (no!=0xff)
				penemy =&enemy[no];
		}while (no!=0xff);
		t_penemy->break_cnt =delays[(framecount+get_rand8()) &3];
		t_penemy->seg_rot_state++;
		break;
	case 3:
		seg_fire_proc();
		if (--t_penemy->break_cnt==0){
			t_penemy->break_cnt=0xc0;
			t_penemy->seg_rot_state++;
		}
		break;
	case 4:
		aim_segs();
		if (--t_penemy->break_cnt==0){
			t_penemy->seg_up =0;
			t_penemy->seg_swing_cnt =0;
			t_penemy->seg_rot_state=0;
		}
		break;
	}
}

void seg_fire_proc(){
	int idx;

	if (--t_penemy->delay_btwn_fire==0){
		t_penemy->delay_btwn_fire=0x90;
		idx =0xa +t_penemy->endseg_no;
		fire_eb_aimed_dir18(chr_xs[idx],chr_ys[idx], 4,5);
	}
}

void aim_segs(){
	int no, dummy, prev_no, next_no, no_found;
	int r_mode;

	no=t_penemy->endseg_no;
	while (1){
		if ((prev_no=enemy[no].seg_prev)==0xff)
			return;
		r_mode =get_segdir_aimed_adjust(prev_no, &dummy);
		if (r_mode==2)
			no =prev_no;
		else{
			no_found =no;
			if (r_mode==0){
				while ((prev_no=enemy[no].seg_prev) !=0xff){
					if (enemy[no].seg_rotno_local!=8)
						break;
					no=prev_no;
				}
				if (no==no_found){
					if ((next_no =enemy[no].seg_next)!=0xff){
						enemy[next_no].seg_rotno_local =
							(enemy[next_no].seg_rotno_local-1) &0x3f;
					}
				}
				enemy[no].seg_rotno_local++;
			}else{
				while ((prev_no=enemy[no].seg_prev) !=0xff){
					if (enemy[no].seg_rotno_local!=0x38)
						break;
					no=prev_no;
				}
				if (no==no_found){
					if ((next_no =enemy[no].seg_next)!=0xff){
						enemy[next_no].seg_rotno_local =
							(enemy[next_no].seg_rotno_local+1) &0x3f;
					}
				}
				enemy[no].seg_rotno_local--;
			}
			enemy[no].seg_rotno_local &=0x3f;
			return;
		}//else of rmode==2
	}//while (1)
}

void rotete_segs(){
	char sum_rforce;
	char momentum_ored;
	int no;

	sum_rforce=0;
	momentum_ored=0;
	no =t_enemy_no;
	do{
		sum_rforce=rotete_segs_sub(no, sum_rforce);
		momentum_ored |=enemy[no].seg_momentum;
		no =enemy[no].seg_next;
	}while (no!=0xff);
	t_penemy->seg_momentum_ored =momentum_ored;
}

char rotete_segs_sub(int no, char sum_rforce){
	struct ENEMY *penemy =&enemy[no];
	char change_local, change_sofar, rforce_local;
	int i;

	if (penemy->delay){
		penemy->delay--;
		change_local=0;
	}else if (penemy->seg_momentum==0)
		change_local=0;
	else if (penemy->seg_momentum >0){
		penemy->seg_momentum--;
		change_local=1;
	}else{
		penemy->seg_momentum++;
		change_local=-1;
	}
	rforce_local=0;
	change_sofar =change_local+sum_rforce;
	if (!change_sofar)
		return sum_rforce;

	if (change_sofar>0){
		for (i=change_sofar; i>0; i--){
			if (penemy->seg_prev!=0xff && penemy->seg_rotno_local==8){
				if (change_local)
					penemy->seg_momentum=0;
				else{
					penemy->delay=1;
					penemy->seg_momentum--;
				}
			}else{
				rforce_local--;
				penemy->seg_rotno_local =(penemy->seg_rotno_local+1) & 0x3f;
			}
		}
	}else{
		for (i=change_sofar; i<0; i++){
			if (penemy->seg_prev!=0xff && penemy->seg_rotno_local==0x38){
				if (change_local)
					penemy->seg_momentum=0;
				else{
					penemy->delay=1;
					penemy->seg_momentum++;
				}
			}else{
				rforce_local++;
				penemy->seg_rotno_local =(penemy->seg_rotno_local-1) & 0x3f;
			}
		}
	}
	return (sum_rforce+rforce_local);
}

void set_segs_pos(){
	static char yxdeltas[]={
		0x00,0x01,0x03,0x04,0x06,0x07,0x08,0x0a,0x0b,0x0c,0x0d,0x0e,0x0e,0x0f,0x0f,0x0f,
		0x0f,0x0f,0x0f,0x0f,0x0e,0x0e,0x0d,0x0c,0x0b,0x0a,0x08,0x07,0x06,0x04,0x03,0x01,
		0x00,-0x01,-0x03,-0x04,-0x06,-0x07,-0x08,-0x0a,-0x0b,-0x0c,-0x0d,-0x0e,-0x0e,-0x0f,-0x0f,-0x0f,
		-0x0f,-0x0f,-0x0f,-0x0f,-0x0e,-0x0e,-0x0d,-0x0c,-0x0b,-0x0a,-0x08,-0x07,-0x06,-0x04,-0x03,-0x01,
		0x00,0x01,0x03,0x04,0x06,0x07,0x08,0x0a,0x0b,0x0c,0x0d,0x0e,0x0e,0x0f,0x0f,0x0f,
	};
	int no, nextno;

	no=t_enemy_no;
	t_penemy->seg_rotno_actual =t_penemy->seg_rotno_local;
	while ((nextno=enemy[no].seg_next)!=0xff){
		enemy[nextno].seg_rotno_actual =
			(enemy[no].seg_rotno_actual +enemy[nextno].seg_rotno_local)&0x3f;
		chr_ys[0xa+nextno] =chr_ys[0xa+no] +yxdeltas[enemy[nextno].seg_rotno_actual];
		chr_xs[0xa+nextno] =chr_xs[0xa+no] +yxdeltas[enemy[nextno].seg_rotno_actual+0x10];
		no=nextno;
	}
}

void e_pipebombgen_st1(){
	move_enemy_scroll();
	if (*t_penemy_x<0xc8){
		t_penemy->delay=1;
		t_penemy->state++;
	}
}

void e_pipebombgen_st2(){
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		t_penemy->delay=0x80;
		gen_new_enemy (0,0, 0x11);
	}
}

void e_pipebombgen_st3(){
	enemy_selfclear();
}

void e_pipebomb_st1(){
	PLAYSOUND4(PIPEBOMB);
	*t_penemy_dattr=0x20;
	t_penemy->xspeed =0x0080;
	t_penemy->yspeed =-0x0200;
	t_penemy->state++;
}

void e_pipebomb_st2(){
	static unsigned char forms[]={0x74,0x75,0x76,0x77};

	if (!(framecount&7))
		t_penemy->form_idx++;
	*t_penemy_dform=forms[t_penemy->form_idx &3];
	refresh_enemyxy();
	t_penemy->yspeed +=0xa;
	if (t_penemy->yspeed>=0){
		*t_penemy_dattr=0;
		if (check_e_bg(0,4)!=0){
			PLAYSOUND3(BOMBING1);
			if (t_penemy->state)
				t_penemy->state++;
		}
	}
}

void e_pipebomb_st3(){
	bombing_init(1);
}

void e_pipebomb_st4(){
	bombing_proc(0);
}

void e_pipebomb_st5(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_btank_st1(){
	*t_penemy_x =0x130;
	*t_penemy_y =0x90;
	t_penemy->aimed_dir =0xc;
	t_penemy->fire_cnt =6;		//借用变量, 实际含义是马达声间延迟
	disable_cg23 =1;
	btank_active =1;
	palcgs_spec[2] =0x3f;
	palcgs_spec[3] =0x41;
	update_pal_buf(0x10);
	t_penemy->state++;
}

void e_btank_st2_sub();
void e_btank_st2(){
	static short delays[]={0x100,0xf8};

	tank_ascroll =framecount &1;
	refresh_btankx();
	if (*t_penemy_x>=0xa0){
		e_btank_st2_sub();
	}else{
		tank_ascroll =0;
		t_penemy->break_cnt =delays[t_penemy->flag2 &1];
		t_penemy->hp =0x47;
		t_penemy->delay=8;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_btank_st2_sub(){
	static unsigned char wheel_bnos[]={0x10, 0x11, 0x14, 0x15};
	int bno, wx;

	if (t_penemy->hp && --t_penemy->fire_cnt==0){
		PLAYSOUND4(MOTOR);
		t_penemy->fire_cnt =6;
	}
	bno =wheel_bnos[framecount &3];
	if (framecount &1){
		wx =*t_penemy_x+0x14;
		if ((wx & 0xff00)==0){
			show_bgobj_4x4(wx, *t_penemy_y, bno);
		}
	}else{
		wx =*t_penemy_x-0xc;
		if ((wx & 0xff00)==0){
			show_bgobj_4x4(wx, *t_penemy_y, bno);
		}
	}
}

void e_btank_st3(){
	static unsigned char bnos[]={0x13, 0x12, 0x0f};
	static unsigned char xydirs[]={0x24,0x03,0x09,0x29,0x09,0x0a,0x2e,0x14,0x0c};
	unsigned char *pxydirs;
	char o_dir;
	int n_dir;
	int tx, ty, bx, by, bdir;
	int idx;

	btank_color_by_hp();
	refresh_btankx();
	if (btank_deep_left())
		return;
	if (!(framecount &1)){
		if (--t_penemy->break_cnt==0){
			if (t_penemy->state)
				t_penemy->state++;
			return;
		}
	}
	if (t_penemy->flag1 & ENEMY_NONSHOOTABLE)
		return;
	if (--t_penemy->delay)
		return;
	if (t_penemy->bullets){
		idx =t_penemy->aimed_dir-0xa;
		pxydirs =&xydirs[(idx<<1)+idx];
		bx =*t_penemy_x - *pxydirs++;
		if ((bx &0xff00)==0){
			by =*t_penemy_y - *pxydirs++;
			bdir =*pxydirs;
			fire_eb(1, bx,by, bdir, t_penemy->flag2);
			t_penemy->bullets--;
			t_penemy->delay =0x20;
		}
	}else{
		o_dir =t_penemy->aimed_dir;
		get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y-0xc, 1, &n_dir);
		if (t_penemy->aimed_dir<0xa || t_penemy->aimed_dir>=0xd){
			t_penemy->aimed_dir=o_dir;
			n_dir =o_dir;
		}
		if (t_penemy->aimed_dir==n_dir)
			t_penemy->bullets =3;
		ty =*t_penemy_y-0x1c;
		tx =*t_penemy_x-0x2c;
		if ((tx &0xff00)==0){
			show_bgobj_4x4(tx,ty,bnos[t_penemy->aimed_dir-0xa]);
			t_penemy->delay =0x30;
		}
	}
}

void e_btank_st4(){
	tank_ascroll =framecount &1;
	btank_color_by_hp();
	refresh_btankx();
	if (btank_deep_left())
		return;
	e_btank_st2_sub();
}

void e_btank_st5(){
	refresh_btankx();
	t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	t_penemy->bombing_idx=6;
	PLAYSOUND4(BOMBING4);
	t_penemy->delay =0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_btank_st6(){
	static short deltas[]={
		0x16,0x04,0x1c,0x0e,
		0x16,-0x1c,0x1c,-0x0e,
		-0x0a,0x04,0x00,0x0e,
		-0x0a,-0x1c,0x00,-0x0e,
		-0x2a,0x04,-0x1c,0x0e,
		-0x2a,-0x1c,-0x1c,-0x0e
	};
	short *pdeltas;
	int bx,by;

	tank_ascroll =framecount &1;
	refresh_btankx();
	if (btank_deep_left())
		return;
	if (t_penemy->delay){
		--t_penemy->delay;
		return;
	}
	if (t_penemy->bombing_idx==0)
		return;
	pdeltas =&deltas[(t_penemy->bombing_idx-1)<<2];
	bx =*t_penemy_x +*pdeltas++;
	if ((bx&0xff00)==0){
		by =*t_penemy_y +*pdeltas++;
		show_bgobj_4x4(bx,by,0x9b);
		bx =*t_penemy_x +*pdeltas++;
		by =*t_penemy_y +*pdeltas++;
		bombing_gen(bx,by);
		t_penemy->delay=4;
	}
	t_penemy->bombing_idx--;
}

void refresh_btankx(){
	int o_xh=*t_penemy_x &0xff00; 

	*t_penemy_x -=(scroll_to_do+tank_ascroll);
	if ( ((*t_penemy_x&0xff00) ^ o_xh) && t_penemy->hp)
		t_penemy->flag1 ^=(ENEMY_NONSHOOTABLE|ENEMY_NONCOL);
}

int btank_deep_left(){
	if (*t_penemy_x<-0x30){
		enemy_selfclear();
		tank_ascroll =0;
		disable_cg23 =0;
		btank_active =0;
		return 1;
	}else
		return 0;
}

void btank_color_by_hp(){
	static unsigned char clrs[]={0x61,0x60,0x5f,0x3f,0x3f};

	palcgs_spec[2] =clrs[t_penemy->hp>>4];
	update_pal_buf(0x10);
}

void e_arc_st1(){
	*t_penemy_dform =0xc4;
	if (btank_active){
		if (scroll_to_do)
			(*t_penemy_x)--;
	}else
		move_enemy_scroll();
}

void e_s4boss_st1(){
	*t_penemy_y =0x10;
	bright_count =0x10;
	update_pal_buf(0x10);
	t_penemy->state++;
}

void e_s4boss_st2(){
	static int xs[]={0x40, 0x60, 0x80, 0x80};

	*t_penemy_x =xs[get_rand8()&3];
	*t_penemy_y +=0x20;
	if (*t_penemy_y>0x71)
		*t_penemy_y =0x30;
	t_penemy->form_cnt=3;
	t_penemy->state++;
}

void e_s4boss_st3_sub(int drawidx, int bno);

void e_s4boss_st3(){
	static int bnos[]={0x0d, 0x0e, 0x07, 0x08};
	e_s4boss_st3_sub(t_penemy->form_cnt, bnos[t_penemy->form_cnt]);
}

void e_s4boss_st3_sub(int drawidx, int bno){
	static int deltas[]={-0x1c,-0x1c,0x04,-0x1c,-0x1c,0x04,0x04,0x04};
	int bx,by;
	int *pdeltas;

	pdeltas =&deltas[drawidx<<1];
	bx =*t_penemy_x +*pdeltas++;
	by =*t_penemy_y +*pdeltas;
	show_bgobj_4x4(bx,by, bno);
	if (--t_penemy->form_cnt<0){
		t_penemy->utemp =3;
		t_penemy->delay =0x60;
		t_penemy->state++;
	}
}

void e_s4boss_st4(){
	if (--t_penemy->delay){
		s4boss_lowhalf_form();
		return;
	}
	t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
	s4boss_uphalf_form();
	if (--t_penemy->utemp==0){
		t_penemy->delay =0x100;
		t_penemy->state++;
	}
}

void e_s4boss_st5(){
	static short gdata[]={
		0x15,0x14,0x0100,0x7c,
		0x16,0x00,0x0000,0x22,
		0x15,-0x14,-0x0200,0x7c,
		0x16,0x00,0x0000,0x22
	};
	short *pgdata;
	int idx, no;
	int etype, xdelta;
	struct ENEMY *penemy;

	if (--t_penemy->delay==0){
		t_penemy->utemp =2;
		t_penemy->delay =8;
		t_penemy->state++;
		return;
	}
	if (t_penemy->delay &0xf){
		s4boss_lowhalf_form();
		return;
	}
	idx =(t_penemy->delay>>4) &3;
	pgdata =&gdata[idx<<2];
	etype =*pgdata++;
	xdelta =*pgdata++;
	no=gen_new_enemy (xdelta,-0xc, etype);
	if (no>=0){
		penemy =&enemy[no];
		penemy->xspeed =*pgdata++;
		penemy->delay =0x10;
		penemy->col_type =2;
		if ((idx &1)==0)
			penemy->xspeed +=0x80;
		disp_forms[0xa+no] =(unsigned char)*pgdata;
		disp_attr[0xa+no] =0;
	}
}

void e_s4boss_st6(){
	if (--t_penemy->delay){
		s4boss_lowhalf_form();
		return;
	}
	s4boss_uphalf_form();
	if (++t_penemy->utemp==5){
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		t_penemy->delay =0x20;
		t_penemy->state++;
	}
}

void e_s4boss_st7(){
	if (--t_penemy->delay){
		s4boss_lowhalf_form();
		return;
	}
	bright_count =0x18;
	update_pal_buf(0x10);
	t_penemy->form_cnt =3;
	t_penemy->state++;
}

void e_s4boss_st8(){
	e_s4boss_st3_sub(t_penemy->form_cnt, 0x9b);
}

void e_s4boss_st9(){
	if (--t_penemy->delay==0)
		t_penemy->state =2;
}

void e_s4boss_sta(){
	stop_bgm();
	PLAYSOUND4(BOMBING4);
	t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	t_penemy->bombing_idx =5;
	enemy_allclear();
	t_penemy->delay =0x10;
	t_penemy->state++;
}

void e_s4boss_stb(){
	static int deltas[]={-0x20,0x00,0x00,0x20,0x20,0x00,-0x10,-0x10,0x00,0x00};
	int *pdeltas;

	if (--t_penemy->delay)
		return;
	if (t_penemy->bombing_idx==0){
		t_penemy->bombing_idx=3;
		t_penemy->delay =0x4;
		t_penemy->state++;
		return;
	}
	pdeltas =&deltas[(t_penemy->bombing_idx-1)<<1];
	*t_penemy_x +=*pdeltas++;
	*t_penemy_y +=*pdeltas;
	enemy_bgobj_4x4(0x9b);
	t_penemy->bombing_idx--;
	t_penemy->delay =8;
	bombing_gen(*t_penemy_x, *t_penemy_y);
}

void e_s4boss_stc(){
	static int xys[]={0xc0,0x80,0x96,0xc0,0xa0,0x97,0xd0,0xc0,0x98};
	int idx;
	int *pxys;

	if (--t_penemy->delay)
		return;
	if (t_penemy->bombing_idx==0){
		stage_clear=1;
		long_delay =0x30;
		return;
	}
	idx =t_penemy->bombing_idx-1;
	idx +=(idx<<1);
	pxys =&xys[idx];
	*t_penemy_x =*pxys++;
	*t_penemy_y =*pxys++;
	enemy_bgobj_4x4(*pxys);
	t_penemy->bombing_idx--;
	t_penemy->delay =8;
	bombing_gen(*t_penemy_x, *t_penemy_y);
}

void s4boss_lowhalf_form(){
	static int bnos[]={0x07,0x08,0x0C,0x06};
	int bnoidx;

	if ((framecount & 7)!=3)
		return;
	t_penemy->form_cnt=2;
	bnoidx =(framecount &8) ?0:2;
	e_s4boss_st3_sub(2, bnos[bnoidx]);
	e_s4boss_st3_sub(3, bnos[bnoidx+1]);
}

void s4boss_uphalf_form(){
	static int bnos[]={0x0b,0x05,0x0a,0x04,0x09,0x03,0x0d,0x0e};
	int idx =(t_penemy->utemp-1)<<1;

	t_penemy->delay=8;
	t_penemy->form_cnt=2;
	e_s4boss_st3_sub(0, bnos[idx]);
	e_s4boss_st3_sub(1, bnos[idx+1]);
}

void e_s_ship_st1(){
	if (--t_penemy->delay)
		s_ship_form();
	else{
		t_penemy->delay=0x100;
		t_penemy->state++;
	}
}

void e_s_ship_st2(){
	t_penemy->delay--;
	s_ship_form();
	refresh_enemyx();
	if (*t_penemy_x<0 || *t_penemy_x>=0x100)
		enemy_selfclear();
	if (*t_penemy_x<0x20 || *t_penemy_x>=0xe0){
		t_penemy->yspeed =0x0180;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_s_ship_st3(){
	t_penemy->delay--;
	s_ship_form();
	refresh_enemyy();
	if (*t_penemy_y>=0xa8){
		*t_penemy_y=0xa9;
		t_penemy->xspeed =(*t_penemy_x<0x80) ?0x0180 : -0x0180;
		t_penemy->yspeed =0;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_s_ship_st4(){
	t_penemy->delay--;
	s_ship_form();
	refresh_enemyxy();
}

void e_s_ship_st5(){
	bombing_init(0);
}

void e_s_ship_st6(){
	bombing_proc(0);
}

void e_s_ship_st7(){
	move_enemy_scroll();
	enemy_selfclear();
}

void s_ship_form(){
	if (!(t_penemy->delay&3))
		if (++(*t_penemy_dform)>=0x7f)
			(*t_penemy_dform) -=3;
}

void e_fireball_st1(){
	t_penemy->yspeed +=0x28;
	if (*t_penemy_y<0xb0)
		refresh_enemyxy();
	else
		t_penemy->state++;
}

void e_fireball_st2(){
	bombing_init(0);
}

void e_fireball_st3(){
	bombing_proc(0);
}

void e_fireball_st4(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_s5boss_st1(){
	t_penemy->hp =64 +(diff_extra<<3);
	t_penemy->motion_state =get_rand8() &3;
	*t_penemy_dform =0xb8;
	*t_penemy_y =0x9b;
	t_penemy->delay =0x31;
	t_penemy->state++;
}

void e_s5boss_st2(){
	refresh_enemyxy();
	if (--t_penemy->delay==0)
		if (t_penemy->state)
			t_penemy->state++;
}

void e_s5boss_st3(){
	static short jump_xspds[]={0x80, 0, 0, -0x80};
	static short walk_xspds[]={0x0118,-0x0118};

	refresh_enemyxy();
	switch (t_penemy->motion_state){
	case 0:
		*t_penemy_dattr =(*t_penemy_x<chr_xs[0]) ?0x40:0;
		t_penemy->yspeed = -0x0680;
		t_penemy->xspeed =jump_xspds[(get_rand8()+framecount)&3];
		t_penemy->bullets =0;
		*t_penemy_dform =0xba;
		if (t_penemy->state)
			t_penemy->state=5;
		return;
	case 1:
		*t_penemy_dattr =(*t_penemy_x<chr_xs[0]) ?0x40:0;
		if (++t_penemy->bullets>=4)
			robot_motion_reset();
		else{
			t_penemy->delay =0x20;
			if (t_penemy->state)
				t_penemy->state++;
		}
		return;
	case 2:
	case 3:
		*t_penemy_dattr =(*t_penemy_x<chr_xs[0]) ?0x40:0;
		t_penemy->xspeed =walk_xspds[get_rand8() &1];
		t_penemy->form_cnt =0xc;
		if (t_penemy->state)
			t_penemy->state=6;
		return;
	}
}

void e_s5boss_st4(){
	int no;

	refresh_enemyxy();
	if (!player_normal){
		robot_motion_reset();
		return;
	}
	if (--t_penemy->delay){
		if (t_penemy->delay<0xf)
			*t_penemy_dform =0xc3;
		robot_color();
	}else{
		no =gen_new_enemy(-0x10, -0x18, 0x14);
		if (no>=0){
			if (*t_penemy_dattr & 0x40)
				chr_xs[0xa +no] +=0x30;
			enemy[no].utemp =t_enemy_no;
		}
		robot_motion_reset();
	}
}

void e_s5boss_st5(){
	robot_color();
	t_penemy->yspeed +=0x38;
	if (*t_penemy_x<0x21){
		*t_penemy_x =0x20;
		t_penemy->xspeed =0;
	}
	if (*t_penemy_x>=0xc0){
		*t_penemy_x =0xc0;
		t_penemy->xspeed =0;
	}
	refresh_enemyxy();
	if (*t_penemy_y>=0x9b){
		PLAYSOUND4(ROBOT_LANDING);
		t_penemy->xspeed =0;
		t_penemy->yspeed =0;
		*t_penemy_y=0x9b;
		*t_penemy_dform =0xb8;
		robot_motion_reset();
	}
}

void e_s5boss_st6(){
	robot_color();
	refresh_enemyxy();
	if (*t_penemy_x<0x20 || *t_penemy_x>=0xc0){
		robot_motion_reset();
		return;
	}
	if (--t_penemy->form_cnt)
		return;
	t_penemy->form_cnt =0xc;
	t_penemy->bullets =0;
	*t_penemy_dform =(++t_penemy->form_idx &1) +0xb8;
}

void e_s5boss_st7(){
	stop_bgm();
	stageclear_init(BOMBING4);
	*t_penemy_dform=0;
	bombing_gen(*t_penemy_x, *t_penemy_y);
	t_penemy->bombing_idx =0;
	t_penemy->state++;
}

void e_s5boss_st8(){
	static int yxdeltas[]={-0x10,-0x10,0x10,0x10,-0x10,0x10,0x10,-0x10};
	int *pyxdeltas;

	t_penemy->delay =8;
	if (t_penemy->bombing_idx==4){
		t_penemy->delay =0x30;
		t_penemy->state++;
	}else{
		pyxdeltas =&yxdeltas[t_penemy->bombing_idx<<1];
		t_penemy->bombing_idx++;
		bombing_gen(*t_penemy_x+pyxdeltas[1], *t_penemy_y+pyxdeltas[0]);
	}
}

void e_s5boss_st9(){
	t_penemy->bombing_idx =0;
	t_penemy->utemp =8;
	t_penemy->delay =0xa;
	t_penemy->state++;
}

void e_s5boss_sta(){
	static int tbl0[]={0x90,0x8b,0x8a,0xff};
	static int tbl2[]={0x58,0x8d,0xff};
	static int *tbls[]={tbl0, tbl0, tbl2};
	int *ptbl, basey, bno;

	if (--t_penemy->delay)
		return;
	t_penemy->delay =0xa;
	if (t_penemy->bombing_idx!=1){
		ptbl =tbls[t_penemy->bombing_idx];
		basey =*ptbl++ +0x10;
		while ((bno=*ptbl++)!=0xff){
			show_bgobj_2xn(0xd0, basey, bno);
			basey +=0x10;
		}
		t_penemy->basey =basey-0x20;
		if (++t_penemy->bombing_idx==3){
			long_delay=1;
			enemy_selfclear();
		}
	}else{
		show_bgobj_2xn(0xd0, t_penemy->basey, 0x8c);
		t_penemy->basey -=8;
		if (--t_penemy->utemp==0)
			t_penemy->bombing_idx++;
	}
}

void robot_motion_reset(){
	*t_penemy_dform =0xb8;
	t_penemy->xspeed =0;
	t_penemy->yspeed =0;
	t_penemy->motion_state =get_rand8() &3;
	t_penemy->delay |=(get_rand8()+framecount) &0x80;
	if (t_penemy->state)
		t_penemy->state=3;
}

void robot_color(){
	if (!(framecount &7))
		return;
	if (t_penemy->hp>=0x20)
		return;
	if (t_penemy->hp>=0x10)
		palcgs_spec[7]=0x51;
	else
		palcgs_spec[7]=0x52;
	update_pal_buf(0x20);
}

void e_discus_st1(){
	t_penemy->delay =6;
	*t_penemy_dform =0xbb;
	t_penemy->xspeed =-0x0300;
	if (disp_attr[0xa +t_penemy->utemp] & 0x40)
		t_penemy->xspeed =0x0300;
	t_penemy->yspeed =0x0200;
	t_penemy->state++;
}

void e_discus_st2(){
	if (*t_penemy_x>=0xe0){
		t_penemy->state++;
		return;
	}
	if (--t_penemy->delay){
		if (*t_penemy_y>=0xaf)
			t_penemy->yspeed =0;
	}else{
		t_penemy->delay =6;
		*t_penemy_dform =0xbb + (++t_penemy->form_idx &1);
	}
	refresh_enemyxy();
}

void e_discus_st3(){
	enemy_selfclear();
}

void e_flameud_st1(){
	static short delays[]={00,0x20,0x40,0x60};
	t_penemy->form_idx=4;
	t_penemy->flag2 |=0x80;
	*t_penemy_y+=8;
	t_penemy->break_cnt =delays[(t_penemy->flag2 &ENEMY_BREAK_TYPE)>>2];
	t_penemy->delay =t_penemy->break_cnt;
	t_penemy->state++;
}

void e_flame_st2_sub();

void e_flameud_st2(){
	flameend_form();
	move_enemy_scroll();
	if (t_penemy->delay){
		t_penemy->delay--;
		return;
	}
	if (get_p_e_xdist()>=0x20)
		return;
	e_flame_st2_sub();
}

void e_flame_st2_sub(){
	static unsigned char lengths[]={0x05,0x09,0x0d,0x0f};

	if (*t_penemy_x<0x30)
		return;
	PLAYSOUND4(FLAME);
	t_penemy->flag1 &=~ENEMY_NONCOL;
	t_penemy->total_len =lengths[t_penemy->flag2 &ENEMY_LENGTH_TYPE];
	*t_penemy_dform=1;
	t_penemy->dyn_len=0;
	t_penemy->dyn_hlen=0;
	t_penemy->dyn_vlen=0;
	t_penemy->delay =0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_flameud_st3(){
	move_enemy_scroll();
	if (!draw_flame(0))
		return;
	if (--t_penemy->total_len)
		t_penemy->dyn_vlen +=8;
	else{
		t_penemy->delay =0x10;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_flameud_st4(){
	move_enemy_scroll();
	if (!draw_flame(2))
		return;
	t_penemy->dyn_vlen -=8;
	if (t_penemy->dyn_vlen<0){
		t_penemy->delay =t_penemy->break_cnt;
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_flamerl_st1(){
	static short delays[]={00,0x20,0x40,0x60};
	t_penemy->flag2 |=0x40;
	*t_penemy_y+=8;
	t_penemy->break_cnt =delays[(t_penemy->flag2 &ENEMY_BREAK_TYPE)>>2];
	t_penemy->delay =t_penemy->break_cnt;
	t_penemy->state++;
}

void e_flamerl_st2(){
	flameend_form();
	move_enemy_scroll();
	if ((framecount &0x7f)==t_penemy->break_cnt){
		e_flame_st2_sub();
	}
}

void e_flamerl_st3(){
	move_enemy_scroll();
	if (!draw_flame(4))
		return;
	if (--t_penemy->total_len)
		t_penemy->dyn_hlen -=8;
	else{
		t_penemy->delay =0x10;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_flamerl_st4(){
	move_enemy_scroll();
	if (!draw_flame(6))
		return;
	t_penemy->dyn_hlen +=8;
	if (t_penemy->dyn_hlen>0){
		t_penemy->delay =t_penemy->break_cnt;
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void e_flamelr_st1(){
	static short delays[]={00,0x20,0x40,0x60};
	*t_penemy_dattr=0x40;
	*t_penemy_y+=8;
	t_penemy->break_cnt =delays[(t_penemy->flag2 &ENEMY_BREAK_TYPE)>>2];
	t_penemy->delay =t_penemy->break_cnt;
	t_penemy->state++;
}

void e_flamelr_st2(){
	flameend_form();
	move_enemy_scroll();
	if (t_penemy->delay){
		t_penemy->delay--;
		return;
	}
	t_penemy->break_cnt =get_rand8() & 0x3f;
	e_flame_st2_sub();
}

void e_flamelr_st3(){
	move_enemy_scroll();
	if (!draw_flame(8))
		return;
	if (--t_penemy->total_len)
		t_penemy->dyn_hlen +=8;
	else{
		t_penemy->delay =0x10;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_flamelr_st4(){
	move_enemy_scroll();
	if (!draw_flame(0xa))
		return;
	t_penemy->dyn_hlen -=8;
	if (t_penemy->dyn_hlen<0){
		t_penemy->delay =t_penemy->break_cnt;
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

void flameend_form(){
	static unsigned char forms[]={0x01,0xbf,0xc0,0xbf,0x01,0xc1,0xc2,0xc1};

	if ((--t_penemy->form_cnt &7)==0){
		*t_penemy_dform =forms[t_penemy->form_idx |((t_penemy->form_cnt>>3)&3)];
	}
}

int draw_flame(int idx){
	static unsigned char bnos[]={
		0x87,0x88,0x80,0x89,0x84,0x85,0x80,0x86,0x81,0x82,0x80,0x83
	};
	int bno;
	int x,y;

	if (t_penemy->delay){
		t_penemy->delay--;
		return 0;
	}
	t_penemy->dyn_len =t_penemy->dyn_hlen |t_penemy->dyn_vlen;
	bno=bnos[(t_penemy->dyn_len) ?idx+1 :idx];
	y =*t_penemy_y+t_penemy->dyn_vlen-1;
	x=*t_penemy_x+t_penemy->dyn_hlen-7;
	if (x<0 || x>0x100)
		return 1;
	show_bgobj_2xn(x,y,bno);
	t_penemy->delay =(*t_penemy_x<0x30) ?0 :1;
	return 1;
}

void e_m_arm_st1(){
	static unsigned char delays[]={0x00,0x20,0x40,0x60};

	move_enemy_scroll();
	t_penemy->form_idx =delays[(t_penemy->flag2 & ENEMY_BREAK_TYPE)>>2];
	t_penemy->flag2 &=ENEMY_LENGTH_TYPE;
	t_penemy->delay=0x20;
	t_penemy->state++;
}

void e_m_arm_st2(){
	static unsigned char lengths[]={0x04,0x03,0x08,0x03};

	move_enemy_scroll();
	if (t_penemy->flag2 !=3){
		if ((framecount & 0x7f) !=t_penemy->form_idx)
			return;
	}else{
		if (t_penemy->delay){
			t_penemy->delay--;
			return;
		}
		if (framecount>0xc0)
			return;
		if (get_p_e_xdist()>=0x10)
			return;
	}
	if (*t_penemy_x<0x2c)
		return;
	t_penemy->utemp =t_penemy->flag2<<1;
	t_penemy->total_len =lengths[t_penemy->flag2];
	t_penemy->hand_step=0;
	t_penemy->delay=0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_m_arm_st3(){
	move_enemy_scroll();
	if (!draw_m_arm())
		return;
	if (--t_penemy->total_len){
		*t_penemy_y+=8;
		t_penemy->hand_step++;
	}else{
		t_penemy->utemp++;
		t_penemy->delay=8;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_m_arm_st4(){
	move_enemy_scroll();
	if (!draw_m_arm())
		return;
	if (--t_penemy->hand_step>=0){
		*t_penemy_y-=8;
	}else{
		t_penemy->delay=t_penemy->form_idx;
		if (t_penemy->state)
			t_penemy->state=2;
	}
}

int draw_m_arm(){
	static unsigned char bnos0[]={0x86,0x86,0x86,0x86};
	static unsigned char bnos1[]={0x80,0x80,0x82,0x84};
	static unsigned char bnos2[]={0x86,0x86,0x86,0x86,0x86,0x86,0x86,0x86};
	static unsigned char bnos3[]={0x80,0x80,0x80,0x80,0x80,0x80,0x82,0x84};
	static unsigned char *bnos[]={
		bnos0, bnos1,bnos0, bnos1,bnos2, bnos3,bnos0, bnos1
	};
	unsigned char *pbnos;

	if (t_penemy->delay){
		t_penemy->delay--;
		return 0;
	}
	pbnos=bnos[t_penemy->utemp];
	show_bgobj_2xn(*t_penemy_x, *t_penemy_y, pbnos[t_penemy->hand_step]);
	t_penemy->delay=(*t_penemy_x<0x2c) ?0 :2;
	return 1;
}

void e_s_wall_st1(){
	static unsigned char dist_delays[]={0x30,0x00,0x50,0x0f,0x70,0x1e,0x40,0x00};
	static unsigned char delays_btwn[]={0x0c,0x08,0x04,0x02};
	unsigned char *ptbl;

	ptbl =&dist_delays[(t_penemy->flag2 & ENEMY_BREAK_TYPE)>>1];
	t_penemy->free_dist =*ptbl++;
	t_penemy->fire_cnt =*ptbl;
	t_penemy->delay_btwn_fire = delays_btwn[t_penemy->flag2 & ENEMY_DELAY_TYPE];
	move_enemy_scroll();
	t_penemy->flag2 |=0xc0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_s_wall_st2(){
	move_enemy_scroll();
	if (get_p_e_xdist()<t_penemy->free_dist){
		t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
		t_penemy->wall_step=6;
		t_penemy->delay=t_penemy->fire_cnt;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_s_wall_st3(){
	static unsigned char params[]={
		0xc0,0x91,0xd0,0xd0,0x91,0xe0,0xe0,0x90,0xf0,0xe0,0x8f,0xf8,0xf0,0x8e,0x00,0xf0,
		0x8d,0x09,0xf0,0x8c,0x09
	};
	unsigned char *pparam, c;
	int idx,x,y,tmp;

	move_enemy_scroll();
	if (t_penemy->delay){
		t_penemy->delay--;
		return;
	}
	idx=(t_penemy->wall_step<<1)+t_penemy->wall_step;
	pparam =&params[idx];
	c=pparam[0];
	tmp=EXT(c);
	y=*t_penemy_y +tmp;
	x=*t_penemy_x -0xd;
	t_penemy->dyn_len =pparam[2];
	show_bgobj_4x4(x,y,pparam[1]);
	if (t_penemy->wall_step<4){
		set_bgval_4x4(0x00, 0x0f);
	}
	t_penemy->delay=t_penemy->delay_btwn_fire;
	if (--t_penemy->wall_step<0){
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_s_wall_st4(){
	move_enemy_scroll();
}

void e_s_wall_st5(){
	t_penemy->bombing_idx=0;
	t_penemy->delay=3;
	move_enemy_scroll();
	PLAYSOUND3(BOMBING1);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_s_wall_st6(){
	static unsigned char params[]={
		0x00,0x84,0x08,0xe0,0x8b,0xf0,0xc0,0x8a,0xd0,0xa0,0x86,0xb0,0x00,0x84,0x08,0xe0,
		0x8b,0xf0,0xc0,0x86,0xd0
	};
	unsigned char *pparam, c;
	int idx,x,y,tmp;
	int nx,ny;

	move_enemy_scroll();
	idx=(t_penemy->bombing_idx<<1)+t_penemy->bombing_idx;
	pparam =&params[idx];
	c=pparam[0];
	tmp=EXT(c);
	y=*t_penemy_y +tmp;
	x=*t_penemy_x -0xd;
	show_bgobj_4x4(x,y,pparam[1]);
	if (t_penemy->bombing_idx &3)
		set_bgval_4x4(0,0);
	c=pparam[2];
	tmp=EXT(c);
	new_e_pos(-4, tmp, &nx, &ny);
	bombing_gen(nx,ny);
	t_penemy->bombing_idx++;
	if (--t_penemy->delay==0)
		enemy_selfclear();
}

void e_s_twall_st1(){
	static unsigned char idxs[]={0x04,0x03,0x00,0x04};

	t_penemy->dyn_len=(char)0xb8;
	t_penemy->bombing_idx =idxs[t_penemy->flag2];
	t_penemy->delay =idxs[t_penemy->flag2+1];
	move_enemy_scroll();
	t_penemy->flag2 |=0xc0;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_s_twall_st2(){
	move_enemy_scroll();
}

void e_s_twall_st3(){
	move_enemy_scroll();
	PLAYSOUND3(BOMBING1);
	if (t_penemy->state)
		t_penemy->state++;

}

void e_s_twall_st4(){
	e_s_wall_st6();
}

void e_m_cargen_st1(){
	t_penemy->form_idx =0x80;
	t_penemy->delay =1;
	t_penemy->state++;
}

void e_m_cargen_st2(){
	int no, idx;
	struct ENEMY *penemy;

	move_enemy_scroll();

	if (t_penemy->form_idx &0x80){
		if (--t_penemy->delay==0){
			t_penemy->delay++;
			if ((no=find_free_emy())>=0){
				enemy_common_init(no,0x14);
				penemy =&enemy[no];
				idx =0xa +no;
				penemy->utemp =1;
				penemy->flag2 = ENEMY_IS_MOVING_CAR;
				chr_xs[idx]=0xf8;
				chr_ys[idx]=0xc8;
				disp_forms[idx] =0x2a;
				penemy->xspeed =-0x0080;
				t_penemy->form_idx =no;
			}
		}
	}else{
		no =t_penemy->form_idx;
		if (!enemy[no].state){
			t_penemy->form_idx =0x80;
			t_penemy->delay =0x80;
		}
	}
}

void e_m_car_st1(){
	static int xdeltas[]={0xf, -0xf};
	int xtest;

	*t_penemy_dform =0x2a +((framecount>>2) &1);
	refresh_enemyxy();
	xtest =*t_penemy_x+xdeltas[t_penemy->utemp];
	if (xtest>=0)
		check_bg(xtest, *t_penemy_y, 0);
	else
		bg_empty_f=1;
	if (bg_empty_f){
		check_e_bg(0, 9);
		if (bg_empty_f)
			t_penemy->yspeed +=0x20;
	}else{
		if (t_penemy->flag2 & ENEMY_IS_MOVING_CAR){
			if (t_penemy->state)
				t_penemy->state=4;
		}else{
			t_penemy->utemp ^=1;
			t_penemy->xspeed =-t_penemy->xspeed;
		}
	}
}

void e_m_car_st4(){
	bombing_init(0);
}

void e_m_car_st5(){
	bombing_proc(0);
}

void e_m_car_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_car_st1(){
	*t_penemy_y=0xc8;
	*t_penemy_dform =0x2a;
	t_penemy->xspeed =0x00c0;
	t_penemy->state++;
}

void e_car_st2(){
	if (t_penemy->form_idx){
		if (t_penemy->state)
			t_penemy->state++;
	}
	else
		move_enemy_scroll();
}

void e_car_st3(){
	e_m_car_st1();
}

void e_car_st4(){
	bombing_init(0);
}

void e_car_st5(){
	bombing_proc(0);
}

void e_car_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_s6boss_st1(){
	t_penemy->state++;
}

void e_s6boss_st2(){
	move_enemy_scroll();
	if (t_penemy->hp<5)
		arms_clred =2;
}

void e_s6boss_st3(){
	*t_penemy_x+=8;
	enemy_bgobj_4x4(5);
	t_penemy->state++;
}

void e_s6boss_st4(){
	stop_bgm();
	stageclear_init(BOMBING3);
	enemy_allclear();
	bombing_init(0);
}

void e_s6boss_st5(){
	bombing_proc(0);
}

void e_s6boss_st6(){
	*t_penemy_dform=0;
	if (t_penemy->state)
		t_penemy->state++;
	*t_penemy_y+=0x20;
}

void e_s6boss_st7(){
	static unsigned char bnos[]={8,4};

	enemy_bgobj_4x4(bnos[t_penemy->bombing_idx]);
	set_bgval_4x4(5, 5);
	bombing_gen(*t_penemy_x, *t_penemy_y);
	*t_penemy_y+=0x20;
	if (++t_penemy->bombing_idx>=2){
		long_delay=0x80;
		enemy_selfclear();
	}
}

void e_3wcannon_st1(){
	*t_penemy_y+=4;
	t_penemy->spd_idx =4;
	t_penemy->delay =(t_penemy->flag2 &1) ?0x60 :0x10;
	t_penemy->state++;
}

void e_3wcannon_st2(){
	move_enemy_scroll();
	if (!boss_ascroll_done || --t_penemy->delay)
		return;
	e_3wcannon_form();
	if (t_penemy->form_idx<2)
		t_penemy->form_idx++;
	else{
		t_penemy->flag1 &=~(ENEMY_NONSHOOTABLE | ENEMY_NONCOL);
		t_penemy->delay_btwn_fire =0x10;
		t_penemy->delay =0x60;
		t_penemy->state++;
	}
}

void e_3wcannon_st3(){
	int no;

	move_enemy_scroll();
	if (--t_penemy->delay){
		if (--t_penemy->delay_btwn_fire==0){
			if ((no=gen_new_enemy (0,0, 0xb))>=0){
				enemy[no].spd_idx =t_penemy->spd_idx;
				if (--t_penemy->spd_idx==0)
					t_penemy->spd_idx=4;
			}
		}
	}else{
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
		t_penemy->delay =1;
		t_penemy->state++;
	}
}

void e_3wcannon_st4(){
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		e_3wcannon_form();
		if (t_penemy->form_idx)
			t_penemy->form_idx--;
		else{
			t_penemy->delay =0xa0;
			t_penemy->state =2;
		}
	}
}

void e_3wcannon_st5(){
	enemy_bgobj_2xn(0xb);
	arms_clred++;
	t_penemy->state++;
}

void e_3wcannon_st6(){
	bombing_init(0);
}

void e_3wcannon_st7(){
	bombing_proc(0);
}

void e_3wcannon_st8(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_3wcannon_form(){
	enemy_bgobj_2xn(t_penemy->form_idx+8);
	t_penemy->delay=8;
}

void e_soldiergen_st1(){
	t_penemy->delay =0xa0;
	enemy_rounds =0;
	t_penemy->state ++;
}

void e_soldiergen_st2(){
	static unsigned char nums[]={3, 4, 2, 4};

	if (enemy_rounds>=30 || arms_clred>=2)
		t_penemy->delay =0xf0;
	if (door_form()){
		if (t_penemy->form_idx<2)
			t_penemy->form_idx++;
		else{
			enemy_rounds++;
			t_penemy->left =(chr_xs[0]>=0xa0) ?1:0;
			t_penemy->utemp =nums[get_rand8() &3];
			t_penemy->delay_btwn_fire =0x10; //实为敌兵产生间延时
			t_penemy->delay =0x80;
			t_penemy->state++;
		}
	}
}

void e_soldiergen_st3(){
	int no, left;

	move_enemy_scroll();
	if (--t_penemy->delay==0){
		t_penemy->delay =1;
		t_penemy->state++;
		return;
	}
	if (--t_penemy->delay_btwn_fire)
		return;
	if ((no=gen_new_enemy (-8,0, 5))<0)
		return;
	left =t_penemy->left;
	if (!left){
		if (enemy_rounds>=20 || arms_clred)
			left =(get_rand8()>>1) &1;
	}
	enemy[no].flag2 =left;
	t_penemy->delay_btwn_fire =(--t_penemy->utemp==0) ?0xff:0x10;
}

void e_soldiergen_st4(){
	static short delays[]={0xf0,0x80,0xa0,0xc0};

	if (door_form()){
		if (t_penemy->form_idx)
			t_penemy->form_idx--;
		else{
			t_penemy->delay =delays[(get_rand8()>>3) &3];
			t_penemy->state =2;
		}
	}
}

void e_soldiergen_st5(){
	enemy_selfclear();
}

int door_form(){
	static unsigned char bnos[]={0x03,0x12,0x13,0x14,0x07,0x15};
	unsigned char *pbnos;

	move_enemy_scroll();
	if (--t_penemy->delay)
		return 0;
	pbnos =&bnos[t_penemy->form_idx<<1];
	enemy_bgobj_4x4xv2(*pbnos, pbnos[1], 0);
	t_penemy->delay=8;
	return 1;
}

void e_demon_st1(){
	int hp =0x37+((diff_extra+num_of_allclears)<<4);

	t_penemy->hp =(hp>0xa0) ?0xa0:hp;
	t_penemy->form_cnt =0x20;
	t_penemy->mouth_bno =0x90;
	demon_ascroll =0x40;
	t_penemy->delay =3;
	t_penemy->state++;
}

void e_demon_st2(){
	if (*t_penemy_x<0x50){
		move_enemy_scroll();
		return;
	}
	if (--t_penemy->form_cnt==0){
		t_penemy->form_cnt =0x20;
		if (t_penemy->mouth_bno==0x90){
			t_penemy->mouth_bno=0x92;
			t_penemy->delay--;
		}else
			t_penemy->mouth_bno=0x90;
		show_mouth_upperblks();
		show_mouth_lowerblk();
	}
	move_enemy_scroll();
	if (t_penemy->delay==0){
		t_penemy->delay =0x20;
		if (t_penemy->state)
			t_penemy->state++;
	}
}

void e_demon_st3(){
	int no_gen=0;

	move_enemy_scroll();
	if (!player_normal){
		t_penemy->delay =1;
		no_gen++;
	}
	if (--t_penemy->delay<2 ||(t_penemy->delay==2 && diff_extra==3)){
		if (!no_gen)
			gen_new_enemy (0,0, 0x11);
		if (!t_penemy->delay){
			t_penemy->delay =3;
			if (t_penemy->state)
				t_penemy->state=2;
		}
	}
}

void e_demon_st4(){
	PLAYSOUND4(BOMBING4);
	move_enemy_scroll();
	bombing_gen(*t_penemy_x, *t_penemy_y);
	t_penemy->delay =5;
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_st5(){
	static int yxdeltas[]={
		0x10,0x10,-0x10,0x10,-0x10,-0x10,0x10,-0x10,0x20,0x20,-0x20,0x20,-0x20,-0x20,0x20,-0x20,
		0x40,0x40,-0x40,0x40,-0x40,-0x40,0x50,0x00	
	};
	int *pyxdeltas;
	int bx, by;

	move_enemy_scroll();
	if (--t_penemy->delay==0){
		t_penemy->delay =5;
		if (++t_penemy->bombing_idx==0xc){
			if (t_penemy->state)
				t_penemy->state++;
		}else{
			pyxdeltas =&yxdeltas[t_penemy->bombing_idx<<1];
			by =*t_penemy_y + *pyxdeltas++;
			bx =*t_penemy_x + *pyxdeltas;
			bombing_gen(bx, by);
		}
	}
}

void e_demon_st6(){
	move_enemy_scroll();
	show_blks_by_no(0);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_st7(){
	int bx =*t_penemy_x+0x10, by =*t_penemy_y+0x20;
	
	move_enemy_scroll();
	show_bgobj_4x4(bx-0xe, by-0x10, 0x83);
	*t_penemy_y -=0x20;
	e_demon_st6();
	*t_penemy_y +=0x20;
}

void e_demon_st8(){
	move_enemy_scroll();
	show_blks_by_no(1);
	show_blks_by_no(2);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_st9(){
	int bx, by;
	move_enemy_scroll();
	bx =*t_penemy_x+0x30;
	by =*t_penemy_y;
	show_bgobj_4x4(bx-0xe, by-0x10, 0x83);
	bx =*t_penemy_x-0x10;
	by =*t_penemy_y-0x40;
	show_bgobj_4x4(bx-0xe, by-0x10, 0x83);
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_sta(){
	move_enemy_scroll();
	show_blks_by_no(9);
	clr_bgval_blks2();
	show_blks_by_no(0xa);
	clr_bgval_blks2();
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_stb(){
	move_enemy_scroll();
	show_blks_by_no(0xb);
	bgobj_addr -=0x40;
	clr_bgval_blks2();
	if (t_penemy->state)
		t_penemy->state++;
}

void e_demon_stc(){
	s7emy_allclear();
}

void e_demon_std(){
	enemy_selfclear();
}

void show_blks2(int x, int y, int blk1, int blk2){
	show_bgobj_4x4(x-0xe, y-0x10, blk1);
	x-=0x20;
	show_bgobj_4x4(x-0xe, y-0x10, blk2);
}

void show_mouth_upperblks(){
	int bno1 =t_penemy->mouth_bno, bno2 =bno1+1;

	show_blks2(*t_penemy_x+0x10, *t_penemy_y, bno1, bno2);
}

void show_mouth_lowerblk(){
	int bno=(t_penemy->mouth_bno==0x92) ?0x94:0x83;
	int bx =*t_penemy_x+0x11, by=*t_penemy_y+0x20;

	show_bgobj_4x4(bx-0xe, by-0x10, bno);
}

void show_blks_by_no(int no){
	static int tbl[]={
		0x83,0x83,0x00,0x10,
		0x95,0x96,-0x40,0x30,
		0x97,0x83,-0x20,0x50,
		0x84,0x85,-0x10,0x10,
		0x86,0x87,0x10,0x10,
		0x88,0x89,-0x10,0x10,
		0x8a,0x8b,0x10,0x10,
		0x8c,0x8d,-0x10,0x10,
		0x8e,0x8f,0x10,0x10,
		0x83,0x83,0x20,-0x10,
		0x83,0x83,0x40,-0x10,
		0x29,0x29,0x60,-0x10,
	};

	int *ptbl=&tbl[no<<2];
	int x=*t_penemy_x +ptbl[3];
	int y=*t_penemy_y +ptbl[2];
	show_blks2(x, y, ptbl[0], ptbl[1]);
}

void s7emy_allclear(){
	int i;
	struct ENEMY *penemy;
	unsigned char c;

	for (i=0; i<0x10; i++){
		penemy=&enemy[i];
		if (!penemy->state)
			continue;
		c=penemy->type;
		switch (c){
		case 0x11:
		case 0x12:
		case 0x15:
			penemy->state =3;
			break;
		case 0x13:
			penemy->state =4;
			break;
		case 0x14:
			penemy->state =6;
			break;
		}
	}
	long_delay=0xa0;
	enemy_selfclear();
}

void e_alien_st1(){
	int dir;

	get_alien_delay_btwn_homing();
	t_penemy->delay_btwn_fire <<=1;
	t_penemy->hp =2+num_of_allclears;
	*t_penemy_dform =0xac;
	t_penemy->delay =6;
	t_penemy->form_idx =0;
	t_penemy->utemp =0;		//剩余跟踪次数
	dir =get_rand8()&3;
	if (!dir) dir=3;
	if (t_penemy->flag2)	//规定朝左的异形
		dir=3;
	t_penemy->aimed_dir =dir<<1;
	get_spds_of_dir(t_penemy->aimed_dir<<1);
	t_penemy->utemp--;
	t_penemy->state++;
}

void e_alien_st2(){
	int tdir, quadrant;
	int dummy;

	if (--t_penemy->delay==0){
		t_penemy->delay =6;
		quadrant =-1;
		tdir =t_penemy->aimed_dir+1;
		if (tdir>=0xc)
			tdir =0;
		do{
			tdir -=3;
			quadrant++;
		}while (tdir>=0);
		quadrant <<=1;
		*t_penemy_dattr &=0x3f;
		if (quadrant>=4){
			*t_penemy_dattr |=0xc0;
			quadrant -=4;
		}
		t_penemy->form_idx ^=1;
		*t_penemy_dform =0xac +quadrant+t_penemy->form_idx;
	}
	if (--t_penemy->delay_btwn_fire==0){
		if (t_penemy->utemp & 0x1f){
			get_alien_delay_btwn_homing();
			get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 0, &dummy);
			get_spds_of_dir(t_penemy->aimed_dir<<1);
			t_penemy->utemp--;
		}
	}
	refresh_enemyxy();
}

void e_alien_st3(){
	bombing_init(0);
}

void e_alien_st4(){
	bombing_proc(0);
}

void e_alien_st5(){
	move_enemy_scroll();
	enemy_selfclear();
}

void get_alien_delay_btwn_homing(){
	static unsigned char delays[]={
		0x16,0x0f,0x08,0x13,0x3a,0x06,0x21,0x3a,0x1d,0x14,0x12,0x28,0x48,0xff	
	};
	unsigned char c;

	c=delays[homing_delaytbl_idx++];
	if (c==0xff){
		homing_delaytbl_idx=0;
		c=delays[homing_delaytbl_idx++];
	}
	t_penemy->delay_btwn_fire =c;	//借用变量
}

void get_spds_of_dir(int dir){
	static short yxspds[]={
		0x0000, 0x0042, 0x007f, 0x00b2, 0x00dd, 0x00f7, 0x00ff, 0x00f7,
		0x00dd, 0x00b2, 0x007f, 0x0042, 0x0000, -0x0042,-0x007f,-0x00b2,
		-0x00dd,-0x00f7,-0x00ff,-0x00f7,-0x00dd,-0x00b2,-0x007f,-0x0042,
		0x0000, 0x0042, 0x007f, 0x00b2, 0x00dd, 0x00f7, 
	};

	t_penemy->yspeed =yxspds[dir];
	t_penemy->xspeed =yxspds[dir +6];
}

void e_flowergen_st1(){
	t_penemy->hp =3+(num_of_allclears<<1)+diff_extra+(get_rand8() &1);
	t_penemy->form_cnt =0x20;
	enemy_bgobj_4x4(1);
	t_penemy->delay =0xa;
	t_penemy->state++;
}

void e_flowergen_st2(){
	move_enemy_scroll();
	if (*t_penemy_x<0x20)
		return;
	if (player_normal && --t_penemy->delay==0){
		gen_new_enemy (0,0, 0x13);
		t_penemy->delay =0xc0+(get_rand8() &0x1f);
	}
	if (--t_penemy->form_cnt==0){
		enemy_bgobj_4x4(t_penemy->form_idx & 1);
		t_penemy->form_cnt =0x20;
		t_penemy->form_idx++;
	}
}

void e_flowergen_st3(){
	enemy_bgobj_4x4(2);
	t_penemy->state++;
}

void e_flowergen_st4(){
	bombing_init(0);
}

void e_flowergen_st5(){
	bombing_proc(0);
}

void e_flowergen_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void e_flower_st1(){
	int newdir;

	t_penemy->delay =0x50+(get_rand8() &0x1f);
	t_penemy->break_cnt =0;
	t_penemy->form_cnt =0xc;
	t_penemy->form_idx =0;
	*t_penemy_dform =0xb0;
	get_dir_aimed_adjust(*t_penemy_x, *t_penemy_y, 1, &newdir);
	t_penemy->aimed_dir =newdir;
	get_spds_of_dir(t_penemy->aimed_dir);
	t_penemy->state++;
}

void e_flower_st2(){
	static char spddeltas[]={
		0x00,-0x03,-0x06,-0x08,-0x0a,-0x0b,
		-0x0c,-0x0b,-0x0a,-0x08,-0x06,-0x03,0x00,0x03,0x06,0x08,0x0a,0x0b,0x0c,0x0b,0x0a,0x08,
		0x06,0x03,0x00,-0x03,-0x06,-0x08,-0x0a,-0x0b,
	};
	int dummy;

	flower_scorpion_form(0xb0);
	refresh_enemyxy();
	if (t_penemy->break_cnt==0){
		if (t_penemy->form_cnt==8){
			t_penemy->yspeed +=spddeltas[t_penemy->aimed_dir];
			t_penemy->xspeed +=spddeltas[t_penemy->aimed_dir+6];
		}
		if (--t_penemy->delay==0){
			t_penemy->break_cnt =2+(get_rand8() &0x20);
		}
	}else{
		t_penemy->xspeed =0;
		t_penemy->yspeed =0;
		if (--t_penemy->break_cnt==0){
			t_penemy->total_len =8;
			t_penemy->fire_cnt =8;	//借用, 实为瞄准间延时
			get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy);
			get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy);
			get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy);
			get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy);
			if (t_penemy->state)
				t_penemy->state++;
		}
	}
}

void e_flower_st3(){
	static short yxspds[]={
		0x0042, 0x007f, 0x00b2, 0x00dd, 0x00f7, 0x00ff, 0x00f7,
		0x00dd, 0x00b2, 0x007f, 0x0042, 0x0000, -0x0042,-0x007f,-0x00b2,
		-0x00dd,-0x00f7,-0x00ff,-0x00f7,-0x00dd,-0x00b2,-0x007f,-0x0042,
		0x0000, 0x0042, 0x007f, 0x00b2, 0x00dd, 0x00f7, 
	};
	int dummy;
	short spd;

	flower_scorpion_form(0xb0);
	if (t_penemy->total_len && --t_penemy->fire_cnt==0){
		t_penemy->total_len +=2;
		t_penemy->fire_cnt =t_penemy->total_len;
		get_dir_aimed_rotate(*t_penemy_x, *t_penemy_y, 1, &dummy);
		spd =yxspds[t_penemy->aimed_dir];
		t_penemy->yspeed =spd +(spd<<1);
		spd =yxspds[t_penemy->aimed_dir+3];
		t_penemy->xspeed =spd +(spd<<1);
	}
	refresh_enemyxy();
}

void e_flower_st4(){
	bombing_init(0);
}

void e_flower_st5(){
	bombing_proc(0);
}

void e_flower_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void flower_scorpion_form(int baseform){
	static int formdeltas[]={0,1,2,1,0xff};
	int d;

	if (--t_penemy->form_cnt==0){
		d =formdeltas[t_penemy->form_idx++];
		if (d==0xff){
			t_penemy->form_idx =0;
			d =formdeltas[t_penemy->form_idx++];
		}
		*t_penemy_dform =baseform+d;
		t_penemy->form_cnt=8;
	}
}

void e_scorpion_st1_sub();

void e_scorpion_st1(){
	set_scorpion_hp_delay_yflip();
	e_scorpion_st1_sub();
}

void e_scorpion_st1_sub(){
	*t_penemy_dform =0xb3;
	t_penemy->xspeed =-0x0180;
	t_penemy->yspeed =0;
	t_penemy->air_jump_f =(framecount+(get_rand8()>>1)) &2;
	t_penemy->utemp =0;
	t_penemy->state =4;
}

void e_scorpion_st2(){
	t_penemy->score_idx =3;
	t_penemy->col_type =3;
	set_scorpion_hp_delay_yflip();
	*t_penemy_dform =0xb6;
	t_penemy->xspeed =-0x00b0;
	t_penemy->oval_yspd =-0x0400;
	t_penemy->state++;
}

void e_scorpion_st3(){
	t_penemy->oval_yspd +=0x28;
	if (*t_penemy_y>=0x80)
		t_penemy->yspeed =t_penemy->oval_yspd;
	else
		t_penemy->yspeed =-t_penemy->oval_yspd;
	refresh_enemyxy();
	if (*t_penemy_y>=0xc1 || *t_penemy_y<0x30){
		*t_penemy_dform =0xb3;
		e_scorpion_st1_sub();
	}
}

void e_scorpion_st4(){
	int skip=0;

	flower_scorpion_form(0xb3);
	if (!t_penemy->utemp && t_penemy->air_jump_f && chr_ys[0]>=0x20 &&
		(unsigned int)(*t_penemy_x-chr_xs[0])<0x30){
		if (*t_penemy_y<chr_ys[0])
			t_penemy->yspeed =0x0240+(diff_extra<<4);
		else{
			if ((*t_penemy_x-chr_xs[0])<0x20)
				t_penemy->yspeed =-0x0100;
			else
				skip=1;
		}
		if (!skip){
			*t_penemy_dform =0xb3;
			*t_penemy_dattr &=0x3f;
			if (t_penemy->yspeed>0)
				t_penemy->xspeed =-0x0080;
			t_penemy->utemp++;
			refresh_enemyxy();
			if (t_penemy->state)
				t_penemy->state++;
			return;
		}
	}
	if (*t_penemy_y<0x38)
		*t_penemy_y =0x38;
	if (*t_penemy_y>=0xb8)
		*t_penemy_y =0xb8;
	t_penemy->yspeed =0;
	refresh_enemyxy();
}

void e_scorpion_st5(){
	check_e_bg(0, 0);
	if (bg_plat_f){
		t_penemy->yspeed =0;
		t_penemy->xspeed =-0x0180;
		t_penemy->state=4;
	}
	refresh_enemyxy();
}

void e_scorpion_st6(){
	bombing_init(0);
}

void e_scorpion_st7(){
	bombing_proc(0);
}

void e_scorpion_st8(){
	move_enemy_scroll();
	enemy_selfclear();
}

void set_scorpion_hp_delay_yflip(){
	t_penemy->hp =1+num_of_allclears+diff_extra;
	t_penemy->form_cnt =6;
	*t_penemy_dattr =(*t_penemy_y<0x80) ?0x80:0;
}

void e_scorpiongen_st1(){
	t_penemy->hp =0x18+((num_of_allclears+diff_extra)<<1);
	t_penemy->delay =0xa0+((framecount+get_rand8()) &0x3f);
	t_penemy->form_idx =(*t_penemy_y<0x80) ?0xa1:0xa5;
	t_penemy->form_cnt=0;
	t_penemy->utemp =0;
	t_penemy->state++;
}

void e_scorpiongen_st2(){
	move_enemy_scroll();
	if (t_penemy->form_cnt==0){
		t_penemy->delay--;
		if (!t_penemy->delay)
			gen_scorpion();
		else if (t_penemy->delay==0x30 || t_penemy->delay==0x10)
			scorpiongen_form();
	}else{
		if (--t_penemy->delay==0){
			t_penemy->delay =0x14;
			scorpiongen_form();
			if (--t_penemy->form_cnt==0)
				gen_scorpion();
		}
	}
}

void e_scorpiongen_st3(){
	int bno=(*t_penemy_y<0x80) ?0xa4:0xa8;

	enemy_bgobj_4x4(bno);
	t_penemy->state++;
}

void e_scorpiongen_st4(){
	bombing_init(0);
}

void e_scorpiongen_st5(){
	bombing_proc(0);
}

void e_scorpiongen_st6(){
	move_enemy_scroll();
	enemy_selfclear();
}

void scorpiongen_form(){
	t_penemy->form_idx++;
	if (++t_penemy->utemp>=3){
		t_penemy->form_idx -=2;
		t_penemy->utemp -=2;
	}
	enemy_bgobj_4x4(t_penemy->form_idx);
}

void gen_scorpion(){
	int no;

	if (player_normal){
		if((no=gen_new_enemy(0,0, 0x14))>=0){
			enemy[no].state =2;
		}
	}
	t_penemy->form_cnt =0xa+((framecount+get_rand8()) &3) -(player.gun_type[player.crt_gun]&3);
	t_penemy->delay =0x14;
}

void e_s7boss_st1(){
	int hp =0x37+((diff_extra+num_of_allclears)<<4);

	t_penemy->hp =(hp>0xa0) ?0xa0:hp;
	t_penemy->delay =0xa;
	t_penemy->form_idx=0;
	t_penemy->state++;
}

void e_s7boss_st2(){
	move_enemy_scroll();
	if (--t_penemy->delay==0){
		t_penemy->delay =t_penemy->hp>>1;
		if (t_penemy->delay==0)
			t_penemy->delay++;
		t_penemy->state++;
	}
}

void e_s7boss_st3(){
	int idx;

	move_enemy_scroll();
	t_penemy->form_idx++;
	idx =(t_penemy->form_idx &1)<<1;
	show_blks_by_no(3+idx);
	show_blks_by_no(4+idx);
	t_penemy->state =2;
}

void e_s7boss_st4(){
	stop_bgm();
	stageclear_init(BOMBING3);
	move_enemy_scroll();
	bombing_gen(*t_penemy_x, *t_penemy_y);
	t_penemy->delay =5;
	t_penemy->state++;
}

void e_s7boss_st5(){
	e_demon_st5();
}

void e_s7boss_st6(){
	show_blks_by_no(7);
	t_penemy->state++;
}

void e_s7boss_st7(){
	show_blks_by_no(8);
	t_penemy->state++;
}

void e_s7boss_st8(){
	s7emy_allclear();
}


short calc_re_delay(int calc);
int get_re_tempx_check(int tempy, int *ptempx);
int re_tempxy_check(int tempx, int tempy);

void enemy_gen_random0();
void enemy_gen_random1();
void enemy_gen_random2();

static SUB enemy_gen_random_proc[] ={
	enemy_gen_random0, enemy_gen_random1, enemy_gen_random2
};

void enemy_gen_random(){
	if (screen_page!=random_e_crt_page){
		random_e_crt_page =screen_page;
		random_e_cnt =0;
	}
	enemy_gen_random_proc[random_e_gen_state]();
}

void enemy_gen_random0(){
	if (calc_re_delay(0)){
		calc_re_delay(1);
		random_e_gen_state++;
	}
}

void enemy_gen_random1(){
	int tempx, tempy, originaly;
	int mode;

	if (!(framecount &1) || !scroll_to_do){
		random_e_delay -=2;
		if (random_e_delay>=0)
			return;
	}else{
		if (random_e_delay--)
			return;
	}
	calc_re_delay(1);
	tempy =originaly =chr_ys[0];
	mode =(framecount+get_rand8()) &3;
	switch (mode){
	case 0:
		tempy =originaly =0;
		get_re_tempx_check(tempy, &tempx);
		while (1){
			tempy +=0x10;
			if (tempy>=0x100)
				tempy -=0x100;
			if (tempy ==originaly)
				return;
			if (re_tempxy_check(tempx,tempy))
				return;
		}
	case 1:
		tempy =originaly =0xf0;
		get_re_tempx_check(tempy, &tempx);
		while (1){
			tempy -=0x10;
			if (tempy<0)
				tempy +=0x100;
			if (tempy ==originaly)
				return;
			if (re_tempxy_check(tempx,tempy))
				return;
		}
	case 2:
	case 3:
		get_re_tempx_check(tempy, &tempx);
		while (1){
			tempy -=0x10;
			if (tempy<0)
				tempy +=0x100;
			if (tempy ==originaly)
				return;
			if (re_tempxy_check(tempx,tempy))
				return;
		}
	}
}

void enemy_gen_random2(){
	static unsigned char flag2s[]={
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x04,0x04,0x00,0x04,0x04,0x04,
		0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x04,0x08,
	};
	int tx, no, idx, left,i;
	unsigned char *pgdata, v;
	struct ENEMY *penemy;

	random_e_gen_state=0;
	if (!screen_page || (stage!=2 && random_e_y<0x40) || 
		random_e_y>=0xe0 || !player_normal)
		return;
	if (stage==4 && screen_page>=0x11 && random_e_x<0x80)
		return;
	solid_or_solidunderplat(random_e_x, random_e_y);
	if (bg_solid_f)
		return;
	tx =(random_e_x<0x80) ?random_e_x+8 :random_e_x-8;
	solid_or_solidunderplat(tx, random_e_y);
	if (bg_solid_f)
		return;
	if ((no =find_first6_free_emy())<0)
		return;
	pgdata =re_gdatas[stage];
	v =pgdata[screen_page-1];
	if (v==0xff)
		return;
	if ((v&0x80) && (framecount &1))
		return;
	if ((v&0x40) && (framecount &3))
		return;
	if (random_e_cnt<30 && !num_of_allclears){
		if (random_e_x<0x80){
			if (chr_xs[0]<0x40)
				return;
		}else{
			if (chr_xs[0]>=0xc0)
				return;
		}
	}
	left =(random_e_x<0x80) ?1:0;
	if (stage==2 || scroll_to_do==0 || (get_rand8()&3)){
		idx =0xa+no;
		penemy =&enemy[no];
		enemy_common_init(no, 5);
		penemy->flag2 =flag2s[((v&0x3f)<<2)+(framecount &3)];
		penemy->flag2 |=(framecount+get_rand8()) & ENEMY_CAN_TURNBACK;
		chr_xs[idx] =random_e_x;
		chr_ys[idx] =random_e_y;
		penemy->flag2 |=left;
		random_e_cnt++;
	}else{
		for (i=2; i>=0; i--){
			if ((no =find_first6_free_emy())<0)
				return;
			idx =0xa+no;
			penemy =&enemy[no];
			enemy_common_init(no, 5);
			chr_xs[idx] =random_e_x;
			chr_ys[idx] =random_e_y;
			penemy->flag2 =((framecount+get_rand8()) & ENEMY_CAN_TURNBACK) |left;
			penemy->flag2 |=(i<<4);
		}
	}
}

short calc_re_delay(int calc){
	static unsigned char delays[]={
		0x90,0x00,0xd8,0x00,0xd0,0xc8,0xc0,0x00
	};
	int temp;

	random_e_delay =delays[stage];
	if (!calc)
		return random_e_delay;
	temp =(num_of_allclears>3) ?3:num_of_allclears;
	random_e_delay -=temp*0x28;
	if (diff_extra)
		random_e_delay -=diff_extra*5;
	return random_e_delay;
}

int get_re_tempx_check(int tempy, int *ptempx){
	static int random_xs[]={
		0xfa,0x0a,0xfa,0xfa,0x0a,0xfa,0x0a,0xfa,0x0a,0x0a,0x0a,0xfa,0xfa,0x0a,0x0a,0xfa,
	};

	if (!num_of_allclears && stage==0 && random_e_cnt<30)
		*ptempx =0xfc;
	else
		*ptempx =random_xs[(framecount+get_rand8()) &0xf];
	return re_tempxy_check(*ptempx, tempy);
}

int re_tempxy_check(int tempx, int tempy){
	check_bg(tempx, tempy, 0);
	if (!bg_plat_f)
		return 0;
	else{
		random_e_x =tempx;
		random_e_y =tempy-0x10;
		random_e_gen_state =2;
		return 1;
	}
}

void refresh_enemyxy(){
	if (vstage_f){
		refresh_enemyacty();
		if (*t_penemy_y>=0xef ||*t_penemy_y<-8)
			enemy_selfclear();
		else{
			refresh_enemyx();
			if (*t_penemy_x<0 || *t_penemy_x>=0x100)
				enemy_selfclear();
		}
	}else{
		refresh_enemyactx();
		if (*t_penemy_x<0 || *t_penemy_x>=0x100)
			enemy_selfclear();
		else{
			refresh_enemyy();
			if (*t_penemy_y>=0xef ||*t_penemy_y<-8)
				enemy_selfclear();
		}
	}
}

void refresh_bonusxy(){
	int tspd_int;

	if (vstage_f){
		tspd_int =(t_penemy->yspeed>>8)+scroll_to_do;
		if (refresh_bonusy(tspd_int))
			enemy_selfclear();
		else{
			refresh_enemyx();
			if (*t_penemy_x<0 || *t_penemy_x>=0x100)
				enemy_selfclear();
		}
	}else{
		refresh_enemyactx();
		if (*t_penemy_x<0 || *t_penemy_x>=0x100)
			enemy_selfclear();
		else{
			tspd_int =(t_penemy->yspeed>>8);
			if (refresh_bonusy(tspd_int))
				enemy_selfclear();
		}
	}
}

int refresh_bonusy(int spd){
	short cspd= (spd<<8) +(t_penemy->yspeed&0xff);
	t_penemy->y_acum &=0xff;
	t_penemy->y_acum +=cspd;
	*t_penemy_y +=t_penemy->y_acum>>8;
	if (*t_penemy_y>=0x100)
		return 1;
	if (*t_penemy_y<0)
		t_penemy->form_idx =0xff;
	else
		t_penemy->form_idx =0;
	return 0;
}

void enemy_selfclear(){
	t_penemy->state=0;
	*t_penemy_dform=0;
}

void move_enemy_scroll(){
	if (vstage_f){
		*t_penemy_y +=scroll_to_do;
		if (*t_penemy_y>=0xef)
			enemy_selfclear();
	}else{
		*t_penemy_x -=scroll_to_do;
		if (*t_penemy_x<0 || *t_penemy_x>=0x100)
			enemy_selfclear();
	}
}

void refresh_enemyactx(){
	refresh_enemyx();
	*t_penemy_x -=scroll_to_do;
}

void refresh_enemyx(){
	t_penemy->x_acum &=0xff;
	t_penemy->x_acum +=t_penemy->xspeed;
	*t_penemy_x +=t_penemy->x_acum>>8;
}

void refresh_enemyacty(){
	refresh_enemyy();
	*t_penemy_y +=scroll_to_do;
}

void refresh_enemyy(){
	t_penemy->y_acum &=0xff;
	t_penemy->y_acum +=t_penemy->yspeed;
	*t_penemy_y +=t_penemy->y_acum>>8;
}

void combine_line_oval_y(){
	t_penemy->oval_ylocus +=t_penemy->oval_yspd;
	t_penemy->y_acum &=0xff;
	t_penemy->y_acum +=t_penemy->yspeed;
	t_penemy->line_ylocus +=t_penemy->y_acum>>8;
	if (t_penemy->line_ylocus>=0xf0)
		enemy_selfclear();
	else{
		*t_penemy_y= t_penemy->line_ylocus +(t_penemy->oval_ylocus>>8);
		refresh_enemyx();
		if (*t_penemy_x<0 || *t_penemy_x>=0x100)
			enemy_selfclear();
	}
}

unsigned char check_e_bg(int xdelta, int ydelta){
	int x= *t_penemy_x+xdelta;
	int y= *t_penemy_y+ydelta;

	if (y>=0x256){
		bg_val=0;
		bg_plat_f=0;
		bg_solid_f=0;
		bg_empty_f=1;
		return 0;
	}
	return (check_bg(x,y,0));
}

void enemy_y_regulate(int delta){
	short tmp;

	tmp= (vscroll&0x0f)-0x10;
	*t_penemy_y = ((*t_penemy_y+tmp) & 0xfffffff0) -tmp+delta;
}

void enemy_common_init(int no, int type){
	struct ENEMY *penemy =&enemy[no];
	int tblidx =(type<0x10) ?8 :stage;
	unsigned char *ptbl, c;


	memset(penemy,0,sizeof(struct ENEMY));
	penemy->type =type;
	penemy->state =1;
	disp_forms[0xa +no]=1;
	disp_attr[0xa +no]=0;
	if (type>0xf)
		type -=0x10;
	ptbl=&enemy_cmm_idata[tblidx][type<<2], c;
	penemy->flag1 =ptbl[0];
	c=ptbl[1];
	penemy->score_idx =c>>4;
	penemy->col_type =c&0xf;
	penemy->hp =ptbl[2];
	penemy->hitsfx_idx =ptbl[3];
}

void enemy_gen_scroll(){
	unsigned char **pstage=enemy_gdatas[stage];
	unsigned char *ppage_of_stage=pstage[screen_page];
	unsigned char c, c2;
	int offbound, type, cnt, i,j,no;

	if (zstage_f){
		if (emydata_ptr)
			return;
		emydata_ptr++;
		enemy_rounds=0;
		b_robject_clred=0;
		bjumper_appeared=0;
		grenadier_active=0;
		for (i=0xf; i>=0; i--){
			t_penemy=&enemy[i];
			t_penemy_dform =&disp_forms[0xa+i];
			enemy_selfclear();
		}
		i=0;
		if ((c=ppage_of_stage[i++])==0xff)
			return;
		s_robject_left=c;
		if (!s_robject_left)
			room_clear=1;
		for (j=0xf; j>=0; j--){
			if ((c=ppage_of_stage[i++])==0xff)
				break;
			c2=ppage_of_stage[i++];
			type =c2&0x3f;
			enemy_common_init(j, type);
			if (c2 &0x80)
				chr_ys[0xa +j] = (c & 0xf0)+8;
			else
				chr_ys[0xa +j] = (c & 0xf0);
			if (c2 &0x40)
				chr_xs[0xa +j] = ((c & 0xf)<<4)+8;
			else
				chr_xs[0xa +j] = ((c & 0xf)<<4);
			enemy[j].flag2 =ppage_of_stage[i++];
		}
	}else{
		if ((c=ppage_of_stage[emydata_ptr])!=0xff){
			if ((offbound=(c&0xfe)-screen_off)<=0){
				offbound=-offbound;
				c2=ppage_of_stage[++emydata_ptr];
				type =c2&0x3f;
				cnt = (c2&0xc0)>>6;
				for (i=0; i<=cnt; i++){
					c2=ppage_of_stage[++emydata_ptr];
					if ((no=find_free_emy())==-1){
						if (!(c & 1))
							continue;
						no=find_emy_sac();
					}
					enemy_common_init(no, type);
					enemy[no].flag2 =c2 & 0xf;
					if (vstage_f){
						chr_xs[0xa +no] =c2 & 0xf0;
						chr_ys[0xa +no] =offbound;
					}else{
						chr_xs[0xa +no] =0xf0-offbound;
						chr_ys[0xa +no] =c2 & 0xf0;
					}
				}//for (i=0; i<=cnt; i++)
				emydata_ptr++;
			}//if ((offbound=(c&0xfe)-screen_off)<=0)
		}//if ((c=ppage_of_stage[emydata_ptr])!=0xff)
	}// elseof if (zstage_f)
}

int find_free_emy(){
	int i;
	for (i=0xf; i>=0; i--){
		if (!enemy[i].state)
			break;
	}
	return i;
}

int find_first6_free_emy(){
	int i;
	for (i=6; i>=0; i--){
		if (!enemy[i].state)
			break;
	}
	return i;
}

int find_emy_sac(){
	int i;
	for (i=0xf; i>=0; i--){
		if (enemy[i].type==1)
			return i;
	}
	return 0;
}

void enemy_player_collide(){
	int mode=0;
	unsigned char ehp, v,m;
	int btype;
	short taccum;


	if (player.state!=1)
		return;
	if (zstage_f &1){
		if (t_penemy->type==1 && player.anim==2)
			return;
	}else{
		if (t_penemy->flag1 & ENEMY_IS_CAR){
			if (t_penemy->flag1 & ENEMY_CAR_BIT){//第7关的小车
				if ((*t_penemy_y-chr_ys[0])>=8)
					t_penemy->flag1 &= ~ENEMY_CAR_CANTRIDE;
				else
					t_penemy->flag1 |= ENEMY_CAR_CANTRIDE;
			}else{
				if (player.jump_f && player.yspeed<0x0100)
					return;
			}
		}
	}
	if (player.inwater_f){
		if (player.keys & 4)
			return;
	}else if (player.jump_f){
		mode=1;
	}else if (player.form==0x17){//趴下
		mode=2;
	}else
		mode=3;
	col_detect(mode);
	if ((unsigned int)(chr_ys[0]-t_col_ycheck)>=t_col_ydist)
		return;
	if ((unsigned int)(chr_xs[0]-t_col_xcheck)>=t_col_xdist)
		return;
	if ((t_penemy->flag1 & ENEMY_IS_CAR) && !(t_penemy->flag1 & ENEMY_CAR_CANTRIDE)){
		if (chr_ys[0]>=player.free_dist){
			t_penemy->form_idx=1;
			taccum =t_penemy->x_acum & 0xff;
			taccum +=t_penemy->xspeed;
			player.carspeed +=taccum & 0xff00;
			chr_ys[0] =(t_penemy->flag1 &ENEMY_CAR_BIT)? *t_penemy_y-0x18 :*t_penemy_y-0x1c;
			player.on_car =1;
			player_landing();
		}
		return;
	}else{
		if (t_penemy->type){
			if (player.inv_cnt)
				return;
			if (player.invul_cnt){
				ehp=t_penemy->hp;
				if (ehp && ehp<0xf0){
					t_penemy->hp=0;
					p_addscore_e_death();
				}
			}else{
				player_death_init();
				if (t_penemy->type==1)
					enemy_selfclear();
			}
		}else{
			player_addscore(10);
			PLAYSOUND0(BONUS);
			btype =t_penemy->flag2 & ENEMY_BONUSTYPE;
			if (btype==6){
				enemy_allclear();
				flash_cnt =0x20;
			}else if (btype==5){
				player.invul_cnt =(stage==6) ?0x90:0x80;
			}else{
				if (btype){
					v=btype;
					m=((player.gun_type[player.crt_gun] ^btype)&0xf)?0xe0:0xf0;
				}else{
					v=0x10;
					m=0xff;
				}
				player.gun_type[player.crt_gun] = (player.gun_type[player.crt_gun] & m) |v;
			}
			enemy_selfclear();
		}//bonus
	}//not car
}

void enemy_pb_collide(){
	static int hitsnd[]={HITSND1,HITSND1,HITSND1,HITSND2,HITSND0};
	int i,j;
	struct P_BULLET *pb;

	col_detect(4);
	for (i=0xf; i>=0; i--){
		pb=&p_bullet[i];
		if (!pb->form || pb->state!=1)
			continue;
		if (zstage_f &1){
			if (t_penemy->flag1 & ENEMY_LOW_POSITION){
				if (!(pb->atype &0x80))
					continue;
			}else{
				if (pb->counter>1)
					continue;
			}
		}
		if ((unsigned int)(pb->y-t_col_ycheck)>=t_col_ydist)
			continue;
		if ((unsigned int)(pb->x-t_col_xcheck)>=t_col_xdist)
			continue;
		if(t_penemy->hp && t_penemy->hp<0xf0){
			if (--t_penemy->hp){
				if (t_penemy->flag1 &ENEMY_HITSND)
					PLAYSOUND3(hitsnd[t_penemy->hitsfx_idx]);
			}else{
				p_addscore_e_death();
			}
		}

		if ((pb->atype & 0xf)==5){
			j=0;
			while(j<i){
				if (p_bullet[j].state==1)
					break;
				j++;
			}
			i=j;
		}
		p_bullet_vanish(i);
		return;
	}//for
}

void col_detect(int mode){
	unsigned char *ptbl, c;
	int tmp, dlen;

	if (t_penemy->col_type==0xf){
		dlen=t_penemy->dyn_len;
		if ((t_penemy->flag2 & ENEMY_FLAME_REVERSE)==ENEMY_FLAME_REVERSE)
			dlen=-dlen;
		dlen +=8;
		ptbl =&cd_dyn_data0[mode<<2];
		c=*ptbl++;
		t_col_ycheck =EXT(c);
		c=*ptbl++;
		t_col_xcheck =EXT(c);
		t_col_ydist =*ptbl++;
		t_col_xdist =*ptbl++;
		ptbl =&cd_dyn_data1[(t_penemy->flag2 & ENEMY_FLAME_TYPE)>>4];
		c=*ptbl++;
		if (c>=0xfe){
			tmp=(c==0xfe)? -dlen :dlen;
		}else{
			tmp=EXT(c);
		}
		t_col_ycheck =*t_penemy_y +t_col_ycheck+tmp;
		c=*ptbl++;
		if (c>=0xfe){
			tmp=(c==0xfe)? -dlen :dlen;
		}else{
			tmp=EXT(c);
		}
		t_col_xcheck =*t_penemy_x +t_col_xcheck+tmp;
		c=*ptbl++;
		if (c>=0xfe){
			tmp=(c==0xfe)? -dlen :dlen;		//数据只可能0xff
		}else{
			tmp=EXT(c);
		}
		t_col_ydist +=tmp;
		c=*ptbl++;
		if (c>=0xfe){
			tmp=(c==0xfe)? -dlen :dlen;		//数据只可能0xff
		}else{
			tmp=EXT(c);
		}
		t_col_xdist +=tmp;
	}else{
		ptbl=&col_detect_data[(mode<<6)-(mode<<2)];
		ptbl=&ptbl[t_penemy->col_type<<2];
		c=*ptbl++;
		tmp=EXT(c);
		t_col_ycheck =*t_penemy_y+tmp;
		c=*ptbl++;
		tmp=EXT(c);
		t_col_xcheck =*t_penemy_x+tmp;
		t_col_ydist =*ptbl++;
		t_col_xdist =*ptbl++;
	}
}

void p_addscore_e_death(){
	static unsigned char scores[]={0x00,0x01,0x03,0x05,0x0a,0x14,0x1E,0x32,0x64,0x96};

	if (t_penemy->score_idx==0xa)
		player_addscore(5000);
	else
		player_addscore(scores[t_penemy->score_idx]);
	t_penemy->score_idx=0;

	e_death_state(t_penemy);
}

void e_death_state(struct ENEMY *penemy){
	static unsigned char s0_dstates[]={0x33,0x20};
	static unsigned char s1_dstates[]={0x43,0x45,0x53,0x33,0x43,0x33,0x43,0x54,0x30};
	static unsigned char s2_dstates[]={0x22,0x24,0x65};
	static unsigned char s4_dstates[]={0x33,0x50,0xa5,0x20};
	static unsigned char s5_dstates[]={0x00,0x07,0x30};
	static unsigned char s6_dstates[]={0x05,0x30,0x44,0x35,0x50};
	static unsigned char s7_dstates[]={0x43,0x34,0x63,0x40};
	static unsigned char cmm_dstates[]={0x04,0x53,0x75,0x56,0x50,0x44,0x44,0x43};
	static unsigned char *dstates[]={
		s0_dstates, s1_dstates, s2_dstates, s1_dstates,
		s4_dstates, s5_dstates, s6_dstates, s7_dstates,
		cmm_dstates
	};
	int idx=8;
	unsigned char *pdstates, ttype, newstate;

	ttype =penemy->type;
	if (ttype>0xf){
		idx=stage;
		ttype-=0x10;
	}
	pdstates =dstates[idx];
	newstate =pdstates[ttype>>1];
	newstate =(ttype&1) ?newstate&0xf: (newstate>>4)&0xf;
	if (newstate>=penemy->state)
		penemy->state =newstate;
}

void enemy_allclear(){
	int i;
	struct ENEMY *penemy;
	unsigned char c;

	for (i=0; i<0x10; i++){
		penemy=&enemy[i];
		c=penemy->type;
		if (penemy->state && disp_forms[0xa+i] &&
			c!=2 && c!=3 && penemy->hp!=0xf0){
			e_death_state(penemy);
			penemy->flag2 |=ENEMY_CLEARED;
		}
	}
}

void bombing_gen(int x, int y){
	int no=find_free_emy();
	struct ENEMY* penemy;
	int idx=0xa +no;

	if (no<0)
		return;
	enemy_common_init(no, 2);
	penemy =&enemy[no];
	penemy->state=6;
	disp_forms[idx]=1;
	penemy->flag1 =ENEMY_NONSHOOTABLE |ENEMY_NONCOL |ENEMY_BOMBING_TYPE1;
	chr_xs[idx]=x;
	chr_ys[idx]=y;
}

void bombing_init(int type){
	if (type){
		t_penemy->col_type=0xd;
		t_penemy->flag1 &=~(ENEMY_IS_CAR |ENEMY_NONCOL);
		t_penemy->flag1 |=ENEMY_NONSHOOTABLE;
	}else{
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	}
	if (t_penemy->flag1 & ENEMY_BOMBING_SND)
		PLAYSOUND3(BOMBING0);
	*t_penemy_dattr = (*t_penemy_dattr & 0xfc) | 6;
	if (!*t_penemy_dform)
		enemy_selfclear();
	t_penemy->form_idx=0xff;
	*t_penemy_dform=1;
	move_enemy_scroll();
	t_penemy->delay=1;
	if (t_penemy->state)
		t_penemy->state++;
}

void bombing_proc(int type){
	static unsigned char bforms[]={
		4, 7, 0xb, 0xe,
		0x38,0x39,0x3a,0x37,0x35,0x36,0x37,0x9d,0x9e,0x9f,0x36,0x37,
	};
	int btype=type, forms;
	unsigned char *pforms;

	if (type==2){
		forms=3;
	}else if (type==3){
		forms=2;
	}else{
		if (t_penemy->flag1 &ENEMY_BOMBING_TYPE1){
			btype=1; forms=4;
		}else{
			btype=0; forms=3;
		}
	}
	move_enemy_scroll();
	if (!t_penemy->state || --t_penemy->delay)
		return;
	t_penemy->form_idx++;
	if (t_penemy->form_idx ==forms){
		t_penemy->state++;
		return;
	}
	if ((t_penemy->form_idx+1)==forms)
		t_penemy->flag1 |=(ENEMY_NONSHOOTABLE |ENEMY_NONCOL);
	t_penemy->delay =0xa;
	pforms =&bforms[bforms[btype]];
	*t_penemy_dform=pforms[t_penemy->form_idx];
}

void enemy_bgobj_4x4(int bno){
	int x=*t_penemy_x-0xc;
	int y=*t_penemy_y-0xc;
	if (x<0 || y<0)
		return;
	show_bgobj_4x4(x,y,bno);
}

void enemy_bgobj_4x4xv2(int bno0, int bno1, int keep_bgval){
	int nx,ny;

	enemy_bgobj_4x4(bno0);
	if (!keep_bgval)
		set_bgval_4x4(0x00, 0x0f);
	nx =*t_penemy_x-0xc;
	ny =*t_penemy_y+0x14;
	show_bgobj_4x4(nx,ny,bno1);
	if (!keep_bgval)
		set_bgval_4x4(0x01, 0x01);
}

void enemy_bgobj_2xn(int bno){
	int x=*t_penemy_x-0x4;
	int y=*t_penemy_y-0x4;
	if (x<0 || y<0)
		return;
	show_bgobj_2xn(x,y,bno);
}

void new_e_pos(int xdelta, int ydelta, int *x, int *y){
	*x =*t_penemy_x+xdelta;
	*y =*t_penemy_y+ydelta;
}

void rfsh_saucer_osc_xspd(){
	int delta =(*t_penemy_x - t_penemy->basex)<<1;

	t_penemy->xspeed = t_penemy->xspeed -delta;
}

void rfsh_saucer_osc_yspd(){
	int delta =(*t_penemy_y - t_penemy->basey)<<1;

	t_penemy->yspeed = t_penemy->yspeed -delta;
}

int get_p_e_xdist(){
	return (chr_xs[0]>*t_penemy_x) ?chr_xs[0]-*t_penemy_x :*t_penemy_x-chr_xs[0];
}

int get_p_e_ydist(){
	return (chr_ys[0]>*t_penemy_y) ?chr_ys[0]-*t_penemy_y :*t_penemy_y-chr_ys[0];
}

int fire_eb(int dirtype, int x,int y,int dir,int speedadj){
	int yxflag=0;
	int btype, adir, no, idx;
	struct ENEMY *pbullet;
	short xs, ys;

	if (!dirtype)
		dir<<=1;
	btype=(dir & 0xe0)>>5;
	adir =dir & 0x1f;
	if (adir>6 && adir <0x12)
		yxflag |=2;
	if (adir>0xc)
		yxflag ++;
	if (btype!=1 && !player_normal)
		return 0;
	if((no=find_free_emy())<0)
		return 0;
	pbullet =&enemy[no];
	idx=0xa+no;
	enemy_common_init(no, 1);
	pbullet->bullet_type=btype;
	if (speedadj>7)
		speedadj=7;
	chr_ys[idx]=y;
	chr_xs[idx]=x;
	get_eb_spd(adir, speedadj, yxflag, &xs, &ys);
	pbullet->xspeed =xs;
	pbullet->yspeed =ys;
	return 1;
}

void fire_eb_aimed_dir18(int x, int y, int btype, int speedadj){
	int adir, yxflag, no, idx;
	struct ENEMY *pbullet;
	short xs, ys;

	yxflag=get_dir_aimed(x,y, 1, &adir);
	if (btype!=1 && !player_normal)
		return;
	if((no=find_free_emy())<0)
		return;
	pbullet =&enemy[no];
	idx=0xa+no;
	enemy_common_init(no, 1);
	pbullet->bullet_type=btype;
	if (speedadj>7)
		speedadj=7;
	chr_ys[idx]=y;
	chr_xs[idx]=x;
	get_eb_spd(adir, speedadj, yxflag, &xs, &ys);
	pbullet->xspeed =xs;
	pbullet->yspeed =ys;
	return;
}

int get_dir_aimed(int x, int y, int dirmode, int *newdir){
	static unsigned char dirs[]={
		0x00,0x00,0x00,0x00,0x32,0x11,0x00,0x00,0x32,0x11,0x11,0x11,0x32,0x22,0x11,0x11,
		0x33,0x22,0x11,0x11,0x33,0x22,0x22,0x11,0x33,0x22,0x22,0x11,0x33,0x22,0x22,0x22,
		0x00,0x00,0x00,0x00,0x63,0x21,0x11,0x11,0x64,0x32,0x21,0x11,0x65,0x43,0x22,0x22,
		0x65,0x44,0x33,0x22,0x65,0x54,0x33,0x32,0x65,0x54,0x43,0x33,0x65,0x54,0x44,0x33,
		0x80,0x00,0x00,0x00,0xf8,0x53,0x32,0x21,0xfb,0x86,0x54,0x33,0xfd,0xa8,0x75,0x54,
		0xfe,0xb9,0x87,0x65,0xfe,0xcb,0x98,0x76,0xfe,0xdb,0xa9,0x87,0xff,0xdc,0xba,0x98,
	};
	int xaimed, yaimed, xdist, ydist;
	int yxflag=0;
	unsigned char *pdirs, c;

	if (player.state==1){
		xaimed=chr_xs[0];
		yaimed=(zstage_f&1) ?0xb0 :chr_ys[0];
	}else{
		xaimed=0x80;	yaimed=0xff;
	}

	ydist =yaimed-y;
	if (ydist<0){
		ydist =-ydist;
		yxflag |=1;
	}
	ydist>>=5;
	xdist =xaimed-x;
	if (xdist<0){
		xdist =-xdist;
		yxflag |=2;
	}
	xdist>>=5;

	pdirs=&dirs[dirmode<<5];
	c =pdirs[(ydist<<2) + (xdist>>1)];
	*newdir=(xdist &1) ?c&0xf :c>>4;
	return yxflag;
}

void get_eb_spd(int dir, int speedadj, int yxflag, short *xspd, short *yspd){
	static unsigned int spdidx[]={
		0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0a,0x08,0x06,0x04,0x02,0x00,0x02,0x04,0x06,
		0x08,0x0a,0x0c,0x0a,0x08,0x06,0x04,0x02
	};
	static unsigned short spds[]={
		0x00,0xff,0x42,0xf7,0x80,0xdd,0xb5,0xb5,0xdd,0x80,0xf7,0x42,0xff,0x00	
	};
	unsigned short *pspds=&spds[spdidx[dir]];

	int spd=*pspds++;
	*yspd =adjust_eb_spd(spd, speedadj);
	spd=*pspds;
	*xspd =adjust_eb_spd(spd, speedadj);
	if (yxflag &1)
		*yspd = -(*yspd);
	if (yxflag &2)
		*xspd = -(*xspd);
}

short adjust_eb_spd(short spd, int speedadj){
	switch (speedadj){
	case 0:	//1/2
		return spd>>1;
		break;
	case 1:	//3/4
		return (spd>>1)+(spd>>2);
		break;
	case 2:
		return spd;
		break;
	case 3:	//5/4
		return spd+(spd>>2);
		break;
	case 4:	//3/2
		return spd+(spd>>1);
		break;
	case 5:	//13/8
		return spd+(spd>>1)+(spd>>3);
		break;
	case 6:	//7/4
		return spd+(spd>>1)+(spd>>2);
		break;
	case 7:	//15/8
		return spd+(spd>>1)+(spd>>2)+(spd>>3);
		break;
	default:
		break;
	}
	return 0;	//never
}

int get_dir_aimed_adjust(int x, int y, int dirmode, int *newdir){
	int yxflag;
	int dirs =(dirmode)?0x18:0xc;
	int halfdirs=dirs>>1;
	int hr_end, hr_end_wrap=0;

	yxflag=get_dir_aimed(x,y,dirmode, newdir);
	if (yxflag&2)
		*newdir=halfdirs-*newdir;
	if (yxflag&1){
		*newdir=dirs-*newdir;
		if (*newdir>=dirs)
			*newdir=0;
	}
	hr_end=t_penemy->aimed_dir+halfdirs;
	if (hr_end>=dirs){
		hr_end -=dirs;
		hr_end_wrap++;
	}
	if (*newdir==t_penemy->aimed_dir)
		return 2;
	if (hr_end_wrap){
		if (*newdir>t_penemy->aimed_dir || *newdir<hr_end)
			return 0;
		else 
			return 1;
	}else{
		if (*newdir<t_penemy->aimed_dir || *newdir>=hr_end)
			return 1;
		else 
			return 0;
	}
}

int get_dir_aimed_rotate(int x, int y, int dirmode, int *newdir){
	int dirs =(dirmode)?0x18:0xc;
	int halfdirs=dirs>>1;
	int r_mode;

	if ((r_mode=get_dir_aimed_adjust(x, y, dirmode, newdir))==2)
		return 1;
	if (r_mode){
		t_penemy->aimed_dir--;
		if (t_penemy->aimed_dir<0)
			t_penemy->aimed_dir=dirs-1;
	}else{
		t_penemy->aimed_dir++;
		if (t_penemy->aimed_dir>=dirs)
			t_penemy->aimed_dir=0;
	}
	if (t_penemy->aimed_dir==*newdir)
		return 1;
	else
		return 0;
}

int get_segdir_aimed_adjust(int segno, int *newdir){
	int idx=0xa +segno, nextno, next_rotno_actual;
	int yxflag;
	int hr_end, hr_end_wrap=0;

	yxflag=get_dir_aimed(chr_xs[idx], chr_ys[idx], 2, newdir);
	if (yxflag&2)
		*newdir=0x20-*newdir;
	if (yxflag&1){
		*newdir=0x40-*newdir;
	}
	*newdir &=0x3f;
	nextno =enemy[segno].seg_next;
	next_rotno_actual =enemy[nextno].seg_rotno_actual;
	hr_end =next_rotno_actual+0x20;
	if (hr_end>=0x40){
		hr_end -=0x40;
		hr_end_wrap++;
	}
	if (*newdir==next_rotno_actual)
		return 2;
	if (hr_end_wrap){
		if (*newdir>next_rotno_actual || *newdir<hr_end)
			return 0;
		else 
			return 1;
	}else{
		if (*newdir<next_rotno_actual || *newdir>=hr_end)
			return 1;
		else 
			return 0;
	}
}

int gen_new_enemy (int xdelta, int ydelta, int type){
	int no, idx;

	if ((no=find_free_emy())<0)
		return -1;
	idx =0xa+no;
	enemy_common_init(no, type);
	chr_xs[idx] =*t_penemy_x+xdelta;
	chr_ys[idx] =*t_penemy_y+ydelta;
	return no;
}
