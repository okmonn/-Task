#pragma once

struct ID3D12Debug;

class Debug
{
	// �R���X�g���N�^
	Debug();
	// �f�X�g���N�^
	~Debug();

private:
	// �f�o�b�K�̐���
	long CreateDebug(void);


	// �f�o�b�O
	ID3D12Debug* debug;
};
