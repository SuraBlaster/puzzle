#include "Item.h"
#include "ItemManager.h"

void ItemManager::Update(float elapsedTime)
{
    for (Item* item : items)
    {
        item->Update(elapsedTime);
    }

    for (Item* item : removes)
    {
        //vector���폜����ꍇ�̓C�e���[�^�[�ō폜����
        std::vector<Item*>::iterator it = std::find(items.begin(), items.end(), item);

        if (it != items.end())
        {
            items.erase(it);
        }

        delete item;
    }
    removes.clear();
}

void ItemManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Item* item : items)
    {
        item->Render(dc, shader);
    }
}

void ItemManager::Register(Item* item)
{
    items.emplace_back(item);
}

void ItemManager::Clear()
{
    for (Item* item : items)
    {
        delete item;
        item = nullptr;
    }
    items.clear();
}

void ItemManager::Remove(Item* item)
{
    removes.insert(item);
}

void ItemManager::DrawDebugPrimitive()
{
    size_t enemyCount = items.size();

    for (int i = 0; i < enemyCount; i++)
    {
        this->items[i]->DrawDebugPrimitive();
    }

}
