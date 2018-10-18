#pragma once
#include <Windows.h>

class Window 
{
public:
	// �R���X�g���N�^
	Window(const unsigned int& x, const unsigned int& y);
	// �f�X�g���N�^
	~Window();

	// �E�B���h�E�n���h���̎擾
	HWND Get(void) const {
		return handle;
	}

private:
	// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// �E�B���h�E�̐ݒ�
	void SetWindow(void);

	// �E�B���h�E�̍쐬
	void Create(const unsigned int& x, const unsigned int& y);


	// �E�B���h�E�n���h��
	HWND handle;

	// �E�B���h�E�\����
	WNDCLASSEX wnd;
};
