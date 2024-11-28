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

	void SetEra(SceneGame::Era newEra) { era = newEra; }
	SceneGame::Era GetEra() const { return era; }

	bool GetPlayerHasBattery() const { return playerHasBattery; }
	void SetPlayerHasBattery(bool state) {
		playerHasBattery = state;
		if (playerHasBattery) {
			batteryFlagPast = false; // �v���C���[�������Ă���ꍇ�A���V�[����false��
			batteryFlagFuture = false;
		}
	}

	bool GetBatteryPast() const { return batteryFlagPast; }
	void SetBatteryPast(bool state) {
		if (!playerHasBattery) { // �v���C���[�������Ă���ꍇ�͕ύX���Ȃ�
			batteryFlagPast = state;
			batteryFlagFuture = !state; // ���̃V�[�����t�ɐݒ�
		}
	}

	bool GetBatteryFuture() const { return batteryFlagFuture; }
	void SetBatteryFuture(bool state) {
		if (!playerHasBattery) { // �v���C���[�������Ă���ꍇ�͕ύX���Ȃ�
			batteryFlagFuture = state;
			batteryFlagPast = !state; // ���̃V�[�����t�ɐݒ�
		}
	}

private:

	SceneGame::Era era = SceneGame::Era::Past;
	bool batteryFlagPast = true;
	bool batteryFlagFuture = false;
	bool playerHasBattery = false;
};
