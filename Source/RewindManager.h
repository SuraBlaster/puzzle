#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "ResourceManager.h"

class RewindManager
{
public:
	//�V���O���g���̂��
	static RewindManager& Instance()
	{
		static RewindManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 angle,
		DirectX::XMFLOAT3 velocity,
		Model& model);

	//�����߂�����
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
