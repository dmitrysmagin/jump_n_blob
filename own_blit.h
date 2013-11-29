bool BlitSurface(SDL_Surface *bitmap,int x, int y, int w, int h,SDL_Surface *bitmap2, int x2, int y2)
{
	if((x2+w<0 || x2>bitmap2->w) && (y2>bitmap2->h || y2+h<0))
	{
		return false;
	};
	SDL_Rect srect;
	SDL_Rect drect;

	srect.x=x;
	srect.y=y;
	srect.w=w;
	srect.h=h;
	drect.x=x2;
	drect.y=y2;
	drect.w=w;
	drect.h=h;
	SDL_BlitSurface(bitmap,&srect,bitmap2,&drect);

	return true;
};

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[ ( y * surface->w ) + x ];
};

Uint16 get_pixel16( SDL_Surface *surface, int x, int y )
{
	Uint16 *pixels = (Uint16 *)surface->pixels;
	return pixels[ ( y * surface->w ) + x ];
};

Uint8 get_pixel8( SDL_Surface *surface, int x, int y )
{
	Uint8 *pixels = (Uint8 *)surface->pixels;
	return pixels[ ( y * surface->w ) + x ];
};

bool image_frames_collide(SDL_Surface *image1, int startx1, int starty1, int sizex1, int sizey1, int posx1, int posy1, SDL_Surface *image2, int startx2, int starty2, int sizex2, int sizey2, int posx2, int posy2)
{
	int x_end1=posx1+sizex1;
	int y_end1=posy1+sizey1;
	int x_end2=posx2+sizex2;
	int y_end2=posy2+sizey2;

	int offset_x_left, offset_y_left, offset_x_right, offset_y_right;
	int left_draw_x, left_draw_y, right_draw_x, right_draw_y;
	SDL_Surface *right_image, *left_image;

	if((x_end1<posx2 || x_end2<posx1) && (y_end1<posy2 || y_end2<posy1))
	{
		return false;
	};

	int x_start, y_start, x_end, y_end;

	if(posx1>posx2)
	{
		x_start=posx1;
	}
	else
	{
		x_start=posx2;
	};

	if(x_end1<x_end2)
	{
		x_end=x_end1;
	}
	else
	{
		x_end=x_end2;
	};

	if(posy1>posy2)
	{
		y_start=posy1;
	}
	else
	{
		y_start=posy2;
	};

	if(y_end1<y_end2)
	{
		y_end=y_end1;
	}
	else
	{
		y_end=y_end2;
	};

	if(x_start==startx1)
	{
		right_image=image1;
		left_image=image2;
		right_draw_x=posx1;
		right_draw_y=posy1;
		left_draw_x=posx2;
		left_draw_y=posy2;

		offset_x_left=startx2;
		offset_y_left=starty2;
		offset_x_right=startx1;
		offset_y_right=starty1;
	}
	else
	{
		left_image=image1;
		right_image=image2;
		left_draw_x=posx1;
		left_draw_y=posy1;
		right_draw_x=posx2;
		right_draw_y=posy2;

		offset_x_right=startx2;
		offset_y_right=starty2;
		offset_x_left=startx1;
		offset_y_left=starty1;
	};

	Uint32 colorkey_val_left=left_image->format->colorkey;
	Uint32 colorkey_val_right=right_image->format->colorkey;
	SDL_LockSurface(left_image);
	SDL_LockSurface(right_image);
	for(int y=y_start;y<y_end;++y)
	{
		for(int x=x_start;x<x_end;++x)
		{
			if(get_pixel16(left_image,x+offset_x_left-left_draw_x,y+offset_y_left-left_draw_y)!=colorkey_val_left)
			{
				if(get_pixel16(right_image,x+offset_x_right-right_draw_x,y+offset_y_right-right_draw_y)!=colorkey_val_right)
				{
					SDL_UnlockSurface(left_image);
					SDL_UnlockSurface(right_image);
					return true;
				};
			};
		};
	};
	SDL_UnlockSurface(left_image);
	SDL_UnlockSurface(right_image);
	return false;
};

void limit_frames(int frames)
{
	static long int time=-1;
	if(time==-1)
	{
		time=SDL_GetTicks();
		return;
	}
	else
	{
		long int frametime=SDL_GetTicks()-time;
		if(frametime<1000/frames)
		{
			SDL_Delay((1000/frames)-frametime);
		};
		time=SDL_GetTicks();
	};
};
