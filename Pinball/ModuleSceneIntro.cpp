#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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

	circle = App->textures->Load("pinball/ball.png"); 
	bg = App->textures->Load("pinball/background.png");
	leftFlipperTex = App->textures->Load("pinball/left_flipper.png");
	rightFlipperTex = App->textures->Load("pinball/right_flipper.png");
	longTube = App->textures->Load("pinball/tube.png");
	leftSquare = App->textures->Load("pinball/side.png");
	rightSquare = App->textures->Load("pinball/side2.png");

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

	createBall = true;
	up = false;
	start = true;

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
	App->renderer->Blit(bg, 0, 0, NULL, 1.0f);

	if (createBall == true)
	{
		circles.add(App->physics->CreateCircle(441, 802, 12));
		circles.getLast()->data->listener = this;
		createBall = false;
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
		App->physics->leftFlipper->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			App->physics->leftFlipper->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
		}

		App->physics->leftFlipper2->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			App->physics->leftFlipper2->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->physics->rightFlipper->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			App->physics->rightFlipper->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		}

		App->physics->rightFlipper2->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			App->physics->rightFlipper2->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		}
	}

	int x, y;
	App->physics->leftFlipper->GetPosition(x, y);
	b2Vec2 point = { (float)x, (float)y };
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper->GetPosition(x, y);
	//point = { (float)x, (float)y };
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));

	App->physics->leftFlipper2->GetPosition(x, y);
	//point = { (float)x, (float)y };
	App->renderer->Blit(leftFlipperTex, x, y, NULL, 1.0f, App->physics->leftFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(12), PIXEL_TO_METERS(12));

	App->physics->rightFlipper2->GetPosition(x, y);
	//point = { (float)x, (float)y };
	App->renderer->Blit(rightFlipperTex, x, y, NULL, 1.0f, App->physics->rightFlipper2->body->GetAngle() * RADTODEG, PIXEL_TO_METERS(60), PIXEL_TO_METERS(12));
	
	p2List_item<PhysBody*>* c = circles.getFirst();

	x, y;
	b2Vec2 force = { 0.f, -2.f };
	c->data->GetPosition(x, y);
	point = { (float)x, (float)y };
	if (up == false)
	{
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

	App->renderer->Blit(longTube, 141, 51, NULL, 1.0f);
	App->renderer->Blit(leftSquare, 54, 415, NULL, 1.0f);
	App->renderer->Blit(rightSquare, 325, 451, NULL, 1.0f);

	if (up == true)
	{
		App->renderer->Blit(circle, x - 6, y - 6, NULL, 1.0f, c->data->GetRotation());
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//CreateBall();
	if(bodyA->body == dead->body || bodyB->body == dead->body)
	{
		App->audio->PlayFx(deathSound);
		start = true;
		createBall = true;
		circles.clear();
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
		
	}
}
