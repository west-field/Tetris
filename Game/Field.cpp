#define SPASE -1
#define WALL 7

#include "Field.h"
#include <DxLib.h>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include "../game.h"
#include "../Sound.h"
#include "../InputState.h"
#include "../DrawFunctions.h"

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
		//��
		m_field[kFieldHeightMax - 1][x] = WALL;
	}
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		//��
		m_field[y][0] = WALL;
		m_field[y][kFieldWidthMax - 1] = WALL;
	}

	m_startPosX = (Game::kScreenWidth - kTetriminoSize * kFieldWidthMax) / 2;
	m_startPosY = (Game::kScreenHeight - kTetriminoSize * kFieldHeightMax) / 2;

	int randNum = rand() % 100;
	m_nextTetriminoType = randNum % TypeMax;
	m_nextTetriminoX = m_startPosX + kTetriminoSize * kFieldWidthMax + kTetriminoSize * 3;
	m_nextTetriminoY = m_startPosY + kTetriminoSize * 3;

	m_holdTetriminoType = 0;
	m_holdTetriminoX = Game::kScreenWidth / 5;
	m_holdTetriminoY = Game::kScreenHeight / 6;

	ResetTetrimino();

	LoadDivGraph(L"Data/tetrimino.bmp", TypeMax+1, TypeMax+1, 1, kTetriminoHSize, kTetriminoHSize, m_tetriminoH);
	for (auto& minoH : m_tetriminoH)
	{
		assert(minoH >= 0);
	}
}

Field::~Field()
{
	for(auto& minoH : m_tetriminoH)
	{
		DeleteGraph(minoH);
	}
	
}

void Field::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void Field::Darw()
{
	//�t�B�[���h��\��
	{
		for (int y = 0; y < kFieldHeightMax; y++)
		{
			for (int x = 0; x < kFieldWidthMax; x++)
			{
				int X = m_startPosX + x * kTetriminoSize;
				int Y = m_startPosY + y * kTetriminoSize;

				//�g��\��
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[8], false);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				//�t�B�[���h�̃e�g���~�m��\��
				if (m_field[y][x] != SPASE)
				{
					DrawExtendGraph(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_tetriminoH[m_field[y][x]], true);
				}
				//�e�g���~�m��\��
				if (!m_isDeleteLine)
				{
					if (m_tetriminoMove[y][x] != 0)
					{
						DrawExtendGraph(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_tetriminoH[m_tetriminoType], true);
					}
				}
			}
		}
	}

	//���̃e�g���~�m��\��
	{
		for (int y = 0; y < kTetriminoHeight; y++)
		{
			for (int x = 0; x < kTetriminoWidth; x++)
			{
				int X = m_nextTetriminoX + x * kTetriminoSize;
				int Y = m_nextTetriminoY + y * kTetriminoSize;

				if (m_tetrimino[m_nextTetriminoType][0][y][x])
				{
					DrawExtendGraph(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_tetriminoH[m_nextTetriminoType], true);
				}
			}
		}
		//������\��
		DrawString(m_nextTetriminoX, m_nextTetriminoY - 30, L"next", 0xffffff);
		int X = m_nextTetriminoX - 10;
		int Y = m_nextTetriminoY - 30;
		//�g���\��
		DrawBox(X, Y, X + 140, Y + 130, 0xffffff, false);
	}

	//�z�[���h���Ă���e�g���~�m��\��
	{
		if (!m_isFirstHold)
		{
			for (int y = 0; y < kTetriminoHeight; y++)
			{
				for (int x = 0; x < kTetriminoWidth; x++)
				{
					int X = m_holdTetriminoX + x * kTetriminoSize;
					int Y = m_holdTetriminoY + y * kTetriminoSize;

					if (m_tetrimino[m_holdTetriminoType][0][y][x])
					{
						DrawExtendGraph(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_tetriminoH[m_holdTetriminoType], true);
					}
				}
			}
		}
		
		//������\��
		DrawString(m_holdTetriminoX, m_holdTetriminoY - 30, L"hold", 0xffffff);
		int X = m_holdTetriminoX - 10;
		int Y = m_holdTetriminoY - 30;
		//�g���\��
		DrawBox(X, Y, X + 140, Y + 130, 0xffffff, false);
	}
}

