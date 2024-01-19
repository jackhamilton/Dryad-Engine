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
#include "UIBlock.h"
#include "TextField.h"
#include "AssetLibrary.h"
#include "GameObjectEvent.h"

static std::shared_ptr<dryad::GameLoop> gameLoop;
static std::shared_ptr<dryad::Spritesheet> linkSpr;
static std::shared_ptr<dryad::Mouse> mouse;
static std::shared_ptr<dryad::GameObject> link;
static std::shared_ptr<dryad::World> gameWorld;
static std::shared_ptr<dryad::Input> input;
static std::shared_ptr<dryad::Window> window;
static std::shared_ptr<dryad::Camera> gameSceneCamera;

static int fps = 60;
static float version = 0.4f;
static dryad::Vector v = Vector(Point(50, 50));

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

int main(int argc, char* argv[]) {
	//----------SETUP------------
	Resolution r = Resolution::ml16_9;
    window = std::shared_ptr<Window>(new Window("Dryad", r));
	pair<int, int> windowDim = Window::calculateResolution(r);
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
	std::shared_ptr<AssetLibrary> libPtr = make_shared<AssetLibrary>(lib);
	gameWorld->setDisplayFPS(true, libPtr);
	gameWorld->setDisplayObjectCount(true, libPtr);
	gameScene->globalIllumination = 0.2;
	//--------END SETUP----------

	//Metadata
	char engineVersionText[40];
	sprintf(engineVersionText, "Dryad v%.1fa, 2021", version);
	Text engineVersionLabel = Text(engineVersionText, gameWorld->getDebugFont(), 18, { 255, 255, 255 }, Point(10, 870), libPtr);
	gameWorld->addDebugObject(make_shared<Text>(engineVersionLabel));

	//bg
	std::shared_ptr<Sprite> bgSpr = make_shared<Sprite>(Sprite({ lib.getAsset("floor.png") }));
	std::shared_ptr<GameObject> bg = make_shared<GameObject>(GameObject(Point(0, 0), bgSpr));
	bg->blocksLighting = false;
	gameScene->addObject(bg);

	//Spritesheet
	vector<string> images = { lib.getAsset("zelda.png") };
    vector<int> heights = { 130, 130, 130, 130, 130, 130, 130, 130 };
	vector<bool> looping = { false, false, false, false, true, true, true, true };
    vector<int> framecounts = { 3, 3, 1, 3, 10, 10, 10, 10 };
	linkSpr = make_shared<Spritesheet>(Spritesheet(images, framecounts, looping, heights, 120, 10));
	linkSpr->setCurrentAnimation(0);
	link = make_shared<GameObject>(GameObject(Point(100, 100), linkSpr));
	link->enableHitbox();
	link->blocksLighting = false;
	gameScene->addObject(link);

	//Wall
	std::shared_ptr<Sprite> wallSpr = make_shared<Sprite>(Sprite({ lib.getAsset("wall.png") }));
	std::shared_ptr<GameObject> wall = make_shared<GameObject>(GameObject(Point(400, 200), wallSpr));
	//wall->queueEvents({ static_pointer_cast<GameObjectEvent>(make_shared<DelayEvent>(DelayEvent(10000))), 
	//	make_shared<GameObjectEvent>(DestroyEvent()) });
	wall->enableHitbox();
	gameScene->addObject(wall);
	std::shared_ptr<GameObject> wall2 = make_shared<GameObject>(GameObject(Point(500, 600), wallSpr));
	wall2->enableHitbox();
	gameScene->addObject(wall2);
	std::shared_ptr<GameObject> wall3 = make_shared<GameObject>(GameObject(Point(1200, 500), wallSpr));
	wall3->enableHitbox();
	gameScene->addObject(wall3);

	//Buttons
	std::shared_ptr<Button> buttonTest = make_shared<Button>(Button(&startGame, "Start", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(700, 400), { 200, 100 }));
	buttonTest->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonTest->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	menu->addObject(buttonTest);

	std::shared_ptr<Button> buttonTestBack = make_shared<Button>(Button(&backToMenu, "Menu", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(1300, 700), { 200, 100 }));
	buttonTestBack->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonTestBack->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	gameScene->addObject(buttonTestBack);

	//Camera
	gameSceneCamera = std::shared_ptr<Camera>(new Camera(window->getResolution()));
	//gameSceneCamera->setTrackObject(link);
	gameScene->setCamera(gameSceneCamera);

	std::shared_ptr<Button> buttonToggleFollow = make_shared<Button>(Button(&toggleCameraTracking, "Toggle Camera Tracking", "Bebas", 24, { 255, 255, 255 }, libPtr, { 30, 30, 30 }, Point(1300, 580), { 200, 100 }));
	buttonToggleFollow->createHoverTexture({ 255, 255, 255 }, { 80, 80, 80 }, libPtr);
	buttonToggleFollow->createClickTexture({ 0, 0, 0 }, { 150, 150, 150 }, libPtr);
	gameScene->addObject(buttonToggleFollow);

	//Title
	std::shared_ptr<Text> titleLabel = make_shared<Text>(Text("Engine Test Sandbox", "OpenSans-Bold", 48, { 255, 255, 255 }, Point(560, 250), libPtr));
	menu->addObject(titleLabel);

	auto l = make_shared<Line>(Line({ 255, 255, 255 }, Point(700, 350), Point(900, 350)));
	menu->addObject(l);

	//TextField testing
	std::shared_ptr<TextField> testTextField = std::shared_ptr<TextField>(new TextField("Engine Test Sandbox", "OpenSans-Regular", 14, { 150, 150, 100, 30 }, libPtr));
	menu->addObject(testTextField);
	/*
	int addX = windowDim.first / 3, addY = windowDim.second / 3;
	Rectangle addRect = { addX, addY, addX, addY };
	std::shared_ptr<UIBlock> addPopup = std::shared_ptr<UIBlock>(new UIBlock(addRect, { 100, 100, 100 }));
	menu->addObject(addPopup);*/

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