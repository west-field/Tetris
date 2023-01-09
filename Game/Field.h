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
	constexpr int kTetriminoHeight = 4;
	constexpr int kTetriminoWidth = 4;
	constexpr int kTetriminoSize = 30;

	//

	//���ړ�����
	constexpr float kMoveTime = 8.0f;
	//��]�ړ�����
	constexpr float kRollTime = 9.5f;
	//��]�ړ�����
	constexpr float kDeleteTime = 20.0f;
	//�����鎞��
	constexpr float kFallTime = 60.0f;
	//�����������Ƃ��̈ړ��X�s�[�h
	constexpr float kFallSpeed = 8.0f;
	//����������Ƃ������鎞�Ԃ��x���Ȃ�
	constexpr float kSlowlyFallTime = 0.5f;
}

//�e�g���~�m�̎��
enum TetriminoType
{
	TypeI,TypeO,
	TypeS,TypeZ,
	TypeJ,TypeL,
	TypeT,TypeMax
};
//�e�g���~�m����]������p�x
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
	//�Q�[���I�[�o�[����
	bool GameOver();
private:
	void NomalUpdate(const InputState& input);
	void DeleteLineUpdate(const InputState& input);
	//�e�g���~�m���t�B�[���h�ɒu��
	void TetriminoToField();
	//�e�g���~�m����񂻂���Ă��邩�m�F����
	void DeleteLine();
	//�e�g���~�m�����������������ɗ��Ƃ�
	void DropLine();
	//�e�g���~�m��\������ʒu�����߂�
	void TetriminoPos();
	//�e�g���~�m�����������Ă��邩�ǂ���
	bool TetriminoIsHit(int X, int Y, int type, int angle);
	//�e�g���~�m����]�����Ƃ��Ƀt�B�[���h���ɂ��邩�ǂ���
	void TetriminoIsField();
	//�e�g���~�m�����Z�b�g����
	void ResetTetrimino();
	
	//Update�p�����o�֐��|�C���^
	using UpdateFunc_t = void(Field::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;

	//�t�B�[���h
	int m_field[kFieldHeightMax][kFieldWidthMax] = {};
	//�e�g���~�m���ړ�������t�B�[���h
	int m_tetriminoMove[kFieldHeightMax][kFieldWidthMax] = {};
	//�e�g���~�m�������郉�C��
	bool m_deleteLine[kFieldHeightMax] = {};

	//�t�B�[���h��\������ʒu
	int m_startPosX = 0;
	int m_startPosY = 0;

	//�e�g���~�m�̎��
	int m_tetriminoType = 0;
	//�e�g���~�m�̊p�x
	int m_tetriminoAngle = 0;
	int m_angle = AngleMax * 10;
	//�e�g���~�m�̈ړ��ʒu
	int m_tetriminoX = 0;
	int m_tetriminoY = 0;

	//���̃e�g���~�m�̎��
	int m_nextTetriminoType = 0;
	//���̃e�g���~�m��\������ʒu
	int m_nextTetriminoX = 0;
	int m_nextTetriminoY = 0;

	//���ړ�����
	float m_moveTime = kMoveTime;
	//��]�ړ�����
	float m_rollTime = kRollTime;
	//��������
	float m_deleteTime = kDeleteTime;
	//�����鎞��
	float m_FallTime = kFallTime;
	//�����郉�C�������邩�ǂ���
	bool m_isDeleteLine = false;

	//�e�g���~�m
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

