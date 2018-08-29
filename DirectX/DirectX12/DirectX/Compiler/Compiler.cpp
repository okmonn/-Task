#include <d3dcompiler.h>
#include "Compiler.h"
#include <tchar.h>

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
Compiler::Compiler() :
	error(nullptr)
{
	vertex.clear();
	pixel.clear();

	Init();
}

// デストラクタ
Compiler::~Compiler()
{
	for (auto itr = vertex.begin(); itr != vertex.end(); ++itr)
	{
		Release(itr->second);
	}
	for (auto itr = pixel.begin(); itr != pixel.end(); ++itr)
	{
		Release(itr->second);
	}
	Release(error);
}

// 頂点シェーダのコンパイル
HRESULT Compiler::ComVertex(const LPCWSTR& fileName, const LPCSTR& func, const LPCSTR& target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertex[fileName], &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\n頂点シェーダのコンパイル：失敗\n"));
	}

	return result;
}

// ピクセルシェーダのコンパイル
HRESULT Compiler::ComPixel(const LPCWSTR& fileName, const LPCSTR& func, const LPCSTR& target)
{
	result = D3DCompileFromFile(fileName, nullptr, nullptr, func, target, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixel[fileName], &error);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nピクセルシェーダのコンパイル：失敗\n"));
	}

	return result;
}

// ファイルを返す
std::wstring Compiler::GetFile(const std::experimental::filesystem::path & p)
{
	std::wstring m;
	//ファイルの場合
	if (std::experimental::filesystem::is_regular_file(p))
	{
		m = p.filename().wstring();
	}
	else if (std::experimental::filesystem::is_directory(p))
	{
		m = p.wstring();
	}

	return m;
}

// ディレクトリのファイル列挙
std::vector<std::wstring> Compiler::GetDirFile(const std::string & point)
{
	//列挙の起点
	std::experimental::filesystem::path p(point.c_str());

	std::vector<std::wstring>fileName;

	for (auto& i : std::experimental::filesystem::recursive_directory_iterator(p))
	{
		fileName.push_back(GetFile(i));
	}

	return fileName;
}

// 文字列検索
std::wstring Compiler::SearchString(const std::wstring& m, const std::wstring & find, bool end)
{
	if (end == false)
	{
		UINT start = m.find_first_of(find);
		auto dummy = m.substr(start + 1, m.size() - start);
		return dummy;
	}
	else if (end == true)
	{
		UINT start = m.find_last_of(find);
		auto dummy = m.substr(start + 1, m.size());
		return dummy;
	}
	
	return std::wstring();
}

// 初期読み込み
HRESULT Compiler::Init(void)
{
	auto file = GetDirFile("Shader");
	for (UINT i = 0; i < file.size(); ++i)
	{
		result = ComVertex(std::wstring(L"Shader/" + file[i]).c_str(), "VS");
		if (FAILED(result))
		{
			break;
		}
		result = ComPixel(std::wstring(L"Shader/" + file[i]).c_str(), "PS");
		if (FAILED(result))
		{
			break;
		}
	}

	return result;
}
