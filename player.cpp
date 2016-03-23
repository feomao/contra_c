#include "globals.h"
#include "player.h"

static int e_mode;

void player_compre2();
void player_proc();
void player_main();
void player_control();
void player_control_sub();
void player_control_main();
void player_jump_init();
void player_shock_proc();
void player_djump_air_proc();
void player_jump_proc();
void player_jump_sub();
void player_jump_sub2();
void player_zin_proc();
void player_form();
void player_walk_form();
void player_walk_form_sub(int tblno);
void player_death_form();
void player_inwater_form();
void player_jump_form();
void player_zstage_form();
void player_zbossstage_form();
void set_player_dir();
void check_in_air_water();
int player_xoff_solid(int off);
int check_scroll();
void player_ysa0023_refresh_y();
void player_refresh_y();
void player_y_regulate();
void calc_zin_xyspeed();
short calc_valdiv80(short val, int comp_f);

void p_bullet_init();
void p_bullet_proc();

void player_sub0();
void player_sub1();
void player_sub2();
void player_sub3();

SUB player_subs[]={
	player_sub0, player_sub1, player_sub2, player_sub3
};

void player_compre(){
	player_compre2();
	if (mag_screen==2)
		mag_screen=0;
	if (boss_ascroll | demon_ascroll)
		scroll_to_do=1;
	p_bullet_proc();
	show_p_bullets();
}

void player_compre2(){
	static unsigned char diffs[]={0,2,1,3,2};
	//$D3B4
	scroll_to_do=0;
	player_normal=0;
	diff_extra=0;

	//$D460
	player_proc();
	if (player.inv_cnt)
		player.inv_cnt--;
	else if (player.state==1)
		player_normal=1;
	if (player.invul_cnt){
		if (!(framecount&7))
			player.invul_cnt--;
	}
	if (player.fire_cnt)
		player.fire_cnt--;
	player.carspeed=0;
	diff_extra =diffs[player.gun_type[player.crt_gun]];
}

void player_proc(){
	static int e_modes[]={0,1,2,1,0,0,0,0};

	e_mode=e_modes[stage];
	if (zstage_f&0x80)
		e_mode=3;
	player_subs[player.state]();
}

void player_sub0(){
	static int xy_init[]={0x20,0x60,0x50,0x60, 0x30,0x70,0x30, 0x70};

	clear_pvars();
	chr_ys[0] =xy_init[e_mode];
	chr_xs[0] =xy_init[4+e_mode];
	player.jump_f=1;
	if (!zstage_f && !check_landingok()){
		chr_xs[0]=0x10;
		while(!check_landingok()){
			chr_xs[0]+=0x10;
			if (chr_xs[0]>=0xe0){
				chr_xs[0]=0x30;
				break;
			}
		}
	}
	player.form_idx=2;
	player.yspeed=0;
	player.inv_cnt=0x80;
	player.state++;
}

void player_sub1(){
	player_main();
	player_form();
	show_player();
	player.dir_last =player.dir;
	if (!player.outof_bound){
		if (chr_ys[0]>=0xe8)
			player_death_init();
	}
}

void player_sub2(){
	static unsigned char dforms[]={4,4,6};
	static short dxspeed[]={-1,0,0};
	int idx=(zstage_f)?(zstage_f&0x80?1:2):0;

	if (boss_ascroll | demon_ascroll){
		if (vstage_f)
			chr_ys[0]++;
		else if (chr_xs[0]>=0x1a)
			chr_xs[0]--;
	}
	player.anim=dforms[idx];
	if (player.counter){
		if (--player.counter){
			player_form();
			show_player();
		}else{
			player.gun_type[player.crt_gun]=0;
			clear_pvars();
			player.state=0;
			if (!player.lives)
				player.go_flag=1;
			else
				player.lives--;
		}
		return;
	}else{
		player.xspeed =dxspeed[idx];
		if (player.dir>=5){
			player.dir_of_death |=2;
			player.xspeed =-player.xspeed;
		}
		if (player.yspeed>=0x0200){
			if (player.outof_bound)
					player.counter=0x40;
			else{
				if (!check_in_air())
					if (bg_val!=2){
						player_y_regulate();
						player.counter=0x40;
					}
			}
		}
		player_ysa0023_refresh_y();
		player_form();
		show_player();
	}

}

void player_sub3(){
	return;
}

void player_main(){
	set_player_dir();
	check_in_air_water();
	p_bullet_init();
	player_control();
	if (boss_ascroll | demon_ascroll){
		if (vstage_f)
			chr_ys[0]++;
		else if (chr_xs[0]>=0x1a)
			chr_xs[0]--;
	}
}

void player_control(){
	if (!player.zin_f)
		player.xspeed =0;
	player_control_main();
	player_control_sub();
}

void player_control_sub(){
	static int xlimit[]={0xe6,0xe0,0xd0,0x1a,0x20,0x30};
	static int xlimit2[]={0x90,0xff,0xff,0xff,0xa0,0xd0,0xb0,0xb0};
	int idx=(zstage_f)?(zstage_f&0x80?1:2):0;

	player.xspeed +=player.carspeed;
	if (player.inwater_f & 0x80)
		return;
	if (player.xspeed==0)
		return;
	if (player.xspeed>0){
		if (!(stage_clear&0x80)){
			if(player_xoff_solid(8))
				return;
			if (chr_xs[0]>=xlimit[idx])
				return;
			if (boss_ascroll_done && chr_xs[0]>=xlimit2[stage])
				return;
		}
		check_scroll();
		player.x_acum &=0xff;
		player.x_acum +=player.xspeed;
		chr_xs[0] +=player.x_acum>>8;
	}else{
		if(player_xoff_solid(-8))
			return;
		if (!(stage_clear&0x80)){
			if (chr_xs[0]<xlimit[idx+3])
				return;
		}
		player.x_acum &=0xff;
		player.x_acum +=player.xspeed;
		chr_xs[0] +=player.x_acum>>8;
	}
}

void player_control_main(){
	int tmp;

	player.anim=3;
	if (player.auto_jump_f){
		player.auto_jump_f=0;
		player.zin_f=0;
		chr_xs[0]=player.x_to_restore;
		chr_ys[0]=player.y_to_restore;
		player_jump_init();
		return;
	}
	if (player.shock_cnt){
		player_shock_proc();
		return;
	}
	if (player.djump_air_f){
		player_djump_air_proc();
		return;
	}
	if (player.jump_f){
		player_jump_proc();
		return;
	}
	if (player.zin_f){
		player_zin_proc();
		return;
	}
	if (!player.inwater_f && (player.triggers &0x80)){
		if ((player.keys&7)==4){
			player.anim=2;
			if (screens_of_stage==0xff || !vstage_f){
				if (solid_solidunderplat_abyss(chr_ys[0]+0x10))
					return;
			}
			player.djump_air_f=0x81;
			player.free_dist =chr_ys[0]+0x14;
			if (player.free_dist>0xff)
				player.free_dist=0xff;
		}else{
			player_jump_init();
		}
		return;
	}
	if (tmp=(player.keys & 3)){
		if (!player.inwater_f || !(player.keys&4))
			player.xspeed=(tmp==1)?0x0100:0xff00;
		return;
	}
	if(player.keys &4){
		player.anim=2;
	}else if (player.keys &8){
		if (!(zstage_f&1))
			player.anim=1;
		else{
			if(room_clear){
				player.anim=5;
				if (!player.zin_f){
					mag_screen=1;
					player.zin_f=1;
					player.form_idx=0;
					player.counter=0;
					calc_zin_xyspeed();
				}
			}else{
				player.anim=1;
				player.shock_cnt=0x30;
				PLAYSOUND1(P_SHOCK);
			}
		}
	}else
		player.anim=0;
}

