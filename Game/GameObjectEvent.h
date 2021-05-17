#pragma once
#include <functional>
#include <time.h>

enum class EventType {
	DESTROY, DELAY, WAIT_FOR_ANIMATION_COMPLETION, NONE
};

class GameObjectEvent
{
public:
	GameObjectEvent() {
		GameObjectEvent::type = EventType::NONE;
		completed = false;
	}
	EventType type;
	bool completed;
};

class DestroyEvent : public GameObjectEvent {
public:
	DestroyEvent() {
		GameObjectEvent::type = EventType::DESTROY;
	}
};

class WaitForAnimationCompletionEvent : public GameObjectEvent {
public:
	WaitForAnimationCompletionEvent() {
		GameObjectEvent::type = EventType::WAIT_FOR_ANIMATION_COMPLETION;
	}
};

class DelayEvent : public GameObjectEvent {
public:
	DelayEvent(time_t msTime) {
		GameObjectEvent::type = EventType::DELAY;
		time = 0;
		popTime = msTime;
		started = false;
	}
private:
	bool started;
	clock_t time;
	time_t popTime;
	friend class GameObject;
	friend class World;
};