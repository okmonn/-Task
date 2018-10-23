#pragma once
#include <memory>

struct ID3D12PipelineState;
struct D3D12_INPUT_ELEMENT_DESC;
enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;

class Device;
class Swap;
class Root;

class Pipe
{
public:
	// �R���X�g���N�^
	Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root);
	// �f�X�g���N�^
	~Pipe();

	// �p�C�v���C���̐���
	long CreatePipe(const D3D12_INPUT_ELEMENT_DESC& input, const unsigned int& num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const bool& depth = false);

	// �p�C�v���C���̎擾
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	ID3D12PipelineState* pipe;
};