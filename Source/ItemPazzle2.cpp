#include "ItemPazzle2.h"
#include <Input/Input.h>
#include "EraManager.h"
#include "Player.h"


ItemPazzle2::ItemPazzle2()
{
    scale.x = scale.y = scale.z = 0.000f;

    model = new Model("Data/Model/Stage/Pazzle2.mdl");


}

ItemPazzle2::~ItemPazzle2()
{
    delete model;
}

void ItemPazzle2::Update(float elapsedTime)
{
    if (EraManager::Instance().GetDropPazzle2() == true)
    {
        scale.x = scale.y = scale.z = 0.005f;
    }
    //当たり判定
    if (scale.x > 0.0f)
    {
        CollisionNodeVsPlayer("Pazzle2", 0.5f);

        if (IsGround())
        {
            position.y += sinf(angle.y * 0.5f) * 0.0005f;
            angle.y += turnSpeed;
        }
    }


    UpdateVelocity(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    model->UpdateTransform(transform);
}

void ItemPazzle2::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ItemPazzle2::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
            Destroy();
            EraManager::Instance().SetPlayerPazzle2(true);
            scale.x = scale.y = scale.z = 0.0f;
        }
    }
}