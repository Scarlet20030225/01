#include "Dx12System.h"

namespace MyDx12
{
	HRESULT Dx12System::InitDx3D()
	{
		DebugLayer();
		CreateDx12Device();
		CreateCommandQueue();
		CreateCommandList();
		CreateSwapChain();
		CreateRenderTarget();
		DepthBuffer();
		CreateFence();

		return S_OK;
	}

	void Dx12System::Render()
	{
		//バックバッファが現在何枚目かを取得
		UINT backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

		//コマンドリストに書き込む前にはコマンドアロケーターをリセットする
		m_commandAllocator->Reset();
		//コマンドリストをリセットする
		m_commandList->Reset(m_commandAllocator, 0);

		//ここからコマンドリストにコマンドを書き込んでいく

		//バックバッファのトランジションをレンダーターゲットモードにする
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		//バックバッファをレンダーターゲットにセット
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), backBufferIndex, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dspHeap->GetCPUDescriptorHandleForHeapStart();

		m_commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//ビューポートをセット
		CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, (float)WindowWidth, (float)WindowHeight);
		CD3DX12_RECT scissorRect = CD3DX12_RECT(0, 0, WindowWidth, WindowHeight);
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &scissorRect);

		//画面クリア
		const float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, NULL);

		//バックバッファのトランジションをPresentモードにする
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		//コマンドの書き込みはここで終わり、Closeする
		m_commandList->Close();

		//コマンドリストの実行
		ID3D12CommandList* ppCommandLists[] = { m_commandList };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//バックバッファをフロントバッファに切り替えてシーンをモニターに表示
		m_swapChain->Present(1, 0);

		//GPUの処理が完了するまで待つ
		WaitGpu();
	}

	void Dx12System::WaitGpu()
	{
		//GPUサイドが全て完了したときにGPUサイドから返ってくる値（フェンス値）をセット
		m_commandQueue->Signal(m_fence, m_fenceValue);

		//上でセットしたシグナルがGPUから帰ってくるまでストール（この行で待機）
		do
		{
			//GPUの完了を待つ間、ここで何か有意義な事（CPU作業）をやるほど効率が上がる

		} while (m_fence->GetCompletedValue() < m_fenceValue);

		//ここでフェンス値を更新する 前回より大きな値であればどんな値でもいいわけだが、1足すのが簡単なので1を足す
		m_fenceValue++;
	}

	void Dx12System::DestroyDx3D()
	{
		//GPUの処理が完了するまで待つ
		WaitGpu();

		SafeRelease(m_swapChain);
		for (int i = 0; i < FrameCount; i++) SafeRelease(m_renderTargets[i]);
		SafeRelease(m_depthBuffer);
		SafeRelease(m_commandAllocator);
		SafeRelease(m_commandQueue);
		SafeRelease(m_commandList);
		SafeRelease(m_rtvHeap);
		SafeRelease(m_fence);
		SafeRelease(m_device);
	}


//-------------------------------------------------------------------------
// InitDx3Dの中身
//-------------------------------------------------------------------------
	void Dx12System::DebugLayer()
	{
		UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
		ID3D12Debug* debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}
#endif
	}

	void Dx12System::CreateDx12Device()
	{
		D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));
	}

	void Dx12System::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	}

	void Dx12System::CreateCommandList()
	{
		m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
		m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, NULL, IID_PPV_ARGS(&m_commandList));
		m_commandList->Close();
	}

	void Dx12System::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = FrameCount;
		swapChainDesc.Width = WindowWidth;
		swapChainDesc.Height = WindowHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		IDXGIFactory4* factory;
		CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		factory->CreateSwapChainForHwnd(m_commandQueue, m_hWnd, &swapChainDesc, NULL, NULL, (IDXGISwapChain1**)&m_swapChain);
	}

	void Dx12System::CreateRenderTarget()
	{
		// レンダーターゲットビューのヒープ作成
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

		//レンダーターゲットビュー作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT n = 0; n < FrameCount; n++)
		{
			m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_device->CreateRenderTargetView(m_renderTargets[n], NULL, rtvHandle);
			rtvHandle.Offset(1, m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}

	}

	void Dx12System::DepthBuffer()
	{
		//深度バッファ自体の作成
		D3D12_HEAP_PROPERTIES dsHeapProp;
		ZeroMemory(&dsHeapProp, sizeof(&dsHeapProp));
		dsHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		dsHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		dsHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		dsHeapProp.CreationNodeMask = NULL;
		dsHeapProp.VisibleNodeMask = NULL;

		D3D12_RESOURCE_DESC dsResDesc;
		ZeroMemory(&dsResDesc, sizeof(D3D12_RESOURCE_DESC));
		dsResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		dsResDesc.Alignment = 0;
		dsResDesc.Width = WindowWidth;
		dsResDesc.Height = WindowHeight;
		dsResDesc.DepthOrArraySize = 1;
		dsResDesc.MipLevels = 1;
		dsResDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsResDesc.SampleDesc.Count = 1;
		dsResDesc.SampleDesc.Quality = 0;
		dsResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		dsResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue;
		ZeroMemory(&clearValue, sizeof(D3D12_CLEAR_VALUE));
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		m_device->CreateCommittedResource
		(
			&dsHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&dsResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&m_depthBuffer)
		);

		//深度バッファのビュー用のヒープ
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		ZeroMemory(&dsvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = NULL;
		m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dspHeap));

		//深度バッファのビュー
		D3D12_DEPTH_STENCIL_VIEW_DESC dsViewDesk;
		ZeroMemory(&dsViewDesk, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
		dsViewDesk.Format = DXGI_FORMAT_D32_FLOAT;
		dsViewDesk.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsViewDesk.Flags = D3D12_DSV_FLAG_NONE;
		dsViewDesk.Texture2D.MipSlice = 0;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dspHeap->GetCPUDescriptorHandleForHeapStart();

		m_device->CreateDepthStencilView(m_depthBuffer, &dsViewDesk, dsvHandle);
	}

	void Dx12System::CreateFence()
	{
		m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		m_fenceValue = 1;
	}
}