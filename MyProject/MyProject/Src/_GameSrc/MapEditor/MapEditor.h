#pragma once

class MapEditor
{
public:
	// �R���X�g���N�^
	MapEditor();
	// �f�X�g���N�^
	~MapEditor();

	// �`��
	void Draw(void);
	
	// ����
	void UpData(void);

private:
	// �}�b�v�`�b�v�̃C���f�b�N�X
	unsigned int index;
};
