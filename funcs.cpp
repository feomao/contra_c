#include <memory.h>
#include "graphics.h"
#include "sound.h"
#include "globals.h"
#include "player.h"
#include "enemies.h"

void disp_chrs(unsigned char form, unsigned char attr, int y, int x, int* spidx, int* sps);
void update_elec();

void update_pals(void){
	if (!colors_to_update)
		return;
	if (colors_to_update<0x10)
		memcpy(Bgpal, pal_buf, colors_to_update);
	else{
		memcpy(Bgpal, pal_buf, 0x10);
		memcpy(Sppal, pal_buf+0x10, colors_to_update-0x10);
	}
	colors_to_update=0;
}

void proc_msg700(void){
	unsigned short vaddr=0;
	int i=0, j;
	unsigned char mode, c, count, dlen;

	if (!in_game){
		while(1){
			mode=msg700[i++];
			if (!mode)
				break;
			V_inc_mode =(mode==2)?1:0;
			vaddr=msg700[i++]<<8;
			vaddr+=msg700[i++];
			V_addr =vaddr;
			if (mode==3){
				count=msg700[i++];
				for(; count>0; count--){
					c=msg700[i++];
					Write_V(c);
				}
				break;
			}else{
				c=msg700[i++];
				while (c!=0xff){
					Write_V(c);
					c=msg700[i++];
				}
			}
		}
		msg700[0]=0;
		msg700_ptr=0;
	}else{
		while(1){
			mode=msg700[i++];
			if (!mode)
				break;
			V_inc_mode =(mode==2)?1:0;
			dlen=msg700[i++];
			count=msg700[i++];
			for(; count>0; count--){
				vaddr=msg700[i++]<<8;
				vaddr+=msg700[i++];
				V_addr =vaddr;
				for (j=0; j<dlen; j++){
					c=msg700[i++];
					Write_V(c);
				}
			}
		}
		msg700[0]=0;
		msg700_ptr=0;
	}
}

int chk_emy_clean(int mode){
	int i,t;
	if (!mode){
		for (i=0xf; i>=0; i--){
			if (enemy[i].state)
				return 0;
		}
		return 1;
	}else{
		for (i=0xf; i>=0; i--){
			t=enemy[i].state;
			t=enemy[i].type;
		}
		return 1;
	}
}

void read_keys(){
	unsigned char t_keys=0;
	static unsigned char dmasks[]={
		0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xf0,
		0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0
	};

	if(g_keys[SDLK_RIGHT])
		t_keys |=1;
	if(g_keys[SDLK_LEFT])
		t_keys |=2;
	if(g_keys[SDLK_DOWN])
		t_keys |=4;
	if(g_keys[SDLK_UP]){
		t_keys |=8;
	}
	if(g_keys[PLAYER_ST])
		t_keys |=0x10;
	if(g_keys[PLAYER_SEL]){
  		t_keys |=0x20;
	}
	if(g_keys[PLAYER_B])
		t_keys |=0x40;
	if(g_keys[PLAYER_A])
		t_keys |=0x80;

	player.triggers = (player.keys ^ t_keys) & t_keys;
	player.keys =t_keys;
	player.triggers &=dmasks[player.triggers &0xf];
	player.keys &=dmasks[player.keys &0xf];

	if (g_keys[PLAYER_RAPIDB]){
		if (++player.rapid_b_cnt>=RAPID_VAL){
			player.rapid_b_cnt=0;
			player.keys |=0x40;
			player.triggers |=0x40;
		}
	}else
		player.rapid_b_cnt=RAPID_VAL;
	if (g_keys[PLAYER_RAPIDA]){
		if (++player.rapid_a_cnt>=RAPID_VAL){
			player.rapid_a_cnt=0;
			player.keys |=0x80;
			player.triggers |=0x80;
		}
	}else
		player.rapid_a_cnt=RAPID_VAL;
}

