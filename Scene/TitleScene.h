#pragma once
#include "Secne.h"

enum Menu
{
	menuGameStart,		//ゲーム
	menuConfig,		//設定
	menuGameEnd,	//ゲーム終了

	menuNum			//項目の数
};

struct MenuElement
{
	int x, y;				//座標
	//TCHAR selectMenuName[100];	//項目名
};

class InputState;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//選択メニュー

	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& input);
public:
	//◇同時に継承元のコンストラクタが呼ばれる.Scene(SceneManager& manager)
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

