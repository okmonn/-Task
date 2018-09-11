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

	// モーションデータの取得
	std::vector<Motion> GetMotion(unsigned int& index) {
		return motion[&index];
	}

private:
	// モーション
	std::map<unsigned int*, std::vector<Motion>>motion;
};

