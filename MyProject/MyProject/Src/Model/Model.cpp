#include "Model.h"
#include "ModelLoader.h"
#include "MotionLoader.h"
#include "../Func/Func.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Texture/Texture.h"
#include "../etc/Release.h"
#include <algorithm>

// �R���X�g���N�^
Model::Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con,
	std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex) :
	dev(dev), list(list), con(con), root(root), pipe(pipe), tex(tex), 
	loader(std::make_unique<ModelLoader>(dev, tex)), motion(std::make_unique<MotionLoader>())
{
	pmd.clear();
}

// �f�X�g���N�^
Model::~Model()
{
	for (auto itr = pmd.begin(); itr != pmd.end(); ++itr)
	{
		UnMap(itr->second.v_rsc);
		Release(itr->second.v_rsc);
	}

	loader.reset();
	motion.reset();
}

// �}�e���A���p�V�F�[�_�r���[�̐���
long Model::CreateMaterialView(int * i)
{
	//�萔�o�b�t�@�r���[�ݒ�p�\����
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes = ((sizeof(pmd::Mat) + 0xff) &~0xff);

	//GPU�A�h���X
	D3D12_GPU_VIRTUAL_ADDRESS address = pmd[i].c_rsc->GetGPUVirtualAddress();
	//�擪�n���h��
	auto handle = pmd[i].heap->GetCPUDescriptorHandleForHeapStart();

	for (unsigned int n = 0; n < pmd[i].material.lock()->size(); ++n)
	{
		desc.BufferLocation = address;

		dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

		address += desc.SizeInBytes;
		handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//�}�b�s���O
	auto hr = pmd[i].c_rsc->Map(0, nullptr, (void**)(&pmd[i].c_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�}�e���A���̃}�b�v�F���s\n"));
		return hr;
	}

	memcpy(pmd[i].c_data, &pmd[i].mat, sizeof(pmd::Mat));

	return hr;
}

// �{�[���m�[�h�̃Z�b�g
void Model::SetNode(int * i)
{
	//�{�[���s��̏�����
	pmd[i].bornMtx.resize(pmd[i].born.lock()->size());
	std::fill(pmd[i].bornMtx.begin(), pmd[i].bornMtx.end(), DirectX::XMMatrixIdentity());

	for (unsigned int n = 0; n < pmd[i].born.lock()->size(); ++n)
	{
		pmd[i].node[pmd[i].born.lock()->at(n).name].index = n;
		pmd[i].node[pmd[i].born.lock()->at(n).name].start = pmd[i].born.lock()->at(n).pos;
	}

	for (auto& n : pmd[i].node)
	{
		if (pmd[i].born.lock()->at(n.second.index).pIndex >= pmd[i].born.lock()->size())
		{
			continue;
		}

		auto name = pmd[i].born.lock()->at(pmd[i].born.lock()->at(n.second.index).pIndex).name;

		pmd[i].node[name].child.push_back(&n.second);
	}
}

