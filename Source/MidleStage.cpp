#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "MidleStage.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageElevator.h"
#include "EraManager.h"
#include <Input/Input.h>
#include "ClockManager.h"
#include "Clock.h"
#include "LightManager.h"
#include "ItemManager.h"

// ������
void MidleStage::Initialize()
{

	era = EraManager::Instance().GetEra();

	EraManager::Instance().SetDifficulty(Stage::Difficulty::Middle);

	MidleStageBGM = Audio::Instance().LoadAudioSource("Data/Audio/SceneGameBGM.wav");

	switch (era)
	{
	case SceneGame::Era::Past:
	{
		// �X�e�[�W������
		StageManager& stageManager = StageManager::Instance();
		StagePast* stagePast = new StagePast();

		// ���v�̔z�u
		for (int i = 0; i < 5; ++i)
		{
			auto clock = new Clock(); // ���v�𐶐�
			auto longHand = new LongHand();
			auto shortHand = new ShortHand();

			float z = -9.95f;
			DirectX::XMFLOAT3 PastClockPositions[5] =
			{
			   { -4.0f, 2.0f,  z },    //1
			   { -8.0f,2.0f,  z },   // 2
			   {  -10.0f,4.0f, z },  // 3
			   { -12.0f, 2.0f, z },  // 4
			   {  -6.0f,4.0f, z }   // 5
			};

			DirectX::XMFLOAT3 position = PastClockPositions[i];
			clock->SetPosition(position);
			longHand->SetPosition(position);
			shortHand->SetPosition(position);

			// ���v�ɒ��j�ƒZ�j��ݒ�
			clock->SetLongHand(longHand);  // LongHand�����v�ɐݒ�
			clock->SetShortHand(shortHand); // ShortHand�����v�ɐݒ�

			// ClockManager �ɒǉ�
			ClockManager::Instance().AddClock(clock);
		}

		button = new Button();
		button->SetPosition({ -8.0f,0.0f,-6.0f });
		button->SetAngle({ 0.0f,DirectX::XM_PI / 2, 0.0f });

		LightManager::Instans().Register(new Light(LightType::Directional));

		//�_������ǉ�
		{
			Light* light = new Light(LightType::Point);
			light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
			light->SetRange(200.0f);
			LightManager::Instans().Register(light);
		}

		//�{�^���̈ʒu�ɓ_������ǉ�
		{
			Light* buttonLight = new Light(LightType::Point);
			buttonLight->SetColor(DirectX::XMFLOAT4(1, 0, 0, 1));
			buttonLight->SetPosition(button->GetPosition());
			buttonLight->SetRange(20.0f);
			LightManager::Instans().Register(buttonLight);
		}

		stageManager.Register(stagePast);

		StageElevator* stageElevator = new StageElevator();
		stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0));
		stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 5, 0));
		stageManager.Register(stageElevator);
	}
	break;
	case SceneGame::Era::Future:
	{
		StageManager& stageManager = StageManager::Instance();
		StageFuture* stageFuture = new StageFuture();
	
		// ���v�̔z�u
		for (int i = 0; i < 5; ++i)
		{
			auto clock = new Clock(); // ���v�𐶐�
			auto longHand = new LongHand();
			auto shortHand = new ShortHand();

			float z = -9.95f;
			DirectX::XMFLOAT3 FuterClockPositions[5] =
			{
			   { 4.0f, 2.0f,  z },    //1
			   { 8.0f,2.0f,  z },   // 2
			   {  10.0f,4.0f, z },  // 3
			   { 12.0f, 2.0f, z },  // 4
			   {  6.0f,4.0f, z }   // 5
			};

			DirectX::XMFLOAT3 position = FuterClockPositions[i];
			clock->SetPosition(position);
			longHand->SetPosition(position);
			shortHand->SetPosition(position);

			// ���v�ɒ��j�ƒZ�j��ݒ�
			clock->SetLongHand(longHand);  // LongHand�����v�ɐݒ�
			clock->SetShortHand(shortHand); // ShortHand�����v�ɐݒ�

			// �p�x��(0, 0, 0)�ɐݒ�
			clock->SetLongHandAngle(0);  // ���j�̊p�x��ݒ�
			clock->SetShortHandAngle(0); // �Z�j�̊p�x��ݒ�

			
			// ClockManager �ɒǉ�
			ClockManager::Instance().AddClock(clock);

			ItemManager& itemManager = ItemManager::Instance();

			itemPazzle4 = new ItemPazzle4();

			itemManager.Register(itemPazzle4);
		}
		
		button = new Button();
		button->SetPosition({ -8.0f,0.0f,-6.0f });
		button->SetAngle({ 0.0f,DirectX::XM_PI / 2, 0.0f });

	
		LightManager::Instans().Register(new Light(LightType::Directional));

		//�_������ǉ�
		{
			Light* futerLight = new Light(LightType::Point);
			futerLight->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
			LightManager::Instans().Register(futerLight);
		}

		//�{�^���̈ʒu�ɓ_������ǉ�
		{
			Light* futerbuttonLight = new Light(LightType::Point);
			futerbuttonLight->SetRange(0.1f);
			LightManager::Instans().Register(futerbuttonLight);
		}

		stageManager.Register(stageFuture);


		StageElevator* stageElevator = new StageElevator();
		stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0));
		stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 5, 0));
		stageManager.Register(stageElevator);
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

	//�J�����R���g���[���[������
	cameraController = new CameraController;
}

