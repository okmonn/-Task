#include "Stage.h"
#include "Load.h"

//1チップ 64x64サイズ

#define CHIP_SIZE 64

const char* path = "ステージ/stage1.fmf";

// コンストラクタ
Stage::Stage()
{
	Load(path);
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
	rect = Rect(0, 0, data.mapWidth * CHIP_SIZE, data.mapHeight * CHIP_SIZE);
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
