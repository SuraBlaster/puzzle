#pragma once
#include "SceneGame.h"

class EraManager
{
public:
	//シングルトンもどき
	static EraManager& Instance()
	{
		static EraManager instance;
		return instance;
	}

	//時代保存
	void SetEra(SceneGame::Era newEra) { era = newEra; }
	SceneGame::Era GetEra() const { return era; }

	//種の位置保存
	void SetSeedPosition(DirectX::XMFLOAT3 newseedPosition) { seedPosition = newseedPosition; }
	DirectX::XMFLOAT3 GetSeedPosition() const{ return seedPosition; }

	//種の状態保存
	void SetSeed(bool newseedFlag) { seedFlag = newseedFlag; }
	bool GetSeed() const { return seedFlag; }
private:

	SceneGame::Era era = SceneGame::Era::Past;
	DirectX::XMFLOAT3 seedPosition = { 0,0,0 };
	bool seedFlag = true;
};
