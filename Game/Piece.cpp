#define SPASE -1

#include "Piece.h"
#include <DxLib.h>
#include <stdlib.h>
#include <time.h>
#include "../game.h"
#include "../InputState.h"

Piece::Piece():m_updateFunc(&Piece::NomalUpdate)
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
		//m_field[0][x] = 7;
		//��
		m_field[kFieldHeightMax - 1][x] = 8;
	}
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		//��
		m_field[y][0] = 8;
		m_field[y][kFieldWidthMax - 1] = 8;
	}

	randNum = rand() % 100;

	m_startPosX = (Game::kScreenWidth - kPieceSize * kFieldWidthMax) / 2;
	m_startPosY = (Game::kScreenHeight - kPieceSize * kFieldHeightMax) / 2;

	m_nextMinoType = randNum % TypeMax;
	m_nextMinoX = m_startPosX + kPieceSize * kFieldWidthMax + kPieceSize * 3;
	m_nextMinoY = m_startPosY + kPieceSize * 3;
}

Piece::~Piece()
{

}

void Piece::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void Piece::Darw()
{
	//�t�B�[���h��\��
	{
		for (int y = 0; y < kFieldHeightMax; y++)
		{
			for (int x = 0; x < kFieldWidthMax; x++)
			{
				int X = m_startPosX + x * kPieceSize;
				int Y = m_startPosY + y * kPieceSize;

				//�g��\��
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawBox(X, Y, X + kPieceSize, Y + kPieceSize, m_color[8], false);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				//�t�B�[���h�̃e�g���~�m��\��
				if (m_field[y][x] != SPASE)
				{
					DrawBox(X, Y, X + kPieceSize, Y + kPieceSize, m_color[m_field[y][x]], true);
				}
				//�e�g���~�m��\��
				if (!m_isDeleteLine)
				{
					if (m_minoMove[y][x] != 0)
					{
						DrawBox(X, Y, X + kPieceSize, Y + kPieceSize, m_color[m_minoType], true);
					}
				}
			}
		}
	}

	//���̃e�g���~�m��\��
	{
		for (int y = 0; y < kPieceHeight; y++)
		{
			for (int x = 0; x < kPieceWidth; x++)
			{
				int X = m_nextMinoX + x * kPieceSize;
				int Y = m_nextMinoY + y * kPieceSize;

				if (m_mino[m_nextMinoType][0][y][x])
				{
					DrawBox(X, Y, X + kPieceSize, Y + kPieceSize, m_color[m_nextMinoType], true);
				}
			}
		}
	}

	DrawFormatString(0, 0, 0xffaaff, L"m_minoType%d,m_minoAngle%d", m_minoType, m_minoAngle);
}

