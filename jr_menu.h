const int MENU_SELECTION_START=0;
const int MENU_SELECTION_LOAD=1;
const int MENU_SELECTION_GUIDE=2;
const int MENU_SELECTION_EXIT=3;

int finished_maps=0;

int worldmap_menu();
void show_guidelines();
bool play_stage(int stage_num);
void show_victory_screen();
void play_blob_intro(SDL_Surface *screen);
bool skip_intro();
void save_game(int saveslots[4]);
void load_game(int saveslots[4]);
int load_save_menu(bool mode);
void update_volume(int buttons);
void play_credits(SDL_Surface *buffer);

char homepath[512]; // path to ~/.jnb
char savefile[512]; // path to ~/.jnb/saves

void menu(SDL_Surface *buffer)
{
	// init paths
#ifndef WIN32
	char *home = getenv("HOME");

	if(home)
		sprintf(homepath, "%s/.jnb", home);

	mkdir(homepath, 0777);
	if(!errno) {
		getcwd(homepath, 512);
		strcat(homepath, "/.jnb");
		mkdir(homepath, 0777);
	}

	sprintf(savefile, "%s/saves", homepath);
#else
	getcwd(homepath, 512);
	strcat(homepath, "/.jnb");
	mkdir(homepath);

	sprintf(savefile, "%s/saves", homepath);
#endif


	int buttons=get_button_state();
	bool exit=false;
	reset_buttons();
	do
	{
		SDL_Surface *bg=LoadImage("menu/bg.png");
		SDL_Surface *selections[4];
		selections[0]=LoadImage("menu/start_game.png");
		selections[1]=LoadImage("menu/load_game.png");
		selections[2]=LoadImage("menu/guidelines.png");
		selections[3]=LoadImage("menu/quit.png");
		menu_music=Mix_LoadMUS("bgm/menu.ogg");
		Mix_PlayMusic(menu_music, -1);
		Mix_Chunk *selection_sound=Mix_LoadWAV("menu/sounds/select.wav");
		Mix_Chunk *start_sound=Mix_LoadWAV("menu/sounds/start.wav");
		Mix_Chunk *load_sound=Mix_LoadWAV("menu/sounds/load.wav");
		Mix_VolumeChunk(selection_sound,20);
		Mix_VolumeChunk(start_sound,20);
		Mix_VolumeChunk(load_sound,20);

		int selection=0;
		bool up_pressed=true;
		bool down_pressed=true;
		bool return_pressed=true;

		do
		{
			if(return_pressed && !(buttons & BTN_MENU)) return_pressed=false;
			buttons=get_button_state();
			update_volume(buttons);

			if(up_pressed && !(buttons & BTN_UP)) up_pressed=false;
			if(down_pressed && !(buttons & BTN_DOWN)) down_pressed=false;
			if((buttons & BTN_UP) && !up_pressed)
			{
				selection--;
				up_pressed=true;
				Mix_PlayChannel(-1,selection_sound,0);
			};
			if((buttons & BTN_DOWN) && !down_pressed)
			{
				selection++;
				down_pressed=true;
				Mix_PlayChannel(-1,selection_sound,0);
			};
			if(selection<0) selection=3;
			selection%=4;

			BlitSurface(bg,0,0,320,240,buffer,0,0);

			BlitSurface(selections[selection],0,0,selections[0]->w,selections[0]->h,buffer,115,96);

			SDL_Flip(buffer);
			limit_frames(60);

		}while(!(buttons & BTN_MENU) && !(buttons & BTN_B));

		int sound_channel=-1;
		switch(selection)
		{
			case MENU_SELECTION_START:
			{
				sound_channel=Mix_PlayChannel(-1,start_sound,0);
				break;
			};
			case MENU_SELECTION_LOAD:
			{
				sound_channel=Mix_PlayChannel(-1,load_sound,0);
				break;
			};
			case MENU_SELECTION_EXIT:
			{
				exit=true;
				break;
			};
		};

		SDL_FreeSurface(bg);
		SDL_FreeSurface(selections[0]);
		SDL_FreeSurface(selections[1]);
		SDL_FreeSurface(selections[2]);
		SDL_FreeSurface(selections[3]);

		Mix_HaltMusic();
		Mix_FreeMusic(menu_music);

		if(sound_channel!=-1)
		{
			while(Mix_Playing(sound_channel));
		};
		Mix_FreeChunk(selection_sound);
		Mix_FreeChunk(start_sound);
		Mix_FreeChunk(load_sound);

		if(selection==MENU_SELECTION_START)
		{
			reset_buttons();
			for(int i=0;i<100;++i)
			{
				boxRGBA(buffer,0,0,320,240,0,0,0,i);
				SDL_Flip(buffer);
				limit_frames(60);
			};

			if(!skip_intro())
			{
				play_blob_intro(buffer);
			};

			if(play_stage(0))
			{
				while(play_stage(worldmap_menu()));
			};
			reset_buttons();
		}
		else
		{
			reset_buttons();
			if(selection==MENU_SELECTION_LOAD)
			{
				finished_maps=load_save_menu(MODE_LOAD);
				while(play_stage(worldmap_menu()));
			}
			else
			{
				if(selection==MENU_SELECTION_GUIDE)
				{
					show_guidelines();
				};
			};
			reset_buttons();
		};
	}while(!exit);
};

void show_guidelines()
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	int buttons=get_button_state();
	SDL_Surface *pages[2];
	pages[0]=LoadImage("menu/guidelines/page1.png");
	pages[1]=LoadImage("menu/guidelines/page2.png");

	Mix_Music *bgm=Mix_LoadMUS("bgm/guidelines.ogg");
	Mix_PlayMusic(bgm,-1);

	bool right_released=true;
	bool left_released=true;
	bool menu_released=false;
	int pagenum=0;

	do
	{
		buttons=get_button_state();
		update_volume(buttons);

		BlitSurface(pages[pagenum],0,0,320,240,buffer,0,0);

		SDL_Flip(buffer);
		limit_frames(60);

		if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
		if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;
		if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;

		if((buttons & BTN_LEFT) && left_released)
		{
			pagenum--;
			left_released=false;
		};
		if((buttons & BTN_RIGHT) && right_released)
		{
			pagenum++;
			right_released=false;
		};
		pagenum%=2;
		if(pagenum<0) pagenum=0;
	}while(!((buttons & BTN_MENU) && menu_released));

	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	SDL_FreeSurface(pages[0]);
	SDL_FreeSurface(pages[1]);
};

