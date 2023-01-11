#pragma once
#include "Secne.h"

class InputState;

enum Level
{
	Easy,
	Normal,
	Hard,

	Max
};

struct LevelElement
{
	int x;
	int y;
};

namespace
{
	constexpr int kLevelFontSize = 50;
	constexpr int kPosX = 200;
	constexpr int kMovePosX = kPosX - kLevelFontSize;
	constexpr int kPosY = 150;
}

/// <summary>
/// �Q�[���J�n�V�[��
/// </summary>
class GameStartCountScene : public Scene
{
public:
	GameStartCountScene(SceneManager& manager);
	~GameStartCountScene();

	virtual void Update(const InputState& input);
	void Draw();
private:
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�J�E���g�_�E���̎���Update�֐�
	void CountDownUpdat(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (GameStartCountScene::* m_updateFunc)(const InputState& input);

	//�I�����Ă��郌�x��
	int m_selectLevel = 0;

	LevelElement m_SelectLevel[Max] = {
		{kMovePosX,kPosY},
		{kPosX,kPosY + Normal * kLevelFontSize},
		{kPosX,kPosY + Hard * kLevelFontSize}
	};

	float m_count = 240.0f;

	bool m_isTuru = false;
};
