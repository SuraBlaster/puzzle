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

// �Q�[���V�[��
class MidleStage : public Scene
{
public:

	MidleStage() = default;
	~MidleStage() override = default;

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
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
    int textureWidth = 0; // �e�N�X�`���̕�
    int gaugeWidth = 0;   // ���݂̃Q�[�W�̕�
    float Length = 5.0f;  // ������
};

