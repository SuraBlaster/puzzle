#include "StageItem.h"

//�R���X�g���N�^
StageItem::StageItem()
{
    model = new Model("Data/Model/Stage/Past1F.mdl");

    scale.x = scale.y = scale.z = 0.01f;
}

//�f�X�g���N�^
StageItem::~StageItem()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageItem::Update(float elapsedTime)
{
    //��邱�Ɩ���
    UpdateTransform();

    model->UpdateTransform(transform);
}

void StageItem::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageItem::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}