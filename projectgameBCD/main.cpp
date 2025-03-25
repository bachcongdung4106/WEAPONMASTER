#include "CommonFunction.h"


bool InitData() 
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret != 0) return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	
	g_window = SDL_CreateWindow(
		"GAME PROJECT BCD",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		success = false;

	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
			int ImgFlags = IMG_INIT_PNG;

		}
    }

	

}

int main(int argc, char* argv[]) {
	return 0;
}