#include "Title.h"
#include "../GameMane/GameMane.h"
#include "Teach.h"
#include "../Camera/Camerah.h"
#include "../Map/Map.h"
#include "../../Func/Func.h"

// �R���X�g���N�^
Title::Title()
{
	cam = std::make_shared<Camera>();
	map = std::make_shared<Map>(cam);
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	
}

// ����
void Title::UpData(void)
{

	if (func::CheckTriger(INPUT_RETURN))
	{
		GameMane::Get().ChangeScene(new Teach());
	}
}
