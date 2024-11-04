#include "StageFuture.h"

//�R���X�g���N�^
StageFuture::StageFuture()
{
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

//�f�X�g���N�^
StageFuture::~StageFuture()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageFuture::Update(float elapsedTime)
{
    //��邱�Ɩ���
}

void StageFuture::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageFuture::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}