#include "ItemBattery.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "Collision.h"

ItemBattery::ItemBattery()
{
    scale.x = scale.y = scale.z = 0.003f;

    model = new Model("Data/Model/Item/Battery.mdl");

    radius = 0.5f;

    height = 1.0f;
}

ItemBattery::~ItemBattery()
{
    delete model;
}

void ItemBattery::Update(float elapsedTime)
{
    angle.y += turnSpeed;

    if (IsGround())
    {
        position.y += sinf(angle.y * 0.5f) * 0.0005f;   
    }
    else
    {
        UpdateVelocity(elapsedTime);
    }

    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateTransform(transform);
    
    CollisionNodeVsPlayer("BatteryBox", 0.5f);
}

void ItemBattery::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);  
}


void ItemBattery::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);

    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );

        //当たり判定表示
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