void player_jump_init(){
	if (player.dir>=5)
		player.jump_f=0x91;
	else
		player.jump_f=0x11;
	player.counter=0;
	player.form_idx=0;
	if (zstage_f)
		player.yspeed =(short)0xfc90;
	else
		player.yspeed =(short)0xfbf0;
}

void player_shock_proc(){
	player.anim=1;
	player.shock_cnt--;
	if (room_clear)
		player.shock_cnt=0;
}

void player_djump_air_proc(){
	int val;

	//if (chr_ys[0]>=player.free_dist && !check_in_air()){
	if (chr_ys[0]>=player.free_dist){
		if (!check_in_air()){
			player_y_regulate();
			player_landing();
			return;
		}
	}
	player_ysa0023_refresh_y();
	if (chr_ys[0]>=player.free_dist){
		val=(player.keys & 2)?0x40:(player.keys & 1)?0x20:0;
		if (val)
			player.djump_air_f =(player.djump_air_f &0x9f) | val;
	}
	if (player.djump_air_f &0x40)
		player.xspeed=(short)0xff00;
	else if (player.djump_air_f &0x20)
		player.xspeed=(short)0x0100;
}

void player_jump_proc(){
	if (player.yspeed>=0){
		if (zstage_f || player.yspeed>=0x400 ||
			(player.yspeed>=0x100 && ((chr_ys[0]+vscroll)&0xf)<8) ){
			if (!check_in_air()){
				player_y_regulate();
				player_jump_sub2();
				player_landing();
				return;
			}
		}
	}else{
		if (!(stage_clear&0x80)){
			check_bg(chr_xs[0], chr_ys[0]-0xa, 0);
			if (bg_solid_f)
				player.yspeed=0;
		}
	}
	player_jump_sub();
}

void player_jump_sub(){
	player.yspeed +=0x0023;
	if (!vstage_f || !check_scroll())
		player_refresh_y();
	player_jump_sub2();
}

void player_jump_sub2(){
	int val;
	val=(player.keys & 2)?0x40:(player.keys & 1)?0x20:0;
	if (val)
		player.jump_f =(player.jump_f &0x9f) | val;
	if (player.jump_f &0x40)
		player.xspeed=(short)0xff00;
	else if (player.jump_f &0x20)
		player.xspeed=(short)0x0100;
}

void player_zin_proc(){
	player.anim=5;
	player.y_acum &=0xff;
	player.y_acum +=player.yspeed_zin;
	chr_ys[0] +=player.y_acum>>8;
	if (mag_screen>=2){
		player.zin_f=0;
		chr_xs[0]=player.x_to_restore;
		chr_ys[0]=player.y_to_restore;
	}
}

void player_form(){
	static unsigned char oform[]={0x0f, 0x16,0x17};

	if (player.inwater_f){
		player_inwater_form();
		return;
	}
	if (player.djump_air_f){
		player.form=5;
		player.attr &=0x3f;
		if (player.dir>=5)
			player.attr |=0x40;
		return;
	}
	if (player.jump_f){
		player_jump_form();
		return;
	}
	if (zstage_f){
		if (zstage_f&0x80)
			player_zbossstage_form();
		else
			player_zstage_form();
		return;
	}
	if (player.anim<3)
		player.form =oform[player.anim];
	else if(player.anim==3){
		player_walk_form();
	}else{
		player_death_form();
	}
}

void player_walk_form(){
	static unsigned char tblno[]={0,2,0,3,0,0,3,0,2,0};
	if (tblno[player.dir])
		player_walk_form_sub(tblno[player.dir]);
	else
		if (player.fire_cnt)
			player_walk_form_sub(1);
		else
			player_walk_form_sub(0);
}

void player_walk_form_sub(int tblno){
	static unsigned char wforms[]={
		0x02,0x03,0x04,0x05,0x03,0x06,
		0x0d,0x0e,0x0f,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x10,0x11,0x12,
		0x13,0x14,0x15,0x13,0x14,0x15,
		0x51,0x52,0x53,0x51,0x52,0x53,
	};
	unsigned char *pforms;

	pforms =&wforms[tblno*6];
	player.form =pforms[player.form_idx];
	if (! ((++player.counter) & 7)){
		player.form_idx++;
		if (player.form_idx>=6)
			player.form_idx=0;
	}
	player.attr &=0x3f;
	if (player.dir>=5)
		player.attr |=0x40;
}

void player_death_form(){
	static unsigned char dforms[]={
		0x0a,0x00,0x0b,0x00,0x0a,0xc0,0x0b,0xc0,0x0c,0x00
	};
	unsigned char *pforms;

	if (! ((++player.counter2) & 7)){
		player.form_idx++;
		if (player.form_idx>4)
			player.form_idx=4;
	}
	pforms =&dforms[player.form_idx<<1];
	player.form=*pforms++;
	player.attr=*pforms;
	if (player.dir_of_death)
		player.attr ^=0x40;
}

void player_inwater_form(){
	static unsigned char wforms[]={
		0x19,0x00,0x19,0x00,0x19,0x00,0x18,0x00,0x18,0x00,0x18,0x40,0x18,0x40,0x19,0x40,
		0x19,0x40,0x19,0x40
	};
	static unsigned char wforms_f[]={
		0x1b,0x00,0x1c,0x00,0x1d,0x00,0x18,0x00,0x18,0x00,0x18,0x40,0x18,0x40,0x1d,0x40,
		0x1c,0x40,0x1b,0x40
	};
	unsigned char* pforms;

	if (!(player.inwater_f & 0x4)){//未完成入水形态动画
		if (!(player.inwater_f & 0x10)){//已完成入水初始设置
			player.form_idx=0;
			player.form=5;
			chr_ys[0] +=0x10;
			if (player.dir>=5)
				player.inwater_f |=2;
			player.wio_form_delay=0x10;
			player.inwater_f |=0x90;
		}
		player.xspeed=0;
		if (player.wio_form_delay){
			if (player.wio_form_delay<0xc){
				player.form=(player.wio_form_delay>=8)?0x73:0x18;
			}
			player.wio_form_delay--;
			if (player.inwater_f & 2)
				player.attr |=0x40;
		}else{
			player.inwater_f |=4;
			player.inwater_f &=0x7f;
		}
		return;
	}else{
		if (player.inwater_f & 8){//出水中?
			if (player.wio_form_delay){
				if (player.wio_form_delay<0x5)
					player.form=5;
				player.wio_form_delay--;
				if (player.inwater_f & 2)
					player.attr |=0x40;
			}else{
				player.inwater_f=0;
				player.form_idx=0;
				chr_ys[0] -=0x10;
			}
			return;
		}else{
			if (player.fire_cnt)
				pforms=wforms_f;
			else
				pforms=wforms;
			player.form =pforms[(player.dir<<1)];
			player.attr =(player.attr&0xf) |pforms[(player.dir<<1)+1];
			if (!(framecount&0xf))
				player.form_idx++;
			if (player.form_idx&1)
				player.attr &=0xf7;
			else
				player.attr |=8;
			if (check_bg(chr_xs[0],chr_ys[0],0)!=2){
				player.inwater_f |=0x88;
				player.wio_form_delay=0xc;
				player.form =0x1a;
				player.inwater_f &=0xfd;
				if (player.dir>=5)
					player.inwater_f |=2;
			}
		}//else of 出水中
	}//else of 未完成入水形态动画
}

