#include "SoundLoader.h"
#include "SoundFunc.h"
#include "../Func/Func.h"
#include <Windows.h>
#include <tchar.h>

// ����}�N��
#define Close(X) { if((X) != nullptr) fclose((X)); (X) = nullptr; }

// �X���b�h�ő吔
#define THREAD_MAX 10

// �R���X�g���N�^
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);
}

// �f�X�g���N�^
SoundLoader::~SoundLoader()
{
	threadFlag = false;

	for (auto itr = th.begin(); itr != th.end(); ++itr)
	{
		itr->join();
	}

	for (auto itr = sound.begin(); itr != sound.end(); ++itr)
	{
		itr->second.data.clear();
	}
}

// �ǂݍ���
int SoundLoader::Load(const std::string & fileName)
{
	if (sound.find(fileName) == sound.end())
	{
		if (fopen_s(&sound[fileName].file, fileName.c_str(), "rb") != 0)
		{
			OutputDebugString(_T("\n�t�@�C���̎Q�ƁF���s\n"));
			return -1;
		}

		sound::RIFF riff = {};
		sound::FMT fmt   = {};
		sound::DATA data = {};

		//RIFF
		if (sound::LoadRIFF(riff, sound[fileName].file) != 0)
		{
			return -1;
		}

		//FMT
		if (sound::LoadFMT(fmt, sound[fileName].file) != 0)
		{
			return -1;
		}

		//DATA
		if (sound::LoadDATA(data, sound[fileName].file) != 0)
		{
			return -1;
		}

		//�K�v�Ȃ��̂̊i�[
		sound[fileName].channel = fmt.channel;
		sound[fileName].sample  = fmt.sample;
		sound[fileName].bit     = fmt.bit;
		sound[fileName].byte    = fmt.byte;

		for (auto itr = th.begin(); itr != th.end(); ++itr)
		{
			if (itr->joinable() == false)
			{
				*itr = std::thread(&SoundLoader::LoadWave, this, fileName);
				break;
			}
		}
	}

	return 0;
}

// �g�`�̓ǂݍ���
void SoundLoader::LoadWave(const std::string& fileName)
{
	int read = 0;

	while (std::feof(sound[fileName].file) == 0 && threadFlag)
	{
		//�z��̃������m��
		sound[fileName].data[read].resize(sound[fileName].byte / (func::Byte(sound[fileName].bit) * sound[fileName].channel));
		
		switch (sound[fileName].channel)
		{
		case 1:
			sound::LoadMono(sound[fileName].data[read], sound[fileName].bit, sound[fileName].file);
			break;
		case 2:
			sound::LoadStereo(sound[fileName].data[read], sound[fileName].bit, sound[fileName].file);
			break;
		default:
			break;
		}

		//�ǂݍ��݈ʒu�����ɐi�߂�
		++read;
	}

	Close(sound[fileName].file);
}
