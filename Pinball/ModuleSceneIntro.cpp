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

	circle = App->textures->Load("pinball/ball.png"); 
	bg = App->textures->Load("pinball/background.png");
	leftFlipper = App->textures->Load("pinball/left_flipper.png");
	rightFlipper = App->textures->Load("pinball/right_flipper.png");

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
	}; App->physics->CreateChain(0, 0, walls, 154);
	int wall1[20] = {
		226, 428,
		264, 442,
		263, 424,
		255, 409,
		253, 387,
		244, 379,
		219, 373,
		220, 386,
		229, 410,
		229, 421
	}; App->physics->CreateChain(0, 0, wall1, 20);
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
	}; App->physics->CreateChain(0, 0, wall2, 66);

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
	}; App->physics->CreateChain(0, 0, leftGround, 28);
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
	}; App->physics->CreateChain(0, 0, rightGround, 24);

	int leftBumper[12] = {
		121, 768,
		122, 726,
		127, 722,
		153, 770,
		153, 783,
		148, 783
	}; App->physics->CreateChain(0, 0, leftBumper, 12);
	int rightBumper[12] = {
		320, 782,
		328, 782,
		352, 767,
		352, 727,
		347, 723,
		320, 772
	}; App->physics->CreateChain(0, 0, rightBumper, 12);

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
	}; App->physics->CreateChain(0, 0, leftTube, 26);
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
	}; App->physics->CreateChain(0, 0, rightTube, 20);

	int upWall1[12] = {
		344, 152,
		351, 149,
		351, 133,
		346, 129,
		340, 133,
		340, 149
	}; App->physics->CreateChain(0, 0, upWall1, 12);
	int upWall2[12] = {
		386, 152,
		393, 148,
		393, 132,
		387, 129,
		381, 134,
		381, 149
	}; App->physics->CreateChain(0, 0, upWall2, 12);
	int downWall[12] = {
		88, 754,
		93, 750,
		93, 726,
		88, 722,
		84, 726,
		84, 751
	}; App->physics->CreateChain(0, 0, downWall, 12);

	int lf[14] = {
		151, 847,
		211, 862,
		218, 856,
		161, 824,
		149, 824,
		143, 834,
		145, 843
	}; leftflippers.add(App->physics->CreateChain(0, 0, lf, 14));
	int rf[14] = {
		263, 862,
		322, 846,
		328, 837,
		327, 828,
		319, 823,
		309, 825,
		256, 857
	}; rightflippers.add(App->physics->CreateChain(0, 0, rf, 14));

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
	p2List_item<PhysBody*>* lf = leftflippers.getFirst();
	p2List_item<PhysBody*>* rf;
	App->renderer->Blit(bg, 0, 0, NULL, 1.0f);

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
	}
	p2List_item<PhysBody*>* c = circles.getFirst();

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->renderer->Blit(leftFlipper, 145, 810, &up, 1.0f);
	}
	else
	{
		App->renderer->Blit(leftFlipper, 145, 824, &down, 1.0f);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->renderer->Blit(rightFlipper, 255, 810, &up, 1.0f);
	}
	else
	{
		App->renderer->Blit(rightFlipper, 255, 824, &down, 1.0f);
	}
	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	while(c != NULL)
	{
		int x, y;
		b2Vec2 force = { 0.f, -2.f };
		c->data->GetPosition(x, y);
		b2Vec2 point = { (float)x, (float)y };
		App->renderer->Blit(circle, x-6, y-6, NULL, 1.0f, c->data->GetRotation());
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN))
		{
			c->data->body->SetFixedRotation(true);
			c->data->body->ApplyLinearImpulse(force, point, true);
		}
		c->data->body->SetFixedRotation(false);
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