void disp_objs(){
	static unsigned char status_forms[]={0xA,0xA,0xA,0xA, 2,4,6,8};
	static unsigned char status_xs[]={0x10,0x1C,0x28,0x34, 0x10,0x1C,0x28,0x34};

	int i=0, spram_idx=0;
	int sps_left=63;

	if (show_status){
		int idx=4, count=4;
		if (!player.go_flag){
			idx=0;
			count=(player.lives>4)?4:player.lives;
		}
		for (; count>0; count--, spram_idx++, idx++, sps_left--){
			Spram[spram_idx].y=0x10;
			Spram[spram_idx].tile=status_forms[idx];
			Spram[spram_idx].attr=0;
			Spram[spram_idx].x=status_xs[idx];
		}
	}
	for (i=0x19; i>=0; i--){
		if (!disp_forms[i])
			continue;
		disp_chrs(disp_forms[i], disp_attr[i], chr_ys[i], chr_xs[i], &spram_idx, &sps_left);
		if (sps_left<0)
			break;
	}
	while (sps_left>=0){
		Spram[spram_idx++].y=0xf0;
		sps_left--;
	}
}

void disp_chrs(unsigned char form, unsigned char attr, int y, int x, int* spidx, int* sps){
	int idx=0, count, cgmask;
	unsigned char tmpattr,c;
	int offset;

	unsigned char* sp_data=spdata[form];
	count=sp_data[idx++];
	if (!count)
		return;
	if(count!=0xfe){ //normal
		tmpattr =attr & 0xc8;
		if (attr & 0x4){
			attr &= 0x23;
			cgmask =0xfc;
		}else{
			attr &= 0x20;
			cgmask =0xff;
		}
		for (; count>0; count--, (*spidx)++){
			c=sp_data[idx];
			if (c==0x80){
				tmpattr &= 0xf7;
				idx++;
				c=sp_data[idx++];
				idx=sp_data[idx];
				sp_data=spdata[c];
			}
			//SPY
			c=sp_data[idx++];
			offset =EXT(c);
			if (tmpattr & 8)
				offset++;
			if (tmpattr & 0x80){
				offset =-0x10-offset;
			}
			Spram[*spidx].y =y+offset;
			//SPT
			Spram[*spidx].tile =sp_data[idx++];
			//SPATTR
			Spram[*spidx].attr =((sp_data[idx++] & cgmask) | attr)^tmpattr;
			//SPX
			c=sp_data[idx++];
			offset =EXT(c);
			if (tmpattr & 0x40){
				offset =-8-offset;
			}
			Spram[*spidx].x =x+offset;
			(*sps)--;
			if (*sps<0)
				return;
		}
	}else{
		Spram[*spidx].y =y-8;
		Spram[*spidx].tile =sp_data[idx++];
		Spram[*spidx].attr = sp_data[idx++] | attr;
		Spram[*spidx].x =x-4;
		(*sps)--;
		(*spidx)++;
	}
}

void gfx_setting(int setidx){
	int idx=gfx_settbl[setidx];
	unsigned char c;
	while(1){
		c=gfx_settbl[idx++];
		if (c==0xff)
			break;
		gfx_setop(c,1);
	}
}

void gfx_setop(int op, int clr_f){
	unsigned short offset =*((unsigned short*)&gfx_optbl[op<<1]);
	unsigned char* pdata =&gfx_optbl[offset];
	unsigned short vaddr=0;
	unsigned char c=0, count;
	int gfxoffset, gfxlen, taddr;

	msg700_ptr=0;
	if (clr_f)
		hscroll=vscroll=0;

	if (offset<0xf000){
		while(c!=0xff){
			vaddr=(pdata[1]<<8) | pdata[0];
			pdata+=2;
			V_addr =vaddr;
			V_inc_mode =0;

			while(1){
				c=*pdata++;
				if(c==0xff || c==0x7f)
					break;
				if(c & 0x80){
					count =c&0x7f;
					for (;count>0;count--){
						c=*pdata++;
						Write_V(c);
					}
				}else{
					count =c;
					c=*pdata++;
					for (;count>0;count--){
						Write_V(c);
					}
				}
			}
		}
	}else{
		offset &=0xfff;
		while(1){
			gfxoffset =gfx_movtbl[offset];
			if(gfxoffset<0)
				break;
			gfxlen=gfx_movtbl[offset+1];
			taddr=gfx_movtbl[offset+2];
			offset +=3;
			memcpy(&Vrom[taddr], &contra_gfx[gfxoffset], gfxlen);
		}
	}
	if (clr_f)
		blank_screens=5;
	vcontol_v |=SP16_BIT;
	vcontol =vcontol_v;
}

