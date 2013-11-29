void show_text_box(std::string txt,int x, int y, int duration);

struct animation_set
{
	std::string image_filename;
	SDL_Surface *image;
	int size_x, size_y;
	int number_of_states, number_of_animations;
};

struct animation_instance
{
	int set_id, tile_id;
	int state, movestate;
	int anim_speed;
	int movespeed_x, movespeed_y, movespeed;
	int starting_pos_x, starting_pos_y, pos_x, pos_y;
};

struct enemy_type
{
	std::string image_filename, image_filename_right;
	SDL_Surface *image, *image_right;
	int size_x, size_y;
	int downwards_correction_value, forwards_correction_value;
	bool throwing_type;
	int throwspeed_x, throwspeed_y, throw_freq_x, throw_freq_y;
	bool flying_type;
	bool allowes_jumping;
	int attacking_pattern;
	int movespeed, move_frequency;
	int attacking_frequency;
	int animation_speed;
	int default_resistance;

	int hit_damage;
};

struct enemy
{
	int type;
	int pos_x, pos_y;
	int state, anim_state, attacking_counter;
	bool walking_direction;
	bool falling;
	bool hit;
	bool hit_direction;
	int number_of_thrown_objects;
	int thrown_x_pos[MAX_THROWN_OBJECTS], thrown_y_pos[MAX_THROWN_OBJECTS];
	bool thrown_directions[MAX_THROWN_OBJECTS];
	int remaining_resistance;
};

struct item_set
{
	std::string filename;
	SDL_Surface *image;
	int number_of_items, number_of_animations;
	int size_x, size_y;
};

struct item_type
{
	int set_id, tile_id;
	int item_action;
	bool activation_type;
	bool vanishing;
	int value;
	std::string txt;
};

struct item
{
	int type_id;
	int pos_x, pos_y;
	int anim_state, anim_speed;
};

struct special_object_type
{
	SDL_Surface *image;
	std::string image_filename;
	int type;
	int max_anim_state;
};

struct special_object
{
	int type_id;
	int anim_state;
	int pos_x, pos_y;
};

struct weather_effect
{
	SDL_Surface *image;
	int speed, anim_speed, number_of_anim_states;
	std::vector<int> pos_x;
	std::vector<int> pos_y;
	std::vector<int> anim_state;
};

class jr_environment
{
	protected:
		friend class jr_map;
		friend class jr_dev_map;
		friend class jr_player;

		SDL_Surface *tileset;
		SDL_Surface *bg_image;
		SDL_Surface *bg_cloud_image;

		int tile_size_x, tile_size_y;

		std::vector<animation_set> animation_sets;

		std::vector<item_set> item_sets;
		std::vector<item_type> item_types;

		std::vector<enemy_type> enemy_types;

		std::vector<special_object_type> special_object_types;

		std::string tileset_filename;
		std::string bg_image_filename;
		std::string bg_cloud_image_filename;

	public:
		jr_environment()
		{
			bg_image=NULL;
			bg_cloud_image=NULL;
			tileset=NULL;
		};

		void unload()
		{
			if(tileset!=NULL)
			{
				SDL_FreeSurface(tileset);
			};
			if(bg_image!=NULL)
			{
				SDL_FreeSurface(bg_image);
			};
			if(bg_cloud_image!=NULL)
			{
				SDL_FreeSurface(bg_cloud_image);
			};
			tileset=NULL;
			bg_image=NULL;
			bg_cloud_image=NULL;

			while(animation_sets.size()>0)
			{
				if(animation_sets[animation_sets.size()-1].image!=NULL) SDL_FreeSurface(animation_sets[animation_sets.size()-1].image);
				animation_sets.pop_back();
			};
			while(item_sets.size()>0)
			{
				if(item_sets[item_sets.size()-1].image!=NULL) SDL_FreeSurface(item_sets[item_sets.size()-1].image);
				item_sets.pop_back();
			};
			if(item_types.size()>0) item_types.erase(item_types.begin(),item_types.end());
			while(enemy_types.size()>0)
			{
				if(enemy_types[enemy_types.size()-1].image_right!=enemy_types[enemy_types.size()-1].image) SDL_FreeSurface(enemy_types[enemy_types.size()-1].image_right);
				if(enemy_types[enemy_types.size()-1].image!=NULL) SDL_FreeSurface(enemy_types[enemy_types.size()-1].image);
				enemy_types.pop_back();
			};
			while(special_object_types.size()>0)
			{
				if(special_object_types[special_object_types.size()-1].image!=NULL) SDL_FreeSurface(special_object_types[special_object_types.size()-1].image);
				special_object_types.pop_back();
			};

			tileset_filename="";
			bg_image_filename="";
			bg_cloud_image_filename="";
		};

		void load_from_file(std::string filename)
		{
			unload();

			int tmp;
			char tmp_char;
			bool tmp_bool;

			std::ifstream infile;
			infile.open(filename.c_str(),std::ios_base::binary);

			infile.read((char*)&tmp,sizeof(int));
			for(int i=0;i<tmp;++i)
			{
				infile.read((char*)&tmp_char,sizeof(char));
				tileset_filename+=tmp_char;
			};

			tileset=LoadImage(tileset_filename.c_str());
			SDL_SetColorKey(tileset,SDL_SRCCOLORKEY,SDL_MapRGB(tileset->format,255,255,255));

			infile.read((char*)&tile_size_x,sizeof(int));
			infile.read((char*)&tile_size_y,sizeof(int));

			infile.read((char*)&tmp_bool,sizeof(bool));
			if(tmp_bool)
			{
				infile.read((char*)&tmp,sizeof(int));
				for(int i=0;i<tmp;++i)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					bg_image_filename+=tmp_char;
				};
				bg_image=LoadImage(bg_image_filename.c_str());
			};

			infile.read((char*)&tmp_bool,sizeof(bool));
			if(tmp_bool)
			{
				infile.read((char*)&tmp,sizeof(int));
				for(int i=0;i<tmp;++i)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					bg_cloud_image_filename+=tmp_char;
				};
				bg_cloud_image=LoadImage(bg_cloud_image_filename.c_str());
			};

			//animation_sets ;-);
			infile.read((char*)&tmp,sizeof(int));
			animation_sets.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				animation_set new_set;
				new_set.image_filename="";
				int tmp_int;
				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_set.image_filename+=tmp_char;
				};
				new_set.image=LoadImage(new_set.image_filename.c_str());
				SDL_SetColorKey(new_set.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_set.image->format,255,255,255));

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.size_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.size_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.number_of_states=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.number_of_animations=tmp_int;

				animation_sets.push_back(new_set);
			};

			//item_sets ;-);
			infile.read((char*)&tmp,sizeof(int));
			item_sets.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				item_set new_set;
				new_set.filename="";
				int tmp_int;
				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_set.filename+=tmp_char;
				};
				new_set.image=LoadImage(new_set.filename.c_str());
				SDL_SetColorKey(new_set.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_set.image->format,255,255,255));

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.number_of_items=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.number_of_animations=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.size_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_set.size_y=tmp_int;

				item_sets.push_back(new_set);
			};

			//item_types ;-);
			infile.read((char*)&tmp,sizeof(int));
			item_types.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				item_type new_type;
				new_type.txt="";
				int tmp_int;
				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_type.txt+=tmp_char;
				};

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.set_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.tile_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.item_action=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.value=tmp_int;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_type.activation_type=tmp_bool;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_type.vanishing=tmp_bool;

				item_types.push_back(new_type);
			};

			//enemy_types ;-);
			infile.read((char*)&tmp,sizeof(int));
			enemy_types.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				enemy_type new_type;
				new_type.image_filename="";
				new_type.image_filename_right="";
				int tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_type.image_filename+=tmp_char;
				};
				new_type.image=LoadImage(new_type.image_filename.c_str());
				SDL_SetColorKey(new_type.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image->format,255,255,255));

				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_type.image_filename_right+=tmp_char;
				};
				if(new_type.image_filename_right==new_type.image_filename)
				{
					new_type.image_right=new_type.image;
				}
				else
				{
					new_type.image_right=LoadImage(new_type.image_filename_right.c_str());
					SDL_SetColorKey(new_type.image_right,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image_right->format,255,255,255));
				};

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.size_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.size_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.downwards_correction_value=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.forwards_correction_value=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.throwspeed_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.throwspeed_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.throw_freq_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.throw_freq_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.attacking_pattern=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.movespeed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.move_frequency=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.attacking_frequency=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.animation_speed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.default_resistance=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.hit_damage=tmp_int;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_type.throwing_type=tmp_bool;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_type.flying_type=tmp_bool;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_type.allowes_jumping=tmp_bool;

				new_type.movespeed=2*new_type.movespeed;
				new_type.animation_speed=new_type.animation_speed/4;
				if(new_type.image_filename=="images/burning_desert/crystal_1.png" || new_type.image_filename=="images/burning_desert/crystal_2.png" || new_type.image_filename=="images/burning_desert/crystal_3.png")
				{
					new_type.attacking_frequency*=2;
				};
				if(new_type.image_filename=="images/foggy_castle/pendulum.png")
				{
					new_type.animation_speed*=4;
				};

				enemy_types.push_back(new_type);
			};

			//special_object_types ;-);
			infile.read((char*)&tmp,sizeof(int));
			special_object_types.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				special_object_type new_type;
				int tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				for(int j=0;j<tmp_int;++j)
				{
					infile.read((char*)&tmp_char,sizeof(char));
					new_type.image_filename+=tmp_char;
				};
				new_type.image=LoadImage(new_type.image_filename.c_str());
				SDL_SetColorKey(new_type.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image->format,255,255,255));

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.type=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_type.max_anim_state=tmp_int;

				special_object_types.push_back(new_type);
			};

			infile.close();
		};

		void add_animation_set(std::string filename, int size_x, int size_y)
		{
			animation_set new_set;
			new_set.image_filename=filename;
			new_set.image=LoadImage(filename.c_str());
			SDL_SetColorKey(new_set.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_set.image->format,255,255,255));
			new_set.size_x=size_x;
			new_set.size_y=size_y;
			new_set.number_of_animations=new_set.image->w/size_x;
			new_set.number_of_states=new_set.image->h/size_y;

			animation_sets.push_back(new_set);
		};

		void add_item_set(std::string filename, int num_of_items, int num_of_animations)
		{
			item_set new_set;
			new_set.filename=filename;
			new_set.image=LoadImage(filename.c_str());
			SDL_SetColorKey(new_set.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_set.image->format,255,255,255));
			new_set.number_of_items=num_of_items;
			new_set.number_of_animations=num_of_animations;
			new_set.size_x=new_set.image->w/num_of_items;
			new_set.size_y=new_set.image->h/num_of_animations;
			item_sets.push_back(new_set);
		};

		void add_item_type(int set_id, int tile_id,int action, bool activation_type, bool vanishing, int value, std::string txt)
		{
			item_type new_type;
			new_type.set_id=set_id;
			new_type.tile_id=tile_id;
			new_type.item_action=action;
			new_type.activation_type=activation_type;
			new_type.vanishing=vanishing;
			new_type.value=value;
			new_type.txt=txt;
			item_types.push_back(new_type);
		};

		void add_enemy_type(std::string image_filename, std::string image_filename_right,int correction_down, int correction_forwards, int speed,int move_freq, int anim_speed, int resistance, int damage, bool throwing, bool flying,bool jumping_allowed, int attack_pattern)
		{
			enemy_type new_type;
			new_type.image_filename=image_filename;
			new_type.image=LoadImage(image_filename.c_str());
			if(image_filename_right=="")
			{
				new_type.image_filename_right=image_filename;
				new_type.image_right=new_type.image;
			}
			else
			{
				new_type.image_filename_right=image_filename_right;
				new_type.image_right=LoadImage(image_filename_right.c_str());
			};
			SDL_SetColorKey(new_type.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image->format,255,255,255));
			SDL_SetColorKey(new_type.image_right,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image_right->format,255,255,255));
			new_type.throwing_type=throwing;
			new_type.flying_type=flying;
			new_type.downwards_correction_value=correction_down;
			new_type.forwards_correction_value=correction_forwards;
			new_type.size_x=new_type.image->w/4;
			if(throwing)
			{
				new_type.size_y=new_type.image->h/4;
			}
			else
			{
				new_type.size_y=new_type.image->h/3;
			};
			new_type.movespeed=speed;
			new_type.move_frequency=move_freq;
			new_type.default_resistance=resistance;
			new_type.hit_damage=damage;
			new_type.animation_speed=anim_speed;
			new_type.attacking_pattern=attack_pattern;
			new_type.attacking_frequency=2;
			new_type.throwspeed_x=1;
			new_type.throwspeed_y=1;
			new_type.throw_freq_x=1;
			new_type.throw_freq_y=1;
			new_type.allowes_jumping=jumping_allowed;

			enemy_types.push_back(new_type);
		};

		void add_special_object_type(std::string filename, int type, int max_anim_state)
		{
			special_object_type new_type;
			new_type.image_filename=filename;
			new_type.type=type;
			new_type.max_anim_state=max_anim_state;
			new_type.image=LoadImage(filename.c_str());
			SDL_SetColorKey(new_type.image,SDL_SRCCOLORKEY,SDL_MapRGB(new_type.image->format,255,255,255));

			special_object_types.push_back(new_type);
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

		void unset_bg_cloud_image()
		{
			if(bg_cloud_image!=NULL)
			{
				SDL_FreeSurface(bg_cloud_image);
				bg_cloud_image=NULL;
			};
		};
};

class jr_map
{
	protected:
		int bg_image_scroll_x, bg_image_scroll_y, bg_image_scrollspeed_x,bg_image_scrollspeed_y;
		int bg_cloud_scroll_x, bg_cloud_scrollspeed_x;
		int scroll_counter, cloud_counter, cloud_move_freq;
		int map_size_x, map_size_y;
		std::string current_map_filename;

		std::vector<std::vector<int> > bg_tiles;
		std::vector<std::vector<int> > tiles;
		std::vector<std::vector<int> > fg_tiles;
		int tile_scroll_x,tile_scroll_y, tile_scrollspeed_x, tile_scrollspeed_y;
		int player_starting_pos_x, player_starting_pos_y;

		std::vector<animation_instance> bg_anims;
		std::vector<animation_instance> anims;
		std::vector<animation_instance> fg_anims;

		std::vector<item> items;

		std::vector<enemy> enemys;

		SDL_Surface *snowball_image;
		int snowball_x_pos[MAX_THROWN_OBJECTS], snowball_y_pos[MAX_THROWN_OBJECTS], snowball_directions[MAX_THROWN_OBJECTS], number_of_thrown_snowballs;

		std::vector<special_object> special_objects;

		SDL_Surface *fg_image;
		bool fg_image_scrolls;
		int fg_image_scroll_x, fg_image_scrollspeed_x;
		weather_effect weather;

		SDL_Surface *fog_image;
		int fog_pos_x;

