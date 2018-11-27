#pragma once
#include <memory>

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Resource;

enum D3D12_RESOURCE_STATES : int;

class Union;
class Device;
class Swap;
class Render;
class MultiRender;

class List
{
public:
	// �R���X�g���N�^
	List(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~List();

	// ���Z�b�g
	void Reset(ID3D12PipelineState* pipe);

	// ���[�g�V�O�l�`���̃Z�b�g
	void SetRoot(ID3D12RootSignature& root);

	// �p�C�v���C���̃Z�b�g
	void SetPipe(ID3D12PipelineState& pipe);

	// �r���[�|�[�g�̃Z�b�g
	void SetViewport(void);
	void SetViewport(const unsigned& w, const unsigned& h);

	// �V�U�[�̃Z�b�g
	void SetScissor(void);
	void SetScissor(const unsigned& w, const unsigned& h);

	// �o���A�̃Z�b�g
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render);

	// �o���A�̃Z�b�g
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<MultiRender>multi);

	// �o���A�̃Z�b�g
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc);

	// �R�}���h�����
	void Close(void);

	// �R�}���h�A���P�[�^�̎擾
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// �R�}���h�A���P�[�^�̐���
	long CreateAllo(void);

	// �R�}���h���X�g�̐���
	long CreateList(void);


	// ���j�I��
	Union& un;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator* allo;

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* list;
};
