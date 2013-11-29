class jr_dev_environment: public jr_environment
{
	public:
		void set_tileset(std::string tiles_filename,int tsize_x, int tsize_y)
		{
			if(tileset!=NULL) SDL_FreeSurface(tileset);
			tileset=LoadImage(tiles_filename.c_str());
			SDL_SetColorKey(tileset,SDL_SRCCOLORKEY,SDL_MapRGB(tileset->format,255,255,255));
			tileset_filename=tiles_filename;
			tile_size_x=tsize_x;
			tile_size_y=tsize_y;
		};

		void set_bg_image(std::string bgimage_filename)
		{
			if(bg_image!=NULL) SDL_FreeSurface(bg_image);
			bg_image=LoadImage(bgimage_filename.c_str());
			bg_image_filename=bgimage_filename;
		};

		void set_bg_cloud_image(std::string cloudimage_filename)
		{
			if(bg_cloud_image!=NULL) SDL_FreeSurface(bg_cloud_image);
			bg_cloud_image=LoadImage(cloudimage_filename.c_str());
			bg_cloud_image_filename=cloudimage_filename;
		};

		std::string get_tileset_filename()
		{
			return tileset_filename;
		};

		void save_to_file(std::string filename)
		{
			std::ofstream outfile;
			outfile.open(filename.c_str(),std::ios_base::binary);

			int tmp;
			char tmp_char;
			bool tmp_bool;

			tmp=tileset_filename.size();
			outfile.write((char*)&tmp,sizeof(int));

			for(int i=0;i<tmp;++i)
			{
				tmp_char=tileset_filename[i];
				outfile.write((char*)&tmp_char,sizeof(char));
			};

			tmp=tile_size_x;
			outfile.write((char*)&tmp,sizeof(int));
			tmp=tile_size_y;
			outfile.write((char*)&tmp,sizeof(int));

			tmp_bool=(bg_image!=NULL);
			outfile.write((char*)&tmp_bool,sizeof(bool));

			if(tmp_bool)
			{
				tmp=bg_image_filename.size();
				outfile.write((char*)&tmp,sizeof(int));

				for(int i=0;i<tmp;++i)
				{
					tmp_char=bg_image_filename[i];
					outfile.write((char*)&tmp_char,sizeof(char));
				};
			};

			tmp_bool=(bg_cloud_image!=NULL);
			outfile.write((char*)&tmp_bool,sizeof(bool));

			if(tmp_bool)
			{
				tmp=bg_cloud_image_filename.size();
				outfile.write((char*)&tmp,sizeof(int));

				for(int i=0;i<tmp;++i)
				{
					tmp_char=bg_cloud_image_filename[i];
					outfile.write((char*)&tmp_char,sizeof(char));
				};
			};

			//animation_sets ;-);
			tmp=animation_sets.size();
			outfile.write((char*)&tmp,sizeof(int));

			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=animation_sets[i].image_filename.size();
				outfile.write((char*)&tmp_int,sizeof(int));

				for(int j=0;j<tmp_int;++j)
				{
					tmp_char=animation_sets[i].image_filename[j];
					outfile.write((char*)&tmp_char,sizeof(char));
				};

				tmp_int=animation_sets[i].size_x;
				outfile.write((char*)&tmp_int,sizeof(int));
				tmp_int=animation_sets[i].size_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=animation_sets[i].number_of_states;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=animation_sets[i].number_of_animations;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//item_sets ;-);
			tmp=item_sets.size();
			outfile.write((char*)&tmp,sizeof(int));

			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=item_sets[i].filename.size();
				outfile.write((char*)&tmp_int,sizeof(int));

				for(int j=0;j<tmp_int;++j)
				{
					tmp_char=item_sets[i].filename[j];
					outfile.write((char*)&tmp_char,sizeof(char));
				};

				tmp_int=item_sets[i].number_of_items;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_sets[i].number_of_animations;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_sets[i].size_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_sets[i].size_y;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//item_types ;-);
			tmp=item_types.size();
			outfile.write((char*)&tmp,sizeof(int));

			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=item_types[i].txt.size();
				outfile.write((char*)&tmp_int,sizeof(int));

				for(int j=0;j<tmp_int;++j)
				{
					tmp_char=item_types[i].txt[j];
					outfile.write((char*)&tmp_char,sizeof(char));
				};

				tmp_int=item_types[i].set_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_types[i].tile_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_types[i].item_action;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=item_types[i].value;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_bool=item_types[i].activation_type;
				outfile.write((char*)&tmp_bool,sizeof(bool));

				tmp_bool=item_types[i].vanishing;
				outfile.write((char*)&tmp_bool,sizeof(bool));
			};

			//enemy_types ;-);
			tmp=enemy_types.size();
			outfile.write((char*)&tmp,sizeof(int));

			for(int i=0;i<tmp;++i)
			{
				int tmp_int=enemy_types[i].image_filename.size();
				outfile.write((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					tmp_char=enemy_types[i].image_filename[j];
					outfile.write((char*)&tmp_char,sizeof(char));
				};

				tmp_int=enemy_types[i].image_filename_right.size();
				outfile.write((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					tmp_char=enemy_types[i].image_filename_right[j];
					outfile.write((char*)&tmp_char,sizeof(char));
				};

				tmp_int=enemy_types[i].size_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].size_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].downwards_correction_value;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].forwards_correction_value;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].throwspeed_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].throwspeed_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].throw_freq_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].throw_freq_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].attacking_pattern;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].movespeed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].move_frequency;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].attacking_frequency;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].animation_speed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].default_resistance;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemy_types[i].hit_damage;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_bool=enemy_types[i].throwing_type;
				outfile.write((char*)&tmp_bool,sizeof(bool));

				tmp_bool=enemy_types[i].flying_type;
				outfile.write((char*)&tmp_bool,sizeof(bool));

				tmp_bool=enemy_types[i].allowes_jumping;
				outfile.write((char*)&tmp_bool,sizeof(bool));
			};

			outfile.close();
		};
};

