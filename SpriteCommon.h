#pragma once
#include "DirectXCommon.h"


class SpriteCommon
{
public:
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	//共通描画設定
	void PreDraw();

private:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	// パイプランステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;

	//ルートシグネチャの作成
	void RootSignatureInitialize();
	//グラフィックスパイプラインの作成
	void PipelineStateInitialize();

	DirectXCommon* dxCommon_;
};

