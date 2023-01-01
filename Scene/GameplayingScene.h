#pragma once
#include "Secne.h"
#include "../Game/Field.h"

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    //UpdateFuncの型
    using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
    UpdateFunc_t m_updateFunc = nullptr;
public:
    //◇アンパサンドを必ずつける
    GameplayingScene(SceneManager& manager);
    virtual ~GameplayingScene();

    void Update(const InputState& input);
    void Draw();

    Field m_field;
};

