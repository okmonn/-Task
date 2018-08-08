#define _USE_MATH_DEFINES
#include "WAVE.h"
#include <iostream>
#include <tchar.h>
#include <math.h>

// �`�����NID�T�C�Y�̍ő�
#define ID_MAX 4

// �I�[�o�[�t���[�̖h�~
#define OVERFLLOW 32768.0

// �o�C�g�ϊ�
#define BYTE(X) (X) / 8

// Hz�ϊ�
#define Hz(kHz) (kHz) * 1000

// kHz�ϊ�
#define kHz(Hz) (Hz) / 1000

// �~����
#define PI 3.14159264

// RIFF�`�����N
struct RIFF {
	//�`�����NID
	unsigned char chunkID[ID_MAX];
	//�`�����N�T�C�Y
	unsigned long chunkSize;
	//�t�H�[�}�b�g�^�C�v
	unsigned char chunkFormatType[ID_MAX];
};

// FMT�`�����N
struct FMT {
	//�`�����NID
	unsigned char chunkID[ID_MAX];
	//�`�����N�T�C�Y
	unsigned long chunkSize;
	//�t�H�[�}�b�g�^�C�v
	unsigned short waveFormatType;
	//�t�H�[�}�b�g�`�����l��
	unsigned short formatChannel;
	//�T���v�����O���g��
	unsigned long samplesPerSec;
	//�r�b�g���[�g
	unsigned long bytesPerSec;
	//�u���b�N�T�C�Y
	unsigned short blockSize;
	//�ʎq���r�b�g��
	unsigned short bitsPerSample;
};

// DATA�`�����N
struct DATA {
	//�`�����NID
	std::string chunkID;
	//�`�����N�T�C�Y
	unsigned long chunkSize;
};

// 8�r�b�g�X�e���I�f�[�^
struct Sample8 {
	//��
	unsigned char left;
	//�E
	unsigned char right;
};

// 16�r�b�g�X�e���I�f�[�^
struct Sample16 {
	//��
	short left;
	//�E
	short right;
};

// �R���X�g���N�^
WAVE::WAVE()
{
}

// �f�X�g���N�^
WAVE::~WAVE()
{
}

// ���
void WAVE::operator=(const WAVE & wave)
{
	channel  = wave.channel;
	sample   = wave.sample;
	bit      = wave.bit;
	length   = wave.length;
	monaural = wave.monaural;
	stereo   = wave.stereo;
}

// �����m�F
int sound::CheckChar(unsigned char * data, int dataSize, const std::string & find)
{
	for (unsigned int i = 0; i < find.size(); ++i)
	{
		for (int d = 0; d < dataSize; ++d)
		{
			if (data[d] == find[i])
			{
				break;
			}
			else
			{
				//�����m�F���s
				if (d >= dataSize - 1)
				{
					return -1;
				}
			}
		}
	}

	return 0;
}

// �������
void sound::SetChar(unsigned char * data, const std::string & set)
{
	for (unsigned int i = 0; i < set.size(); ++i)
	{
		data[i] = set[i];
	}
}

// ���m�����E8�r�b�g
void sound::LoadMonaural8(WAVE & wave, FILE * file)
{
	wave.monaural.resize(wave.length);

	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		wave.monaural[i] = static_cast<double>(data);
	}
}

// ���m�����E16�r�b�g
void sound::LoadMonaural16(WAVE & wave, FILE * file)
{
	wave.monaural.resize(wave.length);

	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		/* ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������ */
		wave.monaural[i] = static_cast<double>(data) / OVERFLLOW;
	}
}

// �X�e���I�E8�r�b�g
void sound::LoadStereo8(WAVE & wave, FILE * file)
{
	wave.stereo.resize(wave.channel);
	for (auto& w : wave.stereo)
	{
		w.resize(wave.length);
	}

	Sample8 data = {};
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		wave.stereo[0][i] = static_cast<double>(data.left);
		wave.stereo[1][i] = static_cast<double>(data.right);
	}
}

