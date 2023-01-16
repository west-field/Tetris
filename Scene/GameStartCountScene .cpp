#include "GameStartCountScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Sound.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../Game/Field.h"


GameStartCountScene::GameStartCountScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameStartCountScene::CountDownUpdat)
{
	Sound::Play(Sound::Count);
}

GameStartCountScene::~GameStartCountScene()
{
}

void GameStartCountScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}
void GameStartCountScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, false);

	if (m_isTuru)
	{
		DrawString(kStartMenuPosX, kStartMenuPosY - kLevelFontSize, L"�Q�[�����x���ݒ�", 0xffffff);
		DrawFormatString(kStartMenuPosX + 200, kStartMenuPosY - kLevelFontSize, 0xffffff, L"Level %d", 1 + m_selectLevel * 4);
		//���j���[���ڂ�`��
		SetFontSize(kLevelFontSize);
		DrawFormatString(m_SelectLevel[Easy].x, m_SelectLevel[Easy].y, 0xffffff, L"Easy");
		DrawFormatString(m_SelectLevel[Normal].x, m_SelectLevel[Normal].y, 0xffffff, L"Normal");
		DrawFormatString(m_SelectLevel[Hard].x, m_SelectLevel[Hard].y, 0xffffff, L"Hard");
		SetFontSize(0);
	}
	else
	{
		int count = static_cast<int>(m_count) / 60;
		SetFontSize(kLevelFontSize);
		if (count > 0)
		{
			DrawFormatString(Game::kScreenWidth / 2, Game::kScreenHeight / 3, 0xffffff, L"%d", count);
		}
		else
		{
			DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 3, L"�X�^�[�g", 0xffffff);
		}
		SetFontSize(0);
	}
}

void GameStartCountScene::NormalUpdat(const InputState& input)
{
	bool isPress = false;//�L�[�������ꂽ���ǂ���
	if (input.IsTriggered(InputType::down))
	{
		m_selectLevel = (m_selectLevel + 1) % Max;//�I����Ԃ��������
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectLevel = (m_selectLevel + (Max - 1)) % Max;//�I����Ԃ���グ��
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < Max; i++)
		{
			if (i == m_selectLevel)
			{
				m_SelectLevel[i].x = kMovePosX;//�ړ��ʒu�Ɉړ�����
			}
			else
			{
				m_SelectLevel[i].x = kStartMenuPosX;//���̈ʒu�ɖ߂�
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		m_isTuru = false;
		m_updateFunc = &GameStartCountScene::CountDownUpdat;
	}
}

void GameStartCountScene::CountDownUpdat(const InputState& input)
{
	if (--m_count <= 0)
	{
		SetLevel(m_selectLevel * 4);

		m_manager.PopScene();
		return;
	}
}