bool play_stage(int stage_num)
{
	if(stage_num<0) return false;
	SDL_Surface *buffer=SDL_GetVideoSurface();
	int buttons=get_button_state();
	load_sounds();

	SDL_Surface *acts[4], *boss_battle_txt, *victory_txt;
	switch(stage_num)
	{
		case 0:
		{
			acts[0]=LoadImage("images/gv_act1.png");
			acts[1]=LoadImage("images/gv_act2.png");
			acts[2]=LoadImage("images/gv_act3.png");
			break;
		};
		case 1:
		{
			acts[0]=LoadImage("images/windy_mountainpath/wmp_act1.png");
			acts[1]=LoadImage("images/windy_mountainpath/wmp_act2.png");
			acts[2]=LoadImage("images/windy_mountainpath/wmp_act3.png");
			break;
		};
		case 2:
		{
			acts[0]=LoadImage("images/icy_cave/ic_act1.png");
			acts[1]=LoadImage("images/icy_cave/ic_act2.png");
			acts[2]=LoadImage("images/icy_cave/ic_run.png");
			break;
		};
		case 3:
		{
			acts[0]=LoadImage("images/beating_jungle/bj_act1.png");
			acts[1]=LoadImage("images/beating_jungle/bj_act2.png");
			acts[2]=LoadImage("images/beating_jungle/bj_act3.png");
			acts[3]=LoadImage("images/beating_jungle/bj_act4.png");
			SDL_SetColorKey(acts[3],SDL_SRCCOLORKEY,SDL_MapRGB(acts[3]->format,255,255,255));
			break;
		};
		case 4:
		{
			acts[0]=LoadImage("images/burning_desert/bd_act1.png");
			acts[1]=LoadImage("images/burning_desert/bd_act2.png");
			acts[2]=LoadImage("images/burning_desert/bd_act3.png");
			break;
		};
		case 5:
		{
			acts[0]=LoadImage("images/foggy_castle/fc_outside.png");
			acts[1]=LoadImage("images/foggy_castle/fc_entrance_hall.png");
			acts[2]=LoadImage("images/foggy_castle/fc_yard.png");
			break;
		};
	};
	boss_battle_txt=LoadImage("images/boss_battle.png");
	victory_txt=LoadImage("images/victory.png");
	SDL_SetColorKey(acts[0],SDL_SRCCOLORKEY,SDL_MapRGB(acts[0]->format,255,255,255));
	SDL_SetColorKey(acts[1],SDL_SRCCOLORKEY,SDL_MapRGB(acts[1]->format,255,255,255));
	SDL_SetColorKey(acts[2],SDL_SRCCOLORKEY,SDL_MapRGB(acts[2]->format,255,255,255));
	SDL_SetColorKey(boss_battle_txt,SDL_SRCCOLORKEY,SDL_MapRGB(boss_battle_txt->format,255,255,255));
	SDL_SetColorKey(victory_txt,SDL_SRCCOLORKEY,SDL_MapRGB(victory_txt->format,255,255,255));

	jr_environment environment;
	switch(stage_num)
	{
		case 0:
		{
			environment.load_from_file("green_valley.env");
			stage_music=Mix_LoadMUS("bgm/green_valley.ogg");
			break;
		};
		case 1:
		{
			environment.load_from_file("windy_mountainpath.env");
			stage_music=Mix_LoadMUS("bgm/windy_mountainpath.ogg");
			break;
		};
		case 2:
		{
			environment.load_from_file("icy_cave.env");
			stage_music=Mix_LoadMUS("bgm/icy_cave.ogg");
			break;
		};
		case 3:
		{
			environment.load_from_file("beating_jungle.env");
			stage_music=Mix_LoadMUS("bgm/beating_jungle.ogg");
			break;
		};
		case 4:
		{
			environment.load_from_file("burning_desert.env");
			stage_music=Mix_LoadMUS("bgm/burning_desert.ogg");
			break;
		};
		case 5:
		{
			environment.load_from_file("foggy_castle.env");
			stage_music=Mix_LoadMUS("bgm/foggy_castle.ogg");
			break;
		};
	};
	jr_map map(&environment);

	switch(stage_num)
	{
		case 0:
		{
			map.load_from_file("green_valley_1.map");
			map.set_fg_image("images/gv_fg.png",255,255,255);
			map.set_bg_image_scrollspeed_x(1);
			map.set_bg_image_scrollspeed_y(1);
			map.set_bg_cloud_scrollspeed_x(1);
			map.set_fg_image_scrollspeed_x(2);
			map.set_cloud_move_freq(4);
			break;
		};
		case 1:
		{
			map.load_from_file("windy_mountainpath_1.map");
			map.set_fg_image("images/windy_mountainpath/fg.png",255,255,255);
			map.set_weather_effect("images/windy_mountainpath/weather_falling_leaf.png",4,16,1,6);
			map.set_bg_image_scrollspeed_x(1);
			map.set_bg_image_scrollspeed_y(1);
			map.set_fg_image_scrollspeed_x(2);
			break;
		};
		case 2:
		{
			map.load_from_file("icy_cave_1.map");
			map.set_bg_image_scrollspeed_x(0);
			map.set_bg_image_scrollspeed_y(0);
			map.set_fg_image("images/icy_cave/fg.png",0,252,53);
			map.set_fg_image_alpha(128);
			map.set_fg_image_scrollspeed_x(2);
			break;
		};
		case 3:
		{
			map.load_from_file("beating_jungle_1.map");
			map.set_bg_image_scrollspeed_x(0);
			map.set_bg_image_scrollspeed_y(0);
			map.set_fg_image("images/beating_jungle/fg.png",255,255,255);
			map.set_fg_image_scrollspeed_x(2);
			break;
		};
		case 4:
		{
			map.load_from_file("burning_desert_1.map");
			map.set_bg_image_scrollspeed_x(1);
			map.set_bg_image_scrollspeed_y(1);
			map.set_fg_image("images/burning_desert/fg.png",255,255,255);
			map.set_fg_image_scrollspeed_x(2);
			break;
		};
		case 5:
		{
			map.load_from_file("foggy_castle_path.map");
			map.set_bg_image_scrollspeed_x(0);
			map.set_bg_image_scrollspeed_y(0);
			map.set_bg_cloud_scrollspeed_x(1);
			map.set_fg_image_scrollspeed_x(2);
			map.set_cloud_move_freq(0);
			map.set_weather_effect("images/foggy_castle/rain_drop.png",1,16,1,200);
			map.set_fog("images/foggy_castle/fog.png");
			break;
		};
	};

 	map.set_tile_scrollspeed_x(1);
 	map.set_tile_scrollspeed_y(1);
	jr_player blob("images/player_charset.png","images/player_charset_left.png","images/player_charset_earth.png","images/player_charset_earth_left.png","images/player_charset_wind.png","images/player_charset_wind_left.png","images/player_charset_fire.png","images/player_charset_fire_left.png","images/player_charset_ice.png","images/player_charset_ice_left.png",6,50,"images/lifebar.png","images/oneups.png",&map,&environment);
	Mix_PlayMusic(stage_music,-1);
	int map_state;
	int map_num=0;
	bool boss_changes_done=false;

	for(int i=255;i>0;i-=2)
	{
		map.jr_map::draw(buffer,0,0);
		blob.draw(buffer);
		map.jr_map::draw_fg_layer(buffer,0,0);
		blob.draw_panel(buffer);
		boxRGBA(buffer,0,0,320,240,0,0,0,i);
		BlitSurface(acts[0],((i/63)%2)*acts[0]->w/2,0,acts[0]->w/2,acts[0]->h,buffer,160-acts[0]->w/4,100);
		SDL_Flip(buffer);
		limit_frames(60);
	};

	do
	{
		int txt_anim_counter=0;
		bool boss_dlg_shown=false;
		do
		{
			if(map_num==2 && stage_num==5) blob.set_map_end(0,280);
			if(!boss_changes_done && map_num==3 && !map.enemys_remaining())
			{
				switch(stage_num)
				{
					case 0:
					{
						environment.set_bg_image("images/spider_bg_end.png");
						break;
					};
				};
				boss_changes_done=true;
			};
			if(!boss_dlg_shown && ((map_num==3 && stage_num!=3) || (map_num==4)) && map.boss_dead())
			{
				boss_dlg_shown=true;
				switch(stage_num)
				{
					case 0:
					{
						show_dialog(&blob,&map,"spidey_end.dlg");
						break;
					};
					case 1:
					{
						show_dialog(&blob,&map,"wingly_end.dlg");
						break;
					};
					case 2:
					{
						show_dialog(&blob,&map,"yeti_end.dlg");
						break;
					};
					case 3:
					{
						show_dialog(&blob,&map,"lavly_end.dlg");
						break;
					};
					case 4:
					{
						show_dialog(&blob,&map,"scorpy_end.dlg");
						break;
					};
					case 5:
					{
						show_dialog(&blob,&map,"bk_end.dlg");
						break;
					};
				};
			};

			buttons=get_button_state();
			update_volume(buttons);
			map_state=blob.update(buttons,buffer);
			map.jr_map::draw(buffer,0,0);
			blob.draw(buffer);
			map.jr_map::draw_fg_layer(buffer,0,0);
			blob.draw_panel(buffer);
			/*if(txt_anim_counter<512)
			{
				if(map_num<3 || (stage_num==3 && map_num<4))
				{
					BlitSurface(acts[map_num],0,0,acts[map_num]->w/2,acts[map_num]->h,buffer,160-acts[map_num]->w/4,100);
				}
				else
				{
					BlitSurface(boss_battle_txt,0,0,boss_battle_txt->w/2,boss_battle_txt->h,buffer,160-boss_battle_txt->w/4,100);
				};
				txt_anim_counter++;
			};*/
			if(((map_num==3 && stage_num!=3) || (map_num==4)) && !map.enemys_remaining())
			{
				//BlitSurface(victory_txt,((txt_anim_counter-512)/64)*victory_txt->w/2,0,victory_txt->w/2,victory_txt->h,buffer,160-victory_txt->w/4,100);
				txt_anim_counter++;
				if(txt_anim_counter>640)
				{
					txt_anim_counter=512;
					map_state=UPDATE_FINISHED;
				};
			};

			static bool lighning_done=false;
			if(stage_num==5 && (rand()%180==0 || lighning_done))
			{
				lightning(buffer);
				if(lighning_done) Mix_PlayChannel(-1,thunder_sound,0);
				lighning_done=!lighning_done;
			};

			SDL_Flip(buffer);
			limit_frames(60);
		}while(map_state==UPDATE_CONTINUE);

		if(map_state==UPDATE_FINISHED)
		{

			for(int i=0;i<100;++i)
			{
				boxRGBA(buffer,0,0,320,240,0,0,0,i);
				SDL_Flip(buffer);
				limit_frames(60);
			};


			if((map_num==3 && stage_num!=3) || (map_num==4))
			{
				map_state=UPDATE_GAME_OVER;
			};

			switch(stage_num)
			{
				case 0:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("green_valley_2.map");
							break;
						};
						case 1:
						{
							map.load_from_file("green_valley_3.map");
							break;
						};
						case 2:
						{
							map.load_from_file("green_valley_boss.map");
							map.set_fg_image("images/gv_boss_fg.png",255,255,255);
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/boss.ogg");
							Mix_PlayMusic(stage_music,-1);
							break;
						};
					};
					map.set_bg_image_scrollspeed_x(1);
					map.set_bg_image_scrollspeed_y(1);
					map.set_bg_cloud_scrollspeed_x(1);
					map.set_cloud_move_freq(4);
					break;
				};
				case 1:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("windy_mountainpath_passage.map");
							map.unset_weather_effect();
							environment.set_bg_image("images/windy_mountainpath/passage_bg.png");
							blob.map_changed();
							blob.set_passage_map(true);
							int tmp_state, buttons;
							do
							{
								buttons=get_button_state();
								update_volume(buttons);
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);


								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_CONTINUE_PREVIOUS:
								{
									map.load_from_file("windy_mountainpath_1.map");
									map.set_weather_effect("images/windy_mountainpath/weather_falling_leaf.png",4,16,1,6);
									environment.set_bg_image("images/windy_mountainpath/bg.png");
									blob.reset();
									continue;
								};
								case UPDATE_FINISHED:
								{
									map.load_from_file("windy_mountainpath_2.map");
									map.set_weather_effect("images/windy_mountainpath/weather_falling_leaf.png",4,16,1,6);
									environment.set_bg_image("images/windy_mountainpath/bg.png");
									break;
								};
							};
							break;
						};
						case 1:
						{
							map.load_from_file("windy_mountainpath_passage2.map");
							map.unset_weather_effect();
							environment.set_bg_image("images/windy_mountainpath/passage_bg.png");
							blob.map_changed();
							blob.set_passage_map(true);
							int tmp_state, buttons;
							do
							{
								buttons=get_button_state();
								update_volume(buttons);
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);

								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_CONTINUE_PREVIOUS:
								{
									map.load_from_file("windy_mountainpath_2.map");
									map.set_weather_effect("images/windy_mountainpath/weather_falling_leaf.png",4,16,1,6);
									environment.set_bg_image("images/windy_mountainpath/bg.png");
									blob.reset();
									continue;
								};
								case UPDATE_FINISHED:
								{
									map.load_from_file("windy_mountainpath_3.map");
									map.set_weather_effect("images/windy_mountainpath/weather_snow_small.png",1,16,1,36);
									environment.set_bg_image("images/windy_mountainpath/final_bg.png");
									map.set_fg_image("images/windy_mountainpath/fg_snow.png",255,255,255);
									break;
								};
							};
							break;
						};
						case 2:
						{
							map.load_from_file("windy_mountainpath_passage3.map");
							map.unset_weather_effect();
							environment.set_bg_image("images/windy_mountainpath/passage_bg.png");
							blob.map_changed();
							blob.set_passage_map(true);
							int tmp_state, buttons;
							do
							{
								buttons=get_button_state();
								update_volume(buttons);
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);

								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_CONTINUE_PREVIOUS:
								{
									map.load_from_file("windy_mountainpath_3.map");
									map.set_weather_effect("images/windy_mountainpath/weather_snow_small.png",1,16,1,36);
									environment.set_bg_image("images/windy_mountainpath/final_bg.png");
									blob.reset();
									continue;
								};
								case UPDATE_FINISHED:
								{
									map.load_from_file("windy_mountainpath_boss.map");
									map.set_weather_effect("images/windy_mountainpath/weather_snow_small.png",1,16,1,36);
									Mix_HaltMusic();
									Mix_FreeMusic(stage_music);
									stage_music=Mix_LoadMUS("bgm/boss.ogg");
									Mix_PlayMusic(stage_music,-1);
									break;
								};
							};
							break;
						};
					};
					map.set_bg_image_scrollspeed_x(1);
					map.set_bg_image_scrollspeed_y(1);
					break;
				};
				case 2:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("icy_cave_2.map");
							break;
						};
						case 1:
						{
							map.load_from_file("icy_cave_3.map");
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/pursuit.ogg");
							Mix_PlayMusic(stage_music,-1);
							break;
						};
						case 2:
						{
							map.load_from_file("icy_cave_boss.map");
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/boss.ogg");
							Mix_PlayMusic(stage_music,-1);
							map.set_weather_effect("images/icy_cave/weather_snow_big.png",1,16,1,48);
							break;
						};
					};
					map.set_bg_image_scrollspeed_x(0);
					map.set_bg_image_scrollspeed_y(0);
					map.set_fg_image_scrollspeed_x(2);
					break;
				};
				case 3:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("beating_jungle_2.map");
							break;
						};
						case 1:
						{
							map.load_from_file("beating_jungle_3.map");
							break;
						};
						case 2:
						{
							map.load_from_file("beating_jungle_4.map");
							break;
						};
						case 3:
						{
							map.load_from_file("beating_jungle_boss.map");
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/boss.ogg");
							Mix_PlayMusic(stage_music,-1);
							break;
						};
					};
					map.set_bg_image_scrollspeed_x(0);
					map.set_bg_image_scrollspeed_y(0);
					if(map_num==3) map.set_bg_image_scrollspeed_x(1);
					map.set_fg_image_scrollspeed_x(2);
					break;
				};
				case 4:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("burning_desert_2.map");
							break;
						};
						case 1:
						{
							map.load_from_file("burning_desert_3.map");
							break;
						};
						case 2:
						{
							map.load_from_file("burning_desert_boss.map");
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/boss.ogg");
							Mix_PlayMusic(stage_music,-1);
							break;
						};
					};
					map.set_bg_image_scrollspeed_x(1);
					map.set_bg_image_scrollspeed_y(1);
					map.set_fg_image_scrollspeed_x(2);
					break;
				};
				case 5:
				{
					switch(map_num)
					{
						case 0:
						{
							map.load_from_file("foggy_castle_draki.map");
							Mix_HaltMusic();
							Mix_FreeMusic(stage_music);
							stage_music=Mix_LoadMUS("bgm/boss.ogg");
							Mix_PlayMusic(stage_music,-1);
							environment.set_bg_image("images/foggy_castle/draki_bg.png");
							map.set_bg_image_scrollspeed_x(1);
							environment.unset_bg_cloud_image();
							blob.map_changed();
							show_cutscene("draki_1.scn");
							blob.boss_battle_start("images/foggy_castle/draki_icon.png");
							show_dialog(&blob,&map,"draki.dlg");
							int tmp_state, buttons;
							bool boss_dlg_shown=false;
							do
							{
								if(!boss_dlg_shown && map.boss_dead())
								{
									boss_dlg_shown=true;
									show_dialog(&blob,&map,"draki_end.dlg");
								};
								buttons=get_button_state();
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);

								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							environment.set_bg_image("images/foggy_castle/scrolling_bg.png");
							environment.set_bg_cloud_image("images/foggy_castle/fixed_bg.png");
							map.set_bg_image_scrollspeed_x(0);
							map.set_bg_image_scrollspeed_y(0);
							map.set_bg_cloud_scrollspeed_x(1);
							map.set_fg_image_scrollspeed_x(2);
							map.set_cloud_move_freq(0);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_FINISHED:
								{
									blob.boss_battle_end();
									show_cutscene("draki_2.scn");
									map.load_from_file("foggy_castle_hall.map");
									map.unset_weather_effect();
									Mix_HaltMusic();
									Mix_FreeMusic(stage_music);
									stage_music=Mix_LoadMUS("bgm/foggy_castle.ogg");
									Mix_PlayMusic(stage_music,-1);
									break;
								};
							};
							break;
						};
						case 1:
						{
							map.load_from_file("foggy_castle_room_1.map");
							blob.map_changed();
							blob.set_passage_map(true);
							blob.set_map_end(20,50);
							int tmp_state, buttons;
							do
							{
								buttons=get_button_state();
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);

								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_CONTINUE_PREVIOUS:
								{
									map.load_from_file("foggy_castle_hall.map");
									blob.reset();
									continue;
								};
								case UPDATE_FINISHED:
								{
									map.load_from_file("foggy_castle_room_2.map");
									blob.map_changed();
									blob.set_map_end(20,40);
									blob.set_passage_map(true);
									do
									{
										buttons=get_button_state();
										tmp_state=blob.update(buttons,buffer);
										map.jr_map::draw(buffer,0,0);
										blob.draw(buffer);
										map.jr_map::draw_fg_layer(buffer,0,0);
										blob.draw_panel(buffer);

										SDL_Flip(buffer);
										limit_frames(60);
									}while(tmp_state==UPDATE_CONTINUE);

									switch(tmp_state)
									{
										case UPDATE_GAME_OVER:
										{
											map_state=UPDATE_GAME_OVER;
											break;
										};
										case UPDATE_CONTINUE_PREVIOUS:
										{
											map.load_from_file("foggy_castle_hall.map");
											blob.reset();
											continue;
										};
										case UPDATE_FINISHED:
										{
											map.load_from_file("foggy_castle_yard.map");
											map.set_weather_effect("images/foggy_castle/rain_drop.png",1,16,1,200);
										};
									};
									break;
								};
							};
							break;
						};
						case 2:
						{
							map.load_from_file("foggy_castle_tower.map");
							map.unset_weather_effect();
							blob.map_changed();
							blob.set_map_end(240,120);
							blob.set_passage_map(true);
							int tmp_state, buttons;
							do
							{
								buttons=get_button_state();
								tmp_state=blob.update(buttons,buffer);
								map.jr_map::draw(buffer,0,0);
								blob.draw(buffer);
								map.jr_map::draw_fg_layer(buffer,0,0);
								blob.draw_panel(buffer);

								SDL_Flip(buffer);
								limit_frames(60);
							}while(tmp_state==UPDATE_CONTINUE);

							switch(tmp_state)
							{
								case UPDATE_GAME_OVER:
								{
									map_state=UPDATE_GAME_OVER;
									break;
								};
								case UPDATE_CONTINUE_PREVIOUS:
								{
									map.load_from_file("foggy_castle_yard.map");
									blob.reset();
									blob.set_map_end(0,280);
									continue;
								};
								case UPDATE_FINISHED:
								{
									map.load_from_file("foggy_castle_room_3.map");
									blob.map_changed();
									blob.set_map_end(20,200);
									blob.set_passage_map(true);
									do
									{
										buttons=get_button_state();
										tmp_state=blob.update(buttons,buffer);
										map.jr_map::draw(buffer,0,0);
										blob.draw(buffer);
										map.jr_map::draw_fg_layer(buffer,0,0);
										blob.draw_panel(buffer);

										SDL_Flip(buffer);
										limit_frames(60);
									}while(tmp_state==UPDATE_CONTINUE);

									map.set_weather_effect("images/foggy_castle/rain_drop.png",1,16,1,200);

									switch(tmp_state)
									{
										case UPDATE_GAME_OVER:
										{
											map_state=UPDATE_GAME_OVER;
											break;
										};
										case UPDATE_CONTINUE_PREVIOUS:
										{
											map.load_from_file("foggy_castle_yard.map");
											blob.reset();
											blob.set_map_end(0,280);
											continue;
										};
										case UPDATE_FINISHED:
										{
											environment.load_from_file("beast_king.env");
											map.load_from_file("foggy_castle_bk.map");
											map.set_bg_image_scrollspeed_x(1);
											map.set_bg_image_scrollspeed_y(1);
											Mix_HaltMusic();
											Mix_FreeMusic(stage_music);
											stage_music=Mix_LoadMUS("bgm/final_boss.ogg");
											Mix_PlayMusic(stage_music,-1);
											environment.set_bg_image("images/foggy_castle/bk_bg.png");
											blob.map_changed();
											blob.boss_battle_start("images/foggy_castle/bk_icon.png");
										};
									};
									break;
								};
							};
							break;
						};
					};
					if(map_num!=2)
					{
						map.set_bg_image_scrollspeed_x(0);
						map.set_bg_image_scrollspeed_y(0);
					};
					map.set_fg_image_scrollspeed_x(2);
					break;
				};
			};
			map.set_tile_scrollspeed_x(1);
 			map.set_tile_scrollspeed_y(1);

			if(map_num==2 && stage_num==0)
			{
				show_cutscene("green_valley.scn");
			};
			if(map_num==2 && stage_num==1)
			{
				show_cutscene("windy_mountainpath.scn");
			};
			if(map_num==2 && stage_num==4)
			{
				show_cutscene("burning_desert_1.scn");
			};
			if(map_num==3 && stage_num==3)
			{
				show_cutscene("beating_jungle.scn");
			};
			if(map_num==1 && stage_num==2)
			{
				show_cutscene("icy_cave_1.scn");
			};
			if(map_num==2 && stage_num==2)
			{
				show_cutscene("icy_cave_2.scn");
			};
			if(map_num==2 && stage_num==5)
			{
				show_cutscene("bk_1.scn");
			};
			if((map_num==2 && stage_num!=3) || map_num==3)
			{
				switch(stage_num)
				{
					case 0:
					{
						blob.boss_battle_start("images/spider_icon.png");
						environment.set_bg_image("images/spider_bg.png");
						environment.unset_bg_cloud_image();
						break;
					};
					case 1:
					{
						blob.boss_battle_start("images/windy_mountainpath/wingly_icon.png");
						environment.set_bg_image("images/windy_mountainpath/wingly_bg.png");
						break;
					};
					case 2:
					{
						blob.boss_battle_start("images/icy_cave/yeti_icon.png");
						environment.set_bg_image("images/icy_cave/yeti_bg.png");
						map.set_fg_image("images/icy_cave/boss_fg.png",45,255,3);
						map.set_fg_image_scrollspeed_x(0);
						break;
					};
					case 3:
					{
						blob.boss_battle_start("images/beating_jungle/lavly_icon.png");
						environment.set_bg_image("images/beating_jungle/lavly_bg.png");
						break;
					};
					case 4:
					{
						blob.boss_battle_start("images/burning_desert/scorpy_icon.png");
						environment.set_bg_image("images/burning_desert/scorpy_bg.png");
						break;
					};
				};
			};
			blob.map_changed();
			map_num++;

			if((map_num<4 || (stage_num==3 && map_num<5)) && map_state!=UPDATE_GAME_OVER)
			{
				for(int i=255;i>0;i-=2)
				{
					map.jr_map::draw(buffer,0,0);
					blob.draw(buffer);
					map.jr_map::draw_fg_layer(buffer,0,0);
					blob.draw_panel(buffer);
					boxRGBA(buffer,0,0,320,240,0,0,0,i);
					if(map_num<3 || (stage_num==3 && map_num<4))
					{
						BlitSurface(acts[map_num],((i/63)%2)*acts[0]->w/2,0,acts[map_num]->w/2,acts[map_num]->h,buffer,160-acts[map_num]->w/4,100);
					}
					else
					{
						BlitSurface(boss_battle_txt,((i/63)%2)*boss_battle_txt->w/2,0,boss_battle_txt->w/2,boss_battle_txt->h,buffer,160-boss_battle_txt->w/4,100);
					};

					SDL_Flip(buffer);
					limit_frames(60);
				};
				if(map_num==3 && stage_num==0) show_dialog(&blob,&map,"spidey.dlg");
				if(map_num==3 && stage_num==1) show_dialog(&blob,&map,"wingly.dlg");
				if(map_num==3 && stage_num==2) show_dialog(&blob,&map,"yeti.dlg");
				if(map_num==4 && stage_num==3) show_dialog(&blob,&map,"lavly.dlg");
				if(map_num==3 && stage_num==4) show_dialog(&blob,&map,"scorpy.dlg");
				if(map_num==3 && stage_num==5) show_dialog(&blob,&map,"bk.dlg");

				if(map_num==2 && stage_num==5) blob.set_map_end(0,280);
			};
		};
	}while(map_state!=UPDATE_GAME_OVER);

	map.unload();
	map.unset_weather_effect();
	map.unset_fg_image();
	environment.unload();
	unload_sounds();
	Mix_HaltMusic();
	Mix_FreeMusic(stage_music);

	SDL_FreeSurface(acts[0]);
	SDL_FreeSurface(acts[1]);
	SDL_FreeSurface(acts[2]);
	if(stage_num==3) SDL_FreeSurface(acts[3]);
	SDL_FreeSurface(boss_battle_txt);
	SDL_FreeSurface(victory_txt);

	if(map_num==4 && stage_num==5)
	{
		show_cutscene("bk_2.scn");
		play_credits(buffer);
		return true;
	};

	if((map_num==4 && stage_num!=3) || map_num==5)
	{
		if(finished_maps==stage_num) finished_maps++;
		show_victory_screen();
		return true;
	}
	else
	{
		return false;
	};
};

