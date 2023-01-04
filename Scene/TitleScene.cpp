#include "TitleScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"
#include "PauseScene.h"

namespace
{
	constexpr int kOriginalPosX = Game::kScreenWidth / 3;    //メニュー文字の元のx位置
	constexpr int kOriginalPosY = Game::kScreenHeight / 2;    //メニュー文字の元のy位置
	constexpr int kTitleFontSize = 70;//文字列の幅
	constexpr int kFontSize = 60;//文字列の幅
	constexpr int kMovedPosX = kOriginalPosX - kFontSize;//メニュー文字の移動したx位置

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuGameEnd}
	};
}

void TitleScene::FadeInUpdat(const InputState& input)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input)
{
	if (input.IsTriggered(InputType::down))
	{//下キーが押されていたら
		m_selectNum = (m_selectNum + 1) % menuNum;//選択状態を一つ下げる
	}
	else if (input.IsTriggered(InputType::up))
	{//上キーが押されていたら
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//選択状態を一つ上げる
	}

	if (input.IsTriggered(InputType::down) || input.IsTriggered(InputType::up))
	{
		// メニュー項目数である4個ループ処理
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//移動位置に移動する
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//元の位置に戻す
			}
		}
	}
	//「次へ」ボタンが押されたら次シーンへ移行する
	if (input.IsTriggered(InputType::next))
	{
		//押されたらNormalUpdatからFadeOutUpdatに変更する
		m_updateFunc = &TitleScene::FadeOutUpdat;
	}
	if (input.IsTriggered(InputType::prev))
	{
		m_manager.ChangeScene(new OptionScene(m_manager));
		return;
	}
}

void TitleScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (++m_fadeTimer == kFadeInterval)
	{
		//現在選択中の状態によって処理を分岐
		switch (m_selectNum)
		{
		case menuGameStart:
			m_manager.ChangeScene(new GameplayingScene(m_manager));
			return;
		case menuConfig:
			m_updateFunc = &TitleScene::FadeInUpdat;
			m_manager.PushScene(new PauseScene(m_manager));
			return;
		case menuGameEnd:
			m_manager.SetIsEnd();
			return;
		}
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat)
{
	
}

TitleScene::~TitleScene()
{
	
}

void
TitleScene::Update(const InputState& input)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	//メニュー項目を描画
	SetFontSize(kTitleFontSize);
	DrawFormatString(kOriginalPosX, Game::kScreenHeight / 3 - kTitleFontSize, 0xffffff, L"テトリス");
	SetFontSize(kFontSize);
	DrawFormatString(SelectMenu[menuGameStart].x, SelectMenu[menuGameStart].y, 0xffffff, L"ゲームスタート");
	DrawFormatString(SelectMenu[menuConfig].x, SelectMenu[menuConfig].y, 0xffffff, L"設定");
	DrawFormatString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, 0xffffff, L"ゲーム終了");
	SetFontSize(0);

	//今から書く画像と、すでに描画されているスクリーンとのブレンドの仕方をしている。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	//◇上から消えるMULADrawGraph(0, 0, gradH_, true);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	//一度変更するとずっと残るのでNOBLEND,0に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}