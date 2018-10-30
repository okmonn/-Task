#include "Func.h"
#include "../Union/Union.h"
#include <Windows.h>
#include <algorithm>
#include <tchar.h>

// �r�b�g�ϊ�
int func::Bit(const int & i)
{
	return i * 8;
}

// �o�C�g�ϊ�
int func::Byte(const int & i)
{
	return i / 8;
}

// �ΐ�
int func::Log(const int & i, const int & n)
{
	return static_cast<int>(std::log(n) / std::log(i));
}

// �ݏ�
int func::Pow(const int & i, const int & n)
{
	return (n == 0) ? 1 : i << (n - 1);
}

// ������̊m�F
bool func::CheckChar(const std::string & find, unsigned char * data, const unsigned int& dataNum)
{
	if (find.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n�������������񂪃f�[�^���𒴂��Ă��܂�\n"));
		return false;
	}

	unsigned int cnt = 0;
	for (unsigned int i = 0; i < dataNum; ++i)
	{
		if (data[i] == find[cnt])
		{
			if (cnt + 1 < find.size())
			{
				++cnt;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

// ������̑��
void func::AddChar(const std::string & add, unsigned char * data, const unsigned int& dataNum)
{
	if (add.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n��������������񂪃f�[�^���𒴂��Ă��܂�\n"));
		return;
	}

	for (unsigned int i = 0; i < dataNum; ++i)
	{
		data[i] = add[i];
	}
}

// ������̌���
std::string func::FindString(const std::string & path, const char & find, const unsigned int & offset, const bool & end)
{
	unsigned int pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// �t�H���_���ƃt�@�C�����̘A��
std::string func::FileLink(const std::string & path1, const std::string & path2)
{
	int index1 = path1.rfind('/');
	int index2 = path1.rfind('\\');
	int index = max(index1, index2);

	std::string folder = path1.substr(0, index) + "/" + path2;

	return folder;
}

// ���C�h������̕ϊ�
std::wstring func::ChangeWString(const std::string & st)
{
	//�������̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// �j���[�g���@
float func::Newton(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop)
{
	if (pos1X== pos1Y && pos2X == pos2Y)
	{
		//�����Ȃ̂Ōv�Z�s�v
		return input;
	}

	float t = input;
	//t^3�̌W��
	float k3 = 1 + (3 * pos1X) - (3 * pos2X);
	//t^2�̌W��
	float k2 = (3 * pos2X) - (6 * pos1X);
	//t�̌W��
	float k1 = 3 * pos1X;

	const float epsilon = 0.0005f;

	for (unsigned int i = 0; i < loop; ++i)
	{
		//f(x)
		float ft = (t * t * t * k3) + (t * t * k2) + (t * k1) - input;
		if (ft <= epsilon && ft >= -epsilon)
		{
			break;
		}
		//f(x)�̔�������
		float fdt = (3 * t * t * k3) + (2 * t * k2) + k1;
		if (fdt == 0.0f)
		{
			break;
		}
		t = t - ft / fdt;
	}

	//���]
	float reverse = (1.0f - t);

	return (3 * reverse * reverse * t * pos1Y) +
		(3 * reverse * t * t * pos2Y) +
		(t * t * t);
}

// 2���@
float func::Bisection(const float & input, const float & pos1X, const float & pos1Y, const float & pos2X, const float & pos2Y, const unsigned int & loop)
{
	if (pos1X == pos1Y && pos2X == pos2Y)
	{
		//�����Ȃ̂Ōv�Z�s�v
		return input;
	}

	float t = input;
	float reverse = 1.0f - t;

	const float epsilon = 0.0005f;

	float ft = 0.0f;
	for (unsigned int i = 0; i < loop; ++i)
	{
		ft = (3 * reverse * reverse * t * pos1X) +
			(3 * reverse * t * t * pos2X) +
			(t * t * t) - input;

		if (ft <= epsilon && ft >= -epsilon)
		{
			break;
		}

		t -= ft / 2.0f;
	}
	reverse = 1.0f - t;

	return (3 * reverse * reverse * t * pos1Y) +
		(3 * reverse * t * t * pos2Y) +
		(t * t * t);
}

// �E�B���h�E�T�C�Y�̃Z�b�g
void func::SetWindowSize(const unsigned int & x, const unsigned int & y)
{
	Union::Get().SetWinSize(x, y);
}

// �������E�X�^�[�g
void func::Start(void)
{
	Union::Get().Start();
}

// WVP�̍X�V
void func::ChangeWVP(const float & eyeX, const float & eyeY, const float & eyeZ, const float & targetX, const float & targetY, const float & targetZ, const float & upX, const float & upY, const float & upZ)
{
	Union::Get().ChangeWVP(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);
}

// ���b�Z�[�W�̊m�F
bool func::CheckMsg(void)
{
	return Union::Get().CheckMsg();
}

// �L�[����
bool func::CheckKey(const int & i)
{
	return Union::Get().CheckKey(i);
}

// �g���K�[����
bool func::CheckTriger(const int & i)
{
	return Union::Get().CheckTriger(i);
}

// �T�E���h�̓ǂݍ���
void func::LoadSnd(const std::string & fileName, int & i)
{
	Union::Get().LoadSnd(fileName, i);
}

// �T�E���h�̍Đ�
void func::Play(int & i, const bool& loop)
{
	Union::Get().Play(i, loop);
}

// �T�E���h�̒�~
void func::Stop(int & i)
{
	Union::Get().Stop(i);
}

// �摜�̓ǂݍ���
void func::LoadImg(const std::string & fileName, int & i)
{
	Union::Get().LoadImg(fileName, i);
}

// PMD�̓ǂݍ���
void func::LoadPmd(const std::string & fileName, int & i)
{
	Union::Get().LoadPmd(fileName, i);
}

// �A�j���[�V�����̃A�^�b�`
void func::Attach(const std::string & fileName, int & i)
{

	Union::Get().Attach(fileName, i);
}

// �`�揀��
void func::Set(void)
{
	Union::Get().Set();
}

// �|�C���g�`��
void func::DrawPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	Union::Get().DrawPoint(x, y, r, g, b, alpha);
}

// ���C���`��
void func::DrawLine(const float & x1, const float & y1, const float & x2, const float & y2, const float & r, const float & g, const float & b, const float & alpha)
{
	Union::Get().DrawLine(x1, y1, x2, y2, r, g, b, alpha);
}

// �g���C�A���O���`��
void func::DrawTriangle(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	Union::Get().DrawTriangle(x1, y1, x2, y2, x3, y3, r, g, b, alpha);
}

// �{�b�N�X�`��
void func::DrawBox(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	Union::Get().DrawTriangle(x1, y1, x2, y2, x3, y3, r, g, b, alpha);
	Union::Get().DrawTriangle(x2, y2, x3, y3, x4, y4, r, g, b, alpha);
}

// �摜�̕`��
void func::DrawImg(int & i, const float & x, const float & y, const float & alpha, const int & turnX, const int & turnY)
{
	Union::Get().DrawImg(i, x, y, alpha, turnX, turnY);
}

// �摜�̕`��E�T�C�Y�w��
void func::DrawSizeImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, const float & alpha, const int & turnX, const int & turnY)
{
	Union::Get().DrawSizeImg(i, x, y, sizeX, sizeY, alpha, turnX, turnY);
}

// �摜�̕`��E�T�C�Y�E�͈͎w��
void func::DrawRectImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, 
	const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	Union::Get().DrawRectImg(i, x, y, sizeX, sizeY, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// �摜�̕`��E4�_�w��
void func::DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, 
	const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY)
{
	Union::Get().DrawFreelyImg(i, x1, y1, x2, y2, x3, y3, x4, y4, alpha, turnX, turnY);
}

// �摜�̕`��E4�_�E�͈͎w��
void func::DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, 
	const float & x4, const float & y4, const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	Union::Get().DrawFreelyRectImg(i, x1, y1, x2, y2, x3, y3, x4, y4, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// PMD�̕`��
void func::DrawPmd(int & i)
{
	Union::Get().DrawPmd(i);
}

// �A�j���[�V����
void func::Animation(int & i, const float & animSpeed)
{
	Union::Get().Animation(i, animSpeed);
}

// �`����s
void func::Do(void)
{
	Union::Get().Do();
}

// �I������
void func::End(void)
{
	Union::Get().End();
}
