#pragma once

//�v���g�^�C�v�錾
class SceneManager;//�V�[���}�l�[�W���[
class InputState;//���̓X�e�[�g

/// <summary>
/// �V�[�����N���X//���ɂȂ��Ă���
/// </summary>
class Scene
{
protected:
	SceneManager& m_manager;//�V�[���}�l�[�W���[�ւ̎Q��
	
	static constexpr int kFadeInterval = 60;
	int m_fadeTimer = kFadeInterval;//�t�F�[�h�^�C�}�[
	int m_fadeValue = 255;//����`�Ƃ̃u�����h�

	//�|�C���g
	int m_point = 0;
	//���x��
	int m_level = 1;
	//���x���ݒ�
	void SetLevel(int i) { m_level += i; }
public:
	Scene(SceneManager& manager) :m_manager(manager){}
	virtual ~Scene() {}
	//�V�[�����X�V����
	virtual void Update(const InputState& input) = 0;
	//�V�[����`�悷��
	virtual void Draw() = 0;
};

