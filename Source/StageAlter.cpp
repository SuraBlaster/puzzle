#include "StageAlter.h"
#include "Player.h"
#include "EraManager.h"

StageAlter::StageAlter()
{
    model = new Model("Data/Model/Stage/Alter.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 10;

    position.z = 5;

    angle.y += DirectX::XMConvertToDegrees(-45);

	state = State::Idle;
}

StageAlter::~StageAlter()
{
    delete model;
}

void StageAlter::Update(float elapsedTime)
{

	switch (state)
	{
	case State::Idle:
		UpdateIdleState();
		break;
	case State::Close:
		UpdateCloseState();
		break;
	case State::Open:
		UpdateOpenState();
		break;
	}

    UpdateTransform();

	// モデルアニメーション更新処理
	model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);

}

void StageAlter::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageAlter::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

bool StageAlter::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
			return true;
		}
	}

	return false;
}

void StageAlter::UpdateIdleState()
{
	if (CollisionNodeVsPlayer("Alter", 5.0f))
	{
		OpenState();
	}
}

void StageAlter::UpdateCloseState()
{
	if (!model->IsPlayAnimation())
	{
		IdleState();
	}
}

void StageAlter::UpdateOpenState()
{
	if (!CollisionNodeVsPlayer("Alter", 5.0f))
	{
		CloseState();
	}
}

void StageAlter::IdleState()
{
	state = State::Idle;
}

void StageAlter::CloseState()
{
	state = State::Close;

	model->PlayAnimation(Anim_Close, false);
}

void StageAlter::OpenState()
{
	state = State::Open;

	model->PlayAnimation(Anim_Open, false);
}
