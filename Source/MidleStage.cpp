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

// 初期化
void MidleStage::Initialize()
{

	era = EraManager::Instance().GetEra();

	EraManager::Instance().SetDifficulty(Stage::Difficulty::Middle);

	MidleStageBGM = Audio::Instance().LoadAudioSource("Data/Audio/SceneGameBGM.wav");

	switch (era)
	{
	case SceneGame::Era::Past:
	{
		// ステージ初期化
		StageManager& stageManager = StageManager::Instance();
		StagePast* stagePast = new StagePast();

		// 時計の配置
		for (int i = 0; i < 5; ++i)
		{
			auto clock = new Clock(); // 時計を生成
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

			// 時計に長針と短針を設定
			clock->SetLongHand(longHand);  // LongHandを時計に設定
			clock->SetShortHand(shortHand); // ShortHandを時計に設定

			// ClockManager に追加
			ClockManager::Instance().AddClock(clock);
		}

		button = new Button();
		button->SetPosition({ -8.0f,0.0f,-6.0f });
		button->SetAngle({ 0.0f,DirectX::XM_PI / 2, 0.0f });

		LightManager::Instans().Register(new Light(LightType::Directional));

		//点光源を追加
		{
			Light* light = new Light(LightType::Point);
			light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
			light->SetRange(200.0f);
			LightManager::Instans().Register(light);
		}

		//ボタンの位置に点光源を追加
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
	
		// 時計の配置
		for (int i = 0; i < 5; ++i)
		{
			auto clock = new Clock(); // 時計を生成
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

			// 時計に長針と短針を設定
			clock->SetLongHand(longHand);  // LongHandを時計に設定
			clock->SetShortHand(shortHand); // ShortHandを時計に設定

			// 角度を(0, 0, 0)に設定
			clock->SetLongHandAngle(0);  // 長針の角度を設定
			clock->SetShortHandAngle(0); // 短針の角度を設定

			
			// ClockManager に追加
			ClockManager::Instance().AddClock(clock);

			ItemManager& itemManager = ItemManager::Instance();

			itemPazzle4 = new ItemPazzle4();

			itemManager.Register(itemPazzle4);
		}
		
		button = new Button();
		button->SetPosition({ -8.0f,0.0f,-6.0f });
		button->SetAngle({ 0.0f,DirectX::XM_PI / 2, 0.0f });

	
		LightManager::Instans().Register(new Light(LightType::Directional));

		//点光源を追加
		{
			Light* futerLight = new Light(LightType::Point);
			futerLight->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
			LightManager::Instans().Register(futerLight);
		}

		//ボタンの位置に点光源を追加
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

	
	//プレイヤー初期化
	player = new Player;
	
	//カメラ初期設定
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

	//カメラコントローラー初期化
	cameraController = new CameraController;
}

// 終了化
void MidleStage::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}

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

	if (button != nullptr)
	{
		delete button;
		button = nullptr;
	}

	ClockManager::Instance().Clear();

	//ステージ終了処理
	StageManager::Instance().Clear();

	LightManager::Instans().Clear();
}

// 更新処理
void MidleStage::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	DirectX::XMFLOAT3 playerPos = player->GetPosition();
	static bool isButtonView = false; // 現在の視点を追跡
	Light* pointLight = LightManager::Instans().GetLight(1);
	// 現在の Era による処理
	switch (era)
	{
	case SceneGame::Era::Past:
		ClockManager::Instance().Update(elapsedTime);
		DirectX::XMFLOAT3 buttonPos = button->GetPosition();
		// 視点切り替え処理
		DirectX::XMFLOAT3 targetView = button->PlayerVsButtonTargetView(playerPos, buttonPos, 5.0f, isButtonView);
		// カメラコントローラーのターゲットを設定
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
	

	// シーン切り替え処理
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

	// ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	// プレイヤー更新処理
	if (!isButtonView)
	{
		player->Update(elapsedTime);

	}

	{
		Light* pointLight = LightManager::Instans().GetLight(0);
		pointLight->SetPosition(player->GetPosition());
	}

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
}

// 描画処理
void MidleStage::Render()
{
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
	LightManager::Instans().PushRenderContext(rc);

	LightManager::Instans().DrawDebugGUI();
	//カメラ初期設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//BGM再生
	MidleStageBGM->Play(true);

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader(ModelShaderId::LambartShader);
		shader->Begin(dc, rc);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//プレイヤー描画
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

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}


	// 3Dデバッグ描画
	{
		player->DrawDebugPrimitive();

		//button->DrawDebugPrimitive();
	
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{

	}

	// 2DデバッグGUI描画
	{

	}
}
