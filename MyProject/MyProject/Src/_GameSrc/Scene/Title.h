#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class Map;
class Player;

class Title :
	public Scene
{
public:
	// �R���X�g���N�^
	Title();
	// �f�X�g���N�^
	~Title();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	std::shared_ptr<Camera>cam;
	std::shared_ptr<Map>map;
	std::shared_ptr<Player>pl;
};
