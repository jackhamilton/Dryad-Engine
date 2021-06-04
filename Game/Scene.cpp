#pragma once
#include "Scene.h"
#include "Point.h"
#include "Spritesheet.h"
#include "TextField.h"
#include "Button.h"
#include "Hitbox.h"

Scene::Scene()
{
	using namespace placeholders;
}

std::vector<vector<shared_ptr<GameObject>>> Scene::getObjects()
{
	vector<vector<shared_ptr<GameObject>>> objs;
	if (objects.size() > 0) {
		auto it = objects.begin();
		for (; it != objects.end(); it++) {
			objs.push_back(it->second);
		}
	}
	return objs;
}

std::vector<shared_ptr<GameObject>> Scene::getObjectsFlat()
{
	vector<shared_ptr<GameObject>> objs;
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		for (shared_ptr<GameObject> o : it->second) {
			objs.push_back(o);
		}
	}
	return objs;
}


int Scene::getObjectsCount()
{
	int total = 0;
	auto it = objects.begin();
	for (; it != objects.end(); it++) {
		total += it->second.size();
	}
	return total;
}


void Scene::addObject(shared_ptr<GameObject> object) {
	addObject(object, "default");
}

void Scene::addObject(shared_ptr<GameObject> object, string layer) {
	if (input && input->lockSceneObjects) {
		postInputLoopObjectAddQueue.push_back(make_pair(object, layer));
	}
	else {
		if (!renderer) {
			printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
			return;
		}
		//Load the sprite's images with the scene renderer
		if (object->hasSprite) {
			for (weak_ptr<Sprite> s : object->renderQueue) {
				if (!s.expired()) {
					s.lock()->loadTextures(renderer);
				}
			}
			Scene::objects[layer].push_back(object);
		}

		function<void()> objectMouseEvents[9];
		object->getMouseEvents(objectMouseEvents);

		if (object->isTextField) {
			using namespace placeholders;
			static_pointer_cast<TextField>(object)->deactivateOtherCallback = bind(&Scene::deactivateUniqueElements, this, _1);
			static_pointer_cast<TextField>(object)->activateFieldCallback = bind(&Scene::activateTextField, this, _1);
		}

		using namespace placeholders;
		object->removeCalls.push_back(bind(&Scene::removeObject, this, _1));
	}
}

void Scene::addSprite(shared_ptr<Sprite> sprite) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	sprite->loadTextures(renderer);
	sprites.push_back(sprite);
}

void Scene::removeObject(GameObject* o)
{
	if (input && input->lockSceneObjects) {
		postInputLoopObjectRemovalQueue.push_back(o);
	}
	else {
		auto it = objects.begin();
		for (; it != objects.end(); it++) {
			for (int i = 0; i < it->second.size(); i++) {
				if (o->id == it->second.at(i)->id) {
					it->second.erase(it->second.begin() + i);
					i -= 1;
				}
			}
		}
	}
}

void Scene::handleObjectModificationQueue() {
	for (GameObject* o : postInputLoopObjectRemovalQueue) {
		removeObject(o);
	}
	for (pair<shared_ptr<GameObject>, string> o : postInputLoopObjectAddQueue) {
		addObject(o.first, o.second);
	}
	postInputLoopObjectRemovalQueue.clear();
	postInputLoopObjectAddQueue.clear();
}

void Scene::deactivateUniqueElements(GameObject* sender)
{
	if (sender->isTextField) {
		for (shared_ptr<GameObject> g : getObjectsFlat()) {
			if (g->isTextField) {
				auto tf = static_pointer_cast<TextField>(g);
				tf->deactivate();
			}
		}
	}
}

