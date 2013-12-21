
class jr_player
{
	private:
		SDL_Surface *image, *image_left;
		SDL_Surface *image_default, *image_default_left;
		SDL_Surface *image_earth, *image_earth_left;
		SDL_Surface *image_wind, *image_wind_left;
		SDL_Surface *image_fire, *image_fire_left;
		SDL_Surface *image_ice, *image_ice_left;
		SDL_Surface *snowball_image;
		int player_size_x, player_size_y;
		int mapsize_x, mapsize_y;
		int pos_x, pos_y;
		int pos_x_on_map, pos_y_on_map;
		int walking_direction;
		int walking_state;
		int state;
		int anim_state;
		int jumpstate;
		bool jumped;
		bool forced_jumping;
		int falling_state;
		bool falling;
		bool punshing;
		bool punshed;
		bool moved;
		bool moved_on_platform;
		bool transformated;
		bool inventory_shown;
		bool hit_direction;
		int default_movespeed;
		int def_speed;

		int life;
		int max_life;
		int one_ups;
		SDL_Surface *lifebar, *oneups_image;

		SDL_Surface *pause_image, *continue_pause, *quit_pause;

		int strength;
		int defense;
		int element_id;
		int element_time_counter;
		int attack_delay;
		int attack_delay_counter;
		bool attack_sound_played;
		unsigned int shake_time_counter, shake_time;

		int attached_liana;
		bool lianas_reseted;

		int attached_ladder;

		int downwards_correction_value;

		std::vector<int> inventory;
		SDL_Surface *item_box, *item_txt;

		bool boss_battle;
		SDL_Surface *boss_icon;

		bool passage_map;

		bool map_end_set;
		int map_end_x, map_end_y;

