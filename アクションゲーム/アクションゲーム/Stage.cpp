#include "Stage.h"
#include "Load.h"

// チップサイズ
#define CHIP_SIZE 64

const char* path = "ステージ/stage1.fmf";

// コンストラクタ
Stage::Stage()
{
	Load(path);
	readX = 0;
}

// デストラクタ
Stage::~Stage()
{
}

// 読み込み
void Stage::Load(std::string fileName)
{
	Load::GetInstance()->LoadMap(fileName);

	data = Load::GetInstance()->GetStageHeader(fileName);
	data.chipHeight *= 2;
	data.chipWidth *= 2;

	//ステージサイズ
	rect = Rect(0, 0, (data.mapWidth * CHIP_SIZE) * 2, (data.mapHeight * CHIP_SIZE));

	//ステージの敵データ
	eData = Load::GetInstance()->GetEnemyData(fileName);
}

// 処理
void Stage::UpData(void)
{
}

// ステージサイズの取得
Rect & Stage::GetStageRange(void)
{
	return rect;
}

// ステージの敵データの取得
std::vector<UCHAR> Stage::GetEnemyData(int minx, int max)
{
	int left = max(minx / CHIP_SIZE, readX);
	int right = max / CHIP_SIZE;
	if (right <= readX)
	{
		return std::vector<UCHAR>();
	}

	auto index = left * data.mapHeight;
	auto indey = right * data.mapHeight;

	auto begin = eData.begin() + index;
	auto itr = eData.end();
	auto end = itr;
	
	if (indey < eData.size())
	{
		end = eData.begin() + indey;
		readX = right;
	}
	else
	{
		return std::vector<UCHAR>();
	}

	return std::vector<UCHAR>(begin, end);
}

std::vector<UCHAR> Stage::GetEnemyData(void)
{
	return eData;
}
