#pragma once
#include "Secne.h"
#include "../game.h"

//メニュー項目
enum MenuItem
{
	menuGameStart,	//ゲームスタート
	menuConfig,		//設定
	menuGameEnd,	//ゲーム終了

	menuNum			//項目の数
};
//メニュー要素
struct MenuElement
{
	int x;
	int y;				//座標
};

namespace
{
	constexpr int kFontSize = 50;//文字のサイズ
	constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kFontSize;    //メニュー文字のx位置
	constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kFontSize;    //メニュー文字のy位置
	constexpr int kMovedPosX = kOriginalPosX - kFontSize;//メニュー文字の移動したx位置
}

class InputState;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//選択しているメニュー項目
	int m_titleH = -1;
	int m_bgH = -1;

	//ブロックの位置
	float m_blockX = 0;
	float m_blockY = 0;

	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& input);

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuGameEnd}
	};

public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