int worldmap_menu()
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *bg=LoadImage("menu/worldmap/bg.png");
	SDL_Surface *dots=LoadImage("menu/worldmap/dots.png");
	SDL_Surface *save_txt=LoadImage("menu/worldmap/save_txt.png");
	SDL_Surface *quit_txt=LoadImage("menu/worldmap/quit_txt.png");
	SDL_Surface *blob=LoadImage("menu/worldmap/blob.png");
	SDL_Surface *clouds=LoadImage("menu/worldmap/clouds.png");
	SDL_SetColorKey(dots,SDL_SRCCOLORKEY,SDL_MapRGB(dots->format,255,255,255));
	SDL_SetColorKey(save_txt,SDL_SRCCOLORKEY,SDL_MapRGB(save_txt->format,255,255,255));
	SDL_SetColorKey(quit_txt,SDL_SRCCOLORKEY,SDL_MapRGB(quit_txt->format,255,255,255));
	SDL_SetColorKey(blob,SDL_SRCCOLORKEY,SDL_MapRGB(blob->format,255,255,255));
	SDL_SetColorKey(clouds,SDL_SRCCOLORKEY,SDL_MapRGB(clouds->format,65,189,0));
	SDL_SetAlpha(clouds,SDL_SRCALPHA | SDL_RLEACCEL,128);
	SDL_Surface *selections[2];
	selections[0]=LoadImage("menu/worldmap/selection_no.png");
	selections[1]=LoadImage("menu/worldmap/selection_yes.png");

	int dot_coords[6][2];
	dot_coords[0][0]=22;
	dot_coords[0][1]=184;
	dot_coords[1][0]=108;
	dot_coords[1][1]=151;
	dot_coords[2][0]=185;
	dot_coords[2][1]=66;
	dot_coords[3][0]=162;
	dot_coords[3][1]=164;
	dot_coords[4][0]=211;
	dot_coords[4][1]=195;
	dot_coords[5][0]=292;
	dot_coords[5][1]=145;

	std::string mapnames[6];
	mapnames[0]="Green Valley";
	mapnames[1]="Windy Mountainpath";
	mapnames[2]="Icy Cave";
	mapnames[3]="Beating Jungle";
	mapnames[4]="Burning Desert";
	mapnames[5]="Foggy Castle";

	Mix_Music *bgm=Mix_LoadMUS("bgm/worldmap.ogg");
	Mix_PlayMusic(bgm,-1);

	int selected_map=finished_maps;
	int anim_counter=0;
	int clouds_x=0;
	bool left_released=false;
	bool right_released=false;
	bool menu_released=false;
	bool jump_released=false;
	bool select_released=false;
	bool quit=false;

	int buttons=get_button_state();
	do
	{
		buttons=get_button_state();
		update_volume(buttons);

		BlitSurface(bg,0,0,320,240,buffer,0,0);
		for(int i=0;i<6;++i)
		{
			if(i<finished_maps)
			{
				BlitSurface(dots,0,(dots->h/4)*3,dots->w,dots->h/4,buffer,dot_coords[i][0],dot_coords[i][1]);
			}
			else
			{
				if(i==finished_maps)
				{
					BlitSurface(dots,0,(dots->h/4),dots->w,dots->h/4,buffer,dot_coords[i][0],dot_coords[i][1]);
				}
				else
				{
					BlitSurface(dots,0,0,dots->w,dots->h/4,buffer,dot_coords[i][0],dot_coords[i][1]);
				};
			};
			if(i==selected_map)
			{
				BlitSurface(dots,0,(dots->h/4)*2,dots->w,dots->h/4,buffer,dot_coords[i][0],dot_coords[i][1]);
				BlitSurface(blob,0,(anim_counter/64)*(blob->h/4),blob->w,blob->h/4,buffer,dot_coords[i][0]-7,dot_coords[i][1]-12);
			};
		};

		stringRGBA(buffer,147-(mapnames[selected_map].size()/2)*5,7,mapnames[selected_map].c_str(),255,255,255,255);
		switch(selected_map)
		{
			case 0:
			{
				stringRGBA(buffer,284,7,"1/6",255,255,255,255);
				break;
			};
			case 1:
			{
				stringRGBA(buffer,284,7,"2/6",255,255,255,255);
				break;
			};
			case 2:
			{
				stringRGBA(buffer,284,7,"3/6",255,255,255,255);
				break;
			};
			case 3:
			{
				stringRGBA(buffer,284,7,"4/6",255,255,255,255);
				break;
			};
			case 4:
			{
				stringRGBA(buffer,284,7,"5/6",255,255,255,255);
				break;
			};
			case 5:
			{
				stringRGBA(buffer,284,7,"6/6",255,255,255,255);
				break;
			};
		};

		BlitSurface(clouds,0,0,320,240,buffer,clouds_x,40);
		BlitSurface(clouds,0,0,320,240,buffer,320+clouds_x,40);

		SDL_Flip(buffer);
		limit_frames(60);

		if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
		if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;
		if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;
		if(!jump_released && !(buttons & BTN_B)) jump_released=true;
		if(!select_released && !(buttons & BTN_SELECT)) select_released=true;

		if((buttons & BTN_LEFT) && left_released)
		{
			if(selected_map>0) selected_map--;
			left_released=false;
		};
		if((buttons & BTN_RIGHT) && right_released)
		{
			if(selected_map<finished_maps) selected_map++;
			right_released=false;
		};

		if((buttons & BTN_MENU) && menu_released)
		{
			menu_released=false;
			bool selected=false;
			do
			{
				buttons=get_button_state();
				BlitSurface(quit_txt,0,0,quit_txt->w,quit_txt->h,buffer,160-quit_txt->w/2,90);
				BlitSurface(selections[selected],0,0,selections[selected]->w,selections[selected]->h,buffer,160-selections[selected]->w/2,110);

				SDL_Flip(buffer);
				limit_frames(60);

				if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
				if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;
				if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;

				if((buttons & BTN_LEFT) && left_released)
				{
					selected=!selected;
					left_released=false;
				};
				if((buttons & BTN_RIGHT) && right_released)
				{
					selected=!selected;
					right_released=false;
				};
			}while(!((buttons & BTN_MENU) && menu_released) && !((buttons & BTN_B) && jump_released));
			if((buttons & BTN_MENU))
			{
				menu_released=false;
			}
			else
			{
				jump_released=false;
			};
			if(selected)
			{
				selected_map=-1;
				quit=true;
			};
		};
		if((buttons & BTN_SELECT) && select_released)
		{
			menu_released=false;
			bool selected=false;
			do
			{
				buttons=get_button_state();
				BlitSurface(save_txt,0,0,save_txt->w,save_txt->h,buffer,160-quit_txt->w/2,90);
				BlitSurface(selections[selected],0,0,selections[selected]->w,selections[selected]->h,buffer,160-selections[selected]->w/2,110);

				SDL_Flip(buffer);
				limit_frames(60);

				if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
				if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;
				if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;

				if((buttons & BTN_LEFT) && left_released)
				{
					selected=!selected;
					left_released=false;
				};
				if((buttons & BTN_RIGHT) && right_released)
				{
					selected=!selected;
					right_released=false;
				};
			}while(!((buttons & BTN_MENU) && menu_released) && !((buttons & BTN_B) && jump_released));
			if((buttons & BTN_MENU))
			{
				menu_released=false;
			}
			else
			{
				jump_released=false;
			};
			if(selected)
			{
				Mix_HaltMusic();
				load_save_menu(MODE_SAVE);
				Mix_PlayMusic(bgm,-1);
			};
		};

		anim_counter++;
		anim_counter%=256;
		if(anim_counter%4==0)
		{
			--clouds_x;
			clouds_x%=320;
		};
	}while(!((buttons & BTN_B) && jump_released) && !quit);

	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(dots);
	SDL_FreeSurface(save_txt);
	SDL_FreeSurface(quit_txt);
	SDL_FreeSurface(blob);
	SDL_FreeSurface(clouds);
	SDL_FreeSurface(selections[0]);
	SDL_FreeSurface(selections[1]);

	return selected_map;
};

