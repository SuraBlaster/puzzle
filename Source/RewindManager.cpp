#include "RewindManager.h"

//更新処理
void RewindManager::Update(float elapsedTime, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 angle,DirectX::XMFLOAT3 velocity,Model& model)
{
    elapsedtime += elapsedTime * 2.4f;

    if (elapsedtime >= stateSaveInterval)
    {
        //引数から情報を取得
        currentState.position = position;
        currentState.angle = angle;
        currentState.velocity = velocity;

        // アニメーション情報を取得
        currentState.animationIndex = model.GetCurrentAnimationIndex();
        currentState.animationTime = model.GetCurrentAnimationSeconds();

        //保存
        pastStates.push_back(currentState);
        elapsedtime = 0.0f;
    }
}

//巻き戻し処理
void RewindManager::RewindTime(float elapsedTime,float rewindTime,Model& model)
{
    while (!pastStates.empty() && rewindTime > 0.0f)
    {
        //最後尾から復元
        currentState = pastStates.back();
        pastStates.pop_back();
        rewindTime -= stateSaveInterval;

        //アニメーション復元
        model.PlayAnimation(currentState.animationIndex, true);
        model.UpdateAnimation(currentState.animationTime);
        
        //念のためデータが空であればbreak
        if (pastStates.empty())
        {
            break;
        }
    }
}
