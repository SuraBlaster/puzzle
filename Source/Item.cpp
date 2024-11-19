#include "Item.h"
#include "Graphics/Graphics.h"
#include "ItemManager.h"

ItemManager* manager{};

void Item::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //判定衝突用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 1, 0, 1));

    //判定衝突用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));

}

void Item::Destroy()
{
    manager->Instance().Remove(this);
}