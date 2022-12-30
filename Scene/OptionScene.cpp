#include "OptionScene.h"
#include <DxLib.h>
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"

void OptionScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}
void OptionScene::Draw()
{
	DrawString(300,200,L"OptionScene", 0xffffff);
}