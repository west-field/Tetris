#define SPASE -1
#define WALL 8

#include "Field.h"
#include <DxLib.h>
#include <stdlib.h>
#include <time.h>
#include "../game.h"
#include "../Sound.h"
#include "../InputState.h"

Field::Field():m_updateFunc(&Field::NomalUpdate)
{
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		for (int x = 0; x < kFieldWidthMax; x++)
		{
			m_field[y][x] = SPASE;
		}
	}
	for (int x = 0; x < kFieldWidthMax; x++)
	{
		//床
		m_field[kFieldHeightMax - 1][x] = WALL;
	}
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		//壁
		m_field[y][0] = WALL;
		m_field[y][kFieldWidthMax - 1] = WALL;
	}

	m_startPosX = (Game::kScreenWidth - kTetriminoSize * kFieldWidthMax) / 2;
	m_startPosY = (Game::kScreenHeight - kTetriminoSize * kFieldHeightMax) / 2;

	int randNum = rand() % 100;
	m_nextTetriminoType = randNum % TypeMax;
	m_nextTetriminoX = m_startPosX + kTetriminoSize * kFieldWidthMax + kTetriminoSize * 3;
	m_nextTetriminoY = m_startPosY + kTetriminoSize * 3;

	ResetTetrimino();
}

Field::~Field()
{

}

void Field::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void Field::Darw()
{
	//フィールドを表示
	{
		for (int y = 0; y < kFieldHeightMax; y++)
		{
			for (int x = 0; x < kFieldWidthMax; x++)
			{
				int X = m_startPosX + x * kTetriminoSize;
				int Y = m_startPosY + y * kTetriminoSize;

				//枠を表示
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[8], false);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				//フィールドのテトリミノを表示
				if (m_field[y][x] != SPASE)
				{
					DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[m_field[y][x]], true);
				}
				//テトリミノを表示
				if (!m_isDeleteLine)
				{
					if (m_tetriminoMove[y][x] != 0)
					{
						DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[m_tetriminoType], true);
					}
				}
			}
		}
	}

	//次のテトリミノを表示
	{
		for (int y = 0; y < kTetriminoHeight; y++)
		{
			for (int x = 0; x < kTetriminoWidth; x++)
			{
				int X = m_nextTetriminoX + x * kTetriminoSize;
				int Y = m_nextTetriminoY + y * kTetriminoSize;

				if (m_tetrimino[m_nextTetriminoType][0][y][x])
				{
					DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[m_nextTetriminoType], true);
				}
			}
		}

		DrawString(m_nextTetriminoX, m_nextTetriminoY - 30, L"next", 0xffffff);
	}

}

void Field::NomalUpdate(const InputState& input)
{
	if (--m_moveTime <= 0)
	{
		//左に移動
		if (input.IsPressed(InputType::left))
		{
			if (!TetriminoIsHit(m_tetriminoX - 1, m_tetriminoY, m_tetriminoType, m_tetriminoAngle))
			{
				m_tetriminoX--;
			}
			m_moveTime = kMoveTime;
			Sound::Play(Sound::BlockMove);
		}
		//右に移動
		else if (input.IsPressed(InputType::right))
		{
			if (!TetriminoIsHit(m_tetriminoX + 1, m_tetriminoY, m_tetriminoType, m_tetriminoAngle))
			{
				m_tetriminoX++;
			}
			m_moveTime = kMoveTime;
			Sound::Play(Sound::BlockMove);
		}
	}
	if (--m_rollTime <= 0)
	{
		//左に回転
		if (input.IsPressed(InputType::leftroll))
		{
			m_angle--;
			m_rollTime = kRollTime;
			if (!TetriminoIsField())m_angle++;
			Sound::Play(Sound::BlockMove);
		}
		//右に回転
		else if (input.IsPressed(InputType::rightroll))
		{
			m_angle++;
			m_rollTime = kRollTime;
			if(!TetriminoIsField())m_angle--;
			Sound::Play(Sound::BlockMove);
		}
	}

	//落ちるスピードを速くする
	if (input.IsPressed(InputType::down))
	{
		m_FallTime -= kFallSpeed;
	}
	//落ちるスピードを遅くする
	else if (input.IsPressed(InputType::up))
	{
		m_FallTime += kSlowlyFallTime;
	}
	TetriminoPos();
	//落ちる時間を制御
	if (--m_FallTime <= 0)
	{
		Sound::Play(Sound::BlockMove);
		if (!TetriminoIsHit(m_tetriminoX, m_tetriminoY + 1, m_tetriminoType, m_tetriminoAngle))
		{
			m_tetriminoY++;
		}
		else
		{
			TetriminoToField();
			DeleteLine();
			ResetTetrimino();
		}
		m_FallTime = kFallTime - m_FallTimeLevel;
	}
}

