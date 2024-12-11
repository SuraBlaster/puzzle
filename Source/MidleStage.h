#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Stage.h"
#include "Clock.h"
#include "Button.h"
#include "Light.h"
#include "SceneGame.h"

// ゲームシーン
class MidleStage : public Scene
{
public:

	MidleStage() = default;
	~MidleStage() override = default;

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;


private:
    Player* player = nullptr;
    CameraController* cameraController = nullptr;
    Sprite* gauge = nullptr;
    Stage* stage = nullptr;
	Button* button = nullptr;
    SceneGame::Era era = SceneGame::Era::Past;
	Light* light = new Light(LightType::Point);
    int clickCount = 0;
    int textureWidth = 0; // テクスチャの幅
    int gaugeWidth = 0;   // 現在のゲージの幅
    float Length = 5.0f;  // 距離間
};