void player_jump_form(){
	static unsigned char jfoms[]={8,9,8,9};

	unsigned char tmp =(player.jump_f&0x80)?0x40:0;
	unsigned char tmp2 =(player.form_idx>=2)?0xc0:0;

	player.attr =((player.attr&0x3f) | tmp2) ^tmp;
	player.form =jfoms[player.form_idx];
	if (++player.counter2>=5){
		player.counter2 =0;
		player.form_idx++;
		if (player.form_idx>=4)
			player.form_idx=0;
	}
}

void player_zstage_form(){
	switch (player.anim){
	case 0:
		player.form =0x50;
		break;
	case 1:
		player.form =0x55;
		break;
	case 2:
		player.form =0x54;
		break;
	case 3:
	case 4:
		if (player.fire_cnt)
			player_walk_form_sub(4);
		else
			player_walk_form_sub(0);
		break;
	case 5:
		if (--player.counter>=0x80){
			PLAYSOUND1(P_LANDING);
			player.counter=0xa;
			player.form_idx++;
			player.form =0x57+(player.form_idx &1);
		}
		break;
	case 6:
		if (player.counter2<0x1b){
			player.counter2++;
			player.form =0x55;
		}else
			player.form =0x56;
		player.attr=0;
	}
}

void player_zbossstage_form(){
	switch (player.anim){
	case 0:
	case 1:
	case 2:
		player.attr &=0x3f;
		player.form=0x50;
		break;
	case 3:
		player_walk_form();
		break;
	case 4:
		if (player.counter2<0x1b){
			player.counter2++;
			player.form =0x55;
		}else
			player.form =0x56;
		player.attr=0;
	}
}

void clear_pvars(){
	disp_forms[0]=0;
	player.x_acum=player.y_acum=0;
	player.xspeed=0;
	player.yspeed_zin=0;
	player.counter=0;
	player.jump_f=0;
	player.djump_air_f=0;
	player.form_idx=0;
	player.y_to_restore=0;
	player.mgun_var=0;
	player.inv_cnt=0;
	player.invul_cnt=0;
	player.inwater_f=0;
	player.on_car=0;
	player.free_dist=0;
	player.outof_bound=0;
	player.x_to_restore=0;
	player.dir_last=0;
	player.dir=0;
	player.yspeed=0;
	player.shock_cnt=0;
	player.auto_jump_f=0;
	player.wio_form_delay=0;
	player.fire_cnt=0;
	player.form=0;
	player.attr=0;
	player.prio=0;
}

int check_landingok(){
	check_in_air();
	if (bg_val & 0x80)
		return 0;
	if (solid_solidunderplat_abyss(chr_ys[0]+0x20))
		return 0;
	return 1;
}

int check_in_air(){
	int ey;

	if (zstage_f){
		ey =zstage_f&0x80 ?0xc8:0xa0;
		if (chr_ys[0]>=ey)
			return 0;
		else
			return 1;
	}else{
		check_bg(chr_xs[0],chr_ys[0]+0x10, 0);
		return bg_empty_f;
	}
}


int solid_solidunderplat_abyss(int y){
	int ty =y>>4;
	unsigned char bgt;
	unsigned char toff;

	if (solid_or_solidunderplat(chr_xs[0], y))
		return 1;

	bgt =bg_toff &0x40;
	toff =bg_toff & 0x3f;
	while(++ty<0xe){
		toff +=4;
		if (vstage_f){
			if (toff>=0x40 || toff>=0x80){
				bgt^=0x40;
				toff &=0x3f;
			}
			bg_toff =bgt+toff;
		}else{
			bg_toff =bgt+(toff &0x3f);
		}
		read_bg(0, bg_toff);
		if (!bg_empty_f)
			return 0;
	}
	return 1;
}

void set_player_dir(){
	static unsigned char tbl1[]={2,2,7,2,4,3,6,2,0,1,8,2,7,2,7,2};
	static unsigned char tbl2[]={7,2,7,2,5,3,6,2,9,1,8,2,7,2,7,2};
	static unsigned char tbl3[]={0,2,7,0,0,2,7,2,0,1,8,2,7,2,7,2};
	static unsigned char tbl4[]={0,2,7,0,0xA,3,6,2,0,1,8,2,7,2,7,2};
	unsigned char *ptbl;

	if (zstage_f&0x80){
		if (player.jump_f)
			ptbl=tbl4;
		else
			ptbl=tbl3;
	}else{
		if (player.dir_last<5)
			ptbl=tbl1;
		else
			ptbl=tbl2;
	}
	player.dir=ptbl[player.keys & 0xf];
}

void check_in_air_water(){
	if (player.on_car){
		player.djump_air_f=0;
		return;
	}
	if (player.inwater_f | player.jump_f | player.djump_air_f | player.zin_f)
		return;
	if(check_in_air()){
		if (player.dir<5)
			player.djump_air_f=0x21;
		else
			player.djump_air_f=0x41;
		player.free_dist =chr_ys[0]+0x14;
		if (player.free_dist>0xff)
			player.free_dist=0xff;
	}else if (bg_val==2){
		player.inwater_f =1;
	}else{
		player.djump_air_f=0;
	}
}

int player_xoff_solid(int off){
	int tx=chr_xs[0]+off;
	int yoff_head =(player.form==0x17)?0:0xb;
	int y_head, yoff_end;
	int second_yoff;
	unsigned char bgt;
	unsigned char toff;

	if (player.outof_bound)
		y_head=0xa;
	else{
		y_head =chr_ys[0]-yoff_head;
		if (y_head<0x10)
			y_head=0xa;
	}
	if (check_in_air() && player.jump_f)
		yoff_end =0x1b;
	else
		yoff_end =yoff_head+0xa;

	second_yoff =0x10-((y_head+vscroll)&0xf);
	if (solid_or_solidunderplat(tx, y_head))
		return 1;
	bgt =bg_toff &0x40;
	toff =bg_toff & 0x3f;
	while(second_yoff<yoff_end){
		second_yoff +=0x10;
		toff +=4;
		if (vstage_f){
			if (toff>=0x40 || toff>=0x80){
				bgt^=0x40;
				toff &=0x3f;
			}
			bg_toff =bgt+toff;
		}else{
			bg_toff =bgt+(toff &0x3f);
		}
		if (solid_or_solidunderplat_cont())
			return 1;
	}
	return 0;
}