class jr_dev_map: public jr_map
{
	private:
		SDL_Surface *player_image;

	public:
		jr_dev_map(std::string player_image_filename,jr_environment *env)
		{
			environment=env;
			player_image=LoadImage(player_image_filename.c_str());
			SDL_SetColorKey(player_image,SDL_SRCCOLORKEY,SDL_MapRGB(player_image->format,255,255,255));
			tile_scroll_x=0;
			tile_scroll_y=0;
			bg_image_scroll_x=0;
			bg_image_scroll_y=0;
		};

		void create(int size_x, int size_y)
		{
			bg_tiles.reserve(size_y);
			tiles.reserve(size_y);
			fg_tiles.reserve(size_y);
			for(int i=0;i<size_y;++i)
			{
				std::vector<int> tmp;
				tmp.reserve(size_x);
				for(int j=0;j<size_x;++j)
					tmp.push_back(0);
				bg_tiles.push_back(tmp);
				tiles.push_back(tmp);
				fg_tiles.push_back(tmp);
			};
		};

		bool save_to_file(std::string filename)
		{
			replace_transparent_tiles();
			std::ofstream outfile;
			outfile.open(filename.c_str(),std::ios_base::binary);

			outfile.write((char*)&player_starting_pos_x,sizeof(int));
			outfile.write((char*)&player_starting_pos_y,sizeof(int));

			int tmp;

			tmp=tiles[0].size(); //size_x;
			outfile.write((char*)&tmp,sizeof(int));
			tmp=tiles.size(); //size_y;
			outfile.write((char*)&tmp,sizeof(int));

			for(int y=0;y<bg_tiles.size();++y)
			{
				for(int x=0;x<bg_tiles[0].size();++x)
				{
					tmp=bg_tiles[y][x];
					outfile.write((char*)&tmp,sizeof(int));
				};
			};

			for(int y=0;y<tiles.size();++y)
			{
				for(int x=0;x<tiles[0].size();++x)
				{
					tmp=tiles[y][x];
					outfile.write((char*)&tmp,sizeof(int));
				};
			};

			for(int y=0;y<fg_tiles.size();++y)
			{
				for(int x=0;x<fg_tiles[0].size();++x)
				{
					tmp=fg_tiles[y][x];
					outfile.write((char*)&tmp,sizeof(int));
				};
			};

			//animation_instances ;-);

			//bg_anims ;-);
			tmp=bg_anims.size();
			outfile.write((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=bg_anims[i].set_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].tile_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].anim_speed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].movespeed_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].movespeed_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].movespeed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].starting_pos_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=bg_anims[i].starting_pos_y;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//anims ;-);
			tmp=anims.size();
			outfile.write((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=anims[i].set_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].tile_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].anim_speed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].movespeed_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].movespeed_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].movespeed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].starting_pos_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=anims[i].starting_pos_y;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//fg_anims ;-);
			tmp=fg_anims.size();
			outfile.write((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=fg_anims[i].set_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].tile_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].anim_speed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].movespeed_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].movespeed_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].movespeed;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].starting_pos_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=fg_anims[i].starting_pos_y;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//items ;-);
			tmp=items.size();
			outfile.write((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				tmp_int=items[i].type_id;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=items[i].pos_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=items[i].pos_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=items[i].anim_speed;
				outfile.write((char*)&tmp_int,sizeof(int));
			};

			//enemys ;-);
			tmp=enemys.size();
			outfile.write((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				bool tmp_bool;
				tmp_int=enemys[i].type;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemys[i].pos_x;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_int=enemys[i].pos_y;
				outfile.write((char*)&tmp_int,sizeof(int));

				tmp_bool=enemys[i].walking_direction;
				outfile.write((char*)&tmp_bool,sizeof(bool));
			};

			outfile.close();
		};
		
		void set_tile(int layer, int x, int y, int value) //layer: 0=bg,1=normal,2=fg ;-);
		{
			if(x>=tiles[0].size() || x<0 || y>=tiles.size() || y<0) return;
			switch(layer)
			{
				case 0:
				{
					bg_tiles[y][x]=value;
					break;
				};
				case 1:
				{
					tiles[y][x]=value;
					break;
				};
				case 2:
				{
					fg_tiles[y][x]=value;
					break;
				};
			};
		};

		void fill_layer_with_tile(int layer, int value)
		{
			for(int y=0;y<tiles.size();++y)
			{
				for(int x=0;x<tiles[0].size();++x)
				{
					set_tile(layer,x,y,value);
				};
			};
		};

		bool tile_is_transparent(int tile_id)
		{
			Uint32 colorkey_val=SDL_MapRGB(environment->tileset->format,255,255,255);
			SDL_LockSurface(environment->tileset);
			for(int y=0;y<environment->tile_size_y;++y)
			{
				for(int x=0;x<environment->tile_size_x;++x)
				{
					if(get_pixel32(environment->tileset,(tile_id*environment->tile_size_x)%environment->tileset->w+x,((tile_id*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y+y)!=colorkey_val)
					{
						SDL_UnlockSurface(environment->tileset);
						return false;
					};
				};
			};
			SDL_UnlockSurface(environment->tileset);
			return true;
		};

		void replace_tile(int tile_id, int new_tile_id)
		{
			for(int y=0;y<map_size_y;++y)
			{
				for(int x=0;x<map_size_x;++x)
				{
					if(bg_tiles[y][x]==tile_id) bg_tiles[y][x]=new_tile_id;
					if(tiles[y][x]==tile_id) tiles[y][x]=new_tile_id;
					if(fg_tiles[y][x]==tile_id) fg_tiles[y][x]=new_tile_id;
				};
			};
		};

		void replace_transparent_tiles()
		{
			int num_of_tiles=(environment->tileset->h/environment->tile_size_y)*(environment->tileset->w/environment->tile_size_x);
			for(int i=0;i<num_of_tiles;++i)
			{
				if(tile_is_transparent(i))
				{
					replace_tile(i+1,0);
				};
			};
		};

		void draw_bg_image(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			if(environment->bg_image!=NULL) 
			{
				BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%640,draw_y+bg_image_scroll_y-240);
				if(bg_image_scroll_x<0)
				{
					BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%640+640,draw_y+bg_image_scroll_y-240);
				}
				else
				{
					BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%640-640,draw_y+bg_image_scroll_y-240);
				};
			};
		};

		void draw_bg_layer(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			for(int y=0;y<bg_tiles.size();++y)
			{
				for(int x=0;x<bg_tiles[0].size();++x)
				{
					if(bg_tiles[y][x]!=0)
					{
						BlitSurface(environment->tileset,((bg_tiles[y][x]-1)*environment->tile_size_x)%environment->tileset->w,(((bg_tiles[y][x]-1)*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,buffer,draw_x+tile_scroll_x+x*environment->tile_size_x,draw_y+tile_scroll_y+y*environment->tile_size_y);
					};
				};
			};
		};

		void draw_player_layer(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			for(int y=0;y<tiles.size();++y)
			{
				for(int x=0;x<tiles[0].size();++x)
				{
					if(tiles[y][x]!=0)
					{
						BlitSurface(environment->tileset,((tiles[y][x]-1)*environment->tile_size_x)%environment->tileset->w,(((tiles[y][x]-1)*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,buffer,draw_x+tile_scroll_x+x*environment->tile_size_x,draw_y+tile_scroll_y+y*environment->tile_size_y);
					};
				};
			};
		};

		void draw_grid(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			for(int y=0;y<=tiles.size();++y)
			{
				SDL_Rect tmp_rect;
				tmp_rect.x=0+tile_scroll_x+draw_x;
				tmp_rect.y=y*environment->tile_size_y+tile_scroll_y+draw_y;
				tmp_rect.w=tiles[0].size()*environment->tile_size_x;
				tmp_rect.h=1;
				SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,255,255,255));
			};
			for(int x=0;x<=tiles[0].size();++x)
			{
				SDL_Rect tmp_rect;
				tmp_rect.x=x*environment->tile_size_x+tile_scroll_x+draw_x;
				tmp_rect.y=0+tile_scroll_y+draw_y;
				tmp_rect.w=1;
				tmp_rect.h=tiles.size()*environment->tile_size_y;
				SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,255,255,255));
			};
		};

		void draw_player_image(SDL_Surface *buffer, int map_draw_x, int map_draw_y)
		{
			BlitSurface(player_image,0,0,player_image->w,player_image->h,buffer,map_draw_x+tile_scroll_x+player_starting_pos_x,map_draw_y+tile_scroll_y+player_starting_pos_y);
		};
};

