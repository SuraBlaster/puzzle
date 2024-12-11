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
void SceneCube::Initialize()
{
	
	era = EraManager::Instance().GetEra();

	//キューブ初期化
	StageCubeManager::Instance().Initialize();

	switch (era)
	{
	case SceneGame::Era::Past:
		{
			// ステージ初期化
			StageManager& stageManager = StageManager::Instance();
			StagePast* stagePast = new StagePast();
			stageManager.Register(stagePast);

			StageElevator* stageElevator = new StageElevator();
			stageElevator->SetStartPoint(DirectX::XMFLOAT3(0, -1, 0));
			stageElevator->SetGoalPoint(DirectX::XMFLOAT3(0, 9, 0));
			stageManager.Register(stageElevator);


			//点光源を追加
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				LightManager::Instans().Register(light);
			}

			//キューブの位置に点光源を追加
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

			//点光源を追加
			{
				Light* light = new Light(LightType::Point);
				light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
				light->SetRange(0);
				LightManager::Instans().Register(light);
			}

			//キューブの位置に点光源を追加
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

	
	LightManager::Instans().Register(new Light(LightType::Directional));


	//カメラコントローラー初期化
	cameraController = new CameraController;
}

// 終了化
void SceneCube::Finalize()
{

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

	LightManager::Instans().Clear();

}

// 更新処理
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
	cameraController->SetIsCubeView(isCubeView);  // 状態を反映
	cameraController->SetTarget(targetPos);
	StageRootCube* rootCube = StageCubeManager::Instance().GetRootCube();
    if (rootCube)
    {
        rootCube->SetIsCubeView(isCubeView);  // isCubeView状態をStageRootCubeに適用
    }
	
	//カメラコントローラー更新処理
	cameraController->Update(elapsedTime);

	//シーン切り替え
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

	//ステージ更新処理
	StageManager::Instance().Update(elapsedTime);

	//プレイヤー更新処理
	if (!isCubeView)
	{
		player->Update(elapsedTime);
	}

	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	//キューブ更新処理
	StageCubeManager::Instance().GetRootCube()->Update(elapsedTime);
}

// 描画処理
void SceneCube::Render()
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

		LightManager::Instans().DrawDebugPrimitive();
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



