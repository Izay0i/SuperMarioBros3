#include "AudioService.h"

Audio* AudioService::_audio = nullptr;
NullAudio AudioService::_nullAudio;

Audio& AudioService::GetAudio() {
	return *_audio;
}

void AudioService::SetAudio(Audio* audio) {
	if (audio == nullptr) {
		_audio = &_nullAudio;
	}
	else {
		_audio = audio;
		_audio->Initialize();
	}
}