		bool shaking;
		bool paused;

		jr_environment *environment;

		friend class jr_player;

	public:
		jr_map()
		{
			environment=NULL;
			fg_image=NULL;
			weather.image=NULL;
			fog_image=NULL;
			fog_pos_x=0;
			tile_scroll_x=0;
			tile_scroll_y=0;
			bg_image_scroll_x=0;
			bg_image_scroll_y=0;
			bg_cloud_scroll_x=0;
			fg_image_scroll_x=0;
			fg_image_scrollspeed_x=0;
			scroll_counter=0;
			cloud_counter=0;
			cloud_move_freq=2;
			player_starting_pos_x=0;
			player_starting_pos_y=0;
			shaking=false;
			paused=false;
			number_of_thrown_snowballs=0;
			snowball_image=NULL;
		};

		jr_map(jr_environment *env)
		{
			environment=env;
			fg_image=NULL;
			weather.image=NULL;
			fog_image=NULL;
			fog_pos_x=0;
			tile_scroll_x=0;
			tile_scroll_y=0;
			bg_image_scroll_x=0;
			bg_image_scroll_y=0;
			bg_cloud_scroll_x=0;
			fg_image_scroll_x=0;
			fg_image_scrollspeed_x=0;
			scroll_counter=0;
			cloud_counter=0;
			cloud_move_freq=2;
			player_starting_pos_x=0;
			player_starting_pos_y=0;
			shaking=false;
			paused=false;
			number_of_thrown_snowballs=0;
			snowball_image=NULL;
		};

		void unload()
		{
			reset_scroll_counter();
			reset_scroll_positions();

			for(int i=0;i<bg_tiles.size();++i)
			{
				bg_tiles[i].erase(bg_tiles[i].begin(),bg_tiles[i].end());
			};
			bg_tiles.erase(bg_tiles.begin(),bg_tiles.end());

			for(int i=0;i<tiles.size();++i)
			{
				tiles[i].erase(tiles[i].begin(),tiles[i].end());
			};
			tiles.erase(tiles.begin(),tiles.end());

			for(int i=0;i<fg_tiles.size();++i)
			{
				fg_tiles[i].erase(fg_tiles[i].begin(),fg_tiles[i].end());
			};
			fg_tiles.erase(fg_tiles.begin(),fg_tiles.end());

			bg_anims.erase(bg_anims.begin(),bg_anims.end());
			anims.erase(anims.begin(),anims.end());
			fg_anims.erase(fg_anims.begin(),fg_anims.end());

			items.erase(items.begin(),items.end());

			enemys.erase(enemys.begin(),enemys.end());

			number_of_thrown_snowballs=0;
			snowball_image=NULL;

			special_objects.erase(special_objects.begin(),special_objects.end());

			current_map_filename="";
			shaking=false;
			paused=false;
		};

		bool load_from_file(std::string filename)
		{
			unload();
			current_map_filename=filename;

			std::ifstream infile;
			infile.open(filename.c_str(),std::ios_base::binary);

			infile.read((char*)&player_starting_pos_x,sizeof(int));
			infile.read((char*)&player_starting_pos_y,sizeof(int));
			
			int tmp;

			int tmpsize_x, tmpsize_y;
			infile.read((char*)&tmpsize_x,sizeof(int));
			infile.read((char*)&tmpsize_y,sizeof(int));
			map_size_x=tmpsize_x;
			map_size_y=tmpsize_y;
			bg_tiles.reserve(tmpsize_y);
			tiles.reserve(tmpsize_y);
			fg_tiles.reserve(tmpsize_y);
			for(int i=0;i<tmpsize_y;++i)
			{
				std::vector<int> tmp;
				tmp.reserve(tmpsize_x);
				for(int j=0;j<tmpsize_x;++j)
					tmp.push_back(0);
				bg_tiles.push_back(tmp);
				tiles.push_back(tmp);
				fg_tiles.push_back(tmp);
			};

			for(int y=0;y<tmpsize_y;++y)
			{
				for(int x=0;x<tmpsize_x;++x)
				{
					infile.read((char*)&tmp,sizeof(int));
					bg_tiles[y][x]=tmp;
				};
			};

			for(int y=0;y<tmpsize_y;++y)
			{
				for(int x=0;x<tmpsize_x;++x)
				{
					infile.read((char*)&tmp,sizeof(int));
					tiles[y][x]=tmp;
				};
			};

			for(int y=0;y<tmpsize_y;++y)
			{
				for(int x=0;x<tmpsize_x;++x)
				{
					infile.read((char*)&tmp,sizeof(int));
					fg_tiles[y][x]=tmp;
				};
			};

			//animation_instances ;-);
			//bg_anims ;-);
			infile.read((char*)&tmp,sizeof(int));
			bg_anims.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				animation_instance new_inst;
				int tmp_int;
				new_inst.state=0;
				new_inst.movestate=0;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.set_id=tmp_int;

		//bool fg_image_scrolls;
				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.tile_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.anim_speed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_y=tmp_int;

				new_inst.pos_x=new_inst.starting_pos_x;
				new_inst.pos_y=new_inst.starting_pos_y;

				new_inst.anim_speed=new_inst.anim_speed/4;

				bg_anims.push_back(new_inst);
			};

			//anims ;-);
			infile.read((char*)&tmp,sizeof(int));
			anims.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				animation_instance new_inst;
				int tmp_int;
				new_inst.state=0;
				new_inst.movestate=0;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.set_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.tile_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.anim_speed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_y=tmp_int;

				new_inst.pos_x=new_inst.starting_pos_x;
				new_inst.pos_y=new_inst.starting_pos_y;

				new_inst.anim_speed=new_inst.anim_speed/4;

