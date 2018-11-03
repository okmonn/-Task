#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

class Camera;

class Map
{
public:
	// �R���X�g���N�^
	Map(std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Map();

	// CSV�̓ǂݍ���
	int LoadCsv(const std::string& dir);

private:
	// �J����
	std::weak_ptr<Camera>cam;

	// �}�b�v�f�[�^
	std::map<int, std::vector<int>>map;
};
