#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "SceneGame.h"
#include "ItemPazzle2.h"
#include "Audio/Audio.h"
//�����Q
class SceneBeginner2 : public Scene
{
public:
	SceneBeginner2() {}
	~SceneBeginner2() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

	SceneGame::Era era;
private:

	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	ItemPazzle2* itemPazzle2 = nullptr;

	std::unique_ptr<AudioSource> GameBGM;
};
