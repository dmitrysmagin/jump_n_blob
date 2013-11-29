SDL_Event event;
SDL_Joystick *joystick;

void exit_buttons()
{
	SDL_JoystickClose(joystick);
};

void init_button_mapping()
{
	SDL_Init(SDL_INIT_JOYSTICK);
	joystick=SDL_JoystickOpen(0);
	std::atexit(exit_buttons);
};

enum
{
     BTN_A = 1<<0,
     BTN_B = 1<<1,
     BTN_X = 1<<2,
     BTN_Y = 1<<3,
     BTN_L = 1<<4,
     BTN_R = 1<<5,
     BTN_MENU = 1<<6,
     BTN_SELECT = 1<<7,
     BTN_LEFT = 1<<8,
     BTN_RIGHT = 1<<9,
     BTN_UP = 1<<10,
     BTN_DOWN = 1<<11,
     BTN_VOLUP = 1<<12,
     BTN_VOLDOWN = 1<<13
};

enum
{
     rBTN_A = 0,
     rBTN_X = 1,
     rBTN_B = 2,
     rBTN_Y = 3,
     rBTN_L = 4,
     rBTN_R = 5,
     rBTN_MENU = 6,
     rBTN_SELECT = 8,
};

long int get_button_state()
{
	long int return_val=0;

	Uint8* keys = SDL_GetKeyState(NULL);

	if(keys[SDLK_LEFT])	return_val|=BTN_LEFT;
	if(keys[SDLK_RIGHT])	return_val|=BTN_RIGHT;
	if(keys[SDLK_UP])	return_val|=BTN_UP;
	if(keys[SDLK_DOWN])	return_val|=BTN_DOWN;
	if(keys[SDLK_SPACE])	return_val|=BTN_A; // make cube
	if(keys[SDLK_LCTRL])	return_val|=BTN_B; // jump
	if(keys[SDLK_LALT])	return_val|=BTN_X; // hit
	if(keys[SDLK_LSHIFT])	return_val|=BTN_Y; // submenu
	if(keys[SDLK_TAB])	return_val|=BTN_L;
	if(keys[SDLK_BACKSPACE])	return_val|=BTN_R;
	if(keys[SDLK_RETURN])	return_val|=BTN_MENU;
	if(keys[SDLK_ESCAPE]) return_val|=BTN_SELECT;
	//if(keys[SDLK_HASH]) return_val|=BTN_VOLUP;
	//if(keys[SDLK_ASTERISK]) return_val|=BTN_VOLDOWN;
	// if(keys[SDLK_BACKSPACE]) SDL_Quit();

	while ( SDL_PollEvent(&event) );
	return return_val;
}

void reset_buttons()
{
	Uint8* keys = SDL_GetKeyState(NULL);

	keys[SDLK_LEFT]		= 0;
	keys[SDLK_RIGHT]		= 0;
	keys[SDLK_UP]		= 0;
	keys[SDLK_DOWN]		= 0;
	keys[SDLK_SPACE]		= 0;
	keys[SDLK_LCTRL]		= 0;
	keys[SDLK_LALT]		= 0;
	keys[SDLK_LSHIFT]		= 0;
	keys[SDLK_TAB]		= 0;
	keys[SDLK_BACKSPACE]		= 0;
	keys[SDLK_RETURN]		= 0;
	keys[SDLK_ESCAPE]		= 0;
}
