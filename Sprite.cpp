#include "Sprite.h"
#include "SpriteCommon.h"
#include <DirectXMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common)
{
	dxcommon_ = dxCommon;
	common_ = common;

	//VertexResource
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(XMFLOAT4) * 3;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	
	HRESULT result = dxcommon_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(result));


	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.BufferLocation = sizeof(DirectX::XMFLOAT4) * 3;
	vertexBufferView.StrideInBytes = sizeof(DirectX::XMFLOAT4);


}

void Sprite::Draw()
{
	XMFLOAT4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData[1] = { +0.0f,+0.5f,0.0f,1.0f };
	vertexData[2] = { +0.5f,-0.5f,0.0f,1.0f };

	dxcommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxcommon_->GetCommandList()->SetPipelineState(common_->GetPipeLineState());

	dxcommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxcommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}