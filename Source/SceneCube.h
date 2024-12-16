#pragma once

#include "Player.h"
#include "Input/Cube.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Stage.h"
#include "Input/Cube.h"
#include "Light.h"
#include "Audio/Audio.h"
#include "ItemPazzle3.h"
//�r���[�|�[�g�͈̔͂̍\����
struct  Viewport
{
	float x;
	float y;
	float Width;
	float Height;
	float MinZ;
	float MaxZ;
};

// �Q�[���V�[��
class SceneCube : public Scene
{
public:
	SceneCube() {}
	~SceneCube() override {}

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

	//���[���h���W����X�N���[�����W�ւ̕ϊ����s���֐�
	DirectX::XMFLOAT2 WorldToScreen(const DirectX::XMFLOAT3& worldPos, const Viewport& viewport, const DirectX::FXMMATRIX projection, const DirectX::FXMMATRIX& view, const DirectX::FXMMATRIX world)
	{
		//�X�N���[�����W���v�Z
		DirectX::XMVECTOR screenPosVec = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&worldPos),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projection,
			view,
			world
		);

		//XMVECTOR����XMFLOAT2�ɕϊ�
		DirectX::XMFLOAT2 screenPos;
		screenPos.x = DirectX::XMVectorGetX(screenPosVec);
		screenPos.y = DirectX::XMVectorGetY(screenPosVec);
		return screenPos;
	};


	//�X�N���[�����W���烏�[���h���W�ւ̕ϊ�������֐�
	DirectX::XMFLOAT3 ScreenToWorld(float x, float y, float z, const Viewport& viewport, const DirectX::FXMMATRIX projectile, const DirectX::FXMMATRIX view, const DirectX::FXMMATRIX world)
	{
		//���[���h���W���v�Z
		DirectX::XMVECTOR worldPosVec = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)),
			viewport.x,
			viewport.y,
			viewport.Width,
			viewport.Height,
			viewport.MinZ,
			viewport.MaxZ,
			projectile,
			view,
			world
		);

		//XMVECTOR����XMFLOAT3�ɕϊ�
		DirectX::XMFLOAT3 worldPos;
		DirectX::XMStoreFloat3(&worldPos, worldPosVec);

		return worldPos;
	};

private:
	Player* player = nullptr;

	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	Stage* stage = nullptr;

	Light* light = new Light(LightType::Point);

	std::unique_ptr<Light> direction_light;

	std::unique_ptr<AudioSource> SceneGameBGM;

	ItemPazzle3* itemPazzle3 = nullptr;

	int clickCount = 0;
	int textureWidth; // �e�N�X�`���̕�
	int gaugeWidth;   // ���݂̃Q�[�W�̕�
	float Length = 5.0f;//������
	
};
