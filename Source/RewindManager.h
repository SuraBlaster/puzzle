#pragma once
#include <vector>
#include <DirectXMath.h>

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
		DirectX::XMFLOAT3 angle);

	void RewindTime(float rewindTime);


	struct Rewind
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 angle;
	};

	std::vector<Rewind> pastStates;
	Rewind currentState;
	const float stateSaveInterval = 0.1f;
	float elapsedtime = 0.0f;

};
