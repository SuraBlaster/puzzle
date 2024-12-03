#include "StageContainer.h"
#include "Player.h"
#include "EraManager.h"
StageContainer::StageContainer()
{
    model = new Model("Data/Model/Stage/Container.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    position.x = 10;

    position.y = 10;
}

StageContainer::~StageContainer()
{
    delete model;
}

void StageContainer::Update(float elapsedTime)
{
    //��邱�Ɩ���
    UpdateTransform();

    model->UpdateTransform(transform);

    CollisionNodeVsPlayer("Container", 0.5f);
}

void StageContainer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool StageContainer::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageContainer::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        Player& player = Player::Instance();
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player.GetPosition(),
            player.GetRadius(),
            player.GetHeight(),
            outPosition))
        {
            EraManager::Instance().SetContainer(true);
            Destroy();
        }
    }
}
