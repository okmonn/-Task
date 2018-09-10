#pragma once
#include "../BaseObj/Obj.h"
#include <string>
#include <map>
#include <filesystem>

class Compiler :
	public Obj
{
public:
	// コンストラクタ
	Compiler();
	// デストラクタ
	~Compiler();

	// 頂点シェーダのコンパイル
	HRESULT ComVertex(const LPCWSTR& fileName, const LPCSTR& func, const LPCSTR& target = "vs_5_0");

	// ピクセルシェーダのコンパイル
	HRESULT ComPixel(const LPCWSTR& fileName, const LPCSTR& func, const LPCSTR& target = "ps_5_0");

	// 頂点シェーダの取得
	ID3DBlob* GetVertex(const LPCWSTR& path) {
		return vertex[path];
	}
	// ピクセルシェーダ
	ID3DBlob* GetPixel(const LPCWSTR& path) {
		return pixel[path];
	}

private:
	// ファイルを返す
	std::wstring GetFile(const std::experimental::filesystem::path& p);

	// ディレクトリのファイル列挙
	std::vector<std::wstring> GetDirFile(const std::string& point);

	// 文字列検索
	std::wstring SearchString(const std::wstring& m, const std::wstring & find, bool end = true);

	// 初期読み込み
	HRESULT Init(void);


	// エラー
	ID3DBlob* error;

	// 頂点シェーダ
	std::map<std::wstring, ID3DBlob*>vertex;

	// ピクセルシェーダ
	std::map<std::wstring, ID3DBlob*>pixel;
};