int check_scroll(){
	int tmpy;

	if (zstage_f | boss_ascroll | demon_ascroll)
		return 0;
	if (player.state!=1)
		return 0;
	if (!vstage_f){
		if (screens_of_stage==0xff)
			return 0;
		if (chr_xs[0]<0x80)
			return 0;
		if (!can_scroll())
			return 0;
		player.x_acum &=0xff;
		player.x_acum +=player.xspeed;
		scroll_to_do +=player.x_acum>>8;
		player.xspeed=0;
		return 1;
	}else{
		if (chr_ys[0]>=0x50)
			return 0;
		if (player.yspeed>=0)
			return 0;
		if (!can_scroll())
			return 0;
		player.y_acum &=0xff;
		player.y_acum +=player.yspeed;
		tmpy =chr_ys[0]+(player.y_acum>>8);
		scroll_to_do =chr_ys[0] -tmpy;
		return 1;
	}
}

void player_ysa0023_refresh_y(){
	player.yspeed +=0x0023;
	player_refresh_y();
}

void player_refresh_y(){
	player.y_acum &=0xff;
	player.y_acum +=player.yspeed;
	chr_ys[0] +=player.y_acum>>8;
	if (!(stage_clear&0x80)){
		if (chr_ys[0]<0 || chr_ys[0]>0x100)
			player.outof_bound=1;
		else
			player.outof_bound=0;
	}
}

void player_y_regulate(){
	static rys[]={0,0xc9,0xa8};
	int idx=(zstage_f)?(zstage_f&0x80?1:2):0;
	short tmp;

	if (rys[idx])
		chr_ys[0]=rys[idx];
	else{
		tmp= (vscroll&0x0f)-0x10;
		chr_ys[0] = ((chr_ys[0]+tmp) & 0xfffffff0) -tmp+4;
	}
}

void player_landing(){
	if (player.djump_air_f | player.jump_f){
		player.form_idx=0;
		player.counter=0;
		player.free_dist=0;
		PLAYSOUND1(P_LANDING);
	}
	player.attr &=0x3f;
	if (player.dir>=5)
		player.attr |=0x40;
	player.djump_air_f =player.jump_f=0;
	player.anim=0;
	player.yspeed=0;
	player.yspeed_zin=0;
}

void calc_zin_xyspeed(){
	short xdelta,comp_f;

	player.yspeed_zin =calc_valdiv80(0x58, 0);
	player.x_to_restore=chr_xs[0];
	player.y_to_restore=chr_ys[0];
	comp_f =xdelta=chr_xs[0]-0x80;
	if (xdelta<0)
		xdelta =-xdelta;
	player.xspeed =calc_valdiv80(xdelta, comp_f);
}

short calc_valdiv80(short val, int comp_f){
	short res=val<<1;
	if (comp_f>=0)
		res =-res;
	return res;
}

void player_death_init(){
	PLAYSOUND1(P_DEATH);
	player.djump_air_f =player.jump_f=0;
	player.anim=0;
	player.yspeed=0;
	player.yspeed_zin=0;
	player.inwater_f=0;
	player.shock_cnt=0;
	player.counter=player.counter2=0;
	player.form_idx=0;
	player.yspeed=(short)0xfd80;
	player.state++;
}

void player_addscore(unsigned short score_to_add){
	unsigned int tmp=player.score+score_to_add;
	unsigned short bscore_inc;

	if (tmp<65536)
		player.score=tmp;
	else
		player.score=65535;

	if (score_to_add>=0x0100)
		bscore_inc=5000;
	else{
		if (player.score >=player.bonus_score){
			bscore_inc=300;
		}else
			bscore_inc=0;
	}
	if (bscore_inc){
		tmp =player.bonus_score + bscore_inc;
		if (tmp<65536)
			player.bonus_score=tmp;
		else
			player.bonus_score=65535;
		player.lives++;
		if (player.lives>99)
			player.lives=99;
		if (bscore_inc==300)
			PLAYSOUND0(P_1UP);
	}
	if (hi_score<player.score)
		hi_score=player.score;
}




int find_free_bno_init(int limit);
void init_bno(int no);
void clear_bvars(int no);
void init_bulletxy();
void init_bulletspd();
int mgun_auto();
void init_fb_spec();
void init_sb(int sb_no);
void init_sbspd(int sb_no);
void init_zbulletxy(int no);
void init_zbulletspd(int no);
void init_zb_counter();
void init_lb_spec(int no, int lb_no);

void p_bullet_init0();
void p_bullet_init1();
void p_bullet_init2();
void p_bullet_init3();
void p_bullet_init4();
void p_bullet_init5();
void p_bullet_init6();
void p_bullet_init7();

SUB p_bullet_inits[]={
	p_bullet_init0, p_bullet_init1, p_bullet_init2, p_bullet_init3,
	p_bullet_init4, p_bullet_init5, p_bullet_init6, p_bullet_init7,
	p_bullet_init3, p_bullet_init4
};

static int tg_type, t_rapid, t_jump_f, tg_dir;
static int tpx, tpy;
static int tb_no;

void p_bullet_init(){
	unsigned char tkey, val;

	if (player.outof_bound | player.shock_cnt)
		return;
	if (player.inwater_f && (player.dir>=3 && player.dir<7))
		return;
	tg_type =player.gun_type[player.crt_gun] & 0xf;
	if (tg_type==1 || tg_type==4)
		tkey=player.keys;
	else
		tkey=player.triggers;
	if (tkey&0x40){
		t_rapid =(player.gun_type[player.crt_gun]>>4) & 1;
		t_jump_f=player.jump_f;
		if (t_jump_f && (player.dir==4 || player.dir==5))
			tg_dir=0xb;
		else
			tg_dir=player.dir;
		tpx=chr_xs[0]; tpy=chr_ys[0];
		if (zstage_f==1)
			p_bullet_inits[tg_type+5]();
		else
			p_bullet_inits[tg_type]();
	}else{
		val=player.mgun_var & 0xf;
		if (val <7){
			val++;
			player.mgun_var =val;
		}
	}
}

void p_bullet_init0(){
	if (!find_free_bno_init(3))
		return;
	init_bulletxy();
	init_bulletspd();
}

void p_bullet_init1(){
	if (mgun_auto()){
		init_bulletxy();
		init_bulletspd();
	}
}

void p_bullet_init2(){
	if (!find_free_bno_init(3))
		return;
	init_bulletxy();
	init_bulletspd();
	init_fb_spec();
}

void p_bullet_init3(){
	int i;
	for (i=0; i<5; i++){
		if (!find_free_bno_init(9))
			return;
		init_sb(i);
	}
}

void p_bullet_init4(){
	int i=0, j;

	t_rapid=1;
	if (!(player.triggers & 0x40)){
		for (j=0; j<4; j++){
			if (p_bullet[j].atype)
				return;
		}
	}
	for (j=3; j>=0; j--,i++){
		tb_no=i;
		init_bno(i);
		init_lb_spec(i,j);
	}
}

void p_bullet_init5(){
	if (!find_free_bno_init(3))
	return;
	init_zbulletxy(tb_no);
	init_zbulletspd(tb_no);
	init_zb_counter();
}

void p_bullet_init6(){
	if (mgun_auto()){
		init_zbulletxy(tb_no);
		init_zbulletspd(tb_no);
		init_zb_counter();
	}
}

void p_bullet_init7(){
	if (!find_free_bno_init(3))
	return;
	init_zbulletxy(tb_no);
	init_zbulletspd(tb_no);
	init_zb_counter();
	p_bullet[tb_no].z_circlexy_idx=4;
	p_bullet[tb_no].line_xlocus =p_bullet[tb_no].x;
	p_bullet[tb_no].line_ylocus =p_bullet[tb_no].y-2;
	p_bullet[tb_no].f_rapid =t_rapid;
}

