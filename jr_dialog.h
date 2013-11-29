struct speech
{
	int x, y;
	std::string txt;
};

void save_dialog_to_file(std::string filename, std::vector<speech> speeches)
{
	std::ofstream outfile;
	outfile.open(filename.c_str(),std::ios_base::binary);

	int tmp=speeches.size();

	outfile.write((char*)&tmp,sizeof(int));

	for(int i=0;i<tmp;++i)
	{
		int tmp_int=speeches[i].txt.size();
		outfile.write((char*)&tmp_int,sizeof(int));
		for(int j=0;j<tmp_int;++j)
		{
			char tmp_char=speeches[i].txt[j];
			outfile.write((char*)&tmp_char,sizeof(char));
		};
		outfile.write((char*)&speeches[i].x,sizeof(int));
		outfile.write((char*)&speeches[i].y,sizeof(int));
	};

	outfile.close();
};

void show_dialog(jr_player *player, jr_map *map, std::string dialog_filename)
{
	std::vector<speech> dialog;
	
	std::ifstream infile;
	infile.open(dialog_filename.c_str(),std::ios_base::binary);

	int tmp;
	infile.read((char*)&tmp,sizeof(int));
	dialog.reserve(tmp);
	for(int i=0;i<tmp;++i)
	{
		speech new_speech;
		new_speech.txt="";
		int tmp_int;
		infile.read((char*)&tmp_int,sizeof(int));
		for(int j=0;j<tmp_int;++j)
		{
			char tmp_char;
			infile.read((char*)&tmp_char,sizeof(char));
			new_speech.txt+=tmp_char;
		};
		infile.read((char*)&new_speech.x,sizeof(int));
		infile.read((char*)&new_speech.y,sizeof(int));
		dialog.push_back(new_speech);
	};
	infile.close();

	SDL_Surface *buffer=SDL_GetVideoSurface();
	SDL_Surface *txtbox=LoadImage("images/textbox.png");
	int displayed_speech=0;

	while(displayed_speech<dialog.size())
	{
		int buttons;
		for(int i=0;i<5000;++i)
		{
			buttons=get_button_state();
			map->draw(buffer,0,0);
			player->draw(buffer);
			map->draw_fg_layer(buffer,0,0);
			player->draw_panel(buffer);

			BlitSurface(txtbox,0,0,txtbox->w,txtbox->h,buffer,dialog[displayed_speech].x,dialog[displayed_speech].y);
			stringRGBA(buffer,dialog[displayed_speech].x+11,dialog[displayed_speech].y+14,dialog[displayed_speech].txt.c_str(),255,255,255,255);

			SDL_Flip(buffer);
			limit_frames(60);
			if(buttons&BTN_B) break;
		};
		displayed_speech++;
		SDL_Delay(200);
	};

	SDL_FreeSurface(txtbox);	
};
