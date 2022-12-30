#pragma once

class InputState;

namespace
{
	//�t�B�[���h�̃e�g���~�m�𓮂�����͈�
	constexpr int kFieldHeight = 20;
	constexpr int kFieldWidth = 10;

	//��
	constexpr int kFieldWall = 1;
	//��
	constexpr int kFieldFloor = 1;
	//��̋��
	constexpr int kFieldSpase = 2;

	//�t�B�[���h�̑傫��
	constexpr int kFieldHeightMax = kFieldHeight + kFieldFloor + kFieldSpase;
	constexpr int kFieldWidthMax = kFieldWidth + kFieldWall * 2;

	//�e�g���~�m�̑傫��
	constexpr int kPieceHeight = 4;
	constexpr int kPieceWidth = 4;
	constexpr int kPieceSize = 30;

	//���ړ�����
	constexpr float kMoveWaitTime = 10.0f;
	//��]�ړ�����
	constexpr float kRollWaitTime = 10.0f;
	//��]�ړ�����
	constexpr float kDeleteWaitTime = 10.0f;
	//�����鎞��
	constexpr float kDropWaitTime = 40.0f;
	constexpr float kSlowlyDropWaitTime = 0.5f;
}

//�e�g���~�m�̎��
enum TetoriminoType
{
	TypeI,TypeO,
	TypeS,TypeZ,
	TypeJ,TypeL,
	TypeT,TypeMax
};
//�e�g���~�m����]������p�x
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
	//�e�g���~�m���t�B�[���h�ɒu��
	void PieceToField();
	//�e�g���~�m����񂻂���Ă��邩�m�F����
	void DeleteLine();
	//�e�g���~�m�����������������ɗ��Ƃ�
	void DropLine();
	//�e�g���~�m��\������ʒu�����߂�
	void display();
	//�e�g���~�m�����������Ă��邩�ǂ���
	bool isHit(int X, int Y, int type, int angle);
	//�e�g���~�m�����Z�b�g����
	void resetMino();

	//Update�p�����o�֐��|�C���^
	using UpdateFunc_t = void(Piece::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;

	//�t�B�[���h
	int m_field[kFieldHeightMax][kFieldWidthMax] = {};
	//�e�g���~�m���ړ�������t�B�[���h
	int m_minoMove[kFieldHeightMax][kFieldWidthMax] = {};
	//�e�g���~�m�������郉�C��
	bool m_deleteLine[kFieldHeightMax] = {};

	int randNum = 0;
	//�t�B�[���h��\������ʒu
	int m_startPosX = 0;
	int m_startPosY = 0;

	//�e�g���~�m�̎��
	int m_minoType = 0;
	//�e�g���~�m�̊p�x
	int m_minoAngle = 0;
	int m_angle = AngleMax * 10;
	//�e�g���~�m�̈ړ��ʒu
	int m_minoX = 0;
	int m_minoY = 0;

	//���̃e�g���~�m�̎��
	int m_nextMinoType = 0;
	//���̃e�g���~�m��\������ʒu
	int m_nextMinoX = 0;
	int m_nextMinoY = 0;

	//���ړ�����
	float m_moveTime = kMoveWaitTime;
	//��]�ړ�����
	float m_rollTime = kRollWaitTime;
	//��������
	float m_deleteTime = kDeleteWaitTime;
	//�����鎞��
	float m_dropTime = kDropWaitTime;
	//�����������Ƃ��̈ړ��X�s�[�h
	float m_dropSpeed = 8.0f;
	//�����郉�C�������邩�ǂ���
	bool m_isDeleteLine = false;

	//�e�g���~�m
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

	//�e�g���~�m�̐F�{�󔒂̐F�{��
	int m_color[TypeMax + 2] =
	{
		0xAAD8E6,//���F
		0xFFFF00,//���F
		0x008000,//��
		0xFF0000,//��
		0x0000FF,//��
		0xFFA500,//�I�����W
		0xFF00FF,//��

		0x87CEEB,//��F
		0xffffff//��
	};
};

