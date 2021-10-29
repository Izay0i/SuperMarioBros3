#include <Windows.h>

#include <string>
#include <unordered_map>

#include "Audio.h"
#include "SFMLAudio.h"

void SFMLAudio::AddAudio(AudioType type, std::string fileName) {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(fileName)) {
		OutputDebugStringA("[SFML Audio] Failed to load sound file to buffer\n");
		return;
	}
	SoundStruct soundStruct;
	soundStruct.buffer = std::move(buffer);
	_audioMap.emplace(std::make_pair(type, std::move(soundStruct)));
}

void SFMLAudio::Initialize() {
	AddAudio(AudioType::AUDIO_TYPE_INTRO, "assets\\audio\\songs\\Intro.ogg");
	AddAudio(AudioType::AUDIO_TYPE_MAP, "assets\\audio\\songs\\GrassLand.ogg");
	AddAudio(AudioType::AUDIO_TYPE_STAGE_1_1, "assets\\audio\\songs\\LevelTheme1.ogg");
	AddAudio(AudioType::AUDIO_TYPE_STAGE_HURRY, "assets\\audio\\songs\\HurryUp.ogg");
	AddAudio(AudioType::AUDIO_TYPE_STAGE_END, "assets\\audio\\songs\\LevelClear.ogg");
	AddAudio(AudioType::AUDIO_TYPE_1UP, "assets\\audio\\sfx\\1UP.ogg");
	AddAudio(AudioType::AUDIO_TYPE_BLOCKBREAK, "assets\\audio\\sfx\\Block Break.ogg");
	AddAudio(AudioType::AUDIO_TYPE_BOOMERANG, "assets\\audio\\sfx\\Boomerang.ogg");
	AddAudio(AudioType::AUDIO_TYPE_BUMP, "assets\\audio\\sfx\\Bump.ogg");
	AddAudio(AudioType::AUDIO_TYPE_COIN, "assets\\audio\\sfx\\Coin.ogg");
	AddAudio(AudioType::AUDIO_TYPE_DEATH, "assets\\audio\\sfx\\Death.ogg");
	AddAudio(AudioType::AUDIO_TYPE_FIREBALL, "assets\\audio\\sfx\\Fireball.ogg");
	AddAudio(AudioType::AUDIO_TYPE_KICK, "assets\\audio\\sfx\\Kick.ogg");
	AddAudio(AudioType::AUDIO_TYPE_LEVELSTART, "assets\\audio\\sfx\\Level Start.ogg");
	AddAudio(AudioType::AUDIO_TYPE_MAPMOVE, "assets\\audio\\sfx\\Map Move.ogg");
	AddAudio(AudioType::AUDIO_TYPE_MAPSTART, "assets\\audio\\sfx\\Map Start.ogg");
	AddAudio(AudioType::AUDIO_TYPE_JUMP, "assets\\audio\\sfx\\Mario Jump.ogg");
	AddAudio(AudioType::AUDIO_TYPE_PIPE, "assets\\audio\\sfx\\Pipe.ogg");
	AddAudio(AudioType::AUDIO_TYPE_POWERUP, "assets\\audio\\sfx\\Powerup.ogg");
	AddAudio(AudioType::AUDIO_TYPE_SKID, "assets\\audio\\sfx\\Skid.ogg");
	AddAudio(AudioType::AUDIO_TYPE_SQUISH, "assets\\audio\\sfx\\Squish.ogg");
	AddAudio(AudioType::AUDIO_TYPE_TRANSFORM, "assets\\audio\\sfx\\Transformation.ogg");
}

void SFMLAudio::PlayAudio(AudioType type, bool isLooping, float pitch, float volume) {
	//WTF SFML???
	_audioMap.at(type).sound.setBuffer(_audioMap.at(type).buffer);
	_audioMap.at(type).sound.setLoop(isLooping);
	_audioMap.at(type).sound.setPitch(pitch);
	_audioMap.at(type).sound.setVolume(volume);
	_audioMap.at(type).sound.play();
}

void SFMLAudio::PauseAudio(AudioType type) {
	_audioMap.at(type).sound.pause();
}

void SFMLAudio::StopAudio(AudioType type) {
	_audioMap.at(type).sound.stop();
}

void SFMLAudio::StopAll() {
	for (auto& sound : _audioMap) {
		sound.second.sound.stop();
	}
}

void SFMLAudio::Release() {
	_audioMap.clear();
}