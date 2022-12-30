#include "TitleScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"

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
		//◇ChangeSceneで自分(TitleScene)がdeleteされるから何もしないreturn
		m_manager.ChangeScene(new GameplayingScene(m_manager));
		return;
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat)
{
	//m_titleH = my::MyLoadGraph(L"Data/img/title.png");
}

TitleScene::~TitleScene()
{
	//DeleteGraph(m_titleH);
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
	//普通の描画
	//DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2,1.0f,0.0f,m_titleH,true);
	DrawString(300, 200, L"TitleScene", 0xffffff);

	//◇最後にやる
	//今から書く画像と、すでに描画されているスクリーンとのブレンドの仕方をしている。
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	//◇上から消えるMULADrawGraph(0, 0, gradH_, true);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	//一度変更するとずっと残るのでNOBLEND,0に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}