// �X�e���I�E16�r�b�g
void sound::LoadStereo16(WAVE & wave, FILE * file)
{
	wave.stereo.resize(wave.channel);
	for (auto& w : wave.stereo)
	{
		w.resize(wave.length);
	}

	Sample16 data = {};
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		/* ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������ */
		wave.stereo[0][i] = static_cast<double>(data.left) / OVERFLLOW;
		wave.stereo[1][i] = static_cast<double>(data.right) / OVERFLLOW;
	}
}

// WAVE�̓ǂݍ���
int sound::LoadWave(const std::string & fileName, WAVE & wave)
{
	FILE* file = nullptr;

	//�t�@�C���I�[�v��
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	//�`�����N�錾
	RIFF riff = {};
	FMT fmt = {};
	DATA data = {};

	//RIFF�ǂݍ���
	fread(&riff.chunkID[0], sizeof(riff.chunkID), 1, file);
	if (CheckChar(riff.chunkID, ID_MAX, "RIFF") != 0)
	{
		fclose(file);
		return -1;
	}
	fread(&riff.chunkSize, sizeof(riff.chunkSize), 1, file);
	fread(&riff.chunkFormatType[0], sizeof(riff.chunkFormatType), 1, file);
	if (CheckChar(riff.chunkFormatType, ID_MAX, "WAVE") != 0)
	{
		fclose(file);
		return -1;
	}

	//FMT�ǂݍ���
	fread(&fmt.chunkID[0], sizeof(fmt.chunkID), 1, file);
	if (CheckChar(fmt.chunkID, ID_MAX, "fmt ") != 0)
	{
		fclose(file);
		return -1;
	}
	fread(&fmt.chunkSize, sizeof(fmt.chunkSize), 1, file);
	fread(&fmt.waveFormatType, sizeof(fmt.waveFormatType), 1, file);
	fread(&fmt.formatChannel, sizeof(fmt.formatChannel), 1, file);
	fread(&fmt.samplesPerSec, sizeof(fmt.samplesPerSec), 1, file);
	fread(&fmt.bytesPerSec, sizeof(fmt.bytesPerSec), 1, file);
	fread(&fmt.blockSize, sizeof(fmt.blockSize), 1, file);
	fread(&fmt.bitsPerSample, sizeof(fmt.bitsPerSample), 1, file);
	//�g������
	std::vector<unsigned char>extended;
	extended.resize(fmt.chunkSize - (sizeof(fmt) - sizeof(fmt.chunkID) - sizeof(fmt.chunkSize)));
	if (extended.size() > 0)
	{
		fread(&extended[0], sizeof(unsigned char) * extended.size(), 1, file);
	}

	//�_�~�[�錾
	std::string chunkID;
	chunkID.resize(sizeof(unsigned char) * ID_MAX);
	//ID����
	fread(&chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);

	//DATA�ȊO�̏ꍇ
	while (chunkID != "data")
	{
		//�T�C�Y
		unsigned long size = 0;
		fread(&size, sizeof(size), 1, file);
		//�f�[�^
		std::vector<unsigned char>data;
		data.resize(size);
		fread(&data[0], sizeof(unsigned char) * size, 1, file);
		//ID
		fread(&chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);
	}

	//DATA�ǂݍ���
	data.chunkID = chunkID;
	fread(&data.chunkSize, sizeof(data.chunkSize), 1, file);

	wave.channel = fmt.formatChannel;
	wave.sample = fmt.samplesPerSec;
	wave.bit = fmt.bitsPerSample;
	wave.length = data.chunkSize / fmt.blockSize;

	//���m����
	if (fmt.formatChannel == 1)
	{
		//8�r�b�g
		if (fmt.bitsPerSample == 8)
		{
			LoadMonaural8(wave, file);
		}
		//16�r�b�g
		else if (fmt.bitsPerSample == 16)
		{
			LoadMonaural16(wave, file);
		}
	}
	//�X�e���I
	else if (fmt.formatChannel == 2)
	{
		//8�r�b�g
		if (fmt.bitsPerSample == 8)
		{
			LoadStereo8(wave, file);
		}
		//16�r�b�g
		else if (fmt.bitsPerSample == 16)
		{
			LoadStereo16(wave, file);
		}
	}

	//�t�@�C���N���[�Y
	fclose(file);

	return 0;
}

