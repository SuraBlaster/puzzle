#include "StageElevator.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Graphics.h"

StageElevator::StageElevator()
{
	scale.x = scale.y = scale.z = 0.5f;
	position.y = -0.5;
	position.z = -1;
	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Elevator/elevator.mdl");

	UpdateOpenState();

}

StageElevator::~StageElevator()
{
	//�X�e�[�W���f����j��
	delete model;
}

void StageElevator::Update(float elapsedTime)
{
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
		DirectX::XMVECTOR hitPosint = DirectX::XMLoadFloat3(&localHit.position);
		hitPosint = DirectX::XMVector3TransformCoord(hitPosint, DirectX::XMLoadFloat4x4(&transform));

		// �ϊ����ꂽ��_���i�[
		DirectX::XMStoreFloat3(&hit.position, hitPosint);

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

void StageElevator::OpenState()
{
	state = State::Open;
	model->PlayAnimation(Anim_Idle, true);

	
}

// �J�X�V����
void StageElevator::UpdateOpenState()
{
	OpenState();
}