void Field::NomalUpdate(const InputState& input)
{
	if (--m_moveTime <= 0)
	{
		//���Ɉړ�
		if (input.IsPressed(InputType::left))
		{
			if (!TetriminoIsHit(m_tetriminoX - 1, m_tetriminoY, m_tetriminoType, m_tetriminoAngle))
			{
				m_tetriminoX--;
			}
			m_moveTime = kMoveTime;
			Sound::Play(Sound::BlockMove);
		}
		//�E�Ɉړ�
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
		//���ɉ�]
		if (input.IsPressed(InputType::leftroll))
		{
			m_angle--;
			m_rollTime = kRollTime;
			if (!TetriminoIsField())m_angle++;
			Sound::Play(Sound::BlockMove);
		}
		//�E�ɉ�]
		else if (input.IsPressed(InputType::rightroll))
		{
			m_angle++;
			m_rollTime = kRollTime;
			if(!TetriminoIsField())m_angle--;
			Sound::Play(Sound::BlockMove);
		}
	}

	if (input.IsPressed(InputType::hold))
	{
		if(!m_isHold)
		{
			TetriminoHold();
		}
	}

	//������X�s�[�h�𑬂�����
	if (input.IsPressed(InputType::down))
	{
		m_FallTime -= kFallSpeed;
	}
	//������X�s�[�h��x������
	else if (input.IsPressed(InputType::up))
	{
		m_FallTime += kSlowlyFallTime;
	}
	TetriminoPos();
	//�����鎞�Ԃ𐧌�
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
		m_lineNum = 0;//����������̂�������
		for (auto& line : m_deleteLine)
		{
			if (line)
			{
				m_lineNum++;
			}
			line = false;//������
		}
		//���������������ɗ��Ƃ�
		for (int i = 0; i < m_lineNum; i++)
		{
			DropLine();
		}
		Sound::Play(Sound::BlockPlacing);
		m_levelChange += m_lineNum;
		
		//������
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
	//��������ꏊ�����邩
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

	//�e�g���~�m������
	//0�`22�̓���2�`21
	for (int y = kFieldSpase; y <= kFieldHeight + kFieldFloor; y++)
	{
		if (m_deleteLine[y] == true)
		{
			//0�`11�̓���1�`10
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
	//0�`22�̓���2�`21
	for (int y = kFieldHeight + kFieldFloor; y >= kFieldSpase; y--)
	{
		//���󔒂��ǂ������m�F
		int pieceCount = 0;
		//0�`11�̓���1�`10
		for (int x = kFieldWidth; x >= kFieldWall; x--)
		{
			if (m_field[y][x] == SPASE)
			{
				pieceCount++;
			}
		}

		//���󔒂�������
		if (pieceCount >= kFieldWidth)
		{
			for (int x = kFieldWidth; x >= kFieldWall; x--)
			{
				//��ɂ���s�[�X�����ɗ��Ƃ�
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
	//�\������e�g���~�m�����Z�b�g
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		for (int x = 0; x < kFieldWidthMax; x++)
		{
			m_tetriminoMove[y][x] = 0;
		}
	}

	//�e�g���~�m�̈ʒu�����߂�
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

	//�e�g���~�m����]�������u���b�N�����
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

	if (num == 4)
	{
		isok = true;
	}

	if (isok)
	{
		m_tetriminoAngle = angle;
		TetriminoPos();
		return true;
	}
	return false;
}

void Field::TetriminoHold()
{
	if (!TetriminoHoldIsField())
	{
		return;
	}

	int temp = m_holdTetriminoType;
	m_holdTetriminoType = m_tetriminoType;
	m_tetriminoType = temp;

	if (m_isFirstHold)
	{
		m_isFirstHold = false;
		ResetTetrimino();
	}
	m_isHold = true;
}

bool Field::TetriminoHoldIsField()
{
	int angle = m_angle % AngleMax;
	int num = 0;
	bool isok = false;
	int copy[kFieldHeightMax][kFieldWidthMax] = {};

	//�t�B�[���h�Ƀz�[���h���Ă����e�g���~�m���o��
	for (int x = 0; x < kTetriminoWidth; x++)
	{
		for (int y = 0; y < kTetriminoHeight; y++)
		{
			if (m_tetrimino[m_holdTetriminoType][angle][y][x] != 0)
			{
				copy[m_tetriminoY + y][m_tetriminoX + x] = m_tetrimino[m_holdTetriminoType][angle][y][x];
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

	if (num == 4)
	{
		isok = true;
	}

	if (isok)
	{
		m_tetriminoAngle = angle;
		TetriminoPos();
		return true;
	}
	return false;
}

void Field::ResetTetrimino()
{
	//�����ʒu
	m_tetriminoX = 5;
	m_tetriminoY = 0;

	//���Ɍ��߂Ă����e�g���~�m��m_minoType�ɓ����
	m_tetriminoType = m_nextTetriminoType;
	
	//���̃e�g���~�m�����߂�
	srand((unsigned int)time(NULL));
	int randNum = rand() % 100;
	m_nextTetriminoType = randNum % TypeMax;
	
	//�\������p�x
	m_tetriminoAngle = 0;
	m_angle = AngleMax * 10;

	//�z�[���h�������ǂ�����false�ɂ���
	m_isHold = false;
}

bool Field::GameOver()
{
	//�V�����~�m���o���������ɁA�d�Ȃ镔�������݂�����Q�[���I�[�o�[
	for (int y = 0; y < 2; y++)
	{
		for (int x = 5; x < 5 + 4; x++)
		{
			if (m_field[y][x] != SPASE)
			{
				return true;
			}
		}
	}

	return false;
}
