#pragma once
#include "Secne.h"

/// <summary>
/// オプションシーン
/// </summary>
class OptionScene : public Scene
{
public:
	//◇同時に継承元のコンストラクタが呼ばれる.Scene(SceneManager& manager)
	OptionScene(SceneManager& manager) : Scene(manager) {};
	~OptionScene() {};

	void Update(const InputState& input);
	void Draw();
};

