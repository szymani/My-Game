#ifndef _Menu
#define _Menu

#include"LTexture.h"
#include"Music.h"
#include<vector>
#include<fstream>
#include<algorithm>
#include<iostream>

enum States
{
	Start,
	ClassChoise,
	Scoreboard,
	Name,
	PauseMenu,
	totalStates
};
struct Choices
{
	LTexture texture;
	int y;
	SDL_Color color;
	std::string text;
};
class CMenu: public LTexture,public CMusic
{
protected:
	CMusic music;
	int characterChoice = 0;
	int state = Start;
	int initialY = 200;
	Choices start;							// niestety gdy uzywalem vektora , load fromRenderedText nie chcialo dzialac
	Choices scoreboard;
	Choices exitGame;
	Choices warrior;
	Choices dwarf;
	Choices mage;
	Choices turnMusic;
	Choices resume;

	LTexture additionalTexture;
	LTexture nameTexture;
	LTexture scoreboardTexture;
	std::string inputText = "";
	Choices characterInfo;
	SDL_Color normalColor;
	SDL_Color pickedColor;
	std::vector<std::pair<std::string, int>> scoreboardVector;

public:
	CMenu();
	int startMenu(int _choice);
	void renderMenu();
	bool menuEvents(SDL_Event& e);
	void menuLoop();
	void loadMenuTextures();
	void loadScoreboard();
	void saveScoreboard(int score);
	void renderStartScreen();

	void checkAllHighlight(int &mouseX, int &mouseY);
	bool checkAllClick(int &mouseX, int &mouseY);
	bool checkIfClicked(LTexture & texture, int &mouseX, int &mouseY, int &textureY);
};
CMenu::CMenu()
{
	normalColor = { 0xFF, 0, 0, 0xFF };
	pickedColor = { 0xFF, 0xFF, 0xFF, 0xFF };
}
int CMenu::startMenu(int _choice)
{
	if (_choice == 0)
	{
		state = Start;
		renderStartScreen();
	}	
	else
		state = PauseMenu;
	loadScoreboard();
	loadMenuTextures();
	menuLoop();
	return characterChoice;
}

