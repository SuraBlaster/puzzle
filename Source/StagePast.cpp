#include "StagePast.h"

//�R���X�g���N�^
StagePast::StagePast()
{
    model = new Model("Data/Model/Stage/Past1F.mdl");

    
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
    //��邱�Ɩ���
}

void StagePast::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StagePast::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