void clear_vars(){
	int i;
	memset(&player, 0,sizeof(player) );
	memset(&p_bullet[0],0,sizeof(P_BULLET)*0x10);
	memset(msg700,0,sizeof(msg700));
	msg700_ptr=0;
	pause=false;
	stage=0;
	colors_to_update=0;
	scroll_to_do=0;
	show_status=false;
	bgswitch_reached=0;
	bright_count=0;
	disable_cg23=0;
	random_e_gen_state =0;
	emydata_ptr=0;
	boss_ascroll_done=false;
	arms_clred=0;
	homing_delaytbl_idx =0;
	random_e_crt_page =0;
	random_e_cnt=0;
	memset(disp_forms,0,sizeof(disp_forms));
	memset(disp_attr,0,sizeof(disp_attr));
	memset(bgbuf680,0,sizeof(bgbuf680));
	for (i=0; i<0x10; i++)
		enemy[i].state=0;
}

void pre_title_init(){
	clear_vars();
	vcontol_v =vcontol_v |BGDISP_BIT |SPDISP_BIT;
	gfx_setting(0xb);
	write_msg(6);
	vcontol=0;
}

void pre_game_init(bool full){
	if (full){
		clear_vars();
		player.continues=3;
	}
	player.score=0;
	player.go_flag=0;
	player.crt_gun =0;
	player.lives=lives_setting;
	player.bonus_score=200;
}

void write_msg(int msg){
	int skipad=2;
	unsigned char* pdata, c;
	int era_f=msg & 0x80;

	msg &=0x7f;
	msg700[msg700_ptr++]=1;
	pdata=msgdata[msg];
	while(1){
		c =*pdata++;
		if (c==0xff)
			break;
		if (c==0xfe){
			msg700[msg700_ptr++]=0xff;
			break;
		}
		if (c==0xfd){
			msg700[msg700_ptr++]=0xff; 
			msg700[msg700_ptr++]=1;
			skipad=2;
		}else{
			msg700[msg700_ptr]=c;
			if (era_f){
				if(skipad)
					skipad--;
				else
					msg700[msg700_ptr]=0;
			}
			msg700_ptr++;
		}
	}
	msg700[msg700_ptr]=0;
}

void add_blks600(int page){
	int blks_len=0x38;
	unsigned char* pdata;
	static int s1_pageremap[]={
		0,1,2,3,0,1,2,3,0,1,2,3,4,1,2,3,5,1,2,3
	};
	static int s2_pageremap[]={
		0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,
		4,1,2,3,0,1,2,3,5,1,2,3
	};
	
	if (vstage_f)
		blks_len=0x40;

	if (stage==1){
		page=s1_pageremap[page];
	}else if(stage==3){
		page=s2_pageremap[page];
	}

	pdata =&screens_blocks_table[page*blks_len];
	memcpy(&vbuf600[ntable_off600], pdata, blks_len);
}

void add_bgs680(int page){
	int blks_len=0x38;
	unsigned char* pdata;
	if (vstage_f)
		blks_len=0x40;
	if (!zstage_f){
		pdata =&screens_bg_table[page*blks_len];
		memcpy(&bgbuf680[ntable_off600],pdata, blks_len);
	}
}

void update_pal_buf(int colors){
	static unsigned char bright_adjust[]={0,0,0x10,0x10,0x20,0x20,0x30,0x30};
	int cg=0, off=0;
	int i, clr, tmp;
	unsigned char *pclr;

	colors_to_update=colors;
	tmp = (zstage_f&0x80)?0:4;

	while (off<colors){
		pal_buf[off++]=0x0f;
		pclr =&colors_table[palcgs_spec[cg]*3];
		for (i=0; i<3; i++){
			clr =*pclr++;
			if (bright_count){
				if (off<0x10){
					if (off>=tmp){
						if (bright_count&0x80 || bright_count>8)
							clr=0xf;
						else{
							clr -=bright_adjust[bright_count-1];
							if (clr<0)
								clr =0xf;
						}
					}
				}
			}
			pal_buf[off++]=(unsigned char)clr;
		}
		cg++;
	}
	if (bright_count && !(bright_count&0x80))
		bright_count--;
}

