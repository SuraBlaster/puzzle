#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "ResourceManager.h"

class RewindManager
{
public:
	//シングルトンのやつ
	static RewindManager& Instance()
	{
		static RewindManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 angle,
		DirectX::XMFLOAT3 velocity,
		Model& model);

	//巻き戻し処理
	void RewindTime(float elapsedTime,float rewindTime,Model& model);

	struct Rewind
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 angle;
		DirectX::XMFLOAT3 velocity;
		int animationIndex;
		float animationTime;
	};

	std::vector<Rewind> pastStates;
	Rewind currentState;
	const float stateSaveInterval = 0.1f;
	float elapsedtime = 0.0f;
};
