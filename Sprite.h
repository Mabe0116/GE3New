#pragma once
#include "DirectXCommon.h"
#include <d3d12.h>
#include <wrl.h>

#include <DirectXMath.h>

#include "SpriteCommon.h"

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

	struct MaterialData {
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX uvTransform;
	};

public:
	void Initialize(DirectXCommon* dxCommon,SpriteCommon* common, std::wstring textureFilePath);
	void Update();
	void Draw();

	//Getter//Setter
	DirectX::XMFLOAT2 GetPosition() { return position; }
	float GetRotation() { return rotation; }
	DirectX::XMFLOAT4 GetColor() { return color_; }
	DirectX::XMFLOAT2 GetSize() { return size; }
	//アンカーポイント
	DirectX::XMFLOAT2 GetAnchorPoint() { return anchorPoint; }
	//左右反転
	bool GetFripX() { return isFlipX;}
	//上下反転
	bool GetFripY() { return isFlipY; }
	//切り抜き関連
	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop; }
	DirectX::XMFLOAT2 GetTextureSize() { return textureSize; }


	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	//アンカーポイント
	void SetAnchorPoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }

	//左右反転
	void SetFlipX(bool isFlag) { isFlipX = isFlag; }
	//上下反転
	void SetFlipY(bool isFlag) { isFlipY = isFlag; }
	
	//切り抜き関連
	void SetTextureLeftTop(DirectX::XMFLOAT2 value) { textureLeftTop = value; }
	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSize = size; }


	void SetTexture(std::wstring texturefilePath);


	////頂点データ
	//struct VectorData {
	//	Vector4 position;
	//	Vector2 texcoord;
	//	Vector3 normal;
	//};

private:
	//頂点情報作成
	void CreateVertex();
	//インデックス作成
	void CreateIndex();
	//マテリアル作成
	void CreateMaterial();
	//行列作成
	void CreateWVP();

	//本来の画像サイズで描画する(textureをいじらずとも全体像を描画する)
	void AdjustTextureSize();

private:
	DirectXCommon* dxcommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	
	//頂点情報
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	//インデックス
	ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//マテリアル情報
	ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	//行列情報
	ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	//パラメータ
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//UV座標
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };

	//時機				   Scale	 Rotate	Translate
	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };
	DirectX::XMFLOAT2 position = { 0,0 };
	float rotation = 0;
	DirectX::XMFLOAT2 size = { 51,51 };

	//アンカーポイント
	DirectX::XMFLOAT2 anchorPoint = { 0,0 };

	//左右反転
	bool isFlipX = false;

	//上下反転
	bool isFlipY = false;

	//切り抜き
	//切り抜きたい画像内の座標
	DirectX::XMFLOAT2 textureLeftTop = { 0,0 };
	//切り抜きたい画像内のサイズ
	DirectX::XMFLOAT2 textureSize = { 414,410 };

	//画像の保存されてる場所
	uint32_t textureIndex_ = 0;

	//カメラ
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };

};

