#include "PauseScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"

PauseScene::PauseScene(SceneManager& manager) : Scene(manager)
{

}

PauseScene::~PauseScene()
{

}

void PauseScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::pause))
	{
		m_manager.PopScene();
		return;
	}
	if (input.IsTriggered(InputType::keyconf))
	{
		m_manager.PushScene(new KeyConfigScene(m_manager,input));
		return;
	}
}

void PauseScene::Draw()
{
	constexpr int pw_width = 400;
	constexpr int pw_height = 300;
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//�|�[�Y�E�B���h�E�Z���t�@��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�|�[�Y�����b�Z�[�W
	DrawString(pw_start_x + 10, pw_start_y + 10, L"Pausing...", 0xffff88);
	//�|�[�Y�E�B���h�E�g��
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
