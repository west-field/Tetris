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
	constexpr int kOriginalPosX = Game::kScreenWidth / 3;    //���j���[�����̌���x�ʒu
	constexpr int kOriginalPosY = Game::kScreenHeight / 2;    //���j���[�����̌���y�ʒu
	constexpr int kTitleFontSize = 70;//������̕�
	constexpr int kFontSize = 60;//������̕�
	constexpr int kMovedPosX = kOriginalPosX - kFontSize;//���j���[�����̈ړ�����x�ʒu

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kFontSize * menuGameEnd}
	};
}

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
	if (input.IsTriggered(InputType::down))
	{//���L�[��������Ă�����
		m_selectNum = (m_selectNum + 1) % menuNum;//�I����Ԃ��������
	}
	else if (input.IsTriggered(InputType::up))
	{//��L�[��������Ă�����
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//�I����Ԃ���グ��
	}

	if (input.IsTriggered(InputType::down) || input.IsTriggered(InputType::up))
	{
		// ���j���[���ڐ��ł���4���[�v����
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//�ړ��ʒu�Ɉړ�����
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//���̈ʒu�ɖ߂�
			}
		}
	}
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
		//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
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
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	//���j���[���ڂ�`��
	SetFontSize(kTitleFontSize);
	DrawFormatString(kOriginalPosX, Game::kScreenHeight / 3 - kTitleFontSize, 0xffffff, L"�e�g���X");
	SetFontSize(kFontSize);
	DrawFormatString(SelectMenu[menuGameStart].x, SelectMenu[menuGameStart].y, 0xffffff, L"�Q�[���X�^�[�g");
	DrawFormatString(SelectMenu[menuConfig].x, SelectMenu[menuConfig].y, 0xffffff, L"�ݒ�");
	DrawFormatString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, 0xffffff, L"�Q�[���I��");
	SetFontSize(0);

	//�����珑���摜�ƁA���łɕ`�悳��Ă���X�N���[���Ƃ̃u�����h�̎d�������Ă���B
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	//���ォ�������MULADrawGraph(0, 0, gradH_, true);
	//��ʑS�̂�^�����ɓh��Ԃ�
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	//��x�ύX����Ƃ����Ǝc��̂�NOBLEND,0�ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}