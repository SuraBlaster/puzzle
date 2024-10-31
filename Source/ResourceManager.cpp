#include "Graphics/Graphics.h"
#include "ResourceManager.h"

std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
    // ���f��������
    auto it = models.find(filename);
    if (it != models.end())
    {
        // �ア�Q�Ƃ��狤�L�Q�Ƃ��擾
        std::shared_ptr<ModelResource> modelResource = it->second.lock();
        if (modelResource)
        {
            return modelResource;
        }
    }

    // �V�K���f�����\�[�X�쐬���ǂݍ���
    std::shared_ptr<ModelResource> newModelResource = std::make_shared<ModelResource>();
    newModelResource->Load(Graphics::Instance().GetDevice(), filename);

    // �}�b�v�ɓo�^
    models[filename] = newModelResource;

    return newModelResource;
}
