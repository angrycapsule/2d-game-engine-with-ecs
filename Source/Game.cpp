#include "Game.h"

#include <filesystem>
#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"

static std::string GetAssetPath(const std::string& rel);

Game::Game()
{
	std::cout << "Game constructor called!" << std::endl;
}

Game::~Game()
{
	std::cout << "Game destructor called!" << std::endl;
}

void Game::Initialize()
{
	isRunning = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initializating SDL!" << std::endl;
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
		std::cerr << "Error creating SDL window!" << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cerr << "Error creating SDL renderer!" << std::endl;
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

}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	const std::string assetPath = GetAssetPath("Assets/Images/tank-tiger-right.png");

	SDL_Surface* surface = IMG_Load(assetPath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = { 10, 10, 32, 32 };

	SDL_RenderCopy(renderer, texture, NULL, &dstRect);

	SDL_DestroyTexture(texture);

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