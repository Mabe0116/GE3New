#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <chrono>
#include <thread>

#include "WinApp.h"

using namespace Microsoft::WRL;


class DirectXCommon
{
public:
	void Initialize(WinApp* winApp);

	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	//Getter
	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

	//スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }

	//RTVデスク
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc{};

	//SRVディスクリプタ―ヒープ
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap.Get(); }

private:
	//デバイス
	void DeviceInitialize();
	//コマンド
	void CommandInitialize();
	//スワップチェーン
	void SwapChainInitialize();
	//レンダーターゲット
	void RenderTargetInitialize();
	//深度バッファ
	void DepthBufferInitialize();
	//フェンス
	void FenceInitialize();

	//ディスクリプタ―ヒープ作成
	ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType,UINT numDescriptor,bool shaderVisible);

	//FPS固定処理
	void InitializeFixFPS();
	void UpdateFixFPS();

private:
	WinApp* winApp = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	ComPtr<ID3D12Resource> depthBuff;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	//記録用時間計測の変数
	std::chrono::steady_clock::time_point reference_;

	//ディスクリプタ―ヒープ
	//RTV
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;

	//SRV
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
}; 

