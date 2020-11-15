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
	PhysBody* startPoint;
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

	PhysBody* whiteStar1;
	PhysBody* whiteStar2;
	PhysBody* whiteStar3;
	PhysBody* whiteStar4;

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

	bool touchingWhiteStar1 = false;
	bool touchingWhiteStar2 = false;
	bool touchingWhiteStar3 = false;
	bool touchingWhiteStar4 = false;

	// Start Tunnel control
	bool startTunnel = false;
	PhysBody* startWall;
	PhysBody* skyAreaCollider; // Outside of wall
	PhysBody* startTunnelCollider; // Inside of wall

	// Sky Wall control
	bool skyWallFlag = true;
	PhysBody* skyWall;
	PhysBody* lobbyAreaCollider;
	PhysBody* lobbyAreaCollider2;

	// Lobby Wall control
	bool lobbyWallFlag = true;
	PhysBody* lobbyWall;
	PhysBody* hotelAreaCollider;

	// Right ramp control
	bool rightRampFlag = false;
	PhysBody* rightRampOutter;
	PhysBody* rightRampInner;
	PhysBody* rightRampWall;
	PhysBody* rightRampInCollider;
	PhysBody* rightRampOutCollider;
	PhysBody* rightRampOutCollider2;

	//Left ramp control
	bool leftRampFlag = false;
	PhysBody* leftRampOutter;
	PhysBody* leftRampInner;
	PhysBody* leftRampWall;
	PhysBody* leftRampInCollider;
	PhysBody* leftRampOutCollider;
	PhysBody* leftRampOutCollider2;

	//Middle ramp control
	bool middleRampFlag = false;
	PhysBody* middleRampRight;
	PhysBody* middleRampLeft;
	PhysBody* middleRampInCollider;
	PhysBody* middleRampOutCollider;
	PhysBody* middleRampOutCollider2;
	PhysBody* wall3;

	// Textures
	SDL_Texture* circle;
	SDL_Texture* bg;
	SDL_Texture* longTube;
	SDL_Texture* leftSquare;
	SDL_Texture* rightSquare;
	SDL_Texture* bumperTex;
	SDL_Texture* scoreTex;
	SDL_Texture* shootTex;

	// SKY Textures and flags
	SDL_Texture* skyTex;
	SDL_Rect sRect = { 0,0,19,25 };
	bool sCond = false;
	SDL_Rect kRect = { 19,0,20,25 };
	bool kCond = false;
	SDL_Rect yRect = { 39,0,20,25 };
	bool yCond = false;

	// Multipliers Textures and flags
	SDL_Texture* multiTex;
	SDL_Rect x2 = { 0,0,37,25 };
	SDL_Rect x4 = { 37,0,36,25 };
	SDL_Rect x6 = { 73,0,37,25 };
	SDL_Rect x8 = { 109,0,36,25 };
	SDL_Rect x10 = { 145,0,36,25 };
	int multi = 1;

	// BAR Textures and flags
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
	uint kickSound;

	p2Point<int> ray;
	bool ray_on;
	bool start;

	// Lifes and scores variables
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
