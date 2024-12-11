#pragma once
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Button.h"

//長針
class LongHand : public Character
{
public:
    LongHand();
    ~LongHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> longHandModel = nullptr;      // 長針のモデル
   
};

//短針
class ShortHand : public Character
{
public:
    ShortHand();
    ~ShortHand() {}

    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc, Shader* shader);
   
private:
    std::unique_ptr<Model> shortHandModel = nullptr;     // 短針のモデル
   
};

// 時計に関連するクラス
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

     //短針の回転を設定
     void SetShortHandAngle(float angle) { ShortHandRotate = angle; m_shortHand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetShortHandRotation() const{ return ShortHandRotate;}

     //長針の回転を設定
     void SetLongHandAngle(float angle) { longHandRotate = angle; m_longhand->SetAngle(DirectX::XMFLOAT3(0.0f, 0.0f, angle)); }
     float GetLongHandRotation() const { return longHandRotate; }

     //各ボタンを押したときの時計の回転処理
     void UpdateRotationHand();

     //全ての針が12自方向に向いているか確認
     bool IsAlignedTo12Clock() const;

private:
    std::unique_ptr<Model> clockModel = nullptr;         // 時計本体のモデル
    LongHand* m_longhand;
    ShortHand* m_shortHand;  
    Clock* m_parent;
    std::vector<Clock*>m_children;

    float ShortHandRotate = 0.0f;         //短針の回転角度
    float shortHandRotationSpeed = 10.0f; // 短針の回転速度（度/秒）
    float longHandRotate = 0.0f;         // 長針の回転角度
    float longHandRotationSpeed = 120.0f;  // 長針の回転速度（度/秒）
};
