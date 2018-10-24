#pragma once
#include <string>
#include <vector>
#include <map>
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
	};

public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

private:
	// �\�[�X�{�C�X�̐���
	long Create(int* i, const int channel, const int sample, const int bit);


	// XAudio2
	XAudio2& audio;

	// �T�E���h���[�_�[
	std::shared_ptr<SoundLoader>loader;

	// �T�E���h�f�[�^
	std::map<int*, Snd>snd;
};
