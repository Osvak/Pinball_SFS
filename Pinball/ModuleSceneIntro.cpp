#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFont.h"
#include <stdio.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = bg = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Sounds Load
	flipperSound = App->audio->LoadFx("pinball/flipper.wav");
	kickerSound = App->audio->LoadFx("pinball/kicker.wav");
	deathSound = App->audio->LoadFx("pinball/death.wav");
	bumperSound = App->audio->LoadFx("pinball/bumper.wav");
	skySound = App->audio->LoadFx("pinball/sky.wav");
	multiSound = App->audio->LoadFx("pinball/multi.wav");
	song = App->audio->LoadFx("pinball/song.wav");
	barSound = App->audio->LoadFx("pinball/barSound.wav");
	looseSound = App->audio->LoadFx("pinball/loose.wav");
	kickSound = App->audio->LoadFx("pinball/kick.wav");

	// Textures Load
	circle = App->textures->Load("pinball/ball.png"); 
	bg = App->textures->Load("pinball/background.png");
	leftFlipperTex = App->textures->Load("pinball/left_flipper.png");
	rightFlipperTex = App->textures->Load("pinball/right_flipper.png");
	longTube = App->textures->Load("pinball/tube.png");
	leftSquare = App->textures->Load("pinball/side.png");
	rightSquare = App->textures->Load("pinball/side2.png");
	bumperTex = App->textures->Load("pinball/bumper.png");
	skyTex = App->textures->Load("pinball/sky.png");
	multiTex = App->textures->Load("pinball/multipliers.png");
	barTex = App->textures->Load("pinball/bar.png");
	copaTex = App->textures->Load("pinball/copa.png");
	shootTex = App->textures->Load("pinball/shoot.png");
	starsTex = App->textures->Load("pinball/stars.png");
	lobbyTex = App->textures->Load("pinball/lobby.png");

	// Colliders' shapes creation
	{
		int walls[158] = {
			437, 832,
			468, 832,
			468, 131,
			463, 104,
			451, 74,
			427, 44,
			398, 25,
			360, 14,
			311, 12,
			147, 12,
			116, 21,
			89, 36,
			61, 62,
			39, 98,
			25, 138,
			20, 172,
			20, 254,
			27, 287,
			44, 362,
			69, 427,
			56, 439,
			51, 455,
			48, 477,
			63, 515,
			35, 545,
			27, 560,
			27, 589,
			33, 600,
			55, 620,
			55, 678,
			13, 686,
			13, 788,
			15, 800,
			29, 815,
			54, 840,
			54, 879,
			0, 879,
			0, 0,
			481, 0,
			481, 879,
			401, 879,
			402, 863,
			411, 845,
			423, 827,
			423, 694,
			417, 686,
			381, 686,
			382, 658,
			385, 645,
			420, 610,
			424, 600,
			423, 572,
			378, 525,
			399, 449,
			399, 405,
			392, 396,
			382, 389,
			355, 389,
			344, 393,
			334, 404,
			329, 412,
			329, 490,
			324, 490,
			323, 380,
			332, 375,
			369, 366,
			379, 361,
			382, 352,
			382, 340,
			372, 330,
			373, 325,
			394, 318,
			407, 310,
			417, 294,
			422, 279,
			423, 271,
			423, 133,
			429, 129,
			437, 134
		}; App->physics->CreateChain(0, 0, walls, 158, b2_staticBody);
		// Pivot 0, 0
		int wall1[22] = {
			258, 444,
			228, 429,
			220, 421,
			213, 415,
			210, 406,
			207, 375,
			210, 362,
			215, 372,
			243, 378,
			244, 385,
			255, 410
		}; App->physics->CreateChain(0, 0, wall1, 22, b2_staticBody);
		int wall2[70] = {
			172, 405,
			85, 338,
			70, 307,
			57, 260,
			53, 233,
			54, 191,
			57, 156,
			67, 127,
			80, 100,
			101, 76,
			123, 58,
			140, 47,
			158, 43,
			310, 43,
			310, 56,
			213, 56,
			197, 58,
			184, 62,
			168, 72,
			155, 84,
			151, 97,
			150, 103,
			143, 104,
			118, 110,
			100, 123,
			86, 139,
			78, 162,
			70, 182,
			70, 258,
			76, 274,
			88, 286,
			107, 295,
			129, 305,
			153, 313,
			167, 318,
		}; App->physics->CreateChain(0, 0, wall2, 70, b2_staticBody);
		// Pivot 0, 0
		int wall3Path[34] = {
			115, 172,
			106, 172,
			107, 165,
			117, 154,
			134, 141,
			168, 140,
			186, 132,
			235, 132,
			244, 138,
			258, 150,
			258, 165,
			251, 167,
			185, 167,
			161, 162,
			144, 160,
			132, 160,
			122, 166
		}; wall3 = App->physics->CreateChain(0, 0, wall3Path, 34, b2_staticBody);

		int lobbyTopWallPath[26] = {
			305, 154,
			303, 140,
			298, 130,
			291, 121,
			285, 115,
			278, 110,
			267, 106,
			256, 102,
			235, 102,
			196, 102,
			216, 93,
			310, 93,
			310, 154,
		}; App->physics->CreateChain(0, 0, lobbyTopWallPath, 26, b2_staticBody);

		int leftGround[28] = {
			84, 879,
			84, 840,
			83, 834,
			77, 826,
			47, 794,
			43, 782,
			43, 727,
			50, 722,
			55, 727,
			56, 772,
			143, 823,
			140, 843,
			167, 866,
			167, 879
		}; App->physics->CreateChain(0, 0, leftGround, 28, b2_staticBody);
		int rightGround[24] = {
			370, 879,
			370, 853,
			394, 815,
			394, 724,
			388, 720,
			382, 725,
			382, 792,
			329, 818,
			333, 831,
			333, 845,
			303, 867,
			303, 879
		}; App->physics->CreateChain(0, 0, rightGround, 24, b2_staticBody);

		int leftBumperPoints[12] = {
			121, 768,
			122, 726,
			127, 722,
			153, 770,
			153, 783,
			148, 783
		}; leftBumper = App->physics->CreateChain(0, 0, leftBumperPoints, 12, b2_staticBody, 0.0f, 1.0f);
		int rightBumperPoints[12] = {
			320, 782,
			328, 782,
			352, 767,
			352, 727,
			347, 723,
			320, 772
		}; rightBumper = App->physics->CreateChain(0, 0, rightBumperPoints, 12, b2_staticBody, 0.0f, 1.0f);

		int leftTube[26] = {
			74, 546,
			80, 561,
			96, 603,
			107, 634,
			107, 655,
			96, 669,
			85, 671,
			84, 620,
			78, 608,
			56, 583,
			55, 567,
			58, 560,
			73, 543
		}; App->physics->CreateChain(0, 0, leftTube, 26, b2_staticBody);
		int rightTube[20] = {
			352, 685,
			353, 656,
			358, 641,
			365, 629,
			391, 602,
			394, 595,
			394, 584,
			371, 557,
			333, 681,
			341, 685
		}; App->physics->CreateChain(0, 0, rightTube, 20, b2_staticBody);

		int upWall1[12] = {
			344, 152,
			351, 149,
			351, 133,
			346, 129,
			340, 133,
			340, 149
		}; App->physics->CreateChain(0, 0, upWall1, 12, b2_staticBody);
		int upWall2[12] = {
			386, 152,
			393, 148,
			393, 132,
			387, 129,
			381, 134,
			381, 149
		}; App->physics->CreateChain(0, 0, upWall2, 12, b2_staticBody);
		int downWall[12] = {
			88, 754,
			93, 750,
			93, 726,
			88, 722,
			84, 726,
			84, 751
		}; App->physics->CreateChain(0, 0, downWall, 12, b2_staticBody);

		// Walls in between floors
		int entranceWall[20] = {
			445, 68,
			445, 89,
			442, 104,
			437, 115,
			431, 124,
			423, 132,
			431, 124,
			437, 115,
			442, 104,
			445, 89,
		}; startWall = App->physics->CreateChain(0, 0, entranceWall, 20, b2_staticBody);
		int skyWallPath[12] = {
			305, 154,
			305, 163,
			391, 206,
			423, 208,
			391, 206,
			305, 163,
		}; skyWall = App->physics->CreateChain(0, 0, skyWallPath, 12, b2_staticBody);
		int lobbyWallPath[16] = {
			258, 443,
			285, 451,
			307, 462,
			318, 474,
			324, 490,
			318, 474,
			307, 462,
			285, 451,
		}; lobbyWall = App->physics->CreateChain(0, 0, lobbyWallPath, 16, b2_staticBody);

		// Right Ramp
		int rightRampPathOut[12] = {
			324, 490,
			325, 520,
			366, 560,
			368, 554,
			330, 517,
			329, 490,
		}; rightRampOutter = App->physics->CreateChain(0, 0, rightRampPathOut, 12, b2_staticBody);
		int rightRampPathIn[12] = {
			358, 428,
			358, 502,
			381, 530,
			383, 523,
			364, 500,
			364, 428,
		}; rightRampInner = App->physics->CreateChain(0, 0, rightRampPathIn, 12, b2_staticBody);
		int rightRampWallPath[8] = {
			378, 525,
			382, 528,
			374, 560,
			371, 557,			
		}; rightRampWall = App->physics->CreateChain(0, 0, rightRampWallPath, 8, b2_staticBody);

		// Left Ramp
		int leftRampPathOut[36] = {
			69, 427,
			81, 422,
			93, 421,
			113, 425,
			126, 434,
			136, 451,
			135, 476,
			126, 493,
			73, 543,
			74, 546, /**/
			129, 496,
			142, 481,
			142, 452,
			134, 436,
			120, 422,
			100, 415,
			71, 418,
			62, 425
		}; leftRampOutter = App->physics->CreateChain(0, 0, leftRampPathOut, 36, b2_staticBody);
		int leftRampPathIn[18] = {
			59, 509,
			84, 484,
			80, 477,
			79, 457,
			88, 450,
			103, 451,
			110, 463,
			101, 479,
			63, 515,
		}; leftRampInner = App->physics->CreateChain(0, 0, leftRampPathIn, 18, b2_staticBody);
		int leftRampWallPath[8]{
			74, 546,
			69, 551,
			58, 520,
			63, 515,
		}; leftRampWall = App->physics->CreateChain(0, 0, leftRampWallPath, 8, b2_staticBody);

		// Middle Ramp
		int middleRampRightPath[44] = {
			221, 420,
			214, 417,
			208, 390,
			192, 252,
			185, 196,
			182, 152,
			181, 119,
			186, 108,
			195, 98,
			216, 87,
			311, 87,
			311, 93,
			216, 93,
			203, 98,
			194, 107,
			186, 118,
			187, 136,
			190, 175,
			202, 293,
			210, 364,
			216, 387,
			223, 405
		};  middleRampRight = App->physics->CreateChain(0, 0, middleRampRightPath, 44, b2_staticBody);
		int middleRampLeftPath[32] = {
			167, 318,
			166, 291,
			161, 257,
			158, 225,
			152, 178,
			148, 140,
			148, 116,
			150, 103,
			143, 103,
			140, 120,
			140, 146,
			146, 181,
			152, 227,
			155, 257,
			160, 293,
			162, 318,
		}; middleRampLeft = App->physics->CreateChain(0, 0, middleRampLeftPath, 32, b2_staticBody);
	}

	dead = App->physics->CreateRectangleSensor(245, 950, 481, 2);
	startPoint = App->physics->CreateRectangleSensor(450, 802, 15, 15);

	// Lobby button
	lobbyL = App->physics->CreateRectangleSensor(200, 178, 12, 5);
	lobbyO = App->physics->CreateRectangleSensor(237, 178, 12, 5);
	lobbyB = App->physics->CreateRectangleSensor(320, 180, 10, 5);
	lobbyB2 = App->physics->CreateRectangleSensor(349, 193, 10, 5);
	lobbyY = App->physics->CreateRectangleSensor(376, 207, 10, 5);

	pointS = App->physics->CreateRectangleSensor(325, 140, 15, 10);
	pointK = App->physics->CreateRectangleSensor(368, 140, 15, 10);
	pointY = App->physics->CreateRectangleSensor(410, 140, 15, 10);

	pointB = App->physics->CreateRectangleSensor(235, 433, 15, 10);
	pointA = App->physics->CreateRectangleSensor(267, 450, 15, 10);
	pointR = App->physics->CreateRectangleSensor(290, 465, 15, 10);
	pointCopa = App->physics->CreateRectangleSensor(106, 728, 10, 10);

	redStar1 = App->physics->CreateRectangleSensor(86, 547, 15, 10);
	redStar2 = App->physics->CreateRectangleSensor(95, 579, 15, 10);
	redStar3 = App->physics->CreateRectangleSensor(108, 608, 15, 10);

	whiteStar1 = App->physics->CreateRectangleSensor(354, 568, 15, 10);
	whiteStar2 = App->physics->CreateRectangleSensor(346, 597, 15, 10);
	whiteStar3 = App->physics->CreateRectangleSensor(337, 628, 15, 10);
	whiteStar4 = App->physics->CreateRectangleSensor(329, 659, 15, 10);

	bumper1 = App->physics->CreateCircle(139, 190, 22, b2_staticBody, 1.0f);
	bumper2 = App->physics->CreateCircle(120, 247, 22, b2_staticBody, 1.0f);
	bumper3 = App->physics->CreateCircle(190, 250, 22, b2_staticBody, 1.0f);

	// Start Wall Colliders
	startTunnelCollider = App->physics->CreateRectangleSensor(450, 120, 10, 10);
	skyAreaCollider = App->physics->CreateRectangleSensor(367, 113, 80, 10);

	// SKY Wall Colliders
	lobbyAreaCollider = App->physics->CreateRectangleSensor(365, 230, 80, 4);

	// Lobby Wall Colliders
	lobbyAreaCollider2 = App->physics->CreateRectangleSensor(290, 432, 50, 4);
	hotelAreaCollider = App->physics->CreateRectangleSensor(291, 493, 60, 4);

	// Right Ramp Colliders
	rightRampInCollider = App->physics->CreateRectangleSensor(378, 456, 10, 10);
	rightRampOutCollider = App->physics->CreateRectangleSensor(397, 563, 10, 10);
	rightRampOutCollider2 = App->physics->CreateRectangleSensor(373, 505, 10, 10);

	// Left Ramp Colliders
	leftRampInCollider = App->physics->CreateRectangleSensor(65, 459, 10, 10);
	leftRampOutCollider = App->physics->CreateRectangleSensor(43, 572, 10, 10);
	leftRampOutCollider2 = App->physics->CreateRectangleSensor(68, 491, 10, 10);

	// Middle Ramp Colliders
	middleRampInCollider = App->physics->CreateRectangleSensor(191, 370, 10, 10);
	middleRampOutCollider = App->physics->CreateRectangleSensor(297, 72, 10, 10);
	middleRampOutCollider2 = App->physics->CreateRectangleSensor(192, 405, 10, 10);


	createBall = true;
	up = false;
	start = true;

	App->audio->PlayFx(song, -1);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// 1RST LAYER
	App->renderer->Blit(bg, 0, 0, NULL, 1.0f);

	// Keep track of lifes and score
	if (lifes == 0)
	{
		App->audio->PlayFx(looseSound);
		previousScore = currentScore;
		for (int i = 0; i < MAX_SCORES; i++)
		{
			if (scoreArray[i] != NULL)
				continue;

			else
			{
				scoreArray[i] = currentScore;
			}
		}
		for (int i = 0; i < MAX_SCORES; i++)
		{
			if (scoreArray[i] != NULL)
			{
				if (scoreArray[i] > scoreArray[i + 1])
				{
					highestScore = scoreArray[i];
				}
				else
				{
					highestScore = scoreArray[i + 1];
				}
			}
		}
		currentScore = 0;
		lifes = 5;
	}

	// Creation of the second ball
	if (secondCreateBall == true)
	{
		ballsCount += 1;
		secondCircles.add(App->physics->CreateCircle(441, 802, 12));
		secondCircles.getLast()->data->listener = this;
		secondballisalive = true;
		secondCreateBall = false;
	}

	// Creation of the first ball
	if (createBall == true)
	{
		ballsCount += 1;
		multi = 1;
		sCond = false;
		kCond = false;
		yCond = false;
		bCond = false;
		aCond = false;
		rCond = false;
		copaCond = false;
		circles.add(App->physics->CreateCircle(441, 802, 12));
		circles.getLast()->data->listener = this;
		createBall = false;
	}

	if (ballsCount == 0)
	{
		start = true;
		createBall = true;
		lifes -= 1;
	}

	// Flippers sound
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperSound);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperSound);
	}

	// Flippers  logic
	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->renderer->Blit(shootTex, 193, 815, NULL, 1.0f);
		App->physics->leftFlipper->body->ApplyForce({ 10, 70 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			App->physics->leftFlipper->body->ApplyForce({ -10, -70 }, { 0, 0 }, true);
		}

		App->physics->leftFlipper2->body->ApplyForce({ 10, 70 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			App->physics->leftFlipper2->body->ApplyForce({ -10, -70 }, { 0, 0 }, true);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->renderer->Blit(shootTex, 193, 815, NULL, 1.0f);
		App->physics->rightFlipper->body->ApplyForce({ -10, -50 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			App->physics->rightFlipper->body->ApplyForce({ 10, 70 }, { 0, 0 }, true);
		}

		App->physics->rightFlipper2->body->ApplyForce({ -10, -70 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			App->physics->rightFlipper2->body->ApplyForce({ 10, 70 }, { 0, 0 }, true);
		}
	}

	// ----- CHANGING COLLIDERS -----
	// Starting Tunnel Wall Control
	if (startTunnel == true)
	{
		startWall->body->SetActive(true);
	}
	else
	{
		startWall->body->SetActive(false);
	}

	// SKY Wall Control
	if (skyWallFlag == true)
	{
		skyWall->body->SetActive(true);
		lobbyB->body->SetActive(true);
		lobbyB2->body->SetActive(true);
		lobbyY->body->SetActive(true);
	}
	else
	{
		skyWall->body->SetActive(false);
		lobbyB->body->SetActive(false);
		lobbyB2->body->SetActive(false);
		lobbyY->body->SetActive(false);
		
	}

	// Lobby Wall Control
	if (lobbyWallFlag == true)
	{
		lobbyWall->body->SetActive(true);
		pointB->body->SetActive(true);
		pointA->body->SetActive(true);
		pointR->body->SetActive(true);
	}
	else
	{
		lobbyWall->body->SetActive(false);
		pointB->body->SetActive(false);
		pointA->body->SetActive(false);
		pointR->body->SetActive(false);
	}

	// Right Ramp Control
	if (rightRampFlag == true)
	{
		rightRampOutter->body->SetActive(true);
		rightRampInner->body->SetActive(true);
		rightRampWall->body->SetActive(false);
	}
	else
	{
		rightRampOutter->body->SetActive(false);
		rightRampInner->body->SetActive(false);
		rightRampWall->body->SetActive(true);
	}

	// Left Ramp Control
	if (leftRampFlag == true)
	{
		leftRampOutter->body->SetActive(true);
		leftRampInner->body->SetActive(true);
		leftRampWall->body->SetActive(false);
	}
	else
	{
		leftRampOutter->body->SetActive(false);
		leftRampInner->body->SetActive(false);
		leftRampWall->body->SetActive(true);
	}

	// Middle Ramp Control
	if (middleRampFlag == true)
	{
		middleRampRight->body->SetActive(true);
		middleRampLeft->body->SetActive(true);
		wall3->body->SetActive(false);
		bumper1->body->SetActive(false);
		bumper3->body->SetActive(false);
		App->physics->leftJoint2->body->SetActive(false);
		App->physics->leftFlipper2->body->SetActive(false);
	}
	else
	{
		middleRampRight->body->SetActive(false);
		middleRampLeft->body->SetActive(false);
		wall3->body->SetActive(true);
		bumper1->body->SetActive(true);
		bumper3->body->SetActive(true);
		App->physics->leftJoint2->body->SetActive(true);
		App->physics->leftFlipper2->body->SetActive(true);
	}

	// BAR buttons texture draw
	if (bCond == true)
	{
		App->renderer->Blit(barTex, 207, 443, &bRect, 1.0f);
	}
	if (aCond == true)
	{
		App->renderer->Blit(barTex, 238, 457, &aRect, 1.0f);
	}
	if (rCond == true)
	{
		App->renderer->Blit(barTex, 269, 474, &rRect, 1.0f);
	}
	if (copaCond == true)
	{
		App->renderer->Blit(copaTex, 98, 686, NULL, 1.0f);
	}

	// Multipliers texture draw
	if (multi == 2)
	{
		App->renderer->Blit(multiTex, 150, 798, &x2, 1.0f);
	}
	else if (multi == 4)
	{
		App->renderer->Blit(multiTex, 150, 798, &x2, 1.0f);
		App->renderer->Blit(multiTex, 188, 789, &x4, 1.0f);
	}
	else if (multi == 6)
	{
		App->renderer->Blit(multiTex, 150, 798, &x2, 1.0f);
		App->renderer->Blit(multiTex, 188, 789, &x4, 1.0f);
		App->renderer->Blit(multiTex, 220, 785, &x6, 1.0f);
	}
	else if (multi == 8)
	{
		App->renderer->Blit(multiTex, 150, 798, &x2, 1.0f);
		App->renderer->Blit(multiTex, 188, 789, &x4, 1.0f);
		App->renderer->Blit(multiTex, 220, 785, &x6, 1.0f);
		App->renderer->Blit(multiTex, 250, 788, &x8, 1.0f);
	}
	else if (multi == 10)
	{
		App->renderer->Blit(multiTex, 150, 798, &x2, 1.0f);
		App->renderer->Blit(multiTex, 188, 789, &x4, 1.0f);
		App->renderer->Blit(multiTex, 220, 785, &x6, 1.0f);
		App->renderer->Blit(multiTex, 250, 788, &x8, 1.0f);
		App->renderer->Blit(multiTex, 285, 798, &x10, 1.0f);
	}

	// Flippers drawing
	int x, y;
	App->physics->leftFlipper->GetPosition(x, y);
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper->GetPosition(x, y);
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));

	App->physics->leftFlipper2->GetPosition(x, y);
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper2->GetPosition(x, y);
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));

	// Bumpers drawing
	SDL_Rect noTouching = { 0, 0, 56, 53 };
	SDL_Rect Touching = { 56, 0, 56, 53 };
	if (touchingBumper1 == true)
	{
		bumper1->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper1->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

	if (touchingBumper2 == true)
	{
		bumper2->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper2->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

	if (touchingBumper3 == true)
	{
		bumper3->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper3->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

	// Sky buttons texture drawing
	if (sCond == true)
	{
		App->renderer->Blit(skyTex, 319, 104, &sRect, 1.0f);
	}
	if (kCond == true)
	{
		App->renderer->Blit(skyTex, 354, 104, &kRect, 1.0f);
	}
	if (yCond == true)
	{
		App->renderer->Blit(skyTex, 394, 104, &yRect, 1.0f);
	}

	if (touchingStar1 == true)
	{
		App->renderer->Blit(starsTex, 88, 531, &redRect, 1.0f);
	}
	if (touchingStar2 == true)
	{
		App->renderer->Blit(starsTex, 99, 561, &redRect, 1.0f);
	}
	if (touchingStar3 == true)
	{
		App->renderer->Blit(starsTex, 110, 591, &redRect, 1.0f);
	}

	if (touchingWhiteStar1 == true)
	{
		App->renderer->Blit(starsTex, 320, 550, &whiteRect, 1.0f);
	}
	if (touchingWhiteStar2 == true)
	{
		App->renderer->Blit(starsTex, 310, 580, &whiteRect, 1.0f);
	}
	if (touchingWhiteStar3 == true)
	{
		App->renderer->Blit(starsTex, 300, 610, &whiteRect, 1.0f);
	}
	if (touchingWhiteStar4 == true)
	{
		App->renderer->Blit(starsTex, 290, 640, &whiteRect, 1.0f);
	}

	if (lobbyLFlag == true)
	{
		App->renderer->Blit(lobbyTex, 205, 182, &lobbyLRect, 1.0f);
	}
	if (lobbyOFlag == true)
	{
		App->renderer->Blit(lobbyTex, 241, 182, &lobbyORect, 1.0f);
	}
	if (lobbyBFlag == true)
	{
		App->renderer->Blit(lobbyTex, 280, 185, &lobbyBRect, 1.0f);
	}
	if (lobbyB2Flag == true)
	{
		App->renderer->Blit(lobbyTex, 317, 199, &lobbyBRect, 1.0f);
	}
	if (lobbyYFlag == true)
	{
		App->renderer->Blit(lobbyTex, 350, 219, &lobbyYRect, 1.0f);
	}

	p2List_item<PhysBody*>* c = circles.getFirst();

	// 3RD LAYER
	if (middleRampFlag == true)
	{
		App->renderer->Blit(longTube, 141, 51, NULL, 1.0f);
	}

	// Second circle logic
	if (secondballisalive == true)
	{
		p2List_item<PhysBody*>* sc = secondCircles.getFirst();
		sc = secondCircles.getFirst();
		b2Vec2 force = { 0.f, -2.05f };
		sc->data->GetPosition(x, y);
		b2Vec2 point = { (float)x, (float)y };
		if (up == false)
		{
			// 2ND LAYER
			App->renderer->Blit(circle, x - 6, y - 6, NULL, 1.0f, sc->data->GetRotation());
		}
		if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) && start == true)
		{
			App->audio->PlayFx(kickerSound);
			sc->data->body->SetFixedRotation(true);
			sc->data->body->ApplyLinearImpulse(force, point, true);
			start = false;
		}
		sc->data->body->SetFixedRotation(false);

		//Ball 2 to mouse debug

		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
			sc->data->body->SetTransform(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
		}
	}

	// First circle logic
	if (c != NULL)
	{
		b2Vec2 force = { 0.f, -2.05f };
		c->data->GetPosition(x, y);
		b2Vec2 point = { (float)x, (float)y };
		if (up == false)
		{
			//2ND LAYER
			App->renderer->Blit(circle, x - 6, y - 6, NULL, 1.0f, c->data->GetRotation());
		}
		if ((App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) && start == true)
		{
			App->audio->PlayFx(kickerSound);
			c->data->body->SetFixedRotation(true);
			c->data->body->ApplyLinearImpulse(force, point, true);
			start = false;
		}
		c->data->body->SetFixedRotation(false);

		//Ball 1 to mouse debug

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
			c->data->body->SetTransform(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())), 0);
		}
	}

	if (up == true)
	{
		App->renderer->Blit(circle, x - 6, y - 6, NULL, 1.0f, c->data->GetRotation());
	}



	if (middleRampFlag == false)
	{
		App->renderer->Blit(longTube, 141, 51, NULL, 1.0f);
	}

	if (leftRampFlag == false)
	{
		App->renderer->Blit(leftSquare, 54, 415, NULL, 1.0f);
	}
	if (rightRampFlag == false)
	{
		App->renderer->Blit(rightSquare, 325, 451, NULL, 1.0f);
	}

	// Multipliers logic
	if (sCond == true && kCond == true && yCond == true)
	{
		App->audio->PlayFx(multiSound);
		if (multi == 1)
		{
			multi += 1;
		}
		else
		{
			multi += 2;
		}
		sCond = false;
		kCond = false;
		yCond = false;
	}

	// Scores Text
	sprintf_s(scoreText, 10, "%d", currentScore);
	sprintf_s(previousScoreText, 10, "%d", previousScore);
	sprintf_s(highestScoreText, 10, "%d", highestScore);
	sprintf_s(lifesText, 10, "%d", lifes);
	App->fonts->BlitText(10, 10, scoreFont, scoreText);
	App->fonts->BlitText(10, 40, scoreFont, previousScoreText);
	App->fonts->BlitText(10, 70, scoreFont, highestScoreText);
	App->fonts->BlitText(10, 100, scoreFont, lifesText);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Control collisions
