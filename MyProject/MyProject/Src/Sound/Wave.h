#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <memory>

struct _iobuf;
typedef _iobuf FILE;

struct IXAudio2SourceVoice;

class XAudio2;
class VoiceCallback;

class Wave
{
public:
	// �R���X�g���N�^
	Wave();
	Wave(const std::string& fileName);
	// �f�X�g���N�^
	~Wave();

	// ���
	void operator=(const Wave& w);

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// �g�`�f�[�^�̍폜
	void Delete(void);

	// �`�����l�����̎擾
	int GetChannel(void) const {
		return channel;
	}
	// �T���v�����O���g���̎擾
	int GetSample(void) const {
		return sample;
	}
	// �ʎq���r�b�g���̎擾
	int GetBit(void) const {
		return bit;
	}
	// �r�b�g���[�g�̎擾
	int GetByte(void) const {
		return byte;
	}
	// �g�`�f�[�^���I���ɓ��B�������m�F
	bool GetArrival(void) const {
		return arrival;
	}

private:
	// �\�[�X�{�C�X�̐���
	long Create(void);

	// �g�`�����ׂēǂݍ��ݏI����ĂȂ��ꍇ�̏���
	void Loading(void);

	// �g�`�����ׂēǂݍ��ݏI����Ă���ꍇ�̏���
	void Loaded(void);

	// �񓯊�
	void Stream(void);


	// XAudio2
	XAudio2& audio;

	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �t�@�C��
	FILE* file;

	// �`�����l����
	int channel;

	// �T���v�����O���g��
	int sample;

	// �ʎq���r�b�g��
	int bit;

	// �r�b�g���[�g
	int byte;

	// �ǂݍ��ݔz��̃C���f�b�N�X
	unsigned int index;

	// �g�`�f�[�^���I���ɓ��B�������̃t���O
	bool arrival;

	// �X���b�h�t���O
	bool threadFlag;

	// �g�`�f�[�^
	std::map<int, std::vector<float>>data;

	// �X���b�h
	std::thread th;

	// �֐��|�C���^
	void (Wave::*func)(void);
};
