#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <memory>
#include <string>
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
#include "GameObjectEvent.h"

using namespace std;

static shared_ptr<GameLoop> gameLoop;
static shared_ptr<Spritesheet> linkSpr;
static shared_ptr<Mouse> mouse;
static shared_ptr<GameObject> link;
static shared_ptr<World> gameWorld;
static shared_ptr<Input> input;
static shared_ptr<Window> window;
static shared_ptr<Camera> gameSceneCamera;

static int fps = 60;
static float version = 0.4f;
static Vector v = Vector(Point(50, 50));

void startGame() {
	gameWorld->setScene("Game");
}

void backToMenu() {
	gameWorld->setScene("Menu");
}

void toggleCameraTracking() {
	if (gameSceneCamera->getIsTracking()) {
		gameSceneCamera->stopTracking();
	}
	else {
		gameSceneCamera->setTrackObject(link);
	}
}

int main(int argc, char* args[]) {
	//----------SETUP------------
    window = shared_ptr<Window>(new Window("Dryad", Resolution::ml16_9));
	mouse = make_shared<Mouse>();
	input = make_shared<Input>(mouse);
    gameWorld = make_shared<World>(World(window->getRenderer(), input));
	auto gameScene = make_shared<Scene>(Scene());
	gameWorld->addScene(gameScene, "Game");
	auto menu = make_shared<Scene>(Scene());
	gameWorld->addScene(menu, "Menu");
	gameWorld->setScene("Menu");
	//TODO: abstract
	gameWorld->setDisplayHitboxes(true);
	GameLoop g = GameLoop(fps, gameWorld, input);
	gameLoop = make_shared<GameLoop>(g);

	AssetLibrary lib = AssetLibrary();
	shared_ptr<AssetLibrary> libPtr = make_shared<AssetLibrary>(lib);
	gameWorld->setDisplayFPS(true, libPtr);
	gameWorld->setDisplayObjectCount(true, libPtr);
	//--------END SETUP----------

	//Metadata
	char engineVersionText[40];
	sprintf(engineVersionText, "Dryad v%.1fa, 2021", version);
	Text engineVersionLabel = Text(engineVersionText, gameWorld->getDebugFont(), 18, { 255, 255, 255 }, Point(10, 870), libPtr);
	gameWorld->addDebugObject(make_shared<Text>(engineVersionLabel));

	//Spritesheet
	vector<string> images = { lib.getAsset("zelda.png") };
    vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
	vector<bool> looping = { false, false, false, false, true, true, true, true };
    vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	linkSpr = make_shared<Spritesheet>(Spritesheet(images, framecounts, looping, heights, 120, 10));
	linkSpr->setCurrentAnimation(0);
	link = make_shared<GameObject>(GameObject(Point(50, 50), linkSpr));
	link->enableHitbox();
	gameScene->addObject(link);

	//Wall
	shared_ptr<Sprite> wallSpr = make_shared<Sprite>(Sprite({ lib.getAsset("wall.png") }));
	shared_ptr<GameObject> wall = make_shared<GameObject>(GameObject(Point(200, 30), wallSpr));
	wall->queueEvents({ static_pointer_cast<GameObjectEvent>(make_shared<DelayEvent>(DelayEvent(4000))), 
		make_shared<GameObjectEvent>(DestroyEvent()) });
	wall->enableHitbox();
	gameScene->addObject(wall);

	//Buttons
	shared_ptr<Button> buttonTest = make_shared<Button>(Button(&startGame, "Start", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(700, 400), { 200, 100 }));
	buttonTest->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonTest->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	menu->addObject(buttonTest);

	shared_ptr<Button> buttonTestBack = make_shared<Button>(Button(&backToMenu, "Menu", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(1300, 700), { 200, 100 }));
	buttonTestBack->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonTestBack->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	gameScene->addObject(buttonTestBack);

	//Camera
	gameSceneCamera = shared_ptr<Camera>(new Camera(window->getResolution()));
	//gameSceneCamera->setTrackObject(link);
	gameScene->setCamera(gameSceneCamera);

	shared_ptr<Button> buttonToggleFollow = make_shared<Button>(Button(&toggleCameraTracking, "Toggle Camera Tracking", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(1300, 580), { 200, 100 }));
	buttonToggleFollow->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonToggleFollow->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	gameScene->addObject(buttonToggleFollow);

	//Title
	shared_ptr<Text> titleLabel = make_shared<Text>(Text("Engine Test Sandbox", "OpenSans-Bold", 48, { 255, 255, 255 }, Point(560, 250), libPtr));
	menu->addObject(titleLabel);

	auto l = make_shared<Line>(Line({ 255, 255, 255 }, Point(700, 350), Point(900, 350)));
	menu->addObject(l);

	//Configure inputs
	input->addKeyboardEvent([]() { gameLoop->stop(); }, { make_pair(SDLK_ESCAPE, SDL_KEYDOWN) });
	input->addKeyboardEvent([]() {
		link->move(Vector(Point(100, 0)));
		static_pointer_cast<Spritesheet>(link->getSprite().lock())->setCurrentAnimation(7);
	}, { make_pair(SDLK_RIGHT, SDL_KEYDOWN)});
	input->addKeyboardEvent([]() {
		link->move(Vector(Point(-100, 0)));
		static_pointer_cast<Spritesheet>(link->getSprite().lock())->setCurrentAnimation(5);
	}, { make_pair(SDLK_LEFT, SDL_KEYDOWN) });
	input->addKeyboardEvent([]() {
		link->move(Vector(Point(0, -100)));
		static_pointer_cast<Spritesheet>(link->getSprite().lock())->setCurrentAnimation(6);
	}, { make_pair(SDLK_UP, SDL_KEYDOWN) });
	input->addKeyboardEvent([]() {
		link->move(Vector(Point(0, 100)));
		static_pointer_cast<Spritesheet>(link->getSprite().lock())->setCurrentAnimation(4);
	}, { make_pair(SDLK_DOWN, SDL_KEYDOWN) });
	input->addKeyboardEvent([]() {
		static_pointer_cast<Spritesheet>(link->getSprite().lock())->setCurrentAnimation(0);
	}, { make_pair(SDLK_LEFT, SDL_KEYUP),
		 make_pair(SDLK_RIGHT, SDL_KEYUP),
		 make_pair(SDLK_DOWN, SDL_KEYUP),
		 make_pair(SDLK_UP, SDL_KEYUP) }); //If all are up, reset animation
	gameLoop->start();


	//---------------TEARDOWN-----------------
	return 0;
	//Missing PNG libraries: libpng, libz
}