void clear_bvars(int no){
	p_bullet[no].atype=0;
	p_bullet[no].form=0;
	p_bullet[no].attr=0;
	p_bullet[no].state=0;
	p_bullet[no].circlexy_idx=0;
	p_bullet[no].counter=0;
	p_bullet[no].line_xlocus=0;
	p_bullet[no].line_xacum=0;
	p_bullet[no].line_ylocus=0;
	p_bullet[no].s_rapid=0;
	p_bullet[no].line_yacum=0;
	p_bullet[no].sb_no=0;
	p_bullet[no].f_rapid=0;
	p_bullet[no].counter2=0;
	p_bullet[no].z_circlexy_idx=0;
	p_bullet[no].s_branch_xacum=0;
	p_bullet[no].xspeed=p_bullet[no].yspeed=0;
	p_bullet[no].x_acum=p_bullet[no].y_acum=0;
}

int find_free_bno_init(int limit){
	int i=0;
	while (limit>=0){
		if (p_bullet[i].atype==0){
			tb_no=i;
			init_bno(i);
			return 1;
		}
		i++;
		limit--;
	}
	return 0;
}

void init_bno(int no){
	static unsigned char g_snd_idx[]={N_GUN, M_GUN, F_GUN, S_GUN, L_GUN};
	static unsigned char b_forms[]={0x1e,0x1f,0x22,0x1f,0x00};
	int snd=g_snd_idx[tg_type];

	player.fire_cnt=0xf;
	clear_bvars(no);
	p_bullet[no].atype=tg_type+1;
	PLAYSOUND2(snd);
	p_bullet[no].form =b_forms[tg_type];
	p_bullet[no].dir =tg_dir;
}

void init_bulletxy(){
	static unsigned char xytbl0[]={
		0x05,0xe5,0x0d,0xf0,0x10,0xfb,0x0d,0x06,0x10,0x09,0xf0,0x09,0xf3,0x06,0xf0,0xfb,
		0xf3,0xf0,0xfb,0xe5
	};
	static unsigned char xytbl1[]={
		0x00,0xf0,0x0f,0xf1,0x10,0x00,0x0f,0x0f,0x00,0x10,0x00,0x10,0xf1,0x0f,0xf0,0x00,
		0xf1,0xf1,0x00,0xf0,0x00,0x10,0x00,0x10
	};
	static unsigned char xytbl2[]={
		0xff,0xe8,0x0f,0xf0,0x10,0xfa,0x0f,0x08,0x10,0x0b,0xf0,0x0b,0xf1,0x08,0xf0,0xfa,
		0xf1,0xf0,0xff,0xe8
	};
	static unsigned char xytbl3[]={
		0x00,0xf0,0x0f,0xf1,0x10,0x00,0x0f,0x0f,0x00,0x10,0x00,0x10,0xf1,0x0f,0xf0,0x00,
		0xf1,0xf1,0x00,0xf0,0x00,0xf0,0x00,0xf0
	};
	static unsigned char *tbls[]={xytbl0,xytbl1,xytbl2,xytbl3};
	unsigned char *ptbl, c;
	int idx=0, tmp;

	if (zstage_f)
		idx=2;
	if (t_jump_f)
		idx ++;
	ptbl =tbls[idx];
	ptbl =&ptbl[tg_dir<<1];
	c =*ptbl++;
	tmp =EXT(c);
	p_bullet[tb_no].x =tpx+tmp;
	c =*ptbl++;
	tmp =EXT(c);
	p_bullet[tb_no].y =tpy+tmp;
}

void init_bulletspd(){
	static unsigned char spdtbl0[]={
		0x00,0x00,0xfd,0x00,0x02,0x1f,0xfd,0xe1,0x03,0x00,0x00,0x00,0x02,0x1f,0x02,0x1f,
		0x03,0x00,0x00,0x00,0xfd,0x00,0x00,0x00,0xfd,0xe1,0x02,0x1f,0xfd,0x00,0x00,0x00,
		0xfd,0xe1,0xfd,0xe1,0x00,0x00,0xfd,0x00,0x00,0x00,0xfd,0x00,0x00,0x00,0x03,0x00,
	};
	static unsigned char spdtbl1[]={
		0x00,0x00,0xfc,0x00,0x02,0xd4,0xfd,0x2c,0x04,0x00,0x00,0x00,0x02,0xd4,0x02,0xd4,
		0x04,0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0xfd,0x2c,0x02,0xd4,0xfc,0x00,0x00,0x00,
		0xfd,0x2c,0xfd,0x2c,0x00,0x00,0xfc,0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0x04,0x00,
	};
	static unsigned char spdtbl2[]={
		0x00,0x00,0xfe,0x00,0x01,0x6a,0xfe,0x96,0x02,0x00,0x00,0x00,0x01,0x6a,0x01,0x6a,
		0x02,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0xfe,0x96,0x01,0x6a,0xfe,0x00,0x00,0x00,
		0xfe,0x96,0xfe,0x96,0x00,0x00,0xfe,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x02,0x00,
	};
	static unsigned char spdtbl3[]={
		0x00,0x00,0xfe,0x80,0x01,0x0f,0xfe,0xf1,0x01,0x80,0x00,0x00,0x01,0x0f,0x01,0x0f,
		0x01,0x80,0x00,0x00,0xfe,0x80,0x00,0x00,0xfe,0xf1,0x01,0x0f,0xfe,0x80,0x00,0x00,
		0xfe,0xf1,0xfe,0xf1,0x00,0x00,0xfe,0x80,0x00,0x00,0xfe,0x80,0x00,0x00,0x01,0x80,
	};
	static unsigned char *spdtbls[]={
		spdtbl0, spdtbl1,  spdtbl1,  spdtbl1, spdtbl3, spdtbl2
	};
	unsigned char *ptbl, c1,c2;
	int idx=0;

	switch (tg_type){
	case 0:
	case 1:
		break;
	case 2:
		idx=4;
		break;
	case 4:
		idx=2;
	}
	idx +=t_rapid;
	ptbl=spdtbls[idx];
	ptbl =&ptbl[tg_dir<<2];
	c1=*ptbl++; c2=*ptbl++;
	p_bullet[tb_no].xspeed =(short)((c1<<8)+c2);
	c1=*ptbl++; c2=*ptbl++;
	p_bullet[tb_no].yspeed =(short)((c1<<8)+c2);
}

int mgun_auto(){
	unsigned char val1, val2;
	val1=++player.mgun_var;
	val2 =(val1) & 0xf;
	if ((val1<0x60 && val2<8) || (val1>=0x60 && val2<0xc))
		return 0;
	player.mgun_var &=0xf0;
	player.mgun_var +=0x10;
	if (player.mgun_var>=0x70)
		player.mgun_var=0;
	if (find_free_bno_init(7))
		return 1;
	player.mgun_var=7;
	return 0;
}