int initial_add_screen(){
	if (!vstage_f){
		if (!zstage_f){
			add_1col();
			add_1col_attr();
			++v_addr_to_add;
			++cr_no_to_add;
			--cols_to_add;
			if (!cols_to_add)
				return 0;
			if (cr_no_to_add==32){
				cr_no_to_add=0;
				v_addr_to_add=0x2020;
				a_addr_to_add=0x3008;
				ntable_off600=0x40;
				add_blks600(1);
				add_bgs680(1);
			}
			return 1;
		}else{
			add_1col();
			add_1col_attr();
			++v_addr_to_add;
			++cr_no_to_add;
			return (--cols_to_add);
		}
	}else{
		add_1lin();
		add_1lin_attr();
		v_addr_to_add -=0x40;
		--cr_no_to_add;
		if ((--rows_to_add)==0)
			return 0;
		if (cr_no_to_add>=0x80){
			cr_no_to_add=0x1d;
			v_addr_to_add=0x2ec0;
			a_addr_to_add=0x3080;
			ntable_off600=0x40;
			add_blks600(1);
			add_bgs680(1);
		}
		return 1;
	}
}

void add_1col(){
	int col_in_block =cr_no_to_add & 3;
	int col_of_block =cr_no_to_add >>2;
	int vbuf600_idx =col_of_block | ntable_off600;
	unsigned char* ptiles;
	int i;

	msg700[msg700_ptr++]=2;
	msg700[msg700_ptr++]=0x1c;
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=v_addr_to_add>>8;
	msg700[msg700_ptr++]=v_addr_to_add & 0xff;

	for (i=0; i<7; i++){
		ptiles=&tiles_of_blocks[vbuf600[vbuf600_idx]<<4];
		msg700[msg700_ptr++] =ptiles[col_in_block];
		msg700[msg700_ptr++] =ptiles[col_in_block+4];
		msg700[msg700_ptr++] =ptiles[col_in_block+8];
		msg700[msg700_ptr++] =ptiles[col_in_block+12];
		vbuf600_idx+=8;
	}
	msg700[msg700_ptr]=0;
}

void add_1col_attr(){
	int col_of_attr =cr_no_to_add >>2;
	int vbuf600_idx =col_of_attr | ntable_off600;
	int a_addr, i;

	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=7;

	a_addr =a_addr_to_add+col_of_attr;
	for (i=0; i<7; i++){
		msg700[msg700_ptr++]=a_addr>>8;
		msg700[msg700_ptr++]=a_addr & 0xff;
		msg700[msg700_ptr++] =attribs_of_blocks[vbuf600[vbuf600_idx]];
		a_addr +=0x10;
		vbuf600_idx+=8;
	}
	msg700[msg700_ptr]=0;
}

void add_1lin(){
	int lin_in_block =(cr_no_to_add & 3)<<2;
	int lin_of_block =(cr_no_to_add >>2)<<3;
	int vbuf600_idx =lin_of_block | ntable_off600;
	unsigned char* ptiles;
	int i;

	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=0x20;
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=v_addr_to_add>>8;
	msg700[msg700_ptr++]=v_addr_to_add & 0xff;

	for (i=0; i<8; i++){
		ptiles=&tiles_of_blocks[vbuf600[vbuf600_idx++]<<4];
		*((unsigned int*)&msg700[msg700_ptr]) =*((unsigned int*)&ptiles[lin_in_block]);
		msg700_ptr+=4;
	}
	msg700[msg700_ptr]=0;
}

void add_1lin_attr(){
	int lin_of_attr =(cr_no_to_add >>2)<<3;
	int vbuf600_idx =lin_of_attr | ntable_off600;
	int a_addr, i;

	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=8;
	msg700[msg700_ptr++]=1;

	a_addr =a_addr_to_add+(lin_of_attr<<1);
	msg700[msg700_ptr++]=a_addr>>8;
	msg700[msg700_ptr++]=a_addr & 0xff;

	for (i=0; i<8; i++){
		msg700[msg700_ptr++] =attribs_of_blocks[vbuf600[vbuf600_idx++]];
	}
	msg700[msg700_ptr]=0;
}