void Piece::NomalUpdate(const InputState& input)
{
	display();

	if (--m_moveTime <= 0)
	{
		//���Ɉړ�
		if (input.IsPressed(InputType::left))
		{
			if (!isHit(m_minoX - 1, m_minoY, m_minoType, m_minoAngle))
			{
				m_minoX--;
			}
			m_moveTime = kMoveWaitTime;
		}
		//�E�Ɉړ�
		else if (input.IsPressed(InputType::right))
		{
			if (!isHit(m_minoX + 1, m_minoY, m_minoType, m_minoAngle))
			{
				m_minoX++;
			}
			m_moveTime = kMoveWaitTime;
		}
	}
//��	
	if (--m_rollTime <= 0)
	{
		//���ɉ�]
		if (input.IsPressed(InputType::leftroll))
		{
			m_angle--;
			m_minoAngle = m_angle % AngleMax;
			m_rollTime = kRollWaitTime;
		}
		//�E�ɉ�]
		else if (input.IsPressed(InputType::rightroll))
		{
			m_angle++;
			m_minoAngle = m_angle % AngleMax;
			m_rollTime = kRollWaitTime;
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
		m_dropTime += kSlowlyDropWaitTime;
	}
	//�����鎞�Ԃ𐧌�
	if (--m_dropTime <= 0)
	{
		if (!isHit(m_minoX, m_minoY + 1, m_minoType, m_minoAngle))
		{
			m_minoY++;
		}
		else
		{
			PieceToField();
			DeleteLine();
			resetMino();
		}
		m_dropTime = kDropWaitTime;
	}
}
//��
void Piece::DeleteLineUpdate(const InputState& input)
{
	if (--m_deleteTime <= 0)
	{
		for (int i = 0; i < kFieldHeightMax; i++)
		{
			DropLine();
		}
		resetMino();
		m_deleteTime = kDeleteWaitTime;
		m_isDeleteLine = false;
		m_updateFunc = &Piece::NomalUpdate;
	}
}

void Piece::PieceToField()
{
	for (int y = kFieldHeightMax - 1; y > 0 ; y--)
	{
		for (int x = kFieldWidthMax - 1; x > 0; x--)
		{
			if (m_minoMove[y][x] != 0)
			{
				m_field[y][x] = m_minoType;
				m_minoMove[y][x] = 0;
			}
		}
	}
}
//���e�g���~�m����񂻂���Ă������������
void Piece::DeleteLine()
{
	//��������ꏊ�����邩
	for (int y = 1; y <= kFieldHeightMax - 2; y++)
	{
		for (int x = 1; x <= kFieldWidthMax - 2; x++)
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
	for (int y = 1; y <= kFieldHeightMax - 2; y++)
	{
		if (m_deleteLine[y] == true)
		{
			for (int x = 1; x <= kFieldWidthMax - 2; x++)
			{
				m_field[y][x] = SPASE;
			}
			m_isDeleteLine = true;
		}
	}
	
	if (m_isDeleteLine)
	{
		m_updateFunc = &Piece::DeleteLineUpdate;
	}
}
//���e�g���~�m�����������������ɗ��Ƃ�
void Piece::DropLine()
{
	for (int y = kFieldHeight + kFieldFloor; y >= kFieldSpase; y--)
	{
		//���󔒂��ǂ������m�F
		int deleteOk = 0;
		for (int x = kFieldWidth + kFieldWall; x >= kFieldWall; x--)
		{
			if (m_field[y][x] == SPASE)
			{
				deleteOk++;
			}
		}

		//���󔒂�������
		if (deleteOk == kFieldWidth)
		{
			for (int x = kFieldWidth + kFieldWall; x >= kFieldWall; x--)
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

void Piece::display()
{
	//�\������e�g���~�m�����Z�b�g
	for (int y = 0; y < kFieldHeightMax; y++)
	{
		for (int x = 0; x < kFieldWidthMax; x++)
		{
			m_minoMove[y][x] = 0;
		}
	}

	//���������ʒu�Ƀe�g���~�m��\������
	for (int y = 0; y < kPieceHeight; y++)
	{
		for (int x = 0; x < kPieceWidth; x++)
		{
			if (m_mino[m_minoType][m_minoAngle][y][x] != 0)
			{
				m_minoMove[m_minoY + y][m_minoX + x] = m_mino[m_minoType][m_minoAngle][y][x];
			}
		}
	}
}

bool Piece::isHit(int X, int Y, int type, int angle)
{
	for (int y = 0; y < kPieceHeight; y++)
	{
		for (int x = 0; x < kPieceWidth; x++)
		{
			if (m_mino[type][angle][y][x] == 1 && m_field[Y + y][X + x] != SPASE)
			{
				return true;
			}
		}
	}
	return false;
}

void Piece::resetMino()
{
	//�����ʒu
	m_minoX = 5;
	m_minoY = 0;

	srand((unsigned int)time(NULL));
	randNum = rand() % 100;

	//���Ɍ��߂Ă����e�g���~�m��m_minoType�ɓ����
	m_minoType = m_nextMinoType;
	//���̃e�g���~�m�����߂�
	m_nextMinoType = randNum % TypeMax;
	
	//�\������p�x
	m_minoAngle = 0;
	m_angle = AngleMax * 10;

	for (auto& deLine : m_deleteLine)
	{
		deLine = false;
	}
}
