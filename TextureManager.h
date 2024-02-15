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
	//シングルトン
	static TextureManager* GetInstance();
	void Finalize();

	void Initialize(DirectXCommon* dxCommon);

	//画像読み込み
	void LoadTexture(const std::wstring& filePath);

public:
	//読み込んだ画像をGPU(シェーダーの送る)
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	//指定した画像が配列の何番目にあるか
	uint32_t GetTextureIndexfilePath(const std::wstring& filePath);

	//指定したGPUハンドルを受け取る
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	//メタデータ取得
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

