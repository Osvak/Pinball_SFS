#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"

#define MAX_FRAMES 25

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	bool finish = false;

private:
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:
	float currentFrame = 0.0f;

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		finish = false;
		currentFrame = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1; // si tienes un loop o un ping pong y estas en el maximo de frames, vuelves al primero (0.0f) o te mantienes en el �ltimo (totalFrames - 1)
			++loopCount;

			finish = true;

			if (pingpong)
				pingpongDirection = -pingpongDirection;

		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void SetCurrentFrame(int frame)
	{
		currentFrame = frame;
	}
};

#endif