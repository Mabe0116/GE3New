#include "SpriteCommon.h"

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;

	PipelineStateInitialize();
}

void SpriteCommon::PreDraw()
{
    // プリミティブ形状の設定コマンド
    dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
    // パイプラインステートとルートシグネチャの設定コマンド
    dxCommon_->GetCommandList()->SetPipelineState(pipelineState.Get());
    dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
}

void SpriteCommon::RootSignatureInitialize()
{
}

void SpriteCommon::PipelineStateInitialize()
{
}