// �I����
void MidleStage::Finalize()
{
	//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

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

	if (button != nullptr)
	{
		delete button;
		button = nullptr;
	}

	ClockManager::Instance().Clear();

	//�X�e�[�W�I������
	StageManager::Instance().Clear();

	LightManager::Instans().Clear();
}

// �X�V����
void MidleStage::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	DirectX::XMFLOAT3 playerPos = player->GetPosition();
	static bool isButtonView = false; // ���݂̎��_��ǐ�
	Light* pointLight = LightManager::Instans().GetLight(1);
	// ���݂� Era �ɂ�鏈��
	switch (era)
	{
	case SceneGame::Era::Past:
		ClockManager::Instance().Update(elapsedTime);
		DirectX::XMFLOAT3 buttonPos = button->GetPosition();
		// ���_�؂�ւ�����
		DirectX::XMFLOAT3 targetView = button->PlayerVsButtonTargetView(playerPos, buttonPos, 5.0f, isButtonView);
		// �J�����R���g���[���[�̃^�[�Q�b�g��ݒ�
		cameraController->SetTarget(targetView);
		button->Update(elapsedTime);
	break;

	case SceneGame::Era::Future:
		ClockManager::Instance().Update(elapsedTime);
		cameraController->SetTarget(playerPos);
		break;
	}


	pointLight->SetPosition(player->GetPosition());
	
	cameraController->Update(elapsedTime);
	

	// �V�[���؂�ւ�����
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Past);
		SceneManager::Instance().ChangeScene(new SceneLoading(new MidleStage));
	}
	else if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Future);
		SceneManager::Instance().ChangeScene(new SceneLoading(new MidleStage));
	}

	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	// �v���C���[�X�V����
	if (!isButtonView)
	{
		player->Update(elapsedTime);

	}

	{
		Light* pointLight = LightManager::Instans().GetLight(0);
		pointLight->SetPosition(player->GetPosition());
	}

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void MidleStage::Render()
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
	LightManager::Instans().PushRenderContext(rc);

	LightManager::Instans().DrawDebugGUI();
	//�J���������ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//BGM�Đ�
	MidleStageBGM->Play(true);

	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader(ModelShaderId::LambartShader);
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�v���C���[�`��
		player->Render(dc, shader);

		switch (era)
		{
		case SceneGame::Era::Past:
			ClockManager::Instance().Render(dc, shader);
			button->Render(dc, shader);
			break;

		case SceneGame::Era::Future:
			ClockManager::Instance().Render(dc, shader);
			break;
		}

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}


	// 3D�f�o�b�O�`��
	{
		player->DrawDebugPrimitive();

		//button->DrawDebugPrimitive();
	
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
