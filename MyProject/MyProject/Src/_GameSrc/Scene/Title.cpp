#include "Title.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../Camera/Camerah.h"
#include "../Map/Map.h"
#include "../Object/Player.h"
#include "../../Func/Func.h"

// コンストラクタ
Title::Title()
{
	cam = std::make_shared<Camera>();
	map = std::make_shared<Map>(cam);
	pl = std::make_shared<Player>(cam);
	cam->SetTarget(pl);
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	pl->Draw();
	map->Draw("tile");
}

// 処理
void Title::UpData(void)
{
	cam->UpData({640 * 3, 480});
	pl->UpData();
	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
