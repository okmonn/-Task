#pragma once
#include "PmdData.h"
#include <memory>

class MotionLoader
{
public:
	// コンストラクタ
	MotionLoader();
	// デストラクタ
	~MotionLoader();

	// 読み込み
	int Load(const std::string& fileName);

	// モーションデータの取得
	std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>> Get(const std::string& fileName) {
		return motion[fileName];
	}

	// アニメーションの時間の取得
	float GetAnimTime(const std::string& fileName) {
		return animTime[fileName];
	}

private:
	// モーションデータ
	std::map<std::string, std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>>>motion;

	// アニメーションの時間
	std::map<std::string, float>animTime;
};