// ���m�����E8�r�b�g
void sound::WriteMonaural8(WAVE & wave, FILE * file)
{
	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		data = static_cast<unsigned char>(wave.monaural[i]);
		fwrite(&data, sizeof(data), 1, file);
	}
}

// ���m�����E16�r�b�g
void sound::WriteMonaural16(WAVE & wave, FILE * file)
{
	double dummy = 0.0;
	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		dummy = (wave.monaural[i] + 1.0) / static_cast<double>(BYTE(wave.bit)) * OVERFLLOW * 2.0;

		/* �N���b�s���O */
		if (dummy > OVERFLLOW * 2.0 - 1.0)
		{
			dummy = OVERFLLOW * 2.0 - 1.0;
		}
		else if (dummy < 0.0)
		{
			dummy = 0.0;
		}

		data = static_cast<short>(dummy + 0.5) - static_cast<short>(OVERFLLOW);

		fwrite(&data, sizeof(data), 1, file);
	}
}

// �X�e���I�E8�r�b�g
void sound::WriteStereo8(WAVE & wave, FILE * file)
{
	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		for (unsigned int p = 0; p < wave.stereo.size(); ++p)
		{
			data = static_cast<unsigned char>(wave.stereo[p][i]);
			fwrite(&data, sizeof(data), 1, file);
		}
	}
}

// �X�e���I�E16�r�b�g
void sound::WriteStereo16(WAVE & wave, FILE * file)
{
	double dummy = 0.0;
	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		for (unsigned int p = 0; p < wave.stereo.size(); ++p)
		{
			dummy = (wave.stereo[p][i] + 1.0) / static_cast<double>(BYTE(wave.bit)) * OVERFLLOW * 2.0;

			/* �N���b�s���O */
			if (dummy > OVERFLLOW * 2.0 - 1.0)
			{
				dummy = OVERFLLOW * 2.0 - 1.0;
			}
			else if (dummy < 0.0)
			{
				dummy = 0.0;
			}

			data = static_cast<short>(dummy + 0.5) - static_cast<short>(OVERFLLOW);

			fwrite(&data, sizeof(data), 1, file);
		}
	}
}

// WAVE�̏�������
int sound::WriteWave(const std::string & fileName, WAVE & wave)
{
	FILE* file = nullptr;

	//�t�@�C���I�[�v��
	if (fopen_s(&file, fileName.c_str(), "wb") != 0)
	{
		return -1;
	}

	//�`�����N�錾
	RIFF riff = {};
	FMT fmt = {};
	DATA data = {};

	//RIFFF��������
	SetChar(riff.chunkID, "RIFF");
	fwrite(&riff.chunkID[0], sizeof(riff.chunkID), 1, file);
	riff.chunkSize = sizeof(RIFF) + sizeof(FMT) + wave.length * (BYTE(wave.bit) * wave.channel);
	fwrite(&riff.chunkSize, sizeof(riff.chunkSize), 1, file);
	SetChar(riff.chunkFormatType, "WAVE");
	fwrite(&riff.chunkFormatType[0], sizeof(riff.chunkFormatType), 1, file);

	//FMT��������
	SetChar(fmt.chunkID, "fmt ");
	fwrite(&fmt.chunkID[0], sizeof(fmt.chunkID), 1, file);
	fmt.chunkSize = sizeof(FMT) - sizeof(fmt.chunkID) - sizeof(fmt.chunkSize);
	fwrite(&fmt.chunkSize, sizeof(fmt.chunkSize), 1, file);
	fmt.waveFormatType = 1;
	fwrite(&fmt.waveFormatType, sizeof(fmt.waveFormatType), 1, file);
	fmt.formatChannel = wave.channel;
	fwrite(&fmt.formatChannel, sizeof(fmt.formatChannel), 1, file);
	fmt.samplesPerSec = wave.sample;
	fwrite(&fmt.samplesPerSec, sizeof(fmt.samplesPerSec), 1, file);
	fmt.bytesPerSec = wave.sample * BYTE(wave.bit) * wave.channel;
	fwrite(&fmt.bytesPerSec, sizeof(fmt.bytesPerSec), 1, file);
	fmt.blockSize = BYTE(wave.bit) * wave.channel;
	fwrite(&fmt.blockSize, sizeof(fmt.blockSize), 1, file);
	fmt.bitsPerSample = wave.bit;
	fwrite(&fmt.bitsPerSample, sizeof(fmt.bitsPerSample), 1, file);

	//DATA��������
	data.chunkID = "data";
	fwrite(&data.chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);
	data.chunkSize = wave.length * BYTE(fmt.bitsPerSample) * fmt.formatChannel;
	fwrite(&data.chunkSize, sizeof(data.chunkSize), 1, file);

	//���m����
	if (fmt.formatChannel == 1)
	{
		//8�r�b�g
		if (fmt.bitsPerSample == 8)
		{
			WriteMonaural8(wave, file);
		}
		//16�r�b�g
		else if (fmt.bitsPerSample == 16)
		{
			WriteMonaural16(wave, file);
		}
	}
	//�X�e���I
	else if (fmt.formatChannel == 2)
	{
		//8�r�b�g
		if (fmt.bitsPerSample == 8)
		{
			WriteStereo8(wave, file);
		}
		//16�r�b�g
		else if (fmt.bitsPerSample == 16)
		{
			WriteStereo16(wave, file);
		}
	}

	//�t�@�C���N���[�Y
	fclose(file);

	return 0;
}

