#pragma once
#include <string>
#include <map>
#include <memory>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;

class TextureLoader
{
	// 画像のオリジンデータ
	struct Origin {
		//メタデータ
		DirectX::TexMetadata* meta;
		//スクラッチイメージ
		DirectX::ScratchImage* img;
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//定数リソース
		ID3D12Resource* c_rsc;
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
		return origin[fileName].c_rsc;
	}
	// メタデータの取得
	DirectX::TexMetadata* GetMeta(const std::string& fileName) {
		return origin[fileName].meta;
	}
	// スクラッチイメージの取得
	DirectX::ScratchImage* GetImg(const std::string& fileName) {
		return origin[fileName].img;
	}

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
