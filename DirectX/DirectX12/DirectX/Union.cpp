#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#include "Union.h"
#include "../Window/Window.h"
#include "../Sound/Xaudio2.h"
#include "../Sound/MIDI_IN.h"
#include "../Input/Input.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"
#include "Descriptor/Render.h"
#include "Descriptor/Depth.h"
#include "Fence.h"
#include "Root.h"
#include "Compiler/Compiler.h"
#include "PipeLine/Pipe.h"
#include "Descriptor/Constant.h"
#include "Draw/Point.h"
#include "Draw/Box.h"
#include "Texture/Texture.h"
#include "PMD/PMD.h"
#include <tchar.h>

#pragma comment(lib, "d3d12.lib")

// コンストラクタ
Union::Union() : 
	alpha(1.0f)
{
	msg = {};
	viewPort = {};
	scissor = {};
	barrier = {};
}

// デストラクタ
Union::~Union()
{
}

// ウィンドウのサイズセット
void Union::ChangeWindowSize(UINT x, UINT y)
{
	this->x = x;
	this->y = y;
}

// クラスのインスタンス化
void Union::Create(void)
{
	win   = std::make_shared<Window> (x, y);
	audio = std::make_shared<Xaudio2>();
	in    = std::make_shared<MIDI_IN>();
	input = std::make_shared<Input>  (win);
#ifdef _DEBUG
	debug = std::make_shared<Debug>();
#endif
	dev    = std::make_shared<Device>();
	queue  = std::make_shared<Queue> (dev);
	list   = std::make_shared<List>  (dev);
	swap   = std::make_shared<Swap>  (win, queue);
	render = std::make_shared<Render>(dev, list, swap);
	depth  = std::make_shared<Depth> (win, dev,  list, swap);
	fence  = std::make_shared<Fence> (dev, queue);
	//ルートシグネチャ生成
	CreateRoot();
	com    = std::make_shared<Compiler>();
	//パイプライン生成
	CreatePipeLine();

	constant = std::make_shared<Constant>(win, dev,  list);
	point    = std::make_shared<Point>   (win, dev,  list,      pointRoot, pointPipe, constant);
	box      = std::make_shared<Box>     (dev, list, pointRoot, boxPipe,   constant);
	tex      = std::make_shared<Texture> (dev, list, texRoot,   texPipe,   constant);
	pmd      = std::make_shared<PMD>     (dev, list, modelRoot, modelPipe, constant,  tex);

	ViewPort();
	Scissor();
}

// ルートシグネチャの生成
void Union::CreateRoot(void)
{
	pointRoot = std::make_shared<Root>(dev);
	{
		//ディスクリプタレンジ設定用構造体
		D3D12_DESCRIPTOR_RANGE range[] = {
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
		};
		//パラメータ設定用構造体
		D3D12_ROOT_PARAMETER param[] = {
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[0] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX },
		};

		pointRoot->CreateRoot(param, _countof(param));
	}

	texRoot = std::make_shared<Root>(dev);
	{
		//ディスクリプタレンジ設定用構造体
		D3D12_DESCRIPTOR_RANGE range[] = {
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
		};
		//パラメータ設定用構造体
		D3D12_ROOT_PARAMETER param[] = {
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[0] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX },
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[1] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL },
		};

		texRoot->CreateRoot(param, _countof(param));
	}

	modelRoot = std::make_shared<Root>(dev);
	{
		//ディスクリプタレンジ設定用構造体
		D3D12_DESCRIPTOR_RANGE range[] = {
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
			{ D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
		};
		//パラメータ設定用構造体
		D3D12_ROOT_PARAMETER param[] = {
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[0] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX },
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[1] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL },
			{ D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, { 1, &range[2] }, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL },
		};

		modelRoot->CreateRoot(param, _countof(param));
	}
}

