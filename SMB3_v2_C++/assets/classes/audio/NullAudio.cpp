#include <Windows.h>

#include "Audio.h"
#include "NullAudio.h"

void NullAudio::Initialize() {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}

void NullAudio::PlayAudio(AudioType, bool, float, float) {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}

void NullAudio::PauseAudio(AudioType) {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}

void NullAudio::StopAudio(AudioType) {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}

void NullAudio::StopAll() {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}

void NullAudio::Release() {
	OutputDebugStringA("[NULL AUDIO] Fallback, no sound\n");
}