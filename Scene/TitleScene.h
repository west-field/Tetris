#pragma once
#include "Secne.h"
#include "../game.h"

//���j���[����
enum MenuItem
{
	menuGameStart,	//�Q�[���X�^�[�g
	menuConfig,		//�ݒ�
	menuGameEnd,	//�Q�[���I��

	menuNum			//���ڂ̐�
};
//���j���[�v�f
struct MenuElement
{
	int x;
	int y;				//���W
};

namespace
{
	constexpr int kMenuFontSize = 50;//�����̃T�C�Y
	constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize;    //���j���[������x�ʒu
	constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize;    //���j���[������y�ʒu
	constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//���j���[�����̈ړ�����x�ʒu
}

class InputState;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//�I�����Ă��郁�j���[����
	int m_titleH = -1;
	int m_bgH = -1;

	//�u���b�N�̈ʒu
	float m_blockX = 0;
	float m_blockY = 0;

	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(const InputState& input);

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd}
	};

public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

