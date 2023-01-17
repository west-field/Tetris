#include "OperateScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

OperateScene::OperateScene(SceneManager& manager) : Scene(manager), m_updateFunc(&OperateScene::Page1Draw)
{
	m_pageX = Game::kScreenWidth - 100;
	m_pageY = Game::kScreenHeight - 30;
	m_operate = my::MyLoadGraph(L"Data/operate.png");
}

OperateScene::~OperateScene()
{
	DeleteGraph(m_operate);
}

void OperateScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		m_pageNum++;

		switch (m_pageNum)
		{
		case Page01:
			m_updateFunc = &OperateScene::Page1Draw;
			break;
		case Page02:
			m_updateFunc = &OperateScene::Page2Draw;
			break;
		case Page03:
			m_updateFunc = &OperateScene::Page3Draw;
			break;
		default:
			m_manager.ChangeScene(new TitleScene(m_manager));
			return;
		}
	}
}

void OperateScene::Draw()
{
	(this->*m_updateFunc)();
}

void OperateScene::Page1Draw()
{
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_operate, true);
	DrawString(m_pageX, m_pageY, L"Page01", 0xffff88);
}

void OperateScene::Page2Draw()
{
	DrawString(0, 0, L"テトリミノの出現順", 0xffff88);
	DrawString(m_pageX, m_pageY, L"Page02", 0xffff88);
}

void OperateScene::Page3Draw()
{
	DrawString(0,0, L"ホールド", 0xffff88);
	DrawString(m_pageX, m_pageY, L"Page03", 0xffff88);
}
