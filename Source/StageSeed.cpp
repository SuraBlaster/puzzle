#include "StageSeed.h"
#include "Player.h"
#include "EraManager.h"

StageSeed::StageSeed()
{
    model = new Model("Data/Model/Stage/Seed.mdl");

    scale.x = scale.y = scale.z = 0.05f;

    position.x = 10;

    position.y = 10;
}

StageSeed::~StageSeed()
{
    delete model;
}

void StageSeed::Update(float elapsedTime)
{
    //��邱�Ɩ���
    UpdateTransform();

    model->UpdateTransform(transform);

}

void StageSeed::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageSeed::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

