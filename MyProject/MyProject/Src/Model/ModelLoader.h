#pragma once
#include "PmdData.h"
#include <memory>

class Device;

class ModelLoader
{
public:
	// コンストラクタ
	ModelLoader(std::weak_ptr<Device>dev);
	// デストラクタ
	~ModelLoader();

	// 読み込み
	long Load(const std::string& fileName);

	// 頂点の取得
	std::shared_ptr<std::vector<pmd::Vertex>> GetVertex(const std::string& fileName) {
		return vertex[fileName];
	}
	// インデックスの取得
	std::shared_ptr<std::vector<unsigned short>> GetIndex(const std::string& fileName) {
		return index[fileName];
	}
	// マテリアルの取得
	std::shared_ptr<std::vector<pmd::Material>> GetMaterial(const std::string& fileName) {
		return material[fileName];
	}
	// ボーンの取得
	std::shared_ptr<std::vector<pmd::Born>> GetBorn(const std::string& fileName) {
		return born[fileName];
	}
	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(const std::string& fileName) {
		return heap[fileName];
	}
	// マテリアル用リソースの取得
	ID3D12Resource* GetMaterialRsc(const std::string& fileName) {
		return c_rsc[fileName];
	}
	// ボーン用リソースの取得
	ID3D12Resource* GetBornRsc(const std::string& fileName) {
		return b_rsc[fileName];
	}
	// インデックス用リソースの取得
	ID3D12Resource* GetIndexRsc(const std::string& fileName) {
		return i_rsc[fileName];
	}

private:
	// ヒープの生成
	long CreateHeap(const std::string& fileName);

	// 定数リソースの生成
	long CreateConRsc(ID3D12Resource** rsc, const int& w);

	// インデックスの生成
	long CreateIndex(const std::string& fileName);


	// デバイス
	std::weak_ptr<Device>dev;

	// 頂点
	std::map<std::string, std::shared_ptr<std::vector<pmd::Vertex>>>vertex;

	// インデックス
	std::map<std::string, std::shared_ptr<std::vector<unsigned short>>>index;

	// マテリアル
	std::map<std::string, std::shared_ptr<std::vector<pmd::Material>>>material;

	// ボーン
	std::map<std::string, std::shared_ptr<std::vector<pmd::Born>>>born;

	// ヒープ
	std::map<std::string, ID3D12DescriptorHeap*>heap;

	// マテリアル用リソース
	std::map<std::string, ID3D12Resource*>c_rsc;

	// ボーン用リソース
	std::map<std::string, ID3D12Resource*>b_rsc;

	// インデックス用リソース
	std::map<std::string, ID3D12Resource*>i_rsc;

	// インデックス送信データ
	std::map<std::string, unsigned int*>data;
};
