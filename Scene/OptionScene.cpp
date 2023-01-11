#include "OptionScene.h"
#include <DxLib.h>
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "KeyConfigScene.h"

OptionScene::OptionScene(SceneManager& manager) : Scene(manager), m_updateFunc(&OptionScene::FadeInUpdat)
{
}

OptionScene::~OptionScene()
{
}

void OptionScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}
void OptionScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//ウィンドウセロファン
	DrawBox(kconfigStartX, kconfigStartY, kconfigStartX + kconfigWidth, kconfigStartY + kconfigHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//枠線
	DrawBox(kconfigStartX, kconfigStartY, kconfigStartX + kconfigWidth, kconfigStartY + kconfigHeight, 0xffffff, false);
	//メッセージ
	SetFontSize(kFontSize);
	DrawString(kconfigStartX+10, kconfigStartY+10, L"Option", 0xffff88);
	SetFontSize(0);
	DrawString(m_configMenu[configKeyconfig].x, m_configMenu[configKeyconfig].y, L"KeyConfig", m_configMenu[configKeyconfig].color);
	DrawString(m_configMenu[configBack].x, m_configMenu[configBack].y, L"戻る", m_configMenu[configBack].color);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void OptionScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &OptionScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void OptionScene::NormalUpdat(const InputState& input)
{
	bool isPress = false;
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % configMax;
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (configMax - 1)) % configMax;
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < configMax; i++)
		{
			if (i == m_selectNum)
			{
				m_configMenu[i].x = kconfigStartX + 20;
				m_configMenu[i].color = 0xAAD8E6;
			}
			else
			{
				m_configMenu[i].x = kconfigStartX + 10;
				m_configMenu[i].color = 0xffffff;
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		switch (m_selectNum)
		{
		case configKeyconfig:
			m_manager.PushScene(new KeyConfigScene(m_manager, input));
			return;
		case configBack:
			m_updateFunc = &OptionScene::FadeOutUpdat;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		m_selectNum = configBack;
		m_updateFunc = &OptionScene::FadeOutUpdat;
	}
}

void OptionScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}
