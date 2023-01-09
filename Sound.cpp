#include "Sound.h"
#include <DxLib.h>
#include <vector>

namespace
{
	//�T�E���h�n���h��
	std::vector<int> m_soundHandle;

	//�T�E���h�t�@�C����
	const TCHAR* const kFileName[Sound::Max] =
	{
		L"Sound/sekibaku.mp3",//���C��BGM
		L"Sound/mistake.mp3",//�I���������BGM

		L"Sound/paperRoll.mp3",//�J�[�\���ړ���
		L"Sound/decisionPressing.mp3",//����{�^��
		L"Sound/count.mp3",//�J�E���g��
		L"Sound/blockMove.mp3",//�u���b�N�ړ���
		L"Sound/blockPlace.mp3",//�u���b�N��u����
		L"Sound/blockBreak.mp3"//�u���b�N���󂵂���
	};
}

namespace Sound
{
	//���[�h�A�����[�h
	void Load()
	{
		// �T�E���h�f�[�^�̓ǂݍ���
		for (auto& fileName : kFileName)
		{
			int handle = LoadSoundMem(fileName);
			m_soundHandle.push_back(handle);
		}
	}
	void Unload()
	{
		// �T�E���h�f�[�^�̉��
		for (auto& handle : m_soundHandle)
		{
			DeleteSoundMem(handle);
			handle = -1;
		}
	}
	//BGM�̍Đ�
	void StartBgm(SoundId id, int volume)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
		SetVolume(id, volume);
	}
	void StopBgm(SoundId id)
	{
		StopSoundMem(m_soundHandle[id]);
	}
	//���ʉ��̍Đ�
	void Play(SoundId id)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
	}
	//���ʐݒ�
	void SetVolume(SoundId id, int volume)
	{
		ChangeVolumeSoundMem(volume, m_soundHandle[id]);
	}
}