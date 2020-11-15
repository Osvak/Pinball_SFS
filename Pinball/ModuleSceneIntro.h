#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#define MAX_SCORES 50

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
	p2List<PhysBody*> secondCircles;
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

	PhysBody* pointB;
	PhysBody* pointA;
	PhysBody* pointR;
	PhysBody* pointCopa;

	PhysBody* redStar1;
	PhysBody* redStar2;
	PhysBody* redStar3;

	bool createBall;
	bool secondCreateBall;
	int ballsCount = 0;
	bool secondballisalive;
	bool sensed;
	bool up;
	bool touchingBumper1 = false;
	bool touchingBumper2 = false;
	bool touchingBumper3 = false;
	bool touchingStar1 = false;
	bool touchingStar2 = false;
	bool touchingStar3 = false;

	SDL_Texture* circle;
	SDL_Texture* bg;
	SDL_Texture* longTube;
	SDL_Texture* leftSquare;
	SDL_Texture* rightSquare;
	SDL_Texture* bumperTex;
	SDL_Texture* scoreTex;
	SDL_Texture* shootTex;

	SDL_Texture* skyTex;
	SDL_Rect sRect = { 0,0,19,25 };
	bool sCond = false;
	SDL_Rect kRect = { 19,0,20,25 };
	bool kCond = false;
	SDL_Rect yRect = { 39,0,20,25 };
	bool yCond = false;

	SDL_Texture* multiTex;
	SDL_Rect x2 = { 0,0,37,25 };
	SDL_Rect x4 = { 37,0,36,25 };
	SDL_Rect x6 = { 73,0,37,25 };
	SDL_Rect x8 = { 109,0,36,25 };
	SDL_Rect x10 = { 145,0,36,25 };
	int multi = 1;

	SDL_Texture* copaTex;
	bool copaCond = false;
	SDL_Texture* barTex;
	SDL_Rect bRect = { 0,0,31,30 };
	bool bCond = false;
	SDL_Rect aRect = { 31,0,31,30 };
	bool aCond = false;
	SDL_Rect rRect = { 62,0,31,30 };
	bool rCond = false;

	SDL_Texture* starsTex;
	SDL_Rect whiteRect = { 0,0,31,30 };
	SDL_Rect shadowWhiteRect = { 31,0,31,30 };
	SDL_Rect redRect = { 62,0,31,30 };

	SDL_Texture* leftFlipperTex;
	SDL_Texture* rightFlipperTex;

	uint flipperSound;
	uint kickerSound;
	uint deathSound;
	uint bumperSound;
	uint skySound;
	uint multiSound;
	uint song;
	uint barSound;
	uint looseSound;

	p2Point<int> ray;
	bool ray_on;
	bool start;

	int lifes = 5;
	int scoreArray[MAX_SCORES];
	int previousScore = 0000;
	int currentScore = 0000;
	int highestScore = 0000;
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
	char previousScoreText[10] = { "\0" };
	char highestScoreText[10] = { "\0" };
	char lifesText[10] = { "\0" };
};
