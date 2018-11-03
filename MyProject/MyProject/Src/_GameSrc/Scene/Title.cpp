#include "Title.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../Camera/Camerah.h"
#include "../Map/Map.h"
#include "../../Func/Func.h"

// コンストラクタ
Title::Title()
{
	cam = std::make_shared<Camera>();
	map = std::make_shared<Map>(cam);
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	
}

// 処理
void Title::UpData(void)
{

	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
