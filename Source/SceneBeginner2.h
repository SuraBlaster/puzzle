#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "SceneGame.h"

//初級２
class SceneBeginner2 : public Scene
{
public:
	SceneBeginner2() {}
	~SceneBeginner2() override {}

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

};
