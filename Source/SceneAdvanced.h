#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "SceneGame.h"
#include "StagePazzle1.h"
#include "StagePazzle2.h"
#include "StagePazzle3.h"
#include "StagePazzle4.h"

//上級
class SceneAdvanced : public Scene
{
public:
	SceneAdvanced() {}
	~SceneAdvanced() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

	SceneGame::Era era;
private:

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	StagePazzle1* stagePazzle1;

	StagePazzle2* stagePazzle2;

	StagePazzle3* stagePazzle3;

	StagePazzle4* stagePazzle4;

};
