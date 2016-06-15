#ifndef _Music
#define _Music

#include"LWindow.h"

enum Mus
{
	Punch,
	Hit,
	Death
};
class CMusic
{
protected:
	Mix_Music *gMusic = NULL;		//music

	Mix_Chunk *gPunch = NULL;		//sound effects
	Mix_Chunk *gDeath = NULL;
	Mix_Chunk *gHit = NULL;
public:
	bool loadMusic(std::string path);
	void startPauseMusic(int _restart=0);
	void soundEffect(int _choice);
};
bool CMusic::loadMusic(std::string path)
{
	bool success = true;
	//Load music
	gMusic = Mix_LoadMUS(path.c_str());
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	//Load sound effects
	gPunch = Mix_LoadWAV("6.ogg");
	gHit = Mix_LoadWAV("hit1.mp3");
	gDeath = Mix_LoadWAV("videoplayback (mp3cut.net).mp3");
	if ( gDeath == NULL || gHit == NULL || gPunch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	return success;
}
void CMusic::startPauseMusic(int _restart)
{

	if (Mix_PlayingMusic() == 0 || _restart == -1)	//Play the music	
		Mix_PlayMusic(gMusic, -1);
	else	//If music is being played
	{
		if (Mix_PausedMusic() == 1)		//If the music is paused
			Mix_ResumeMusic();			
		else	//If the music is playing			
			Mix_PauseMusic();
	}
}
void CMusic::soundEffect(int _choice)
{
	switch (_choice)
	{
	case Punch:		Mix_PlayChannel(-1,gPunch , 0);		 break;
	case Hit:		Mix_PlayChannel(-1, gHit, 0);		 break;
	case Death:		Mix_PlayChannel(-1, gDeath, 0);		 break;
	}
	
}
#endif