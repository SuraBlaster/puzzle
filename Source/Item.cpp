#include "Item.h"
#include "Graphics/Graphics.h"
#include "ItemManager.h"

ItemManager* manager{};

void Item::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //����Փ˗p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 1, 0, 1));

    //����Փ˗p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));

}

void Item::Destroy()
{
    manager->Instance().Remove(this);
}