vector<Polygon> Scene::generateSceneLightingMasks(Light l, Rectangle renderZone)
{
	double precision = 1; //720*4n raytraces per tick
	//TODO does not do lighting for object children - add??
	vector<Polygon> lightingMasks;
	vector<Polygon> sceneLightingMasks;
	vector<shared_ptr<GameObject>> objToMask = getObjectsFlat();
	for (shared_ptr<GameObject> obj : objToMask) {
		if (!obj->isUIElement && obj->blocksLighting) {
			Polygon objLightingMask = obj->getLightingMask();
			bool add = false;
			for (Point p : objLightingMask.shape) {
				if (renderZone.isInside(p)) {
					add = true;
				}
			}
			if (add) {
				lightingMasks.push_back(objLightingMask);
			}
		}
	}
	if (lightingMasks.size() != 0) {
		//Raytracing setup
		Point origin = l.p;
		PolarVector vec = PolarVector();
		vec.r = 10000;
		vec.theta = 0.0;
		double vecCTheta = 0.0;
		//nearest object data
		//given a point, find the line it's attached to
		vector<LineData> collidableLines;
		for (Polygon p : lightingMasks) {
			vector<LineData> tLines = p.getSidesAsLineData();
			for (LineData tLine : tLines) {
				collidableLines.push_back(tLine);
			}
		}
		//view bounding rect
		collidableLines.push_back({ (double)renderZone.x, (double)renderZone.y, (double)renderZone.x + (double)renderZone.width, (double)renderZone.y });
		collidableLines.push_back({ (double)renderZone.x + (double)renderZone.width, (double)renderZone.y, (double)renderZone.x + (double)renderZone.width, (double)renderZone.y + (double)renderZone.height });
		collidableLines.push_back({ (double)renderZone.x + (double)renderZone.width, (double)renderZone.y + (double)renderZone.height, (double)renderZone.x, (double)renderZone.y + (double)renderZone.height });
		collidableLines.push_back({ (double)renderZone.x, (double)renderZone.y + (double)renderZone.height, (double)renderZone.x, (double)renderZone.y });

		Point prevFinePt;
		Point prevTestedPt;
		Point firstTriangleVertex;
		bool testingBegun = false, firstJump = true;
		do {
			shared_ptr<Point> result = make_shared<Point>();
			bool collided = raytrace(result, &origin, &vec, &collidableLines);
			if (collided) {
				if (testingBegun) {
					if (!onSameLine(&prevTestedPt, result, &collidableLines)) {
						bool becameNearer = false;
						if (result->distance(origin) <= prevTestedPt.distance(origin)) {
							becameNearer = true;
						}
						Point closePoint, farPoint;
						LineData farLine;
						if (becameNearer) {
							//farther can be anything, nearer will always be a point
							closePoint = nearestPoint(*result, &collidableLines);
							farLine = collidableLines.at(nearestLine(prevTestedPt, &collidableLines));
						}
						else {
							closePoint = nearestPoint(prevTestedPt, &collidableLines);
							farLine = collidableLines.at(nearestLine(*result, &collidableLines));
						}
						if ((farLine.x1 == closePoint.x && farLine.y1 == closePoint.y)
							|| (farLine.x2 == closePoint.x && farLine.y2 == closePoint.y)) {
							farPoint = closePoint;
						}
						else {
							Vector lineExtensionDown = Vector(farLine.x2, farLine.y2, farLine.x1, farLine.y1);
							lineExtensionDown += 5000;
							Vector lineExtensionUp = Vector(farLine.x1, farLine.y1, farLine.x2, farLine.y2);
							lineExtensionUp += 5000;
							farLine.x1 += lineExtensionDown.x;
							farLine.y1 += lineExtensionDown.y;
							farLine.x2 += lineExtensionUp.x;
							farLine.y2 += lineExtensionUp.y;

							Vector projection = Vector(origin.x, origin.y, closePoint.x, closePoint.y);
							projection += 10000;
							shared_ptr<Point> coll = make_shared<Point>();
							if (Hitbox::lineLine(origin.x, origin.y, origin.x + projection.x, origin.y + projection.y,
								farLine.x1, farLine.y1, farLine.x2, farLine.y2, coll)) {
								farPoint = *coll;
							}
							else {
								printf("Error raytracing line jump's farther point\n");
							}
						}
						if (!firstJump) {
							//form triangle: origin, prevFinePt, prevTestedPt
							Polygon poly;
							poly.shape.push_back(origin);
							poly.shape.push_back(prevFinePt);
							if (becameNearer) {
								poly.shape.push_back(farPoint);
								prevFinePt = closePoint;
							}
							else {
								poly.shape.push_back(closePoint);
								prevFinePt = farPoint;
							}
							sceneLightingMasks.push_back(poly);
						}
						else {
							if (becameNearer) {
								firstTriangleVertex = farPoint;
								prevFinePt = closePoint;
							}
							else {
								firstTriangleVertex = closePoint;
								prevFinePt = farPoint;
							}
							firstJump = false;
						}
					}
				}
				prevTestedPt = *result;
				if (!testingBegun) {
					testingBegun = true;
				}
			}
			vecCTheta += precision;
			vec.setTheta(vecCTheta);
		} while (vecCTheta <= 360);
		if (!firstJump) {
			Polygon poly;
			poly.shape.push_back(origin);
			poly.shape.push_back(prevFinePt);
			poly.shape.push_back(firstTriangleVertex);
			sceneLightingMasks.push_back(poly);
		}
	}
	return sceneLightingMasks;
}

bool Scene::raytrace(shared_ptr<Point> result, Point* origin, PolarVector* projection, vector<LineData>* testArray) {
	Point p = *origin + projection->getCartesian();
	int minDist = 10000;
	for (LineData l : *testArray) {
		shared_ptr<Point> fullCollResults = make_shared<Point>();
		if (Hitbox::lineLine(origin->x, origin->y, p.x, p.y, l.x1, l.y1, l.x2, l.y2, fullCollResults)) {
			int dist = origin->distance(*fullCollResults);
			if (dist < minDist) {
				minDist = dist;
				*result = *fullCollResults;
			}
		}
	}
	if (minDist == 10000) {
		return false;
	}
	return true;
}

Point Scene::nearestPoint(Point p, vector<LineData>* testArray) {
	Point bestMatch;
	double dist = 100000.0;
	for (const LineData& l : *testArray) {
		Point p1 = Point(l.x1, l.y1);
		Point p2 = Point(l.x2, l.y2);
		double tDist = p1.distance(p);
		if (tDist < dist) {
			bestMatch = p1;
			dist = tDist;
		}
		tDist = p2.distance(p);
		if (tDist < dist) {
			bestMatch = p2;
			dist = tDist;
		}
	}
	return bestMatch;
}

bool Scene::onSameLine(Point* p1, shared_ptr<Point> p2, vector<LineData>* testArray) {
	//TODO
	//find closest line to each point
	int lineIndex1 = 5000, lineIndex2 = 5000;
	lineIndex1 = nearestLine(*p1, testArray);
	lineIndex2 = nearestLine(*p2, testArray);
	if (lineIndex1 != 5000 && lineIndex2 != 5000) {
		return lineIndex1 == lineIndex2;
	}
	else {
		printf("Error computing line distances");
		return false;
	}
}

int Scene::nearestLine(Point p1, vector<LineData>* testArray) {
	double minDist = 10000;
	int lineIndex = 5000;
	for (int x = 0; x < testArray->size(); x++) {
		double distance = testArray->at(x).distToPoint(p1);
		if (distance < minDist) {
			lineIndex = x;
			minDist = distance;
		}
	}
	if (lineIndex == 5000) {
		printf("Error computing line distances");
		return 0;
	}
	return lineIndex;
}

void Scene::activateTextField(TextField* object) {
	input->activeField = object;
}
