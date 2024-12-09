#include "StageFuture.h"
#include "EraManager.h"

//�R���X�g���N�^
StageFuture::StageFuture()
{

    difficulty = EraManager::Instance().GetDifficulty();

    switch (difficulty)
    {
    case Difficulty::Tutorial:
        {
            model = new Model("Data/Model/Stage/Future1_2F.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    case Difficulty::Beginner2:
        {
            model = new Model("Data/Model/Stage/Future3FDebris.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    case Difficulty::Advanced:
        {
            model = new Model("Data/Model/Stage/Future3F.mdl");
            scale.x = scale.y = scale.z = 0.005f;
        }
        break;
    }
    
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
    //�s��X�V����
    UpdateTransform();

    model->UpdateTransform(transform);
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