		jr_map *map;
		jr_environment *environment;
	public:
		jr_player(std::string image_filename,std::string image_filename_left,std::string image_filename_earth,std::string image_filename_earth_left,std::string image_filename_wind,std::string image_filename_wind_left,std::string image_filename_fire,std::string image_filename_fire_left,std::string image_filename_ice,std::string image_filename_ice_left,int correction_down,int default_life, std::string lifebar_image,std::string oneups_filename, jr_map *player_map, jr_environment *env)
		{
			map=player_map;
			environment=env;
			image=LoadImage(image_filename.c_str());
			image_left=LoadImage(image_filename_left.c_str());
			SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,255,255,255));
			SDL_SetColorKey(image_left,SDL_SRCCOLORKEY,SDL_MapRGB(image_left->format,255,255,255));
			image_earth=LoadImage(image_filename_earth.c_str());
			image_earth_left=LoadImage(image_filename_earth_left.c_str());
			SDL_SetColorKey(image_earth,SDL_SRCCOLORKEY,SDL_MapRGB(image_earth->format,255,255,255));
			SDL_SetColorKey(image_earth_left,SDL_SRCCOLORKEY,SDL_MapRGB(image_earth_left->format,255,255,255));
			image_wind=LoadImage(image_filename_wind.c_str());
			image_wind_left=LoadImage(image_filename_wind_left.c_str());
			SDL_SetColorKey(image_wind,SDL_SRCCOLORKEY,SDL_MapRGB(image_wind->format,255,255,255));
			SDL_SetColorKey(image_wind_left,SDL_SRCCOLORKEY,SDL_MapRGB(image_wind_left->format,255,255,255));
			image_fire=LoadImage(image_filename_fire.c_str());
			image_fire_left=LoadImage(image_filename_fire_left.c_str());
			SDL_SetColorKey(image_fire,SDL_SRCCOLORKEY,SDL_MapRGB(image_fire->format,255,255,255));
			SDL_SetColorKey(image_fire_left,SDL_SRCCOLORKEY,SDL_MapRGB(image_fire_left->format,255,255,255));
			image_ice=LoadImage(image_filename_ice.c_str());
			image_ice_left=LoadImage(image_filename_ice_left.c_str());
			SDL_SetColorKey(image_ice,SDL_SRCCOLORKEY,SDL_MapRGB(image_ice->format,255,255,255));
			SDL_SetColorKey(image_ice_left,SDL_SRCCOLORKEY,SDL_MapRGB(image_ice_left->format,255,255,255));
			snowball_image=LoadImage("images/snowball.png");
			SDL_SetColorKey(snowball_image,SDL_SRCCOLORKEY,SDL_MapRGB(snowball_image->format,255,255,255));
			player_size_x=image->w/4;
			player_size_y=image->h/9;
			mapsize_x=map->map_size_x*environment->tile_size_x;
			mapsize_y=map->map_size_y*environment->tile_size_y;
			state=PLAYER_STATE_IDLE;
			walking_direction=PLAYER_DIRECTION_RIGHT;
			hit_direction=DIRECTION_LEFT;
			walking_state=PLAYER_WALKING_STATE_NORMAL;
			default_movespeed=2;
			def_speed=2;
			pos_x=0;
			pos_y=0;
			pos_x_on_map=0;
			pos_y_on_map=0;
			jumped=false;
			forced_jumping=false;
			falling=true;
			falling_state=0;
			punshing=false;
			punshed=false;
			moved=false;
			moved_on_platform=false;
			transformated=false;
			inventory_shown=false;
			move_to_pos(map->player_starting_pos_x,map->player_starting_pos_y);

			downwards_correction_value=correction_down;
			life=default_life;
			max_life=default_life;
			one_ups=2;
			lifebar=LoadImage(lifebar_image.c_str());
			oneups_image=LoadImage(oneups_filename.c_str());
			SDL_SetColorKey(lifebar,SDL_SRCCOLORKEY,SDL_MapRGB(lifebar->format,255,255,255));

			pause_image=LoadImage("images/pause.png");
			continue_pause=LoadImage("images/continue_pause.png");
			quit_pause=LoadImage("images/quit_pause.png");
			SDL_SetColorKey(pause_image,SDL_SRCCOLORKEY,SDL_MapRGB(pause_image->format,255,255,255));
			SDL_SetColorKey(continue_pause,SDL_SRCCOLORKEY,SDL_MapRGB(continue_pause->format,255,255,255));
			SDL_SetColorKey(quit_pause,SDL_SRCCOLORKEY,SDL_MapRGB(quit_pause->format,255,255,255));

			item_box=LoadImage("images/item_box.png");
			item_txt=LoadImage("images/item_txt.png");
			SDL_SetColorKey(item_txt,SDL_SRCCOLORKEY,SDL_MapRGB(item_txt->format,255,255,255));

			strength=3;
			defense=0;
			attack_delay=160;
			attack_delay_counter=0;

			shake_time=40;

			attached_liana=-1;
			lianas_reseted=false;

			attached_ladder=-1;

			element_id=NO_ELEMENT;
			element_time_counter=0;

			image_default=image;
			image_default_left=image_left;
			attack_sound_played=false;

			boss_battle=false;
			passage_map=false;
			map_end_set=false;
		};

		void reset()
		{
			if(element_id!=NO_ELEMENT)
			{
				element_id=NO_ELEMENT;
				element_time_counter=0;
				image=image_default;
				image_left=image_default_left;
			};
			life=max_life;
			one_ups=2;
			state=PLAYER_STATE_IDLE;
			walking_direction=PLAYER_DIRECTION_RIGHT;
			hit_direction=DIRECTION_LEFT;
			moved=false;
			moved_on_platform=false;
			if(walking_state==PLAYER_WALKING_STATE_RUNNING)
			{
				walking_state=PLAYER_WALKING_STATE_NORMAL;
				default_movespeed/=2;
			};
			if(boss_battle && inventory.size()>0)
			{
				inventory.pop_back();
			}
			else
			{
				inventory.erase(inventory.begin(),inventory.end());
			};
			map->reload();
			map_changed();

			attached_liana=-1;
			lianas_reseted=false;
			attached_ladder=-1;
		};

		void map_changed()
		{
			mapsize_x=map->map_size_x*environment->tile_size_x;
			mapsize_y=map->map_size_y*environment->tile_size_y;
			map->reset_scroll_positions();
			move_to_pos(map->player_starting_pos_x,map->player_starting_pos_y);
			passage_map=false;
			map_end_set=false;
		};

		void reset_pos()
		{
			move_to_pos(map->player_starting_pos_x,map->player_starting_pos_y);
		};

		void set_pos(int x, int y)
		{
			pos_x=x;
			pos_y=y;
			pos_x_on_map=pos_x+map->get_tile_scroll_x();
			pos_y_on_map=pos_y+map->get_tile_scroll_y();
		};

		void move_to_pos(int x, int y)
		{
			pos_x_on_map=x;
			pos_y_on_map=y;
			if(x<160-player_size_x/2)
			{
				map->tile_scroll_x=0;
			}
			else
			{
				if(x>=map->map_size_x*environment->tile_size_x-320)
				{
					map->tile_scroll_x=-(map->map_size_x*environment->tile_size_x-320);
				}
				else
				{
					map->tile_scroll_x=-(pos_x_on_map-160);
				};
			};

			if(y<160)
			{
				map->tile_scroll_y=0;
			}
			else
			{
				if(y>map->map_size_y*environment->tile_size_y-240)
				{
					map->tile_scroll_y=-(map->map_size_y*environment->tile_size_y-240);
				}
				else
				{
					map->tile_scroll_y=-(pos_y_on_map-120);
				};
			};

			pos_x=pos_x_on_map+map->tile_scroll_x;
			pos_y=pos_y_on_map+map->tile_scroll_y;
			if(environment->bg_image!=NULL && map->map_size_y*environment->tile_size_y==environment->bg_image->h)
			{
				map->bg_image_scroll_y=(map->tile_scroll_y*(map->bg_image_scrollspeed_y/map->tile_scrollspeed_y))%environment->bg_image->h+(environment->bg_image->h-240);
			};
		};

		void push_sidewards(int push_distance)
		{
			if(push_distance>0)
			{
				while(collides(PLAYER_DIRECTION_RIGHT,push_distance) && push_distance>0) push_distance--;
				if(pos_x_on_map+player_size_x<mapsize_x)
				{
					if(map->tile_scroll_x-push_distance<=-(mapsize_x-320) || pos_x+push_distance<=160-player_size_x/2)
					{
						pos_x+=push_distance;
						pos_x_on_map+=push_distance;
					}
					else
					{
						pos_x_on_map+=push_distance;
						map->scroll(0,push_distance);
					};
				};
			}
			else
			{
				push_distance=-push_distance;
				while(collides(PLAYER_DIRECTION_LEFT,push_distance) && push_distance>0) push_distance--;
				if(pos_x_on_map>0)
				{
					if(map->tile_scroll_x+push_distance>=0 || pos_x-push_distance>=160-player_size_x/2)
					{
						pos_x-=push_distance;
						pos_x_on_map-=push_distance;
					}
					else
					{
						pos_x_on_map-=push_distance;
						map->scroll(1,push_distance);
					};
				};
			};
		};

		void push_upwards(int push_distance) //works only upwards ;-);
		{
			if(map->tile_scroll_y+push_distance>=0 || pos_y>=120-player_size_y || map->map_size_y==24)
			{
				pos_y-=push_distance;
			}
			else
			{
				map->scroll(2,push_distance);
			};
			pos_y_on_map-=push_distance;
		};

		void push_downwards(int push_distance) //does not work properly with negative values ;-) ;-);
		{
			if((-(map->tile_scroll_y)+push_distance>=mapsize_y-240) || pos_y<=160 || map->map_size_y==24)
			{
				pos_y+=push_distance;
			}
			else
			{
				map->scroll(3,push_distance);
			};
			pos_y_on_map+=push_distance;
		};


		void boss_battle_start(std::string boss_icon_filename)
		{
			boss_icon=LoadImage(boss_icon_filename.c_str());
			SDL_SetColorKey(boss_icon,SDL_SRCCOLORKEY,SDL_MapRGB(boss_icon->format,255,255,255));
			boss_battle=true;
		};

		void boss_battle_end()
		{
			SDL_FreeSurface(boss_icon);
			boss_battle=false;
		};

		void set_passage_map(bool value)
		{
			passage_map=value;
		};

		void set_map_end(int x, int y)
		{
			map_end_set=true;
			map_end_x=x;
			map_end_y=y;
		};

		int get_pos_x()
		{
			return pos_x;
		};

		int get_pos_y()
		{
			return pos_y;
		};

		int get_size_x()
		{
			return player_size_x;
		};

		int get_size_y()
		{
			return player_size_y;
		};

		void increase_animation_counter()
		{
			int def_movespeed=default_movespeed;
			if(walking_state==PLAYER_WALKING_STATE_RUNNING) def_movespeed/=2;
			anim_state+=default_movespeed;
			if(element_id==ELEMENT_WIND)
			{
				if(!transformated && anim_state>=128) transformated=true;
				if(state==PLAYER_STATE_HEALING || state==PLAYER_STATE_HIT)
				{
					if(anim_state>127)
					{
						anim_state=127;
					};
				}
				anim_state%=128;
				if(state==PLAYER_STATE_JUMPING) jumpstate+=def_movespeed;
				return;
			};
			if(state==PLAYER_STATE_JUMPING || falling)
			{
				if(anim_state<63)
				{
					anim_state+=2*default_movespeed;
				}
				else
				{
					if(walking_state==PLAYER_WALKING_STATE_RUNNING) anim_state-=def_movespeed;
					if(!falling) jumpstate+=def_movespeed;
				};
				if(anim_state>127)
				{
					anim_state=127;
				};
				if(falling) falling_state+=def_movespeed;
			}
			else
			{
				if(state==PLAYER_STATE_ATTACK)
				{
					if(anim_state>95)
					{
						if(collides(PLAYER_DIRECTION_DOWN,1))
						{
							anim_state=95;
						}
						else
						{
							if(anim_state>127)
							{
								anim_state=127;
							};
						};
					};
				}
				else
				{
					if(punshing || state==PLAYER_STATE_HEALING || state==PLAYER_STATE_HIT)
					{
						if(anim_state>127)
						{
							anim_state=127;
						};
					}
					else
					{
						anim_state%=128;
					};
				};
			};
		};

		void update_snowballs()
		{
			for(int i=0;i<map->number_of_thrown_snowballs;++i)
			{
				if(map->snowball_directions[i]==PLAYER_DIRECTION_LEFT)
				{
					map->snowball_x_pos[i]-=default_movespeed*2;
				}
				else
				{
					map->snowball_x_pos[i]+=default_movespeed*2;
				};

				bool delete_ball=false;
				if(map->snowball_x_pos[i]<0 || map->snowball_x_pos[i]+snowball_image->w>=map->map_size_x*environment->tile_size_x)
				{
					delete_ball=true;
				}
				else
				{
					for(int j=0;j<snowball_image->h;j+=environment->tile_size_y)
					{
						if(map->tiles[(map->snowball_y_pos[i]+j)/environment->tile_size_y][(map->snowball_x_pos[i]+snowball_image->w/2)/environment->tile_size_y]!=0)
						{
							delete_ball=true;
							break;
						};
					};
				};

				if(delete_ball)
				{
					map->number_of_thrown_snowballs--;
					for(int u=i;u<map->number_of_thrown_snowballs;++u)
					{
						map->snowball_directions[u]=map->snowball_directions[u+1];
						map->snowball_x_pos[u]=map->snowball_x_pos[u+1];
						map->snowball_y_pos[u]=map->snowball_y_pos[u+1];
					};
					--i;
				};
			};
		};

		void attach_to_ladder(int ladder_id)
		{
			if(walking_state==PLAYER_WALKING_STATE_RUNNING)
			{
				walking_state=PLAYER_WALKING_STATE_NORMAL;
				default_movespeed/=2;
			};
			attached_ladder=ladder_id;
			anim_state=0;
			falling=false;
			punshing=false;
			punshed=false;
			state=PLAYER_STATE_WALKING;
			push_sidewards((map->special_objects[attached_ladder].pos_x-5)-pos_x_on_map);
		};

		void update_ladder(int buttons)
		{
			falling=false;
			if((buttons & BTN_UP) && pos_y_on_map>(map->special_objects[attached_ladder].pos_y/10)*10)
			{
				push_upwards(default_movespeed);
				increase_animation_counter();
				increase_animation_counter();
			};
			if((buttons & BTN_DOWN) && pos_y_on_map+player_size_y<map->special_objects[attached_ladder].pos_y+(map->special_objects[attached_ladder].pos_y%10)*60)
			{
				push_downwards(default_movespeed);
				increase_animation_counter();
				increase_animation_counter();
			};
			if((buttons & BTN_B))
			{
				if((buttons & BTN_RIGHT)) walking_direction=PLAYER_DIRECTION_RIGHT;
				else	walking_direction=PLAYER_DIRECTION_LEFT;
				jumped=true;
				state=PLAYER_STATE_JUMPING;
				attached_ladder=-1;
				jumpstate=2*default_movespeed;
				anim_state=63;
			};
		};

		void attach_to_liana(int liana_id)
		{
			if(walking_state==PLAYER_WALKING_STATE_RUNNING)
			{
				walking_state=PLAYER_WALKING_STATE_NORMAL;
				default_movespeed/=2;
			};
			attached_liana=liana_id;
			switch(map->special_objects[attached_liana].anim_state/(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
			{
				case 0:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+17,map->special_objects[attached_liana].pos_y+62);
					break;
				};
				case 1:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+22,map->special_objects[attached_liana].pos_y+80);
					break;
				};
				case 2:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+39,map->special_objects[attached_liana].pos_y+107);
					break;
				};
				case 3:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+48,map->special_objects[attached_liana].pos_y+119);
					break;
				};
				case 4:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+57,map->special_objects[attached_liana].pos_y+107);
					break;
				};
				case 5:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+74,map->special_objects[attached_liana].pos_y+80);
					break;
				};
				case 6:
				{
					move_to_pos(map->special_objects[attached_liana].pos_x+79,map->special_objects[attached_liana].pos_y+62);
					break;
				};
			};
		};

		void update_liana(int buttons)
		{
			if(walking_direction==PLAYER_DIRECTION_RIGHT)
			{
				map->special_objects[attached_liana].anim_state++;
				if(map->special_objects[attached_liana].anim_state==environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)
				{
					move_to_pos(pos_x_on_map+5,pos_y_on_map+18);
				};
				if((map->special_objects[attached_liana].anim_state==2*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map+17,pos_y_on_map+27);
				};
				if((map->special_objects[attached_liana].anim_state==3*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map+9,pos_y_on_map+12);
				};
				if((map->special_objects[attached_liana].anim_state==4*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map+9,pos_y_on_map-12);
				};
				if((map->special_objects[attached_liana].anim_state==5*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map+17,pos_y_on_map-27);
				};
				if((map->special_objects[attached_liana].anim_state==6*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map+5,pos_y_on_map-18);
				};
			}
			else
			{

				map->special_objects[attached_liana].anim_state--;
				if(map->special_objects[attached_liana].anim_state+1==environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)
				{
					move_to_pos(pos_x_on_map-5,pos_y_on_map-18);
				};
				if((map->special_objects[attached_liana].anim_state+1==2*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map-17,pos_y_on_map-27);
				};
				if((map->special_objects[attached_liana].anim_state+1==3*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map-9,pos_y_on_map-12);
				};
				if((map->special_objects[attached_liana].anim_state+1==4*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map-9,pos_y_on_map+12);
				};
				if((map->special_objects[attached_liana].anim_state+1==5*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map-17,pos_y_on_map+27);
				};
				if((map->special_objects[attached_liana].anim_state+1==6*environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7))
				{
					move_to_pos(pos_x_on_map-5,pos_y_on_map+18);
				};
			};
			if(map->special_objects[attached_liana].anim_state==environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state)
			{
				walking_direction=PLAYER_DIRECTION_LEFT;
				map->special_objects[attached_liana].anim_state--;
			};
			if(map->special_objects[attached_liana].anim_state==0)
			{
				walking_direction=PLAYER_DIRECTION_RIGHT;
				map->special_objects[attached_liana].anim_state++;
			};

			if(!(buttons & BTN_B)) jumped=false;
			if((buttons & BTN_B) && !jumped)
			{
				if(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/map->special_objects[attached_liana].anim_state<4 || (walking_direction==PLAYER_DIRECTION_RIGHT && environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/map->special_objects[attached_liana].anim_state<5))
				{
					move_to_pos(pos_x_on_map+30, pos_y_on_map-10);
					walking_direction=PLAYER_DIRECTION_RIGHT;
				}
				else
				{
					move_to_pos(pos_x_on_map-30, pos_y_on_map-10);
					walking_direction=PLAYER_DIRECTION_LEFT;
				};
				jumped=true;
				state=PLAYER_STATE_JUMPING;
				attached_liana=-1;
				jumpstate=2*default_movespeed;
				anim_state=63;
				lianas_reseted=false;
			};
		};

		void reset_lianas()
		{
			if(!lianas_reseted)
			{
				bool finished=true;
				for(int i=0;i<map->special_objects.size();++i)
				{
					if(environment->special_object_types[map->special_objects[i].type_id].type==OBJECT_TYPE_LIANA)
					{
						if(map->special_objects[i].anim_state<environment->special_object_types[map->special_objects[i].type_id].max_anim_state/2)
						{
							++map->special_objects[i].anim_state;
							finished=false;
						}
						else
						{
							if(map->special_objects[i].anim_state>environment->special_object_types[map->special_objects[i].type_id].max_anim_state/2)
							{
								--map->special_objects[i].anim_state;
								finished=false;
							};
						};
					};
				};
				if(finished) lianas_reseted=true;
			};
		};

		bool collides(int direction, int distance)
		{
			switch(direction)
			{
				case PLAYER_DIRECTION_RIGHT:
				{
					for(int i=0;i<=player_size_y;i+=environment->tile_size_y)
					{
						if(map->tiles[(pos_y_on_map+i)/environment->tile_size_y][(pos_x_on_map+player_size_x+distance)/environment->tile_size_x]!=0)
						{
							if(map->tiles[(pos_y_on_map+i)/environment->tile_size_y][(pos_x_on_map+player_size_x)/environment->tile_size_x]==0)
							{
								return true;
							};
						};
					};

					SDL_Surface *tmp_img;
					int tmp_state=state;
					if(element_id==ELEMENT_WIND)
					{
						tmp_state=PLAYER_STATE_IDLE;
					};
					if(walking_direction==PLAYER_DIRECTION_RIGHT)
					{
						tmp_img=image;
					}
					else
					{
						tmp_img=image_left;
					};
					int last_id;
					if(map->special_objects_collide(tmp_img,pos_x_on_map+distance,pos_y_on_map,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state,last_id))
					{
						if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_VANISHING)
						{
							if(element_id==ELEMENT_FIRE)
							{
								map->remove_special_object(last_id);
								return false;
							};
							if(!map->special_object_collides(last_id,tmp_img,pos_x_on_map-1,pos_y_on_map,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state))
							{
								return true;
							}
							else
							{
								return false;
							};
						}
						else
						{
							if(punshing && anim_state==3*default_movespeed && environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK && map->special_objects[last_id].anim_state<2*((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3))
							{
								map->special_objects[last_id].anim_state+=((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3);
								Mix_PlayChannel(-1,hit_sound,0);
							};
							return false;
						};
					};
					return false;
				};

				case PLAYER_DIRECTION_LEFT:
				{
					for(int i=0;i<=player_size_y;i+=environment->tile_size_y)
					{
						if(map->tiles[(pos_y_on_map+i)/environment->tile_size_y][(pos_x_on_map-distance)/environment->tile_size_x]!=0)
						{
							if(map->tiles[(pos_y_on_map+i)/environment->tile_size_y][(pos_x_on_map)/environment->tile_size_x]==0)
							{
								return true;
							};
						};
					};

					SDL_Surface *tmp_img;
					int tmp_state=state;
					if(element_id==ELEMENT_WIND)
					{
						tmp_state=PLAYER_STATE_IDLE;
					};
					if(walking_direction==PLAYER_DIRECTION_RIGHT)
					{
						tmp_img=image;
					}
					else
					{
						tmp_img=image_left;
					};
					int last_id;
					if(map->special_objects_collide(tmp_img,pos_x_on_map-distance,pos_y_on_map,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state,last_id))
					{
						if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_VANISHING)
						{
							if(element_id==ELEMENT_FIRE)
							{
								map->remove_special_object(last_id);
								return false;
							};
							if(!map->special_object_collides(last_id,tmp_img,pos_x_on_map+1,pos_y_on_map,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state))
							{
								return true;
							}
							else
							{
								return false;
							};
						}
						else
						{
							if(punshing && anim_state==3*default_movespeed && environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK && map->special_objects[last_id].anim_state<2*((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3))
							{
								map->special_objects[last_id].anim_state+=((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3);
								Mix_PlayChannel(-1,hit_sound,0);
							};
							return false;
						};
					};
					return false;
				};

				case PLAYER_DIRECTION_DOWN:
				{
					for(int i=0;((i<=player_size_x) || ((player_size_x%environment->tile_size_x!=0) && i<=environment->tile_size_x*((player_size_x/environment->tile_size_x)+1)));i+=environment->tile_size_x)
					{
						if(i>player_size_x) i=player_size_x;
						if(map->tiles[(pos_y_on_map+player_size_y+distance-downwards_correction_value)/environment->tile_size_y][(pos_x_on_map+i)/environment->tile_size_x]!=0)
						{
							if(map->tiles[(pos_y_on_map+player_size_y-downwards_correction_value)/environment->tile_size_y][(pos_x_on_map+i)/environment->tile_size_x]==0)
							{
								return true;
							};
						};
					};

					SDL_Surface *tmp_img;
					int tmp_state=state;
					if(element_id==ELEMENT_WIND)
					{
						tmp_state=PLAYER_STATE_IDLE;
					};
					if(walking_direction==PLAYER_DIRECTION_RIGHT)
					{
						tmp_img=image;
					}
					else
					{
						tmp_img=image_left;
					};

					int last_id;
					if(map->special_objects_collide(tmp_img,pos_x_on_map,pos_y_on_map+distance,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state,last_id))
					{
						if(!moved && environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_MOVING_PLATFORM)
						{
							if(map->special_objects[last_id].anim_state/(environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state/8)<4)
							{
								if(pos_x_on_map>0)
								{
									if(map->tile_scroll_x+def_speed>=0 || pos_x>160-player_size_x/2)
									{
										pos_x-=def_speed;
										pos_x_on_map-=def_speed;
									}
									else
									{
										pos_x_on_map-=def_speed;
										map->scroll(1,def_speed);
									};
								};
							}
							else
							{
								if(map->special_objects[last_id].anim_state/(environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state/8)>4)
								{
									if(pos_x_on_map+player_size_x<mapsize_x)
									{
										if(map->tile_scroll_x-def_speed<=-(mapsize_x-320) || pos_x<160-player_size_x/2)
										{
											pos_x+=def_speed;
											pos_x_on_map+=def_speed;
										}
										else
										{
											pos_x_on_map+=def_speed;
											map->scroll(0,def_speed);
										};
									};
								};
							};

							int downspeed=4;
							while(downspeed>0 && map->special_object_collides(last_id,tmp_img,pos_x_on_map,pos_y_on_map+downspeed,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state)) downspeed--;

							if(downspeed>0)
							{
								pos_y_on_map+=downspeed;
								if((-(map->tile_scroll_y)+downspeed>=mapsize_y-240) || pos_y<=160 || map->map_size_y==24)
								{
									pos_y+=downspeed;
								}
								else
								{
									map->scroll(3,downspeed);
								};
							};
							moved=true;
						}
						else
						{
							if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_FLOATING_PLATFORM)
							{
								if(!map->special_object_collides(last_id,tmp_img,pos_x_on_map,pos_y_on_map-1,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state))
								{
									if(!moved_on_platform)
									{
										if(map->special_objects[last_id].anim_state==FLOATING_DIRECTION_RIGHT)
										{
											push_sidewards(1);
										}
										else
										{
											push_sidewards(-1);
										};
										moved_on_platform=true;
									};
									return true;
								}
								else
								{
									return false;
								};
							}
							else
							{
								if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_VANISHING)
								{
									if(element_id==ELEMENT_FIRE)
									{
										map->remove_special_object(last_id);
										return false;
									};
									if(!map->special_object_collides(last_id,tmp_img,pos_x_on_map,pos_y_on_map-1,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state))
									{
										return true;
									}
									else
									{
										return false;
									};
								}
								else
								{
									if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_LIANA)
									{
										attach_to_liana(last_id);
										return true;
									}
									else
									{
										if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK)
										{
											switch(map->special_objects[last_id].anim_state/((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3))
											{
												case 0:
												{
													if(state==PLAYER_STATE_ATTACK && defense<=0 && attack_delay_counter==0)
													{
														map->special_objects[last_id].anim_state=((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3);
														attack_delay_counter=1;
														Mix_PlayChannel(-1,hit_sound,0);
													};
													return false;
												};
												case 1:
												{
													if(state==PLAYER_STATE_ATTACK && defense<=0 && attack_delay_counter==0)
													{
														map->special_objects[last_id].anim_state=2*((environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state)/3);
														attack_delay_counter=1;
														Mix_PlayChannel(-1,hit_sound,0);
													};
													return false;
												};
												case 2:
												{
													if(attack_delay_counter==0 || attack_delay_counter>attack_delay)
													{
														if(element_id==ELEMENT_FIRE) //water extinguishes fire ;-);
														{
															element_id=NO_ELEMENT;
															element_time_counter=0;
															image=image_default;
															image_left=image_default_left;
														};
														if(!moved) push_upwards(1);
														moved=true;
													}
													else
													{
														if(state==PLAYER_STATE_ATTACK)
														{
															state=PLAYER_STATE_IDLE;
															anim_state=0;
															defense=0;
															attack_sound_played=false;
														};
													};
													return true;
												};
											};
										}
										else
										{
											if(environment->special_object_types[map->special_objects[last_id].type_id].type==OBJECT_TYPE_RISING_PLATFORM)
											{
												if(!map->special_object_collides(last_id,tmp_img,pos_x_on_map,pos_y_on_map-1,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state))
												{
													if(!moved_on_platform)
													{
														int tmp_speed=0;
														if(map->special_objects[last_id].anim_state>environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state/2)
														{
															++tmp_speed;
														}
														else
														{
															--tmp_speed;
														};
														if(map->special_objects[last_id].anim_state%(environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state/8)==0)
														{
															switch((map->special_objects[last_id].anim_state/(environment->special_object_types[map->special_objects[last_id].type_id].max_anim_state/8))%4)
															{
																case 0:
																{
																	tmp_speed-=2;
																	break;
																};
																case 1:
																{
																	tmp_speed+=2;
																	break;
																};
																case 2:
																{
																	tmp_speed+=4;
																	break;
																};
																case 3:
																{
																	tmp_speed-=4;
																	break;
																};
															};
														};
														moved_on_platform=true;
														if(tmp_speed>0)
														{
															punshed=false;
															push_upwards(tmp_speed);
														};
													};
													return true;
												}
												else
												{
													return false;
												};
											};
										};
									};
								};
							};
						};
						return true;
					};

					return false;
				};
			};
		};

		void draw(SDL_Surface *buffer)
		{
			if(element_id==ELEMENT_WIND)
			{
				int tmp_state=PLAYER_STATE_IDLE;
				if(!transformated) tmp_state=PLAYER_STATE_HIT; //Hit is transformation in this case ;-);
				if(walking_direction==PLAYER_DIRECTION_RIGHT)
				{
					BlitSurface(image,player_size_x*(anim_state/32),tmp_state*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
				}
				else
				{
					BlitSurface(image_left,player_size_x*(anim_state/32),tmp_state*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
				};

			}
			else
			{
				if(attached_liana!=-1 || attached_ladder!=-1 || (!falling && !punshing && !moved))
				{
					if(attached_liana!=-1)
					{
						switch(element_id)
						{
							case NO_ELEMENT:
							case ELEMENT_WIND:
							{
								BlitSurface(environment->special_object_types[map->special_objects[attached_liana].type_id].image,(environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7)*(map->special_objects[attached_liana].anim_state/(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)),environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5,environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7,environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5,buffer,map->tile_scroll_x+map->special_objects[attached_liana].pos_x,map->tile_scroll_y+map->special_objects[attached_liana].pos_y);
								break;
							};
							case ELEMENT_FIRE:
							{
								BlitSurface(environment->special_object_types[map->special_objects[attached_liana].type_id].image,(environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7)*(map->special_objects[attached_liana].anim_state/(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)),(environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5)*3,environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7,environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5,buffer,map->tile_scroll_x+map->special_objects[attached_liana].pos_x,map->tile_scroll_y+map->special_objects[attached_liana].pos_y);
								break;
							};
							case ELEMENT_EARTH:
							{
								BlitSurface(environment->special_object_types[map->special_objects[attached_liana].type_id].image,(environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7)*(map->special_objects[attached_liana].anim_state/(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)),(environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5)*2,environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7,environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5,buffer,map->tile_scroll_x+map->special_objects[attached_liana].pos_x,map->tile_scroll_y+map->special_objects[attached_liana].pos_y);
								break;
							};
							case ELEMENT_ICE:
							{
								BlitSurface(environment->special_object_types[map->special_objects[attached_liana].type_id].image,(environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7)*(map->special_objects[attached_liana].anim_state/(environment->special_object_types[map->special_objects[attached_liana].type_id].max_anim_state/7)),(environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5)*4,environment->special_object_types[map->special_objects[attached_liana].type_id].image->w/7,environment->special_object_types[map->special_objects[attached_liana].type_id].image->h/5,buffer,map->tile_scroll_x+map->special_objects[attached_liana].pos_x,map->tile_scroll_y+map->special_objects[attached_liana].pos_y);
								break;
							};
						};
					}
					else
					{
						if(attached_ladder!=-1)
						{
							switch(element_id)
							{
								case NO_ELEMENT:
								case ELEMENT_WIND:
								{
									BlitSurface(environment->special_object_types[map->special_objects[attached_ladder].type_id].image,(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4)*(anim_state/32),0,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4,buffer,pos_x,pos_y);
									break;
								};
								case ELEMENT_FIRE:
								{
									BlitSurface(environment->special_object_types[map->special_objects[attached_ladder].type_id].image,(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4)*(anim_state/32),(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4)*3,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4,buffer,pos_x,pos_y);
									break;
								};
								case ELEMENT_EARTH:
								{
									BlitSurface(environment->special_object_types[map->special_objects[attached_ladder].type_id].image,(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4)*(anim_state/32),(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4),environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4,buffer,pos_x,pos_y);
									break;
								};
								case ELEMENT_ICE:
								{
									BlitSurface(environment->special_object_types[map->special_objects[attached_ladder].type_id].image,(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4)*(anim_state/32),(environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4)*2,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->w/4,environment->special_object_types[map->special_objects[attached_ladder].type_id].image->h/4,buffer,pos_x,pos_y);
									break;
								};
							};
						}
						else
						{
							if(walking_direction==PLAYER_DIRECTION_RIGHT)
							{
								BlitSurface(image,player_size_x*(anim_state/32),state*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							}
							else
							{
								BlitSurface(image_left,player_size_x*(anim_state/32),state*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							};
						};
					};
				}
				else
				{
					if(punshing)
					{

						if(walking_direction==PLAYER_DIRECTION_RIGHT)
						{
							BlitSurface(image,player_size_x*(anim_state/32),PLAYER_STATE_PUNSHING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
						}
						else
						{
						BlitSurface(image_left,player_size_x*(anim_state/32),PLAYER_STATE_PUNSHING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
						};
					}
					else
					{
						if(moved)
						{
							if(walking_direction==PLAYER_DIRECTION_RIGHT)
							{
								BlitSurface(image,player_size_x*(anim_state/32),PLAYER_STATE_ROLLING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							}
							else
							{
								BlitSurface(image_left,player_size_x*(anim_state/32),PLAYER_STATE_ROLLING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							};
						}
						else
						{
							if(walking_direction==PLAYER_DIRECTION_RIGHT)
							{
								BlitSurface(image,player_size_x*(anim_state/32),PLAYER_STATE_FALLING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							}
							else
							{
								BlitSurface(image_left,player_size_x*(anim_state/32),PLAYER_STATE_FALLING*player_size_y,player_size_x,player_size_y,buffer,pos_x,pos_y);
							};
						};
					};
				};
			};

			for(int i=0;i<map->number_of_thrown_snowballs;++i)
			{
				BlitSurface(snowball_image,map->snowball_directions[i]*snowball_image->w/2,0,snowball_image->w/2,snowball_image->h,buffer,map->snowball_x_pos[i]+map->tile_scroll_x,map->snowball_y_pos[i]+map->tile_scroll_y);
			};
		};

		void draw_panel(SDL_Surface *buffer)
		{
			BlitSurface(lifebar,0,0,lifebar->w,lifebar->h,buffer,0,0);
			BlitSurface(oneups_image,(one_ups-1)*(oneups_image->w/5),0,oneups_image->w/5,oneups_image->h,buffer,119,2);
			SDL_Rect pl_rect;
			pl_rect.x=61;
			pl_rect.y=8;
			pl_rect.h=7;
			pl_rect.w=(life*52)/max_life;
			if(life>max_life/2)
			{
				SDL_FillRect(buffer,&pl_rect,SDL_MapRGB(buffer->format,0,255,0));
			}
			else
			{
				if(life>0)
				{
					SDL_FillRect(buffer,&pl_rect,SDL_MapRGB(buffer->format,255,0,0));
				};
			};

			if(boss_battle)
			{
				int enemy_num=(environment->bg_image_filename=="images/beating_jungle/lavly_bg.png" && map->enemys.size()>26)?25:0;
				BlitSurface(boss_icon,0,0,boss_icon->w,boss_icon->h,buffer,179,3);
				if(map->enemys[enemy_num].remaining_resistance<=0) return;
				SDL_Rect bl_rect;
				bl_rect.x=205;
				bl_rect.y=8;
				bl_rect.h=7;
				bl_rect.w=(map->enemys[enemy_num].remaining_resistance*102)/environment->enemy_types[map->enemys[enemy_num].type].default_resistance;
				SDL_FillRect(buffer,&bl_rect,SDL_MapRGB(buffer->format,255,0,0));
			};
		};


		bool pause(SDL_Surface *buffer)
		{
			bool jump_button_pressed=true;
			bool up_button_pressed=true;
			bool down_button_pressed=true;
			bool continue_sel=true;
			int buttons;
			int pause_anim_counter=0;
			map->paused=true;

			do
			{
				buttons=get_button_state();

				if((buttons & BTN_UP) && !up_button_pressed)
				{
					continue_sel=!continue_sel;
					up_button_pressed=true;
				};
				if((buttons & BTN_DOWN) && !down_button_pressed)
				{
					continue_sel=!continue_sel;
					down_button_pressed=true;
				};

				if(up_button_pressed && !(buttons & BTN_UP)) up_button_pressed=false;
				if(down_button_pressed && !(buttons & BTN_DOWN)) down_button_pressed=false;
				if(jump_button_pressed && !(buttons & BTN_B)) jump_button_pressed=false;
				pause_anim_counter++;
				pause_anim_counter%=192;

				map->jr_map::draw(buffer,0,0);
				draw(buffer);
				map->jr_map::draw_fg_layer(buffer,0,0);
				draw_panel(buffer);
				boxRGBA(buffer,0,0,320,240,0,0,0,127);

				BlitSurface(pause_image,pause_image->w/3*(pause_anim_counter/64),0,pause_image->w/3,pause_image->h,buffer,135,70);

				if(continue_sel)
				{
					BlitSurface(continue_pause,continue_pause->w/2,0,continue_pause->w/2,continue_pause->h,buffer,130,110);
					BlitSurface(quit_pause,0,0,quit_pause->w/2,quit_pause->h,buffer,144,130);
				}
				else
				{
					BlitSurface(continue_pause,0,0,continue_pause->w/2,continue_pause->h,buffer,130,110);
					BlitSurface(quit_pause,quit_pause->w/2,0,quit_pause->w/2,quit_pause->h,buffer,144,130);
				};

				SDL_Flip(buffer);
				limit_frames(60);

			}while(!(buttons & BTN_B) || jump_button_pressed);

			map->paused=false;
			return continue_sel;
		};

		bool show_inventory(SDL_Surface *buffer) //Return value is heal of not ;-);
		{
			bool menu_button_pressed=true;
			bool left_pressed=false;
			bool right_pressed=false;
			bool jump_pressed=true;
			int offset=0;
			int selection=0;
			SDL_Rect inventory_rect;
			int buttons;

			// hackfix
			map->jr_map::draw(buffer,0,0);
			draw(buffer);
			map->jr_map::draw_fg_layer(buffer,0,0);
			draw_panel(buffer);

			do
			{
				buttons=get_button_state();

				if(!left_pressed && (buttons & BTN_LEFT))
				{
					left_pressed=true;
					if(selection==0)
					{
						if(offset>0)
						{
							offset--;
						};
					}
					else
					{
						selection--;
					};
				};
				if(!right_pressed && (buttons & BTN_RIGHT))
				{
					right_pressed=true;
					if(selection==7)
					{
						if(selection+offset+1<inventory.size())
						{
							offset++;
						};
					}
					else
					{
						if(selection+offset+1<inventory.size())
						{
							selection++;
						};
					};
				};

				if((buttons & BTN_B) && !jump_pressed && selection+offset<inventory.size())
				{
					if(environment->item_types[inventory[selection+offset]].item_action==ITEM_ACTION_HEAL)
					{
						Mix_PlayChannel(-1,heal_sound,0);
						life+=environment->item_types[inventory[selection+offset]].value;
						if(life>max_life)
						{
							life=max_life;
						};
						if(environment->item_types[inventory[selection+offset]].vanishing)
						{
							inventory.erase(inventory.begin()+selection+offset);
						};
						return true;
					};
					if(environment->item_types[inventory[selection+offset]].item_action==ITEM_ACTION_ELEMENT_EARTH)
					{
						element_id=ELEMENT_EARTH;
						element_time_counter=0;
						image=image_earth;
						image_left=image_earth_left;
						if(environment->item_types[inventory[selection+offset]].vanishing)
						{
							inventory.erase(inventory.begin()+selection+offset);
						};
						return false;
					};
					if(environment->item_types[inventory[selection+offset]].item_action==ITEM_ACTION_ELEMENT_WIND)
					{
						element_id=ELEMENT_WIND;
						element_time_counter=0;
						transformated=false;
						image=image_wind;
						image_left=image_wind_left;
						if(environment->item_types[inventory[selection+offset]].vanishing)
						{
							inventory.erase(inventory.begin()+selection+offset);
						};
						return false;
					};
					if(environment->item_types[inventory[selection+offset]].item_action==ITEM_ACTION_ELEMENT_FIRE)
					{
						element_id=ELEMENT_FIRE;
						element_time_counter=0;
						image=image_fire;
						image_left=image_fire_left;
						if(environment->item_types[inventory[selection+offset]].vanishing)
						{
							inventory.erase(inventory.begin()+selection+offset);
						};
						return false;
					};
					if(environment->item_types[inventory[selection+offset]].item_action==ITEM_ACTION_ELEMENT_ICE)
					{
						element_id=ELEMENT_ICE;
						element_time_counter=0;
						image=image_ice;
						image_left=image_ice_left;
						if(environment->item_types[inventory[selection+offset]].vanishing)
						{
							inventory.erase(inventory.begin()+selection+offset);
						};
						return false;
					};
				};

				if(left_pressed && !(buttons & BTN_LEFT)) left_pressed=false;
				if(right_pressed && !(buttons & BTN_RIGHT)) right_pressed=false;
				if(jump_pressed && !(buttons & BTN_B)) jump_pressed=false;
				if(menu_button_pressed && !(buttons & BTN_Y)) menu_button_pressed=false;

				BlitSurface(item_txt,0,0,item_txt->w,item_txt->h,buffer,160-item_txt->w/2,70-item_txt->h);

				BlitSurface(item_box,0,0,item_box->w,item_box->h,buffer,89,77);
				inventory_rect.x=92+selection*17;
				inventory_rect.y=80;
				inventory_rect.w=17;
				inventory_rect.h=1;
				SDL_FillRect(buffer,&inventory_rect,SDL_MapRGB(buffer->format,255,255,255));
				inventory_rect.y=96;
				SDL_FillRect(buffer,&inventory_rect,SDL_MapRGB(buffer->format,255,255,255));
				inventory_rect.y=80;
				inventory_rect.h=17;
				inventory_rect.w=1;
				SDL_FillRect(buffer,&inventory_rect,SDL_MapRGB(buffer->format,255,255,255));
				inventory_rect.x+=16;
				SDL_FillRect(buffer,&inventory_rect,SDL_MapRGB(buffer->format,255,255,255));
				inventory_rect.x=105;
				inventory_rect.y=110;
				inventory_rect.w=110;
				inventory_rect.h=10;
				SDL_FillRect(buffer,&inventory_rect,SDL_MapRGB(buffer->format,0,0,0));
				if(inventory.size()>0)
				{
					stringRGBA(buffer,160-environment->item_types[inventory[selection+offset]].txt.size()*5/2,112,environment->item_types[inventory[selection+offset]].txt.c_str(),255,255,255,255);
				}
				else
				{
					stringRGBA(buffer,142,112,"No Item",255,255,255,255);
				};

				int tmp_max=8;
				if(inventory.size()<8) tmp_max=inventory.size();
				for(int i=0;i<tmp_max;++i)
				{
					BlitSurface(environment->item_sets[environment->item_types[inventory[offset+i]].set_id].image,environment->item_sets[environment->item_types[inventory[offset+i]].set_id].size_x*environment->item_types[inventory[offset+i]].tile_id,0,environment->item_sets[environment->item_types[inventory[offset+i]].set_id].size_x,environment->item_sets[environment->item_types[inventory[offset+i]].set_id].size_y,buffer,93+i*17,81);
				};

				SDL_Flip(buffer);
				limit_frames(60);
			}while(!(buttons & BTN_Y) || menu_button_pressed);

			return false;
		};

		void show_death_animation(SDL_Surface *buffer)
		{
			anim_state=0;
			SDL_Surface *death_ani=LoadImage("images/player_dying.png");
			SDL_SetColorKey(death_ani,SDL_SRCCOLORKEY,SDL_MapRGB(death_ani->format,255,255,255));
			do
			{
				map->draw(buffer,0,0);
				BlitSurface(death_ani,(anim_state/32)*(death_ani->w/4),(death_ani->h/2)*walking_direction,death_ani->w/4,death_ani->h/2,buffer,pos_x,pos_y+downwards_correction_value);
				map->draw_fg_layer(buffer,0,0);
				draw_panel(buffer);

				SDL_Flip(buffer);
				limit_frames(60);

				increase_animation_counter();
			}while(anim_state<127-default_movespeed);
			SDL_FreeSurface(death_ani);
		};

		int update(int buttons, SDL_Surface *buffer)
		{
			map->update_clouds();
			map->reset_scroll_counter();
			update_snowballs();


			if(buttons & BTN_MENU)
			{
				if(!pause(buffer))
				{
					return UPDATE_GAME_OVER;
				}
				else
				{
					jumped=true; //Blob should not jump only because someone left the pause menu ;-);
				};
			};

			if(element_id!=NO_ELEMENT)
			{
				element_time_counter+=def_speed;
				if(element_time_counter>5000)
				{
					element_id=NO_ELEMENT;
					element_time_counter=0;
					image=image_default;
					image_left=image_default_left;
				};
			};
			if(pos_y_on_map+player_size_y>=mapsize_y)
			{
				life=0;
				one_ups=0;
			};
			if(life<=0)
			{
				if(element_id!=NO_ELEMENT)
				{
					element_id=NO_ELEMENT;
					element_time_counter=0;
					image=image_default;
					image_left=image_default_left;
				};

				if(one_ups>1)
				{
					life=max_life;
					one_ups--;
				}
				else
				{
					show_death_animation(buffer);
					if(game_over_continue())
					{
						if(passage_map) return UPDATE_CONTINUE_PREVIOUS;
						reset();
					}
					else
					{
						return UPDATE_GAME_OVER;
					};
				};
			};
			if((!map_end_set && pos_x_on_map+player_size_x+20>=mapsize_x) && !(boss_battle && map->enemys_remaining()))
			{
				return UPDATE_FINISHED;
			}
			else
			{
				if(map_end_set && !(boss_battle && map->enemys_remaining()))
				{
					int tmp_dist_x=pos_x_on_map>map_end_x?pos_x_on_map-map_end_x:map_end_x-pos_x_on_map;
					int tmp_dist_y=pos_y_on_map>map_end_y?pos_y_on_map-map_end_y:map_end_y-pos_y_on_map;
					if(tmp_dist_x<30 && tmp_dist_y<30)
					{
						return UPDATE_FINISHED;
					};
				};
			};
			if(attached_liana!=-1)
			{
				update_liana(buttons);
				return UPDATE_CONTINUE;
			}
			else
			{
				reset_lianas();
			};

			if(((buttons & BTN_UP) || (buttons & BTN_DOWN)) && !(buttons & BTN_B) && attached_ladder==-1)
			{
				int tmp_ladder_id;
				SDL_Surface *tmp_img;
				int tmp_state=state;
				if(element_id==ELEMENT_WIND)
				{
					tmp_state=PLAYER_STATE_IDLE;
				};
				if(walking_direction==PLAYER_DIRECTION_RIGHT)
				{
					tmp_img=image;
				}
				else
				{
					tmp_img=image_left;
				};
				if(map->ladder_collides(tmp_img,pos_x_on_map,pos_y_on_map,player_size_x,player_size_y-downwards_correction_value,tmp_state,anim_state,tmp_ladder_id))
				{
					attach_to_ladder(tmp_ladder_id);
				};
			};

			if(attached_ladder!=-1)
			{
				update_ladder(buttons);
				return UPDATE_CONTINUE;
			};

			if((buttons & BTN_Y) && !inventory_shown)
			{
				if(show_inventory(buffer))
				{
					anim_state=0;
					state=PLAYER_STATE_HEALING;
				};
				inventory_shown=true;
			};
			if(inventory_shown && !(buttons & BTN_Y))
			{
				inventory_shown=false;
			};

			if(state==PLAYER_STATE_HEALING)
			{
				if(map->shaking)
				{
					map->shaking=false;
					shake_time_counter=0;
				};
				if(anim_state==127)
				{
					state=PLAYER_STATE_IDLE;
					anim_state=0;
				};

				increase_animation_counter();
				return UPDATE_CONTINUE;
			};

			int tmp_damage, tmp_state, tmp_item;
			if(state==PLAYER_STATE_ATTACK)
			{
				if(collides(PLAYER_DIRECTION_DOWN,1))
				{
					tmp_state=PLAYER_STATE_WALKING;
				}
				else
				{
					tmp_state=PLAYER_STATE_ATTACK;
				};
			}
			else
			{
				if(punshing)
				{
					tmp_state=PLAYER_STATE_PUNSHING;
				}
				else
				{
					if(falling)
					{
						tmp_state=PLAYER_STATE_FALLING;
					}
					else
					{
						tmp_state=state;
					};
				};
			};
			if(element_id==ELEMENT_WIND)
			{
				tmp_state=PLAYER_STATE_IDLE;
			};

			forced_jumping=false;
			bool reset_elem=false;
			if(walking_direction==PLAYER_DIRECTION_RIGHT)
			{
				tmp_item=map->update_items(image,pos_x_on_map,pos_y_on_map,player_size_x,player_size_y,tmp_state,anim_state/32);
				tmp_damage=map->update_enemys(image,pos_x_on_map,pos_y_on_map,player_size_x, player_size_y,tmp_state,falling,falling_state-default_movespeed,strength, anim_state/32,element_id, hit_direction, forced_jumping, boss_battle, reset_elem);
			}
			else
			{
				tmp_item=map->update_items(image_left,pos_x_on_map,pos_y_on_map,player_size_x,player_size_y,tmp_state,anim_state/32);
				tmp_damage=map->update_enemys(image_left,pos_x_on_map,pos_y_on_map,player_size_x, player_size_y,tmp_state,falling,falling_state-default_movespeed,strength, anim_state/32,element_id,hit_direction,forced_jumping, boss_battle,reset_elem);
			};

			if(map->shaking)
			{
				shake_time_counter+=default_movespeed;
				if(shake_time_counter>shake_time)
				{
					map->shaking=false;
					shake_time_counter=0;
				};
			};

			if(boss_battle && forced_jumping && environment->bg_image_filename=="images/beating_jungle/lavly_bg.png")
			{
				forced_jumping=false;
				move_to_pos(0,pos_y_on_map);
				map->bg_image_scroll_x=0;
			};


			if(reset_elem)
			{
				element_id=NO_ELEMENT;
				element_time_counter=0;
				image=image_default;
				image_left=image_default_left;
			};

			if(tmp_item>=0)
			{
				Mix_PlayChannel(-1,got_item_sound,0);
				inventory.push_back(tmp_item);
			}
			else
			{
				if(tmp_item==-1)
				{
					Mix_PlayChannel(-1,one_up_sound,0);
					if(one_ups<5) one_ups++;
				};
			};

			if(state!=PLAYER_STATE_HIT && tmp_damage!=0)
			{
				defense-=tmp_damage;
				if(defense<0)
				{
					Mix_PlayChannel(-1,hit_sound,0);
					if(walking_state==PLAYER_WALKING_STATE_RUNNING)
					{
						walking_state=PLAYER_WALKING_STATE_NORMAL;
						default_movespeed/=2;
					};
					state=PLAYER_STATE_HIT;
					anim_state=0;
					falling=false;
					punshing=false;
					if(!element_id==ELEMENT_EARTH)
					{
						life+=defense;
					}
					else
					{
						if(defense+strength<0)
						{
							life+=(defense+strength);
						};
					};
					defense=0;
					return UPDATE_CONTINUE;
				};
			}
			else
			{
				if(state==PLAYER_STATE_HIT)
				{
					if(map->shaking) map->shaking=false;
					int tmpspeed=0;
					if(anim_state<24)
					{
						tmpspeed=6;
					}
					else
					{
						if(anim_state<48)
						{
							tmpspeed=4;
						}
						else
						{
							if(anim_state<126)
							{
								tmpspeed=2;
							}
							else
							{
								state=PLAYER_STATE_IDLE;
								anim_state=0;
							};
						};
					};

					if(tmpspeed>0 && anim_state%3==0)
					{
						int tmpspeed_up=tmpspeed-1;
						if(tmpspeed_up<0 || pos_y_on_map-tmpspeed_up<=0) tmpspeed_up=0;
						pos_y_on_map-=tmpspeed_up;
						if(/*map->tile_scroll_y-upspeed<=0 ||*/ pos_y>=120-player_size_y || map->map_size_y==24)
						{
							pos_y-=tmpspeed_up;
						}
						else
						{
							map->scroll(2,tmpspeed_up);
						};

						if(hit_direction==DIRECTION_LEFT)
						{
							while(collides(PLAYER_DIRECTION_LEFT,tmpspeed)) tmpspeed--;
							if(pos_x_on_map>0)
							{
								if(map->tile_scroll_x>=0 || pos_x>160-player_size_x/2)
								{
									pos_x-=tmpspeed;
									pos_x_on_map-=tmpspeed;
								}
								else
								{
									pos_x_on_map-=tmpspeed;
									map->scroll(1,tmpspeed);
								};
							};
						}
						else
						{
							while(collides(PLAYER_DIRECTION_RIGHT,tmpspeed)) tmpspeed--;
							if(pos_x_on_map+player_size_x<mapsize_x)
							{
								if(map->tile_scroll_x<=-(mapsize_x-320) || pos_x<160-player_size_x/2)
								{
									pos_x+=tmpspeed;
									pos_x_on_map+=tmpspeed;
								}
								else
								{
									pos_x_on_map+=tmpspeed;
									map->scroll(0,tmpspeed);
								};
							};
						};
					};

					increase_animation_counter();
					increase_animation_counter();
					return UPDATE_CONTINUE;
				};
			};

			if(((buttons & BTN_A) || state==PLAYER_STATE_ATTACK) && !punshing && !moved && element_id!=ELEMENT_WIND)
			{
				if(state!=PLAYER_STATE_ATTACK)
				{
					if(walking_state==PLAYER_WALKING_STATE_RUNNING)
					{
						walking_state=PLAYER_WALKING_STATE_NORMAL;
						default_movespeed/=2;
					};
					state=PLAYER_STATE_ATTACK;
					falling=false;
					anim_state=0;
					attack_delay_counter=0;
					if(collides(PLAYER_DIRECTION_DOWN,1))
					{
						defense=strength;
					};
				}
				else
				{
					if(defense<=0 && !collides(PLAYER_DIRECTION_DOWN,1) && !moved)
					{
						int drop_speed=4;
						if(anim_state%3==0) drop_speed=0;
						while(drop_speed!=0 && collides(PLAYER_DIRECTION_DOWN,drop_speed)) drop_speed--;
						pos_y_on_map+=drop_speed;
						if((-(map->tile_scroll_y)+drop_speed>=mapsize_y-240) || pos_y<=160 || map->map_size_y==24)
						{
							pos_y+=drop_speed;
						}
						else
						{
							map->scroll(3,drop_speed);
						};
					};

					if(collides(PLAYER_DIRECTION_DOWN,1) && !moved)
					{
						if(!attack_sound_played && attack_delay_counter==0 && defense<=0)
						{
							Mix_PlayChannel(-1,attack_sound,0);
							attack_sound_played=true;
							if(anim_state==127)
							{
								map->shaking=true;
								shake_time_counter=0;
							};
						};

						if(!(buttons & BTN_A))
						{
							attack_delay_counter+=default_movespeed;
							if(attack_delay_counter>attack_delay)
							{
								state=PLAYER_STATE_IDLE;
								anim_state=0;
								defense=0;
								attack_sound_played=false;
							};
						};
					};
				};

				increase_animation_counter();
				increase_animation_counter();
				increase_animation_counter();
				return UPDATE_CONTINUE;
			};

			if(punshing)
			{
				int tmpspeed=0;
				if(anim_state<30)
				{
					tmpspeed=6;
				}
				else
				{
					if(anim_state<60)
					{
						tmpspeed=4;
					}
					else
					{
						if(anim_state<126)
						{
							tmpspeed=2;
						}
						else
						{
							anim_state=0;
							punshing=false;
							if(element_id==ELEMENT_ICE)
							{
								map->throw_snowball(pos_x_on_map,pos_y_on_map,walking_direction,snowball_image);
							};
						};
					};
				};

				if(tmpspeed>0 && anim_state%3==0)
				{
					int tmpspeed_up=tmpspeed-4;
					if(tmpspeed_up<0) tmpspeed_up=0;
					pos_y_on_map-=tmpspeed_up;
					if(/*map->tile_scroll_y-upspeed<=0 ||*/ pos_y>=120-player_size_y || map->map_size_y==24)
					{
						pos_y-=tmpspeed_up;
					}
					else
					{
						map->scroll(2,tmpspeed_up);
					};

					if(walking_direction==PLAYER_DIRECTION_LEFT)
					{
						while(collides(PLAYER_DIRECTION_LEFT,tmpspeed)) tmpspeed--;
						if(pos_x_on_map>0)
						{
							if(map->tile_scroll_x+tmpspeed>=0 || pos_x>160-player_size_x/2)
							{
								pos_x-=tmpspeed;
								pos_x_on_map-=tmpspeed;
							}
							else
							{
								pos_x_on_map-=tmpspeed;
								map->scroll(1,tmpspeed);
							};
						};
					}
					else
					{
						while(collides(PLAYER_DIRECTION_RIGHT,tmpspeed)) tmpspeed--;
						if(pos_x_on_map+player_size_x<mapsize_x)
						{
							if(map->tile_scroll_x-tmpspeed<=-(mapsize_x-320) || pos_x<160-player_size_x/2)
							{
								pos_x+=tmpspeed;
								pos_x_on_map+=tmpspeed;
							}
							else
							{
								pos_x_on_map+=tmpspeed;
								map->scroll(0,tmpspeed);
							};
						};
					};
				};

				increase_animation_counter();
				increase_animation_counter();
				return UPDATE_CONTINUE;
			};

			if(!(buttons & BTN_B) || forced_jumping) jumped=false;
			if(walking_state==PLAYER_WALKING_STATE_NORMAL)
			{
				if(((buttons & BTN_L) && collides(PLAYER_DIRECTION_DOWN,1)) || element_id==ELEMENT_WIND)
				{
					walking_state=PLAYER_WALKING_STATE_RUNNING;
					default_movespeed*=2;
				};
			}
			else
			{
				if(!(buttons & BTN_L) && element_id!=ELEMENT_WIND)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
				};
			};

			int last_state=state;
			state=PLAYER_STATE_IDLE;
 			int movespeed=default_movespeed;

			if((buttons & BTN_X) && !punshing && !punshed && !moved && element_id!=ELEMENT_WIND  && element_id!=ELEMENT_FIRE)
			{
				Mix_PlayChannel(-1,punsh_sound,0);
				punshing=true;
				punshed=true;
				anim_state=0;
				falling=false;
				state=PLAYER_STATE_IDLE;
				last_state=PLAYER_STATE_IDLE;

				if(walking_state==PLAYER_WALKING_STATE_RUNNING)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
				};
			};

			if(punshed && !(buttons & BTN_X) && collides(PLAYER_DIRECTION_DOWN,1))
			{
				punshed=false;
			};

			if((buttons & BTN_RIGHT) &&!moved)
			{
				if(walking_state==PLAYER_WALKING_STATE_RUNNING && falling)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
					movespeed/=2;
				};

				while(collides(PLAYER_DIRECTION_RIGHT,movespeed) && movespeed>0) movespeed--;
				if(pos_x_on_map+player_size_x<mapsize_x)
				{
					if(map->tile_scroll_x-movespeed<=-(mapsize_x-320) || pos_x+movespeed<=160-player_size_x/2)
					{
						pos_x+=movespeed;
						pos_x_on_map+=movespeed;
					}
					else
					{
						pos_x_on_map+=movespeed;
						map->scroll(0,movespeed);
					};

					state=PLAYER_STATE_WALKING;
					walking_direction=PLAYER_DIRECTION_RIGHT;
					if(last_state!=PLAYER_STATE_WALKING && last_state!=PLAYER_STATE_JUMPING)
					{
						anim_state=0;
					};
				};
			};
			if((buttons & BTN_LEFT) && !moved)
			{
				if(walking_state==PLAYER_WALKING_STATE_RUNNING && falling)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
					movespeed/=2;
				};

				while(collides(PLAYER_DIRECTION_LEFT,movespeed) && movespeed>0) movespeed--;
				if(pos_x_on_map>0)
				{
					if(map->tile_scroll_x+movespeed>=0 || pos_x-movespeed>=160-player_size_x/2)
					{
						pos_x-=movespeed;
						pos_x_on_map-=movespeed;
					}
					else
					{
						pos_x_on_map-=movespeed;
						map->scroll(1,movespeed);
					};

					state=PLAYER_STATE_WALKING;
					walking_direction=PLAYER_DIRECTION_LEFT;
					if(last_state!=PLAYER_STATE_WALKING && last_state!=PLAYER_STATE_JUMPING)
					{
						anim_state=0;
					};
				};
			};

			moved=false;
			moved_on_platform=false;
			int falling_speed=0;
			if(falling)
			{
				if(walking_state==PLAYER_WALKING_STATE_RUNNING)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
				};

				if(falling_state<16)
				{
					falling_speed=1;
				}
				else
				{
					if(falling_state<36)
					{
						falling_speed=2;
					}
					else
					{
						falling_speed=3;
					};
				};
				if(falling_state%2!=0) falling_speed=0;
			};
			while(falling_speed>0 && collides(PLAYER_DIRECTION_DOWN,falling_speed)) falling_speed--; 
			if(last_state!=PLAYER_STATE_JUMPING && !collides(PLAYER_DIRECTION_DOWN,1))
			{

				falling=true;
				pos_y_on_map+=falling_speed;
				if((-(map->tile_scroll_y)+falling_speed>=mapsize_y-240) || pos_y<=160 || map->map_size_y==24)
				{
					pos_y+=falling_speed;
				}
				else
				{
					map->scroll(3,falling_speed);
				};
			}
			else
			{
				falling=false;
				falling_state=0;
			};

			if(((buttons & BTN_B) && (collides(PLAYER_DIRECTION_DOWN,1) || forced_jumping) && !jumped))
			{
				Mix_PlayChannel(-1,jump_sound,0);
				if(forced_jumping)
				{
					jumpstate=2*default_movespeed;
					anim_state=63;
				}
				else
				{
					anim_state=0;
					jumpstate=0;
				};
				state=PLAYER_STATE_JUMPING;
				jumped=true;
				if(last_state!=PLAYER_STATE_WALKING && walking_state==PLAYER_WALKING_STATE_RUNNING)
				{
					walking_state=PLAYER_WALKING_STATE_NORMAL;
					default_movespeed/=2;
				};
			};

			if((last_state==PLAYER_STATE_JUMPING || forced_jumping))
			{
				state=PLAYER_STATE_JUMPING;
				int upspeed=0;
				if(jumpstate>0)
				{
					if(jumpstate<30 && jumpstate%2==0)
					{
						upspeed=3;
						if(jumpstate==1 && !forced_jumping && !collides(PLAYER_DIRECTION_DOWN,1))
						{
							anim_state=0;
							jumpstate=0;
							upspeed=0;
							state=PLAYER_STATE_IDLE;
						};
					}
					else
					{
						if(jumpstate<50 && jumpstate%2==0)
						{
							upspeed=2;
						}
						else
						{
		
							if(jumpstate<66 && jumpstate%2==0)
							{
 								upspeed=1;
							}
							else
							{
								if(jumpstate>66)
								{
									anim_state=0;
									jumpstate=0;
									forced_jumping=false;
									state=PLAYER_STATE_IDLE;
								};
							};
						};
					};
				};

				if(state==PLAYER_STATE_JUMPING)
				{

					while(pos_y_on_map-upspeed<0 && upspeed>0) upspeed--;
					pos_y_on_map-=upspeed;
					if(map->tile_scroll_y+upspeed>=0 || pos_y>=120-player_size_y || map->map_size_y==24)
					{
						pos_y-=upspeed;
					}
					else
					{
						map->scroll(2,upspeed);
					};

					if(!(buttons & BTN_B))
					{
						anim_state=0;
						jumpstate=0;
						state=PLAYER_STATE_IDLE;
					};
				};
			};

			if(state==PLAYER_STATE_IDLE && last_state!=PLAYER_STATE_IDLE)
			{
				anim_state=0;
			};

			increase_animation_counter();
			return UPDATE_CONTINUE;
		};
};
