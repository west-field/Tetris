#pragma once

class InputState;

namespace
{
	//フィールドのテトリミノを動かせる範囲
	constexpr int kFieldHeight = 20;
	constexpr int kFieldWidth = 10;
	//壁
	constexpr int kFieldWall = 1;
	//床
	constexpr int kFieldFloor = 1;
	//上の空間
	constexpr int kFieldSpase = 2;

	//フィールドの大きさ
	constexpr int kFieldHeightMax = kFieldHeight + kFieldFloor + kFieldSpase;
	constexpr int kFieldWidthMax = kFieldWidth + kFieldWall * 2;

	//テトリミノの大きさ
	constexpr int kTetriminoHeight = 4;
	constexpr int kTetriminoWidth = 4;
	constexpr int kTetriminoSize = 30;

	//

	//横移動時間
	constexpr float kMoveTime = 8.0f;
	//回転移動時間
	constexpr float kRollTime = 9.5f;
	//回転移動時間
	constexpr float kDeleteTime = 20.0f;
	//落ちる時間
	constexpr float kFallTime = 60.0f;
	//下を押したときの移動スピード
	constexpr float kFallSpeed = 8.0f;
	//上を押したとき落ちる時間が遅くなる
	constexpr float kSlowlyFallTime = 0.5f;
}

//テトリミノの種類
enum TetriminoType
{
	TypeI,TypeO,
	TypeS,TypeZ,
	TypeJ,TypeL,
	TypeT,TypeMax
};
//テトリミノを回転させる角度
enum TetriminoAngle
{
	Angle0,Angle90,
	Angle180,Angle270,
	AngleMax
};

class Field
{
public:
	Field();
	virtual ~Field();
	void Update(const InputState& input);
	void Darw();
	//ゲームオーバー判定
	bool GameOver();
private:
	void NomalUpdate(const InputState& input);
	void DeleteLineUpdate(const InputState& input);
	//テトリミノをフィールドに置く
	void TetriminoToField();
	//テトリミノが一列そろっているか確認する
	void DeleteLine();
	//テトリミノを一列消した後列を下に落とす
	void DropLine();
	//テトリミノを表示する位置を決める
	void TetriminoPos();
	//テトリミノがあったっているかどうか
	bool TetriminoIsHit(int X, int Y, int type, int angle);
	//テトリミノが回転したときにフィールド内にあるかどうか
	void TetriminoIsField();
	//テトリミノをリセットする
	void ResetTetrimino();
	
	//Update用メンバ関数ポインタ
	using UpdateFunc_t = void(Field::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;

	//フィールド
	int m_field[kFieldHeightMax][kFieldWidthMax] = {};
	//テトリミノを移動させるフィールド
	int m_tetriminoMove[kFieldHeightMax][kFieldWidthMax] = {};
	//テトリミノを消せるライン
	bool m_deleteLine[kFieldHeightMax] = {};

	//フィールドを表示する位置
	int m_startPosX = 0;
	int m_startPosY = 0;

	//テトリミノの種類
	int m_tetriminoType = 0;
	//テトリミノの角度
	int m_tetriminoAngle = 0;
	int m_angle = AngleMax * 10;
	//テトリミノの移動位置
	int m_tetriminoX = 0;
	int m_tetriminoY = 0;

	//次のテトリミノの種類
	int m_nextTetriminoType = 0;
	//次のテトリミノを表示する位置
	int m_nextTetriminoX = 0;
	int m_nextTetriminoY = 0;

	//横移動時間
	float m_moveTime = kMoveTime;
	//回転移動時間
	float m_rollTime = kRollTime;
	//消す時間
	float m_deleteTime = kDeleteTime;
	//落ちる時間
	float m_FallTime = kFallTime;
	//消せるラインがあるかどうか
	bool m_isDeleteLine = false;

	//テトリミノ
	int m_tetrimino[TypeMax][AngleMax][kTetriminoHeight][kTetriminoWidth] =
	{
		//TypeI
		{
			//Angle0
			{
				{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,1,0,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,1,1},
				{0,0,0,0}
			},
			//Angle270
			{
				{0,0,1,0},
				{0,0,1,0},
				{0,0,1,0},
				{0,0,1,0}
			}
		},
		//TypeO
		{
			//Angle0
			{
				{0,0,0,0},
				{0,1,1,0},
				{0,1,1,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,1,1,0},
				{0,1,1,0},
				{0,0,0,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,1,1,0},
				{0,1,1,0},
				{0,0,0,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{0,1,1,0},
				{0,1,1,0},
				{0,0,0,0}
			}
		},
		//TypeS
		{
			//Angle0
			{
				{0,0,0,0},
				{0,1,1,0},
				{1,1,0,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,1,0,0},
				{0,1,1,0},
				{0,0,1,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{0,1,1,0},
				{1,1,0,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{1,0,0,0},
				{1,1,0,0},
				{0,1,0,0}
			}
		},
		//TypeZ
		{
			//Angle0
			{
				{0,0,0,0},
				{1,1,0,0},
				{0,1,1,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,0,1,0},
				{0,1,1,0},
				{0,1,0,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,0,0},
				{0,1,1,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{0,1,0,0},
				{1,1,0,0},
				{1,0,0,0}
			}
		},
		//TypeJ
		{
			//Angle0
			{
				{0,0,0,0},
				{1,0,0,0},
				{1,1,1,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,1,1,0},
				{0,1,0,0},
				{0,1,0,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,1,0},
				{0,0,1,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{1,1,0,0}
			}
		},
		//TypeL
		{
			//Angle0
			{
				{0,0,0,0},
				{0,0,1,0},
				{1,1,1,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,1,0,0},
				{0,1,0,0},
				{0,1,1,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,1,0},
				{1,0,0,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{1,1,0,0},
				{0,1,0,0},
				{0,1,0,0}
			}
		},
		//TypeT
		{
			//Angle0
			{
				{0,0,0,0},
				{0,1,0,0},
				{1,1,1,0},
				{0,0,0,0}
			},
			//Angle90
			{
				{0,0,0,0},
				{0,1,0,0},
				{0,1,1,0},
				{0,1,0,0}
			},
			//Angle180
			{
				{0,0,0,0},
				{0,0,0,0},
				{1,1,1,0},
				{0,1,0,0}
			},
			//Angle270
			{
				{0,0,0,0},
				{0,1,0,0},
				{1,1,0,0},
				{0,1,0,0}
			}
		}
	};

	//テトリミノの色＋空白の色＋白
	int m_color[TypeMax + 2] =
	{
		0xAAD8E6,//水色
		0xFFFF00,//黄色
		0x008000,//緑
		0xFF0000,//赤
		0x0000FF,//青
		0xFFA500,//オレンジ
		0xFF00FF,//紫

		0x87CEEB,//空色
		0xffffff//白
	};
};

