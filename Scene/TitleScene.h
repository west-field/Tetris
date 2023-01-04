#pragma once
#include "Secne.h"

enum Menu
{
	menuGameStart,		//�Q�[��
	menuConfig,		//�ݒ�
	menuGameEnd,	//�Q�[���I��

	menuNum			//���ڂ̐�
};

struct MenuElement
{
	int x, y;				//���W
	//TCHAR selectMenuName[100];	//���ږ�
};

class InputState;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//�I�����j���[

	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(const InputState& input);
public:
	//�������Ɍp�����̃R���X�g���N�^���Ă΂��.Scene(SceneManager& manager)
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