// �T�C���g
void sound::SineWave(int sample, std::vector<double>&data, const double& amplitude, const double& period)
{
	/* ���� */
	// ��{���g��
	/* 1.0 / (�T���v���� / �T���v�����O���g��) */
	// �T�C���g
	/* �U�� * sin((2.0 * �~���� * ��{���g�� * n) / �T���v�����O���g��)) */

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] = amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i / static_cast<double>(sample));
	}
}

// �T�C���g�E�̂�����g
void sound::SineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* �l���� */
	/*�T�C���g�����Z�i�d�ˍ��킹�āj���Ă���*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			data[i] += amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// �T�C���g�E�Z�`�g
void sound::SineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* �l���� */
	/*�T�C���g����{�̂��̂����Z�i�d�ˍ��킹�āj���Ă���*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			if (n % 2 == 0)
			{
				continue;
			}
			data[i] += amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// �R�T�C���g
void sound::CosineWave(int sample, std::vector<double>& data, const double& amplitude, const double& period)
{
	/* ���� */
	// ��{���g��
	/* 1.0 / (�T���v���� / �T���v�����O���g��) */
	// �R�T�C���g
	/* �U�� * cos((2.0 * �~���� * ��{���g�� * n) / �T���v�����O���g��)) */

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] = amplitude * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i / static_cast<double>(sample));
	}
}

// �R�T�C���g�E�̂�����g
void sound::CosineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* �l���� */
	/*�R�T�C���g�����Z�i�d�ˍ��킹�āj���Ă���*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			data[i] += amplitude / n * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// �R�T�C���g�E�Z�`�g
void sound::CosineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* �l���� */
	/*�R�T�C���g����{�̂��̂����Z�i�d�ˍ��킹�āj���Ă���*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			if (n % 2 == 0)
			{
				continue;
			}
			data[i] += amplitude / n * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// �ΐ�
int sound::Log(int i, int n)
{
	return static_cast<int>(std::log(n) / std::log(i));
}

// �ݏ�
int sound::Pow(int i, int n)
{
	return (n == 0) ? 1 : i << (n - 1);
}

// ���֐�
std::vector<double> sound::HanningWindow(int size)
{
	/*�g�`�ɒޏ���̑��֐�
	�@�g�`�̌J��Ԃ��ɔ����[���̕s�A���_�������܂��ɂ���*/
	std::vector<double>data;
	data.resize(size);

	if (size % 2 == 0)
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] = 0.5 - 0.5 * std::cos(2.0 * M_PI * i / size);
			if (data[i] == 0.0)
			{
				data[i] = 1.0;
			}
		}
	}
	else
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] = 0.5 - 0.5 * std::cos(2.0 * M_PI * (i + 0.5) / size);
			if (data[i] == 0.0)
			{
				data[i] = 1.0;
			}
		}
	}

	return data;
}

