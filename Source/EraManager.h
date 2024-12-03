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

	void SetContainer(bool newContainer) { container = newContainer; }
	bool GetContainer()const { return container; }

	void SetSeed(bool newSeed) { seed = newSeed; }
	bool GetSeed()const { return seed; }

	void SetPlayerSeed(bool newPlayerSeed) { playerSeed = newPlayerSeed; }
	bool GetPlayerSeed()const { return playerSeed; }

	void SetPlayerSeedPosition(DirectX::XMFLOAT3 newPlayerSeedPosition) { playerSeedPosition = newPlayerSeedPosition; }
	DirectX::XMFLOAT3 GetPlayerSeedPosition()const { return playerSeedPosition; }

	void SetPlayerSeedScale(DirectX::XMFLOAT3 newPlayerSeedScale) { playerSeedScale = newPlayerSeedScale; }
	DirectX::XMFLOAT3 GetPlayerSeedScale()const { return playerSeedScale; }

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
	bool container = false;
	bool seed = false;
	bool playerSeed = false;
	DirectX::XMFLOAT3 playerSeedPosition = {};
	DirectX::XMFLOAT3 playerSeedScale = {};
};
