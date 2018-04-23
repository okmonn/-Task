#pragma once

#ifdef EXPORT_MAIN
#define EXTERN_MAIN
#else
#define EXTERN_MAIN extern
#endif

#include "BackGround.h"
#include "Player.h"
#include <memory>

// �V�X�e���̏�����
bool System(void);

// �C���X�^���X��
void Create(void);

// �`��
void Draw(void);

// ����
void UpData(void);

// �I������
void Destroy(void);

// �w�i�N���X
EXTERN_MAIN std::shared_ptr<BackGround>back;

// �v���C���[�N���X
EXTERN_MAIN std::shared_ptr<Player>pl;