#include "InputState.h"
#include<DxLib.h>

InputState::InputState()
{
	m_inputMapTable[InputType::next] = { {InputCategory::keybd,KEY_INPUT_RETURN},
										{InputCategory::pad,PAD_INPUT_R},//�X�^�[�g�{�^��
										{InputCategory::mouse,MOUSE_INPUT_LEFT} };

	m_inputMapTable[InputType::prev] = { {InputCategory::keybd,KEY_INPUT_ESCAPE},
										{InputCategory::pad,PAD_INPUT_B} };

	m_inputMapTable[InputType::pause] = { {InputCategory::keybd,KEY_INPUT_P},
										{InputCategory::pad,PAD_INPUT_L} };//�Z���N�g�{�^��

	m_inputMapTable[InputType::keyconf] = { {InputCategory::keybd,KEY_INPUT_K},
											{InputCategory::pad,PAD_INPUT_Y} };//���V�����_�[

	m_inputMapTable[InputType::up] = { {InputCategory::keybd,KEY_INPUT_UP},
											{InputCategory::pad,PAD_INPUT_UP} };//��V�����_�[

	m_inputMapTable[InputType::down] = { {InputCategory::keybd,KEY_INPUT_DOWN},
											{InputCategory::pad,PAD_INPUT_DOWN} };//���V�����_�[

	m_inputMapTable[InputType::left] = { {InputCategory::keybd , KEY_INPUT_LEFT},
											{InputCategory::pad , PAD_INPUT_LEFT} };//��

	m_inputMapTable[InputType::right] = { {InputCategory::keybd , KEY_INPUT_RIGHT},
											{InputCategory::pad , PAD_INPUT_RIGHT} };//�E

	m_inputMapTable[InputType::leftroll] = {	{InputCategory::keybd , KEY_INPUT_Z},
												{InputCategory::pad , PAD_INPUT_A} };

	m_inputMapTable[InputType::rightroll] = {	{InputCategory::keybd , KEY_INPUT_X},
												{InputCategory::pad , PAD_INPUT_B} };

	//���̓^�C�v�̖��O�e�[�u�������
	m_inputNameTable[InputType::next] = L"next";
	m_inputNameTable[InputType::prev] = L"prev";
	m_inputNameTable[InputType::pause] = L"pause";
	m_inputNameTable[InputType::keyconf] = L"keyconf";

	m_currentInput.resize(static_cast<int>(InputType::max));
	m_lastInput.resize(static_cast<int>(InputType::max));
}

void
InputState::Update()
{
	m_lastInput = m_currentInput;//���O�̓��͏����L�����Ă���

	char keystate[256];
	GetHitKeyStateAll(keystate);//�S�L�[���擾
	int padState = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h1�R���̏����擾����
	int mouseState = GetMouseInput();
	//����input�̒��g�́Akeybd,KEY_INPUT_RETURN�Ȃǂ̃Z�b�g(InputInfo)�������Ă���
	//keymap.second�ɂ́A���̓��͏��Z�b�g�������Ă���
	//keymap.first�ɂ́A�Ή�����Q�[�����͖���"InputType::next"�Ȃǂ������Ă���
	for (const auto& keymap : m_inputMapTable)
	{
		for (const auto& input : keymap.second)
		{
			if (input.cat == InputCategory::keybd)
			{
				m_currentInput[static_cast<int>(keymap.first)] = keystate[input.id];
			}
			else if (input.cat == InputCategory::pad)
			{
				m_currentInput[static_cast<int>(keymap.first)] = padState & input.id;
			}
			else if (input.cat == InputCategory::mouse)
			{
				m_currentInput[static_cast<int>(keymap.first)] = mouseState & input.id;
			}
			if (m_currentInput[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

bool
InputState::IsPressed(InputType type) const
{
	return m_currentInput[static_cast<int>(type)];
}

bool
InputState::IsTriggered(InputType type) const
{
	return IsPressed(type) && !m_lastInput[static_cast<int>(type)];
}

void InputState::RewriteInputInfo(InputType type, InputCategory cat, int id)
{
	//���͎��(���蓖�Đ�)���Ȃ���΁A�����Ȃ̂Ŗ�������B
	if (m_inputMapTable.count(type) == 0)
	{
		return;
	}
	bool isRewrite = false;
	for (auto& inputInfo : m_inputMapTable[type])
	{
		if (inputInfo.cat == cat)//�J�e�S�����q�b�g������
		{
			//ID���㏑������
			inputInfo.id = id;
			isRewrite = true;
			break;
		}
	}
	if (!isRewrite)
	{
		//�����J�e�S�������݂��Ȃ�������A�����Œǉ����Ă���
		m_inputMapTable[type].push_back({ cat,id });
	}
}