void init_fb_spec(){
	static unsigned char stbl[]={
		0x0c,0x00,0xf0,0x0e,0x0b,0xf5,0x00,0x10,0x00,0x02,0x0b,0x0b,0x00,0x10,0x00,0x08,
		0xf0,0x00,0x06,0xf5,0x0b,0x08,0xf0,0x00,0x0a,0xf5,0xf5,0x0c,0x00,0xf0,0x0c,0x00,
		0xf0,0x04,0x00,0x10
	};
	unsigned char *ptbl=&stbl[(tg_dir<<1)+tg_dir],c;
	int tmp;

	p_bullet[tb_no].circlexy_idx=*ptbl++;
	c=*ptbl++;
	tmp=EXT(c);
	p_bullet[tb_no].line_xlocus =p_bullet[tb_no].x +tmp;
	c=*ptbl++;
	tmp=EXT(c);
	p_bullet[tb_no].line_ylocus =p_bullet[tb_no].y +tmp;
	if (p_bullet[tb_no].line_ylocus<0 || p_bullet[tb_no].line_ylocus>=0x256)
		clear_bvars(tb_no);
}

void init_sb(int sb_no){
	struct P_BULLET *pbullet;

	pbullet=&p_bullet[tb_no];
	p_bullet[tb_no].sb_no =sb_no;
	if (!zstage_f || (zstage_f&0x80)){
		init_bulletxy();
		init_sbspd(sb_no);
	}else{
		init_zbulletxy(tb_no);
		init_zbulletspd(tb_no);
		pbullet->line_xlocus =pbullet->x;
		pbullet->s_rapid =t_rapid;
		init_zb_counter();
	}
}

void init_sbspd(int sb_no){
	static int dir_idx[]={0x00,0x04,0x08,0x0c,0x08,0x18,0x14,0x18,0x1c,0x00,0x00,0x10};
	static int sbno_idx[]={0,1,-1,2,-2};
	static int tbl_offs[]={0,0x50,0x10,0x60};
	static unsigned char vals[]={
		0x03,0xfd,0x0f,0xfd,0x3c,0xfd,0x84,0xfd,0xe1,0xfd,0x56,0xfe,0xdd,0xfe,0x6d,0xff,
		0x00,0x00,0x93,0x00,0x23,0x01,0xaa,0x01,0x1f,0x02,0x7c,0x02,0xc4,0x02,0xf1,0x02,
		0xfd,0x02,0xf1,0x02,0xc4,0x02,0x7c,0x02,0x1f,0x02,0xaa,0x01,0x23,0x01,0x93,0x00,
		0x00,0x00,0x6d,0xff,0xdd,0xfe,0x56,0xfe,0xe1,0xfd,0x84,0xfd,0x3c,0xfd,0x0f,0xfd,
		0x03,0xfd,0x0f,0xfd,0x3c,0xfd,0x84,0xfd,0xe1,0xfd,0x56,0xfe,0xdd,0xfe,0x6d,0xff,
		0x84,0xfc,0x92,0xfc,0xc6,0xfc,0x1a,0xfd,0x87,0xfd,0x0f,0xfe,0xad,0xfe,0x55,0xff,
		0x00,0x00,0xab,0x00,0x53,0x01,0xf1,0x01,0x79,0x02,0xe6,0x02,0x3a,0x03,0x6e,0x03,
		0x7c,0x03,0x6e,0x03,0x3a,0x03,0xe6,0x02,0x79,0x02,0xf1,0x01,0x53,0x01,0xab,0x00,
		0x00,0x00,0x55,0xff,0xad,0xfe,0x0f,0xfe,0x87,0xfd,0x1a,0xfd,0xc6,0xfc,0x92,0xfc,
		0x84,0xfc,0x92,0xfc,0xc6,0xfc,0x1a,0xfd,0x87,0xfd,0x0f,0xfe,0xad,0xfe,0x55,0xff,
	};
	unsigned char *pvals;
	int idx =((dir_idx[tg_dir]+sbno_idx[sb_no])&0x1f)<<1;

	pvals=&vals[tbl_offs[t_rapid]];
	p_bullet[tb_no].yspeed=*((short*)&pvals[idx]);
	pvals=&vals[tbl_offs[t_rapid+2]];
	p_bullet[tb_no].xspeed=*((short*)&pvals[idx]);
}

void init_zbulletxy(int no){
	int xoff, yoff;

	if (t_jump_f){
		xoff=yoff=0;
	}else{
		if (tg_dir==4 || tg_dir==5){
			xoff=-1; yoff=-0xc;
		}else{
			xoff=-1;
			yoff=-0x18;
			if (tg_dir<5)
				xoff=1;
		}
	}
	p_bullet[no].y =tpy+yoff;
	p_bullet[no].x =tpx+xoff;
	if (yoff==-0xc)
		p_bullet[no].atype |=0x80;
	if (t_jump_f && p_bullet[no].y>=0x98)
		p_bullet[no].y=0x98;
}

void init_zbulletspd(int no){
	int shift =t_rapid?5:6;
	int delta;

	p_bullet[no].yspeed = -(0x4000>>shift);
	if (tpx<0x80){
		delta =0x80-tpx;
		p_bullet[no].xspeed = (delta<<8) >>shift;
	}else{
		delta =tpx-0x80;
		p_bullet[no].xspeed = -((delta<<8) >>shift);
	}
}

void init_zb_counter(){
	if (tg_type==4)
		p_bullet[tb_no].counter=0x2a;
	else{
		if (t_rapid)
			p_bullet[tb_no].counter=0x15;
		else
			p_bullet[tb_no].counter=0x2a;
	}
}

void init_lb_spec(int no, int lb_no){
	static unsigned char dtbl[]={0xa,7,4,1,7,5,3,1};

	if (zstage_f && !(zstage_f&0x80)){
		p_bullet[no].counter=dtbl[lb_no+4];
		init_zbulletxy(no);
		init_zbulletspd(no);
	}else{
		p_bullet[no].counter=dtbl[lb_no];
		init_bulletxy();
		init_bulletspd();
	}
}




int pb_cut_by_solid();
void refresh_pb_xy();
void move_pb_scroll();
void check_pb_outofbound();
void move_pfb_scroll();
void refresh_pfb_line_locus();
void combine_pfb_xy();
void dec_pzb_counter();
void combine_pzfb_xy();
void refresh_pzsb_xy();
void set_pzlb_form();

void p_bullet_proc0_0();
void p_bullet_proc4_0();
void p_bullet_proc9_0();
void p_bullet_proc0_1();
void p_bullet_proc2_1();
void p_bullet_proc3_1();
void p_bullet_proc5_1();
void p_bullet_proc7_1();
void p_bullet_proc8_1();
void p_bullet_proc9_1();
void p_bullet_proc2();

SUB p_bullet_ptbl0[]={
	p_bullet_proc0_0, p_bullet_proc0_1, p_bullet_proc2
};

SUB p_bullet_ptbl1[]={
	p_bullet_proc0_0, p_bullet_proc0_1, p_bullet_proc2
};

SUB p_bullet_ptbl2[]={
	p_bullet_proc0_0, p_bullet_proc2_1, p_bullet_proc2
};

SUB p_bullet_ptbl3[]={
	p_bullet_proc0_0, p_bullet_proc3_1, p_bullet_proc2
};

SUB p_bullet_ptbl4[]={
	p_bullet_proc4_0, p_bullet_proc0_1, p_bullet_proc2
};

SUB p_bullet_ptbl5[]={
	p_bullet_proc0_0, p_bullet_proc5_1, p_bullet_proc2
};

SUB p_bullet_ptbl6[]={
	p_bullet_proc0_0, p_bullet_proc5_1, p_bullet_proc2
};