				anims.push_back(new_inst);
			};

			//fg_anims ;-);
			infile.read((char*)&tmp,sizeof(int));
			fg_anims.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				animation_instance new_inst;
				int tmp_int;
				new_inst.state=0;
				new_inst.movestate=0;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.set_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.tile_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.anim_speed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.movespeed=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_inst.starting_pos_y=tmp_int;

				new_inst.pos_x=new_inst.starting_pos_x;
				new_inst.pos_y=new_inst.starting_pos_y;

				new_inst.anim_speed=new_inst.anim_speed/4;

				fg_anims.push_back(new_inst);
			};

			//items ;-);
			infile.read((char*)&tmp,sizeof(int));
			items.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				item new_item;
				infile.read((char*)&tmp_int,sizeof(int));
				new_item.type_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_item.pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_item.pos_y=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_item.anim_speed=tmp_int;

				new_item.anim_state=0;

				new_item.anim_speed=new_item.anim_speed/4;

				items.push_back(new_item);
			};

			//enemys ;-);
			infile.read((char*)&tmp,sizeof(int));
			enemys.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				bool tmp_bool;
				enemy new_enemy;
				infile.read((char*)&tmp_int,sizeof(int));
				new_enemy.type=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_enemy.pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_enemy.pos_y=tmp_int;

				infile.read((char*)&tmp_bool,sizeof(bool));
				new_enemy.walking_direction=tmp_bool;

				new_enemy.state=0;
				new_enemy.anim_state=0;
				new_enemy.attacking_counter=0;
				new_enemy.falling=true;
				new_enemy.hit=false;
				new_enemy.hit_direction=!new_enemy.walking_direction;
				new_enemy.number_of_thrown_objects=0;
				new_enemy.remaining_resistance=environment->enemy_types[new_enemy.type].default_resistance;

				enemys.push_back(new_enemy);
			};

			//special_objects ;-);
			infile.read((char*)&tmp,sizeof(int));
			special_objects.reserve(tmp);
			for(int i=0;i<tmp;++i)
			{
				int tmp_int;
				special_object new_object;
				infile.read((char*)&tmp_int,sizeof(int));
				new_object.type_id=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_object.pos_x=tmp_int;

				infile.read((char*)&tmp_int,sizeof(int));
				new_object.pos_y=tmp_int;

				new_object.anim_state=environment->special_object_types[new_object.type_id].max_anim_state/2;
				if(environment->special_object_types[new_object.type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK) new_object.anim_state=0;

				special_objects.push_back(new_object);
			};

			infile.close();
		};

		void reload()
		{
			load_from_file(current_map_filename);
		};

		void set_fg_image(std::string filename, int r, int g, int b)
		{
			if(fg_image!=NULL) SDL_FreeSurface(fg_image);
			fg_image=LoadImage(filename.c_str());
			SDL_SetColorKey(fg_image,SDL_SRCCOLORKEY,SDL_MapRGB(fg_image->format,r,g,b));
			if(filename!="images/icy_cave/fg.png" && filename!="images/beating_jungle/fg.png" && filename!="images/burning_desert/fg.png")
			{
				fg_image_scrolls=true;
			}
			else
			{
				fg_image_scrolls=false;
			};
		};

		void set_fg_image_alpha(int alpha)
		{
			if(fg_image!=NULL)
			{
				SDL_SetAlpha(fg_image,SDL_SRCALPHA | SDL_RLEACCEL,alpha);
			};
		};

		void unset_fg_image()
		{
			if(fg_image!=NULL)
			{
				SDL_FreeSurface(fg_image);
				fg_image=NULL;
			};
		};

		void set_fog(std::string image_filename)
		{
			if(fog_image!=NULL) SDL_FreeSurface(fog_image);
			fog_image=LoadImage(image_filename.c_str());
			SDL_SetColorKey(fog_image,SDL_SRCCOLORKEY,SDL_MapRGB(fog_image->format,255,255,255));
			SDL_SetAlpha(fog_image,SDL_SRCALPHA | SDL_RLEACCEL,128);
		};

		void unset_fog()
		{
			if(fog_image!=NULL) SDL_FreeSurface(fog_image);
			fog_image=NULL;
		};

		void set_weather_effect(std::string image_filename, int anim_states, int anim_speed, int speed, int number_of_images)
		{
			if(weather.image!=NULL) SDL_FreeSurface(weather.image);
			if(weather.pos_x.size()>0) weather.pos_x.erase(weather.pos_x.begin(),weather.pos_x.end());
			if(weather.pos_y.size()>0) weather.pos_y.erase(weather.pos_y.begin(),weather.pos_y.end());
			weather.image=LoadImage(image_filename.c_str());
			if(image_filename=="images/foggy_castle/rain_drop.png")
			{
				SDL_SetColorKey(weather.image,SDL_SRCCOLORKEY,SDL_MapRGB(weather.image->format,106,197,255)); //Later used to identify this one for the different movement ;-);
			}
			else
			{
				SDL_SetColorKey(weather.image,SDL_SRCCOLORKEY,SDL_MapRGB(weather.image->format,255,255,255));
			};
			weather.number_of_anim_states=anim_states;
			weather.anim_speed=anim_speed;
			weather.speed=speed;
			weather.pos_x.resize(number_of_images);
			weather.pos_y.resize(number_of_images);
			weather.anim_state.resize(number_of_images);
			for(int i=0;i<number_of_images;++i)
			{
				if(image_filename=="images/foggy_castle/rain_drop.png")
				{
					weather.pos_x[i]=rand()%600;
				}
				else
				{
					weather.pos_x[i]=rand()%320;
				};
				weather.pos_y[i]=-40-(rand()%400);
				weather.anim_state[i]=0;
			};
		};

		void unset_weather_effect()
		{
			if(weather.pos_x.size()>0) weather.pos_x.erase(weather.pos_x.begin(),weather.pos_x.end());
			if(weather.pos_y.size()>0) weather.pos_y.erase(weather.pos_y.begin(),weather.pos_y.end());
			if(weather.image!=NULL)
			{
				SDL_FreeSurface(weather.image);
				weather.image=NULL;
			};
		};

		void set_tile_scrollspeed_x(int value)
		{
			tile_scrollspeed_x=value;
		};

		void set_tile_scrollspeed_y(int value)
		{
			tile_scrollspeed_y=value;
		};

		void set_bg_image_scrollspeed_x(int value)
		{
			bg_image_scrollspeed_x=value;
		};

		void set_bg_image_scrollspeed_y(int value)
		{
			bg_image_scrollspeed_y=value;
		};

		void set_bg_cloud_scrollspeed_x(int value)
		{
			bg_cloud_scrollspeed_x=value;
		};

		void set_cloud_move_freq(int freq)
		{
			cloud_move_freq=freq;
		};

		void set_fg_image_scrollspeed_x(int value)
		{
			fg_image_scrollspeed_x=value;
		};

		void reset_scroll_counter()
		{
			scroll_counter=0;
		};

		void update_clouds()
		{
			if(environment->bg_cloud_image!=NULL && cloud_move_freq!=0) //cloud_move_freq==0 fixes the clouds to the upper left corner ;-);
			{
				cloud_counter++;
				cloud_counter%=cloud_move_freq;
				if(cloud_counter==0) bg_cloud_scroll_x-=bg_cloud_scrollspeed_x;
				bg_cloud_scroll_x+=scroll_counter;
				bg_cloud_scroll_x%=environment->bg_cloud_image->w;
			};
		};

		int get_tile_scroll_x()
		{
			return tile_scroll_x;
		};

		int get_tile_scroll_y()
		{
			return tile_scroll_y;
		};

		int get_tile_size_x()
		{
			return environment->tile_size_x;
		};

		int get_tile_size_y()
		{
			return environment->tile_size_y;
		};

		int get_map_size_x()
		{
			return tiles[0].size();
		};

		int get_map_size_y()
		{
			return tiles.size();
		};

		void set_player_starting_position(int x_start, int y_start)
		{
			if(x_start<map_size_x*environment->tile_size_x && x_start>=0 && y_start>=0 && y_start<map_size_y*environment->tile_size_y)
			{
				player_starting_pos_x=x_start;
				player_starting_pos_y=y_start;
			};
		};

		int get_player_starting_pos_x()
		{
			return player_starting_pos_x;
		};

		int get_player_starting_pos_y()
		{
			return player_starting_pos_y;
		};

		void add_animation_instance(int set_id, int tile_id, int layer_id, int starting_x, int starting_y, int anim_speed, int movespeed_x, int movespeed_y, int movespeed)
		{
			animation_instance new_anim;
			new_anim.state=0;
			new_anim.movestate=0;
			new_anim.set_id=set_id;
			new_anim.tile_id=tile_id;
			new_anim.starting_pos_x=starting_x;
			new_anim.pos_x=starting_x;
			new_anim.starting_pos_y=starting_y;
			new_anim.pos_y=starting_y;
			new_anim.anim_speed=anim_speed;
			new_anim.movespeed_x=movespeed_x;
			new_anim.movespeed_y=movespeed_y;
			new_anim.movespeed=movespeed;

			switch(layer_id)
			{
				case LAYER_BG:
				{
					bg_anims.push_back(new_anim);
					break;
				};
				case LAYER_PLAYER:
				{
					anims.push_back(new_anim);
					break;
				};
				case LAYER_FG:
				{
					fg_anims.push_back(new_anim);
					break;
				};
			};
		};

		void add_item(int type, int x, int y, int anim_speed)
		{
			item new_item;
			new_item.type_id=type;
			new_item.pos_x=x;
			new_item.pos_y=y;
			new_item.anim_state=0;
			new_item.anim_speed=anim_speed;
			items.push_back(new_item);
		};

		void add_special_object(int type_id, int pos_x, int pos_y)
		{
			special_object new_object;
			new_object.type_id=type_id;
			new_object.pos_x=pos_x;
			new_object.pos_y=pos_y;
			if(environment->special_object_types[type_id].type==OBJECT_TYPE_MOVING_PLATFORM)
			{
				new_object.anim_state=environment->special_object_types[type_id].max_anim_state/2;
			}
			else
			{
				new_object.anim_state=0;
			};
			special_objects.push_back(new_object);
		};

		void remove_special_object(int object_id)
		{
			special_objects.erase(special_objects.begin()+object_id);
		};

		void add_enemy(int type_id, int starting_x, int starting_y)
		{
			enemy new_enemy;
			new_enemy.type=type_id;
			new_enemy.pos_x=starting_x;
			new_enemy.pos_y=starting_y;
			new_enemy.state=ENEMY_STATE_WALKING;
			new_enemy.anim_state=0;
			new_enemy.attacking_counter=0;
			new_enemy.falling=false;
			new_enemy.hit=false;
			new_enemy.walking_direction=ENEMY_WALKING_LEFT;
			new_enemy.number_of_thrown_objects=0;
			new_enemy.remaining_resistance=environment->enemy_types[type_id].default_resistance;

			enemys.push_back(new_enemy);
		};

		void throw_snowball(int x, int y, int direction, SDL_Surface *image)
		{
			if(snowball_image==NULL) snowball_image=image;
			if(number_of_thrown_snowballs<MAX_THROWN_OBJECTS)
			{
				snowball_x_pos[number_of_thrown_snowballs]=x;
				snowball_y_pos[number_of_thrown_snowballs]=y;
				snowball_directions[number_of_thrown_snowballs]=direction;
				++number_of_thrown_snowballs;
			};
		};

		bool ladder_collides(SDL_Surface *player_image, int player_pos_x, int player_pos_y, int player_size_x, int player_size_y, int player_state, int player_anim_state, int &ladder_id)
		{
			ladder_id=-1;
			for(int i=0;i<special_objects.size();++i)
			{
				if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_LADDER)
				{
					if(player_pos_x<special_objects[i].pos_x+30 && player_pos_x+player_size_x>special_objects[i].pos_x && player_pos_y<(special_objects[i].pos_y/10)*10+60*(special_objects[i].pos_y%10) && player_pos_y+player_size_y>special_objects[i].pos_y)
					//Thanks to me having to save pos_x, pos_y as well as the size within 2 variables, i used pos_y for both, pos_y and size_y ;-)
					//pretty awefull, yet useful ;-) pos_y==(pos_y/10)*10, size_y=60*(pos_y%10) ;-)
					//I dont even lose precision, because the ladder is in tiles, which are all 10 pxs in size, therefore the ladder is never positioned in a way where pos_y%10!=0 ;-)
					//well whatever ;-);
					{
						ladder_id=i;
						return true;
					};
				};
			};
			return false;
		};

		bool special_object_collides(int object_id, SDL_Surface *player_image, int player_pos_x, int player_pos_y, int player_size_x, int player_size_y, int player_state, int player_anim_state)
		{
			if((environment->special_object_types[special_objects[object_id].type_id].type!=OBJECT_TYPE_LADDER) && (player_pos_x>special_objects[object_id].pos_x+environment->special_object_types[special_objects[object_id].type_id].image->w || player_pos_x+player_size_x<special_objects[object_id].pos_x || player_pos_y>special_objects[object_id].pos_y+environment->special_object_types[special_objects[object_id].type_id].image->h || player_pos_y+player_size_y<special_objects[object_id].pos_y))
			{
				return false;
			}
			else
			{
				if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_MOVING_PLATFORM)
				{
					int img_starting_point=0;
					if((special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/8))>3)
					{
						img_starting_point=5;
					};
					if(image_frames_collide(player_image,player_size_x*(player_anim_state/32)+img_starting_point,player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,0,(environment->special_object_types[special_objects[object_id].type_id].image->h/9)*(special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/8)),environment->special_object_types[special_objects[object_id].type_id].image->w,environment->special_object_types[special_objects[object_id].type_id].image->h/9,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
					{
						if(player_pos_x+player_size_x/2>special_objects[object_id].pos_x+environment->special_object_types[special_objects[object_id].type_id].image->w/2)
						{
							if(special_objects[object_id].anim_state<environment->special_object_types[special_objects[object_id].type_id].max_anim_state)
							{
								special_objects[object_id].anim_state++;
							};
						}
						else
						{
							if(special_objects[object_id].anim_state>0)
							{
								special_objects[object_id].anim_state--;
							};
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
					if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_FLOATING_PLATFORM || environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_VANISHING)
					{
						if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,0,0,environment->special_object_types[special_objects[object_id].type_id].image->w,environment->special_object_types[special_objects[object_id].type_id].image->h,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
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
						if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_LIANA)
						{
							if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,(environment->special_object_types[special_objects[object_id].type_id].image->w/7)*(special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/7)),0,environment->special_object_types[special_objects[object_id].type_id].image->w/7,environment->special_object_types[special_objects[object_id].type_id].image->h/5,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
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
							if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK)
							{
								switch(special_objects[object_id].anim_state/((environment->special_object_types[special_objects[object_id].type_id].max_anim_state)/3))
								{
									case 0:
									{
										if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,(environment->special_object_types[special_objects[object_id].type_id].image->w/4)*(special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/12)),0,environment->special_object_types[special_objects[object_id].type_id].image->w/4,environment->special_object_types[special_objects[object_id].type_id].image->h/3,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
										{
											return true;
										}
										else
										{
											return false;
										};
										break;
									};
									case 1:
									{
										if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,(environment->special_object_types[special_objects[object_id].type_id].image->w/4)*((special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/12))-4),environment->special_object_types[special_objects[object_id].type_id].image->h/3,environment->special_object_types[special_objects[object_id].type_id].image->w/4,environment->special_object_types[special_objects[object_id].type_id].image->h/3,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
										{
											return true;
										}
										else
										{
											return false;
										};
										break;
									};
									case 2:
									{
										if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,(environment->special_object_types[special_objects[object_id].type_id].image->w/4)*((special_objects[object_id].anim_state/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/12))-8),2*(environment->special_object_types[special_objects[object_id].type_id].image->h/3),environment->special_object_types[special_objects[object_id].type_id].image->w/4,environment->special_object_types[special_objects[object_id].type_id].image->h/3,special_objects[object_id].pos_x,special_objects[object_id].pos_y-15))
										{
											return true;
										}
										else
										{
											return false;
										};
										break;
									};
								};
							}
							else
							{
								if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_CHEST)
								{
									if(player_pos_x<special_objects[object_id].pos_x+environment->special_object_types[special_objects[object_id].type_id].image->w/2)
									{
										if(special_objects[object_id].anim_state==0)
										{
											switch(environment->special_object_types[special_objects[object_id].type_id].max_anim_state)
											{
												case CHEST_CLOSED_NICE:
												{
													show_text_box("Thank you Blob!",special_objects[object_id].pos_x+tile_scroll_x,special_objects[object_id].pos_y+tile_scroll_y,800);
													add_item(rand()%environment->item_types.size(),special_objects[object_id].pos_x,special_objects[object_id].pos_y+40,32);
													break;
												};
												case CHEST_CLOSED_RANDOM:
												{
													if(rand()%3==0) //Damn mimics ;-)
													{
														add_enemy(rand()%2,special_objects[object_id].pos_x,special_objects[object_id].pos_y); //dont allow all enemy_types, a mimic being a boss would be quite troublesome ;-);
													}
													else
													{
														add_item(rand()%environment->item_types.size(),special_objects[object_id].pos_x,special_objects[object_id].pos_y+20,32);
													};
													break;
												};
											};
											special_objects[object_id].anim_state=1;
										};
									};
								}
								else
								{
									if(environment->special_object_types[special_objects[object_id].type_id].type==OBJECT_TYPE_RISING_PLATFORM)
									{
										if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x-5,player_size_y,player_pos_x, player_pos_y,environment->special_object_types[special_objects[object_id].type_id].image,(environment->special_object_types[special_objects[object_id].type_id].image->w/4)*(((special_objects[object_id].anim_state-1)%(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/2))/(environment->special_object_types[special_objects[object_id].type_id].max_anim_state/8)),0,environment->special_object_types[special_objects[object_id].type_id].image->w/4,environment->special_object_types[special_objects[object_id].type_id].image->h,special_objects[object_id].pos_x,special_objects[object_id].pos_y))
										{
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
			};
			return false;
		};

		bool special_objects_collide(SDL_Surface *player_image, int player_pos_x, int player_pos_y, int player_size_x, int player_size_y, int player_state, int player_anim_state, int &last_id)
		{
			for(int i=0;i<special_objects.size();++i)
			{
				if(special_object_collides(i,player_image, player_pos_x, player_pos_y, player_size_x, player_size_y, player_state, player_anim_state))
				{
					last_id=i;
					return true;
				};
			};
			return false;
		};

		bool enemy_collides(int id, int distance)
		{
			for(int i=0;i<environment->enemy_types[enemys[id].type].size_x;i+=environment->tile_size_x)
			{
				if(tiles[(enemys[id].pos_y+environment->enemy_types[enemys[id].type].size_y+distance-environment->enemy_types[enemys[id].type].downwards_correction_value)/environment->tile_size_y][(enemys[id].pos_x+i)/environment->tile_size_x]!=0)
					return true;
			};
			return false;
		};

		bool thrown_object_collides(int enemy_id, int object_id)
		{
			if(enemys[enemy_id].state>ENEMY_STATE_VANISHING || enemys[enemy_id].thrown_x_pos[object_id]+environment->enemy_types[enemys[enemy_id].type].size_x==0 || enemys[enemy_id].thrown_x_pos[object_id]+environment->enemy_types[enemys[enemy_id].type].size_x>=map_size_x*environment->tile_size_x || enemys[enemy_id].thrown_y_pos[object_id]+environment->enemy_types[enemys[enemy_id].type].size_y==0 || enemys[enemy_id].thrown_y_pos[object_id]+environment->enemy_types[enemys[enemy_id].type].size_y>=map_size_y*environment->tile_size_y || enemys[enemy_id].thrown_x_pos[object_id]<0 || enemys[enemy_id].thrown_y_pos[object_id]<0)
			{
				return false;
			};

			int x_change=environment->enemy_types[enemys[enemy_id].type].throwspeed_x;
			int y_change=environment->enemy_types[enemys[enemy_id].type].throwspeed_y;
			SDL_Surface *enemy_image;
			if(enemys[enemy_id].thrown_directions[object_id]==ENEMY_DIRECTION_LEFT)
			{
				x_change=-x_change;
				enemy_image=environment->enemy_types[enemys[enemy_id].type].image;
			}
			else
			{
				enemy_image=environment->enemy_types[enemys[enemy_id].type].image_right;
			};

			if(enemys[enemy_id].anim_state%environment->enemy_types[enemys[enemy_id].type].throw_freq_x==0 ||  enemys[enemy_id].anim_state%environment->enemy_types[enemys[enemy_id].type].throw_freq_y==0)
			{
				int enemy_start_x=environment->enemy_types[enemys[enemy_id].type].size_x*(enemys[enemy_id].anim_state/environment->enemy_types[enemys[enemy_id].type].animation_speed);
				int enemy_start_y=ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[enemy_id].type].size_y;

				for(int y=0;y<environment->enemy_types[enemys[enemy_id].type].size_y;y+=environment->tile_size_y)
				{
					for(int x=0;x<environment->enemy_types[enemys[enemy_id].type].size_x;x+=environment->tile_size_x)
					{
							if(tiles[(enemys[enemy_id].thrown_y_pos[object_id]+y+y_change)/environment->tile_size_y][(enemys[enemy_id].thrown_x_pos[object_id]+x+x_change)/environment->tile_size_x]!=0 && image_frames_collide(enemy_image,enemy_start_x,enemy_start_y,environment->enemy_types[enemys[enemy_id].type].size_x,environment->enemy_types[enemys[enemy_id].type].size_y,enemys[enemy_id].thrown_x_pos[object_id],enemys[enemy_id].thrown_y_pos[object_id],environment->tileset,(tiles[(enemys[enemy_id].thrown_y_pos[object_id]+y+y_change)/environment->tile_size_y][(enemys[enemy_id].thrown_x_pos[object_id]+x+x_change)/environment->tile_size_x]-1)*environment->tile_size_x%environment->tileset->w,((tiles[(enemys[enemy_id].thrown_y_pos[object_id]+y+y_change)/environment->tile_size_y][(enemys[enemy_id].thrown_x_pos[object_id]+x+x_change)/environment->tile_size_x]-1)*environment->tile_size_x/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,(int((enemys[enemy_id].thrown_x_pos[object_id]+x+x_change)/environment->tile_size_x))*environment->tile_size_x,(int((enemys[enemy_id].thrown_y_pos[object_id]+y+y_change)/environment->tile_size_y))*environment->tile_size_y))
							{
								return true;
							};
					};
				};
			};

			return false;
		};

              	int update_enemys(SDL_Surface *player_image, int player_pos_x, int player_pos_y, int player_size_x, int player_size_y, int player_state,bool falling, int player_falling_state, int player_strength, int player_anim_state, int elem_id, bool &hit_direction, bool &forced_jumping, bool boss_fight, bool &reset_elem) //Return Value is damage to blob ;-);
		{
			int return_value=0;
			for(int i=0;i<enemys.size();++i)
			{
				if(boss_fight && enemys[i].state<ENEMY_STATE_THROWN_OBJECT && environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/lavly.png")
				{
					enemys[i].state=ENEMY_STATE_SPECIAL1;
				};
				int last_return_value=return_value;
				SDL_Surface *enemy_surf;
				if(enemys[i].walking_direction==ENEMY_WALKING_LEFT)
				{
					enemy_surf=environment->enemy_types[enemys[i].type].image;
				}
				else
				{
					enemy_surf=environment->enemy_types[enemys[i].type].image_right;
				};
				if(!enemys[i].hit && (enemys[i].state<ENEMY_STATE_THROWN_OBJECT || (boss_fight && i==0 && enemys[i].state<ENEMY_STATE_SPECIAL2 && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/scorpy.png")) && ((environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_ON_TOUCH) || ((environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_REGULARY) && enemys[i].state==ENEMY_STATE_ATTACKING)) || environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/pendulum.png")
				{
					if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y,enemy_surf,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, enemys[i].pos_x, enemys[i].pos_y))
					{
						if((player_state!=PLAYER_STATE_ATTACK && player_state!=PLAYER_STATE_PUNSHING && elem_id!=ELEMENT_WIND && elem_id!=ELEMENT_FIRE) || ((environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_REGULARY) && enemys[i].state==ENEMY_STATE_ATTACKING && (!(environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_ON_TOUCH) || environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/beast_king_2.png") ) || ((environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/spitting_volcano.png" || environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/hot_lava.png") && elem_id!=ELEMENT_ICE) || environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/poisoned_fog.png" || environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/spikes.png" || environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/pendulum.png" || (boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/scorpy_protected.png") || (boss_fight && (enemys[0].state==ENEMY_STATE_THROWN_OBJECT || enemys[0].state==ENEMY_STATE_SPECIAL1))) //I get bored of it yet i will say it once more ;-) I hate myself for using this ;-);
						{
							int tmp_fall_speed=0;
							if(falling)
							{
								if(player_falling_state<16)
								{
									tmp_fall_speed=1;
								}
								else
								{
									if(player_falling_state<36)
									{
										tmp_fall_speed=2;
									}
									else
									{
										tmp_fall_speed=3;
									};
								};
								if(player_falling_state%2!=0) tmp_fall_speed=0;
							};

							if(!environment->enemy_types[enemys[i].type].allowes_jumping || !falling || tmp_fall_speed==0 || image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y-tmp_fall_speed,enemy_surf,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, enemys[i].pos_x, enemys[i].pos_y))
							{
								return_value+=environment->enemy_types[enemys[i].type].hit_damage;
								if(boss_fight && enemys[0].state==ENEMY_STATE_SPECIAL1) return_value+=environment->enemy_types[enemys[i].type].hit_damage;
								if(player_state!=PLAYER_STATE_HIT)
								{
									if((player_pos_x+(player_size_x/2))>(enemys[i].pos_x+(environment->enemy_types[enemys[i].type].size_x/2)))
									{
										hit_direction=DIRECTION_RIGHT;
										enemys[i].walking_direction=ENEMY_DIRECTION_RIGHT;
									}
									else
									{
										hit_direction=DIRECTION_LEFT;
										enemys[i].walking_direction=ENEMY_DIRECTION_LEFT;
									};
									if(environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/hot_danger.png") enemys[i].walking_direction=DIRECTION_LEFT;
								};
							}
							else
							{
								forced_jumping=true;
							};
						}
						else
						{
							Mix_PlayChannel(-1,hit_sound,0);
							enemys[i].hit=true;
							enemys[i].anim_state=0;
							enemys[i].remaining_resistance-=player_strength;
							if(elem_id==ELEMENT_FIRE) enemys[i].remaining_resistance-=player_strength;
							if(player_state==PLAYER_STATE_ATTACK)
							{
								enemys[i].remaining_resistance-=player_strength;
							};
							if((player_pos_x+(player_size_x/2))>(enemys[i].pos_x+(environment->enemy_types[enemys[i].type].size_x/2)))
							{
								enemys[i].hit_direction=DIRECTION_LEFT;
							}
							else
							{
								enemys[i].hit_direction=DIRECTION_RIGHT;
							};
							if(enemys[i].remaining_resistance<=0)
							{
								Mix_PlayChannel(-1,enemy_death_sound,0);
								enemys[i].state=ENEMY_STATE_VANISHING;
								if(boss_fight && i==0)
								{
									for(int j=1;j<enemys.size();++j)
									{
										enemys[j].state=ENEMY_STATE_VANISHING;
										enemys[j].remaining_resistance=0;
										enemys[j].hit=true;
									};
								};
								if(boss_fight && environment->bg_image_filename=="images/beating_jungle/lavly_bg.png")
								{
									forced_jumping=true;
								};
								if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_1.png")
								{
									shaking=true;

									//remove old tiles ;-);
									for(int tmp_x=37;tmp_x<41;++tmp_x)
									{
										for(int tmp_y=28;tmp_y<36;++tmp_y)
										{
											tiles[tmp_y][tmp_x]=0;
											if(tmp_y>29) bg_tiles[tmp_y][tmp_x-3]=0;
											if(tmp_y>31 && tmp_x>38) bg_tiles[tmp_y][tmp_x+2]=0;
										};
									};

									//and set new ones ;-);
									bg_tiles[33][35]=37;bg_tiles[33][36]=38;bg_tiles[33][37]=39;bg_tiles[33][38]=40;bg_tiles[33][39]=45;bg_tiles[33][40]=46;bg_tiles[33][41]=47;
									bg_tiles[34][35]=101;bg_tiles[34][36]=102;bg_tiles[34][37]=103;bg_tiles[34][38]=104;bg_tiles[34][39]=109;bg_tiles[34][40]=110;bg_tiles[34][41]=111;
									bg_tiles[35][35]=165;bg_tiles[35][36]=166;bg_tiles[35][37]=167;bg_tiles[35][38]=168;bg_tiles[35][39]=173;bg_tiles[35][40]=174;bg_tiles[35][41]=175;

									bg_tiles[38][49]=57;tiles[38][50]=58;tiles[38][51]=59;bg_tiles[38][52]=60;bg_tiles[38][53]=61;bg_tiles[38][54]=62;
									bg_tiles[39][49]=121;tiles[39][50]=122;tiles[39][51]=123;bg_tiles[39][52]=124;bg_tiles[39][53]=125;bg_tiles[39][54]=126;
									bg_tiles[40][49]=185;tiles[40][50]=186;tiles[40][51]=187;bg_tiles[40][52]=188;bg_tiles[40][53]=189;bg_tiles[40][54]=190;
									bg_tiles[41][49]=249;tiles[41][50]=250;tiles[41][51]=250;bg_tiles[41][52]=252;bg_tiles[41][53]=253;bg_tiles[41][54]=254;
									bg_tiles[42][45]=309;bg_tiles[42][46]=310;tiles[42][47]=311;tiles[42][48]=312;tiles[42][49]=313;tiles[42][50]=314;tiles[42][51]=315;tiles[42][52]=316;tiles[42][53]=317;bg_tiles[42][54]=318;bg_tiles[42][55]=319;
									bg_tiles[43][45]=373;bg_tiles[43][46]=374;tiles[43][47]=375;tiles[43][48]=376;tiles[43][49]=377;tiles[43][50]=378;tiles[43][51]=379;tiles[43][52]=380;tiles[43][53]=381;bg_tiles[43][54]=382;bg_tiles[43][55]=383;
									bg_tiles[44][45]=437;bg_tiles[44][46]=438;tiles[44][47]=439;tiles[44][48]=440;tiles[44][49]=441;tiles[44][50]=442;tiles[44][51]=443;tiles[44][52]=444;tiles[44][53]=445;bg_tiles[44][54]=446;bg_tiles[44][55]=447;
									bg_tiles[45][45]=501;bg_tiles[45][46]=502;tiles[45][47]=503;tiles[45][48]=504;tiles[45][49]=505;tiles[45][50]=506;tiles[45][51]=507;tiles[45][52]=508;tiles[45][53]=509;bg_tiles[45][54]=510;bg_tiles[45][55]=511;
								}
								else
								{
									if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_3.png")
									{
										show_cutscene("burning_desert_2.scn");
										add_enemy(9,300,300);
									};
								};

								if(boss_fight && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/draki_vaultkeeper.png")
								{
									--enemys[0].type;
								};
							}
							else
							{
								enemys[i].state=ENEMY_STATE_WALKING;
							};
							if(boss_fight && environment->bg_image_filename=="images/spider_bg.png") //yeah i know its lame ;-);
							{
								enemys[0].state=ENEMY_STATE_VANISHING;
								enemys[1].anim_state=0;
								enemys[1].hit=true;
								enemys[2].anim_state=0;
								enemys[2].hit=true;
							};

							if((boss_fight && environment->bg_image_filename=="images/icy_cave/yeti_bg.png") || environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/spitting_volcano.png" || environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/hot_lava.png") //yeah i know its lame ;-); I even copied it, shame on me ;-);
							{
								reset_elem=true;
							};
						};
					};
				};

				for(int j=0;j<number_of_thrown_snowballs;++j)
				{
					if(enemys[i].state<ENEMY_STATE_THROWN_OBJECT && image_frames_collide(snowball_image,snowball_directions[j]*snowball_image->w/2,0,snowball_image->w/2,snowball_image->h,snowball_x_pos[j],snowball_y_pos[j],enemy_surf,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, enemys[i].pos_x, enemys[i].pos_y))					if(image_frames_collide(snowball_image,snowball_directions[j]*snowball_image->w/2,0,snowball_image->w/2,snowball_image->h,snowball_x_pos[j],snowball_y_pos[j],enemy_surf,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, enemys[i].pos_x, enemys[i].pos_y))
					{
							Mix_PlayChannel(-1,hit_sound,0);
							enemys[i].hit=true;
							enemys[i].anim_state=0;
							enemys[i].remaining_resistance-=player_strength;
							if((snowball_x_pos[j]+(snowball_image->w/2))>(enemys[i].pos_x+(environment->enemy_types[enemys[i].type].size_x/2)))
							{
								enemys[i].hit_direction=DIRECTION_LEFT;
							}
							else
							{
								enemys[i].hit_direction=DIRECTION_RIGHT;
							};
							if(enemys[i].remaining_resistance<=0)
							{
								Mix_PlayChannel(-1,enemy_death_sound,0);
								enemys[i].state=ENEMY_STATE_VANISHING;
								if(boss_fight && i==0)
								{
									for(int j=1;j<enemys.size();++j)
									{
										enemys[j].state=ENEMY_STATE_VANISHING;
										enemys[j].remaining_resistance=0;
										enemys[j].hit=true;
									};
								};

								if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_1.png")
								{
									shaking=true;

									//remove old tiles ;-);
									for(int tmp_x=37;tmp_x<41;++tmp_x)
									{
										for(int tmp_y=28;tmp_y<36;++tmp_y)
										{
											tiles[tmp_y][tmp_x]=0;
											if(tmp_y>29) bg_tiles[tmp_y][tmp_x-3]=0;
											if(tmp_y>31 && tmp_x>38) bg_tiles[tmp_y][tmp_x+2]=0;
										};
									};

									//and set new ones ;-);
									bg_tiles[33][35]=37;bg_tiles[33][36]=38;bg_tiles[33][37]=39;bg_tiles[33][38]=40;bg_tiles[33][39]=45;bg_tiles[33][40]=46;bg_tiles[33][41]=47;
									bg_tiles[34][35]=101;bg_tiles[34][36]=102;bg_tiles[34][37]=103;bg_tiles[34][38]=104;bg_tiles[34][39]=109;bg_tiles[34][40]=110;bg_tiles[34][41]=111;
									bg_tiles[35][35]=165;bg_tiles[35][36]=166;bg_tiles[35][37]=167;bg_tiles[35][38]=168;bg_tiles[35][39]=173;bg_tiles[35][40]=174;bg_tiles[35][41]=175;

									bg_tiles[38][49]=57;tiles[38][50]=58;tiles[38][51]=59;bg_tiles[38][52]=60;bg_tiles[38][53]=61;bg_tiles[38][54]=62;
									bg_tiles[39][49]=121;tiles[39][50]=122;tiles[39][51]=123;bg_tiles[39][52]=124;bg_tiles[39][53]=125;bg_tiles[39][54]=126;
									bg_tiles[40][49]=185;tiles[40][50]=186;tiles[40][51]=187;bg_tiles[40][52]=188;bg_tiles[40][53]=189;bg_tiles[40][54]=190;
									bg_tiles[41][49]=249;tiles[41][50]=250;tiles[41][51]=250;bg_tiles[41][52]=252;bg_tiles[41][53]=253;bg_tiles[41][54]=254;
									bg_tiles[42][45]=309;bg_tiles[42][46]=310;tiles[42][47]=311;tiles[42][48]=312;tiles[42][49]=313;tiles[42][50]=314;tiles[42][51]=315;tiles[42][52]=316;tiles[42][53]=317;bg_tiles[42][54]=318;bg_tiles[42][55]=319;
									bg_tiles[43][45]=373;bg_tiles[43][46]=374;tiles[43][47]=375;tiles[43][48]=376;tiles[43][49]=377;tiles[43][50]=378;tiles[43][51]=379;tiles[43][52]=380;tiles[43][53]=381;bg_tiles[43][54]=382;bg_tiles[43][55]=383;
									bg_tiles[44][45]=437;bg_tiles[44][46]=438;tiles[44][47]=439;tiles[44][48]=440;tiles[44][49]=441;tiles[44][50]=442;tiles[44][51]=443;tiles[44][52]=444;tiles[44][53]=445;bg_tiles[44][54]=446;bg_tiles[44][55]=447;
									bg_tiles[45][45]=501;bg_tiles[45][46]=502;tiles[45][47]=503;tiles[45][48]=504;tiles[45][49]=505;tiles[45][50]=506;tiles[45][51]=507;tiles[45][52]=508;tiles[45][53]=509;bg_tiles[45][54]=510;bg_tiles[45][55]=511;
								}
								else
								{
									if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_3.png")
									{
										show_cutscene("burning_desert_2.scn");
										add_enemy(9,300,300);
									};
								};


								if(boss_fight && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/draki_vaultkeeper.png")
								{
									--enemys[0].type;
								};
							}
							else
							{
								enemys[i].state=ENEMY_STATE_WALKING;
							};
							if(boss_fight && environment->bg_image_filename=="images/spider_bg.png") //yeah i know its lame ;-);
							{
								enemys[0].state=ENEMY_STATE_VANISHING;
								enemys[1].anim_state=0;
								enemys[1].hit=true;
								enemys[2].anim_state=0;
								enemys[2].hit=true;
							};

							number_of_thrown_snowballs--;
							for(int u=j;u<number_of_thrown_snowballs;++u)
							{
								snowball_directions[u]=snowball_directions[u+1];
								snowball_x_pos[u]=snowball_x_pos[u+1];
								snowball_y_pos[u]=snowball_y_pos[u+1];
							};
					};
				};

				if(enemys[i].state==ENEMY_STATE_ATTACKING)
				{
					if(enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*4-1)
					{
						enemys[i].anim_state=0;
						enemys[i].state=ENEMY_STATE_WALKING;
						if(boss_fight && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_transformation.png")
						{
							enemys[i].anim_state=environment->enemy_types[enemys[i].type].animation_speed*4-1;
							enemys[i].state=ENEMY_STATE_VANISHING;
						};
						if(environment->enemy_types[enemys[i].type].throwing_type && enemys[i].number_of_thrown_objects<MAX_THROWN_OBJECTS && !(boss_fight && environment->bg_image_filename=="images/spider_bg.png" && enemys[i].number_of_thrown_objects!=0))
						{
							enemys[i].thrown_x_pos[enemys[i].number_of_thrown_objects]=enemys[i].pos_x;
							enemys[i].thrown_y_pos[enemys[i].number_of_thrown_objects]=enemys[i].pos_y;
							enemys[i].thrown_directions[enemys[i].number_of_thrown_objects]=enemys[i].walking_direction;
							enemys[i].number_of_thrown_objects++;
							if(boss_fight && environment->bg_image_filename=="images/spider_bg.png")
							{
								environment->enemy_types[enemys[i].type].throwspeed_x=4;
								environment->enemy_types[enemys[i].type].throwspeed_y=-1;
								environment->enemy_types[enemys[i].type].throw_freq_y=3;
								enemys[i].thrown_directions[enemys[i].number_of_thrown_objects-1]=ENEMY_WALKING_LEFT;
							};
							if(boss_fight && environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
							{
								enemys[i].thrown_directions[enemys[i].number_of_thrown_objects-1]=ENEMY_WALKING_LEFT;
								if(enemys[i].number_of_thrown_objects==3)
								{
									enemys[i].pos_y=player_pos_y;
									if(player_pos_x>map_size_x*environment->tile_size_x/2)
									{
										enemys[i].anim_state=-environment->enemy_types[enemys[i].type].size_x;
										enemys[i].state=ENEMY_STATE_SPECIAL1;
										enemys[i].walking_direction=ENEMY_DIRECTION_RIGHT;
									}
									else
									{
										enemys[i].anim_state=0;
										enemys[i].state=ENEMY_STATE_SPECIAL2;
										enemys[i].walking_direction=ENEMY_DIRECTION_LEFT;
									};
								};
							};
							if(boss_fight && environment->bg_image_filename=="images/beating_jungle/lavly_bg.png")
							{
								enemys[i].thrown_y_pos[enemys[i].number_of_thrown_objects-1]=-(rand()%100);
								enemys[i].thrown_x_pos[enemys[i].number_of_thrown_objects-1]=rand()%(map_size_x*environment->tile_size_x);
							};
							if(boss_fight && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_2.png")
							{
								if(enemys[i].number_of_thrown_objects<MAX_THROWN_OBJECTS)
								{
									enemys[i].thrown_x_pos[enemys[i].number_of_thrown_objects]=enemys[i].pos_x;
									enemys[i].thrown_y_pos[enemys[i].number_of_thrown_objects]=enemys[i].pos_y;
									enemys[i].thrown_directions[enemys[i].number_of_thrown_objects]=enemys[i].walking_direction==ENEMY_WALKING_LEFT?ENEMY_WALKING_RIGHT:ENEMY_WALKING_LEFT;
									enemys[i].number_of_thrown_objects++;
								};
							};
						};
					};
				}
				else
				{
					if(return_value!=last_return_value && (environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_ON_TOUCH) && enemys[i].state<ENEMY_STATE_THROWN_OBJECT)
					{
						if(!(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/scorpy.png")) enemys[i].anim_state=0;
						enemys[i].state=ENEMY_STATE_ATTACKING;
					};
				};

				if(environment->enemy_types[enemys[i].type].throwing_type && enemys[i].number_of_thrown_objects!=0)
				{
					for(int j=0;j<enemys[i].number_of_thrown_objects;++j)
					{
						SDL_Surface *enemy_surf;
						int tmp_enemy_anim_state;
						if(enemys[i].thrown_directions[j]==ENEMY_WALKING_LEFT)
						{
							enemy_surf=environment->enemy_types[enemys[i].type].image;
						}
						else
						{
							enemy_surf=environment->enemy_types[enemys[i].type].image_right;
						};
						if(boss_fight && environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
						{
							if(enemys[i].thrown_directions[j]==ENEMY_WALKING_LEFT)
							{
								tmp_enemy_anim_state=0;
							}
							else
							{
								tmp_enemy_anim_state=environment->enemy_types[enemys[i].type].animation_speed;
							};
						}
						else
						{
							tmp_enemy_anim_state=enemys[i].anim_state;
						};
						if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y,enemy_surf,environment->enemy_types[enemys[i].type].size_x*(tmp_enemy_anim_state/environment->enemy_types[enemys[i].type].animation_speed),ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, enemys[i].thrown_x_pos[j], enemys[i].thrown_y_pos[j]))
						{
							if(elem_id!=ELEMENT_WIND)
							{
								return_value+=environment->enemy_types[enemys[i].type].hit_damage;
								if(player_state!=PLAYER_STATE_HIT)
								{
									if((player_pos_x+(player_size_x/2))>(enemys[i].thrown_x_pos[j]+(environment->enemy_types[enemys[i].type].size_x/2)))
									{
										hit_direction=DIRECTION_RIGHT;
									}
									else
									{
										hit_direction=DIRECTION_LEFT;
									};
								};
								enemys[i].number_of_thrown_objects--;
								for(int u=j;u<enemys[i].number_of_thrown_objects;++u)
								{
									enemys[i].thrown_directions[u]=enemys[i].thrown_directions[u+1];
									enemys[i].thrown_x_pos[u]=enemys[i].thrown_x_pos[u+1];
									enemys[i].thrown_y_pos[u]=enemys[i].thrown_y_pos[u+1];
								};
							};
						};
					};
				};
				if(environment->enemy_types[enemys[i].type].throwing_type && enemys[i].number_of_thrown_objects!=0)
				{
					if(enemys[i].anim_state%environment->enemy_types[enemys[i].type].throw_freq_x==0)
					{
						for(int z=0;z<enemys[i].number_of_thrown_objects;++z)
						{
							if(enemys[i].thrown_directions[z]==ENEMY_DIRECTION_RIGHT)
							{
								enemys[i].thrown_x_pos[z]+=environment->enemy_types[enemys[i].type].throwspeed_x;
							}
							else
							{
								enemys[i].thrown_x_pos[z]-=environment->enemy_types[enemys[i].type].throwspeed_x;
							};
						};
					};
					if(enemys[i].anim_state%environment->enemy_types[enemys[i].type].throw_freq_y==0)
					{
						for(int z=0;z<enemys[i].number_of_thrown_objects;++z)
						{
							enemys[i].thrown_y_pos[z]+=environment->enemy_types[enemys[i].type].throwspeed_y;
						};
					};

					for(int z=0;z<enemys[i].number_of_thrown_objects;++z)
					{
						if(enemys[i].thrown_x_pos[z]+environment->enemy_types[enemys[i].type].size_x<0 || enemys[i].thrown_x_pos[z]>map_size_x*environment->tile_size_x || enemys[i].thrown_y_pos[z]+environment->enemy_types[enemys[i].type].size_y<0 || enemys[i].thrown_y_pos[z]>map_size_y*environment->tile_size_y || ( !(boss_fight && environment->bg_image_filename=="images/spider_bg.png") && thrown_object_collides(i,z)))
						{
							enemys[i].number_of_thrown_objects--;
							for(int u=z;u<enemys[i].number_of_thrown_objects;++u)
							{
								enemys[i].thrown_directions[u]=enemys[i].thrown_directions[u+1];
								enemys[i].thrown_x_pos[u]=enemys[i].thrown_x_pos[u+1];
								enemys[i].thrown_y_pos[u]=enemys[i].thrown_y_pos[u+1];
							};
						};
						if(boss_fight && environment->bg_image_filename=="images/spider_bg.png" && enemys[i].thrown_x_pos[0]+environment->enemy_types[enemys[i].type].size_x/2<player_pos_x+player_size_x/2)
						{
								environment->enemy_types[enemys[i].type].throwspeed_x=0;
								environment->enemy_types[enemys[i].type].throwspeed_y=1;
								environment->enemy_types[enemys[i].type].throw_freq_y=2;
						};
						if(boss_fight && environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
						{
							if(!(player_pos_x>enemys[i].thrown_x_pos[z]+environment->enemy_types[enemys[i].type].size_x || player_pos_x+player_size_x<enemys[i].thrown_x_pos[z] || player_pos_y>enemys[i].thrown_y_pos[z]+environment->enemy_types[enemys[i].type].size_y || player_pos_y+player_size_y<enemys[i].thrown_y_pos[z]))
							{
								enemys[i].thrown_directions[z]=ENEMY_DIRECTION_RIGHT;
							};
						};
					};
				};

				if(!environment->enemy_types[enemys[i].type].flying_type)
				{
					int falling_speed=2;
					while(falling_speed>0 && enemy_collides(i,falling_speed)) falling_speed--;
					if(falling_speed>0)
					{
						enemys[i].pos_y+=falling_speed;
					}
					else
					{
						int tmp_x_pos;
						if(enemys[i].walking_direction==ENEMY_WALKING_LEFT)
						{
							tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].forwards_correction_value;
						}
						else
						{
							tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].size_x-environment->enemy_types[enemys[i].type].forwards_correction_value;
						};
						if(tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value+1)/environment->tile_size_y][tmp_x_pos/environment->tile_size_x]==0)
						{
							enemys[i].walking_direction=!enemys[i].walking_direction;
						};
					};
				}
				else
				{
					int tmp_x_pos;
					if(enemys[i].walking_direction==ENEMY_WALKING_LEFT)
					{
						tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].forwards_correction_value;
					}
					else
					{
						tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].size_x-environment->enemy_types[enemys[i].type].forwards_correction_value;
					};
					if(enemys[i].state<ENEMY_STATE_THROWN_OBJECT && (tmp_x_pos/environment->tile_size_x)+1<tiles[0].size())
					{
						if((tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value)/environment->tile_size_y][tmp_x_pos/environment->tile_size_x]==0 && tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value)/environment->tile_size_y][(tmp_x_pos/environment->tile_size_x)+1]!=0)|| (tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value)/environment->tile_size_y][tmp_x_pos/environment->tile_size_x]!=0 && tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value)/environment->tile_size_y][(tmp_x_pos/environment->tile_size_x)+1]==0))
						{
							enemys[i].walking_direction=!enemys[i].walking_direction;
						};
					};
				};

				if(enemys[i].anim_state%environment->enemy_types[enemys[i].type].move_frequency==0 && !enemys[i].hit && enemys[i].state<ENEMY_STATE_THROWN_OBJECT)
				{
					int tmp_speed=environment->enemy_types[enemys[i].type].movespeed;
					if(!(environment->enemy_types[enemys[i].type].image_filename=="images/icy_cave/yeti.png" && enemys[i].pos_x>player_pos_x) && tmp_speed!=0) //Yeah, another one of those very bad tricks ;-);
					{
						if(environment->enemy_types[enemys[i].type].image_filename=="images/icy_cave/yeti.png" && enemys[i].walking_direction==ENEMY_DIRECTION_LEFT)
						{
							enemys[i].walking_direction=ENEMY_DIRECTION_RIGHT;
						};
						if(enemys[i].walking_direction==ENEMY_WALKING_LEFT)
						{
							while(tmp_speed>0 && enemys[i].pos_x-tmp_speed<0) tmp_speed--;
							enemys[i].pos_x-=tmp_speed;
						}
						else
						{
							while(tmp_speed>0 && enemys[i].pos_x+environment->enemy_types[enemys[i].type].size_x-environment->enemy_types[enemys[i].type].forwards_correction_value+tmp_speed>map_size_x*environment->tile_size_x) tmp_speed--;
								enemys[i].pos_x+=tmp_speed;
						};

						if(tmp_speed==0) enemys[i].walking_direction=!enemys[i].walking_direction;

						if(boss_fight)
						{
							if(enemys[0].pos_y>20 && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/draki_vaultkeeper.png")
							{
								--enemys[0].pos_y;
							}
							else
							{
								if(enemys[0].pos_y<140 && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/draki_vaultkeeper_walking.png")
								{
									++enemys[0].pos_y;
								}
								else
								{
									if(environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_transformed.png")
									{
										if(player_state!=PLAYER_STATE_HIT && rand()%4==0)
										{
											if(enemys[0].pos_y<player_pos_y && enemys[0].pos_y<290)
											{
												if(rand()%3!=0) ++enemys[0].pos_y;
											}
											else
											{
												if(rand()%3!=0) --enemys[0].pos_y;
											};
											if(enemys[0].pos_x<player_pos_x && enemys[0].pos_x<600)
											{
												if(rand()%3==0) ++enemys[0].pos_x;
												enemys[0].walking_direction=ENEMY_WALKING_RIGHT;
											}
											else
											{
												if(rand()%3==0) --enemys[0].pos_x;
												enemys[0].walking_direction=ENEMY_WALKING_LEFT;
											};
										};
									};
								};
							};
						};
					};
				};

				enemys[i].anim_state++;
				if(enemys[i].state<ENEMY_STATE_THROWN_OBJECT)
				{
					enemys[i].anim_state%=environment->enemy_types[enemys[i].type].animation_speed*4;

					if(boss_fight && i==0 && environment->enemy_types[enemys[0].type].image_filename=="images/burning_desert/scorpy.png" && enemys[0].anim_state==0)
					{
						++enemys[0].number_of_thrown_objects;
						if(enemys[0].number_of_thrown_objects>=3 && !enemys[0].hit)
						{
							enemys[i].state=ENEMY_STATE_THROWN_OBJECT;
							enemys[0].number_of_thrown_objects=0;
						};
					};
				}
				else
				{
					if(boss_fight && environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
					{
						int tmp_enemy_offset, tmp_enemy_x;
						if(enemys[i].state==ENEMY_STATE_SPECIAL1)
						{
							tmp_enemy_offset=environment->enemy_types[enemys[i].type].size_x*2;
							tmp_enemy_x=enemys[i].anim_state;
						}
						else
						{
							tmp_enemy_offset=environment->enemy_types[enemys[i].type].size_x*3;
							tmp_enemy_x=map_size_x*environment->tile_size_x-(enemys[i].anim_state+environment->enemy_types[enemys[i].type].size_x);
						};
						if(elem_id!=ELEMENT_WIND && image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y,enemy_surf,tmp_enemy_offset,ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x, environment->enemy_types[enemys[i].type].size_y, tmp_enemy_x, enemys[i].pos_y))
						{
							return_value+=environment->enemy_types[enemys[i].type].hit_damage;
							if(player_state!=PLAYER_STATE_HIT)
							{
								if(enemys[i].state==ENEMY_STATE_SPECIAL1)
								{
									hit_direction=DIRECTION_RIGHT;
								}
								else
								{
									hit_direction=DIRECTION_LEFT;
								};
							};
						};
						enemys[i].anim_state++;
						if(enemys[i].anim_state>=map_size_x*environment->tile_size_x)
						{
							enemys[i].pos_y=rand()%(map_size_y*environment->tile_size_y-2*environment->enemy_types[enemys[i].type].size_y);
							enemys[i].pos_x=rand()%(map_size_x*environment->tile_size_x-environment->enemy_types[enemys[i].type].size_x);
							enemys[i].anim_state=1;
							enemys[i].state=ENEMY_STATE_WALKING;
						};
					}
					else
					{
						if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/lavly.png")
						{
							if(enemys[i].state==ENEMY_STATE_SPECIAL1 && enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*5)
							{
								enemys[i].state=ENEMY_STATE_SPECIAL2;
							}
							else
							{
								if(enemys[i].state==ENEMY_STATE_SPECIAL2 && enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*5)
								{
									++enemys[i].number_of_thrown_objects;
									if(enemys[i].number_of_thrown_objects==4)
									{
										enemys[i].number_of_thrown_objects=0;
										enemys[i].state=ENEMY_STATE_SPECIAL3;
									};
								}
								else
								{
									if(enemys[i].state==ENEMY_STATE_SPECIAL3 && enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*5)
									{
										enemys[i].state=ENEMY_STATE_SPECIAL4;
									}
									else
									{
										if(enemys[i].state==ENEMY_STATE_SPECIAL4 && enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*5)
										{
											enemys[i].state=ENEMY_STATE_THROWN_OBJECT;
										}
										else
										{
											if(enemys[i].state==ENEMY_STATE_THROWN_OBJECT && enemys[i].anim_state>=environment->enemy_types[enemys[i].type].animation_speed*5)
											{
												++enemys[i].number_of_thrown_objects;
												if(enemys[i].number_of_thrown_objects==8)
												{
													enemys[i].number_of_thrown_objects=0;
													enemys[i].state=ENEMY_STATE_SPECIAL1;
												};
											};
										};
									};
								};
							};
							enemys[i].anim_state%=environment->enemy_types[enemys[i].type].animation_speed*5;

							if(enemys[i].state>ENEMY_STATE_THROWN_OBJECT && image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y,enemy_surf,(environment->enemy_types[enemys[i].type].image->w/5)*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),(enemys[i].state-ENEMY_STATE_SPECIAL1)*environment->enemy_types[enemys[i].type].image->h/4,environment->enemy_types[enemys[i].type].image->w/5,environment->enemy_types[enemys[i].type].image->h/4, enemys[i].pos_x, enemys[i].pos_y))
							{
								return_value+=environment->enemy_types[enemys[i].type].hit_damage;
								hit_direction=DIRECTION_LEFT;

							};
						}
						else
						{
							if(boss_fight && environment->enemy_types[enemys[0].type].image_filename=="images/burning_desert/scorpy.png")
							{
								enemys[0].anim_state%=environment->enemy_types[enemys[0].type].animation_speed*4;
								if(enemys[0].state==ENEMY_STATE_SPECIAL1 && enemys[0].anim_state!=0 && enemys[0].anim_state+1<environment->enemy_types[enemys[0].type].animation_speed*4) ++enemys[0].anim_state;
								if(enemys[0].anim_state==0)
								{
									++enemys[0].number_of_thrown_objects;
									if(enemys[0].state==ENEMY_STATE_THROWN_OBJECT)
									{
										if(enemys[0].number_of_thrown_objects==2)
										{
											++enemys[0].state;
											enemys[0].number_of_thrown_objects=0;
										};
									}
									else
									{
										if(enemys[0].state==ENEMY_STATE_SPECIAL1)
										{
											if(enemys[0].number_of_thrown_objects==6)
											{
												++enemys[0].state;
												enemys[0].number_of_thrown_objects=0;
											};
										}
										else
										{
											if(enemys[0].state==ENEMY_STATE_SPECIAL2)
											{
												if(enemys[0].number_of_thrown_objects==3)
												{
													enemys[0].state=ENEMY_STATE_WALKING;
													enemys[0].number_of_thrown_objects=0;
												};
											};
										};
									};
								};
							};
						};
					};
				};


				if(enemys[i].hit && enemys[i].state<ENEMY_STATE_THROWN_OBJECT)
				{
					if(enemys[i].anim_state<player_size_x/2 && enemys[i].anim_state%2==0)
					{
						int push_back_speed;
						if(enemys[i].anim_state<player_size_x/6)
						{
							push_back_speed=3;
						}
						else
						{
							if(enemys[i].anim_state<player_size_x/4)
							{
								push_back_speed=2;
							}
							else
							{
								push_back_speed=1;
							};
						};

						if((boss_fight && i==0 && environment->bg_image_filename=="images/spider_bg.png") || (boss_fight && (environment->bg_image_filename=="images/beating_jungle/lavly_bg.png" || environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_1.png" || environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_2.png" || environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_3.png")) || (environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/spitting_volcano.png" || environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/hot_lava.png")) //really cheap trick to get to know which boss i am fighting with ;-) *ashamed* ;-);
						{
							push_back_speed=0;
						};

						int tmp_x_pos;
						if(enemys[i].hit_direction==DIRECTION_LEFT)
						{
							if(enemys[i].pos_x<push_back_speed)
							{
								enemys[i].pos_x=0;
							}
							else
							{
								enemys[i].pos_x-=push_back_speed;
							};
							tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].forwards_correction_value;
						}
						else
						{
							if(enemys[i].pos_x+environment->enemy_types[enemys[i].type].size_x-environment->enemy_types[enemys[i].type].forwards_correction_value+push_back_speed<map_size_x*environment->tile_size_x)
							{
								enemys[i].pos_x+=push_back_speed;
							};
							tmp_x_pos=enemys[i].pos_x+environment->enemy_types[enemys[i].type].size_x-environment->enemy_types[enemys[i].type].forwards_correction_value;
						};

						if(!environment->enemy_types[enemys[i].type].flying_type && tiles[(enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y-environment->enemy_types[enemys[i].type].downwards_correction_value+1)/environment->tile_size_y][tmp_x_pos/environment->tile_size_x]==0)
						{
							if(enemys[i].hit_direction==DIRECTION_LEFT)
							{
								enemys[i].pos_x+=push_back_speed;
							}
							else
							{
								enemys[i].pos_x-=push_back_speed;
							};
						};
					};
					if(environment->enemy_types[enemys[i].type].flying_type && enemys[i].state==ENEMY_STATE_VANISHING && enemys[i].anim_state%3==0 && (enemys[i].pos_y+environment->enemy_types[enemys[i].type].size_y+1)/environment->tile_size_y<tiles.size() && !(boss_fight && i==0 && environment->bg_image_filename=="images/spider_bg.png") && environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/spitting_volcano.png" && environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/hot_lava.png") //Yeah i will say it once more ;-) this is bad and i fucking know it ;-)
					{
						enemys[i].pos_y++;
					};
					if(enemys[i].anim_state==0)
					{
						if(enemys[i].state==ENEMY_STATE_VANISHING && enemys[i].remaining_resistance<1 && environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/spitting_volcano.png" &&environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/hot_lava.png") //yeah yeah fucking bad i know ;-) ;-);
						{
							enemys.erase(enemys.begin()+i);
							i--;
							continue;
						};
						if(environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/spitting_volcano.png" && environment->enemy_types[enemys[i].type].image_filename!="images/beating_jungle/hot_lava.png")
						{
							enemys[i].hit=false;
							enemys[i].state=ENEMY_STATE_WALKING;
						}
						else
						{
							--enemys[i].remaining_resistance;
							if(enemys[i].remaining_resistance<-20) //in this case i abuse the remaining_resistance variable in order to count down the time for the melting lava ;-); sorry ;-);
							{
								enemys[i].hit=false;
								enemys[i].state=ENEMY_STATE_WALKING;
								enemys[i].remaining_resistance=1;
							};
						};
					};
				};

				if(environment->enemy_types[enemys[i].type].attacking_pattern & ATTACKING_REGULARY)
				{
					if(enemys[i].anim_state==0 && enemys[i].state<ENEMY_STATE_THROWN_OBJECT)
					{
						enemys[i].attacking_counter++;
						if(enemys[i].attacking_counter==environment->enemy_types[enemys[i].type].attacking_frequency)
						{
							if(!(environment->enemy_types[enemys[i].type].throwing_type && enemys[i].number_of_thrown_objects>MAX_THROWN_OBJECTS))
							{
								enemys[i].attacking_counter=0;
								enemys[i].state=ENEMY_STATE_ATTACKING;
							};

							if(boss_fight)
							{
								if(environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/draki_vaultkeeper.png")
								{
									if(rand()%3==0)
									{
										--enemys[i].type;
									};
								}
								else
								{
									if(environment->enemy_types[enemys[i].type].image_filename=="images/foggy_castle/draki_vaultkeeper_walking.png")
									{
										if(rand()%4==0)
										{
											++enemys[i].type;
										};
									}
									else
									{
										if(environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_1.png")
										{
											if(rand()%2==0)
											{
												++enemys[0].type;
											};
										}
										else
										{
											if(environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_2.png")
											{
												if(rand()%2==0)
												{
													--enemys[0].type;
													enemys[0].number_of_thrown_objects=0;
												}
												else
												{
													if(rand()%3==0)
													{
														++enemys[0].type;
														enemys[0].number_of_thrown_objects=0;
													};
												};
											}
											else
											{
												if(environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_transformation.png")
												{
													++enemys[0].type;
												}
												else
												{
													if(environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/beast_king_transformed.png")
													{
														enemys[0].type=0;
														enemys[0].pos_y=100;
														enemys[0].pos_x=100;
													};
												};
											};
										};
									};
								};
							};
						};
					}
					else
					{
						if(boss_fight && enemys[0].state==ENEMY_STATE_ATTACKING &&  environment->enemy_types[enemys[i].type].image_filename=="images/icy_cave/yeti_boss.png" && enemys[0].anim_state/(environment->enemy_types[enemys[0].type].animation_speed)==3)
						{
							shaking=true;
							if(player_pos_y>90)
							{
								return_value+=environment->enemy_types[enemys[0].type].hit_damage;
								if(player_state!=PLAYER_STATE_HIT)
								{
									if((player_pos_x+(player_size_x/2))>(enemys[i].pos_x+(environment->enemy_types[enemys[i].type].size_x/2)))
									{
										hit_direction=DIRECTION_RIGHT;
										enemys[i].walking_direction=ENEMY_DIRECTION_RIGHT;
									}
									else
									{
										hit_direction=DIRECTION_LEFT;
										enemys[i].walking_direction=ENEMY_DIRECTION_LEFT;
									};
								};
							};
						};
					};
				};

				if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_1.png") //i wont comment on that anymore ;_; ;_; ;
				{
					enemys[i].walking_direction=ENEMY_DIRECTION_RIGHT;
				};
				if(boss_fight && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/crystal_2.png")
				{
					enemys[i].walking_direction=ENEMY_DIRECTION_LEFT;
				};
			};

			return return_value;
		};

		int update_items(SDL_Surface *player_image, int player_pos_x, int player_pos_y, int player_size_x, int player_size_y, int player_state, int player_anim_state)
		{
			for(int i=0;i<items.size();++i)
			{
				if(image_frames_collide(player_image,player_size_x*(player_anim_state/32),player_state*player_size_y,player_size_x, player_size_y,player_pos_x, player_pos_y,environment->item_sets[environment->item_types[items[i].type_id].set_id].image,environment->item_types[items[i].type_id].tile_id*environment->item_sets[environment->item_types[items[i].type_id].set_id].size_x,(items[i].anim_state/items[i].anim_speed)*environment->item_sets[environment->item_types[items[i].type_id].set_id].size_y,environment->item_sets[environment->item_types[items[i].type_id].set_id].size_x,environment->item_sets[environment->item_types[items[i].type_id].set_id].size_y,items[i].pos_x,items[i].pos_y))
				{
					if(environment->item_types[items[i].type_id].activation_type==ITEM_ACTIVATE_INVENTORY)
					{
						int tmp_id=items[i].type_id;
						items.erase(items.begin()+i);
						return tmp_id;
					}
					else
					{
						if(environment->item_types[items[i].type_id].activation_type==ITEM_ACTIVATE_INSTANT)
						{
							if(environment->item_types[items[i].type_id].item_action==ITEM_ACTION_ONE_UP)
							{
								items.erase(items.begin()+i);
								return -1;
							};
						};
					};
				};
			};

			return -2;
		};

		bool enemys_remaining()
		{
			if(environment->bg_image_filename=="images/beating_jungle/lavly_bg.png") //no comment ;-);
			{
				return ((enemys.size()-1)>0);
			}
			else
			{
				return (enemys.size()>0);
			};
		};

		bool boss_dead()
		{
			if(enemys.size()==0 || enemys[0].remaining_resistance>0) return false;
			return true;
		};

		void reset_scroll_positions()
		{
			tile_scroll_x=0;
			tile_scroll_y=0;
			bg_image_scroll_x=0;
			if(environment->bg_image!=NULL && map_size_y*environment->tile_size_y==environment->bg_image->h)
			{
				bg_image_scroll_y=environment->bg_image->h-240;
			}
			else
			{
				bg_image_scroll_y=0;
			};
			bg_cloud_scroll_x=0;
			fg_image_scroll_x=0;
		};

		void scroll(int direction) //0=left,1=right,2=up,3=down;
		{
			if(direction==0)
			{
				bg_image_scroll_x-=bg_image_scrollspeed_x;
				fg_image_scroll_x-=fg_image_scrollspeed_x;
				tile_scroll_x-=tile_scrollspeed_x;

				scroll_counter-=tile_scrollspeed_x;
			}
			else
			{
				if(direction==1)
				{
					bg_image_scroll_x+=bg_image_scrollspeed_x;
					fg_image_scroll_x+=fg_image_scrollspeed_x;
					tile_scroll_x+=tile_scrollspeed_x;

					scroll_counter+=tile_scrollspeed_x;
				}
				else
				{
					if(direction==2)
					{
						bg_image_scroll_y+=bg_image_scrollspeed_y;
						tile_scroll_y+=tile_scrollspeed_y;
					}
					else
					{
						bg_image_scroll_y-=bg_image_scrollspeed_y;
						tile_scroll_y-=tile_scrollspeed_y;
					};
				};
			};
			if(bg_image_scroll_y>environment->bg_image->h-240) bg_image_scroll_y=environment->bg_image->h-240;
			if(bg_image_scroll_y<0) bg_image_scroll_y=0;
			if(fg_image!=NULL) fg_image_scroll_x%=fg_image->w;
		};

		void scroll(int direction,int distance) //0=left,1=right,2=up,3=down;
		{
			if(direction==0)
			{
				bg_image_scroll_x-=bg_image_scrollspeed_x*(distance/tile_scrollspeed_x);
				fg_image_scroll_x-=fg_image_scrollspeed_x*(distance/tile_scrollspeed_x);
				tile_scroll_x-=distance;

				scroll_counter-=distance;
			}
			else
			{
				if(direction==1)
				{
					bg_image_scroll_x+=bg_image_scrollspeed_x*(distance/tile_scrollspeed_x);
					fg_image_scroll_x+=fg_image_scrollspeed_x*(distance/tile_scrollspeed_x);
					tile_scroll_x+=distance;

					scroll_counter+=distance;
				}
				else
				{
					if(direction==2)
					{
 						bg_image_scroll_y+=bg_image_scrollspeed_y*(distance/tile_scrollspeed_y);
						tile_scroll_y+=distance;
					}
					else
					{
 						bg_image_scroll_y-=bg_image_scrollspeed_y*(distance/tile_scrollspeed_y);
						tile_scroll_y-=distance;
					};
				};
			};
			if(bg_image_scroll_y>environment->bg_image->h-240) bg_image_scroll_y=environment->bg_image->h-240;
			if(bg_image_scroll_y<0) bg_image_scroll_y=0;
			if(fg_image!=NULL) fg_image_scroll_x%=fg_image->w;
		};


		void draw(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			if(shaking)
			{
				draw_x+=(rand()%10)-5;
				draw_y+=(rand()%10)-5;
			};

			if(environment->bg_image!=NULL) 
			{
				if(environment->bg_image_filename=="images/icy_cave/yeti_bg.png") //yeah once more i use an extremly dirty and ugly trick ;-);
				{
					bg_image_scroll_x-=4;
					bg_image_scroll_y-=3;
					bg_image_scroll_x%=environment->bg_image->w;
					bg_image_scroll_y%=environment->bg_image->h;
					BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x,draw_y+bg_image_scroll_y);
					BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x+320,draw_y+bg_image_scroll_y+240);
				}
				else
				{
					BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%environment->bg_image->w,draw_y+bg_image_scroll_y-(environment->bg_image->h-240));
					if(bg_image_scroll_x<0)
					{
						BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%environment->bg_image->w+environment->bg_image->w,draw_y+bg_image_scroll_y-(environment->bg_image->h-240));
					}
					else
					{
						BlitSurface(environment->bg_image,0,0,environment->bg_image->w,environment->bg_image->h,buffer,draw_x+bg_image_scroll_x%(environment->bg_image->w+1)-environment->bg_image->w,draw_y+bg_image_scroll_y-(environment->bg_image->h-240));
					};
				};
			};

			if(environment->bg_cloud_image!=NULL)
			{
					if(cloud_move_freq==0)
					{
						BlitSurface(environment->bg_cloud_image,0,0,environment->bg_cloud_image->w,environment->bg_cloud_image->h,buffer,0,0);
					}
					else
					{
						BlitSurface(environment->bg_cloud_image,0,0,environment->bg_cloud_image->w,environment->bg_cloud_image->h,buffer,draw_x+bg_cloud_scroll_x,draw_y+bg_image_scroll_y-(environment->bg_image->h-240));
						if(bg_cloud_scroll_x<0)
						{
							BlitSurface(environment->bg_cloud_image,0,0,environment->bg_cloud_image->w,environment->bg_cloud_image->h,buffer,draw_x+bg_cloud_scroll_x+environment->bg_cloud_image->w,draw_y+bg_image_scroll_y-(environment->bg_image->h-240));
						};
					};
			};

			if(fog_image!=NULL)
			{
				BlitSurface(fog_image,0,0,fog_image->w,fog_image->h,buffer,(fog_pos_x/10),100);
				if((fog_pos_x/10)>320-fog_image->w)
				{
					BlitSurface(fog_image,0,0,fog_image->w,fog_image->h,buffer,(fog_pos_x/10)-320,100);
				};
			};

			int draw_start_x, draw_start_y, draw_stop_x, draw_stop_y;
			draw_start_x=tile_scroll_x/(-environment->tile_size_x);
			draw_start_y=tile_scroll_y/(-environment->tile_size_y);
			if(draw_start_x<0)
			{
				draw_start_x=0;
			};
			if(draw_start_y<0)
			{
				draw_start_y=0;
			};
			draw_stop_x=draw_start_x+(320/environment->tile_size_x)+1;
			draw_stop_y=draw_start_y+(240/environment->tile_size_y)+1;
			if(draw_stop_x>bg_tiles[0].size())
			{
				draw_stop_x=bg_tiles[0].size();
			};
			if(draw_stop_y>bg_tiles.size())
			{
				draw_stop_y=bg_tiles.size();
			};

			for(int y=draw_start_y;y<draw_stop_y;++y)
			{
				for(int x=draw_start_x;x<draw_stop_x;++x)
				{
					if(bg_tiles[y][x]!=0)
					{
						BlitSurface(environment->tileset,((bg_tiles[y][x]-1)*environment->tile_size_x)%environment->tileset->w,(((bg_tiles[y][x]-1)*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,buffer,draw_x+tile_scroll_x+x*environment->tile_size_x,draw_y+tile_scroll_y+y*environment->tile_size_y);
					};
				};
			};

			for(int i=0;i<bg_anims.size();++i)
			{
				BlitSurface(environment->animation_sets[bg_anims[i].set_id].image,bg_anims[i].tile_id*environment->animation_sets[bg_anims[i].set_id].size_x,(bg_anims[i].state/bg_anims[i].anim_speed)*environment->animation_sets[bg_anims[i].set_id].size_y,environment->animation_sets[bg_anims[i].set_id].size_x,environment->animation_sets[bg_anims[i].set_id].size_y,buffer,draw_x+tile_scroll_x+bg_anims[i].pos_x,draw_y+tile_scroll_y+bg_anims[i].pos_y);

				bg_anims[i].state++;
				if(bg_anims[i].state/bg_anims[i].anim_speed==environment->animation_sets[bg_anims[i].set_id].number_of_states)
				{
					bg_anims[i].state=0;
				};
			};

			for(int y=draw_start_y;y<draw_stop_y;++y)
			{
				for(int x=draw_start_x;x<draw_stop_x;++x)
				{
					if(tiles[y][x]!=0)
					{
						BlitSurface(environment->tileset,((tiles[y][x]-1)*environment->tile_size_x)%environment->tileset->w,(((tiles[y][x]-1)*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,buffer,draw_x+tile_scroll_x+x*environment->tile_size_x,draw_y+tile_scroll_y+y*environment->tile_size_y);
					};
				};
			};

			for(int i=0;i<anims.size();++i)
			{
				BlitSurface(environment->animation_sets[anims[i].set_id].image,anims[i].tile_id*environment->animation_sets[anims[i].set_id].size_x,(anims[i].state/anims[i].anim_speed)*environment->animation_sets[anims[i].set_id].size_y,environment->animation_sets[anims[i].set_id].size_x,environment->animation_sets[anims[i].set_id].size_y,buffer,draw_x+tile_scroll_x+anims[i].pos_x,draw_y+tile_scroll_y+anims[i].pos_y);

				anims[i].state++;
				if(anims[i].state/anims[i].anim_speed==environment->animation_sets[anims[i].set_id].number_of_states)
				{
					anims[i].state=0;
				};
			};

			for(int i=0;i<special_objects.size();++i)
			{
				if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_MOVING_PLATFORM)
				{
					BlitSurface(environment->special_object_types[special_objects[i].type_id].image,0,(environment->special_object_types[special_objects[i].type_id].image->h/9)*(special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/8)),environment->special_object_types[special_objects[i].type_id].image->w,environment->special_object_types[special_objects[i].type_id].image->h/9,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
				}
				else
				{
					if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_FLOATING_PLATFORM)
					{
						BlitSurface(environment->special_object_types[special_objects[i].type_id].image,0,0,environment->special_object_types[special_objects[i].type_id].image->w,environment->special_object_types[special_objects[i].type_id].image->h,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
						if(!paused)
						{
							if(special_objects[i].anim_state==FLOATING_DIRECTION_LEFT)
							{
								if(tiles[(special_objects[i].pos_y/environment->tile_size_y)+1][(special_objects[i].pos_x-1)/environment->tile_size_x]==0)
								{
									special_objects[i].pos_x--;
								}
								else
								{
									special_objects[i].anim_state=FLOATING_DIRECTION_RIGHT;
								};
							}
							else
							{
								if(tiles[(special_objects[i].pos_y/environment->tile_size_y)+1][(special_objects[i].pos_x+environment->special_object_types[special_objects[i].type_id].image->w+1)/environment->tile_size_x]==0)
								{
									special_objects[i].pos_x++;
								}
								else
								{
									special_objects[i].anim_state=FLOATING_DIRECTION_LEFT;
								};
							};
						};
					}
					else
					{
						if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_VANISHING)
						{
							BlitSurface(environment->special_object_types[special_objects[i].type_id].image,0,0,environment->special_object_types[special_objects[i].type_id].image->w,environment->special_object_types[special_objects[i].type_id].image->h,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
						}
						else
						{
							if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_LIANA)
							{
								BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/7)*(special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/7)),0,environment->special_object_types[special_objects[i].type_id].image->w/7,environment->special_object_types[special_objects[i].type_id].image->h/5,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
							}
							else
							{
								if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_FOUNTAIN_ROCK)
								{
									switch(special_objects[i].anim_state/((environment->special_object_types[special_objects[i].type_id].max_anim_state)/3))
									{
										case 0:
										{
											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/4)*(special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/12)),0,environment->special_object_types[special_objects[i].type_id].image->w/4,environment->special_object_types[special_objects[i].type_id].image->h/3,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);

											++special_objects[i].anim_state;
											special_objects[i].anim_state%=(environment->special_object_types[special_objects[i].type_id].max_anim_state/3);
											break;
										};
										case 1:
										{
											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/4)*((special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/12))-4),environment->special_object_types[special_objects[i].type_id].image->h/3,environment->special_object_types[special_objects[i].type_id].image->w/4,environment->special_object_types[special_objects[i].type_id].image->h/3,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);

											++special_objects[i].anim_state;
											if(special_objects[i].anim_state>=(environment->special_object_types[special_objects[i].type_id].max_anim_state/3)*2)
												special_objects[i].anim_state=(environment->special_object_types[special_objects[i].type_id].max_anim_state/3);
											break;
										};
										case 2:
										{

											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/4)*((special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/12))-8),0,environment->special_object_types[special_objects[i].type_id].image->w/4,environment->special_object_types[special_objects[i].type_id].image->h/3,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);

											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/4)*((special_objects[i].anim_state/(environment->special_object_types[special_objects[i].type_id].max_anim_state/12))-8),2*(environment->special_object_types[special_objects[i].type_id].image->h/3),environment->special_object_types[special_objects[i].type_id].image->w/4,environment->special_object_types[special_objects[i].type_id].image->h/3,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y-15);

											++special_objects[i].anim_state;
											if(special_objects[i].anim_state>=environment->special_object_types[special_objects[i].type_id].max_anim_state)
												special_objects[i].anim_state=(environment->special_object_types[special_objects[i].type_id].max_anim_state/3)*2;
											break;
										};
									};
								}
								else
								{
									if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_CHEST)
									{
										if(special_objects[i].anim_state!=0)
										{
											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,environment->special_object_types[special_objects[i].type_id].image->w/2,0,environment->special_object_types[special_objects[i].type_id].image->w/2,environment->special_object_types[special_objects[i].type_id].image->h,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
										}
										else
										{
											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,0,0,environment->special_object_types[special_objects[i].type_id].image->w/2,environment->special_object_types[special_objects[i].type_id].image->h,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
										};
									}
									else
									{
										if(environment->special_object_types[special_objects[i].type_id].type==OBJECT_TYPE_RISING_PLATFORM)
										{
											BlitSurface(environment->special_object_types[special_objects[i].type_id].image,(environment->special_object_types[special_objects[i].type_id].image->w/4)*((special_objects[i].anim_state%(environment->special_object_types[special_objects[i].type_id].max_anim_state/2))/(environment->special_object_types[special_objects[i].type_id].max_anim_state/8)),0,environment->special_object_types[special_objects[i].type_id].image->w/4,environment->special_object_types[special_objects[i].type_id].image->h,buffer,draw_x+tile_scroll_x+special_objects[i].pos_x,draw_y+tile_scroll_y+special_objects[i].pos_y);
											if(!paused)
											{
												++special_objects[i].anim_state;
												int tmp_speed=0;
												if(special_objects[i].anim_state>environment->special_object_types[special_objects[i].type_id].max_anim_state/2)
												{
													special_objects[i].anim_state%=environment->special_object_types[special_objects[i].type_id].max_anim_state/2;
													special_objects[i].anim_state+=environment->special_object_types[special_objects[i].type_id].max_anim_state/2;
													--tmp_speed;
												}
												else
												{
													special_objects[i].anim_state%=environment->special_object_types[special_objects[i].type_id].max_anim_state/2;
													++tmp_speed;
												};

												if(special_objects[i].pos_y+tmp_speed-50<0 || tiles[((special_objects[i].pos_y+tmp_speed)/environment->tile_size_y)+1][(special_objects[i].pos_x)/environment->tile_size_x+1]!=0 || tiles[((special_objects[i].pos_y+environment->special_object_types[special_objects[i].type_id].image->h+tmp_speed)/environment->tile_size_y)+1][((special_objects[i].pos_x+environment->special_object_types[special_objects[i].type_id].image->w/4)/environment->tile_size_x)+1]!=0)
												{
													special_objects[i].anim_state+=environment->special_object_types[special_objects[i].type_id].max_anim_state/2;
													special_objects[i].anim_state%=environment->special_object_types[special_objects[i].type_id].max_anim_state;
												};

												special_objects[i].pos_y+=tmp_speed;
											};
										};
									};
								};
							};
						};
					};
				};
			};

			for(int i=0;i<items.size();++i)
			{
				BlitSurface(environment->item_sets[environment->item_types[items[i].type_id].set_id].image,environment->item_types[items[i].type_id].tile_id*environment->item_sets[environment->item_types[items[i].type_id].set_id].size_x,(items[i].anim_state/items[i].anim_speed)*environment->item_sets[environment->item_types[items[i].type_id].set_id].size_y,environment->item_sets[environment->item_types[items[i].type_id].set_id].size_x,environment->item_sets[environment->item_types[items[i].type_id].set_id].size_y,buffer,draw_x+tile_scroll_x+items[i].pos_x,draw_y+tile_scroll_y+items[i].pos_y);

				items[i].anim_state++;
				if(items[i].anim_state/items[i].anim_speed==environment->item_sets[environment->item_types[items[i].type_id].set_id].number_of_animations)
				{
					items[i].anim_state=0;
				};
			};

			for(int i=0;i<enemys.size();++i)
			{
				if(environment->enemy_types[enemys[i].type].throwing_type && enemys[i].number_of_thrown_objects!=0)
				{
					for(int j=0;j<enemys[i].number_of_thrown_objects;++j)
					{
						if(environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
						{
							if(enemys[i].thrown_directions[j]==ENEMY_WALKING_LEFT)
							{
								BlitSurface(environment->enemy_types[enemys[i].type].image,0,ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].thrown_x_pos[j]+tile_scroll_x,enemys[i].thrown_y_pos[j]+tile_scroll_y);
							}
							else
							{
								BlitSurface(environment->enemy_types[enemys[i].type].image,environment->enemy_types[enemys[i].type].size_x,ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].thrown_x_pos[j]+tile_scroll_x,enemys[i].thrown_y_pos[j]+tile_scroll_y);
							};
						}
						else
						{
							if(enemys[i].thrown_directions[j]==ENEMY_WALKING_LEFT)
							{
								BlitSurface(environment->enemy_types[enemys[i].type].image,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].thrown_x_pos[j]+tile_scroll_x,enemys[i].thrown_y_pos[j]+tile_scroll_y);
							}
							else
							{
								BlitSurface(environment->enemy_types[enemys[i].type].image_right,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].thrown_x_pos[j]+tile_scroll_x,enemys[i].thrown_y_pos[j]+tile_scroll_y);
							};
						};
					};
				};

				if(i==0 && enemys[0].pos_y<140 && enemys[0].remaining_resistance>0 && environment->enemy_types[enemys[0].type].image_filename=="images/foggy_castle/draki_vaultkeeper_walking.png")
				{
					BlitSurface(environment->enemy_types[enemys[i].type+1].image,environment->enemy_types[enemys[i].type+1].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type+1].animation_speed),ENEMY_STATE_VANISHING*environment->enemy_types[enemys[i].type+1].size_y,environment->enemy_types[enemys[i].type+1].size_x,environment->enemy_types[enemys[i].type+1].size_y,buffer,enemys[i].pos_x+tile_scroll_x,enemys[i].pos_y+tile_scroll_y);
					continue;
				};

				if(!(i==0 && environment->bg_image_filename=="images/spider_bg.png" && enemys[0].remaining_resistance<1))
				{
					if(enemys[i].state<ENEMY_STATE_THROWN_OBJECT || (i==0 && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/scorpy.png"))
					{
						bool scorpy=false;
						if(i==0 && environment->enemy_types[enemys[i].type].image_filename=="images/burning_desert/scorpy.png")
						{
							scorpy=true;
							if(enemys[0].state==ENEMY_STATE_ATTACKING) enemys[0].state=ENEMY_STATE_WALKING;
							else if(enemys[0].state==ENEMY_STATE_VANISHING) enemys[0].state=5;
							else if(enemys[0].hit) enemys[i].state=ENEMY_STATE_ATTACKING;
							else if(enemys[0].state>=ENEMY_STATE_THROWN_OBJECT) --enemys[0].state;
						};

						if(enemys[i].walking_direction==ENEMY_WALKING_LEFT)
						{
							BlitSurface(environment->enemy_types[enemys[i].type].image,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].pos_x+tile_scroll_x,enemys[i].pos_y+tile_scroll_y);
						}
						else
						{
							BlitSurface(environment->enemy_types[enemys[i].type].image_right,environment->enemy_types[enemys[i].type].size_x*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),enemys[i].state*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].pos_x+tile_scroll_x,enemys[i].pos_y+tile_scroll_y);
						};

						if(scorpy)
						{
							if(enemys[0].state==5) enemys[0].state=ENEMY_STATE_VANISHING;
							else if(enemys[0].state==ENEMY_STATE_ATTACKING) enemys[0].state=ENEMY_STATE_WALKING;
							else if(enemys[0].state+1>=ENEMY_STATE_THROWN_OBJECT && enemys[0].remaining_resistance>0) ++enemys[0].state;
						};
					}
					else
					{
						if(environment->bg_image_filename=="images/windy_mountainpath/wingly_bg.png")
						{
							if(enemys[i].state==ENEMY_STATE_SPECIAL1)
							{
								BlitSurface(environment->enemy_types[enemys[i].type].image,environment->enemy_types[enemys[i].type].size_x*2,ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,enemys[i].anim_state+tile_scroll_x,enemys[i].pos_y+tile_scroll_y);
							}
							else
							{

								BlitSurface(environment->enemy_types[enemys[i].type].image,environment->enemy_types[enemys[i].type].size_x*3,ENEMY_STATE_THROWN_OBJECT*environment->enemy_types[enemys[i].type].size_y,environment->enemy_types[enemys[i].type].size_x,environment->enemy_types[enemys[i].type].size_y,buffer,tile_scroll_x+(map_size_x*environment->tile_size_x-(enemys[i].anim_state+environment->enemy_types[enemys[i].type].size_x)),enemys[i].pos_y+tile_scroll_y);
							};
						}
						else
						{
							if(environment->enemy_types[enemys[i].type].image_filename=="images/beating_jungle/lavly.png")
							{
								if(enemys[i].state>ENEMY_STATE_THROWN_OBJECT && enemys[i].remaining_resistance>0)
								{
									BlitSurface(environment->enemy_types[enemys[i].type].image,(environment->enemy_types[enemys[i].type].image->w/5)*(enemys[i].anim_state/environment->enemy_types[enemys[i].type].animation_speed),(enemys[i].state-ENEMY_STATE_SPECIAL1)*environment->enemy_types[enemys[i].type].image->h/4,environment->enemy_types[enemys[i].type].image->w/5,environment->enemy_types[enemys[i].type].image->h/4,buffer,enemys[i].pos_x+tile_scroll_x,enemys[i].pos_y+tile_scroll_y);
								};
							};
						};
					};
				};
			};
		};

		void draw_fg_layer(SDL_Surface *buffer, int draw_x, int draw_y)
		{
			if(shaking)
			{
				draw_x+=(rand()%10)-5;
				draw_y+=(rand()%10)-5;
			};

			int draw_start_x, draw_start_y, draw_stop_x, draw_stop_y;
			draw_start_x=tile_scroll_x/(-environment->tile_size_x);
			draw_start_y=tile_scroll_y/(-environment->tile_size_y);
			draw_stop_x=draw_start_x+(320/environment->tile_size_x)+1;
			draw_stop_y=draw_start_y+(240/environment->tile_size_y)+1;
			if(draw_start_x<0)
			{
				draw_start_x=0;
			};
			if(draw_start_y<0)
			{
				draw_start_y=0;
			};
			if(draw_stop_x>fg_tiles[0].size())
			{
				draw_stop_x=fg_tiles[0].size();
			};
			if(draw_stop_y>fg_tiles.size())
			{
				draw_stop_y=fg_tiles.size();
			};

			for(int y=draw_start_y;y<draw_stop_y;++y)
			{
				for(int x=draw_start_x;x<draw_stop_x;++x)
				{
					if(fg_tiles[y][x]!=0)
					{
						BlitSurface(environment->tileset,((fg_tiles[y][x]-1)*environment->tile_size_x)%environment->tileset->w,(((fg_tiles[y][x]-1)*environment->tile_size_x)/environment->tileset->w)*environment->tile_size_y,environment->tile_size_x,environment->tile_size_y,buffer,draw_x+tile_scroll_x+x*environment->tile_size_x,draw_y+tile_scroll_y+y*environment->tile_size_y);
					};
				};
			};

			for(int i=0;i<fg_anims.size();++i)
			{
				BlitSurface(environment->animation_sets[fg_anims[i].set_id].image,fg_anims[i].tile_id*environment->animation_sets[fg_anims[i].set_id].size_x,(fg_anims[i].state/fg_anims[i].anim_speed)*environment->animation_sets[fg_anims[i].set_id].size_y,environment->animation_sets[fg_anims[i].set_id].size_x,environment->animation_sets[fg_anims[i].set_id].size_y,buffer,draw_x+tile_scroll_x+fg_anims[i].pos_x,draw_y+tile_scroll_y+fg_anims[i].pos_y);

				fg_anims[i].state++;
				if(fg_anims[i].state/fg_anims[i].anim_speed==environment->animation_sets[fg_anims[i].set_id].number_of_states)
				{
					fg_anims[i].state=0;
				};
			};

			if(weather.image!=NULL)
			{
				for(int i=0;i<weather.pos_x.size();++i)
				{
					if(weather.pos_y[i]+weather.image->h>0)
					{
						BlitSurface(weather.image,0,(weather.image->h/weather.number_of_anim_states)*(weather.anim_state[i]/weather.anim_speed),weather.image->w,weather.image->h/weather.number_of_anim_states,buffer,weather.pos_x[i],weather.pos_y[i]);
					};

					weather.anim_state[i]++;
					if(weather.anim_state[i]/weather.anim_speed==weather.number_of_anim_states)
					{
						weather.anim_state[i]=0;
					};
					if(weather.anim_state[i]%4==0)
					{
						weather.pos_y[i]++;
						if(weather.image->format->colorkey==SDL_MapRGB(weather.image->format,106,197,255)) weather.pos_x[i]--; //only happens for foggy castles rain drop ;-);
					};
					if(weather.pos_y[i]>240 || (weather.pos_x[i]<0 && weather.pos_y[i]>0))
					{
						if(weather.image->format->colorkey==SDL_MapRGB(weather.image->format,106,197,255))
						{
							weather.pos_x[i]=rand()%600;
						}
						else
						{
							weather.pos_x[i]=rand()%320;
						};
						weather.pos_y[i]=-40-(rand()%400);
					};
				};
			};

			if(fog_image!=NULL)
			{
				BlitSurface(fog_image,0,0,fog_image->w,fog_image->h,buffer,320-((fog_pos_x/10)+fog_image->w),120);
				if((fog_pos_x/10)+fog_image->w>320)
				{
					BlitSurface(fog_image,0,0,fog_image->w,fog_image->h,buffer,640-((fog_pos_x/10)+fog_image->w),120);
				};
				++fog_pos_x;
				fog_pos_x%=3200;
			};

			if(fg_image!=NULL)
			{
				if(environment->enemy_types[enemys[0].type].image_filename=="images/icy_cave/yeti_boss.png")
				{
					BlitSurface(fg_image,0,0,fg_image->w,fg_image->h,buffer,draw_x,draw_y);
					return;
				};

				int fg_y=fg_image_scrolls?map_size_y*environment->tile_size_y-fg_image->h+tile_scroll_y:0;
				BlitSurface(fg_image,0,0,fg_image->w,fg_image->h,buffer,fg_image_scroll_x,fg_y);
				if(fg_image_scroll_x<0)
				{
					BlitSurface(fg_image,0,0,fg_image->w,fg_image->h,buffer,fg_image_scroll_x+fg_image->w,fg_y);
				}
				else
				{
					if(fg_image_scroll_x>0)
					{
						BlitSurface(fg_image,0,0,fg_image->w,fg_image->h,buffer,fg_image_scroll_x-fg_image->w,fg_y);
					};
				};
			};
		};
};

