#include "Main.h"

void playMusic(const char* musicName)
{
	while (!AUDIO::PREPARE_MUSIC_EVENT((char*)musicName))
	{
		WAIT(10);
	}
	AUDIO::TRIGGER_MUSIC_EVENT((char*)musicName);
}

void stopMusic(const char* musicName)
{
	AUDIO::CANCEL_MUSIC_EVENT((char*)musicName);
}