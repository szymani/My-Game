#include"ALLHeaders.h"

bool init();
bool loadMedia(CTile* tiles[]);
void close();

LWindow gWindow;
CAllSceneObjects everything;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		if (!gWindow.init())
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1200) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!everything.loadHeroTexture("allHeroClasses.png"))
	{
		printf("Failed to load walking animation texture!\n");
		success = false;
	}
	//Load tile texture
	if (!gTileTexture.loadFromFile("tiles1.png"))
	{
		printf("Failed to load tile set texture!\n");
		success = false;
	}
	else
	{
		if (!everything.setTiles())
		{
			printf("Failed to load tile set!\n");
			success = false;
		}
	}
	//Load other nature elements
	if (!gElementTexture.loadFromFile("ales.png"))
	{
		printf("Failed to load terrain elements texture!\n");
		success = false;
	}
	else
	{
		if (!everything.setTerrainElements())
		{
			printf("Failed to load terrain elements!\n");
			success = false;
		}
	}
	//Load monsters
	if (!everything.loadMonstersTexture("monsters.png"))
	{
		printf("Failed to load monsters texture!\n");
		success = false;
	}
	else
	{
		if (!everything.setMonsters())
		{
			printf("Failed to load monsters!\n");
			success = false;
		}
	}
	//Load attacks
	if (!gAttackTexture.loadFromFile("attacks.png"))
	{
		printf("Failed to load attack texture!\n");
		success = false;
	}
	//Load Music
	if (!menu.loadMusic("1-03 Buy! Buy! Buy!.mp3"))
	{
		printf("Failed to load music SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	//Load fonts
	gFont = TTF_OpenFont("8bitlimr.ttf", 28);
	menuFont = TTF_OpenFont("Northwood High.ttf", 60);
	iconFont = TTF_OpenFont("Old_Skull_Hellron.ttf", 256);
	titleFont = TTF_OpenFont("Northwood High.ttf", 128);
	if (gFont == NULL||menuFont==NULL||iconFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}
void close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();
	gRenderer = NULL;
	gWindow.free();

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
		printf("Failed to initialize!\n");

	else
	{	
		//Load media
		if (!loadMedia())
			printf("Failed to load media!\n");

		else
		{
			SDL_Event e;				//Event handler

			bool quit = false;
			menu.startPauseMusic();
			everything.settingHero(menu.startMenu(0));		//Menu loop (inside)
			everything.createCamera();			
			while (!quit)
			{
				everything.movement();
				while (SDL_PollEvent(&e) != 0)		//Handle events on queue
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
							menu.startMenu(1);			//pause menu
						
					gWindow.handleEvent(e);
					everything.eventsHandling(e);				
				}
				if (!gWindow.isMinimized())
				{
					everything.renderAll();
					SDL_RenderPresent(gRenderer);			//Update screen
				}
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}