void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Death control
	if(bodyA->body == dead->body || bodyB->body == dead->body)
	{
		if (circles.getFirst() != NULL)
		{
			if (bodyA->body == circles.getFirst()->data->body || bodyB->body == circles.getFirst()->data->body)
			{
				App->audio->PlayFx(deathSound);
				circles.clear();
				ballsCount -= 1;
			}
		}
		if (secondCircles.getFirst() != NULL)
		{
			if ((bodyA->body == secondCircles.getFirst()->data->body || bodyB->body == secondCircles.getFirst()->data->body) && secondballisalive == true)
			{
				App->audio->PlayFx(deathSound);
				secondCircles.clear();
				secondballisalive = false;
				ballsCount -= 1;
			}
		}
	}

	// Start collision
	if (bodyA->body == startPoint->body || bodyB->body == startPoint->body)
	{
		start = true;
		startTunnel = true;
	}

	// Top of start tunnel
	if (bodyA->body == startTunnelCollider->body || bodyB->body == startTunnelCollider->body)
	{
		startTunnel = false;
	}

	// Sky area collisions
	if (bodyA->body == skyAreaCollider->body || bodyB->body == skyAreaCollider->body)
	{
		startTunnel = true;
		skyWallFlag = false;
	}

	// Lobby area collisions
	if (bodyA->body == lobbyAreaCollider->body || bodyB->body == lobbyAreaCollider->body || bodyA->body == lobbyAreaCollider2->body || bodyB->body == lobbyAreaCollider2->body)
	{
		skyWallFlag = true;
		lobbyWallFlag = false;
	}

	// Hotel area collisions
	if (bodyA->body == hotelAreaCollider->body || bodyB->body == hotelAreaCollider->body)
	{
		lobbyWallFlag = true;
	}

	// Right ramp collisions
	if (bodyA->body == rightRampInCollider->body || bodyB->body == rightRampInCollider->body)
	{
		rightRampFlag = true;
	}
	if (bodyA->body == rightRampOutCollider->body || bodyB->body == rightRampOutCollider->body)
	{
		rightRampFlag = false;
	}

	// Left ramp collisions
	if (bodyA->body == leftRampInCollider->body || bodyB->body == leftRampInCollider->body)
	{
		leftRampFlag = true;
	}
	if (bodyA->body == leftRampOutCollider->body || bodyB->body == leftRampOutCollider->body || bodyA->body == leftRampOutCollider2->body || bodyB->body == leftRampOutCollider2->body)
	{
		leftRampFlag = false;
	}

	// Middle ramp collisions
	if (bodyA->body == middleRampInCollider->body || bodyB->body == middleRampInCollider->body)
	{
		middleRampFlag = true;
	}
	if (bodyA->body == middleRampOutCollider->body || bodyB->body == middleRampOutCollider->body || bodyA->body == middleRampOutCollider2->body || bodyB->body == middleRampOutCollider2->body)
	{
		middleRampFlag = false;
	}


	if (bodyA->body == bumper1->body || bodyB->body == bumper1->body)
	{
		App->audio->PlayFx(bumperSound);
		touchingBumper1 = true;
		currentScore += 100*multi;
	}
	else
	{
		touchingBumper1 = false;
	}

	if (bodyA->body == bumper2->body || bodyB->body == bumper2->body)
	{
		App->audio->PlayFx(bumperSound);
		touchingBumper2 = true;
		currentScore += 100*multi;
	}
	else
	{
		touchingBumper2 = false;
	}

	if (bodyA->body == bumper3->body || bodyB->body == bumper3->body)
	{
		App->audio->PlayFx(bumperSound);
		touchingBumper3 = true;
		currentScore += 100*multi;
	}
	else
	{
		touchingBumper3 = false;
	}

	if (bodyA->body == redStar1->body || bodyB->body == redStar1->body)
	{
		App->audio->PlayFx(kickSound);
		touchingStar1 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingStar1 = false;
	}
	if (bodyA->body == redStar2->body || bodyB->body == redStar2->body)
	{
		App->audio->PlayFx(kickSound);
		touchingStar2 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingStar2 = false;
	}
	if (bodyA->body == redStar3->body || bodyB->body == redStar3->body)
	{
		App->audio->PlayFx(kickSound);
		touchingStar3 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingStar3 = false;
	}

	if (bodyA->body == whiteStar1->body || bodyB->body == whiteStar1->body)
	{
		App->audio->PlayFx(kickSound);
		touchingWhiteStar1 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingWhiteStar1 = false;
	}
	if (bodyA->body == whiteStar2->body || bodyB->body == whiteStar2->body)
	{
		App->audio->PlayFx(kickSound);
		touchingWhiteStar2 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingWhiteStar2 = false;
	}
	if (bodyA->body == whiteStar3->body || bodyB->body == whiteStar3->body)
	{
		App->audio->PlayFx(kickSound);
		touchingWhiteStar3 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingWhiteStar3 = false;
	}
	if (bodyA->body == whiteStar4->body || bodyB->body == whiteStar4->body)
	{
		App->audio->PlayFx(kickSound);
		touchingWhiteStar4 = true;
		currentScore += 20 * multi;
	}
	else
	{
		touchingWhiteStar4 = false;
	}

	if (bodyA->body == lobbyL->body || bodyB->body == lobbyL->body)
	{
		App->audio->PlayFx(kickSound);
		lobbyLFlag = true;
		currentScore += 20 * multi;
	}
	else
	{
		lobbyLFlag = false;
	}
	if (bodyA->body == lobbyO->body || bodyB->body == lobbyO->body)
	{
		App->audio->PlayFx(kickSound);
		lobbyOFlag = true;
		currentScore += 20 * multi;
	}
	else
	{
		lobbyOFlag = false;
	}
	if (bodyA->body == lobbyB->body || bodyB->body == lobbyB->body)
	{
		App->audio->PlayFx(kickSound);
		lobbyBFlag = true;
		currentScore += 20 * multi;
	}
	else
	{
		lobbyBFlag = false;
	}
	if (bodyA->body == lobbyB2->body || bodyB->body == lobbyB2->body)
	{
		App->audio->PlayFx(kickSound);
		lobbyB2Flag = true;
		currentScore += 20 * multi;
	}
	else
	{
		lobbyB2Flag = false;
	}
	if (bodyA->body == lobbyY->body || bodyB->body == lobbyY->body)
	{
		App->audio->PlayFx(kickSound);
		lobbyYFlag = true;
		currentScore += 20 * multi;
	}
	else
	{
		lobbyYFlag = false;
	}

	if (bodyA->body == pointY->body || bodyB->body == pointY->body)
	{
		if (yCond != true && multi < 10)
		{
			App->audio->PlayFx(skySound);
			yCond = true;
		}
	}
	if (bodyA->body == pointK->body || bodyB->body == pointK->body)
	{
		if (kCond != true && multi < 10)
		{
			App->audio->PlayFx(skySound);
			kCond = true;
		}
	}
	if (bodyA->body == pointS->body || bodyB->body == pointS->body)
	{
		if (sCond != true && multi < 10)
		{
			App->audio->PlayFx(skySound);
			sCond = true;
		}
	}

	if (bodyA->body == pointB->body || bodyB->body == pointB->body)
	{
		if (bCond != true)
		{
			App->audio->PlayFx(barSound);
			bCond = true;
		}
	}
	if (bodyA->body == pointA->body || bodyB->body == pointA->body)
	{
		if (aCond != true)
		{
			App->audio->PlayFx(barSound);
			aCond = true;
		}
	}
	if (bodyA->body == pointR->body || bodyB->body == pointR->body)
	{
		if (rCond != true)
		{
			App->audio->PlayFx(barSound);
			rCond = true;
		}
	}
	if (bodyA->body == pointCopa->body || bodyB->body == pointCopa->body)
	{
		if (copaCond != true)
		{
			if (bCond == true && aCond == true && rCond == true)
			{
				App->audio->PlayFx(barSound);
				copaCond = true;
				secondCreateBall = true;
			}
		}
	}
}