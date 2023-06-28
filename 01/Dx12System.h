#pragma once

// 必要なヘッダーファイルのインクルード
#include<Windows.h>		// Win32APIの機能を使用するのに必要
#include<d3d12.h>		// Direct3D 12 API
#include<d3dx12.h>		// Direct3D 12 Graphicsを開発するのに必要なインターフェイスが含まれている
#include<dxgi1_4.h>		// DXGIを開発するのに必要
#include<d3dcompiler.h>	// HLSLコードまたはエフェクトファイルを、特定のターゲットのバイトコードにコンパイルする
#include<DirectXMath.h>	// 単精度浮動小数点ベクトルまたは行列に対する算術演算と線形代数演算に最適で移植可能なインターフェイスを実装

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"Ole32.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"winmm.lib")

// ネームスペース
using namespace DirectX;

// 定数定義
#define WindowWidth 480		// ウィンドウ幅
#define WindowHeight 270	// ウィンドウ高さ
#define AppName "NoName"	// アプリケーションの名前
#define FrameCount 3		// 画面バッファ数

// マクロ定義
#define SafeRelease(x) if(x){x->Release(); x = NULL;}

namespace MyDx12
{
	/// <summary>
	/// DirectX12の初期化等
	/// </summary>
	class Dx12System
	{
	public:
		Dx12System() {}
		~Dx12System() {}

		/// <summary>
		/// Direct3D12初期化
		/// </summary>
		/// <returns></returns>
		HRESULT InitDx3D();
		
		/// <summary>
		/// シーンを画面にレンダリング
		/// </summary>
		void Render();

		/// <summary>
		/// 同期処理
		/// </summary>
		void WaitGpu();

		/// <summary>
		/// Dx12オブジェクトを全て破棄
		/// </summary>
		void DestroyDx3D();

		HWND m_hWnd;
		IDXGISwapChain3* m_swapChain;
		ID3D12Device* m_device;
		ID3D12CommandAllocator* m_commandAllocator;
		ID3D12CommandQueue* m_commandQueue;
		ID3D12GraphicsCommandList* m_commandList;
		ID3D12Fence* m_fence;
		UINT64 m_fenceValue;

		// RTV, DSV系
		ID3D12Resource* m_renderTargets[FrameCount];
		ID3D12Resource* m_depthBuffer;
		ID3D12DescriptorHeap* m_rtvHeap;
		ID3D12DescriptorHeap* m_dspHeap;

		// CBV, SRV 系

		// VBV, IBV 系


	private:
		/// <summary>
		/// デバックレイヤー
		/// </summary>
		void DebugLayer();

		/// <summary>
		/// DirectX12デバイス作成
		/// </summary>
		void CreateDx12Device();

		/// <summary>
		/// コマンドキュー作成
		/// </summary>
		void CreateCommandQueue();

		/// <summary>
		/// コマンドリスト作成
		/// </summary>
		void CreateCommandList();

		/// <summary>
		/// スワップチェーン作成
		/// </summary>
		void CreateSwapChain();

		/// <summary>
		/// レンダーターゲット作成
		/// </summary>
		void CreateRenderTarget();

		/// <summary>
		/// 深度バッファ
		/// </summary>
		void DepthBuffer();

		/// <summary>
		/// フェンス作成
		/// </summary>
		void CreateFence();
	};
}