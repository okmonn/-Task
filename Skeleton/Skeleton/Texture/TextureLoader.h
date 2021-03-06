#pragma once
#include <string>
#include <memory>
#include <unordered_map>

struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class Device;

class TextureLoader
{
	// 元データ
	struct Origin {
		//リソース
		ID3D12Resource* rsc;
		//頂点リソース
		ID3D12Resource* vRsc;
		//デコード
		std::unique_ptr<unsigned char[]>decode;
		//サブデータ
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};

public:
	// デストラクタ
	~TextureLoader();

	// インスタンス変数の取得
	static TextureLoader& Get(void) {
		static TextureLoader instance;
		return instance;
	}

	// 読み込み
	long Load(std::weak_ptr<Device>dev, const std::string& fileName);

	// 画像の横幅の取得
	unsigned int GetWidth(const std::string& fileName);

	// 画像の横幅の取得
	unsigned int GetHeight(const std::string& fileName);

	// データの削除
	void Delete(const std::string& fileName);

	// リソースの取得
	ID3D12Resource* GetRsc(const std::string& fileName) {
		return origin[fileName].rsc;
	}
	// デコードの取得
	unsigned char* GetDecode(const std::string& fileName) {
		return origin[fileName].decode.get();
	}
	// サブデータの取得
	std::shared_ptr<D3D12_SUBRESOURCE_DATA>GetSub(const std::string& fileName) {
		return origin[fileName].sub;
	}

private:
	// コンストラクタ
	TextureLoader();
	TextureLoader(const TextureLoader&)    = delete;
	void operator=(const TextureLoader&) = delete;


	// オリジンデータ
	std::unordered_map<std::string, Origin>origin;
};
