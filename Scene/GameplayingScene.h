#pragma once
#include "Secne.h"
#include "../Game/Field.h"

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    //UpdateFunc�̌^
    using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
    UpdateFunc_t m_updateFunc = nullptr;
public:
    //���A���p�T���h��K������
    GameplayingScene(SceneManager& manager);
    virtual ~GameplayingScene();

    void Update(const InputState& input);
    void Draw();

    Field m_field;
};

