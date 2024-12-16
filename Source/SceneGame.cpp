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

// 初期化
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
			// ステージ初期化
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
			
			//点光源を追加
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

			//アイテム初期化
			ItemManager& itemManager = ItemManager::Instance();

			itemContainer = new ItemContainer();
			itemSeed = new ItemSeed();
			itemPazzle1 = new ItemPazzle1();

			itemManager.Register(itemContainer);
			itemManager.Register(itemSeed);
			itemManager.Register(itemPazzle1);


			//点光源を追加
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				LightManager::Instans().Register(light);
			}
		}
		break;
	}
	//プレイヤー初期化
	player = new Player;

	
	
	//カメラ初期設定
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

	//カメラコントローラー初期化
	cameraController = new CameraController;

	//ゲージスプライト
	gauge = new Sprite();
}

// 終了化
void SceneGame::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}


	ItemManager::Instance().Clear();

	//カメラコントローラー終了処理
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	//プレイヤー終了処理
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	//ステージ終了処理
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

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	ItemManager& itemManager = ItemManager::Instance();

	{
		Light* pointLight = LightManager::Instans().GetLight(1);
		pointLight->SetPosition(player->GetPosition());
	}

	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//シーン切り替え処理
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

	//バッテリー切り替え処理
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


	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//アイテム更新処理
	ItemManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
}

void SceneGame::InsertBattery(const char* nodeName, float nodeRadius)
{
	//ノードの位置と当たり判定を行う
	Model::Node* node = stageBatterySlot->GetModel()->FindNode(nodeName);

	if (node != nullptr && stageBatterySlot->scale.x > 0.0f)
	{
		//ノードのワールド座標
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
	//ノードの位置と当たり判定を行う
	Model::Node* nodePast = stageBatteryPast->GetModel()->FindNode(nodeName);
	Model::Node* nodeFuture = stageBatteryFuture->GetModel()->FindNode(nodeName);

	if (nodePast != nullptr && stageBatteryPast->scale.x > 0.0f)
	{
		//ノードのワールド座標
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
			//ノードのワールド座標
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


// 描画処理
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

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	{
		LightManager::Instans().PushRenderContext(rc);

		LightManager::Instans().DrawDebugGUI();
	}

	//カメラ初期設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader(ModelShaderId::LambartShader);
		shader->Begin(dc, rc);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//プレイヤー描画
		player->Render(dc, shader);


		ItemManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	

	// 3Dデバッグ描画
	{
		player->DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);

		//ItemManager::Instance().DrawDebugPrimitive();
	}

	// 2Dスプライト描画
	{
		
	}


	

	// 2DデバッグGUI描画
	{
		player->DrawDebugGUI();
	}
}


