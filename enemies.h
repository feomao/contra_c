#ifndef ENEMIES_H
#define ENEMIES_H

#define ENEMY_NONSHOOTABLE			0x80
#define ENEMY_IS_CAR				0x40
#define ENEMY_CAR_BIT				0x20
#define ENEMY_LOW_POSITION			0x20
#define ENEMY_CAR_CANTRIDE			0x10
#define ENEMY_BOMBING_TYPE1			0x08
#define ENEMY_HITSND				0x04
#define ENEMY_BOMBING_SND			0x02
#define ENEMY_NONCOL				0x01

#define ENEMY_FLAME_TYPE			0xc0
#define ENEMY_CLEARED				0x80
#define ENEMY_IS_MOVING_CAR			0x80
#define ENEMY_FLAME_REVERSE			0x40
#define ENEMY_BBOX_BGTYPE1			0x08
#define ENEMY_SOLDIER_FIRETYPE1		0x08
#define ENEMY_SOLDIER_FIRETYPE0		0x04
#define ENEMY_BIG_S_ROBJECT			0x08
#define ENEMY_HAS_SHIELD			0x04
#define ENEMY_BONUSTYPE				0x07
#define ENEMY_BREAK_TYPE			0x0c
#define ENEMY_DELAY_TYPE			0x03
#define ENEMY_LENGTH_TYPE			0x03
#define ENEMY_BULLETS_TYPE			0x03
#define ENEMY_CAN_TURNBACK			0x02
#define ENEMY_LEFT_SOLDIER			0x01
#define ENEMY_TANK_BGTYPE1			0x01

struct ENEMY{
	unsigned char	state;						//$04B8
	short			y_acum;						//$04C8
	short			x_acum;						//$04D8
	short			yspeed;						//$04F8,$04E8
	short			xspeed;						//$0518,$0508
	unsigned char	type;						//$0528
	short			delay;						//$0538
	unsigned char	hitsfx_idx;					//$0548
	short			form_cnt;					//$0548
	short			break_cnt;					//$0548 (flame)
	short			delay_btwn_fire;			//$0558
	unsigned char	form_idx;					//$0568
	unsigned char	hp;							//$0578
	unsigned char	score_idx;					//$0588:H
	unsigned char	col_type;					//$0588:L
	unsigned char	flag1;						//$0598
	unsigned char	flag2;						//$05A8
	unsigned char	line_ylocus;				//$05B8 (bonus)
	char			dyn_len;					//$05B8 (flame,s_wall)
	int				basey;						//$05B8 (saucer)
	char			aimed_dir;					//$05B8 (turret)
	char			fire_cnt;					//$05B8
	unsigned char	bullet_type;				//$05B8
	unsigned char	bombobj_idx;				//$05B8 (bridge)
	int				free_dist;					//$05B8 (stone)
	int				right_limit;				//$05B8 (aircar)
	unsigned char	sub_no;						//$05B8 (type1518gen)
	unsigned char	data_ptr;					//$05B8 (rolling_ggen)
	unsigned char	motion_state;					//$05C8
	int				basex;						//$05C8 (saucer)
	unsigned char	left;						//$05C8 (soldier)
	unsigned char	bombing_idx;				//$05C8 (bridge)
	char			shield_hp;					//$05C8 (s_robject)
	int				left_limit;					//$05C8 (aircar)
	unsigned char	total_len;					//$05C8	(flame)
	char			wall_step;					//$05C8
	unsigned char	action_no;					//$05C8 (runner4p)
	char			dyn_hlen;					//$05D8 (flame,s_wall)
	short			oval_ylocus;				//$05D8, 05C8 (bonus)
	unsigned char	air_jump_f;					//$05D8
	char			bullets;					//$05D8
	char			hand_step;					//$05D8
	unsigned char	still;						//$05D8
	unsigned char	turnback_cnt;				//$05E8
	char			dyn_vlen;					//$05E8 (flame,s_wall)

	short			oval_yspd;					//$0558, 05E8 (bonus)

	unsigned char	seg_active_cnt;				//$05C8
	unsigned char	seg_last;					//$05C8
	unsigned char	seg_next;					//$05D8
	unsigned char	seg_prev;					//$05E8
	char			seg_momentum_ored;				//$04E8
	unsigned char	seg_up;						//$04F8
	unsigned char	seg_rotno_actual;			//$0508
	unsigned char	endseg_no;					//$0518
	unsigned char	seg_swing_cnt;					//$0558
	unsigned char	seg_rot_state;					//$0568
	unsigned char	seg_rotno_local;			//$05B8
	char			seg_momentum;				//$05C8

	unsigned char	spd_idx;					//$05B8

	unsigned char	low_hp;

	unsigned char	mouth_bno;

	unsigned char	utemp;
};

extern struct ENEMY enemy[0x10];

void init_stage_emy_jtables();
void enemy_gen_scroll();
void enemy_proc();
void enemy_gen_random();

#endif