// ���U�t�[���G�ϊ�
void sound::DFT(int size, std::vector<double>&real, std::vector<double>&imag)
{
	//����
	double w_r;
	auto o_r = real;
	//����
	double w_i;
	auto o_i = imag;

	//�n�~���O�f�[�^
	std::vector<double>hanning = HanningWindow(size);

	for (int i = 0; i < size; ++i)
	{
		real[i] = 0.0;
		imag[i] = 0.0;
		for (int n = 0; n < size; n++)
		{
			w_r = std::cos(2.0 * M_PI * i * n / size);
			w_i = -(std::sin(2.0 * M_PI * i * n / size));

			//������
			real[i] += w_r * (o_r[n] * hanning[n]) - w_i * o_i[n];
			//������
			imag[i] += w_r * o_i[n] + w_i * (o_r[n] * hanning[n]);
		}
	}
}

// �t���U�t�[���G�ϊ�
void sound::IDFT(int size, std::vector<double>& real, std::vector<double>& imag)
{
	//����
	double w_r;
	auto o_r = real;
	//����
	double w_i;
	auto o_i = imag;

	//�n�~���O�f�[�^
	std::vector<double>hanning = HanningWindow(size);

	for (int i = 0; i < size; ++i)
	{
		real[i] = 0.0;
		imag[i] = 0.0;
		for (int n = 0; n < size; n++)
		{
			w_r = std::cos(2.0 * M_PI * i * n / size);
			w_i = std::sin(2.0 * M_PI * i * n / size);
			//������
			real[i] += (w_r * (o_r[n]) - w_i * o_i[n]) / size;
			//������
			imag[i] += (w_r * o_i[n] + w_i * (o_r[n])) / size;
		}

		real[i] /= hanning[i];
		imag[i] /= hanning[i];
	}
}

// �����t�[���G�ϊ�
void sound::FFT(int size, std::vector<double>& real, std::vector<double>& imag, int log, int pow)
{
	//�z��ԍ�
	int index1 = 0;
	int index2 = 0;

	//�z��e�[�u��
	std::vector<int>indexTbl;
	indexTbl.resize(size);

	//�ݏ�
	int p = 0;

	//�ϊ��f�[�^
	double re[3];
	double im[3];

	//�����t�[���G�̒i��
	int stage = Log(log, size);

	//�o�^�t���C�v�Z
	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < Pow(pow, s - 1); ++i)
		{
			for (int n = 0; n < Pow(pow, stage - s); ++n)
			{
				index1 = Pow(pow, stage - s + 1) * i + n;
				index2 = Pow(pow, stage - s) + index1;

				p = Pow(pow, s - 1) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] = std::cos((2.0 * M_PI * p) / size);
				im[2] = -(std::sin((2.0 * M_PI * p) / size));

				if (s < stage)
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//�C���f�b�N�X���ѕς��悤�̃e�[�u���쐬
			indexTbl[Pow(pow, s - 1) + i] = indexTbl[i] + Pow(pow, stage - s);
		}
	}

	//�C���f�b�N�X�̕��ёւ�
	double dr = 0.0;
	double di = 0.0;
	for (int i = 0; i < size; ++i)
	{
		if (indexTbl[i] > i)
		{
			dr = real[indexTbl[i]];
			di = imag[indexTbl[i]];
			real[indexTbl[i]] = real[i];
			imag[indexTbl[i]] = imag[i];
			real[i] = dr;
			imag[i] = di;
		}
	}
}