// パイプラインの生成
void Union::CreatePipeLine(void)
{
	pointPipe = std::make_shared<Pipe>(L"Shader/PointShader.hlsl", dev, swap, pointRoot, com);
	boxPipe   = std::make_shared<Pipe>(L"Shader/PointShader.hlsl", dev, swap, pointRoot, com);
	{
		D3D12_INPUT_ELEMENT_DESC input[] =
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		pointPipe->CreatePipe(input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
		boxPipe->CreatePipe(  input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	texPipe = std::make_shared<Pipe>(L"Shader/BasicShader.hlsl", dev, swap, texRoot, com);
	{
		D3D12_INPUT_ELEMENT_DESC input[] =
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "ALPHA",    0, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		texPipe->CreatePipe(input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	modelPipe = std::make_shared <Pipe>(L"Shader/ModelShader.hlsl", dev, swap, modelRoot, com);
	{
		D3D12_INPUT_ELEMENT_DESC input[] =
		{
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		modelPipe->CreatePipe(input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}
}

// ビューポートのセット
void Union::ViewPort(void)
{
	viewPort.Height   = static_cast<FLOAT>(y);
	viewPort.MaxDepth = 1.0f;
	viewPort.MinDepth = 0.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width    = static_cast<FLOAT>(x);
}

// シザーのセット
void Union::Scissor(void)
{
	scissor.bottom = static_cast<LONG>(y);
	scissor.left   = 0;
	scissor.right  = static_cast<LONG>(x);
	scissor.top    = 0;
}

// バリアのセット
void Union::Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter)
{
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = render->GetResource(swap->Get()->GetCurrentBackBufferIndex());
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	//バリア設置
	list->GetList()->ResourceBarrier(1, &barrier);
}

// メッセージの確認
bool Union::CheckMsg(void)
{
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換
			TranslateMessage(&msg);
			//1つのウィドウプロシージャにメッセージを送出する
			DispatchMessage(&msg);
		}
	}

	return true;
}

// ビュー行列のセット
void Union::ChangeView(const Vec3f & pos, const Vec3f & target, const Vec3f & up)
{
	constant->ChangeView(pos, target, up);
}

// 描画準備
void Union::Set(void)
{
	list->Reset(nullptr);

	/*list->SetRoot(pointRoot->Get());

	list->SetPipe(pointPipe->Get());*/

	constant->UpDataWVP();

	list->SetViewPort(viewPort);

	list->SetScissor(scissor);

	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

	render->SetRender(depth->GetHeap());

	depth->SetDepth();
}

// 実行
void Union::Do(void)
{
	point->Draw();
	box->Draw();

	Barrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	list->Close();

	//コマンド実行.
	ID3D12CommandList* ppCmdLists[] = {
		list->GetList(),
	};
	queue->Get()->ExecuteCommandLists(_countof(ppCmdLists), ppCmdLists);

	swap->Present();

	fence->Wait();

	point->Reset();
	box->Reset();
}

// キー入力
bool Union::CheckKey(UINT index)
{
	return input->CheckKey(index);
}

// トリガーキー入力
bool Union::TriggerKey(UINT index)
{
	return input->TriggerKey(index);
}

// 点の描画
void Union::DrawPoint(const Vec2f & pos, const Vec3f & color)
{
	point->AddList(pos, color, alpha);
}

// ボックスの描画
void Union::DrawBox(const Vec2f & pos, const Vec2f & size, const Vec3f & color)
{
	box->AddList(pos, size, color, alpha);
}

// 画像読み込み
void Union::LoadImg(UINT & index, const std::string & fileName)
{
	tex->LoadWIC(index, fileName);
}

// 描画
void Union::Draw(UINT& index, const Vec2f& pos, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, alpha, turnX, turnY);
}

// 描画・サイズ指定
void Union::Draw(UINT & index, const Vec2f & pos, const Vec2f & size, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, size, alpha, turnX, turnY);
}

// 描画・サイズ指定・分割
void Union::Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rect, const Vec2f& rectSize, UINT turnX, UINT turnY)
{
	tex->Draw(index, pos, size, rect, rectSize, alpha, turnX, turnY);
}

// 画像の消去
void Union::DeleteImg(UINT & index)
{
	tex->Delete(index);
}

// PMD読み込み
void Union::LoadPMD(UINT & index, const std::string & fileName)
{
	pmd->LoadPMD(index, fileName);
}

// PMD描画
void Union::DrawPMD(UINT & index)
{
	pmd->Draw(index);
}

// PMDの消去
void Union::DeletePMD(UINT & index)
{
	pmd->Delete(index);
}

// WAVE読み込み
void Union::LoadWave(UINT& index, const std::string & fileName)
{
	audio->LoadWAVE(index, fileName);
}

// WAVEの再生
void Union::PlayWave(UINT& index)
{
	audio->Play(index);
}

// WAVEの再生停止
void Union::StopWave(UINT& index)
{
	audio->Stop(index);
}

// WAVEの消去
void Union::DeleteWAVE(UINT & index)
{
	audio->Delete(index);
}

// MIDIデバイス数の取得
UINT Union::GetMidiNum(void)
{
	return in->GetDevNum();
}

// MIDIステータスバイトの取得
UCHAR Union::GetMidiState(void)
{
	return in->GetState();
}

// MIDIデータバイト1の取得
UCHAR Union::GetMidiData1(void)
{
	return in->GetData1();
}

// MIDIデータバイト2の取得
UCHAR Union::GetMidiData2(void)
{
	return in->GetData2();
}

// ファイルを返す
std::string Union::GetFile(const fs::path & p)
{
	std::string m;
	//ファイルの場合
	if (fs::is_regular_file(p))
	{
		m = p.filename().string();
	}
	else if (fs::is_directory(p))
	{
		//m = p.string();
	}

	return m;
}

// ファイルを返す
std::wstring Union::GetFileW(const fs::path & p)
{
	std::wstring m;
	//ファイルの場合
	if (fs::is_regular_file(p))
	{
		m = p.filename().wstring();
	}
	else if (fs::is_directory(p))
	{
		//m = p.wstring();
	}

	return m;
}

// ディレクトリのファイル列挙
std::vector<std::string> Union::GetDirFile(const std::string & point)
{
	//列挙の起点
	fs::path p(point.c_str());

	std::vector<std::string>fileName;

	for (auto& i : fs::recursive_directory_iterator(p))
	{
		auto dummy = GetFile(i);
		if (dummy.size() > 0)
		{
			fileName.push_back(dummy);
		}
	}

	return fileName;
}

// ディレクトリのファイル列挙
std::vector<std::wstring> Union::GetDirFile(const std::wstring & point)
{
	//列挙の起点
	fs::path p(point.c_str());

	std::vector<std::wstring>fileName;

	for (auto& i : fs::recursive_directory_iterator(p))
	{
		auto dummy = GetFileW(i);
		if (dummy.size() > 0)
		{
			fileName.push_back(dummy);
		}
	}

	return fileName;
}
