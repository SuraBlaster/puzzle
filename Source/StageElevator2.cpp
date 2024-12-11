#include "StageElevator2.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "EraManager.h"

StageElevator2::StageElevator2()
{
	scale.x = scale.y = scale.z = 0.5f;
	position.y = 8.9;
	position.z = -12.5;
	angle.y = DirectX::XMConvertToRadians(0);
	//ステージモデルを読み込み
	model = new Model("Data/Model/Elevator/エレベーター.mdl");


	//UpdateOpenState();
}

StageElevator2::~StageElevator2()
{
	//ステージモデルを破棄
	delete model;
}

void StageElevator2::Update(float elapsedTime)
{
	era = EraManager::Instance().GetEra();
	difficulty = EraManager::Instance().GetDifficulty();

	switch (difficulty)
	{
	case Difficulty::Tutorial:
		switch (era)
		{
		case SceneGame::Era::Future:
			if (EraManager::Instance().GetBatteryFuture())
			{
				angle.y = DirectX::XMConvertToRadians(180);


				if (CollisionNodeVsPlayer("ElevatorBox", 1.0f))
				{
					Player& player = Player::Instance();
					player.SetPosition({ 0,0,-9 });
				}
			}
			break;
		}
		break;

	}



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



	// 前回の情報を保存 
	oldTransform = transform;
	oldAngle = angle;

	// 行列更新
	UpdateTransform();

	// モデルアニメーション更新処理
	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);

	// レイキャスト用にモデル空間表列にするために単位行列を渡す
	const DirectX::XMFLOAT4X4 transformIdentity = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	model->UpdateTransform(transformIdentity);
}

void StageElevator2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// 表示用のためワールド行列に更新する
	model->UpdateTransform(transform);

	shader->Draw(dc, model);
}

bool StageElevator2::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	// 前回のワールド行列と逆行列化する
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));

	// 前回のローカル空間でレイに変換
	DirectX::XMVECTOR RayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR RayEnd = DirectX::XMLoadFloat3(&end);
	RayStart = DirectX::XMVector3TransformCoord(RayStart, WorldTransform);
	RayEnd = DirectX::XMVector3TransformCoord(RayEnd, WorldTransform);
	// ローカル空間でのレイとの交点を求める
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, RayStart);
	DirectX::XMStoreFloat3(&localEnd, RayEnd);

	HitResult localHit;
	if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
	{
		DirectX::XMVECTOR hitPosition = DirectX::XMLoadFloat3(&localHit.position);
		hitPosition = DirectX::XMVector3TransformCoord(hitPosition, DirectX::XMLoadFloat4x4(&transform));

		// 変換された交点を格納
		DirectX::XMStoreFloat3(&hit.position, hitPosition);

		// 衝突情報を設定
		hit.normal = localHit.normal;
		hit.distance = localHit.distance;

		return true;
	}
	return false;

}

void StageElevator2::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}

void StageElevator2::IdleState()
{
	state = State::Idle;
}

// 通常状態更新処理
void StageElevator2::UpdateIdleState()
{
	if (CollisionNodeVsPlayer("ElevatorBox", 3.0f))
	{
		OpenState();
	}
}

void StageElevator2::OpenState()
{
	state = State::Open;

	model->PlayAnimation(Anim_Open, false);

	if (timer <= 0)
	{
		timer = 100;
	}

}

// 開更新処理
void StageElevator2::UpdateOpenState()
{
	timer--;

	if (!model->IsPlayAnimation() && timer <= 0)
	{
		CloseState();
	}
}


void StageElevator2::CloseState()
{
	state = State::Close;

	model->PlayAnimation(Anim_Close, false);
}

// 開閉更新処理
void StageElevator2::UpdateCloseState()
{
	if (!model->IsPlayAnimation() && !CollisionNodeVsPlayer("ElevatorBox", 1.0f))
	{
		IdleState();
	}
}


bool StageElevator2::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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