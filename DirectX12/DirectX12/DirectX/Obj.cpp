#include "Obj.h"
#include <tchar.h>

// �R���X�g���N�^
Obj::Obj() : result(S_OK)
{
}

// �f�X�g���N�^
Obj::~Obj()
{
}

// �f�o�b�O�p�o��
void Obj::OutDebug(const std::tstring tex, HRESULT result)
{
#ifdef _DEBUG
	if (FAILED(result))
	{
		OutputDebugString(tex.c_str());
	}
#endif
}