// �{�[���p�V�F�[�_�r���[�̐���
long Model::CreateBornView(int * i)
{
	SetNode(i);

	//�萔�o�b�t�@�r���[�ݒ�p�\����
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = pmd[i].b_rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = ((sizeof(DirectX::XMMATRIX) * pmd[i].born.lock()->size() + 0xff) &~0xff);

	auto handle = pmd[i].heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * pmd[i].material.lock()->size();
	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

	//�}�b�s���O
	auto hr = pmd[i].b_rsc->Map(0, nullptr, (void**)(&pmd[i].b_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�{�[���̃}�b�v�F���s\n"));
		return hr;
	}

	//�R�s�[
	memcpy(pmd[i].b_data, pmd[i].bornMtx.data(), ((sizeof(DirectX::XMMATRIX) * pmd[i].born.lock()->size() + 0xff) &~0xff));

	return hr;
}

// ���_�o�b�t�@�̐���
long Model::CreateVertex(int * i)
{
	//�v���p�e�B�ݒ�p�\����
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\����
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = sizeof(pmd::Vertex) * pmd[i].vertex.lock()->size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���\�[�X����
	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pmd[i].v_rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̒��_���\�[�X�̐����F���s\n"));
		return hr;
	}

	//�}�b�s���O
	hr = pmd[i].v_rsc->Map(0, nullptr, (void**)(&pmd[i].v_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̒��_�}�b�v�F���s\n"));
		return hr;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(pmd[i].v_data, pmd[i].vertex.lock()->data(), sizeof(pmd::Vertex) * pmd[i].vertex.lock()->size());

	return hr;
}

// �ǂݍ���
long Model::Load(const std::string & fileName, int & i)
{
	if (FAILED(loader->Load(fileName)))
	{
		return S_FALSE;
	}

	pmd[&i].vertex   = loader->GetVertex(fileName);
	pmd[&i].index    = loader->GetIndex(fileName);
	pmd[&i].material = loader->GetMaterial(fileName);
	pmd[&i].born     = loader->GetBorn(fileName);
	pmd[&i].ikBorn   = loader->GetIkBorn(fileName);
	pmd[&i].heap     = loader->GetHeap(fileName);
	pmd[&i].c_rsc    = loader->GetMaterialRsc(fileName);
	pmd[&i].b_rsc    = loader->GetBornRsc(fileName);
	pmd[&i].i_rsc    = loader->GetIndexRsc(fileName);
	pmd[&i].sph      = loader->GetSph(fileName);
	pmd[&i].spa      = loader->GetSpa(fileName);
	pmd[&i].tex      = loader->GetTex(fileName);
	pmd[&i].toon     = loader->GetToon(fileName);
	
	auto hr = CreateMaterialView(&i);
	hr = CreateBornView(&i);
	hr = CreateVertex(&i);

	return hr;
}

// ���[�V�����̓K��
int Model::Attach(const std::string & fileName, int & i)
{
	if (motion->Load(fileName) != 0)
	{
		return -1;
	}

	pmd[&i].flam = 0;
	pmd[&i].motion = motion->Get(fileName);
	pmd[&i].animTime = motion->GetAnimTime(fileName);

	return 0;
}

// ��������Ɍ�����s������
DirectX::XMMATRIX Model::LookAt(const DirectX::XMFLOAT3 & look, const DirectX::XMFLOAT3 & up, const DirectX::XMFLOAT3 & right)
{
	auto vz = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&look));
	auto vx = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&right));
	auto vy = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vz, vx));
	vx = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vy, vz));
	auto mtx = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 fx = {}, fy = {}, fz = {};
	DirectX::XMStoreFloat3(&fx, vx);
	DirectX::XMStoreFloat3(&fy, vy);
	DirectX::XMStoreFloat3(&fz, vz);

	mtx.r[1].m128_f32[1] = fx.x;
	mtx.r[1].m128_f32[2] = fx.y;
	mtx.r[1].m128_f32[3] = fx.z;

	mtx.r[2].m128_f32[1] = fy.x;
	mtx.r[2].m128_f32[2] = fy.y;
	mtx.r[2].m128_f32[3] = fy.z;

	mtx.r[3].m128_f32[1] = fz.x;
	mtx.r[3].m128_f32[2] = fz.y;
	mtx.r[3].m128_f32[3] = fz.z;

	return mtx;
}

// �{�[���̉�]
void Model::RotateBorn(int & i, const std::string & name, const DirectX::XMMATRIX & mtx)
{
	if (pmd[&i].node.find(name) == pmd[&i].node.end())
	{
		return;
	}

	auto vec = DirectX::XMLoadFloat3(&pmd[&i].node[name].start);

	pmd[&i].bornMtx[pmd[&i].node[name].index] = DirectX::XMMatrixTranslationFromVector(
		DirectX::XMVectorScale(vec, -1.0f)) *
		mtx * 
		DirectX::XMMatrixTranslationFromVector(vec);
}

// �{�[���̍ċA����
void Model::RecursiveBorn(int * i, pmd::BornNode & node, const DirectX::XMMATRIX& mtx)
{
	pmd[i].bornMtx[node.index] *= mtx;

	for (auto& child : node.child)
	{
		RecursiveBorn(i, *child, pmd[i].bornMtx[node.index]);
	}
}