void Field::DeleteLineUpdate(const InputState& input)
{
	if (--m_deleteTime <= 0)
	{
		m_lineNum = 0;//何列消したのか数える
		for (auto& line : m_deleteLine)
		{
			if (line)
			{
				m_lineNum++;
			}
			line = false;//初期化
		}
		//消した分だけ下に落とす
		for (int i = 0; i < m_lineNum; i++)
		{
			DropLine();
		}
		Sound::Play(Sound::BlockPlacing);
		m_levelChange += m_lineNum;
		
		//初期化
		m_deleteTime = kDeleteTime;
		m_isDeleteLine = false;
		m_updateFunc = &Field::NomalUpdate;
	}
}

void Field::TetriminoToField()
{
	for (int y = kFieldHeightMax - 1; y > 0 ; y--)
	{
		for (int x = kFieldWidthMax - 1; x > 0; x--)
		{
			if (m_tetriminoMove[y][x] != 0)
			{
				m_field[y][x] = m_tetriminoType;
				m_tetriminoMove[y][x] = 0;
			}
		}
	}
	Sound::Play(Sound::BlockPlacing);
}

void Field::DeleteLine()
{
	//一列消せる場所があるか
	for (int y = kFieldSpase; y <= kFieldHeight + kFieldFloor; y++)
	{
		for (int x = kFieldWall; x <= kFieldWidth; x++)
		{
			if (m_field[y][x] == SPASE)
			{
				m_deleteLine[y] = false;
				break;
			}
			m_deleteLine[y] = true;
		}
	}

	//テトリミノを消す
	//0～22の内の2～21
	for (int y = kFieldSpase; y <= kFieldHeight + kFieldFloor; y++)
	{
		if (m_deleteLine[y] == true)
		{
			//0～11の内の1～10
			for (int x = kFieldWall; x <= kFieldWidth; x++)
			{
				m_field[y][x] = SPASE;
			}
			m_isDeleteLine = true;
		}
	}
	
	if (m_isDeleteLine)
	{
		Sound::Play(Sound::Blocking);
		m_updateFunc = &Field::DeleteLineUpdate;
	}
}

void Field::DropLine()
{
	//0～22の内の2～21
	for (int y = kFieldHeight + kFieldFloor; y >= kFieldSpase; y--)
	{
		//一列空白かどうかを確認
		int pieceCount = 0;
		//0～11の内の1～10
		for (int x = kFieldWidth; x >= kFieldWall; x--)
		{
			if (m_field[y][x] == SPASE)
			{
				pieceCount++;
			}
		}

		//一列空白だったら
		if (pieceCount >= kFieldWidth)
		{
			for (int x = kFieldWidth; x >= kFieldWall; x--)
			{
				//上にあるピースを下に落とす
				if (m_field[y - 1][x] != SPASE)
				{
					m_field[y][x] = m_field[y - 1][x];
					m_field[y - 1][x] = SPASE;
				}
			}
		}
	}
}

