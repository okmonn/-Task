#include "Model.h"
#include "ModelLoader.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// �R���X�g���N�^
Model::Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list) : 
	dev(dev), list(list), loader(std::make_shared<ModelLoader>())
{
}

// �f�X�g���N�^
Model::~Model()
{
	loader.reset();
}

// �ǂݍ���
long Model::Load(const std::string & fileName, int & i)
{
	if (FAILED(loader->Load(fileName)))
	{
		return S_FALSE;
	}

	vertex[&i] = loader->GetVertex(fileName);
	return 0;
}
