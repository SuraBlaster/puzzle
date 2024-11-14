#include "StageElevator.h"

StageElevator::StageElevator()
{
    scale.x = scale.y = scale.z = 0.005f;

    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Elevator/Elevator.mdl");
}

StageElevator::~StageElevator()
{
    //�X�e�[�W���f����j��
    delete model;

}

void StageElevator::Update(float elapsedTime)
{
    //�O��̏���ۑ�
    oldTransform = transform;

    //�X�^�[�g����S�[���܂ł̋������Z�o����
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

    float length;
    DirectX::XMStoreFloat(&length, Length);

    //�X�^�[�g����S�[���܂ł̊Ԃ�1�b�ԂŐi�ފ���(0.0�`1.0)���Z�o����
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //�S�[���ɓ��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //���`�⊮�ňʒu���Z�o����
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //�s��X�V
    UpdateTransform();

    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    //model->UpdateTransform(transformIdentity);
}

//�s��X�V����
void StageElevator::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

//���C�L���X�g
bool StageElevator::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

//�s��X�V����
void StageElevator::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}