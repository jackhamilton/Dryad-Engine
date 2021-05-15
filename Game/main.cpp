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
#include "Vector.h"
#include "AssetLibrary.h"

using namespace std;

static GameLoop* gameLoop;
static Spritesheet* linkSpr;
static GameObject* link;
static World* gameWorld;

static int fps = 60;
static float version = 0.3f;
static Vector v = Vector(Point(50, 50));

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
	world.setDisplayHitboxes(true);
	GameLoop g = GameLoop(fps, &world, &input);
	gameLoop = &g;
	//--------END SETUP----------

	//Metadata
	char engineVersionText[40];
	sprintf(engineVersionText, "Dryad v%.1fa, 2021", version);
	Text engineVersionLabel = Text(engineVersionText, world.getDebugFont(), 18, { 255, 255, 255 }, Point(10, 870));
	world.addDebugObject(&engineVersionLabel);

	AssetLibrary lib = AssetLibrary();
	//Spritesheet
	vector<const char*> images = { lib.getAsset("zelda.png") };
    vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
	vector<bool> looping = { false, false, false, false, true, true, true, true };
    vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	linkSpr = new Spritesheet(images, framecounts, looping, heights, 120, 10);
	linkSpr->setCurrentAnimation(0);
	link = new GameObject(Point(50, 50), linkSpr);
	link->enableHitbox();
	gameScene.addObject(link);

	//Wall
	Sprite* wallSpr = new Sprite({ lib.getAsset("wall.png") });
	GameObject* wall = new GameObject(Point(200, 30), wallSpr);
	wall->enableHitbox();
	gameScene.addObject(wall);

	//Buttons
	Button* buttonTest = new Button(&startGame, "Start", "Bebas.ttf", 24, { 255, 255, 255 }, { 30, 30, 30 }, Point(700, 400), { 200, 100 });
	buttonTest->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 });
	buttonTest->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 });
	menu.addObject(buttonTest);

	Button* buttonTestBack = new Button(&backToMenu, "Menu", "Bebas.ttf", 24, { 255, 255, 255 }, { 30, 30, 30 }, Point(1300, 700), { 200, 100 });
	buttonTestBack->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 });
	buttonTestBack->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 });
	gameScene.addObject(buttonTestBack);

	//Title
	Text titleLabel = Text("Engine Test Sandbox", "OpenSans-Bold.ttf", 48, { 255, 255, 255 }, Point(560, 250));
	menu.addObject(&titleLabel);

	//Configure inputs
	input.addKeyboardEvent([]() { gameLoop->stop(); }, { make_pair(SDLK_ESCAPE, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() { 
		if (gameWorld->getCurrentSceneName() == "Game") {
			link->move(Vector(Point(300, 0)));
			((Spritesheet*)link->getSprite())->setCurrentAnimation(7);
		}
	}, { make_pair(SDLK_RIGHT, SDL_KEYDOWN)});
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			link->move(Vector(Point(-300, 0)));
			((Spritesheet*)link->getSprite())->setCurrentAnimation(5);
		}
	}, { make_pair(SDLK_LEFT, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			link->move(Vector(Point(0, -300)));
			((Spritesheet*)link->getSprite())->setCurrentAnimation(6);
		}
		}, { make_pair(SDLK_UP, SDL_KEYDOWN) });
	input.addKeyboardEvent([]() {
		if (gameWorld->getCurrentSceneName() == "Game") {
			link->move(Vector(Point(0, 300)));
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