bool skip_intro()
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *skip_txt=LoadImage("images/skip_intro.png");
	SDL_SetColorKey(skip_txt,SDL_SRCCOLORKEY,SDL_MapRGB(skip_txt->format,255,255,255));
	SDL_Surface *selection_txts[2];
	selection_txts[0]=LoadImage("images/continue_no.png");
	selection_txts[1]=LoadImage("images/continue_yes.png");
	int buttons;

	bool selected=false;
	bool right_released=true;
	bool left_released=true;
	bool jump_released=false;
	bool menu_released=false;

	buttons=get_button_state();
	do
	{
		buttons=get_button_state();
		update_volume(buttons);

		BlitSurface(skip_txt,0,0,skip_txt->w,skip_txt->h,buffer,160-skip_txt->w/2,80);
		BlitSurface(selection_txts[selected],0,0,selection_txts[selected]->w,selection_txts[selected]->h,buffer,160-selection_txts[selected]->w/2,120);

		SDL_Flip(buffer);
		limit_frames(60);

		if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
		if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;
		if(!jump_released && !(buttons & BTN_B)) jump_released=true;
		if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;

		if((buttons & BTN_LEFT) && left_released)
		{
			selected=!selected;
			left_released=false;
		};
		if((buttons & BTN_RIGHT) && right_released)
		{
			selected=!selected;
			right_released=false;
		};
	}while(!( ((buttons & BTN_MENU) && menu_released) || ((buttons & BTN_B) && jump_released)));

	SDL_FreeSurface(selection_txts[0]);
	SDL_FreeSurface(selection_txts[1]);
	SDL_FreeSurface(skip_txt);
	return selected;
};

