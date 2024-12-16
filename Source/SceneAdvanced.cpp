#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneAdvanced.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "ItemManager.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageElevator.h"
#include "StageAlter.h"
#include "StageGrid.h"
#include "EraManager.h"
#include <Input/Input.h>
#include "LightManager.h"
#include "SceneTitle.h"
// ������
void SceneAdvanced::Initialize()
{

	era = EraManager::Instance().GetEra();
	EraManager::Instance().SetDifficulty(Stage::Difficulty::Advanced);
	GameBGM = Audio::Instance().LoadAudioSource("Data/Audio/SceneTitle.wav");
	GameBGM->Play(true);
	switch (era)
	{
	case SceneGame::Era::Past:
		{
			// �X�e�[�W������
			StageManager& stageManager = StageManager::Instance();
			StagePast* stagePast = new StagePast();
			stageManager.Register(stagePast);

			stagePazzle1 = new StagePazzle1;
			stageManager.Register(stagePazzle1);
			stagePazzle2 = new StagePazzle2;
			stageManager.Register(stagePazzle2);
			stagePazzle3 = new StagePazzle3;
			stageManager.Register(stagePazzle3);
			stagePazzle4 = new StagePazzle4;
			stageManager.Register(stagePazzle4);

			StageAlter* stageAlter = new StageAlter();
			stageManager.Register(stageAlter);

			StageGrid* stageGrid = new StageGrid();
			stageManager.Register(stageGrid);

			StageElevator* stageElevator = new StageElevator();
			stageManager.Register(stageElevator);
			//�_������ǉ�
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				LightManager::Instans().Register(light);
			}

		}
		break;
	case SceneGame::Era::Future:
		{
			StageManager& stageManager = StageManager::Instance();
			StageFuture* stageFuture = new StageFuture();
			stageManager.Register(stageFuture);

			StageElevator* stageElevator = new StageElevator();
			stageManager.Register(stageElevator);
			//�_������ǉ�
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				LightManager::Instans().Register(light);
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
		DirectX::XMFLOAT3(0, 10, -10),
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

	Clear = new Sprite("Data/Sprite/GameClear.png");

}

// �I����
void SceneAdvanced::Finalize()
{

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

	LightManager::Instans().Clear();
}

// �X�V����
void SceneAdvanced::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	ItemManager& itemManager = ItemManager::Instance();

	{
		Light* pointLight = LightManager::Instans().GetLight(1);
		pointLight->SetPosition(player->GetPosition());
	}

	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�V�[���؂�ւ�����
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Past);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneAdvanced));
	}
	else if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Future);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneAdvanced));
	}

	if (gamepad.GetButtonDown() & GamePad::BTN_ENTER && flag == true)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Past);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}

	LightManager::Instans().Register(new Light(LightType::Directional));

	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	//�A�C�e���X�V����
	ItemManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}


// �`�揈��
void SceneAdvanced::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	GamePad& gamepad = Input::Instance().GetGamePad();

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

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}


	// 3D�f�o�b�O�`��
	{
		//player->DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//ItemManager::Instance().DrawDebugPrimitive();
	}

	// 2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(Clear->GetTextureWidth());
		float textureHeight = static_cast<float>(Clear->GetTextureHeight());
		//�^�C�g���X�v���C�g�`��
		if (EraManager::Instance().GetPazzle1() == true &&
			EraManager::Instance().GetPazzle2() == true &&
			EraManager::Instance().GetPazzle3() == true &&
			EraManager::Instance().GetPazzle4() == true)
		{
			Clear->Render(dc,
				0, 0, screenWidth, screenHeight,
				0, 0, textureWidth, textureHeight,
				0,
				1, 1, 1, 1);

			flag = true;
		}
		

	}

	

	// 2D�f�o�b�OGUI�`��
	{
		player->DrawDebugGUI();
	}
}