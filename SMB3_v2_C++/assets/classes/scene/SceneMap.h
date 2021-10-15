#pragma once

class SceneMap : public Scene {
public:
	SceneMap(SceneType, std::string);
	~SceneMap();

	void HandleStates() override;
	void OnKeyDown(int) override;

	void LoadScene() override;

	void Update(DWORD);
	void Render();

	void Release();
};