#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
	// �T�E���h
	struct Snd {
		//�\�[�X�{�C�X
		IXAudio2SourceVoice* voice;
		//�g�`�f�[�^
		std::map<unsigned int, std::vector<float>>*data;
		//�ǂݍ��ݔԍ�
		int index;
		//���[�v�t���O
		bool loop;
		//�X���b�h
		std::thread th;
	};

public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	long Load(const std::string& fileName, int& i);

	// �Đ�
	long Play(int& i, const bool& loop = false);

	// ��~
	long Stop(int& i);

	// �ŏ��̈ʒu�Ƀ��Z�b�g
	void ZeroPos(int& i);

private:
	// �\�[�X�{�C�X�̐���
	long Create(int* i, const int& channel, const int& sample, const int& bit);

	// �񓯊�����
	void Stream(int* i);


	// XAudio2
	XAudio2& audio;

	// �T�E���h���[�_�[
	std::shared_ptr<SoundLoader>loader;

	// �X���b�h�t���O
	bool threadFlag;

	// �g�`�f�[�^
	std::map<int*, Snd>snd;
};
