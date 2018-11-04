#include "Title.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../Camera/Camerah.h"
#include "../Map/Map.h"
#include "../Object/Player.h"
#include "../../Func/Func.h"

// �R���X�g���N�^
Title::Title()
{
	cam = std::make_shared<Camera>();
	map = std::make_shared<Map>(cam);
	pl = std::make_shared<Player>(cam);
	cam->SetTarget(pl);
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	pl->Draw();
	map->Draw("tile");
}

// ����
void Title::UpData(void)
{
	cam->UpData({640 * 3, 480});
	pl->UpData();
	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
