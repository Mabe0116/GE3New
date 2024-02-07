#pragma once

#include <wrl.h>
#include <string>
#include <dxcapi.h>
#include <d3d12.h>

#include "DirectXCommon.h"

class SpriteCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);

	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	ID3D12PipelineState* GetPipeLineState() { return pipelineState.Get(); }

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	//共通描画設定
	void PreDraw();

private:
 	static IDxcBlob* CompilerShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
		);



	//ルートシグネチャの作成
	void RootSignatureInitialize();
	//グラフィックスパイプラインの作成
	/*void PipelineStateInitialize();*/

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

