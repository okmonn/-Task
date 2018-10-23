#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>

struct _iobuf;
typedef _iobuf FILE;

class SoundLoader
{
	// �g�`���
	struct Sound {
		//�t�@�C��
		FILE* file;
		//�`�����l��
		int channel;
		//�T���v�����O���g��
		int sample;
		//�r�b�g
		int bit;
		//�r�b�g���[�g
		int byte;
		//�g�`�f�[�^
		std::map<unsigned int, std::vector<float>>data;
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
		return sound[fileName].channel;
	}
	// �T���v�����O���g��
	int GetSample(const std::string& fileName) {
		return sound[fileName].sample;
	}
	// �ʎq���r�b�g���̎擾
	int GetBit(const std::string& fileName) {
		return sound[fileName].bit;
	}
	// �r�b�g���[�g�̎擾
	int GetByte(const std::string& fileName) {
		return sound[fileName].byte;
	}
	// �g�`�f�[�^�̎擾
	std::map<unsigned int, std::vector<float>>* GetData(const std::string& fileName) {
		return &sound[fileName].data;
	}

private:
	// �g�`�̓ǂݍ���
	void LoadWave(const std::string& fileName);


	// �X���b�h�t���O
	bool threadFlag;

	// �g�`���
	std::map<std::string, Sound>sound;

	// �ǂݍ��݃X���b�h
	std::vector<std::thread>th;
};
