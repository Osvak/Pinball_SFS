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

	flipperSound = App->audio->LoadFx("pinball/flipper.wav");
	kickerSound = App->audio->LoadFx("pinball/kicker.wav");
	deathSound = App->audio->LoadFx("pinball/death.wav");
	bumperSound = App->audio->LoadFx("pinball/bumper.wav");
	skySound = App->audio->LoadFx("pinball/sky.wav");
	multiSound = App->audio->LoadFx("pinball/multi.wav");
	song = App->audio->LoadFx("pinball/song.wav");
	barSound = App->audio->LoadFx("pinball/barSound.wav");

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

	int walls[154] = {
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
		323, 415,
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
	}; App->physics->CreateChain(0, 0, walls, 154, b2_staticBody);
	// Pivot 0, 0
	int wall1[20] = {
		257, 441,
		228, 429,
		220, 421,
		213, 415,
		210, 406,
		207, 375,
		215, 372,
		243, 378,
		244, 385,
		255, 410
	}; App->physics->CreateChain(0, 0, wall1, 20, b2_staticBody);
	int wall2[66] = {
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
		310, 152,
		304, 152,
		303, 140,
		292, 122,
		282, 112,
		266, 104,
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
		167, 318
	}; App->physics->CreateChain(0, 0, wall2, 66, b2_staticBody);

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
	}; leftBumper = App->physics->CreateChain(0, 0, leftBumperPoints, 12, b2_staticBody);
	int rightBumperPoints[12] = {
		320, 782,
		328, 782,
		352, 767,
		352, 727,
		347, 723,
		320, 772
	}; rightBumper = App->physics->CreateChain(0, 0, rightBumperPoints, 12, b2_staticBody);

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

	// Pivot 0, 0
	int longTube[44] = {
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
	}; lt = App->physics->CreateChain(0, 0, longTube, 44, b2_staticBody);
	lt->body->SetActive(false);
	// Pivot 0, 0
	int wallPoint[8] = {
		311, 47,
		311, 12,
		305, 12,
		305, 47
	}; wall = App->physics->CreateChain(0, 0, wallPoint, 8, b2_staticBody);
	wall->body->SetActive(true);

	dead = App->physics->CreateRectangleSensor(245, 950, 481, 2);
	layer1 = App->physics->CreateRectangleSensor(195, 396, 15, 15);
	startPoint = App->physics->CreateRectangleSensor(450, 802, 15, 15);

	pointS = App->physics->CreateRectangleSensor(325, 140, 15, 10);
	pointK = App->physics->CreateRectangleSensor(368, 140, 15, 10);
	pointY = App->physics->CreateRectangleSensor(410, 140, 15, 10);

	pointB = App->physics->CreateRectangleSensor(235, 433, 15, 10);
	pointA = App->physics->CreateRectangleSensor(267, 450, 15, 10);
	pointR = App->physics->CreateRectangleSensor(290, 465, 15, 10);
	pointCopa = App->physics->CreateRectangleSensor(106, 728, 10, 10);

	bumper1 = App->physics->CreateCircle(139, 190, 22, b2_staticBody);
	bumper2 = App->physics->CreateCircle(120, 247, 22, b2_staticBody);
	bumper3 = App->physics->CreateCircle(190, 250, 22, b2_staticBody);

	createBall = true;
	up = false;
	start = true;

	//App->audio->PlayFx(song, -1);

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
	//1RST LAYER
	App->renderer->Blit(bg, 0, 0, NULL, 1.0f);

	if (lifes == 0)
	{
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

	if (secondCreateBall == true)
	{
		ballsCount += 1;
		secondCircles.add(App->physics->CreateCircle(353, 68, 12));
		secondCircles.getLast()->data->listener = this;
		secondballisalive = true;
		secondCreateBall = false;
	}

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

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperSound);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->audio->PlayFx(flipperSound);
	}

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

	int x, y;
	App->physics->leftFlipper->GetPosition(x, y);
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper->GetPosition(x, y);
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));

	App->physics->leftFlipper2->GetPosition(x, y);
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper2->GetPosition(x, y);
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));

	SDL_Rect noTouching = { 0, 0, 56, 53 };
	SDL_Rect Touching = { 56, 0, 56, 53 };
	if (touching1 == true)
	{
		bumper1->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper1->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

	if (touching2 == true)
	{
		bumper2->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper2->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

	if (touching3 == true)
	{
		bumper3->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &Touching, 1.0f);
	}
	else
	{
		bumper3->GetPosition(x, y);
		App->renderer->Blit(bumperTex, x - 5, y - 3, &noTouching, 1.0f);
	}

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

	p2List_item<PhysBody*>* c = circles.getFirst();

	x, y;
	b2Vec2 force = { 0.f, -2.f };
	c->data->GetPosition(x, y);0
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

	if (secondballisalive == true)
	{
		c = secondCircles.getFirst();
		x, y;
		force = { 0.f, -2.f };
		c->data->GetPosition(x, y);
		point = { (float)x, (float)y };
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
	}

	//3RD LAYER
	App->renderer->Blit(longTube, 141, 51, NULL, 1.0f);
	App->renderer->Blit(leftSquare, 54, 415, NULL, 1.0f);
	App->renderer->Blit(rightSquare, 325, 451, NULL, 1.0f);

	if (up == true)
	{
		App->renderer->Blit(circle, x - 6, y - 6, NULL, 1.0f, c->data->GetRotation());
	}

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

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//CreateBall();
	if(bodyA->body == dead->body || bodyB->body == dead->body)
	{
		if (bodyA->body == circles.getFirst()->data->body || bodyB->body == circles.getFirst()->data->body)
		{
			App->audio->PlayFx(deathSound);
			circles.clear();
		}
		/*if (secondCircles.getFirst()->data->body != NULL)
		{
			if ((bodyA->body == secondCircles.getFirst()->data->body || bodyB->body == secondCircles.getFirst()->data->body) && secondballisalive == true)
			{
				App->audio->PlayFx(deathSound);
				secondCircles.clear();
				secondballisalive = false;
			}
		}*/
	}

	if (bodyA->body == layer1->body || bodyB->body == layer1->body)
	{
		/*up = true;
		if (!lt->body->IsActive())
		{
			lt->body->SetActive(true);
		}
		if (App->physics->leftFlipper2->body->IsActive())
		{
			App->physics->leftFlipper2->body->SetActive(false);
		}
		if (App->physics->leftJoint2->body->IsActive())
		{
			App->physics->leftJoint2->body->SetActive(false);
		}*/
	}

	if (bodyA->body == leftBumper->body || bodyB->body == leftBumper->body)
	{
		int x, y;
		b2Vec2 force = { 0.5f, -0.5f };
		circles.getFirst()->data->GetPosition(x, y);
		b2Vec2 point = { (float)x, (float)y };
		circles.getFirst()->data->body->ApplyForce(force, point, true);
	}

	if (bodyA->body == startPoint->body || bodyB->body == startPoint->body)
	{
		start = true;
	}

	if (bodyA->body == bumper1->body || bodyB->body == bumper1->body)
	{
		App->audio->PlayFx(bumperSound);
		touching1 = true;
		currentScore += 100*multi;
	}
	else
	{
		touching1 = false;
	}

	if (bodyA->body == bumper2->body || bodyB->body == bumper2->body)
	{
		App->audio->PlayFx(bumperSound);
		touching2 = true;
		currentScore += 100*multi;
	}
	else
	{
		touching2 = false;
	}

	if (bodyA->body == bumper3->body || bodyB->body == bumper3->body)
	{
		App->audio->PlayFx(bumperSound);
		touching3 = true;
		currentScore += 100*multi;
	}
	else
	{
		touching3 = false;
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
			secondCreateBall = true;
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
			//App->audio->PlayFx(barSound);
			copaCond = true;
			secondCreateBall = true;
		}
	}
}
