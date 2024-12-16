#include "ItemSeed.h"
#include <Input/Input.h>
#include "EraManager.h"
#include "Player.h"

ItemSeed::ItemSeed()
{
    scale.x = scale.y = scale.z = 0.000f;

    model = new Model("Data/Model/Stage/Seed.mdl");

    
}

ItemSeed::~ItemSeed()
{
    delete model;
}

void ItemSeed::Update(float elapsedTime)
{
    //当たり判定
    if (scale.x > 0.0f)
    {
        CollisionNodeVsPlayer("Seed", 1.0f);

        if (IsGround() && position.y < 1.0f)
        {
            position.y += sinf(angle.y * 0.5f) * 0.0005f;
            angle.y += turnSpeed;
        }
        
    }

    position = EraManager::Instance().GetPlayerSeedPosition();
    scale = EraManager::Instance().GetPlayerSeedScale();
    UpdateVelocity(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateTransform(transform);
}

void ItemSeed::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}



void ItemSeed::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
            EraManager::Instance().SetPlayerSeed(true);
            Destroy();
        }
    }
}