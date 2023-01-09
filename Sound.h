#pragma once

//�T�E���h�֘A����
namespace Sound
{
	enum SoundId
	{
		//BGM
		BgmMain,		//���C��BGM
		BgmGameover,	//�I���������BGM
		//���ʉ�
		Cursor,			//�J�[�\���ړ���
		Determinant,	//����{�^��
		Count,			//�J�E���g��
		BlockMove,		//�u���b�N�ړ���
		BlockPlacing,	//�u���b�N��u����
		Blocking,		//�u���b�N���󂵂���

		Max
	};

	//���[�h�A�����[�h
	void Load();
	void Unload();
	//BGM�̍Đ�
	void StartBgm(SoundId id,int volume = 255);
	void StopBgm(SoundId id);
	//���ʉ��̍Đ�
	void Play(SoundId id);
	//���ʐݒ�
	void SetVolume(SoundId id, int volume);
};

