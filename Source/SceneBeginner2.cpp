#include "Graphics/Graphics.h"
#include "SceneManager.h"
#include "SceneBeginner2.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "ItemManager.h"
#include "EffectManager.h"
#include "StageManager.h"
#include "StagePast.h"
#include "StageFuture.h"
#include "StageElevator.h"
#include "StageFlowerPot.h"
#include "EraManager.h"
#include <Input/Input.h>
// 初期化
void SceneBeginner2::Initialize()
{

	era = EraManager::Instance().GetEra();
	EraManager::Instance().SetDifficulty(Stage::Difficulty::Beginner2);

	switch (era)
	{
	case SceneGame::Era::Past:
	{
		// ステージ初期化
		StageManager& stageManager = StageManager::Instance();
		StagePast* stagePast = new StagePast();
		stageManager.Register(stagePast);

		StageFlowerPot* stageFlowerPot = new StageFlowerPot();
		stageManager.Register(stageFlowerPot);
		
	}
	break;
	case SceneGame::Era::Future:
	{
		StageManager& stageManager = StageManager::Instance();
		StageFuture* stageFuture = new StageFuture();
		stageManager.Register(stageFuture);
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

	//カメラコントローラー初期化
	cameraController = new CameraController;

}

// 終了化
void SceneBeginner2::Finalize()
{

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
}

// 更新処理
void SceneBeginner2::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();
	ItemManager& itemManager = ItemManager::Instance();

	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//シーン切り替え処理
	if (gamepad.GetButtonDown() & GamePad::BTN_A)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Past);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBeginner2));
	}
	else if (gamepad.GetButtonDown() & GamePad::BTN_B)
	{
		EraManager::Instance().SetEra(SceneGame::Era::Future);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBeginner2));
	}

	if (gamepad.GetButtonDown() & GamePad::BTN_Y && EraManager::Instance().GetPlayerHasBattery() == true)
	{
		EraManager::Instance().SetPlayerHasBattery(false);

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


// 描画処理
void SceneBeginner2::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0〜1.0)
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

		ItemManager::Instance().DrawDebugPrimitive();
	}

	// 2Dスプライト描画
	{
	}


	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("Flag", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}
	}

	ImGui::End();

	// 2DデバッグGUI描画
	{
		player->DrawDebugGUI();
	}
}
