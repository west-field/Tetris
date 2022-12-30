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
	constexpr int kPieceHeight = 4;
	constexpr int kPieceWidth = 4;
	constexpr int kPieceSize = 30;

	//横移動時間
	constexpr float kMoveWaitTime = 10.0f;
	//回転移動時間
	constexpr float kRollWaitTime = 10.0f;
	//回転移動時間
	constexpr float kDeleteWaitTime = 10.0f;
	//落ちる時間
	constexpr float kDropWaitTime = 40.0f;
	constexpr float kSlowlyDropWaitTime = 0.5f;
}

//テトリミノの種類
enum TetoriminoType
{
	TypeI,TypeO,
	TypeS,TypeZ,
	TypeJ,TypeL,
	TypeT,TypeMax
};
//テトリミノを回転させる角度
enum TetoriminoAngle
{
	Angle0,Angle90,
	Angle180,Angle270,
	AngleMax
};

class Piece
{
public:
	Piece();
	virtual ~Piece();
	void Update(const InputState& input);
	void Darw();
private:
	void NomalUpdate(const InputState& input);
	void DeleteLineUpdate(const InputState& input);
	//テトリミノをフィールドに置く
	void PieceToField();
	//テトリミノが一列そろっているか確認する
	void DeleteLine();
	//テトリミノを一列消した後列を下に落とす
	void DropLine();
	//テトリミノを表示する位置を決める
	void display();
	//テトリミノがあったっているかどうか
	bool isHit(int X, int Y, int type, int angle);
	//テトリミノをリセットする
	void resetMino();

	//Update用メンバ関数ポインタ
	using UpdateFunc_t = void(Piece::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;

	//フィールド
	int m_field[kFieldHeightMax][kFieldWidthMax] = {};
	//テトリミノを移動させるフィールド
	int m_minoMove[kFieldHeightMax][kFieldWidthMax] = {};
	//テトリミノを消せるライン
	bool m_deleteLine[kFieldHeightMax] = {};

	int randNum = 0;
	//フィールドを表示する位置
	int m_startPosX = 0;
	int m_startPosY = 0;

	//テトリミノの種類
	int m_minoType = 0;
	//テトリミノの角度
	int m_minoAngle = 0;
	int m_angle = AngleMax * 10;
	//テトリミノの移動位置
	int m_minoX = 0;
	int m_minoY = 0;

	//次のテトリミノの種類
	int m_nextMinoType = 0;
	//次のテトリミノを表示する位置
	int m_nextMinoX = 0;
	int m_nextMinoY = 0;

	//横移動時間
	float m_moveTime = kMoveWaitTime;
	//回転移動時間
	float m_rollTime = kRollWaitTime;
	//消す時間
	float m_deleteTime = kDeleteWaitTime;
	//落ちる時間
	float m_dropTime = kDropWaitTime;
	//下を押したときの移動スピード
	float m_dropSpeed = 8.0f;
	//消せるラインがあるかどうか
	bool m_isDeleteLine = false;

	//テトリミノ
	int m_mino[TypeMax][AngleMax][kPieceHeight][kPieceWidth] =
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
				{1,1,1,1},
				{0,0,0,0},
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