void check_pause(){
	if (blank_screens)
		return;
	if (!pause){
		if (player.triggers & 0x10){
			pause=true;
			PLAYSOUND0(PAUSE_SND);
			pause_bgm();
			show_p_bullets();
		}
		if (player.triggers & 0x20){
			player.crt_gun ^=1;
		}
	}else{
		show_p_bullets();
		show_player();
		if (player.triggers & 0x10){
			pause=false;
			resume_bgm();
		}
	}
}

void show_p_bullets(){
	int i, alt=framecount&1, idx;
	unsigned char *pforms=&disp_forms[2], *pattrs=&disp_attr[2];
	int *pxs=&chr_xs[2], *pys=&chr_ys[2];

	for (i=7; i>=0; i--){
		idx=(i<<1) | alt;
		pforms[i] =p_bullet[idx].form;
		pattrs[i] =p_bullet[idx].attr;
		pxs[i] =p_bullet[idx].x;
		pys[i] =p_bullet[idx].y;
	}
}

void show_player(){
	static unsigned char attrs[]={0,5};
	unsigned char tform=player.form, tattr;

	if (player.outof_bound)
		tform=0;
	if (player.inv_cnt && (framecount&1))
		tform=0;
	disp_forms[0]=tform;
	tattr=attrs[player.crt_gun];
	if (player.shock_cnt){
		tattr=4;
		if (framecount&2)
			tattr++;
	}else if (player.invul_cnt){
		tattr=4;
		if (framecount&4)
			tattr++;
	}
	if (player.prio & 0x80)
		tattr |=SP_PRIORITY_BIT;
	if (player.fire_cnt && player.fire_cnt>=0xc)
		tattr |=8;
	disp_attr[0] =(player.attr&0xc8)|tattr;
}

unsigned char check_bg(int x, int y, int xovr){
	int acty, actx;
	unsigned char bgt, off;

	acty =y+vscroll;
	actx =x+hscroll;
	if (actx>511)
		actx-=512;

	if (!vstage_f){
		if (acty<0)
			acty =0;
		if (actx>255){
			bgt=0x40;
			actx-=256;
		}else
			bgt=0;
	}else{
		if (acty<0)
			acty +=480;
		if (acty>239){
			bgt=0x40;
			acty-=240;
		}else
			bgt=0;
	}

	acty= (acty>>4)<<2;
	actx >>=4;
	off= (actx>>2) | acty |bgt;
	actx &=3;
	bg_bitno =actx;
	return read_bg(y, off);
}

unsigned char read_bg(int y, unsigned char off){
	static unsigned char convtbl[]={0,1,2,0x80};

	int idx;
	unsigned char val;

	bg_toff =off;
	if (y>=0xe0){
		bg_val=0;
		bg_plat_f=0;
		bg_solid_f=0;
		bg_empty_f=1;
		return 0;
	}else{
		idx = ( bgbuf680[bg_toff]>>(6-(bg_bitno<<1)) ) &3;
		val=bg_val =convtbl[idx];
		bg_plat_f =val&1;
		bg_solid_f =val&0x80;
		bg_empty_f =(val==0);
		return val;
	}
}

int solid_or_solidunderplat(int x, int y){
	check_bg(x,y,0);
	return solidunderplat();
}

int solidunderplat(){
	unsigned char bgt;
	unsigned char toff;

	if (!bg_plat_f)
		return bg_solid_f;

	bgt =bg_toff &0x40;
	toff =bg_toff & 0x3f;
	toff +=4;
	if (vstage_f){
		if (toff>=0x40 || toff>=0x80){
			bgt^=0x40;
			toff &=0x3f;
		}
	}
	toff =bgt+(toff &0x3f);
	read_bg(0, toff);
	return bg_solid_f;
}

int solid_or_solidunderplat_cont(){
	read_bg(0,bg_toff);
	return solidunderplat();
}

int can_scroll(){
	static unsigned char xspec[]={0xa0,0xc0};
	static unsigned char ascroll[]={0x60,0x40};
	if (stage_clear)
		return 1;
	if (screens_of_stage==0xff)
		return 0;
	if (screen_page!=screens_of_stage)
		return 1;
	if (screen_off<xspec[vstage_f])
		return 1;
	boss_ascroll=ascroll[vstage_f];
	screens_of_stage=0xff;
	if (bossbgm){
		stop_bgm();
		PLAYSOUND0(ALARM);
	}
	return 0;
}

