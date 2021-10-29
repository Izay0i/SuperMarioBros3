#pragma once

#include "Audio.h"
#include "NullAudio.h"

class AudioService {
private:
	static Audio* _audio;
	static NullAudio _nullAudio;

public:
	static Audio& GetAudio();
	static void SetAudio(Audio*);
};