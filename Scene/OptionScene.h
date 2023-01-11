#pragma once
#include "Secne.h"
#include "../game.h"

enum ItemConfig
{
	configKeyconfig,
	configBack,

	configMax
};

struct ElementConfig
{
	int x, y;
	int color;
};

namespace
{
	constexpr int kconfigWidth = 500;
	constexpr int kconfigHeight = 400;
	constexpr int kconfigStartX = (Game::kScreenWidth - kconfigWidth) / 2;
	constexpr int kconfigStartY = (Game::kScreenHeight - kconfigHeight) / 2;

	constexpr int kFontSize = 50;
}

class InputState;
class SceneManager;
/// <summary>
/// オプションシーン
/// </summary>
class OptionScene : public Scene
{
public:
	//◇同時に継承元のコンストラクタが呼ばれる.Scene(SceneManager& manager)
	OptionScene(SceneManager& manager);
	~OptionScene();

	void Update(const InputState& input);
	void Draw();

	ElementConfig m_configMenu[configMax] = {
		{kconfigStartX + 20,kconfigStartY + kFontSize+10 + 20 * configKeyconfig,0xAAD8E6},
		{kconfigStartX + 10,kconfigStartY + kFontSize+10 + 20 * configBack,0xffffff},
	};
	int m_selectNum = 1;
private:
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (OptionScene::* m_updateFunc)(const InputState& input);
};

