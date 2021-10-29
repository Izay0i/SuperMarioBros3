#pragma once

class NullAudio : public Audio {
public:
	void Initialize() override;

	void PlayAudio(AudioType, bool = false, float = 1.0f, float = 100.0f) override;
	void PauseAudio(AudioType) override;
	void StopAudio(AudioType) override;
	void StopAll() override;

	void Release() override;
};