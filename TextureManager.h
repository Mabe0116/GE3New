#pragma once
#include "DirectXCommon.h"
#include <string>
#include <DirectXTex.h>

class TextureManager
{
private:
	struct TextureData {
		std::wstring filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

public:
	//�V���O���g��
	static TextureManager* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* dxCommon);

	//�摜�ǂݍ���
	void LoadTexture(const std::wstring& filePath);

public:
	//�ǂݍ��񂾉摜��GPU(�V�F�[�_�[�̑���)
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	//�w�肵���摜���z��̉��Ԗڂɂ��邩
	uint32_t GetTextureIndexfilePath(const std::wstring& filePath);

	//�w�肵��GPU�n���h�����󂯎��
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	//���^�f�[�^�擾
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

private:
	static TextureManager* instance;
	static uint32_t kSRVIndexTop;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator = (TextureManager&) = delete;

private:
	DirectXCommon* dxCommon_ = nullptr;

	std::vector<TextureData> textureDatas;
};

