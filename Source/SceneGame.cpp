#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "ItemManager.h"
#include "ItemBattery.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageMoveFloor.h"
#include "StageElevator.h"
#include "StageElevator2.h"
#include "EraManager.h"
#include "SceneAdvanced.h"
#include <Input/Input.h>
#include "LightManager.h"

// ������
void SceneGame::Initialize()
{
	
	era = EraManager::Instance().GetEra();
	EraManager::Instance().SetDifficulty(Stage::Difficulty::Tutorial);
	GameBGM = Audio::Instance().LoadAudioSource("Data/Audio/SceneTitle.wav");
	GameBGM->Play(true);
	switch (era)
	{
	case Era::Past:
		{
			// �X�e�[�W������
			StageManager& stageManager = StageManager::Instance();
			StagePast* stagePast = new StagePast();
			stageManager.Register(stagePast);

			stageBatteryPast = new StageBatteryPast();
			stageBatteryFuture = new StageBatteryFuture();
			stageBatterySlot = new StageBatterySlot();

			stageManager.Register(stageBatteryPast);
			stageManager.Register(stageBatterySlot);

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
	case Era::Future:
		{
			StageManager& stageManager = StageManager::Instance();
			StageFuture* stageFuture = new StageFuture();
			stageManager.Register(stageFuture);

			stageBatteryPast = new StageBatteryPast();
			stageBatteryFuture = new StageBatteryFuture();
			stageBatterySlot = new StageBatterySlot();

			stageManager.Register(stageBatteryFuture);
			stageManager.Register(stageBatterySlot);

			stageContainer = new StageContainer();
			stageManager.Register(stageContainer);

			StageElevator* stageElevator = new StageElevator();
			stageManager.Register(stageElevator);

			StageElevator2* stageElevator2 = new StageElevator2();
			stageManager.Register(stageElevator2);

			//�A�C�e��������
			ItemManager& itemManager = ItemManager::Instance();

			itemContainer = new ItemContainer();
			itemSeed = new ItemSeed();
			itemPazzle1 = new ItemPazzle1();

			itemManager.Register(itemContainer);
			itemManager.Register(itemSeed);
			itemManager.Register(itemPazzle1);


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

	//�Q�[�W�X�v���C�g
	gauge = new Sprite();
}

// �I����
void SceneGame::Finalize()
{
	//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}


	ItemManager::Instance().Clear();

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
	switch (era)
	{
	case Era::Past:
		delete stageBatteryFuture;
		break;
	case Era::Future:
		delete stageBatteryPast;
		break;
	}

	StageManager::Instance().Clear();

	LightManager::Instans().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
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
		EraManager::Instance().SetEra(Era::Past);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
	else if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		EraManager::Instance().SetEra(Era::Future);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}

	//�o�b�e���[�؂�ւ�����
	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{

		if (EraManager::Instance().GetPlayerHasBattery())
		{
			InsertBattery("BatterySlot", 2.0f);
		}
		else if (!EraManager::Instance().GetPlayerHasBattery())
		{
			RemoveBattery("Battery", 1.0f);
		}

		if (EraManager::Instance().GetContainer() == true)
		{

			DirectX::XMFLOAT3 fowardPos = player->GetForwardPosition(2.0f);

			itemContainer->SetPosition({
				fowardPos.x,
				fowardPos.y,
				fowardPos.z 
				});
			itemContainer->SetScale({ 0.003f,0.003f,0.003f });
		}
	}


	//�X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);

	//�v���C���[�X�V����
	player->Update(elapsedTime);

	//�A�C�e���X�V����
	ItemManager::Instance().Update(elapsedTime);

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

void SceneGame::InsertBattery(const char* nodeName, float nodeRadius)
{
	//�m�[�h�̈ʒu�Ɠ����蔻����s��
	Model::Node* node = stageBatterySlot->GetModel()->FindNode(nodeName);

	if (node != nullptr && stageBatterySlot->scale.x > 0.0f)
	{
		//�m�[�h�̃��[���h���W
		DirectX::XMFLOAT3 nodePosition(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);

		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			switch (era)
			{
			case Era::Past:
				{
					EraManager::Instance().SetPlayerHasBattery(false);

					if (EraManager::Instance().GetBatteryPast() == true) 
					{
						EraManager::Instance().SetBatteryPast(false);
					}
					else 
					{
						EraManager::Instance().SetBatteryPast(true);
					}
				}
				break;
			case Era::Future:
				{
					EraManager::Instance().SetPlayerHasBattery(false);

					if (EraManager::Instance().GetBatteryFuture() == true) {
						EraManager::Instance().SetBatteryFuture(false);
					}
					else {
						EraManager::Instance().SetBatteryFuture(true);
					}
				}
			}
		}
	}
}

void SceneGame::RemoveBattery(const char* nodeName, float nodeRadius)
{
	//�m�[�h�̈ʒu�Ɠ����蔻����s��
	Model::Node* nodePast = stageBatteryPast->GetModel()->FindNode(nodeName);
	Model::Node* nodeFuture = stageBatteryFuture->GetModel()->FindNode(nodeName);

	if (nodePast != nullptr && stageBatteryPast->scale.x > 0.0f)
	{
		//�m�[�h�̃��[���h���W
		DirectX::XMFLOAT3 nodePosition(
			nodePast->worldTransform._41,
			nodePast->worldTransform._42,
			nodePast->worldTransform._43
		);

		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			EraManager::Instance().SetPlayerHasBattery(true);
		}
		else if (nodeFuture != nullptr && stageBatteryFuture->scale.x > 0.0f)
		{
			//�m�[�h�̃��[���h���W
			DirectX::XMFLOAT3 nodePosition(
				nodeFuture->worldTransform._41,
				nodeFuture->worldTransform._42,
				nodeFuture->worldTransform._43
			);

			Player& player = Player::Instance();
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				nodePosition,
				nodeRadius,
				player.GetPosition(),
				player.GetRadius(),
				player.GetHeight(),
				outPosition))
			{
				EraManager::Instance().SetPlayerHasBattery(true);
			}
		}
	}
	
}


// �`�揈��
void SceneGame::Render()
{
	bool BatteryPast = EraManager::Instance().GetBatteryPast();
	bool BatteryFuture = EraManager::Instance().GetBatteryFuture();
	bool PlayerGetBattery = EraManager::Instance().GetPlayerHasBattery();
	bool GetContainer = EraManager::Instance().GetContainer();

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


		ItemManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	

	// 3D�f�o�b�O�`��
	{
		player->DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//ItemManager::Instance().DrawDebugPrimitive();
	}

	// 2D�X�v���C�g�`��
	{
		
	}


	

	// 2D�f�o�b�OGUI�`��
	{
		player->DrawDebugGUI();
	}
}