void save_game(int saveslots[4])
{
	std::ofstream outfile(savefile,std::ios_base::binary);
	outfile.write((char*)&saveslots[0],sizeof(int));
	outfile.write((char*)&saveslots[1],sizeof(int));
	outfile.write((char*)&saveslots[2],sizeof(int));
	outfile.write((char*)&saveslots[3],sizeof(int));
};

void load_game(int saveslots[4])
{
	std::ifstream infile(savefile,std::ios_base::binary);
	if(infile.is_open())
	{
		infile.read((char*)&saveslots[0],sizeof(int));
		infile.read((char*)&saveslots[1],sizeof(int));
		infile.read((char*)&saveslots[2],sizeof(int));
		infile.read((char*)&saveslots[3],sizeof(int));
	}
	else
	{
		saveslots[0]=0;
		saveslots[1]=0;
		saveslots[2]=0;
		saveslots[3]=0;
	};
	infile.close();
};

int load_save_menu(bool mode)
{
	SDL_Surface *buffer=SDL_GetVideoSurface();

	SDL_Surface *bg=LoadImage("menu/load_save/bg.png");
	SDL_Surface *stageicons=LoadImage("menu/load_save/stageicons.png");
	SDL_Surface *selection=LoadImage("menu/load_save/selection.png");
	SDL_Surface *mode_txt;
	if(mode==MODE_LOAD)
	{
		mode_txt=LoadImage("menu/load_save/load_game_txt.png");
	}
	else
	{
		mode_txt=LoadImage("menu/load_save/save_game_txt.png");
	};
	SDL_SetColorKey(selection,SDL_SRCCOLORKEY,SDL_MapRGB(selection->format,0,0,0));
	SDL_SetColorKey(mode_txt,SDL_SRCCOLORKEY,SDL_MapRGB(mode_txt->format,255,255,255));

	std::string mapnames[6];
	mapnames[0]="Green Valley 0/6 completed";
	mapnames[1]="Windy Mountainpath 1/6 completed";
	mapnames[2]="Icy Cave 2/6 completed";
	mapnames[3]="Beating Jungle 3/6 completed";
	mapnames[4]="Burning Desert 4/6 completed";
	mapnames[5]="Foggy Castle 5/6 completed";

	Mix_Music *bgm=Mix_LoadMUS("bgm/guidelines.ogg");
	Mix_PlayMusic(bgm,-1);

	int saveslots[4];
	load_game(saveslots);

	int buttons;
	buttons=get_button_state();

	bool up_released=false;
	bool down_released=false;
	bool menu_released=false;
	bool jump_released=false;
	int selected_slot=0;

	do
	{
		buttons=get_button_state();
		update_volume(buttons);

		BlitSurface(bg,0,0,320,240,buffer,0,0);
		BlitSurface(mode_txt,0,0,mode_txt->w,mode_txt->h,buffer,160-mode_txt->w/2,2);
		BlitSurface(selection,0,0,selection->w,selection->h,buffer,3,23+50*selected_slot);
		for(int i=0;i<4;++i)
		{
			BlitSurface(stageicons,saveslots[i]*stageicons->w/6,0,stageicons->w/6,stageicons->h,buffer,6,36+50*i);
			stringRGBA(buffer,60,50+50*i,mapnames[saveslots[i]].c_str(),255,255,255,255);
		};

		SDL_Flip(buffer);
		limit_frames(60);

		if(!up_released && !(buttons & BTN_UP)) up_released=true;
		if(!down_released && !(buttons & BTN_DOWN)) down_released=true;
		if(!jump_released && !(buttons & BTN_B)) jump_released=true;
		if(!menu_released && !(buttons & BTN_MENU)) menu_released=true;

		if((buttons & BTN_UP) && up_released)
		{
			selected_slot--;
			up_released=false;
		};
		if((buttons & BTN_DOWN) && down_released)
		{
			selected_slot++;
			down_released=false;
		};
		if(selected_slot<0) selected_slot=3;
		if(selected_slot>3) selected_slot=0;
	}while(!( ((buttons & BTN_MENU) && menu_released) || ((buttons & BTN_B) && jump_released)));

	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(stageicons);
	SDL_FreeSurface(selection);
	SDL_FreeSurface(mode_txt);

	if(mode==MODE_SAVE)
	{
		saveslots[selected_slot]=finished_maps;
		save_game(saveslots);
		return -1;
	}
	else
	{
		return saveslots[selected_slot];
	};
};

