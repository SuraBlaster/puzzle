#pragma once
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Button.h"

//’·j
class LongHand : public Character
{
public:
    LongHand();
    ~LongHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> longHandModel = nullptr;      // ’·j‚Ìƒ‚ƒfƒ‹
   
};

//’Zj
class ShortHand : public Character
{
public:
    ShortHand();
    ~ShortHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> shortHandModel = nullptr;     // ’Zj‚Ìƒ‚ƒfƒ‹
   
};

// Œv‚ÉŠÖ˜A‚·‚éƒNƒ‰ƒX
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

     //’Zj‚Ì‰ñ“]‚ğİ’è
     void SetShortHandAngle(float angle) { ShortHandRotate = angle; m_shortHand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetShortHandRotation() const{ return ShortHandRotate;}

     //’·j‚Ì‰ñ“]‚ğİ’è
     void SetLongHandAngle(float angle) { longHandRotate = angle; m_longhand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetLongHandRotation() const { return longHandRotate; }

     //Šeƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚Æ‚«‚ÌŒv‚Ì‰ñ“]ˆ—
     void UpdateRotationHand();

     //‘S‚Ä‚Ìj‚ª12©•ûŒü‚ÉŒü‚¢‚Ä‚¢‚é‚©Šm”F
     bool IsAlignedTo12Clock() const;

private:
    std::unique_ptr<Model> clockModel = nullptr;         // Œv–{‘Ì‚Ìƒ‚ƒfƒ‹
    LongHand* m_longhand;
    ShortHand* m_shortHand;  
    Clock* m_parent;
    std::vector<Clock*>m_children;

    float ShortHandRotate = 0.0f;         //’Zj‚Ì‰ñ“]Šp“x
    float shortHandRotationSpeed = 10.0f; // ’Zj‚Ì‰ñ“]‘¬“xi“x/•bj
    float longHandRotate = 0.0f;         // ’·j‚Ì‰ñ“]Šp“x
    float longHandRotationSpeed = 120.0f;  // ’·j‚Ì‰ñ“]‘¬“xi“x/•bj
};
