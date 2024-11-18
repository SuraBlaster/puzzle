#include "StageTree.h"

//�R���X�g���N�^
StageTree::StageTree()
{
    model = new Model("Data/Model/Cube/Cube.mdl");
}

//�f�X�g���N�^
StageTree::~StageTree()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageTree::Update(float elapsedTime)
{
    //��邱�Ɩ���
}

void StageTree::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageTree::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}