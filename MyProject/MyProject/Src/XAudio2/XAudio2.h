#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// �f�X�g���N�^
	~XAudio2();

	// �C���X�^���X�ϐ��̎擾
	static XAudio2& Get(void) {
		static XAudio2 instance;
		return instance;
	}

	// XAudio2�̎擾
	IXAudio2* GetAudio(void) const {
		return audio;
	}
	// �}�X�^�����O�{�C�X
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// �R���X�g���N�^
	XAudio2();
	XAudio2(const XAudio2&) {
	}
	void operator=(const XAudio2&) {
	}

	// COM�̏�����
	long InitCom(void);

	// XAudio2�̐���
	long CreateAudio(void);

	// �}�X�^�����O�{�C�X�̐���
	long CreateMastering(void);

	// ������
	void Init(void);


	// XAudio2
	IXAudio2* audio;

	// �}�X�^�����O�{�C�X
	IXAudio2MasteringVoice* mastering;
};
