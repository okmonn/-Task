#pragma once
#include <string>
#include <map>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;

class Device;

class TextureLoader
{
	// 画像のオリジンデータ
	struct Origin {
		//デコードデータ
		std::unique_ptr<unsigned char[]>decode;
		//サブデータ
		D3D12_SUBRESOURCE_DATA* sub;
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//定数リソース
		ID3D12Resource* rsc;
	};

public:
	// コンストラクタ
	TextureLoader(std::weak_ptr<Device>dev);
	// デストラクタ
	~TextureLoader();

	// 読み込み
	long Load(const std::string& fileName);

	// 定数ヒープの取得
	ID3D12DescriptorHeap* GetHeap(const std::string& fileName) {
		return origin[fileName].heap;
	}
	// 定数リソースの取得
	ID3D12Resource* GetConRsc(const std::string& fileName) {
		return origin[fileName].rsc;
	}
	// デコードデータの取得
	unsigned char* GetDecode(const std::string& fileName) {
		return origin[fileName].decode.get();
	}
	// サブデータの取得
	D3D12_SUBRESOURCE_DATA* GetSub(const std::string& fileName) {
		return origin[fileName].sub;
	}
	//// メタデータの取得
	//DirectX::TexMetadata* GetMeta(const std::string& fileName) {
	//	return origin[fileName].meta;
	//}
	//// スクラッチイメージの取得
	//DirectX::ScratchImage* GetImg(const std::string& fileName) {
	//	return origin[fileName].img;
	//}

private:
	// ヒープの生成
	long CreateHeap(const std::string& fileName);

	// 定数リソースの生成
	long CreateRsc(const std::string& fileName);


	// デバイス
	std::weak_ptr<Device>dev;

	// 画像データ
	std::map<std::string, Origin> origin;
};
