#pragma once
#include "Obj.h"
#include <d3d12.h>

class Debug :
	public Obj
{
public:
	// �R���X�g���N�^
	Debug();
	// �f�X�g���N�^
	~Debug();

private:
	// �f�o�b�O
	ID3D12Debug * debug;
};

