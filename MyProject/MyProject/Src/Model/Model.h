#pragma once
#include "PmdData.h"
#include <memory>

class Device;
class List;
class ModelLoader;

class Model
{
public:
	// �R���X�g���N�^
	Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~Model();

	// �ǂݍ���
	long Load(const std::string& fileName, int& i);

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// ���f�����[�_�[
	std::shared_ptr<ModelLoader>loader;

	// ���_
	std::map<int*, std::weak_ptr<std::vector<pmd::Vertex>>>vertex;
};
