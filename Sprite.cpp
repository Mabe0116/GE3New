#include "Sprite.h"
#include "SpriteCommon.h"
#include <DirectXMath.h>
#include <BufferResource.h>
#include "ImGuiManager.h"

#include "External/imgui/imgui.h"

#include "TextureManager.h"

using namespace Microsoft::WRL;
using namespace DirectX;

void Sprite::Initialize(DirectXCommon* dxCommon, SpriteCommon* common, std::wstring textureFilePath)
{
	dxcommon_ = dxCommon;
	common_ = common;

	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexfilePath(textureFilePath);

	//頂点情報
	CreateVertex();
	//インデックス情報
	CreateIndex();
	//色
	CreateMaterial();
	//行列
	CreateWVP();

	//画像のサイズを整理する
	AdjustTextureSize();
}

void Sprite::Update()
{
	//更新処理
	transform.translate = {position.x,position.y,0};
	transform.rotate = { 0,0,rotation };
	materialData->color = color_;
	transform.scale = { size.x,size.y,1.0f };

	//アンカーポイント更新
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	//フリップ
	if (isFlipX == true) {
		//左右反転
		left = -left;
		right = -right;
	}

	if (isFlipY == true) {
		//上下反転
		top = -top;
		bottom = -bottom;
	}

	//頂点情報
	vertexData[0].position = { left,bottom,0.0f,1.0f };
	vertexData[1].position = { left,top,0.0f,1.0f };
	vertexData[2].position = { right,bottom,0.0f,1.0f };
	vertexData[3].position = { right,top,0.0f,1.0f };

	
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureIndex_);

	//UV座標
	float tex_left = textureLeftTop.x / metaData.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metaData.width;
	float tex_top = textureLeftTop.y / metaData.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metaData.height;

	
	vertexData[0].texcoord = { tex_left,tex_bottom };
	vertexData[1].texcoord = { tex_left,tex_top };
	vertexData[2].texcoord = { tex_right,tex_bottom };
	vertexData[3].texcoord = { tex_right,tex_top };

	ImGui::Begin("Texture");
	ImGui::DragFloat3("Pos", &transform.translate.x, 0.1f);

	ImGui::DragFloat3("UV-Pos", &uvTransform.translate.x, 0.01f, -10.f, 10.f);
	ImGui::SliderAngle("UV-Rot", &uvTransform.rotate.z);
	ImGui::DragFloat3("UV-Scale", &uvTransform.scale.x, 0.01f, -10.f, 10.f);

	ImGui::End();
}

void Sprite::Draw()
{
	//Y軸中心に回転
	transform.rotate.y += 0.03f;
	//ワールド
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&transform.scale));
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&transform.rotate));
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&transform.translate));

	//回転行列とスケール行列の掛け算
	XMMATRIX rotationAndScaleMatrix = XMMatrixMultiply(rotateMatrix, scaleMatrix);
	//最終的な行列変換
	XMMATRIX worldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);

	//カメラ
	XMMATRIX cameraScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&cameraTransform.scale));
	XMMATRIX cameraRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&cameraTransform.rotate));
	XMMATRIX cameraTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&cameraTransform.translate));
	//回転行列とスケール行列の掛け算
	XMMATRIX cameraRotateAndScaleMatirix = XMMatrixMultiply(cameraRotateMatrix, cameraScaleMatrix);
	//最終的な行列
	XMMATRIX cameraMatrix = XMMatrixMultiply(cameraRotateAndScaleMatirix, cameraTranslationMatrix);

	//View
	XMMATRIX view = XMMatrixInverse(nullptr, cameraMatrix);
	//Proj
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0, WinApp::window_width, WinApp::window_height, 0, 0.1f, 100.f);


	//WVP
	XMMATRIX worldViewProjectionMatrix = worldMatrix * (view * proj);


	//行列の代入
	*wvpData = worldViewProjectionMatrix;

	dxcommon_->GetCommandList()->SetGraphicsRootSignature(common_->GetRootSignature());
	dxcommon_->GetCommandList()->SetPipelineState(common_->GetPipeLineState());

	//UV座標
	XMMATRIX uvScaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&uvTransform.scale));
	XMMATRIX uvRotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&uvTransform.rotate));
	XMMATRIX uvTranslationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&uvTransform.translate));

	//回転行列とスケール行列の掛け算
	XMMATRIX uvRotationAndScaleMatrix = XMMatrixMultiply(uvTranslationMatrix, uvScaleMatrix);
	//最終的な行列変換
	XMMATRIX uvWorldMatrix = XMMatrixMultiply(rotationAndScaleMatrix, translationMatrix);
	materialData->uvTransform = uvWorldMatrix;

	//頂点情報
	dxcommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//インデックス情報
	dxcommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	dxcommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//色情報
	dxcommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//行列
	dxcommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	//画像
	dxcommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex_));

	//dxcommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	dxcommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::SetTexture(std::wstring textureFilePath)
{
	textureIndex_ = TextureManager::GetInstance()->GetTextureIndexfilePath(textureFilePath);

}
void Sprite::CreateVertex()
{
	//VertexResource
	vertexResource = CreateBufferResource(dxcommon_->GetDevice(), sizeof(VertexData) * 4);

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点情報
	
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 1.0f };

	vertexData[1].position = { +0.0f, +0.0f, 0.0f, 1.0f };
	vertexData[1].texcoord = { +0.0f, 0.0f };

	vertexData[2].position = { 1.0f, 1.0f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };

	vertexData[3].position = { +1.0f, +0.0f, 0.0f, 1.0f };
	vertexData[3].texcoord = { +1.0f, 0.0f };

}

void Sprite::CreateIndex()
{
	indexResource = CreateBufferResource(dxcommon_->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	//1枚作製
	indexData[0] = 0; indexData[1] = 1;  indexData[2] = 2;
	
	indexData[3] = 1; indexData[4] = 3;  indexData[5] = 2;

}

void Sprite::CreateMaterial()
{
	materialResource = CreateBufferResource(dxcommon_->GetDevice(),sizeof(MaterialData));

	
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	materialData->color = color_;
	materialData->uvTransform = XMMatrixIdentity();

}

void Sprite::CreateWVP()
{
	wvpResource = CreateBufferResource(dxcommon_->GetDevice(), sizeof(XMMATRIX));

	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*wvpData = XMMatrixIdentity();
}

void Sprite::AdjustTextureSize()
{
	const DirectX::TexMetadata& metaData = TextureManager::GetInstance()->GetMetaData(textureIndex_);

	textureSize.x = static_cast<float>(metaData.width);
	textureSize.y = static_cast<float>(metaData.height);

	size = textureSize;
}