void Field::TetriminoPos()
{
	{
		//表示するテトリミノをリセット
		for (int y = 0; y < kFieldHeightMax; y++)
		{
			for (int x = 0; x < kFieldWidthMax; x++)
			{
				m_tetriminoMove[y][x] = 0;
			}
		}

		//テトリミノの位置を決める
		for (int x = 0; x < kTetriminoWidth; x++)
		{
			for (int y = 0; y < kTetriminoHeight; y++)
			{
				if (m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x] != 0)
				{
					m_tetriminoMove[m_tetriminoY + y][m_tetriminoX + x] = m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x];
				}
			}
		}
	}
}

bool Field::TetriminoIsHit(int X, int Y, int type, int angle)
{
	for (int y = 0; y < kTetriminoHeight; y++)
	{
		for (int x = 0; x < kTetriminoWidth; x++)
		{
			if (m_tetrimino[type][angle][y][x] == 1 && m_field[Y + y][X + x] != SPASE)
			{
				return true;
			}
		}
	}
	return false;
}

bool Field::TetriminoIsField()
{
	int angle = m_angle % AngleMax;
	int num = 0;
	bool isok = false;
	int copy[kFieldHeightMax][kFieldWidthMax] = {};

	//テトリミノを回転させたブロックを作る
	for (int x = 0; x < kTetriminoWidth; x++)
	{
		for (int y = 0; y < kTetriminoHeight; y++)
		{
			if (m_tetrimino[m_tetriminoType][angle][y][x] != 0)
			{
				copy[m_tetriminoY + y][m_tetriminoX + x] = m_tetrimino[m_tetriminoType][angle][y][x];
			}
		}
	}

	for (int y = 0; y < kFieldHeightMax; y++)
	{
		for (int x = 0; x < kFieldWidthMax; x++)
		{
			if (copy[y][x] == 1)
			{
				if (m_field[y][x] == SPASE)
				{
					num++;
				}
			}
		}
	}

	////ブロックを表示
	//{
	//	for (int y = 0; y < kFieldHeightMax; y++)
	//	{
	//		for (int x = 0; x < kFieldWidthMax; x++)
	//		{
	//			int X = m_startPosX + x * kTetriminoSize;
	//			int Y = m_startPosY + y * kTetriminoSize;
	//			//フィールドのテトリミノを表示
	//			if (copy[y][x] != 0)
	//			{
	//				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//				DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[copy[y][x]], true);
	//				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//			}
	//		}
	//	}
	//}

	if (num == 4)
	{
		isok = true;
	}

	if (isok)
	{
		m_tetriminoAngle = angle;
		//表示するテトリミノをリセット
		for (int y = 0; y < kFieldHeightMax; y++)
		{
			for (int x = 0; x < kFieldWidthMax; x++)
			{
				m_tetriminoMove[y][x] = 0;
			}
		}

		//テトリミノの位置を決める
		for (int x = 0; x < kTetriminoWidth; x++)
		{
			for (int y = 0; y < kTetriminoHeight; y++)
			{
				if (m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x] != 0)
				{
					m_tetriminoMove[m_tetriminoY + y][m_tetriminoX + x] = m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x];
				}
			}
		}
		return true;
	}
	return false;
}

void Field::ResetTetrimino()
{
	//初期位置
	m_tetriminoX = 5;
	m_tetriminoY = 0;

	//次に決めていたテトリミノをm_minoTypeに入れる
	m_tetriminoType = m_nextTetriminoType;
	
	//次のテトリミノを決める
	srand((unsigned int)time(NULL));
	int randNum = rand() % 100;
	m_nextTetriminoType = randNum % TypeMax;
	
	//表示する角度
	m_tetriminoAngle = 0;
	m_angle = AngleMax * 10;
}

bool Field::GameOver()
{
	for (int x = kFieldWall; x < kFieldWidth; x++)
	{
		if (m_field[1][x] != SPASE)
		{
			return true;
		}
	}

	return false;
}
