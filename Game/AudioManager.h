#pragma once

#include <vector>
#include <string>

using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Playback;
using namespace Windows::Media::Core; 
using namespace Windows::Foundation;
using namespace Platform;

class AudioManager
{
public:
	AudioManager() = default;
	~AudioManager() = default;

	bool Initialize(std::string);
	bool Shutdown();

	void PlayBackgroundMusic();

private:
	// Media players
	MediaPlayer^ music_loop;
	MediaPlayer^ character_player;
	std::vector<MediaPlayer^> sfx_player;

	MediaSource^ bg_loop;

	// Volume
	float main_loop_volume = .2;
};