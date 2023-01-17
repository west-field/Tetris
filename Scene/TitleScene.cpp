#include "TitleScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Sound.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OperateScene.h"

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
	m_blockY += 1.0f;
	if (m_blockY >= Game::kScreenHeight)
	{
		m_blockY = -kBlockSize;
	}

	bool isPress = false;//キーが押されたかどうか
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//選択状態を一つ下げる
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//選択状態を一つ上げる
		isPress = true;
	}

	if (isPress)
	{
		Sound::Play(Sound::Cursor);
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
		Sound::Play(Sound::Determinant);
		m_updateFunc = &TitleScene::FadeOutUpdat;
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
		case menuOperation:
			m_manager.ChangeScene(new OperateScene(m_manager));
			return;
		case menuGameEnd:
			m_manager.SetIsEnd();
			return;
		}
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat)
{
	m_titleH = my::MyLoadGraph(L"Data/title.png");
	m_bgH = my::MyLoadGraph(L"Data/bg.png");
	Sound::StartBgm(Sound::BgmMain);
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_titleH);
	DeleteGraph(m_bgH);
	Sound::StopBgm(Sound::BgmMain);
}

void
TitleScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x87CEEB, true);//背景
	DrawBoxAA(m_blockX, m_blockY, m_blockX + kBlockSize, m_blockY + kBlockSize, 0xFF0000, true);//動くブロック
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_bgH, true);//背景ブロック
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 3, 6.0, 0.0, m_titleH, true, false);//タイトル

	//メニュー項目を描画
	SetFontSize(kMenuFontSize);
	DrawFormatString(SelectMenu[menuGameStart].x, SelectMenu[menuGameStart].y, 0xffffff, L"ゲームスタート");
	DrawFormatString(SelectMenu[menuOperation].x, SelectMenu[menuOperation].y, 0xffffff, L"操作説明");
	DrawFormatString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, 0xffffff, L"ゲーム終了");
	SetFontSize(0);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}