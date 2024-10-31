#include "StageMoveFloor.h"

StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
    //�X�e�[�W���f����j��
    delete model;
   
}

void StageMoveFloor::Update(float elapsedTime)
{
    //�O��̏���ۑ�
    oldTransform = transform;
    oldAngle = angle;

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

    //��]
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    //�s��X�V
    UpdateTransform();

    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0 ,0,1,0,0 ,0,0,1,0 ,0,0,0,1 };
    model->UpdateTransform(transformIdentity);
}

//�s��X�V����
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);

    shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    // �O��̃��[���h�s��Ƌt�s������߂�
    DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX invWorldMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);

    // �O��̃��[�J����Ԃł̃��C�ɕϊ�
    DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR endVec = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR localStartVec = DirectX::XMVector3Transform(startVec, invWorldMatrix);
    DirectX::XMVECTOR localEndVec = DirectX::XMVector3Transform(endVec, invWorldMatrix);
   
    // ���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, localStartVec);
    DirectX::XMStoreFloat3(&localEnd, localEndVec);

    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    {
        // �O��̃��[�J����Ԃ��獡��̃��[���h��Ԃ֕ϊ�
        DirectX::XMVECTOR hitPointVec = DirectX::XMLoadFloat3(&localHit.position);
        DirectX::XMVECTOR worldHitPointVec = DirectX::XMVector3Transform(hitPointVec, DirectX::XMLoadFloat4x4(&transform));
        DirectX::XMStoreFloat3(&hit.position, worldHitPointVec);


        hit.rotation.x = angle.x - oldAngle.x;
        hit.rotation.y = angle.y - oldAngle.y;
        hit.rotation.z = angle.z - oldAngle.z;

        return true;
    }
    return false;

}

//�s��X�V����
void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}