void show_victory_screen()
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *bg=LoadImage("menu/victory/bg.png");
	SDL_Surface *blob[2];
	blob[0]=LoadImage("menu/victory/blob_1.png");
	blob[1]=LoadImage("menu/victory/blob_2.png");
	SDL_SetColorKey(blob[0],SDL_SRCCOLORKEY,SDL_MapRGB(blob[0]->format,255,255,255));
	SDL_SetColorKey(blob[1],SDL_SRCCOLORKEY,SDL_MapRGB(blob[1]->format,255,255,255));
	SDL_Surface *txt[2];
	txt[0]=LoadImage("menu/victory/victory_1.png");
	txt[1]=LoadImage("menu/victory/victory_2.png");
	SDL_SetColorKey(txt[0],SDL_SRCCOLORKEY,SDL_MapRGB(txt[0]->format,255,255,255));
	SDL_SetColorKey(txt[1],SDL_SRCCOLORKEY,SDL_MapRGB(txt[1]->format,255,255,255));
	SDL_Surface *button_images[2];
	button_images[0]=LoadImage("menu/victory/button_1.png");
	button_images[1]=LoadImage("menu/victory/button_2.png");
	SDL_SetColorKey(button_images[0],SDL_SRCCOLORKEY,SDL_MapRGB(button_images[0]->format,255,255,255));
	SDL_SetColorKey(button_images[1],SDL_SRCCOLORKEY,SDL_MapRGB(button_images[1]->format,255,255,255));

	Mix_Music *bgm=Mix_LoadMUS("bgm/stage_clear.ogg");
	Mix_PlayMusic(bgm,-1);

	int buttons=get_button_state();
	int anim_counter=0;
	int scroll_x=0, scroll_y=0;

	do
	{
		buttons=get_button_state();

		BlitSurface(bg,0,0,320,240,buffer,scroll_x,scroll_y);
		BlitSurface(bg,0,0,320,240,buffer,scroll_x,scroll_y+240);
		BlitSurface(bg,0,0,320,240,buffer,scroll_x-320,scroll_y+240);
		BlitSurface(bg,0,0,320,240,buffer,scroll_x-320,scroll_y);
		BlitSurface(blob[anim_counter/64],0,0,320,240,buffer,0,0);
		BlitSurface(txt[anim_counter/64],0,0,320,240,buffer,160-(txt[anim_counter/64]->w/2),120-(txt[anim_counter/64]->h/2));
		BlitSurface(button_images[anim_counter/64],0,0,320,240,buffer,0,0);

		SDL_Flip(buffer);
		limit_frames(60);

		++anim_counter;
		anim_counter%=128;
		if(anim_counter%3==0)
		{
			scroll_x+=4;
			scroll_y-=3;
			scroll_x%=320;
			scroll_y%=240;
		};
	}while(!(buttons & BTN_B) && !(buttons & BTN_MENU));

	Mix_HaltMusic();
	Mix_FreeMusic(bgm);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(blob[0]);
	SDL_FreeSurface(blob[1]);
	SDL_FreeSurface(txt[0]);
	SDL_FreeSurface(txt[1]);
	SDL_FreeSurface(button_images[0]);
	SDL_FreeSurface(button_images[1]);
};

