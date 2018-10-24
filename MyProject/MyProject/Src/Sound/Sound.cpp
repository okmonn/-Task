#include "Sound.h"
#include "SoundLoader.h"
#include "../XAudio2/XAudio2.h"
#include "../XAudio2/VoiceCallback.h"

// �R���X�g���N�^
Sound::Sound() : 
	audio(XAudio2::Get()), loader(std::make_shared<SoundLoader>())
{
	snd.clear();
}

// �f�X�g���N�^
Sound::~Sound()
{
	loader.reset();
}

// �\�[�X�{�C�X�̐���
long Sound::Create(int* i, const int channel, const int sample, const int bit)
{
	return 0;
}

// �ǂݍ���
void Sound::Load(const std::string & fileName, int& i)
{
	if (loader->Load(fileName) != 0)
	{
		return;
	}

	if (FAILED(Create(&i, loader->GetChannel(fileName), loader->GetSample(fileName), loader->GetBit(fileName))))
	{
		return;
	}

	snd[&i].wave = loader->GetData(fileName);
}
