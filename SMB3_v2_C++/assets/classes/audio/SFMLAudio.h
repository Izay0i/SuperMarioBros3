#pragma once

#include <deque>

#include <SFML/Audio.hpp>

struct SoundStruct {
	sf::SoundBuffer buffer;
	sf::Sound sound;
};

class SFMLAudio : public Audio {
private:
	std::unordered_map<AudioType, SoundStruct> _audioMap;

public:
	void AddAudio(AudioType, std::string);

	void Initialize() override;

	void PlayAudio(AudioType, bool = false, float = 1.0f, float = 100.0f) override;
	void PauseAudio(AudioType) override;
	void StopAudio(AudioType) override;
	void StopAll() override;

	void Release() override;
};