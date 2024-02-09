#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include <DirectXMath.h>

#include "SpriteCommon.h"
//
//#include "math/Vector4.h"
//#include "math/Vector2.h"
//#include "math/Vector3.h"

class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	struct Transform {
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 rotate;
		DirectX::XMFLOAT3 translate;
	};

	struct VertexData {
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texcoord;
	};

public:
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common);
	void Update();
	void Draw();

	////頂点データ
	//struct VectorData {
	//	Vector4 position;
	//	Vector2 texcoord;
	//	Vector3 normal;
	//};

private:
	//頂点情報作成
	void CreateVertex();
	//マテリアル作成
	void CreateMaterial();
	//行列作成
	void CreateWVP();

private:
	DirectXCommon* dxcommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//マテリアル情報
	ComPtr<ID3D12Resource> materialResource;

	//行列情報
	ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	//画像保存先のアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	//パラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//						 Scale	 Rotate	Translate
	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };

	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };

};

