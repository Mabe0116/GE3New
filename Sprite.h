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
	//�A���J�[�|�C���g
	DirectX::XMFLOAT2 GetAnchorPoint() { return anchorPoint; }
	//���E���]
	bool GetFripX() { return isFlipX;}
	//�㉺���]
	bool GetFripY() { return isFlipY; }
	//�؂蔲���֘A
	DirectX::XMFLOAT2 GetTextureLeftTop() { return textureLeftTop; }
	DirectX::XMFLOAT2 GetTextureSize() { return textureSize; }


	void SetPosition(DirectX::XMFLOAT2 pos) { position = pos; }
	void SetRotation(float rot) { rotation = rot; }
	void SetColor(DirectX::XMFLOAT4 color) { color_ = color; }
	void SetSize(DirectX::XMFLOAT2 size) { this->size = size; }

	//�A���J�[�|�C���g
	void SetAnchorPoint(DirectX::XMFLOAT2 anchor) { anchorPoint = anchor; }

	//���E���]
	void SetFlipX(bool isFlag) { isFlipX = isFlag; }
	//�㉺���]
	void SetFlipY(bool isFlag) { isFlipY = isFlag; }
	
	//�؂蔲���֘A
	void SetTextureLeftTop(DirectX::XMFLOAT2 value) { textureLeftTop = value; }
	void SetTextureSize(DirectX::XMFLOAT2 size) { textureSize = size; }


	void SetTexture(std::wstring texturefilePath);


	////���_�f�[�^
	//struct VectorData {
	//	Vector4 position;
	//	Vector2 texcoord;
	//	Vector3 normal;
	//};

private:
	//���_���쐬
	void CreateVertex();
	//�C���f�b�N�X�쐬
	void CreateIndex();
	//�}�e���A���쐬
	void CreateMaterial();
	//�s��쐬
	void CreateWVP();

	//�{���̉摜�T�C�Y�ŕ`�悷��(texture�������炸�Ƃ��S�̑���`�悷��)
	void AdjustTextureSize();

private:
	DirectXCommon* dxcommon_ = nullptr;
	SpriteCommon* common_ = nullptr;
	
	//���_���
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	VertexData* vertexData = nullptr;

	//�C���f�b�N�X
	ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	//�}�e���A�����
	ComPtr<ID3D12Resource> materialResource;
	MaterialData* materialData = nullptr;

	//�s����
	ComPtr<ID3D12Resource> wvpResource;
	DirectX::XMMATRIX* wvpData = nullptr;

	//�p�����[�^
	DirectX::XMFLOAT4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//UV���W
	Transform uvTransform = { {1,1,1},{0,0,0},{0,0,0} };

	//���@				   Scale	 Rotate	Translate
	Transform transform = { {1,1,1},{0,0,0},{0,0,0} };
	DirectX::XMFLOAT2 position = { 0,0 };
	float rotation = 0;
	DirectX::XMFLOAT2 size = { 51,51 };

	//�A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorPoint = { 0,0 };

	//���E���]
	bool isFlipX = false;

	//�㉺���]
	bool isFlipY = false;

	//�؂蔲��
	//�؂蔲�������摜���̍��W
	DirectX::XMFLOAT2 textureLeftTop = { 0,0 };
	//�؂蔲�������摜���̃T�C�Y
	DirectX::XMFLOAT2 textureSize = { 414,410 };

	//�摜�̕ۑ�����Ă�ꏊ
	uint32_t textureIndex_ = 0;

	//�J����
	Transform cameraTransform = { {1,1,1},{0,0,0},{0,0,-5} };

};

