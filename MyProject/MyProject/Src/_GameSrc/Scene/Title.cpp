#include "Title.h"
#include "../MapEditor/MapEditor.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../../Func/Func.h"

// コンストラクタ
Title::Title()
{
	map = std::make_shared<MapEditor>();
	map->MapInfo({ 1280, 960 }, { 10, 10 });
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	map->Draw();
}

// 処理
void Title::UpData(void)
{
	map->UpData();

	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
