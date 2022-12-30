#pragma once
#include "Secne.h"

class InputState;
class SceneManager;

class KeyConfigScene : public Scene
{
private:
    const InputState& m_inputState;
    int m_currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
    bool m_isEditing = false;//���ݓ���̂ɂイ��傭�͕ҏW���ł��B
public:
    KeyConfigScene(SceneManager& manager,const InputState& input);
    virtual ~KeyConfigScene();
    void Update(const InputState& input);
    void Draw();
};

