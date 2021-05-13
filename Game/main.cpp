#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "Window.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Spritesheet.h"
#include "Scene.h"
#include "GameLoop.h"
#include "Input.h"
#include <GameObject.h>
#include <Point.h>
#include "Button.h"
#include "World.h"
#include "Line.h"

using namespace std;

static GameLoop* gameLoop;
static Spritesheet* linkSpr;
static GameObject* link;
static World* gameWorld;

static int fps = 60;
static double fpsSpeedFactor;
static float version = 0.3f;

void startGame() {
	gameWorld->setScene("Game");
}

void backToMenu() {
	gameWorld->setScene("Menu");
}

int main(int argc, char* args[]) {
	//----------SETUP------------
    Window* window = new Window("Dryad", ml16_9);
	Mouse mouse;
	Input input = Input(&mouse);
    World world = World(window->getRenderer(), &input);
	gameWorld = &world;
	Scene gameScene;
	world.addScene(&gameScene, "Game");
	Scene menu;
	world.addScene(&menu, "Menu");
	world.setScene("Menu");
	//TODO: abstract
	world.setDisplayFPS(true);
	world.setDisplayObjectCount(true);
	fpsSpeedFactor = 60.0 / (double)fps;
	GameLoop g = GameLoop(fps, &world, &input);
	gameLoop = &g;
	char* file_home = SDL_GetBasePath();
	//--------END SETUP----------

	//Metadata
	char engineVersionText[40];
	sprintf(engineVersionText, "Dryad v%.1fa, 2021", version);
	Text engineVersionLabel = Text(engineVersionText, world.getDebugFont(), 18, { 255, 255, 255 }, Point(10, 870));
	world.addDebugObject(&engineVersionLabel);

	//Spritesheet
	char* zelda_dir = file_home;
	strcat(zelda_dir, "res/zelda.png");
    vector<char*> images = { zelda_dir };
    vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
	vector<bool> looping = { false, false, false, false, true, true, true, true };
    vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	linkSpr = new Spritesheet(images, framecounts, looping, heights, 120);
	linkSpr->setCurrentAnimation(0);
	link = new GameObject(Point(50, 50), linkSpr);
	gameScene.addObject(link);

	//Button
	Button* buttonTest = new Button(&startGame, "Start", "Bebas.ttf", 24, { 255, 255, 255 }, { 30, 30, 30 }, Point(550, 250), { 200, 100 });
	buttonTest->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 });
	buttonTest->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 });
	menu.addObject(buttonTest);

	Button* buttonTestBack = new Button(&backToMenu, "Menu", "Bebas.ttf", 24, { 255, 255, 255 }, { 30, 30, 30 }, Point(1000, 500), { 200, 100 });
	buttonTestBack->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 });
	buttonTestBack->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 });
	gameScene.addObject(buttonTestBack);

	//Configure inputs
	input.addKeyboardEvent([]() { gameLoop->stop(); }, { make_pair(SDLK_ESCAPE, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() { 
		if (gameWorld->getCurrentSceneName() == "Game") {
			Point p = link->getPosition();
			p.x += 7.0*fpsSpeedFactor;
			link->setPosition(p);
			((Spritesheet*)link->getSprite())->setCurrentAnimation(7);
		}
	}, { make_pair(SDLK_RIGHT, SDL_KEYDOWN)});
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			Point p = link->getPosition();
			p.x -= 7.0 * fpsSpeedFactor;
			link->setPosition(p);
			((Spritesheet*)link->getSprite())->setCurrentAnimation(5);
		}
	}, { make_pair(SDLK_LEFT, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			Point p = link->getPosition();
			p.y -= 7.0 * fpsSpeedFactor;
			link->setPosition(p);
			((Spritesheet*)link->getSprite())->setCurrentAnimation(6);
		}
		}, { make_pair(SDLK_UP, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			Point p = link->getPosition();
			p.y += 7.0 * fpsSpeedFactor;
			link->setPosition(p);
			((Spritesheet*)link->getSprite())->setCurrentAnimation(4);
		}
		}, { make_pair(SDLK_DOWN, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			((Spritesheet*)link->getSprite())->setCurrentAnimation(0);
		}
	}, { make_pair(SDLK_LEFT, SDL_KEYUP),
		 make_pair(SDLK_RIGHT, SDL_KEYUP),
		 make_pair(SDLK_DOWN, SDL_KEYUP),
		 make_pair(SDLK_UP, SDL_KEYUP) }); //If all are up, reset animation*/
	
	gameLoop->start();


	//---------------TEARDOWN-----------------
	window->destroy();
	world.destroy();
	IMG_Quit();
	return 0;
	//Missing PNG libraries: libpng, libz
}