void scroll_addcolrow(){
	int tscroll, roomno, i;
	short savehsc;
	if (vstage_f){
		if (boss_ascroll){
			bright_count=0x10;
			scroll_to_do=1;
			if (--boss_ascroll==0){
				play_bgm(1);
				boss_ascroll_done++;
			}
		}
		tscroll =scroll_to_do;
		if (!tscroll)
			return;
		while (tscroll--){
			screen_off++;
			if (screen_off==0xf0){
				add_bgs680(screen_page+2);
				screen_off=0;
				emydata_ptr=0;
				screen_page++;
				if (screen_page==page_of_bgswitch){
					bgswitch_reached=1;
					bright_count=0x80;
					switch_bgvars();
				}
			}
			if (screen_off & 7){
				if ((screen_off &0xf)==7)
					add_1lin_attr();
			}else{
				add_1lin();
				v_addr_to_add -=0x40;
				--cr_no_to_add;
				if (cr_no_to_add>=0x80){
					ntable_off600 ^=0x40;
					a_addr_to_add ^=0x80;
					cr_no_to_add=0x1d;
					add_blks600(screen_page+2);
					if (v_addr_to_add==0x1fc0)
						v_addr_to_add=0x2ec0;
				}
			}
			vscroll--;
			if (vscroll<-479)
				vscroll+=480;
		}
	}else if (zstage_f){//Zนุ
		if (!(room_clear&0x80)){
			update_elec();
			return;
		}if (cols_to_add &0x80){
			cols_to_add=0;
			mag_screen++;
			screen_off++;
			if (screen_off==4){
				player.auto_jump_f=1;
				room_clear=0;
				screen_off=0;
				emydata_ptr=0;
				screen_page++;
				if (screen_page==screens_of_stage){
					zstage_f=0x80;
					switch_bgvars();
					i=(stage==1)?8:9;
					savehsc =hscroll;
					gfx_setting(i);
					hscroll=savehsc;
					play_bgm(1);
					vscroll =-0x10;
					vcontol=0;
				}
				bright_count=0xc;
				update_pal_buf(0x20);
			}
			hscroll ^=0x100;
			return;
		}else if(!cols_to_add){
			if (!mag_screen)
				return;
			cr_no_to_add=0;
			cols_to_add =32;
			if (v_addr_to_add==0x2040)
				v_addr_to_add=0x2000;
			a_addr_to_add ^=8;
			if ((screen_page!=page_of_bgswitch) || screen_off!=3){
				roomno =(screen_page<<2)+screen_off+1;
			}else{
				i=*((unsigned short*)&screens_blocks_data[8<<1]);
				screens_blocks_table=&screens_blocks_data[i];
				i=*((unsigned short*)&stage_tiles_data[8<<1]);
				tiles_of_blocks=&stage_tiles_data[i];
				i=*((unsigned short*)&stage_attrs_data[8<<1]);
				attribs_of_blocks=&stage_attrs_data[i];
				roomno =(stage==1)?0:1;
			}
			add_blks600(roomno);
		}
		add_1col();
		add_1col_attr();
		cr_no_to_add++;
		v_addr_to_add++;
		if ((--cols_to_add)==0)
			cols_to_add=0x80;
	}else{//บแนุ
		if (demon_ascroll){
			if (--demon_ascroll)
				scroll_to_do=1;
		}
		if (boss_ascroll){
			if (!(--boss_ascroll)){
				boss_ascroll_done++;
				play_bgm(1);
			}
			scroll_to_do=1;
		}
		tscroll =scroll_to_do+tank_ascroll;
		if (!tscroll)
			return;
		while (tscroll--){
			screen_off++;
			if (!screen_off){
				add_bgs680(screen_page+2);
				screen_page++;
				if (screen_page==page_of_bgswitch){
					bgswitch_reached=1;
					switch_bgvars();
				}
				emydata_ptr=0;
			}
			if (screen_off & 7){
				if ((screen_off &0xf)==3)
					add_1col_attr();
			}else{
				add_1col();
				v_addr_to_add++;
				cr_no_to_add++;
				if (cr_no_to_add==32){
					ntable_off600 ^=0x40;
					add_blks600(screen_page+2);
					cr_no_to_add=0;
					if (v_addr_to_add==0x2040)
						v_addr_to_add=0x2000;
					a_addr_to_add ^=8;
				}
			}
			hscroll++;
			if (hscroll>511)
				hscroll -=512;
		}
	}//บแนุ
}

