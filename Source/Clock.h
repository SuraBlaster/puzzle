#pragma once
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Button.h"

//���j
class LongHand : public Character
{
public:
    LongHand();
    ~LongHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> longHandModel = nullptr;      // ���j�̃��f��
   
};

//�Z�j
class ShortHand : public Character
{
public:
    ShortHand();
    ~ShortHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> shortHandModel = nullptr;     // �Z�j�̃��f��
   
};

// ���v�Ɋ֘A����N���X
class Clock : public Character
{
public:
    Clock();
    ~Clock() {}

     void Update(float elapsedTime);
     void Render(ID3D11DeviceContext* dc, Shader* shader);
   
     const std::vector<Clock*>& GetChildren() const { return m_children; }
   
     void SetLongHand(LongHand* longHand) { m_longhand = longHand; }
     void SetShortHand(ShortHand* shortHand) { m_shortHand = shortHand; }
     LongHand* GetLongHand() { return m_longhand; }
     ShortHand* GetShortHand() { return m_shortHand; }

     //�Z�j�̉�]��ݒ�
     void SetShortHandAngle(float angle) { ShortHandRotate = angle; m_shortHand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetShortHandRotation() const{ return ShortHandRotate;}

     //���j�̉�]��ݒ�
     void SetLongHandAngle(float angle) { longHandRotate = angle; m_longhand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetLongHandRotation() const { return longHandRotate; }

     //�e�{�^�����������Ƃ��̎��v�̉�]����
     void UpdateRotationHand();

     //�S�Ă̐j��12�������Ɍ����Ă��邩�m�F
     bool IsAlignedTo12Clock() const;

private:
    std::unique_ptr<Model> clockModel = nullptr;         // ���v�{�̂̃��f��
    LongHand* m_longhand;
    ShortHand* m_shortHand;  
    Clock* m_parent;
    std::vector<Clock*>m_children;

    float ShortHandRotate = 0.0f;         //�Z�j�̉�]�p�x
    float shortHandRotationSpeed = 10.0f; // �Z�j�̉�]���x�i�x/�b�j
    float longHandRotate = 0.0f;         // ���j�̉�]�p�x
    float longHandRotationSpeed = 120.0f;  // ���j�̉�]���x�i�x/�b�j
};
