#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>

class SoundLoader
{
	// �T�E���h�f�[�^
	struct Data {
		//�`�����l����
		int channel;
		//�T���v�����O���g��
		int sample;
		//�ʎq���r�b�g��
		int bit;
		//�r�b�g���[�g
		int byte;
		//�t�@�C��
		FILE* file;
		//�g�`�f�[�^
		std::shared_ptr<std::map<int, std::vector<float>>>wave;
	};

public:
	// �R���X�g���N�^
	SoundLoader();
	// �f�X�g���N�^
	~SoundLoader();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �`�����l�����̎擾
	int GetChannel(const std::string& fileName) {
		return snd[fileName].channel;
	}
	// �T���v�����O���g���̎擾
	int GetSample(const std::string& fileName) {
		return snd[fileName].sample;
	}
	// �ʎq���r�b�g���̎擾
	int GetBit(const std::string& fileName) {
		return snd[fileName].bit;
	}
	// �r�b�g���[�g�̎擾
	int GetByte(const std::string& fileName) {
		return snd[fileName].byte;
	}
	// �g�`�f�[�^�̎擾
	std::shared_ptr<std::map<int, std::vector<float>>> GetData(const std::string& fileName) {
		return snd[fileName].wave;
	}

private:
	// �g�`�f�[�^�̓ǂݍ���
	void LoadWave(const std::string& fileName);


	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::vector<std::thread>th;

	// �T�E���h�f�[�^
	std::map<std::string, Data>snd;
};
