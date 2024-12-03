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
#include "ItemContainer.h"
#include "ItemSeed.h"
// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame(){}
	~SceneGame() override{}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

	//�o�b�e���[����ւ�����
	void InsertBattery(const char* nodeName,float nodeRadius);

	void RemoveBattery(const char* nodeName, float nodeRadius);

	
	enum class Era
	{
		Past,
		Future
	};

	Era era;

private:

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	StageBatteryPast* stageBatteryPast = nullptr;

	StageBatteryFuture* stageBatteryFuture = nullptr;

	StageBatterySlot* stageBatterySlot = nullptr;

	StageContainer* stageContainer = nullptr;

	ItemContainer* itemContainer = nullptr;

	ItemSeed* itemSeed = nullptr;
};
