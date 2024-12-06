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

//ã‹‰
class SceneAdvanced : public Scene
{
public:
	SceneAdvanced() {}
	~SceneAdvanced() override {}

	// ‰Šú‰»
	void Initialize()override;

	// I—¹‰»
	void Finalize()override;

	// XVˆ—
	void Update(float elapsedTime)override;

	// •`‰æˆ—
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