void play_blob_intro(SDL_Surface *screen)
{
	SDL_Surface *images[12];
	images[0]=LoadImage("intro/images/1.png");
	images[1]=LoadImage("intro/images/2.png");
	images[2]=LoadImage("intro/images/3.png");
	images[3]=LoadImage("intro/images/4.png");
	images[4]=LoadImage("intro/images/5.png");
	images[5]=LoadImage("intro/images/6.png");
	images[6]=LoadImage("intro/images/7.png");
	images[7]=LoadImage("intro/images/8.png");
	images[8]=LoadImage("intro/images/9.png");
	images[9]=LoadImage("intro/images/10.png");
	images[10]=LoadImage("intro/images/11.png");
	images[11]=LoadImage("intro/images/12.png");

	SDL_Rect strip;
	strip.w=320;
	strip.h=40;
	strip.x=0;
	strip.y=0;

	for(int i=0;i<280;++i)
	{
		BlitSurface(images[0],0,0,images[0]->w,images[0]->h,screen,0,20-i);
		if(i>25)
		{
			boxRGBA(screen,0,0,320,240,0,0,0,i-25);
		};

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,118,215,"A distant land...",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<320;++i)
	{
		BlitSurface(images[1],0,0,images[1]->w,images[1]->h,screen,-i,0);
		if(i<255)
		{
			boxRGBA(screen,0,0,320,240,0,0,0,255-i);
		}
		else
		{
			SDL_SetAlpha(images[2],SDL_SRCALPHA,i-255);
			BlitSurface(images[2],0,0,images[2]->w,images[2]->h,screen,(i-255)-320,0);
		};

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,5,215,"consisting of high mountains, deep lakes and beautiful valleys",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<256;++i)
	{
		BlitSurface(images[1],0,0,images[1]->w,images[1]->h,screen,-320,0);
		if(i<192)
		{
			SDL_SetAlpha(images[2],SDL_SRCALPHA,64+i);
			BlitSurface(images[2],0,0,images[2]->w,images[2]->h,screen,i-256,0);
		}
		else
		{
			BlitSurface(images[2],0,0,images[2]->w,images[2]->h,screen,i-256,0);
			boxRGBA(screen,0,0,320,240,0,0,0,(i-192)*4);
		};


		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,60,215,"A paradise were creatures of all kind...",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<240;++i)
	{
		BlitSurface(images[3],0,0,images[3]->w,images[3]->h,screen,0,-i);
		boxRGBA(screen,0,0,320,240,0,0,0,255-i);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,95,215,"...live together in harmony",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<320;++i)
	{
		BlitSurface(images[3],0,0,images[3]->w,images[3]->h,screen,0,-240);
		if(i<255)
		{
			SDL_SetAlpha(images[4],SDL_SRCALPHA,i);
		};
		BlitSurface(images[4],0,0,images[4]->w,images[4]->h,screen,-i,0);

		if(i<15) boxRGBA(screen,0,0,320,240,0,0,0,15-i);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,115,215,"in a small town...",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<255;++i)
	{
		BlitSurface(images[4],0,0,images[4]->w,images[4]->h,screen,-320,0);
		SDL_SetAlpha(images[5],SDL_SRCALPHA,i);
		BlitSurface(images[5],0,0,images[5]->w,images[5]->h,screen,0,0);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,65,215,"...little kind creatures called Blobs",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<200;++i)
	{
		BlitSurface(images[6],0,0,images[6]->w,images[6]->h,screen,0,i-240);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,90,215,"...enjoy this wonderful life",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<120;++i)
	{
		BlitSurface(images[7],0,0,images[7]->w,images[7]->h,screen,0,-(i*2));

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,138,215,"until now",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<300;++i)
	{
		BlitSurface(images[8],0,0,images[8]->w,images[8]->h,screen,0,0);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,138,215,"until now",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	}

	for(int i=0;i<255;++i)
	{
		BlitSurface(images[8],0,0,images[8]->w,images[8]->h,screen,0,0);
		boxRGBA(screen,0,0,320,240,0,0,0,i);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,138,215,"until now",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<255;++i)
	{
		BlitSurface(images[9],0,0,images[9]->w,images[9]->h,screen,0,0);
		boxRGBA(screen,0,0,320,240,0,0,0,255-i);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,138,215,"until now",255,255,255,255);
		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<320;++i)
	{
		BlitSurface(images[9],0,0,images[9]->w,images[9]->h,screen,0,0);
		if(i<255) SDL_SetAlpha(images[10],SDL_SRCALPHA,i);
		BlitSurface(images[10],0,0,images[10]->w,images[10]->h,screen,-i,0);
		if(i>64) boxRGBA(screen,0,0,320,240,0,0,0,i-64);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,70,215,"but not all of them are defenseless",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	for(int i=0;i<240;++i)
	{
		BlitSurface(images[11],0,0,images[11]->w,images[11]->h,screen,0,i-240);
		boxRGBA(screen,0,0,320,240,0,0,0,255-i);

		strip.y=0;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		strip.y=200;
		SDL_FillRect(screen,&strip,SDL_MapRGB(screen->format,0,0,0));
		stringRGBA(screen,125,215,"One will rise!",255,255,255,255);

		SDL_Flip(screen);
		limit_frames(60);
	};

	SDL_FreeSurface(images[0]);
	SDL_FreeSurface(images[1]);
	SDL_FreeSurface(images[2]);
	SDL_FreeSurface(images[3]);
	SDL_FreeSurface(images[4]);
	SDL_FreeSurface(images[5]);
	SDL_FreeSurface(images[6]);
	SDL_FreeSurface(images[7]);
	SDL_FreeSurface(images[8]);
	SDL_FreeSurface(images[9]);
	SDL_FreeSurface(images[10]);
	SDL_FreeSurface(images[11]);
};

void update_volume(int buttons)
{
	if(buttons & BTN_VOLUP)
	{
		Mix_VolumeMusic(Mix_VolumeMusic(-1)+1);
		Mix_Volume(-1,Mix_VolumeMusic(-1));
	}
	else
	{
		if(buttons & BTN_VOLDOWN)
		{
			Mix_VolumeMusic(Mix_VolumeMusic(-1)-1);
			Mix_Volume(-1,Mix_VolumeMusic(-1));
		};
	};
};

