#pragma once
#include "Secne.h"
#include "../game.h"

namespace
{
	constexpr int pw_width = 400;
	constexpr int pw_height = 300;
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	enum Page
	{
		Page01,
		Page02,
		Page03,

		PageMax
	};
}

class InputState;
class SceneManager;
/// <summary>
/// 操作方法
/// </summary>
class OperateScene : public Scene
{
public:
	OperateScene(SceneManager& manager);
	virtual ~OperateScene();

	virtual void Update(const InputState& input) override;
	void Draw();
private:
	//フェードインの時のUpdate関数
	void Page1Draw();
	//通常状態の時のUpdate関数
	void Page2Draw();
	//フェードアウトの時のUpdate関数
	void Page3Draw();

	//Draw用メンバ関数ポインタ
	void (OperateScene::* m_updateFunc)();

	//ページ数を表示する位置
	int m_pageX;
	int m_pageY;
	int m_pageNum = 0;

	//説明画像
	int m_operate = -1;
};

