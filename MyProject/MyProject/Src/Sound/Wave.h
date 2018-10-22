#pragma once
#include <string>
#include <vector>
#include <map>
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
	// �f�X�g���N�^
	~Wave();

	// �ǂݍ���
	int Load(const std::string& fileName);

private:
	// �\�[�X�{�C�X�̐���
	long Create(void);

	// �g�`�����ׂēǂݍ��ݏI����ĂȂ��ꍇ�̏���
	void Loading(void);

	// �g�`�����ׂēǂݍ��ݏI����Ă���ꍇ�̏���
	void Loaded(void);

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

	// �g�`�̊J�n�ʒu
	int start;

	// �ǂݍ��ݔz��̃C���f�b�N�X
	int index;

	// �g�`�f�[�^
	std::map<int, std::vector<float>>data;

	// �֐��|�C���^
	void (Wave::*func)(void);
};
