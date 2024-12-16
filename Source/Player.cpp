#include "Player.h"
#include <imgui.h>
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include <ProjectileHoming.h>
#include "RewindManager.h"
static Player* instance = nullptr;

Player& Player::Instance()
{
    return *instance;
}

//コンストラクタ
Player::Player()
{
    model = new Model("Data/Model/Player/Player.mdl");

    //インスタンスポインタ取得
    instance = this;

    //スケーリング
    scale.x = scale.y = scale.z = 0.7f;

    position.z = -3;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/thunder.efk");

    //待機ステートへ遷移
    TransitionIdleState();
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    
    //衝突判定用のデバッグ円柱を描画
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //projectileManager.DrawDebugPrimitive();

    //攻撃衝突用の左手ノードのデバッグ球を描画
    if (attackCollisionFlag)
    {
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
    
}


Player::~Player()
{
    delete hitEffect;

    if (model != nullptr)
    {
        delete model;
        model = nullptr;
    }
    
}

//更新処理
void Player::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //Cキーを押したとき
    if (gamePad.GetButton() & GamePad::BTN_X)
    {
        //巻き戻し処理
        RewindManager::Instance().RewindTime(elapsedTime,0.1f, Player::GetModel());
        position = RewindManager::Instance().currentState.position;
        angle = RewindManager::Instance().currentState.angle;
        velocity = RewindManager::Instance().currentState.velocity;
    }
    else if (gamePad.GetButtonUp())
    {
        if (InputMove(elapsedTime))
        {
            Player::GetModel().PlayAnimation(Anim_Walking, true);
        }
        else
        {
            Player::GetModel().PlayAnimation(Anim_Idle, true);
        }     
    }
    else
    {
        //0.1秒ごとにposition,angle,velocity,animationを保存
        RewindManager::Instance().Update(elapsedTime, position, angle, velocity, Player::GetModel());
    }


    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;
    }

    UpdateTransform();
    
    //走力速度更新
    UpdateVelocity(elapsedTime);

    UpdateInvincibleTimer(elapsedTime);

    projectileManager.Update(elapsedTime);

    model->UpdateAnimation(elapsedTime);

    model->UpdateTransform(transform);
}

bool Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    Move(moveVec.x, moveVec.z, moveSpeed);

    Turn(elapsedTime, moveVec.x ,moveVec.z, turnSpeed);

    return moveVec.x != 0 || moveVec.z != 0;
}

bool Player::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }
    return false;
}







void Player::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void Player::UpdateIdleState(float elapsedTime)
{

    if (InputMove(elapsedTime))
    {
        TransitionMoveState();
    }

    //移動入力処理
    InputMove(elapsedTime);

}

void Player::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Walking, true);
}

void Player::UpdateMoveState(float elapsedTime)
{
    if (!InputMove(elapsedTime))
    {
        TransitionIdleState();
    }

    //移動入力処理
    InputMove(elapsedTime);
 
}








//ジャンプ入力
bool Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount < jumpLimit)
        {
            jumpCount++;
            Jump(jumpSpeed);

            return true;
            
        }
        
    }
    return false;
}



void Player::Rewind(float elapsedTime,float rewindTime)
{
   
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{


    
}

DirectX::XMFLOAT3 Player::GetMoveVec()const
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
  
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    if (cameraRightLength > 0.0f)
    {
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    DirectX::XMFLOAT3 vec;
    vec.x = ax * cameraRightX + ay * cameraFrontX;
    vec.z = ay * cameraFrontZ + ax * cameraRightZ;
    vec.y = 0;


    return vec;
}

