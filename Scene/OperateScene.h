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
/// ������@
/// </summary>
class OperateScene : public Scene
{
public:
	OperateScene(SceneManager& manager);
	virtual ~OperateScene();

	virtual void Update(const InputState& input) override;
	void Draw();
private:
	//�t�F�[�h�C���̎���Update�֐�
	void Page1Draw();
	//�ʏ��Ԃ̎���Update�֐�
	void Page2Draw();
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void Page3Draw();

	//Draw�p�����o�֐��|�C���^
	void (OperateScene::* m_updateFunc)();

	//�y�[�W����\������ʒu
	int m_pageX;
	int m_pageY;
	int m_pageNum = 0;

	//�����摜
	int m_operate = -1;
};

