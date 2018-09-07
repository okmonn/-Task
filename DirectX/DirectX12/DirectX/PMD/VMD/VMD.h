#pragma once
#include "VmdDate.h"
#include <map>
#include <vector>

class VMD
{
public:
	// コンストラクタ
	VMD();
	// デストラクタ
	~VMD();

	// 読み込み
	int Load(unsigned int& index, const std::string& fileName);

private:
	// モーション
	std::map<unsigned int*, std::vector<Motion>>motion;
};

