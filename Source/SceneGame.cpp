#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageMoveFloor.h"
#include "StageElevator.h"
#include "EraManager.h"
#include <Input/Input.h>
#include "Input/Cube.h"

//using namespace DirectX; 
//これを書くとDirectX::が不必要
// D3D11_VIEWPORT型のデータをViewport型に変換する関数
Viewport ConvertD3D11ViewportToViewport(const D3D11_VIEWPORT& d3dViewport)
{
	return Viewport{
		d3dViewport.TopLeftY,// Y軸の最上端
		d3dViewport.TopLeftX,// X軸の左端
		d3dViewport.Width,   // 幅
		d3dViewport.Height,  // 高さ
		d3dViewport.MinDepth,// 最小深度
		d3dViewport.MaxDepth // 最大深度
	};
}

// 初期化
void SceneGame::Initialize()
{
	
	era = EraManager::Instance().GetEra();


	switch (era)
	{
	case Era::Past:
		{
			// ステージ初期化
			StageManager& stageManager = StageManager::Instance();
			StagePast* stagePast = new StagePast();
			stageManager.Register(stagePast);

			StageElevator* stageElevator = new StageElevator();
			stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0));
			stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 5, 0));
			stageManager.Register(stageElevator);
		}
		break;
	case Era::Future:
		{
			StageManager& stageManager = StageManager::Instance();
			StageFuture* stageFuture = new StageFuture();
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

	//エネミー初期化
	/*EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 1; ++i) 
	{
		EnemySlime* slime = new EnemySlime;
		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		enemyManager.Register(slime);
	}*/
	
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

	//カメラコントローラー初期化
	cameraController = new CameraController;


	StageCubeManager::Instance().Initialize();
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

	EnemyManager::Instance().Clear();

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
	StageManager::Instance().Clear();

	StageCubeManager::Instance().Finalize();

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//シーン切り替え
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

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	player->Update(elapsedTime);

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	//キューブ更新処理
	StageCubeManager::Instance().GetRootCube()->Update(elapsedTime);
}

// 描画処理
void SceneGame::Render()
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
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラ初期設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//プレイヤー描画
		player->Render(dc, shader);

		//キューブ描画
		StageCubeManager::Instance().GetRootCube()->Render(dc, shader);

		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
	

	// 3Dデバッグ描画
	{
		EnemyManager::Instance().DrawDebugPrimitive();

		player->DrawDebugPrimitive();
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
		player->DrawDebugGUI();
	}
}



