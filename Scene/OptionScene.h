#pragma once
#include "Secne.h"

/// <summary>
/// �I�v�V�����V�[��
/// </summary>
class OptionScene : public Scene
{
public:
	//�������Ɍp�����̃R���X�g���N�^���Ă΂��.Scene(SceneManager& manager)
	OptionScene(SceneManager& manager) : Scene(manager) {};
	~OptionScene() {};

	void Update(const InputState& input);
	void Draw();
};

