#pragma once
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Audio/Audio.h"
class Button : public Character
{
public:
    Button();
    ~Button() { Finalize(); }

    static Button& Instance() {
        static Button instance;
        return instance;
    }

    enum Animations
    {
        None,
        None1,
        PushAnimationC,
        None2,
        PushAnimationB,
        None3,
        PushAnimationA,
        None4,
        None5
    };
    
    enum class State
    {
        BottonA,
        BottonB,
        BottonC,
        null
    };

    void Update(float elapsedTime) ;
    void Render(ID3D11DeviceContext* dc, Shader* shader);
    void Finalize();
    void DrawDebugPrimitive();

    bool GetIsButtonView() const { return isButtonView; }
    void SetIsButtonView(bool view) { isButtonView = view; }

    void TransitionPushA();
    void TransitionPushB();
    void TransitionPushC();
    void UpdatePushA(float elapsedTime);
    void UpdatePushB(float elapsedTime);
    void UpdatePushC(float elapsedTime);

    //プレイヤーとボタン間の視点の切り替え操作
    DirectX::XMFLOAT3 PlayerVsButtonTargetView(DirectX::XMFLOAT3& playerPos, DirectX::XMFLOAT3& buttonPos,float len,bool& isButtonView);
private:
 
    std::unique_ptr<Model> buttonModel = nullptr;
  
    State state = State::null;

    std::unique_ptr<AudioSource> ButtonSE;

    bool isButtonView = false;
};
