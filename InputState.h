#pragma once

#include <map>
#include <vector>
#include <string>

enum class InputType
{
	next,//���֍s���{�^��
	prev,//�O�ɖ߂�{�^��
	pause,//�|�[�Y�{�^��
	keyconf,//�L�[�R���t�B�O
	up,//��
	down,//��
	left,//��
	right,//�E

	leftroll,//����]
	rightroll,//�E��]

	hold,//�z�[���h

	max
};

/// <summary>
/// ���͑��u�J�e�S��
/// </summary>
enum class InputCategory
{
	keybd,
	pad,
	mouse
};

/// <summary>
/// ���͏��
/// </summary>
struct InputInfo
{
	InputCategory cat;//
	int id;
};

class KeyConfigScene;

/// <summary>
/// ���͏�Ԃ��Ǘ�����
/// </summary>
class InputState
{
	friend KeyConfigScene;//KeyConfigScene�ɂ����A���ׂĂ�������B
private:
	std::map<InputType,std::vector<InputInfo>> m_inputMapTable;

	//����
	std::map<InputType, std::wstring> m_inputNameTable;

	std::vector<bool> m_currentInput;
	std::vector<bool> m_lastInput;

public:
	InputState();

	/// <summary>
	/// ���͏����X�V����
	/// �i���Ӂj���t���[��Update���Ă΂Ȃ��Ɠ��͏󋵂͍X�V����Ȃ�
	/// </summary>
	void Update();
	
	/// <summary>
	/// ������Ă���true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>������Ă�����true,������ĂȂ��Ȃ�false</returns>
	bool IsPressed(InputType type) const;
	
	/// <summary>
	/// �������u�Ԃ�true�ɂȂ�
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>�������u�ԂȂ�true�A����ȊO�Ȃ�false</returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// ���͏����X�V����
	/// </summary>
	/// <param name="type">�Q�[���ɂ�������͎��</param>
	/// <param name="cat">���̓J�e�S��</param>
	/// <param name="id">���ۂ̓���</param>
	void RewriteInputInfo(InputType type,InputCategory cat,int id);
};