class tile_selection: public sdl_gui_object
{
	private:
		SDL_Surface *tileset;
		int tilesize_x, tilesize_y;
		int selected, selected_size_x, selected_size_y;
		int pos_x, pos_y;
		bool multiple_selection;

	public:
		tile_selection(std::string tileset_filename, int tile_size_x, int tile_size_y)
		{
			tileset=LoadImage(tileset_filename.c_str());
			tilesize_x=tile_size_x;
			tilesize_y=tile_size_y;
			selected=0;
			selected_size_x=1;
			selected_size_y=1;
			multiple_selection=false;
		};

		tile_selection(SDL_Surface *tileset_surface, int tile_size_x, int tile_size_y)
		{
			tileset=tileset_surface;
			tilesize_x=tile_size_x;
			tilesize_y=tile_size_y;
			selected=0;
			selected_size_x=1;
			selected_size_y=1;
			multiple_selection=false;
		};

		int get_size_x()
		{
			return tileset->w+1;
		};

		int get_size_y()
		{
			return tileset->h+1;
		};

		int get_selected_id()
		{
			return selected+1;
		};

		int get_selected_id(int selected_pos_x, int selected_pos_y)
		{
			return selected+1+selected_pos_x+(tileset->w/tilesize_x)*selected_pos_y;
		};

