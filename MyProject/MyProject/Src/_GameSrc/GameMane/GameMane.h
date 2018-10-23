#pragma once
#include <memory>

class Scene;

class GameMane 
{
public:
	// �f�X�g���N�^
	~GameMane();

	// �C���X�^���X�ϐ��̎擾
	static GameMane& Get(void) {
		static GameMane instance;
		return instance;
	}

	// �V�[���ڍs
	void ChangeScene(Scene* scene);

	// ����
	void Run(void);

private:
	// �R���X�g���N�^
	GameMane();
	GameMane(const GameMane&) {
	}
	void operator=(const GameMane&) {
	}

	// ������
	void Init(void);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	
	// �V�[��
	std::shared_ptr<Scene>scene;
};
