#ifndef PLAYER_H
#define PLAYER_H
struct PLAYER{
	char			lives;				//$32
	unsigned char	go_flag;			//$38
	unsigned char	continues;			//$3A
	unsigned short	bonus_score;		//$3C
	unsigned char	state;				//$90
	short			x_acum;				//$92
	short			y_acum;				//$94
	short			xspeed;				//$98,96
	unsigned short	yspeed_zin;			//$9C,9A
	unsigned char	counter;			//$9E
	unsigned char	jump_f;				//$A0
										//$A2(≤ª”√)
	unsigned char	djump_air_f;		//$A4
	unsigned char	form_idx;			//$A6
	int				y_to_restore;		//$A8
	unsigned char	gun_type[2];			//$AA
	unsigned char	crt_gun;
	unsigned char	mgun_var;			//$AC
	unsigned char	inv_cnt;			//$AE
	unsigned char	invul_cnt;			//$B0
	unsigned char	inwater_f;			//$B2
	unsigned char	dir_of_death;		//B4
	unsigned char	on_car;				//B6
	int				free_dist;			//B8
	unsigned char	outof_bound;		//$BA
	unsigned char	anim;				//$BC
	int				x_to_restore;		//$BE
	unsigned char	dir_last;			//$C0
	unsigned char	dir;				//$C2
	short			yspeed;				//$C6,C4
	unsigned char	shock_cnt;			//$C8
	unsigned char	auto_jump_f;		//$CA
	unsigned char	wio_form_delay;		//$CC
	unsigned char	fire_cnt;			//$CE
	unsigned char	zin_f;				//$D0
	unsigned char	counter2;			//$D2
	short			carspeed;			//$D4
	unsigned char	form;				//$D6
	unsigned char	attr;				//$D8
	unsigned char	prio;				//$DA
	unsigned char	keys;				//$F1
	unsigned char	triggers;			//$F5
	unsigned short	score;				//$07E2
	unsigned char	rapid_a_cnt;
	unsigned char	rapid_b_cnt;
	unsigned char	sc_var;
};

struct P_BULLET{
	unsigned char	form;				//$0368
	unsigned char	attr;				//$0378
	unsigned char	atype;				//$0388
	short			y_acum;				//$398
	short			x_acum;				//$3A8
	int				y;					//$03B8
	int				x;					//$03C8
	short			yspeed;				//$03F8,$03D8
	short			xspeed;				//$0408,$03E8
	unsigned char	circlexy_idx;		//$0418
	unsigned char	counter;			//$0418
	unsigned char	dir;				//$0428
	unsigned char	state;				//$0438
	unsigned char	f_rapid;			//$0458
	short			s_branch_xacum;		//$0458,$468
	unsigned char	counter2;			//$0468
	unsigned char	z_circlexy_idx;		//$0468
	int				line_xlocus;		//$0478
	int				line_ylocus;		//$0488
	unsigned char	s_rapid;			//$0488
	short			line_xacum;			//$0498
	short			line_yacum;			//$04A8
	unsigned char	sb_no;				//$04A8
};

extern struct PLAYER player;
extern struct P_BULLET p_bullet[];

void player_compre();
void clear_pvars();
int check_landingok();
int check_in_air();
int solid_solidunderplat_abyss(int y);
void player_landing();
void player_death_init();
void player_addscore(unsigned short score_to_add);

#endif