SUB p_bullet_ptbl7[]={
	p_bullet_proc0_0, p_bullet_proc7_1, p_bullet_proc2
};

SUB p_bullet_ptbl8[]={
	p_bullet_proc0_0, p_bullet_proc8_1, p_bullet_proc2
};

SUB p_bullet_ptbl9[]={
	p_bullet_proc9_0, p_bullet_proc9_1, p_bullet_proc2
};

SUB *p_bullet_procs[]={
	0, p_bullet_ptbl0, p_bullet_ptbl1, p_bullet_ptbl2, p_bullet_ptbl3,p_bullet_ptbl4,
	0, p_bullet_ptbl5, p_bullet_ptbl6, p_bullet_ptbl7, p_bullet_ptbl8, p_bullet_ptbl9
};

void p_bullet_proc(){
	SUB *ptbl;
	int i, type;

	for (i=0; i<0x10; i++){
		type =p_bullet[i].atype;
		if (!type)
			continue;
		tb_no =i;
		type &=0xf;
		if (zstage_f && !(zstage_f&0x80))
			ptbl=p_bullet_procs[type+6];
		else
			ptbl=p_bullet_procs[type];
		ptbl[p_bullet[i].state]();
	}
}

void p_bullet_proc0_0(){
	p_bullet[tb_no].state++;
}

void p_bullet_proc4_0(){
	static unsigned char form_attr[]={
		0x23,0x00,0x25,0x80,0x24,0x00,0x25,0x00,0x24,0x00,0x24,0x40,0x25,0x40,0x24,0x40,
		0x25,0xc0,0x23,0x00,0x23,0x80,0x23,0x80
	};
	struct P_BULLET *pbullet;
	unsigned char* pval;

	pbullet=&p_bullet[tb_no];
	if (vstage_f)
		pbullet->y +=scroll_to_do;
	else
		pbullet->x -=scroll_to_do;
	if (--pbullet->counter)
		return;
	pbullet->state++;
	pval =&form_attr[pbullet->dir<<1];
	pbullet->form =*pval++;
	pbullet->attr =*pval;
}

void p_bullet_proc9_0(){
	struct P_BULLET *pbullet;

	pbullet=&p_bullet[tb_no];
	if ((--pbullet->counter)==0){
		pbullet->state++;
		pbullet->counter=0x15;
		set_pzlb_form();
	}
}

void p_bullet_proc0_1(){
	refresh_pb_xy();
	move_pb_scroll();
	check_pb_outofbound();
}

void p_bullet_proc2_1(){
	if (pb_cut_by_solid())
		return;
	move_pfb_scroll();
	refresh_pfb_line_locus();
	combine_pfb_xy();
	check_pb_outofbound();
}

void p_bullet_proc3_1(){
	struct P_BULLET *pbullet;
	int val, val2;

	pbullet=&p_bullet[tb_no];
	val=++pbullet->counter2;
	if (val>=0x20)
		val2=2;
	else if (val>=0x10)
		val2=1;
	else
		val2=0;
	pbullet->form =0x1f +val2;
	p_bullet_proc0_1();
}

void p_bullet_proc5_1(){
	refresh_pb_xy();
	dec_pzb_counter();
}

void p_bullet_proc7_1(){
	refresh_pfb_line_locus();
	combine_pzfb_xy();
	dec_pzb_counter();
}

void p_bullet_proc8_1(){
	struct P_BULLET *pbullet;
	unsigned char c;

	pbullet=&p_bullet[tb_no];
	c=pbullet->counter;
	pbullet->form =(c>=0x1a)?0x1f:((c>=0xa)?0x20:0x21);
	refresh_pzsb_xy();
	dec_pzb_counter();
}

void p_bullet_proc9_1(){
	refresh_pb_xy();
	dec_pzb_counter();
}

void p_bullet_proc2(){
	if (room_clear){
		clear_bvars(tb_no);
		return;
	}
	p_bullet[tb_no].form =0x47;
	move_pb_scroll();
	if ((--p_bullet[tb_no].counter)==0)
		clear_bvars(tb_no);
}

void refresh_pb_xy(){
	struct P_BULLET *pbullet;
	if (pb_cut_by_solid())
		return;
	pbullet=&p_bullet[tb_no];
	pbullet->x_acum &=0xff;
	pbullet->x_acum +=pbullet->xspeed;
	pbullet->x +=(pbullet->x_acum>>8);
	pbullet->y_acum &=0xff;
	pbullet->y_acum +=pbullet->yspeed;
	pbullet->y +=(pbullet->y_acum>>8);
}

int pb_cut_by_solid(){
	if (through_solid<0x80)
		return 0;
	if (!solid_or_solidunderplat(p_bullet[tb_no].x, p_bullet[tb_no].y))
		return 0;
	p_bullet_vanish(tb_no);
	return 1;
}

void move_pb_scroll(){
	if (scroll_to_do){
		if (vstage_f)
			p_bullet[tb_no].y +=scroll_to_do;
		else
			p_bullet[tb_no].x--;
	}
}

void check_pb_outofbound(){
	struct P_BULLET *pbullet;
	pbullet=&p_bullet[tb_no];

	if (pbullet->x<0 || pbullet->x>0xff)
		clear_bvars(tb_no);
	if (pbullet->y<0 || pbullet->y>0xff)
		clear_bvars(tb_no);
}

void move_pfb_scroll(){
	if (scroll_to_do){
		if (vstage_f)
			p_bullet[tb_no].line_ylocus +=scroll_to_do;
		else
			p_bullet[tb_no].line_xlocus--;
	}
}

void refresh_pfb_line_locus(){
	struct P_BULLET *pbullet;
	pbullet=&p_bullet[tb_no];
	pbullet->line_yacum &=0xff;
	pbullet->line_yacum +=pbullet->yspeed;
	pbullet->line_ylocus +=(pbullet->line_yacum>>8);
	pbullet->line_xacum &=0xff;
	pbullet->line_xacum +=pbullet->xspeed;
	pbullet->line_xlocus +=(pbullet->line_xacum>>8);
}

void combine_pfb_xy(){
	static unsigned char xydelta[]={
		0x00,0xfa,0xf5,0xf2,0xf1,0xf2,0xf5,0xfa,0x00,0x06,0x0b,0x0e,0x0f,0x0e,0x0b,0x06,
		0x00,0xfa,0xf5,0xf2
	};
	int idx, tmp;
	struct P_BULLET *pbullet;
	unsigned char c;

	pbullet=&p_bullet[tb_no];
	idx =pbullet->circlexy_idx & 0xf;
	c=xydelta[idx+4];
	tmp =EXT(c);
	pbullet->x =pbullet->line_xlocus +tmp;
	c=xydelta[idx];
	tmp =EXT(c);
	pbullet->y =pbullet->line_ylocus +tmp;
	if (pbullet->dir<5 || pbullet->dir==0xa)
		pbullet->circlexy_idx++;
	else
		pbullet->circlexy_idx--;
}

void dec_pzb_counter(){
	if (!(--p_bullet[tb_no].counter))
		p_bullet_vanish(tb_no);
}