void switch_bgvars(){
	int i;
	page_of_bgswitch=0xff;
	unsigned char *pdata =&bgswitch_tbl[stage *0xf];
	for (i=0; i<3; i++)
		tjuge[i] =*pdata++;
	for (i=0; i<4; i++)
		bgcg3_spec[i] =*pdata++;
	for (i=0; i<8; i++)
		palcgs_spec[i] =*pdata++;
	update_pal_buf(0x20);
}

void color_op(){
	static unsigned char cgidx_tbl[]={0x04,0x04,0x03,0x04,0x04,0x04,0x04,0x04,0x04};
	static unsigned char zboss1_tbl[]={0x13,0x14,0x15,0x14};
	static unsigned char zboss3_tbl[]={0x1b,0x1c,0x1d,0x1c};
	static unsigned char flash_tbl[]={0x0f,0x30,0x16,0x11};
	static unsigned char s_tbl[]={0x04,0x5c,0x04,0x5d};
	static unsigned char ending_tbl[]={0x66,0x6a,0x6b,0x6a};
	unsigned char *ptbl, idx;

	if ((framecount&7)==5 && !disable_cg23){
		if (++cg23_idx>=cgidx_tbl[stage])
			cg23_idx=0;
		palcgs_spec[3]=bgcg3_spec[cg23_idx];
		ptbl=0;
		if (zstage_f & 0x80){
			if (stage==1)
				ptbl=zboss1_tbl;
			else
				ptbl=zboss3_tbl;
		}else{
			if (!stage)
				ptbl=s_tbl;
			else if (stage==7)
				ptbl=0;
			else if (stage==8)
				ptbl=ending_tbl;
			else if (page_of_bgswitch!=0xff)
				ptbl=s_tbl;
		}
		if (ptbl)
			palcgs_spec[2]=ptbl[cg23_idx];
		update_pal_buf(0x10);
	}
	if (flash_cnt){
		flash_cnt--;
		if (!(flash_cnt&1)){
			idx=(flash_cnt>>1)&3;
			pal_buf[0x0]=pal_buf[0x4]=pal_buf[0x8]=pal_buf[0xc]=flash_tbl[idx];
			colors_to_update=0x20;
		}
	}
}

void update_elec(){
	static unsigned char elec_4t[]={
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,
		0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01,0x00,0x00,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe
	};
	static unsigned char mod_tbl[]={
		0x00,0x00,0x04,0x44,0xeb,0x32,0x20,0x00,0x00,0x00,0x10,0x30,0xeb,0x6a,0x44,0x00,
		0x00,0x00,0x08,0x0c,0xd7,0x56,0x22,0x00,0x00,0x00,0x20,0x22,0xd7,0x4c,0x04,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	};
	int modidx,i, tidx;
	if (zstage_f&0x80)
		return;
	if (room_clear){
		room_clear |=0x80;
		modidx=0x20;
	}else{
		if (framecount &3)
			return;
		modidx=(framecount &0xc)<<1;
	}
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=0x40;
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=0x1f;
	msg700[msg700_ptr++]=0xc0;
	for (i=0; i<0x40; i++){
		tidx =modidx+(i&7);
		msg700[msg700_ptr++]=elec_4t[i] | mod_tbl[tidx];
	}
	msg700[msg700_ptr]=0;
}

void chang_boss_tiles(){
	static unsigned char ctbl[]={0x1b, 0, 0x1c, 0, 0x1d, 0x1e, 0, 0x1f};
	unsigned char cno;

	if (bgswitch_reached){
		cno=ctbl[stage];
		if (cno){
			gfx_setop(cno,0);
		}
		bgswitch_reached=0;
	}
}

void p_bullet_vanish(int no){
	p_bullet[no].state=2;
	p_bullet[no].counter=6;
}

int calc_bgobj_addr(int x, int y){
	x +=hscroll;
	y +=vscroll;
	if (x>511)
		x-=512;
	if (y<0)
		y+=480;

	return (bgobj_addr =(((y&0x1f8)<<3)+(x>>3)) | 0x2000);
	
}