// �A�j���[�V�������Ԃ̃��Z�b�g
void Model::ResetAnim(int & i)
{
	pmd[&i].flam = 0.0f;

	std::fill(pmd[&i].bornMtx.begin(), pmd[&i].bornMtx.end(), DirectX::XMMatrixIdentity());

	for (auto itr = pmd[&i].motion.lock()->begin(); itr != pmd[&i].motion.lock()->end(); ++itr)
	{
		auto& key = itr->second;

		auto now = std::find_if(key.rbegin(), key.rend(),
			[&](const vmd::Motion& m) {return m.flam <= (unsigned int)pmd[&i].flam; });
		if (now == key.rend())
		{
			continue;
		}
		auto nowVec = DirectX::XMLoadFloat4(&now->rotation);

		RotateBorn(i, itr->first, DirectX::XMMatrixRotationQuaternion(nowVec));
	}

	RecursiveBorn(&i, pmd[&i].node["�Z���^�["], pmd[&i].bornMtx[pmd[&i].node["�Z���^�["].index]);

	memcpy(pmd[&i].b_data, pmd[&i].bornMtx.data(), ((sizeof(DirectX::XMMATRIX) * pmd[&i].born.lock()->size() + 0xff) &~0xff));
}

// �A�j���[�V����
void Model::Animation(int & i, const bool& loop, const float & animSpeed)
{
	std::fill(pmd[&i].bornMtx.begin(), pmd[&i].bornMtx.end(), DirectX::XMMatrixIdentity());

	for (auto itr = pmd[&i].motion.lock()->begin(); itr != pmd[&i].motion.lock()->end(); ++itr)
	{
		auto& key = itr->second;

		auto now = std::find_if(key.rbegin(), key.rend(),
			[&](const vmd::Motion& m) {return m.flam <= (unsigned int)pmd[&i].flam; });
		if (now == key.rend())
		{
			continue;
		}
		auto nowVec = DirectX::XMLoadFloat4(&now->rotation);
		float nowFlam = (float)now->flam;
		

		auto next = now.base();
		if (next == key.end())
		{
			RotateBorn(i, itr->first, DirectX::XMMatrixRotationQuaternion(nowVec));
		}
		else
		{
			auto nextVec = DirectX::XMLoadFloat4(&next->rotation);
			float nextFlam = (float)next->flam;
			float time = (pmd[&i].flam - nowFlam) / (nextFlam - nowFlam);

			time = func::Newton(time, next->a.x, next->a.y, next->b.x, next->b.y);
			//time = func::Bisection(time, next->a.x, next->a.y, next->b.x, next->b.y);

			auto trans = (DirectX::XMMatrixTranslation(now->pos.x, now->pos.y, now->pos.z) * (1.0f - time))
				+ (DirectX::XMMatrixTranslation(next->pos.x, next->pos.y, next->pos.z) * time);

			RotateBorn(i, itr->first, DirectX::XMMatrixRotationQuaternion(
				DirectX::XMQuaternionSlerp(nowVec, nextVec, time)) * trans);
		}
	}

	auto a = std::fmodf(pmd[&i].flam, (float)pmd[&i].motion.lock()->at("�Z���^�[")[1].flam);

	RecursiveBorn(&i, pmd[&i].node["�Z���^�["], pmd[&i].bornMtx[pmd[&i].node["�Z���^�["].index]);

	memcpy(pmd[&i].b_data, pmd[&i].bornMtx.data(), ((sizeof(DirectX::XMMATRIX) * pmd[&i].born.lock()->size() + 0xff) &~0xff));

	pmd[&i].flam += animSpeed;
	if (loop == true && (pmd[&i].flam >= pmd[&i].animTime + 10.0f))
	{
		ResetAnim(i);
	}
}