void play_credits(SDL_Surface *buffer)
{
	SDL_Surface *stars[4];
	stars[0]=LoadImage("credits/star1.png");
	stars[1]=LoadImage("credits/star2.png");
	stars[2]=LoadImage("credits/star3.png");
	stars[3]=LoadImage("credits/star4.png");
	for(int i=0;i<4;++i)
	{
		SDL_SetColorKey(stars[i],SDL_SRCCOLORKEY,SDL_MapRGB(stars[i]->format,255,255,255));
	};

	SDL_Surface *bg=LoadImage("credits/bg1.png");
	SDL_Surface *end_bg[3];
	end_bg[0]=LoadImage("credits/end0.png");
	end_bg[1]=LoadImage("credits/end1.png");
	end_bg[2]=LoadImage("credits/end2.png");
	SDL_Surface *baloon[2];
	baloon[0]=LoadImage("credits/baloon.png");
	baloon[1]=LoadImage("credits/baloon_blob.png");
	SDL_Surface *parachute[2];
	parachute[0]=LoadImage("credits/blob_parachute.png");
	parachute[1]=LoadImage("credits/blob_parachute_damaged.png");
	for(int i=0;i<2;++i)
	{
		SDL_SetColorKey(baloon[i],SDL_SRCCOLORKEY,SDL_MapRGB(baloon[i]->format,255,255,255));
		SDL_SetColorKey(parachute[i],SDL_SRCCOLORKEY,SDL_MapRGB(parachute[i]->format,255,255,255));
	};
	SDL_Surface *package=LoadImage("credits/package.png");
	SDL_SetColorKey(package,SDL_SRCCOLORKEY,SDL_MapRGB(package->format,255,255,255));
	SDL_Surface *exclamation=LoadImage("credits/blob_exclamation.png");
	SDL_SetColorKey(exclamation,SDL_SRCCOLORKEY,SDL_MapRGB(exclamation->format,255,255,255));
	SDL_Surface *credits_txt=LoadImage("credits/credits.png");

	Mix_Music *credits_music=Mix_LoadMUS("bgm/credits.ogg");
	Mix_PlayMusic(credits_music, -1);

	int star_x[8], star_y[8];

	int anim_counter=0;
	do
	{
		if(anim_counter<440)
		{
			BlitSurface(bg,0,0,320,240,buffer,0,0);
			if(anim_counter<300) BlitSurface(exclamation,(exclamation->w/4)*((anim_counter%120)/30),0,(exclamation->w/4),exclamation->h,buffer,260,25);
			if(anim_counter>120)
			{
				if(anim_counter<300) BlitSurface(baloon[0],(baloon[0]->w/4)*((anim_counter%120)/30),0,baloon[0]->w/4,baloon[0]->h,buffer,260,360-anim_counter);
				else BlitSurface(baloon[1],(baloon[1]->w/4)*((anim_counter%120)/30),0,baloon[1]->w/4,baloon[1]->h,buffer,260,360-anim_counter);
				BlitSurface(baloon[0],(baloon[0]->w/4)*((anim_counter%120)/30),0,baloon[0]->w/4,baloon[0]->h,buffer,100,370-anim_counter);
				BlitSurface(baloon[0],(baloon[0]->w/4)*((anim_counter%120)/30),0,baloon[0]->w/4,baloon[0]->h,buffer,200,380-anim_counter);
				BlitSurface(baloon[0],(baloon[0]->w/4)*((anim_counter%120)/30),0,baloon[0]->w/4,baloon[0]->h,buffer,150,365-anim_counter);
				BlitSurface(baloon[0],(baloon[0]->w/4)*((anim_counter%120)/30),0,baloon[0]->w/4,baloon[0]->h,buffer,30,360-anim_counter);
			};
		}
		else
		{
			if(anim_counter<1690)
			{
				SDL_FillRect(buffer,NULL,SDL_MapRGB(buffer->format,0,0,0));
				BlitSurface(credits_txt,0,0,credits_txt->w,credits_txt->h,buffer,160-credits_txt->w/2,680-anim_counter);
				if(anim_counter<1600) BlitSurface(parachute[0],(parachute[0]->w/4)*((anim_counter%120)/30),0,parachute[0]->w/4,parachute[0]->h,buffer,160-parachute[0]->w/8,-1500+anim_counter);
				else BlitSurface(parachute[1],(parachute[1]->w/4)*(((anim_counter-40)%120)/30),0,parachute[1]->w/4,parachute[1]->h,buffer,160-parachute[1]->w/8,-1500+anim_counter);
			}
			else
			{
				if(anim_counter<1890)
				{
					SDL_FillRect(buffer,NULL,SDL_MapRGB(buffer->format,0,0,0));
					BlitSurface(credits_txt,0,0,credits_txt->w,credits_txt->h,buffer,160-credits_txt->w/2,680-anim_counter);
					BlitSurface(parachute[1],(parachute[1]->w/4)*3,0,parachute[1]->w/4,parachute[1]->h,buffer,160-parachute[1]->w/8,1890-anim_counter);
					BlitSurface(package,0,0,package->w,package->h,buffer,160-package->w/2,240-package->h);
				}
				else
				{
					if(anim_counter<1940)
					{
						BlitSurface(end_bg[0],0,0,320,240,buffer,0,0);
					}
					else
					{
						if(anim_counter<1970)
						{
							BlitSurface(end_bg[1],0,0,320,240,buffer,0,0);
							for(int i=0;i<8;++i)
							{
								star_x[i]=20+rand()%200;
								star_y[i]=50+rand()%150;
							};
						}
						else
						{
							BlitSurface(end_bg[2],0,0,320,240,buffer,0,0);
							for(int i=0;i<8;++i)
							{
								star_x[i]+=(star_x[i]>160?1:-1);
								--star_y[i];
								BlitSurface(stars[i%4],0,0,stars[i%4]->w,stars[i%4]->h,buffer,star_x[i],star_y[i]);
							};
						};
					};
				};
			};
		};

		++anim_counter;

		SDL_Flip(buffer);
		limit_frames(60);
	}while(anim_counter<2200);


	SDL_FreeSurface(stars[0]);
	SDL_FreeSurface(stars[1]);
	SDL_FreeSurface(stars[2]);
	SDL_FreeSurface(stars[3]);
	SDL_FreeSurface(baloon[0]);
	SDL_FreeSurface(baloon[1]);
	SDL_FreeSurface(parachute[0]);
	SDL_FreeSurface(parachute[1]);
	SDL_FreeSurface(end_bg[0]);
	SDL_FreeSurface(end_bg[1]);
	SDL_FreeSurface(end_bg[2]);
	SDL_FreeSurface(bg);
	SDL_FreeSurface(package);
	SDL_FreeSurface(exclamation);
	SDL_FreeSurface(credits_txt);


	Mix_HaltMusic();
	Mix_FreeMusic(credits_music);
};

void play_2d_rp_intro(SDL_Surface *buffer)
{
	SDL_Event event;
	SDL_Surface *tmp;
	Mix_Chunk *snd[4];
	snd[0]=Mix_LoadWAV("2d_rp/0.wav");
	snd[1]=Mix_LoadWAV("2d_rp/1.wav");
	snd[2]=Mix_LoadWAV("2d_rp/2.wav");
	snd[3]=Mix_LoadWAV("2d_rp/3.wav");
	Mix_VolumeChunk(snd[0],40);
	Mix_VolumeChunk(snd[1],40);
	Mix_VolumeChunk(snd[2],40);
	Mix_VolumeChunk(snd[3],40);
	for(int i=1;i<49;++i)
	{
		std::string tmps="2d_rp/";
		tmps=tmps+(char)('0'+(i/10));
		tmps=tmps+(char)('0'+(i%10));
		tmps+=".png";
		tmp=LoadImage(tmps.c_str());
		BlitSurface(tmp,0,0,320,240,buffer,0,0);
		SDL_FreeSurface(tmp);
		if(i==20) Mix_PlayChannel(-1,snd[0],0);
		if(i==22) Mix_PlayChannel(-1,snd[1],0);
		if(i==29) Mix_PlayChannel(-1,snd[2],0);
		if(i==40) Mix_PlayChannel(-1,snd[3],0);
		while(SDL_PollEvent(&event));

		SDL_Flip(buffer);
		SDL_Delay(140);
	};

	Mix_FreeChunk(snd[0]);
	Mix_FreeChunk(snd[1]);
	Mix_FreeChunk(snd[2]);
	Mix_FreeChunk(snd[3]);
	SDL_Delay(1000);
};