// �t�����t�[���G�ϊ�
void sound::IFFT(int size, std::vector<double>& real, std::vector<double>& imag, int log, int pow)
{
	//�z��ԍ�
	int index1 = 0;
	int index2 = 0;

	//�z��e�[�u��
	std::vector<int>indexTbl;
	indexTbl.resize(size);

	//�ݏ�
	int p = 0;

	//�ϊ��f�[�^
	//�ϊ��f�[�^
	double re[3];
	double im[3];

	//�����t�[���G�̒i��
	int stage = Log(log, size);

	//�o�^�t���C�v�Z
	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < Pow(pow, s - 1); ++i)
		{
			for (int n = 0; n < Pow(pow, stage - s); ++n)
			{
				index1 = Pow(pow, stage - s + 1) * i + n;
				index2 = Pow(pow, stage - s) + index1;

				p = Pow(pow, s - 1) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] = std::cos((2.0 * M_PI * p) / size);
				im[2] = std::sin((2.0 * M_PI * p) / size);

				if (s < stage)
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//�C���f�b�N�X���ѕς��悤�̃e�[�u���쐬
			indexTbl[Pow(pow, s - 1) + i] = indexTbl[i] + Pow(pow, stage - s);
		}
	}

	//�C���f�b�N�X�̕��ёւ�
	double dr = 0.0;
	double di = 0.0;
	for (int i = 0; i < size; ++i)
	{
		if (indexTbl[i] > i)
		{
			dr = real[indexTbl[i]];
			di = imag[indexTbl[i]];
			real[indexTbl[i]] = real[i];
			imag[indexTbl[i]] = imag[i];
			real[i] = dr;
			imag[i] = di;
		}

		//�v�Z���ʂ��T�C�Y�Ŋ���
		real[i] /= size;
		imag[i] /= size;
	}
}

// �f�B���C
void sound::Delay(int sample, std::vector<double>&data, double attenuation, double delayTime, int loopCnt, int startPos)
{
	//�K���f�[�^
	std::vector<double>wave;
	wave.resize(data.size() - startPos);

	//���Z�f�[�^
	int d = 0;

	for (unsigned int i = 0; i < wave.size(); ++i)
	{
		wave[i] = data[startPos + i];

		for (int l = 1; l <= loopCnt; ++l)
		{
			d = (int)((double)i - (double)l * ((double)sample * delayTime));
			if (d >= 0)
			{
				wave[i] += std::pow(attenuation, static_cast<double>(l)) * data[startPos + d];
			}
		}

		// ����ւ�
		data[startPos + i] = wave[i];
	}
}

// �f�B�X�g�[�V����
void sound::Distortion(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		//�����̑���
		data[i] *= amplification;

		//�N���b�s���O
		if (data[i] > 1.0)
		{
			data[i] = 1.0;
		}
		else if (data[i] < -1.0)
		{
			data[i] = -1.0;
		}

		//���ʂ̒���
		data[i] *= level;
	}
}

// �I�[�o�[�h���C�u
void sound::OverDrive(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] *= amplification;
		
		//�N���b�s���O
		if (data[i] >= 0.0)
		{
			data[i] = std::atan(data[i]) / (M_PI / 2.0);
		}
		else if (data[i] < -0.0)
		{
			data[i] = std::atan(data[i]) / (M_PI / 2.0) * 0.1;
		}

		//���ʂ̒���
		data[i] *= level;
	}
}

// �t�@�Y
void sound::Fuzz(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		//���f�[�^�̑S�g����
		if (data[i] < 0.0)
		{
			data[i] *= -1.0;
		}

		//�N���b�s���O
		if (data[i] > 1.0)
		{
			data[i] = 1.0;
		}
		else if (data[i] < -1.0)
		{
			data[i] = -1.0;
		}

		//���ʂ̒���
		data[i] *= level;
	}
}

// �f�o�b�O�`��
void sound::Debug(int max, double data)
{
	for (int i = -max; i <= max; ++i)
	{
		if (static_cast<int>(data) < 0)
		{
			if (i >= static_cast<int>(data) && i <= 0)
			{
				printf("-");
			}
			else
			{
				printf(" ");
			}
		}
		else if (static_cast<int>(data) > 0)
		{
			if (i <= static_cast<int>(data) && i >= 0)
			{
				printf("-");
			}
			else
			{
				printf(" ");
			}
		}
		else if (static_cast<int>(data) == 0)
		{
			if (i == 0)
			{
				printf("�E");
			}
			else
			{
				printf(" ");
			}
		}
	}
	printf("\n");
}
