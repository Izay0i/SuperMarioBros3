#pragma once

class ScenePlay : public Scene {
public:
	ScenePlay(SceneType, std::string);
	~ScenePlay();

	void HandleStates() override;
	void OnKeyUp(int) override;
	void OnKeyDown(int) override;

	void UpdateCameraPosition() override;

	void Update(DWORD);
	void Render();

	void Release();
};