void load_sounds()
{
	jump_sound=Mix_LoadWAV("sounds/jump.wav");
	hit_sound=Mix_LoadWAV("sounds/hit.wav");
	got_item_sound=Mix_LoadWAV("sounds/item.wav");
	one_up_sound=Mix_LoadWAV("sounds/oneup.wav");
	enemy_death_sound=Mix_LoadWAV("sounds/enemy_death.wav");
	heal_sound=Mix_LoadWAV("sounds/heal.wav");
	punsh_sound=Mix_LoadWAV("sounds/punsh.wav");
	attack_sound=Mix_LoadWAV("sounds/attack.wav");
	thunder_sound=Mix_LoadWAV("sounds/thunder.wav");

	Mix_VolumeChunk(jump_sound,20);
	Mix_VolumeChunk(hit_sound,20);
	Mix_VolumeChunk(got_item_sound,20);
	Mix_VolumeChunk(one_up_sound,20);
	Mix_VolumeChunk(enemy_death_sound,20);
	Mix_VolumeChunk(heal_sound,20);
	Mix_VolumeChunk(punsh_sound,20);
	Mix_VolumeChunk(attack_sound,20);
	Mix_VolumeChunk(thunder_sound,40);
};
void unload_sounds()
{
	Mix_FreeChunk(jump_sound);
	Mix_FreeChunk(hit_sound);
	Mix_FreeChunk(got_item_sound);
	Mix_FreeChunk(one_up_sound);
	Mix_FreeChunk(enemy_death_sound);
	Mix_FreeChunk(heal_sound);
	Mix_FreeChunk(punsh_sound);
	Mix_FreeChunk(attack_sound);
	Mix_FreeChunk(thunder_sound);
};

