#include "MapEditor.h"
#include "../../Func/Func.h"

// �R���X�g���N�^
MapEditor::MapEditor() :
	index(0), pos(0.0f), size(0.0f), mapSize(0.0f), chipCnt(0)
{
	map.clear();
}

// �f�X�g���N�^
MapEditor::~MapEditor()
{
}

// �}�b�v���̃Z�b�g
void MapEditor::MapInfo(const Vec2f & mapSize, const Vec2 & chipCnt)
{
	this->mapSize = mapSize;
	this->chipCnt = chipCnt;
}

// �`��
void MapEditor::Draw(void)
{
	for (int y = 0; y < chipCnt.y; ++y)
	{
		func::DrawLine(0, mapSize.y / chipCnt.y * y, mapSize.x, mapSize.y / chipCnt.y * y, 1, 1, 1);
	}
}

// ����
void MapEditor::UpData(void)
{
}
