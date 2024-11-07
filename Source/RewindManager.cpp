#include "RewindManager.h"

void RewindManager::Update(float elapsedTime, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 angle,DirectX::XMFLOAT3 velocity,Model& model)
{
    elapsedtime += elapsedTime;

    if (elapsedtime >= stateSaveInterval)
    {
        currentState.position = position;
        currentState.angle = angle;
        currentState.velocity = velocity;

        // アニメーション情報を取得
        currentState.animationIndex = model.GetCurrentAnimationIndex();
        currentState.animationTime = model.GetCurrentAnimationSeconds();

        pastStates.push_back(currentState);
        elapsedtime = 0.0f;
    }

}

void RewindManager::RewindTime(float rewindTime,Model& model)
{
    while (!pastStates.empty() && rewindTime > 0.0f)
    {
      
        currentState = pastStates.back();
        pastStates.pop_back();
        rewindTime -= stateSaveInterval;

        model.PlayAnimation(currentState.animationIndex, true, 0.0f);
        model.UpdateAnimation(currentState.animationTime);

        if (pastStates.empty()) break;
    }
}
