#include "AllegroSystem.h"
#include "AllegroScreen.h"
#include "AllegroKeyboard.h"

#include <iostream>
#include <string>
#include <allegro.h>

void IncrementSpeedCounter()
{
	AllegroSystem::SpeedCounter++;
	//	LastFps = Fps;
	//	Fps = 0;
	//	AvgFps = (AvgFps * LastFps) / 2;
}

END_OF_FUNCTION(IncrementSpeedCounter);


void TimeTicker()
{
	AllegroSystem::TimeTicks++;
}

END_OF_FUNCTION(TimeTicker);


AllegroSystem *AllegroSystem::mInstance = 0;
volatile long AllegroSystem::SpeedCounter = 0;
volatile long AllegroSystem::TimeTicks = 0;
volatile int AllegroSystem::Fps = 0;
volatile int AllegroSystem::AvgFps = 0;
volatile int AllegroSystem::LastFps = 0;

AllegroSystem::AllegroSystem()
{
	allegro_init();
}

AllegroSystem::~AllegroSystem()
{
}

AllegroSystem *AllegroSystem::Instance()
{
	if (!mInstance)
		mInstance = new AllegroSystem();

	return mInstance;
}

bool AllegroSystem::setupScreen(const ObjectSize &size,
	bool fullScreen,
	ColorDepth colorDepth)
{
	try {
		mScreen = new AllegroScreen(size, fullScreen, colorDepth);
	} catch (std::string e) {
		std::cout << "exception occured : " << e << std::endl;
		return false;
	}

	return true;
}

AllegroKeyboard *AllegroSystem::installKeyboard()
{
	return (mKeyboard = new AllegroKeyboard());
}

void AllegroSystem::initializeTimers()
{
	install_timer();

	LOCK_VARIABLE(AllegroSystem::SpeedCounter);
	LOCK_FUNCTION(IncrementSpeedCounter);
	install_int_ex(IncrementSpeedCounter, BPS_TO_TIMER(30));

	LOCK_VARIABLE(TimeTicks);
	LOCK_FUNCTION(TimeTicker);
	install_int_ex(TimeTicker, BPS_TO_TIMER(30));
}

void AllegroSystem::pollEvents()
{
	if (mKeyboard)
		mKeyboard->update();
}

void AllegroSystem::waitForTicks()
{
}


