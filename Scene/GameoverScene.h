#pragma once
#include "Secne.h"

/// <summary>
/// �Q�[���I�[�o�[�V�[��
/// </summary>
class GameoverScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    using UpdateFunc_t = void (GameoverScene::*)(const InputState&);
    UpdateFunc_t m_updateFunc;
public:
    GameoverScene(SceneManager& manager);
    virtual ~GameoverScene();

    void Update(const InputState& input);
    void Draw();
};

