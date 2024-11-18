#include "ItemBattery.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"

ItemBattery::ItemBattery()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    //�X�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;

    height = 1.0f;
}

ItemBattery::~ItemBattery()
{
    delete model;
}

void ItemBattery::Update(float elapsedTime)
{
}

void ItemBattery::Render(ID3D11DeviceContext* dc, Shader* shader)
{
}

void ItemBattery::DrawDebugPrimitive()
{
}

void ItemBattery::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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

        //�����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
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
            Destroy();
        }
    }
}
