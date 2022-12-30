#pragma once
#include "Secne.h"

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameoverScene : public Scene
{
private:
    //int m_gameoverH = -1;
    
    unsigned int m_fadeColor = 0xff0000;//フェードの色（赤

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

