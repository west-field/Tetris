#include "GameplayingScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "GameStartCountScene.h"
#include "GameoverScene.h"

void GameplayingScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameplayingScene::NormalUpdat;
		m_fadeValue = 0;
		m_manager.PushScene(new GameStartCountScene(m_manager));
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input)
{
	m_field.Update(input);

	if (m_field.GetChangeLevel() >= 5)
	{
		m_level++;
		m_field.SetFallTime(m_level * 3.0f);
	}

	int line = m_field.GetLineNum();
	m_point += (line * line * m_level * 100);

	if (m_field.GameOver())
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
	}

	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
	}
	if (input.IsTriggered(InputType::prev))
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
	if (input.IsTriggered(InputType::pause))
	{
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if(++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameoverScene(m_manager));
		return;
	}
}

GameplayingScene::GameplayingScene(SceneManager& manager) : Scene(manager),m_updateFunc(&GameplayingScene::FadeInUpdat) 
{
	
}

GameplayingScene::~GameplayingScene()
{
	
}

void
GameplayingScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void
GameplayingScene::Draw()
{
	m_field.Darw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//スコア表示
	DrawFormatString((Game::kScreenWidth - kTetriminoSize * kFieldWidthMax) / 2 + kTetriminoSize * kFieldWidthMax + kTetriminoSize * 3,
		(Game::kScreenHeight - kTetriminoSize * kFieldHeightMax) / 2 + kTetriminoSize * 8, 0xffffff, L"SCORE %d", m_point);
	//レベル表示
	DrawFormatString(0, 0, 0xffffff, L"Level %d", m_level);

}