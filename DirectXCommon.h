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

	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//Getter
	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

	//�X���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }

	//RTV�f�X�N
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc{};

	//SRV�f�B�X�N���v�^�\�q�[�v
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap.Get(); }

private:
	//�f�o�C�X
	void DeviceInitialize();
	//�R�}���h
	void CommandInitialize();
	//�X���b�v�`�F�[��
	void SwapChainInitialize();
	//�����_�[�^�[�Q�b�g
	void RenderTargetInitialize();
	//�[�x�o�b�t�@
	void DepthBufferInitialize();
	//�t�F���X
	void FenceInitialize();

	//�f�B�X�N���v�^�\�q�[�v�쐬
	ID3D12DescriptorHeap* CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType,UINT numDescriptor,bool shaderVisible);

	//FPS�Œ菈��
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

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	//�L�^�p���Ԍv���̕ϐ�
	std::chrono::steady_clock::time_point reference_;

	//�f�B�X�N���v�^�\�q�[�v
	//RTV
	ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;

	//SRV
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
}; 

