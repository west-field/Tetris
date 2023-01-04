#define SPASE -1

#include "Field.h"
#include <DxLib.h>
#include <stdlib.h>
#include <time.h>
#include "../game.h"
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
		//���
		m_field[0][x] = 7;
		m_field[1][x] = 7;
		//��
		m_field[kFieldHeightMax - 1][x] = 8;
	}
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		//��
		m_field[y][0] = 8;
		m_field[y][kFieldWidthMax - 1] = 8;
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
					DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[m_field[y][x]], true);
				}
				//�e�g���~�m��\��
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
					DrawBox(X, Y, X + kTetriminoSize, Y + kTetriminoSize, m_color[m_nextTetriminoType], true);
				}
			}
		}
	}

	DrawFormatString(0, 0, 0xffaaff, L"m_minoType%d,m_minoAngle%d", m_tetriminoType, m_tetriminoAngle);
}

void Field::NomalUpdate(const InputState& input)
{
	TetriminoPos();

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
		}
		//�E�Ɉړ�
		else if (input.IsPressed(InputType::right))
		{
			if (!TetriminoIsHit(m_tetriminoX + 1, m_tetriminoY, m_tetriminoType, m_tetriminoAngle))
			{
				m_tetriminoX++;
			}
			m_moveTime = kMoveTime;
		}
	}
	if (--m_rollTime <= 0)
	{
		//���ɉ�]
		if (input.IsPressed(InputType::leftroll))
		{
			m_angle--;
			m_tetriminoAngle = m_angle % AngleMax;
			m_rollTime = kRollTime;
		}
		//�E�ɉ�]
		else if (input.IsPressed(InputType::rightroll))
		{
			m_angle++;
			m_tetriminoAngle = m_angle % AngleMax;
			m_rollTime = kRollTime;
		}
	}

	//������X�s�[�h�𑬂�����
	if (input.IsPressed(InputType::down))
	{
		m_dropTime -= m_dropSpeed;
	}
	//������X�s�[�h��x������
	else if (input.IsPressed(InputType::up))
	{
		m_dropTime += kSlowlyFallTime;
	}
	//�����鎞�Ԃ𐧌�
	if (--m_dropTime <= 0)
	{
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
		m_dropTime = kFallTime;
	}
}

void Field::DeleteLineUpdate(const InputState& input)
{
	if (--m_deleteTime <= 0)
	{
		int lineNum = 0;//����������̂�������
		for (auto& line : m_deleteLine)
		{
			if (line)
			{
				lineNum++;
			}
			line = false;//������
		}
		//���������������ɗ��Ƃ�
		for (int i = 0; i < lineNum; i++)
		{
			DropLine();
		}
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
	for (int y = 0; y < kTetriminoHeight; y++)
	{
		for (int x = 0; x < kTetriminoWidth; x++)
		{
			if (m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x] != 0)
			{
				m_tetriminoMove[m_tetriminoY + y][m_tetriminoX + x] = m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x];
			}
		}
	}

	//���������e�g���~�m���t�B�[���h���ɂ��邩�ǂ���
	bool isInField = true;
	int X = 0;
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		//�ǂɓ��肱��ł��邩
		if (m_tetriminoMove[y][0] != 0)
		{
			X = 0;
			isInField = false;
			break;
		}
		if (m_tetriminoMove[y][kFieldWidthMax - 1] != 0)
		{
			X = kFieldWidth;
			isInField = false;
			break;
		}
	}

	if (!isInField)
	{
		//������x�e�g���~�m�̈ʒu�����߂�
		for (int y = 0; y < kTetriminoHeight; y++)
		{
			for (int x = 0; x < kTetriminoWidth; x++)
			{
				if (m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x] != 0)
				{
					m_tetriminoMove[m_tetriminoY + y][X + x] = m_tetrimino[m_tetriminoType][m_tetriminoAngle][y][x];
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
}