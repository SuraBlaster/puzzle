#include "StageElevator.h"

StageElevator::StageElevator()
{
    scale.x = scale.y = scale.z = 0.3f;

    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Elevator/Elevator.mdl");

    position.y = -1.0f;

    position.z = -1.0f;

    //angle.y = 135.0f;

}

StageElevator::~StageElevator()
{
    //�X�e�[�W���f����j��
    delete model;

}

void StageElevator::Update(float elapsedTime)
{


    model->PlayAnimation(0,true,0.5f);

    //�s��X�V
    UpdateTransform();

    model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);

    
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
    model->UpdateTransform(transform);

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