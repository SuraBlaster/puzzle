#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "StageBatteryPast.h"
#include "StageBatteryFuture.h"
#include "StageBatterySlot.h"
#include "StageContainer.h"
// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame(){}
	~SceneGame() override{}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

	//バッテリー入れ替え処理
	void InsertBattery(const char* nodeName,float nodeRadius);

	void RemoveBattery(const char* nodeName, float nodeRadius);

	enum class Era
	{
		Past,
		Future
	};

	Era era;

private:
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

private:

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	StageBatteryPast* stageBatteryPast = nullptr;

	StageBatteryFuture* stageBatteryFuture = nullptr;

	StageBatterySlot* stageBatterySlot = nullptr;

	StageContainer* stageContainer = nullptr;

};
