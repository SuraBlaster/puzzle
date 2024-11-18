#include "Graphics/Model.h"
#include "StagePast.h"

//�R���X�g���N�^
StagePast::StagePast()
{
    scale.x = scale.y = scale.z = 0.005f;

    model = new Model("Data/Model/ExampleStage/kakostage.mdl");
}

//�f�X�g���N�^
StagePast::~StagePast()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StagePast::Update(float elapsedTime)
{
    // �s��X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

void StagePast::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    model->UpdateTransform(transform);

    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StagePast::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StagePast::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, W);
}