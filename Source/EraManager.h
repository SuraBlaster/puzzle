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

	void SetEra(SceneGame::Era newEra) { era = newEra; }
	SceneGame::Era GetEra() const { return era; }
private:

	SceneGame::Era era = SceneGame::Era::Past;
};
