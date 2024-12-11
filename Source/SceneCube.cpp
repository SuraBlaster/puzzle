#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageMoveFloor.h"
#include "StageElevator.h"
#include "EraManager.h"
#include <Input/Input.h>
#include "Input/Cube.h"
#include "LightManager.h"
#include "SceneCube.h"


//using namespace DirectX; 
//�����������DirectX::���s�K�v
// D3D11_VIEWPORT�^�̃f�[�^��Viewport�^�ɕϊ�����֐�
Viewport ConvertD3D11ViewportToViewport(const D3D11_VIEWPORT& d3dViewport)
{
	return Viewport{
		d3dViewport.TopLeftY,// Y���̍ŏ�[
		d3dViewport.TopLeftX,// X���̍��[
		d3dViewport.Width,   // ��
		d3dViewport.Height,  // ����
		d3dViewport.MinDepth,// �ŏ��[�x
		d3dViewport.MaxDepth // �ő�[�x
	};
}

// ������
void SceneCube::Initialize()
{
	
	era = EraManager::Instance().GetEra();

	//�L���[�u������
	StageCubeManager::Instance().Initialize();

	switch (era)
	{
	case SceneGame::Era::Past:
		{
			// �X�e�[�W������
			StageManager& stageManager = StageManager::Instance();
			StagePast* stagePast = new StagePast();
			stageManager.Register(stagePast);

			StageElevator* stageElevator = new StageElevator();
			stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0));
			stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 9, 0));
			stageManager.Register(stageElevator);


			//�_������ǉ�
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				LightManager::Instans().Register(light);
			}

			//�L���[�u�̈ʒu�ɓ_������ǉ�
			{
				DirectX::XMFLOAT3 rootPosition = StageCubeManager::Instance().GetRootCube()->GetPosition();
				Light* CubeLight = new Light(LightType::Point);
				CubeLight->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				CubeLight->SetPosition({ rootPosition.x + 1,2.0f,rootPosition.z + 1 });
				CubeLight->SetRange(200.0f);
				LightManager::Instans().Register(CubeLight);
			}
		}
		break;
	case SceneGame::Era::Future:
		{
			StageManager& stageManager = StageManager::Instance();
			StageFuture* stageFuture = new StageFuture();
			stageManager.Register(stageFuture);

			StageElevator* stageElevator = new StageElevator();
			stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0)); 
			stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 9, 0));
			stageManager.Register(stageElevator);

			//�_������ǉ�
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				light->SetRange(0);
				LightManager::Instans().Register(light);
			}

			//�L���[�u�̈ʒu�ɓ_������ǉ�
			{
				DirectX::XMFLOAT3 rootPosition = StageCubeManager::Instance().GetRootCube()->GetPosition();
				Light* CubeLight = new Light(LightType::Point);
				CubeLight->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				CubeLight->SetPosition({ rootPosition.x + 1,2.0f,rootPosition.z + 1 });
				CubeLight->SetRange(10.0f);
				LightManager::Instans().Register(CubeLight);
			}
		}
		break;
	}

	
	//�v���C���[������
	player = new Player;

	

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 0, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetParspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	
	LightManager::Instans().Register(new Light(LightType::Directional));


	//�J�����R���g���[���[������
	cameraController = new CameraController;
}

// �I����
void SceneCube::Finalize()
{

	EnemyManager::Instance().Clear();

	//�J�����R���g���[���[�I������
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//�v���C���[�I������
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//�X�e�[�W�I������
	StageManager::Instance().Clear();

	StageCubeManager::Instance().Finalize();

	LightManager::Instans().Clear();

}

// �X�V����
void SceneCube::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	DirectX::XMFLOAT3 playerPos = player->GetPosition();

	{
		Light* pointLight = LightManager::Instans().GetLight(1);
		pointLight->SetPosition(player->GetPosition());
	}
	
	static bool isCubeView = cameraController->GetIsCubeView();
	DirectX::XMFLOAT3 targetPos = cameraController->TargetPosition(playerPos, { 1, 1, 1 }, Length, isCubeView);
	cameraController->SetIsCubeView(isCubeView);  // ��Ԃ𔽉f
	cameraController->SetTarget(targetPos);
	StageRootCube* rootCube = StageCubeManager::Instance().GetRootCube();
    if (rootCube)
    {
        rootCube->SetIsCubeView(isCubeView);  // isCubeView��Ԃ�StageRootCube�ɓK�p
    }
	
	//�J�����R���g���[���[�X�V����
	cameraController->Update(elapsedTime);

	//�V�[���؂�ւ�
	if (!isCubeView)
	{
		if (gamepad.GetButtonDown() & GamePad::BTN_A)
		{
			EraManager::Instance().SetEra(SceneGame::Era::Past);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
		else if (gamepad.GetButtonDown() & GamePad::BTN_B)
		{
			EraManager::Instance().SetEra(SceneGame::Era::Future);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
	}

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	if (!isCubeView)
	{
		player->Update(elapsedTime);
	}

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);

	//�L���[�u�X�V����
	StageCubeManager::Instance().GetRootCube()->Update(elapsedTime);
}

// �`�揈��
void SceneCube::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	{
		LightManager::Instans().PushRenderContext(rc);

		LightManager::Instans().DrawDebugGUI();
	}

	//�J���������ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();



	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader(ModelShaderId::LambartShader);
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�v���C���[�`��
		player->Render(dc, shader);

		//�L���[�u�`��
		StageCubeManager::Instance().GetRootCube()->Render(dc, shader);

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
	

		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	

	// 3D�f�o�b�O�`��
	{
		EnemyManager::Instance().DrawDebugPrimitive();

		player->DrawDebugPrimitive();

		LightManager::Instans().DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		
	}

	// 2D�f�o�b�OGUI�`��
	{
		
	}
	
}



