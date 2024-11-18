#pragma once
#include "SceneGame.h"

class EraManager
{
public:
	//�V���O���g�����ǂ�
	static EraManager& Instance()
	{
		static EraManager instance;
		return instance;
	}

	//����ۑ�
	void SetEra(SceneGame::Era newEra) { era = newEra; }
	SceneGame::Era GetEra() const { return era; }

	//��̈ʒu�ۑ�
	void SetSeedPosition(DirectX::XMFLOAT3 newseedPosition) { seedPosition = newseedPosition; }
	DirectX::XMFLOAT3 GetSeedPosition() const{ return seedPosition; }

	//��̏�ԕۑ�
	void SetSeed(bool newseedFlag) { seedFlag = newseedFlag; }
	bool GetSeed() const { return seedFlag; }
private:

	SceneGame::Era era = SceneGame::Era::Past;
	DirectX::XMFLOAT3 seedPosition = { 0,0,0 };
	bool seedFlag = true;
};
