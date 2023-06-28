#pragma once

// �K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include<Windows.h>		// Win32API�̋@�\���g�p����̂ɕK�v
#include<d3d12.h>		// Direct3D 12 API
#include<d3dx12.h>		// Direct3D 12 Graphics���J������̂ɕK�v�ȃC���^�[�t�F�C�X���܂܂�Ă���
#include<dxgi1_4.h>		// DXGI���J������̂ɕK�v
#include<d3dcompiler.h>	// HLSL�R�[�h�܂��̓G�t�F�N�g�t�@�C�����A����̃^�[�Q�b�g�̃o�C�g�R�[�h�ɃR���p�C������
#include<DirectXMath.h>	// �P���x���������_�x�N�g���܂��͍s��ɑ΂���Z�p���Z�Ɛ��`�㐔���Z�ɍœK�ňڐA�\�ȃC���^�[�t�F�C�X������

// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"Ole32.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"winmm.lib")

// �l�[���X�y�[�X
using namespace DirectX;

// �萔��`
#define WindowWidth 480		// �E�B���h�E��
#define WindowHeight 270	// �E�B���h�E����
#define AppName "NoName"	// �A�v���P�[�V�����̖��O
#define FrameCount 3		// ��ʃo�b�t�@��

// �}�N����`
#define SafeRelease(x) if(x){x->Release(); x = NULL;}

namespace MyDx12
{
	/// <summary>
	/// DirectX12�̏�������
	/// </summary>
	class Dx12System
	{
	public:
		Dx12System() {}
		~Dx12System() {}

		/// <summary>
		/// Direct3D12������
		/// </summary>
		/// <returns></returns>
		HRESULT InitDx3D();
		
		/// <summary>
		/// �V�[������ʂɃ����_�����O
		/// </summary>
		void Render();

		/// <summary>
		/// ��������
		/// </summary>
		void WaitGpu();

		/// <summary>
		/// Dx12�I�u�W�F�N�g��S�Ĕj��
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

		// RTV, DSV�n
		ID3D12Resource* m_renderTargets[FrameCount];
		ID3D12Resource* m_depthBuffer;
		ID3D12DescriptorHeap* m_rtvHeap;
		ID3D12DescriptorHeap* m_dspHeap;

		// CBV, SRV �n

		// VBV, IBV �n


	private:
		/// <summary>
		/// �f�o�b�N���C���[
		/// </summary>
		void DebugLayer();

		/// <summary>
		/// DirectX12�f�o�C�X�쐬
		/// </summary>
		void CreateDx12Device();

		/// <summary>
		/// �R�}���h�L���[�쐬
		/// </summary>
		void CreateCommandQueue();

		/// <summary>
		/// �R�}���h���X�g�쐬
		/// </summary>
		void CreateCommandList();

		/// <summary>
		/// �X���b�v�`�F�[���쐬
		/// </summary>
		void CreateSwapChain();

		/// <summary>
		/// �����_�[�^�[�Q�b�g�쐬
		/// </summary>
		void CreateRenderTarget();

		/// <summary>
		/// �[�x�o�b�t�@
		/// </summary>
		void DepthBuffer();

		/// <summary>
		/// �t�F���X�쐬
		/// </summary>
		void CreateFence();
	};
}