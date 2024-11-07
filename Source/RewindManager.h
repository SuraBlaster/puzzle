#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "ResourceManager.h"

class RewindManager
{
public:
	static RewindManager& Instance()
	{
		static RewindManager instance;
		return instance;
	}

	void Update(float elapsedTime,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 angle,
		DirectX::XMFLOAT3 velocity,
		Model& model);

	void RewindTime(float rewindTime,Model& model);

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
