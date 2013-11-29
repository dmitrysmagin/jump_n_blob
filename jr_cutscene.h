const int TRANSITION_NONE=0;
const int TRANSITION_FADE=1;
const int TRANSITION_FADE_TO_BLACK=2;

struct cutscene_image
{
	int transition, duration;
	SDL_Surface *image;
};

struct cutscene_image_dev
{
	int transition, duration;
	SDL_Surface *image;
	std::string filename;
};

void save_cutscene_to_file(std::string filename, std::vector<cutscene_image_dev> images)
{
	std::ofstream outfile;
	outfile.open(filename.c_str(),std::ios_base::binary);

	int tmp=images.size();

	outfile.write((char*)&tmp,sizeof(int));

	for(int i=0;i<tmp;++i)
	{
		int tmp_int=images[i].filename.size();
		outfile.write((char*)&tmp_int,sizeof(int));
		for(int j=0;j<tmp_int;++j)
		{
			char tmp_char=images[i].filename[j];
			outfile.write((char*)&tmp_char,sizeof(char));
		};
		outfile.write((char*)&images[i].transition,sizeof(int));
		outfile.write((char*)&images[i].duration,sizeof(int));
	};

	outfile.close();
};

void show_cutscene(std::string filename)
{
	std::vector<cutscene_image> images;

	std::ifstream infile;
	infile.open(filename.c_str(),std::ios_base::binary);

	int tmp;
	infile.read((char*)&tmp,sizeof(int));
	images.reserve(tmp);
	for(int i=0;i<tmp;++i)
	{
		cutscene_image new_image;
		std::string tmp_str="";
		int tmp_int;
		infile.read((char*)&tmp_int,sizeof(int));
		for(int j=0;j<tmp_int;++j)
		{
			char tmp_char;
			infile.read((char*)&tmp_char,sizeof(char));
			tmp_str+=tmp_char;
		};
		new_image.image=LoadImage(tmp_str.c_str());
		infile.read((char*)&new_image.transition,sizeof(int));
		infile.read((char*)&new_image.duration,sizeof(int));
		images.push_back(new_image);
	};
	infile.close();

	SDL_Surface *screen=SDL_GetVideoSurface();

	for(int i=0;i<images.size();++i)
	{
		BlitSurface(images[i].image,0,0,320,240,screen,0,0);
		SDL_Flip(screen);
		SDL_Delay(images[i].duration);
		switch(images[i].transition)
		{
			case TRANSITION_FADE:
			{
				if(i+1<images.size())
				{
					for(int j=0;j<255;++j)
					{
						SDL_SetAlpha(images[i+1].image,SDL_SRCALPHA,j);
						BlitSurface(images[i].image,0,0,320,240,screen,0,0);
						BlitSurface(images[i+1].image,0,0,320,240,screen,0,0);
						SDL_Flip(screen);
						limit_frames(60);
					};
				};
				break;
			};
			case TRANSITION_FADE_TO_BLACK:
			{
				for(int j=0;j<255;++j)
				{
					BlitSurface(images[i].image,0,0,320,240,screen,0,0);
					boxRGBA(screen,0,0,320,240,0,0,0,j);
					SDL_Flip(screen);
					limit_frames(60);
				};
				if(i+1<images.size())
				{
					for(int j=255;j>0;--j)
					{
						BlitSurface(images[i+1].image,0,0,320,240,screen,0,0);
						boxRGBA(screen,0,0,320,240,0,0,0,j);
						SDL_Flip(screen);
						limit_frames(60);
					};
				};
				break;
			};
		};
	};

	for(int i=0;i<images.size();++i)
	{
		SDL_FreeSurface(images[i].image);
	};
};
