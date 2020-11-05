#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> leftflippers;
	p2List<PhysBody*> rightflippers;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* bg;

	SDL_Texture* leftFlipper;
	SDL_Texture* rightFlipper;
	SDL_Rect down = { 0,0,73,39 };
	SDL_Rect up = { 73,0,73,39 };

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
