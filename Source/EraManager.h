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

	bool GetPlayerHasBattery() const { return playerHasBattery; }
	void SetPlayerHasBattery(bool state) {
		playerHasBattery = state;
		if (playerHasBattery) {
			batteryFlagPast = false; // プレイヤーが持っている場合、両シーンをfalseに
			batteryFlagFuture = false;
		}
	}

	bool GetBatteryPast() const { return batteryFlagPast; }
	void SetBatteryPast(bool state) {
		if (!playerHasBattery) { // プレイヤーが持っている場合は変更しない
			batteryFlagPast = state;
			batteryFlagFuture = !state; // 他のシーンを逆に設定
		}
	}

	bool GetBatteryFuture() const { return batteryFlagFuture; }
	void SetBatteryFuture(bool state) {
		if (!playerHasBattery) { // プレイヤーが持っている場合は変更しない
			batteryFlagFuture = state;
			batteryFlagPast = !state; // 他のシーンを逆に設定
		}
	}

private:

	SceneGame::Era era = SceneGame::Era::Past;
	bool batteryFlagPast = true;
	bool batteryFlagFuture = false;
	bool playerHasBattery = false;
};
