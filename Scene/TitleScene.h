#pragma once
#include "Secne.h"
#include "../game.h"

namespace
{
	constexpr int kMenuFontSize = 50;//文字のサイズ
	constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize;    //メニュー文字のx位置
	constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize;    //メニュー文字のy位置
	constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//メニュー文字の移動したx位置
	constexpr float kBlockSize = Game::kScreenWidth / 20;//ブロックのサイズ

	//メニュー項目
	enum MenuItem
	{
		menuGameStart,	//ゲームスタート
		menuOperation,	//操作説明
		menuGameEnd,	//ゲーム終了

		menuNum			//項目の数
	};
	//メニュー要素
	struct MenuElement
	{
		int x;
		int y;				//座標
	};
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
	float m_blockX = 0.0f;
	float m_blockY = -kBlockSize;

	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& input);

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuOperation},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd}
	};

public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

