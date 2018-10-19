#pragma once
#include <string>
#include <map>
#include <memory>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

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
		//リソース
		ID3D12Resource* rsc;
	};

public:
	// コンストラクタ
	TextureLoader(std::weak_ptr<Device>dev);
	// デストラクタ
	~TextureLoader();

	// 読み込み
	long Load(const std::string& fileName, ID3D12Resource* rsc, DirectX::TexMetadata* meta, DirectX::ScratchImage* img);

private:
	// 定数リソースの生成
	long CreateRsc(const std::string& fileName);


	// デバイス
	std::weak_ptr<Device>dev;

	// 画像データ
	std::map<std::string, Origin> origin;
};
