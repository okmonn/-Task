#pragma once
#include <vector>
#include <string>

class WAVE
{
public:
	// �R���X�g���N�^
	WAVE();
	// �f�X�g���N�^
	~WAVE();

	// ���
	void operator=(const WAVE& wave);


	// �`�����l����
	int channel;

	// �T���v�����O���g��
	int sample;

	// �ʎq���r�b�g��
	int bit;

	// ����
	int length;

	// ���y�f�[�^
	//std::vector<double>data;

	// ���y�f�[�^�E���m����
	std::vector<double>monaural;

	// ���y�f�[�^�E�X�e���I
	std::vector <std::vector<double>>stereo;
};

// ���O��Ԏw��
namespace sound {
	// �����m�F
	int CheckChar(unsigned char* data, int dataSize, const std::string& find);
	// �������
	void SetChar(unsigned char* data, const std::string& set);

	// ���m�����E8�r�b�g
	void LoadMonaural8(WAVE& wave, FILE* file);
	// ���m�����E16�r�b�g
	void LoadMonaural16(WAVE& wave, FILE* file);
	// �X�e���I�E8�r�b�g
	void LoadStereo8(WAVE& wave, FILE* file);
	// �X�e���I�E16�r�b�g
	void LoadStereo16(WAVE& wave, FILE* file);
	// WAVE�̓ǂݍ���
	int LoadWave(const std::string& fileName, WAVE& wave);

	// ���m�����E8�r�b�g
	void WriteMonaural8(WAVE& wave, FILE* file);
	// ���m�����E16�r�b�g
	void WriteMonaural16(WAVE& wave, FILE* file);
	// �X�e���I�E8�r�b�g
	void WriteStereo8(WAVE& wave, FILE* file);
	// �X�e���I�E16�r�b�g
	void WriteStereo16(WAVE& wave, FILE* file);
	// WAVE�̏�������
	int WriteWave(const std::string& fileName, WAVE& wave);

	// �T�C���g
	void SineWave(int sample, std::vector<double>&data, const double& amplitude, const double& period);
	// �T�C���g�E�̂�����g
	void SineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);
	// �T�C���g�E�Z�`�g
	void SineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);

	// �R�T�C���g
	void CosineWave(int sample, std::vector<double>& data, const double& amplitude, const double& period);
	// �R�T�C���g�E�̂�����g
	void CosineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);
	// �R�T�C���g�E�Z�`�g
	void CosineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);

	// �ΐ�
	int Log(int i, int n);
	// �ݏ�
	int Pow(int i, int n);
	
	// ���֐�
	std::vector<double> HanningWindow(int size);
	// ���U�t�[���G�ϊ�
	void DFT(int size, std::vector<double>&real, std::vector<double>&imag);
	// �t���U�t�[���G�ϊ�
	void IDFT(int size, std::vector<double>&real, std::vector<double>&imag);
	/* �����t�[���G�ϊ��Esize��2�̗ݏ�Ŏw�� */
	void FFT(int size, std::vector<double>&real, std::vector<double>&imag, int log = 2, int pow = 2);
	/* �t�����t�[���G�ϊ��Esize��2�̗ݏ�Ŏw�� */
	void IFFT(int size, std::vector<double>&real, std::vector<double>&imag, int log = 2, int pow = 2);

	/* �f�B���C�E�x�����Ԃ�Z�����ČJ��Ԃ��𑽂�����ƃ��o�[�u���� */
	void Delay(int sample, std::vector<double>&data, double attenuation, double delayTime, int loopCnt = 2, int startPos = 0);

	// �f�B�X�g�[�V����
	void Distortion(std::vector<double>& data, double amplification, double level = 0.5);

	// �I�[�o�[�h���C�u
	void OverDrive(std::vector<double>& data, double amplification, double level = 0.5);

	// �t�@�Y
	void Fuzz(std::vector<double>&data, double amplification, double level = 0.5);

	// �f�o�b�O�`��
	void Debug(int max, double data);
}