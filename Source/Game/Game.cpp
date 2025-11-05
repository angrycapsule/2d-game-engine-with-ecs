#include "Game.h"

#include <filesystem>
#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"

static std::string GetAssetPath(const std::string& rel);

Game::Game() : isRunning(false), millisecondsPreviousFrame(0)
{
	Logger::Log("Game constructor called!");
}

Game::~Game()
{
	Logger::Log("Game destructor called!");
}

void Game::Initialize()
{
	isRunning = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error initializating SDL!");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = displayMode.w / 2;
	windowHeight = displayMode.h / 2;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		Logger::Err("Error creating SDL window!");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		Logger::Err("Error creating SDL renderer!");
		return;
	}

	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}

void Game::Setup()
{
	
}

void Game::Run() 
{
	Setup();

	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			break;
		}
	}
}

void Game::Update()
{
	// If need FPS clamping
	/*
	int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
	
	if (timeToWait > 0.0f && timeToWait <= MILLISECONDS_PER_FRAME)
	{
		SDL_Delay(timeToWait);
	}
	*/

	double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

	millisecondsPreviousFrame = SDL_GetTicks();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	const std::string assetPath = GetAssetPath("Assets/Images/tank-tiger-right.png");

	/*
	SDL_Surface* surface = IMG_Load(assetPath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = { 
		10, 
		20, 
		32, 
		32 };

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);

	SDL_DestroyTexture(texture);
	*/

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static std::string GetAssetPath(const std::string& rel)
{
	namespace fs = std::filesystem;
	fs::path root = PROJECT_SOURCE_DIR;
	return (root / rel).string();
}