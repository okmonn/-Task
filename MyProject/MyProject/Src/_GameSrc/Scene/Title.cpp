#include "Title.h"
#include "../MapEditor/MapEditor.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../../Func/Func.h"

// �R���X�g���N�^
Title::Title()
{
	map = std::make_shared<MapEditor>();
	map->MapInfo({ 1280, 960 }, { 10, 10 });
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	map->Draw();
}

// ����
void Title::UpData(void)
{
	map->UpData();

	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
