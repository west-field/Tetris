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
	//���ǂ�ǂ񖾂邭�Ȃ�
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input)
{
	//�u���ցv�{�^���������ꂽ�玟�V�[���ֈڍs����
	if (input.IsTriggered(InputType::next))
	{
		//�����ꂽ��NormalUpdat����FadeOutUpdat�ɕύX����
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
		//��ChangeScene�Ŏ���(TitleScene)��delete����邩�牽�����Ȃ�return
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
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	//���ʂ̕`��
	//DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2,1.0f,0.0f,m_titleH,true);
	DrawString(300, 200, L"TitleScene", 0xffffff);

	//���Ō�ɂ��
	//�����珑���摜�ƁA���łɕ`�悳��Ă���X�N���[���Ƃ̃u�����h�̎d�������Ă���B
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	//���ォ�������MULADrawGraph(0, 0, gradH_, true);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	//��x�ύX����Ƃ����Ǝc��̂�NOBLEND,0�ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}