void show_bgobj_4x4(int x, int y, int bno){
	unsigned int *pntbl, *pval,val,v;
	int vaddr, idx, actno=bno&0x7f, i,j;
	unsigned char *ptiles, *pattrs;

	vaddr=calc_bgobj_addr(x,y);
	idx=(zstage_f & 0x80) ?8:stage;
	ptiles =bgobj_4x4_tiles[idx];

	if (!(bno & 0x80)){
		pattrs =bgobj_4x4_attrs[idx];
		val=pattrs[actno];
		pntbl =&Nametbl[vaddr & 0xfff];
		for (i=0; i<4; i++){
			pval=&pntbl[((i>>1)<<7)+((i&1)<<1)];
			v =((val)>>(i<<1))<<30;
			*pval= ((*pval) & 0x3fffffff) | v;
			pval=&pntbl[((i>>1)<<7)+((i&1)<<1)+1];
			*pval= ((*pval) & 0x3fffffff) | v;
			pval=&pntbl[((i>>1)<<7)+64+((i&1)<<1)];
			*pval= ((*pval) & 0x3fffffff) | v;
			pval=&pntbl[((i>>1)<<7)+64+((i&1)<<1)+1];
			*pval= ((*pval) & 0x3fffffff) | v;
		}
	}
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=4;
	msg700[msg700_ptr++]=4;

	ptiles =&ptiles[actno<<4];
	j=0;
	for (i=0; i<4; i++){
		msg700[msg700_ptr++]=vaddr>>8;
		msg700[msg700_ptr++]=vaddr&0xff;
		do {
			msg700[msg700_ptr++]=ptiles[j++];
		}while (j&3);
		vaddr +=0x40;
	}
	msg700[msg700_ptr]=0;
}

void show_bgobj_2xn(int x, int y, int bno){
	int vaddr, actno=bno&0x7f, i,j, rows;
	unsigned char *ptbl, c;

	vaddr=calc_bgobj_addr(x,y);
	ptbl =bgobj_2xn_tbls[stage];
	ptbl =&ptbl[(actno<<2) +actno];
	c=*ptbl++;
	rows =(c & 0x80) ? c&0x7f:2;
	msg700[msg700_ptr++]=1;
	msg700[msg700_ptr++]=2;
	msg700[msg700_ptr++]=rows;

	for (i=0; i<rows; i++){
		msg700[msg700_ptr++]=vaddr>>8;
		msg700[msg700_ptr++]=vaddr&0xff;
		for (j=0; j<2; j++){
			msg700[msg700_ptr++]=*ptbl++;
		}
		vaddr +=0x40;
	}

	msg700[msg700_ptr]=0;
}

void set_bgval_4x4(int v20, int v31){
	static unsigned char masks[]={0x3f,0xcf,0xf3,0xfc};
	int bgt, bgoff, no_2tcol;
	int i,j, crt_bgoff,crt_2tcol;
	unsigned char val;

	if (zstage_f){
		if (bgobj_addr<0x2780){
			bgt=0;
		}else{
			bgt=0x40;
			bgobj_addr-=0x2780;
		}
	}else{
		if ((bgobj_addr & 0x3f)<0x20){
			bgt=0;
		}else{
			bgt=0x40;
			bgobj_addr-=0x20;
		}
	}
	no_2tcol=(bgobj_addr>>1) &3;
	bgoff= bgt |((bgobj_addr & 0xf80)>>5) |((bgobj_addr & 0x18)>>3);
	for (i=0; i<2; i++){
		crt_2tcol=no_2tcol;
		crt_bgoff=bgoff;
		for (j=1; j>=0; j--){
			val=(j&1) ? v20&3 :v31&3;
			val <<= ((3-crt_2tcol)<<1);
			bgbuf680[bgoff] =(bgbuf680[bgoff] &masks[crt_2tcol]) | val;
			if ((++crt_2tcol & 3)==0)
				crt_bgoff++;
		}
		v20 >>=2; v31 >>=2;
		bgoff+=4;
	}
}

void clr_bgval_blks2(){
	set_bgval_4x4(0,0);
	bgobj_addr+=4;
	set_bgval_4x4(0,0);
}

void stageclear_init(int sndno){
	PLAYSOUND0(sndno);
	long_delay =0xffff;
	stage_clear=1;
}
