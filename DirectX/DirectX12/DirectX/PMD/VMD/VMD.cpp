#include "VMD.h"

// コンストラクタ
VMD::VMD()
{
	motion.clear();
}

// デストラクタ
VMD::~VMD()
{
}

// 読み込み
int VMD::Load(unsigned int & index, const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	//ヘッダー　
	VmdHeader header = {};
	fread(&header, sizeof(header), 1, file);

	int num = 0;
	fread(&num, sizeof(num), 1, file);

	//モーション
	std::vector<VmdMotion>motion(num);
	this->motion[&index].resize(num);
	int cnt = 0;
	for (auto& i : motion)
	{
		fread(&i.bornName,      sizeof(i.bornName),      1, file);
		fread(&i.flam,          sizeof(i.flam),          1, file);
		fread(&i.pos,           sizeof(i.pos),           1, file);
		fread(&i.rotation,      sizeof(i.rotation),      1, file);
		fread(&i.interpolation, sizeof(i.interpolation), 1, file);

		this->motion[&index][cnt].name = i.bornName;
		this->motion[&index][cnt].quaternion = DirectX::XMLoadFloat4(&i.rotation);
		++cnt;
	}

	return 0;
}
