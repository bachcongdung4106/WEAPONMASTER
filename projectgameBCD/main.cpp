#include "CommonFunction.h"
#include "Object.h"
Object g_background;


bool InitData() 
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret != 0) return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");//tang chat luong phong to hinh anh
	
	g_window = SDL_CreateWindow(//tao cua so
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
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);// tao but ve
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
			int ImgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(ImgFlags) & ImgFlags))
				success = false;

		}
    }
	return success;

	

}
bool LoadBackground() {
	bool ret = g_background.LoadImg("img//background.png", g_screen);
		if (ret == false)
			return false;
		
	return true;

}
void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	if (InitData() == false) return -1;
	if (LoadBackground() == false) return -1;
	bool is_quit = false;
		while (!is_quit)
	{
			while (SDL_PollEvent(&g_event))//trong huong dan là &g_svent
		{
				if (g_event.type == SDL_QUIT)
				{
					is_quit = true;
			    }

		}
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR);
			SDL_RenderClear(g_screen);
			g_background.Render(g_screen, NULL);
			SDL_RenderPresent(g_screen);



	}
	return 0;
}