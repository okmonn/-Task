#pragma once
#include "PmdData.h"
#include <memory>

class Device;
class List;
class Constant;
class Root;
class Pipe;
class Texture;
class ModelLoader;
class MotionLoader;

class Model
{
	// PMD情報
	struct Pmd {
		//頂点
		std::weak_ptr<std::vector<pmd::Vertex>>vertex;
		//インデックス
		std::weak_ptr<std::vector<unsigned short>>index;
		//マテリアル
		std::weak_ptr<std::vector<pmd::Material>>material;
		//ボーン
		std::weak_ptr<std::vector<pmd::Born>>born;
		//IKボーン
		std::weak_ptr<std::map<std::string, pmd::IkBorn>>ikBorn;

		//ヒープ
		ID3D12DescriptorHeap* heap;
		//マテリアル用リソース
		ID3D12Resource* c_rsc;
		//ボーン用リソース
		ID3D12Resource* b_rsc;
		//インデックス用リソース
		ID3D12Resource* i_rsc;

		//頂点用リソース
		ID3D12Resource* v_rsc;

		//マテリアルデータ送信
		unsigned char* c_data;
		//ボーン行列データ送信
		unsigned char* b_data;
		//頂点データ送信
		unsigned int* v_data;

		//テクスチャ
		std::weak_ptr<std::map<int, int>>tex;
		//乗算テクスチャ
		std::weak_ptr<std::map<int, int>>sph;
		//加算テクスチャ
		std::weak_ptr<std::map<int, int>>spa;
		//トゥーンテクスチャ
		std::weak_ptr<std::map<int, int>>toon;

		//ボーンノード
		std::map<std::string, pmd::BornNode>node;

		//送信マテリアルデータ
		pmd::Mat mat;
		//送信ボーン行列データ
		std::vector<DirectX::XMMATRIX>bornMtx;

		// モーション
		std::weak_ptr<std::map<std::string, std::vector<vmd::Motion>>>motion;

		// アニメーションフレーム
		float flam;

		// アニメーション時間
		float animTime;
	};

public:
	// コンストラクタ
	Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con,
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex);
	// デストラクタ
	~Model();

	// 読み込み
	long Load(const std::string& fileName, int& i);

	// モーションの適応
	int Attach(const std::string& fileName, int& i);

	// アニメーション時間のリセット
	void ResetAnim(int& i);

	// アニメーション
	void Animation(int& i, const bool& loop, const float& animSpeed);

	// 描画
	void Draw(int& i);

	// アニメーションの終了確認
	bool CheckEndAnim(int& i);

	// モデルの削除
	void DeleteMdl(int& i);

private:
	// マテリアル用シェーダビューの生成
	long CreateMaterialView(int* i);

	// ボーンノードのセット
	void SetNode(int* i);

	// ボーン用シェーダビューの生成
	long CreateBornView(int* i);

	// 頂点バッファの生成
	long CreateVertex(int* i);

	// 特定方向に向ける行列を作る
	DirectX::XMMATRIX LookAt(const DirectX::XMFLOAT3& look, const DirectX::XMFLOAT3& up, const DirectX::XMFLOAT3& right);

	// ボーンの回転
	void RotateBorn(int& i, const std::string& name, const DirectX::XMMATRIX& mtx);

	// ボーンの再帰処理
	void RecursiveBorn(int* i, pmd::BornNode& node, const DirectX::XMMATRIX& mtx);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// テクスチャ
	std::weak_ptr<Texture>tex;

	// モデルローダー
	std::unique_ptr<ModelLoader>loader;

	// モーションローダー
	std::unique_ptr<MotionLoader>motion;

	// PMD情報
	std::map<int*, Pmd>pmd;
};
