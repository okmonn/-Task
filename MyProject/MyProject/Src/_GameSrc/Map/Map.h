#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <thread>

class Camera;
class Chip;

class Map
{
public:
	// �R���X�g���N�^
	Map(std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	~Map();

	// CSV�̓ǂݍ���
	int LoadCsv(const std::string& fileName);

	// �`��
	void Draw(const std::string& type);

private:
	// �\�[�g
	void Sort(void);

	// �摜�̓ǂݍ���
	void Load(const std::string& fileName, const std::string& type);

	

	
	// �J����
	std::weak_ptr<Camera>cam;

	// �X���b�h�Ǘ��t���O
	bool threadFlag;

	// �}�b�v�f�[�^
	std::map<std::string, std::vector<int>>map;

	// �}�b�v�`�b�v
	std::map<std::string, std::list<std::shared_ptr<Chip>>>chip;

	// �X���b�h
	std::map<std::string, std::thread>th;
};
