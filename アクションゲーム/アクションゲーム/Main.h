#pragma once

#ifdef EXPORT_MAIN
#define EXTERN_MAIN
#else
#define EXTERN_MAIN extern
#endif

// �V�X�e���̏�����
bool System(void);

// �`��
void Draw(void);

// ����
void UpData(void);

// �I������
void Destroy(void);