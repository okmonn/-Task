#pragma once
class Interface
{
public:
	// �R���X�g���N�^
	Interface();
	// �f�X�g���N�^
	virtual ~Interface();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	int top;
	int bottom;
};

