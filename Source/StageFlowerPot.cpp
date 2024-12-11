#include "StageFlowerPot.h"
#include "Player.h"
#include "EraManager.h"
#include "Input/Input.h"

StageFlowerPot::StageFlowerPot()
{
    model = new Model("Data/Model/Stage/FlowerPot.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = -8;

    position.y = 0;
}

StageFlowerPot::~StageFlowerPot()
{
    delete model;
}

void StageFlowerPot::Update(float elapsedTime)
{
	GamePad& gamepad = Input::Instance().GetGamePad();

	if (gamepad.GetButtonDown() & GamePad::BTN_Y)
	{
		CollisionNodeVsPlayer("Pot", 1.0f);
	}

    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageFlowerPot::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageFlowerPot::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageFlowerPot::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	//ノードの位置と当たり判定を行う
	Model::Node* node = model->FindNode(nodeName);

	if (node != nullptr)
	{
		//ノードのワールド座標
		DirectX::XMFLOAT3 nodePosition(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);

		Player& player = Player::Instance();
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsCylinder(
			nodePosition,
			nodeRadius,
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{
			EraManager::Instance().SetPlayerSeed(false);
		}
	}
}