void combine_pzfb_xy(){
	static unsigned char ctbl0[]={
		0x00,0xff,0xfe,0xfd,0xfd,0xfd,0xfe,0xff,
		0x00,0x01,0x02,0x03,0x03,0x03,0x02,0x01,0x00,0xff,0xfe,0xfd,
	};
	static unsigned char ctbl1[]={
		0x00,0xfd,0xfb,0xf9,0xf9,0xf9,0xfb,0xfd,0x00,0x03,0x05,0x07,0x07,0x07,0x05,0x03,
		0x00,0xfd,0xfb,0xf9,
	};
	static unsigned char ctbl2[]={
		0x00,0xfc,0xf8,0xf5,0xf5,0xf5,0xf8,0xfc,0x00,0x04,0x08,0x0b,0x0b,0x0b,0x08,0x04,
		0x00,0xfc,0xf8,0xf5
	};
	static unsigned char *ctbls[]={ctbl0, ctbl1, ctbl2};
	static unsigned char compv[]={
		0x1c,0x0e, 0x0e, 0x07
	};
	struct P_BULLET *pbullet;
	int compidx, tno=2, idx, tmp;
	unsigned char c, *ptbl;

	pbullet=&p_bullet[tb_no];
	compidx=(pbullet->f_rapid)?2:0;
	c=pbullet->counter;
	if (c<2){
		pbullet->x =pbullet->line_xlocus;
		pbullet->y =pbullet->line_ylocus;
		return;
	}
	while (tno && (c<compv[compidx++])){
		tno--;
	}
	ptbl =ctbls[tno];
	idx =pbullet->z_circlexy_idx & 0xf;
	c=ptbl[idx+4];
	tmp =EXT(c);
	pbullet->x =pbullet->line_xlocus +tmp;
	c=ptbl[idx];
	tmp =EXT(c);
	pbullet->y =pbullet->line_ylocus +tmp;
	pbullet->z_circlexy_idx +=(pbullet->f_rapid+1);
}

void refresh_pzsb_xy(){
	static short bspeed[]={0,0x20,-0x20,0x40,-0x40, 0, 0x40,-0x40, 0x80, -0x80};
	struct P_BULLET *pbullet;
	short *pbs;

	pbullet=&p_bullet[tb_no];
	pbullet->line_xacum &=0xff;
	pbullet->line_xacum +=pbullet->xspeed;
	pbullet->line_xlocus +=(pbullet->line_xacum>>8);
	pbullet->y_acum &=0xff;
	pbullet->y_acum +=pbullet->yspeed;
	pbullet->y +=(pbullet->y_acum>>8);
	pbullet->x =pbullet->line_xlocus+(pbullet->s_branch_xacum>>8);
	pbs =(pbullet->s_rapid) ? &bspeed[5]:bspeed;
	pbullet->s_branch_xacum +=pbs[pbullet->sb_no];
}

void set_pzlb_form(){
	static unsigned char xjudges[]={
		0x40,0x50,0x60,0x74,0x8c,0xa0,0xb0,0xc0
	};
	static unsigned char lforms[]={
		0x92,0x84,0x83,0x82,0x23,0x82,0x83,0x84,0x92
	};
	struct P_BULLET *pbullet;
	int idx=8;

	pbullet=&p_bullet[tb_no];
	while (idx){
		if (pbullet->x>=xjudges[idx])
			break;
		idx--;
	}
	pbullet->form =lforms[idx];
	pbullet->attr =(idx<4)?0x40:0;
}



static short sc_cnt, sc_total_cnt;		//$0190, $0191
static scv;

void stageclear_main();
void sc_outof_bound();

void stageclear_proc0();
void stageclear_proc1();
void stageclear_proc2();

SUB stageclear_procs[]={stageclear_proc0, stageclear_proc1, stageclear_proc2};

void stageclear_proc(){
	player.keys=0;
	player.triggers=0;
	stageclear_procs[sclear_proc]();
}

void stageclear_proc0(){
	if (player.jump_f)
		return;
	player.sc_var=1;
	stage_clear=0x81;
	sc_total_cnt=0xf0;
	sc_cnt=0x20;
	sclear_proc++;
}

void stageclear_proc1(){
	static unsigned char delays[]={0xa0,0xa0,0xe0,0xa0,0xa0,0xa0,0xa0,0xa0};

	if (sc_cnt){
		sc_cnt--;
		return;
	}
	if (player.sc_var){
		scv=player.sc_var-1;
		stageclear_main();
		sc_outof_bound();
	}
	if (!(framecount&1))
		sc_total_cnt--;
	if (!sc_total_cnt || !player.sc_var){
		sc_cnt=delays[stage];
		sclear_proc++;
	}
}

void stageclear_proc2(){
	if (--sc_cnt)
		return;
	stage_clear=2;
	in_game=false;
	STOPSOUND(0);
	game_proc=5;
}

void stageclear_s0();
void stageclear_s1();
void stageclear_s2();
void stageclear_s4();
void stageclear_s5();
void stageclear_s6();
void stageclear_s7();

SUB stageclear_stages[]={
	stageclear_s0, stageclear_s1, stageclear_s2, stageclear_s1,
	stageclear_s4, stageclear_s5, stageclear_s6, stageclear_s7
};

void stageclear_main(){
	stageclear_stages[stage]();
}

void stageclear_s0(){
	player.prio =(chr_xs[0]>=0x98) ?0x80 :0;
	switch (scv){
	case 0:
		player.keys=1;
		if (chr_xs[0]>=0x90)
			player.sc_var++;
		break;
	case 1:
		if (player.jump_f)
			player.sc_var++;
		else{
			player.keys =0x81;
			player.triggers =0x81;
		}
		break;
	case 2:
		player.keys=1;
		break;
	}
}

void stageclear_s1(){
	int xdelta;

	switch (scv){
	case 0:
		player.keys=2;
		xdelta =chr_xs[0]-0xc;
		if (xdelta<0)
			xdelta =-xdelta;
		if (xdelta<2)
			player.sc_var++;
		break;
	case 1:
		player.state =3;
		disp_forms[0]=0x91;
		disp_attr[0]=0;
		player.sc_var++;
		break;
	case 2:
		chr_ys[0]--;
		break;
	}
}

void stageclear_s2(){
	int xdelta;

	switch (scv){
	case 0:
		player.keys=(chr_xs[0]<0x80) ?1:2;
		xdelta =chr_xs[0]-0x80;
		if (xdelta<0)
			xdelta =-xdelta;
		if (xdelta<8)
			player.sc_var++;
		break;
	case 1:
		player.triggers =0x80;
		if (player.jump_f && player.yspeed>0 && chr_ys[0]>=0xb0){
			player.outof_bound=1;
			player.sc_var=0;
			disp_forms[0]=0;
		}
		break;
	}
}

void stageclear_s4(){
	player.keys=1;
	if (chr_xs[0]>=0xb8)
		player.prio =0x80;
	else
		player.prio =0;
}

void stageclear_s5(){
	player.keys=1;
	if (chr_xs[0]>=0xd0)
		player.prio =0x80;
	else
		player.prio =0;
}

void stageclear_s6(){
	stageclear_s5();
}

void stageclear_s7(){
	if (!scv){
		sc_total_cnt =0x40;
		player.sc_var++;
	}
}

void sc_outof_bound(){
	if (chr_ys[0]<8 || chr_xs[0]>=0xf8 || chr_xs[0]<4){
		player.outof_bound=1;
		player.sc_var=0;
		disp_forms[0]=0;
	}
}
