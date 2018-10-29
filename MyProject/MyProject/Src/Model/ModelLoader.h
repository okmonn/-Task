#pragma once
#include "PmdData.h"
#include <array>
#include <memory>

class Device;
class Texture;

class ModelLoader
{
public:
	// コンストラクタ
	ModelLoader(std::weak_ptr<Device>dev, std::weak_ptr<Texture>texture);
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
	// IKボーンの取得
	std::shared_ptr<std::map<std::string, pmd::IkBorn>> GetIkBorn(const std::string& fileName) {
		return ikBorn[fileName];
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
	// 乗算テクスチャの取得
	std::shared_ptr<std::map<int, int>> GetSph(const std::string& fileName) {
		return sph[fileName];
	}
	// 加算テクスチャの取得
	std::shared_ptr<std::map<int, int>> GetSpa(const std::string& fileName) {
		return spa[fileName];
	}
	// 通常テクスチャの取得
	std::shared_ptr<std::map<int, int>> GetTex(const std::string& fileName) {
		return tex[fileName];
	}
	// トゥーンテクスチャの取得
	std::shared_ptr<std::map<int, int>> GetToon(const std::string& fileName) {
		return toon[fileName];
	}

private:
	// テクスチャの読み込み
	long LoadTex(const std::string& fileName);

	// トゥーンテクスチャの読み込み
	long LoadToon(const std::string& fileName);

	// ヒープの生成
	long CreateHeap(const std::string& fileName);

	// 定数リソースの生成
	long CreateConRsc(ID3D12Resource** rsc, const int& w);

	// インデックスの生成
	long CreateIndex(const std::string& fileName);


	// デバイス
	std::weak_ptr<Device>dev;

	// テクスチャ
	std::weak_ptr<Texture>texture;

	// 頂点
	std::map<std::string, std::shared_ptr<std::vector<pmd::Vertex>>>vertex;

	// インデックス
	std::map<std::string, std::shared_ptr<std::vector<unsigned short>>>index;

	// マテリアル
	std::map<std::string, std::shared_ptr<std::vector<pmd::Material>>>material;

	// ボーン
	std::map<std::string, std::shared_ptr<std::vector<pmd::Born>>>born;

	// IKボーン
	std::map < std::string, std::shared_ptr<std::map<std::string, pmd::IkBorn>>>ikBorn;

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

	// 乗算テクスチャ
	std::map<std::string, std::shared_ptr<std::map<int, int>>>sph;

	// 加算テクスチャ
	std::map<std::string, std::shared_ptr<std::map<int, int>>>spa;

	// 通常テクスチャ
	std::map<std::string, std::shared_ptr<std::map<int, int>>>tex;

	// トゥーンのパス
	std::array<char[100], 10> toonPath;

	// トゥーンテクスチャ
	std::map<std::string, std::shared_ptr<std::map<int, int>>>toon;
};
