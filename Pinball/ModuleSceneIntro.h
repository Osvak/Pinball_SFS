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
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	PhysBody* dead;
	PhysBody* layer1;
	PhysBody* startPoint;
	PhysBody* lt;
	PhysBody* wall;
	PhysBody* leftBumper;
	PhysBody* rightBumper;

	PhysBody* bumper1;
	PhysBody* bumper2;
	PhysBody* bumper3;

	PhysBody* pointS;
	PhysBody* pointK;
	PhysBody* pointY;

	bool createBall;
	bool sensed;
	bool up;
	bool touching1 = false;
	bool touching2 = false;
	bool touching3 = false;

	SDL_Texture* circle;
	SDL_Texture* bg;
	SDL_Texture* longTube;
	SDL_Texture* leftSquare;
	SDL_Texture* rightSquare;
	SDL_Texture* bumperTex;
	SDL_Texture* scoreTex;

	SDL_Texture* skyTex;
	SDL_Rect sRect = { 0,0,19,25 };
	bool sCond = false;
	SDL_Rect kRect = { 19,0,20,25 };
	bool kCond = false;
	SDL_Rect yRect = { 39,0,20,25 };
	bool yCond = false;
	int skyCount = 0;

	SDL_Texture* multiTex;
	SDL_Rect x2 = { 0,0,37,25 };
	SDL_Rect x4 = { 37,0,36,25 };
	SDL_Rect x6 = { 73,0,37,25 };
	SDL_Rect x8 = { 109,0,36,25 };
	SDL_Rect x10 = { 145,0,36,25 };
	int multi = 1;

	SDL_Texture* leftFlipperTex;
	SDL_Texture* rightFlipperTex;

	uint flipperSound;
	uint kickerSound;
	uint deathSound;
	uint bumperSound;
	uint skySound;
	uint multiSound;

	p2Point<int> ray;
	bool ray_on;
	bool start;

	int score = 0000;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};
