#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include "SpriteCommon.h"
//
//#include "math/Vector4.h"
//#include "math/Vector2.h"
//#include "math/Vector3.h"


class SpriteCommon;

class Sprite
{
public:
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common);

	void Draw();

	////頂点データ
	//struct VectorData {
	//	Vector4 position;
	//	Vector2 texcoord;
	//	Vector3 normal;
	//};

private:
	SpriteCommon* common_ = nullptr;
	DirectXCommon* dxcommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
};

