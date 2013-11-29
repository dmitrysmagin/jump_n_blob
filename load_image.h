SDL_Surface* LoadImage(std::string image_filename)
{
	SDL_Surface *tmp=IMG_Load(image_filename.c_str());
	SDL_Surface *tmp2=SDL_ConvertSurface(tmp,SDL_GetVideoSurface()->format,SDL_HWSURFACE);
	SDL_FreeSurface(tmp);
	return tmp2;
};


SDL_Surface* LoadImageAlpha(std::string image_filename)
{
	SDL_Surface *tmp=IMG_Load(image_filename.c_str());
	SDL_Surface *tmp2=SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
	return tmp2;
};