void CMenu::menuLoop()
{
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)		//Handle events on queue
		{		
			if (e.type == SDL_QUIT)			//User requests quit
			{
				exit(0);
			}
			quit = menuEvents(e);
		}
		renderMenu();	
	}
	if (state != PauseMenu)
	{
		loadMusic("3-09 Enspiron.mp3");
		startPauseMusic(-1);
	}
}
bool CMenu::menuEvents(SDL_Event& e)
{

	if (state == Name)
	{	
		if (e.type == SDL_TEXTINPUT)			//Text input
		{		
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
					inputText += e.text.text;
		}	
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)	//Handle backspace
				inputText.pop_back();
			else if (e.key.keysym.sym == SDLK_RETURN)
				return true;
		}				
	}
	else
	{
			if (e.type == SDL_MOUSEMOTION)
			{
				int x, y;							//Get mouse position
				SDL_GetMouseState(&x, &y);
				checkAllHighlight(x,y);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)		
			{		
				int x, y;							//Get mouse position
				SDL_GetMouseState(&x, &y);
				return checkAllClick(x, y);
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.repeat == 0 && e.key.keysym.sym == SDLK_ESCAPE && state != 0 && state != PauseMenu)
					state = 0;
			}
	}	
	return false;
}
void CMenu::loadMenuTextures()
{
	start.text = "Start Game";		start.color = normalColor;		start.y = initialY;
	start.texture.loadFromRenderedText(start.text, start.color, menuFont);

	scoreboard.text = "Scoreboard";		scoreboard.color = normalColor;		scoreboard.y = initialY+75;
	scoreboard.texture.loadFromRenderedText(scoreboard.text, scoreboard.color, menuFont);

	exitGame.text = "I give up...";		exitGame.color = normalColor;		exitGame.y = initialY + 150;
	exitGame.texture.loadFromRenderedText(exitGame.text, exitGame.color, menuFont);

	additionalTexture.loadFromRenderedText("Chose your hero", normalColor, menuFont);
	nameTexture.loadFromRenderedText("How should we remember you", normalColor, menuFont);

	warrior.text = "Warrior";		warrior.color = normalColor;		warrior.y = initialY + 75;
	warrior.texture.loadFromRenderedText(warrior.text, warrior.color, menuFont);

	dwarf.text = "Guard";		dwarf.color = normalColor;		dwarf.y = initialY + 150;
	dwarf.texture.loadFromRenderedText(dwarf.text, dwarf.color, menuFont);


	std::string tempS;
	for (int i = 0; i < scoreboardVector.size(); i++)
		tempS = tempS + scoreboardVector[i].first + "         " + std::to_string(scoreboardVector[i].second) + "\n";

	scoreboardTexture.loadFromRenderedText(tempS, normalColor, menuFont);

	turnMusic.text = "Music on off";	turnMusic.color = normalColor;	turnMusic.y = initialY + 75;
	turnMusic.texture.loadFromRenderedText(turnMusic.text, turnMusic.color, menuFont);
	
	resume.text = "Resume";	resume.color = normalColor; resume.y = initialY;
	resume.texture.loadFromRenderedText(resume.text, resume.color, menuFont);
}
void CMenu::renderMenu()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);

	if (state == Start)
	{
		start.texture.renderTexture((SCREEN_WIDTH - start.texture.getWidth()) / 2, start.y);
		scoreboard.texture.renderTexture((SCREEN_WIDTH - scoreboard.texture.getWidth()) / 2, scoreboard.y);
		exitGame.texture.renderTexture((SCREEN_WIDTH - exitGame.texture.getWidth()) / 2, exitGame.y);
	}
	else if (state == ClassChoise)
	{
		additionalTexture.renderTexture((SCREEN_WIDTH - additionalTexture.getWidth()) / 2, initialY);
		warrior.texture.renderTexture((SCREEN_WIDTH - warrior.texture.getWidth()) / 2, warrior.y);
		dwarf.texture.renderTexture((SCREEN_WIDTH - dwarf.texture.getWidth()) / 2, dwarf.y);
		characterInfo.texture.renderTexture(SCREEN_WIDTH/2 + 200, characterInfo.y);

	}
	else if (state == Scoreboard)
	{
		scoreboardTexture.renderTexture((SCREEN_WIDTH - 300) / 2, initialY);
	}
	else if (state == Name)
	{
		nameTexture.renderTexture((SCREEN_WIDTH - nameTexture.getWidth()) / 2, initialY);
		characterInfo.text = inputText;
		characterInfo.texture.loadFromRenderedText(inputText, normalColor, menuFont);
		characterInfo.texture.renderTexture(SCREEN_WIDTH / 2-150, characterInfo.y);
	}
	else if (state == PauseMenu)
	{
		turnMusic.texture.renderTexture((SCREEN_WIDTH - turnMusic.texture.getWidth()) / 2, turnMusic.y);
		resume.texture.renderTexture((SCREEN_WIDTH - resume.texture.getWidth()) / 2, resume.y);
		exitGame.texture.renderTexture((SCREEN_WIDTH - exitGame.texture.getWidth()) / 2, exitGame.y);
	}
	SDL_RenderPresent(gRenderer);
}
void CMenu::renderStartScreen()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);
	start.text = "Slay the Dragon";		start.color = normalColor;		start.y = (SCREEN_HEIGHT-start.texture.getHeight())/2-100;
	start.texture.loadFromRenderedText(start.text, start.color, titleFont);
	start.texture.renderTexture((SCREEN_WIDTH - start.texture.getWidth()) / 2, start.y);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(3000);
}
void CMenu::checkAllHighlight(int &mouseX, int &mouseY)
{
	if (state == Start)
	{
		if (checkIfClicked(start.texture, mouseX, mouseY, start.y))
			start.color = pickedColor;
		else
			start.color = normalColor;
		start.texture.loadFromRenderedText(start.text, start.color, menuFont);

		if (checkIfClicked(scoreboard.texture, mouseX, mouseY, scoreboard.y))
			scoreboard.color = pickedColor;
		else
			scoreboard.color = normalColor;
		scoreboard.texture.loadFromRenderedText(scoreboard.text, scoreboard.color, menuFont);

		if (checkIfClicked(exitGame.texture, mouseX, mouseY, exitGame.y))
			exitGame.color = pickedColor;
		else
			exitGame.color = normalColor;
		exitGame.texture.loadFromRenderedText(exitGame.text, exitGame.color, menuFont);
	}
	else if (state == ClassChoise)
	{
		if (checkIfClicked(warrior.texture, mouseX, mouseY, warrior.y))
		{
			dwarf.color = normalColor;
			warrior.color = pickedColor;
			characterInfo.y = initialY + 75;
			characterInfo.texture.loadFromRenderedText("Mighty attack , but low hp", pickedColor, menuFont);
		}			
		else if(checkIfClicked(dwarf.texture, mouseX, mouseY, dwarf.y))
		{
			warrior.color = normalColor;
			dwarf.color = pickedColor;
			characterInfo.y = initialY + 150;
			characterInfo.texture.loadFromRenderedText("Durable , but low attack", pickedColor, menuFont);
		}
		else
		{
			warrior.color = normalColor;
			dwarf.color = normalColor;
			characterInfo.texture.loadFromRenderedText("", pickedColor, menuFont);
		}
			
		warrior.texture.loadFromRenderedText(warrior.text, warrior.color, menuFont);
		dwarf.texture.loadFromRenderedText(dwarf.text, dwarf.color, menuFont);
	}
	else if (state == PauseMenu)
	{
		if (checkIfClicked(turnMusic.texture, mouseX, mouseY, turnMusic.y))
		{
			resume.color = normalColor;
			turnMusic.color = pickedColor;
			exitGame.color = normalColor;
		}
		else if (checkIfClicked(resume.texture, mouseX, mouseY, resume.y))
		{
			resume.color = pickedColor;
			turnMusic.color = normalColor;
			exitGame.color = normalColor;
		}
		else if (checkIfClicked(exitGame.texture, mouseX, mouseY, exitGame.y))
		{
			resume.color = normalColor;
			turnMusic.color = normalColor;
			exitGame.color = pickedColor;
		}
		else
		{
			resume.color = normalColor;
			turnMusic.color = normalColor;
			exitGame.color = normalColor;
		}
		resume.texture.loadFromRenderedText(resume.text, resume.color, menuFont);
		turnMusic.texture.loadFromRenderedText(turnMusic.text, turnMusic.color, menuFont);
		exitGame.texture.loadFromRenderedText(exitGame.text, exitGame.color, menuFont);
	}

}
bool CMenu::checkAllClick(int &mouseX, int &mouseY)
{
	if (state == Start)
	{
		if (checkIfClicked(start.texture, mouseX, mouseY, start.y))
			state = ClassChoise;

		if (checkIfClicked(scoreboard.texture, mouseX, mouseY, scoreboard.y))
			state = Scoreboard;

		if (checkIfClicked(exitGame.texture, mouseX, mouseY, exitGame.y))
			exit(0);
	}
	else if (state == ClassChoise)
	{
		if (checkIfClicked(warrior.texture, mouseX, mouseY, warrior.y) )
		{
			state = Name;
			characterChoice = 0;
			characterInfo.y = warrior.y;
		}
		else if (checkIfClicked(dwarf.texture, mouseX, mouseY, dwarf.y))
		{
			state = Name;
			characterChoice = 1;
			characterInfo.y = warrior.y;
		}
	}
	else if (state == PauseMenu)
	{
		if (checkIfClicked(turnMusic.texture, mouseX, mouseY, turnMusic.y))
			music.startPauseMusic();

		else if (checkIfClicked(resume.texture, mouseX, mouseY, resume.y))
			return true;

		else if (checkIfClicked(exitGame.texture, mouseX, mouseY, exitGame.y))
			exit(0);
	}
	return false;
}
bool CMenu::checkIfClicked(LTexture & texture,int &mouseX,int &mouseY,int &textureY)
{
	if (mouseX >= (SCREEN_WIDTH / 2) - texture.getWidth() && mouseX <= (SCREEN_WIDTH / 2) + texture.getWidth()
		&& mouseY >= textureY && mouseY <= textureY + texture.getHeight())
		return true;
	return false;
}
void CMenu::loadScoreboard()
{
	std::pair < std::string, int > tempP;
	std::string tempS;
	std::fstream file;
	file.open("scoreboard.txt", std::ios::in);
	if (file.is_open() == 1)
	{
		while (!file.eof())
		{
			getline(file, tempS, ';');
			tempP.first = tempS;
			getline(file, tempS, '\n');
			if (tempS.size() != 0)
				tempP.second = stoi(tempS);
			else
				break;
			scoreboardVector.push_back(tempP);
		}
		file.close();
	}
}
bool cmp(std::pair<std::string, int> &a, std::pair<std::string, int> &b)
{
	if (a.second > b.second)
		return true;
	return false;
}
void CMenu::saveScoreboard(int score)
{
	std::fstream file;
	std::pair<std::string, int> tempP;
	tempP.first = inputText;													//Adding new record
	tempP.second = score;
	scoreboardVector.push_back(tempP);
	std::sort(scoreboardVector.begin(), scoreboardVector.end(), cmp);			//sorting	

	file.open("scoreboard.txt", std::ios::out);
	if (file.is_open() == 1)
	{
		for (int i = 0; i < scoreboardVector.size() && i < 8; i++)			//saving top 8
			file << scoreboardVector[i].first << ";"<< scoreboardVector[i].second << "\n";
	}

	file.close();
}

#endif