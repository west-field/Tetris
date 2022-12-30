#include "SceneManager.h"
#include "Secne.h"

void
SceneManager::ChangeScene(Scene* scene)
{
	//�V�[���X�^�b�N��(empty)��ł͂Ȃ�������
	if (!m_scenes.empty())
	{
		delete m_scenes.front();
		m_scenes.pop_front();
	}
	m_scenes.push_front(scene);//�������
}

void SceneManager::PushScene(Scene* scene)
{
	m_scenes.push_front(scene);//�������
}

void SceneManager::PopScene()
{
	//�|�b�v�̌��ʃV�[����0�ɂȂ�Ȃ��悤�ɂ���
	if (m_scenes.size() > 1)
	{
		//�����̃V�[���Ɉړ����邽�߂�top�ɂ���Scene�̃|�C���^������
		//���|�C���^��������������scenes_�ɂ͎c���Ă��邩��pop�ň�ԏ�̗v�f�������Ă���
		delete m_scenes.front();//top�͂Ă��؂�̗v�f��Ԃ��Ă���
		m_scenes.pop_front();//����遨���ʂ�0�ɂȂ�
	}
}

void
SceneManager::Update(const InputState& input)
{
	m_scenes.front()->Update(input);
}

void
SceneManager::Draw()
{
	for (int i = m_scenes.size() - 1; i >= 0; i--)
	{
		m_scenes[i]->Draw();
	}
}