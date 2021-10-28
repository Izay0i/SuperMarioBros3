#pragma once

//Hard-coding galore
class SceneIntro : public Scene {
public:
	SceneIntro(SceneType, std::string);
	~SceneIntro();

	void OnKeyDown(int) override;

	void LoadScene() override;

	void Update(DWORD) override;
	void Render() override;

	void Release() override;
};