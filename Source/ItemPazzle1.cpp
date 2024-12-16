#include "ItemPazzle1.h"
#include <Input/Input.h>
#include "EraManager.h"
#include "Player.h"


ItemPazzle1::ItemPazzle1()
{
    scale.x = scale.y = scale.z = 0.000f;

    model = new Model("Data/Model/Stage/Pazzle1.mdl");


}

ItemPazzle1::~ItemPazzle1()
{
    delete model;
}

void ItemPazzle1::Update(float elapsedTime)
{
    if (EraManager::Instance().GetDropPazzle1())
    {
        scale.x = scale.y = scale.z = 0.005f;
    }

    //�����蔻��
    if (scale.x > 0.0f)
    {
        CollisionNodeVsPlayer("Pazzle1", 0.5f);

        if (IsGround())
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

void ItemPazzle1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void ItemPazzle1::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
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
            Destroy();
            EraManager::Instance().SetPlayerPazzle1(true);
            scale.x = scale.y = scale.z = 0.0f;
        }
    }
}