// �`��
void Model::Draw(int & i)
{
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	int* n = &i;

	UINT offset = 0;

	//�萔�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &pmd[n].heap);

	//�q�[�v�̐擪�n���h��
	D3D12_GPU_DESCRIPTOR_HANDLE handle = pmd[n].heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * pmd[n].material.lock()->size();
	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(3, handle);

	//���_�o�b�t�@�r���[�̃Z�b�g
	D3D12_VERTEX_BUFFER_VIEW vView = {};
	vView.BufferLocation = pmd[n].v_rsc->GetGPUVirtualAddress();
	vView.SizeInBytes    = sizeof(pmd::Vertex) * pmd[n].vertex.lock()->size();
	vView.StrideInBytes  = sizeof(pmd::Vertex);
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &vView);

	//���_�C���f�b�N�X�r���[�̃Z�b�g
	D3D12_INDEX_BUFFER_VIEW iView = {};
	iView.BufferLocation = pmd[n].i_rsc->GetGPUVirtualAddress();
	iView.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;;
	iView.SizeInBytes    = sizeof(USHORT) * pmd[n].index.lock()->size();
	list.lock()->GetList()->IASetIndexBuffer(&iView);

	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���M�p�f�[�^
	UINT8* data = pmd[n].c_data;

	//�q�[�v�̐擪�n���h��
	handle = pmd[n].heap->GetGPUDescriptorHandleForHeapStart();

	for (unsigned int i = 0; i < pmd[n].material.lock()->size(); ++i)
	{
		pmd[n].mat = {};

		//�}�e���A���\���̂Ɋi�[
		pmd[n].mat.diffuse     = pmd[n].material.lock()->at(i).diffuse;
		pmd[n].mat.alpha       = pmd[n].material.lock()->at(i).alpha;
		pmd[n].mat.specularity = pmd[n].material.lock()->at(i).specularity;
		pmd[n].mat.specula     = pmd[n].material.lock()->at(i).specula;
		pmd[n].mat.mirror      = pmd[n].material.lock()->at(i).mirror;

		//sph
		if (pmd[n].sph.lock()->find(i) != pmd[n].sph.lock()->end())
		{
			pmd[n].mat.sphFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].sph.lock()->at(i), 4);
		}
		else
		{
			tex.lock()->SetWhite();
		}

		//spa
		if (pmd[n].spa.lock()->find(i) != pmd[n].spa.lock()->end())
		{
			pmd[n].mat.spaFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].spa.lock()->at(i), 5);
		}
		else
		{
			tex.lock()->SetBlack();
		}

		//�ʏ�e�N�X�`��
		if (pmd[n].tex.lock()->find(i) != pmd[n].tex.lock()->end())
		{
			pmd[n].mat.texFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].tex.lock()->at(i));
		}
		else
		{
			tex.lock()->SetDraw(pmd[n].tex.lock()->begin()->second);
		}

		//�g�D�[���e�N�X�`��
		if (pmd[n].toon.lock()->find(pmd[n].material.lock()->at(i).toonIndex) != pmd[n].toon.lock()->end())
		{
			tex.lock()->SetDraw(pmd[n].toon.lock()->at(pmd[n].material.lock()->at(i).toonIndex), 6);
		}
		else
		{
			tex.lock()->SetGrade();
		}

		//�萔�q�[�v�̃Z�b�g
		list.lock()->GetList()->SetDescriptorHeaps(1, &pmd[n].heap);
		//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
		list.lock()->GetList()->SetGraphicsRootDescriptorTable(2, handle);

		//�R�s�[
		memcpy(data, &pmd[n].mat, sizeof(pmd::Mat));

		//�`��
		list.lock()->GetList()->DrawIndexedInstanced(pmd[n].material.lock()->at(i).indexNum, 1, offset, 0, 0);

		//�n���h���X�V
		handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//�f�[�^�X�V
		data = (UINT8*)(((sizeof(pmd::Mat) + 0xff) &~0xff) + (CHAR*)(data));

		//�I�t�Z�b�g�X�V
		offset += pmd[n].material.lock()->at(i).indexNum;
	}
}

// �A�j���[�V�����̏I���m�F
bool Model::CheckEndAnim(int & i)
{
	return (pmd[&i].flam >= pmd[&i].animTime + 10.0f);
}

// ���f���̍폜
void Model::DeleteMdl(int & i)
{
	if (pmd.find(&i) != pmd.end())
	{
		UnMap(pmd[&i].v_rsc);
		Release(pmd[&i].v_rsc);
		pmd.erase(pmd.find(&i));
	}
}
