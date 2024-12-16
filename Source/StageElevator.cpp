#include "StageElevator.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Graphics.h"
#include "Player.h"
#include "EraManager.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneCube.h"
#include "SceneBeginner2.h"
#include "SceneAdvanced.h"
#include "MidleStage.h"
StageElevator::StageElevator()
{
	scale.x = scale.y = scale.z = 0.5f;
	position.y = -0.1;
	position.z = -12.5;
	angle.y = DirectX::XMConvertToRadians(0);
	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Elevator/�G���x�[�^�[.mdl");

	
}

StageElevator::~StageElevator()
{
	//�X�e�[�W���f����j��
	delete model;
}

void StageElevator::Update(float elapsedTime)
{
	era = EraManager::Instance().GetEra();
	difficulty = EraManager::Instance().GetDifficulty();

	switch (era)
	{
	case SceneGame::Era::Future:
		switch (difficulty)
		{
		case Difficulty::Tutorial:

			if (EraManager::Instance().GetBatteryFuture())
			{
				angle.y = DirectX::XMConvertToRadians(180);

				if (CollisionNodeVsPlayer("ElevatorBox", 1.0f) && EraManager::Instance().GetPlayerPazzle1())
				{
					EraManager::Instance().SetEra(SceneGame::Era::Future);
					Player& player = Player::Instance();
					player.SetPosition({ 0,0,-9 });
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneBeginner2));
				}
				else if (CollisionNodeVsPlayer("ElevatorBox", 1.0f))
				{
					Player& player = Player::Instance();
					player.SetPosition({ 0,10,-9 });
				}
			}
			break;

		case Difficulty::Begginer1:

			if (EraManager::Instance().GetPlayerSeed() == false)
			{
				angle.y = DirectX::XMConvertToRadians(180);

				if (CollisionNodeVsPlayer("ElevatorBox", 1.0f) && EraManager::Instance().GetPlayerPazzle2())
				{
					EraManager::Instance().SetEra(SceneGame::Era::Future);
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneCube));
				}
			}
			break;

		case Difficulty::Beginner2:

			if (EraManager::Instance().GetCubeFlag() == true)
			{
				angle.y = DirectX::XMConvertToRadians(180);

				if (CollisionNodeVsPlayer("ElevatorBox", 1.0f) && EraManager::Instance().GetPlayerPazzle3())
				{
					EraManager::Instance().SetEra(SceneGame::Era::Future);
					SceneManager::Instance().ChangeScene(new SceneLoading(new MidleStage));
				}
			}
			break;

		case Difficulty::Middle:
			if (EraManager::Instance().GetClockFlag())
			{
				angle.y = DirectX::XMConvertToRadians(180);

				if (CollisionNodeVsPlayer("ElevatorBox", 1.0f) && EraManager::Instance().GetPlayerPazzle4())
				{
					EraManager::Instance().SetEra(SceneGame::Era::Future);
					SceneManager::Instance().ChangeScene(new SceneLoading(new SceneAdvanced));
				}
			}
			break;
		}

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

	

	// �O��̏���ۑ� 
	oldTransform = transform;
	oldAngle = angle;

	// �s��X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);

	// ���C�L���X�g�p�Ƀ��f����ԕ\��ɂ��邽�߂ɒP�ʍs���n��
	const DirectX::XMFLOAT4X4 transformIdentity = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	
	model->UpdateTransform(transformIdentity);
}

void StageElevator::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// �\���p�̂��߃��[���h�s��ɍX�V����
	model->UpdateTransform(transform);

	shader->Draw(dc, model);

	StageElevator::DrawDebugPrimitive();
}

bool StageElevator::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	// �O��̃��[���h�s��Ƌt�s�񉻂���
	DirectX::XMMATRIX WorldTransform = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));
	
	// �O��̃��[�J����ԂŃ��C�ɕϊ�
	DirectX::XMVECTOR RayStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR RayEnd = DirectX::XMLoadFloat3(&end);
	RayStart = DirectX::XMVector3TransformCoord(RayStart, WorldTransform);
	RayEnd = DirectX::XMVector3TransformCoord(RayEnd, WorldTransform);
	// ���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, RayStart);
	DirectX::XMStoreFloat3(&localEnd, RayEnd);

	HitResult localHit;
	if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
	{
		DirectX::XMVECTOR hitPosition = DirectX::XMLoadFloat3(&localHit.position);
		hitPosition = DirectX::XMVector3TransformCoord(hitPosition, DirectX::XMLoadFloat4x4(&transform));

		// �ϊ����ꂽ��_���i�[
		DirectX::XMStoreFloat3(&hit.position, hitPosition);

		// �Փˏ���ݒ�
		hit.normal = localHit.normal;
		hit.distance = localHit.distance;

		return true;
	}
	return false;

}

void StageElevator::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}

void StageElevator::IdleState()
{
	state = State::Idle;
}

// �ʏ��ԍX�V����
void StageElevator::UpdateIdleState()
{
	if (CollisionNodeVsPlayer("ElevatorBox", 3.0f))
	{
		OpenState();
	}
}

void StageElevator::OpenState()
{
	state = State::Open;

	model->PlayAnimation(Anim_Open, false);

	if (timer <= 0)
	{
		timer = 100;
	}
	
}

// �J�X�V����
void StageElevator::UpdateOpenState()
{
	timer--;

	if (!model->IsPlayAnimation() && timer <= 0)
	{
		CloseState();
	}
}


void StageElevator::CloseState()
{
	state = State::Close;

	model->PlayAnimation(Anim_Close, false);
}

// �J�X�V����
void StageElevator::UpdateCloseState()
{
	if (!model->IsPlayAnimation() && !CollisionNodeVsPlayer("ElevatorBox", 1.0f))
	{
		IdleState();
	}
}


bool StageElevator::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	//�m�[�h�̈ʒu�Ɠ����蔻����s��
	Model::Node* node = model->FindNode(nodeName);

	if (node != nullptr)
	{
		//�m�[�h�̃��[���h���W
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


void StageElevator::DrawDebugPrimitive()
{
	//DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	////����Փ˗p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 1, 0, 1));

	////����Փ˗p�̃f�o�b�O�~����`��
	//debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));

}