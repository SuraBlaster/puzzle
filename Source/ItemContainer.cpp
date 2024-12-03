#include "ItemContainer.h"
#include <Input/Input.h>
#include "EraManager.h"
#include "Player.h"
#include "ItemSeed.h"


ItemContainer::ItemContainer()
{
    scale.x = scale.y = scale.z = 0.000f;

    model = new Model("Data/Model/Item/Container.mdl");

    
}

ItemContainer::~ItemContainer()
{
    delete model;
}

void ItemContainer::Update(float elapsedTime)
{

    //�����蔻��(�ĂяE����悤�ɂ���)
    if (scale.x > 0.0f)
    {
        CollisionNodeVsPlayer("Container", 0.5f);

        if (IsGround() && position.y < 1.0f)
        {
            EraManager::Instance().SetPlayerSeedPosition(position);
            EraManager::Instance().SetPlayerSeedScale({ 0.5f,0.5f,0.5f });
            EraManager::Instance().SetSeed(true);
            Destroy();
            EraManager::Instance().SetContainer(false);

        }
        else if (IsGround())
        {
            position.y += sinf(angle.y * 0.5f) * 0.0005f;
            angle.y += turnSpeed;
        }
    }
    

    UpdateVelocity(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    model->UpdateTransform(transform);
}

void ItemContainer::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ItemContainer::DrawDebugPrimitive()
{
    Item::DrawDebugPrimitive();
}

void ItemContainer::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
            scale.x = scale.y = scale.z = 0.0f;
        }
    }
}