		int get_selected_size_x()
		{
			return selected_size_x;
		};

		int get_selected_size_y()
		{
			return selected_size_y;
		};

		void draw(SDL_Surface *buffer,int draw_x, int draw_y)
		{
			pos_x=draw_x;
			pos_y=draw_y;

			BlitSurface(tileset,0,0,tileset->w,tileset->h,buffer, draw_x, draw_y);
			for(int y=0;y<=tileset->h;y+=tilesize_y)
			{
				SDL_Rect tmp_rect;
				tmp_rect.x=draw_x;
				tmp_rect.y=y+draw_y;
				tmp_rect.w=tileset->w;
				tmp_rect.h=1;
				SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,255,255,255));
			};
			for(int x=0;x<=tileset->w;x+=tilesize_x)
			{
				SDL_Rect tmp_rect;
				tmp_rect.x=x+draw_x;
				tmp_rect.y=draw_y;
				tmp_rect.w=1;
				tmp_rect.h=tileset->h;
				SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,255,255,255));
			};

			SDL_Rect tmp_rect;
			tmp_rect.x=draw_x+((selected*tilesize_x)%tileset->w);
			tmp_rect.y=draw_y+((selected*tilesize_x)/tileset->w)*tilesize_y;
			tmp_rect.w=selected_size_x*tilesize_x;
			tmp_rect.h=1;
			SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,0,255,0));

			tmp_rect.x=draw_x+((selected*tilesize_x)%tileset->w);
			tmp_rect.y=draw_y+((selected*tilesize_x)/tileset->w)*tilesize_y;
			tmp_rect.w=1;
			tmp_rect.h=selected_size_y*tilesize_y;
			SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,0,255,0));

			tmp_rect.x=draw_x+((selected*tilesize_x)%tileset->w)+selected_size_x*tilesize_x;
			tmp_rect.y=draw_y+((selected*tilesize_x)/tileset->w)*tilesize_y;
			tmp_rect.w=1;
			tmp_rect.h=selected_size_y*tilesize_y;
			SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,0,255,0));

			tmp_rect.x=draw_x+((selected*tilesize_x)%tileset->w);
			tmp_rect.y=draw_y+((selected*tilesize_x)/tileset->w)*tilesize_y+selected_size_y*tilesize_y;
			tmp_rect.w=selected_size_x*tilesize_x;
			tmp_rect.h=1;
			SDL_FillRect(buffer,&tmp_rect,SDL_MapRGB(buffer->format,0,255,0));
		};

		void update()
		{
			int mouse_x,mouse_y;
			Uint8 mousestate=SDL_GetMouseState(&mouse_x,&mouse_y);

			if(multiple_selection && mouse_x>pos_x && mouse_x<pos_x+tileset->w && mouse_y>pos_y && mouse_y<pos_y+tileset->h && !(mousestate & SDL_BUTTON (SDL_BUTTON_RIGHT)))
			{
				multiple_selection=false;
				if(selected_size_x<=0 || selected_size_x*tilesize_x>tileset->w)
				{
					selected_size_x=1;
				};
				if(selected_size_y<=0 || selected_size_y*tilesize_y>tileset->h)
				{
					selected_size_y=1;
				};
			};

			if(mouse_x>pos_x && mouse_x<pos_x+tileset->w && mouse_y>pos_y && mouse_y<pos_y+tileset->h && (mousestate & SDL_BUTTON (SDL_BUTTON_LEFT)))
			{
				selected=((mouse_x-pos_x)/tilesize_x)+((mouse_y-pos_y)/tilesize_y)*(tileset->w/tilesize_x);
				selected_size_x=1;
				selected_size_y=1;
			};

			if(mouse_x>pos_x && mouse_x<pos_x+tileset->w && mouse_y>pos_y && mouse_y<pos_y+tileset->h && (mousestate & SDL_BUTTON (SDL_BUTTON_RIGHT)))
			{
				if(!multiple_selection)
				{
					multiple_selection=true;
					selected=((mouse_x-pos_x)/tilesize_x)+((mouse_y-pos_y)/tilesize_y)*(tileset->w/tilesize_x);
				}
				else
				{
					int tmp=((mouse_x-pos_x)/tilesize_x)+((mouse_y-pos_y)/tilesize_y)*(tileset->w/tilesize_x);
					selected_size_x=1+((tmp*tilesize_x)%tileset->w)/tilesize_x-((selected*tilesize_x)%tileset->w)/tilesize_x;
					selected_size_y=1+((tmp*tilesize_x)/tileset->w)-((selected*tilesize_x)/tileset->w);
				};
			};
		};
};
