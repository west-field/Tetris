#include "OperateScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "SceneManager.h"

OperateScene::OperateScene(SceneManager& manager) : Scene(manager)
{

}

OperateScene::~OperateScene()
{

}

void OperateScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{

	}
	if (input.IsTriggered(InputType::prev))
	{
		m_manager.PopScene();
		return;
	}
}

void OperateScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//�|�[�Y�E�B���h�E�Z���t�@��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�|�[�Y�����b�Z�[�W
	DrawString(pw_start_x + 10, pw_start_y + 10, L"Operate...", 0xffff88);
	//�|�[�Y�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void OperateScene::Page1Draw(const InputState& input)
{

}

void OperateScene::Page2Draw(const InputState& input)
{

}

void OperateScene::Page3Draw(const InputState& input)
{

}
