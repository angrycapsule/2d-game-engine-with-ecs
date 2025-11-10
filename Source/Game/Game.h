#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "../ECS/ECS.h"

const int FPS = 144;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	bool isRunning;
	int millisecondsPreviousFrame;
	
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::unique_ptr<Registry> registry;

public:
	int windowWidth;
	int windowHeight;

	Game();
	~Game();

	void Initialize();
	void Setup();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
};

#endif