#pragma once

class ScenePlay : public Scene {
private:
	const unsigned int _NEAR_TIME_LIMIT = 100;

	//I blame sf::SoundSource::Status not working
	bool _isInHurry;
	bool _hurryUp;

	//Again, I blame sf::SoundSource::Status not working
	bool _isInSecret;

public:
	ScenePlay(SceneType, std::string);
	~ScenePlay();

	void HandleStates() override;
	void OnKeyUp(int) override;
	void OnKeyDown(int) override;

	void LoadScene() override;

	void UpdateCameraPosition() override;

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};