bool game_over_continue()
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *bg=LoadImage("images/game_over_bg.png");
	SDL_Surface *continue_txt=LoadImage("images/continue.png");
	SDL_SetColorKey(continue_txt,SDL_SRCCOLORKEY,SDL_MapRGB(continue_txt->format,255,255,255));
	SDL_Surface *selection_txts[2];
	selection_txts[0]=LoadImage("images/continue_no.png");
	selection_txts[1]=LoadImage("images/continue_yes.png");
	int buttons;

	bool selected=true;
	bool right_released=true;
	bool left_released=true;

	for(int i=0;i<80;i+=2)
	{
		SDL_SetAlpha(bg,SDL_SRCALPHA,i);
		BlitSurface(bg,0,0,bg->w,bg->h,buffer,0,0);
		SDL_Flip(buffer);
		limit_frames(60);
	};
	SDL_SetAlpha(bg,SDL_SRCALPHA,255);

	buttons=get_button_state();
	while(!((buttons & BTN_MENU) || buttons & BTN_B))
	{
		buttons=get_button_state();
		
		BlitSurface(bg,0,0,bg->w,bg->h,buffer,0,0);
		BlitSurface(continue_txt,0,0,continue_txt->w,continue_txt->h,buffer,65,170);
		BlitSurface(selection_txts[selected],0,0,selection_txts[selected]->w,selection_txts[selected]->h,buffer,40,190);

		SDL_Flip(buffer);
		limit_frames(60);

		if(!left_released && !(buttons & BTN_LEFT)) left_released=true;
		if(!right_released && !(buttons & BTN_RIGHT)) right_released=true;

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
	};

	SDL_FreeSurface(bg);
	SDL_FreeSurface(selection_txts[0]);
	SDL_FreeSurface(selection_txts[1]);
	SDL_FreeSurface(continue_txt);
	return selected;
};

void show_text_box(std::string txt,int x, int y, int duration)
{
	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *txtbox=LoadImage("images/textbox.png");

	BlitSurface(txtbox,0,0,txtbox->w,txtbox->h,buffer,x,y);
	stringRGBA(buffer,x+11,y+14,txt.c_str(),255,255,255,255);

	SDL_Flip(buffer);
	SDL_Delay(duration);

	SDL_FreeSurface(txtbox);
};

void lightning(SDL_Surface *srf)
{
	Uint8 r, g, b;
	SDL_LockSurface(srf);
	Uint16 *pixels=(Uint16*)srf->pixels;
	for(int x=0;x<320;++x)
	{
		for(int y=24;y<240;++y) //the first 24 pixels are covered by the lifebar ;-);
		{
			SDL_GetRGB(pixels[(y*320)+x],srf->format,&r,&g,&b);
			if(r>55) r=255;
			else r+=200;
			if(g>55) g=255;
			else g+=200;
			if(b>55) b=255;
			else b+=200;
			pixels[(y*320)+x]=SDL_MapRGB(srf->format,r,g,b);
		};
	};
	SDL_UnlockSurface(srf);
};
