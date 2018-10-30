#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <memory>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
	// �T�E���h�f�[�^
	struct Snd {
		//�\�[�X�{�C�X
		IXAudio2SourceVoice* voice;
		//�g�`�f�[�^
		std::weak_ptr<std::map<int, std::vector<float>>>wave;
		//�z��ԍ�
		unsigned int index;
		//���[�v�t���O
		bool loop;
		//�X���b�h
		std::thread th;
		//�X���b�h�t���O
		bool threadFlag;
	};

public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// �Đ�
	long Play(int& i, const bool& loop);

	// ��~
	long Stop(int& i);

	// �T�E���h�̍폜
	void DeleteSnd(int& i);

private:
	// �\�[�X�{�C�X�̐���
	long Create(int* i, const int channel, const int sample, const int bit);

	// �񓯊�
	void Stream(int* i);


	// XAudio2
	std::unique_ptr<XAudio2>audio;

	// �T�E���h���[�_�[
	std::unique_ptr<SoundLoader>loader;

	// �T�E���h�f�[�^
	std::map<int*, Snd>snd;
};
