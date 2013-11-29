#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <assert.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <SDL/SDL_gfxPrimitives.h>
#include "SDL_gfx_5x7_fnt.h"

#include "own_blit.h"
#include "load_image.h"
#include "jr_globals.h"

#include "jr_buttons.h"
#include "jr_cutscene.h"
#include "jr_map.h"
#include "jr_player.h"

#include "jr_dialog.h"
#include "jr_menu.h"

#undef main
int main(int argc, char *argv[])
{
	init_button_mapping();
	gfxPrimitivesSetFont(& SDL_gfx_font_5x7_fnt,5,7);
	SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_JOYSTICK*/ | SDL_INIT_AUDIO);

	Mix_OpenAudio(44100, AUDIO_S16, 2, 1024);
	Mix_VolumeMusic(60);

	SDL_Surface *screen=SDL_SetVideoMode(320,240,16,SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_ShowCursor(false);

	play_2d_rp_intro(screen);

	menu(screen);

	return 0;
};
