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

	//�X�e�[�W�ۑ�
	void SetDifficulty(Stage::Difficulty newDifficulty) { difficulty = newDifficulty; }
	Stage::Difficulty GetDifficulty() const { return difficulty; }

	//���ݑI�����Ă���p�Y����ۑ�
	void SetPazzle(Stage::Pazzle newPazzle) { pazzle = newPazzle; }
	Stage::Pazzle GetPazzle() const { return pazzle; }

	//Pazzle1�����ܐݒu����Ă��邩
	void SetPazzle1(bool newPazzle1) { pazzle1 = newPazzle1;}
	bool GetPazzle1()const { return pazzle1; }

	//Pazzle2�����ܐݒu����Ă��邩
	void SetPazzle2(bool newPazzle2) { pazzle2 = newPazzle2;}
	bool GetPazzle2()const { return pazzle2; }

	//Pazzle3�����ܐݒu����Ă��邩
	void SetPazzle3(bool newPazzle3) { pazzle3 = newPazzle3;}
	bool GetPazzle3()const { return pazzle3; }

	//Pazzle4�����ܐݒu����Ă��邩
	void SetPazzle4(bool newPazzle4) { pazzle4 = newPazzle4;}
	bool GetPazzle4()const { return pazzle4; }

	//�R���e�i�̎擾��Ԃ�ۑ�
	void SetContainer(bool newContainer) { container = newContainer; }
	bool GetContainer()const { return container; }

	//��̎擾��Ԃ�ۑ�
	void SetSeed(bool newSeed) { seed = newSeed; }
	bool GetSeed()const { return seed; }

	//�v���C���[�̎�̎擾��Ԃ�ۑ�
	void SetPlayerSeed(bool newPlayerSeed) { playerSeed = newPlayerSeed; }
	bool GetPlayerSeed()const { return playerSeed; }

	//�v���C���[�����A�����ʒu
	void SetPlayerSeedPosition(DirectX::XMFLOAT3 newPlayerSeedPosition) { playerSeedPosition = newPlayerSeedPosition; }
	DirectX::XMFLOAT3 GetPlayerSeedPosition()const { return playerSeedPosition; }

	void SetPlayerSeedScale(DirectX::XMFLOAT3 newPlayerSeedScale) { playerSeedScale = newPlayerSeedScale; }
	DirectX::XMFLOAT3 GetPlayerSeedScale()const { return playerSeedScale; }

	//�v���C���[���o�b�e���[�������Ă��邩
	bool GetPlayerHasBattery() const { return playerHasBattery; }
	void SetPlayerHasBattery(bool state) {
		playerHasBattery = state;
		if (playerHasBattery) {
			batteryFlagPast = false; // �v���C���[�������Ă���ꍇ�A���V�[����false��
			batteryFlagFuture = false;
		}
	}

	//�ߋ��X�e�[�W�Ńo�b�e���[�����邩
	bool GetBatteryPast() const { return batteryFlagPast; }
	void SetBatteryPast(bool state) {
		if (!playerHasBattery) { // �v���C���[�������Ă���ꍇ�͕ύX���Ȃ�
			batteryFlagPast = state;
			batteryFlagFuture = !state; // ���̃V�[�����t�ɐݒ�
		}
	}

	//�����X�e�[�W�Ńo�b�e���[�����邩
	bool GetBatteryFuture() const { return batteryFlagFuture; }
	void SetBatteryFuture(bool state) {
		if (!playerHasBattery) { // �v���C���[�������Ă���ꍇ�͕ύX���Ȃ�
			batteryFlagFuture = state;
			batteryFlagPast = !state; // ���̃V�[�����t�ɐݒ�
		}
	}

private:

	SceneGame::Era era = SceneGame::Era::Past;
	Stage::Difficulty difficulty = Stage::Difficulty::Tutorial;
	Stage::Pazzle pazzle = Stage::Pazzle::Pazzle1;
	bool batteryFlagPast = true;
	bool batteryFlagFuture = false;
	bool playerHasBattery = false;
	bool container = false;
	bool seed = false;
	bool playerSeed = false;
	bool pazzle1 = false; 
	bool pazzle2 = false;
	bool pazzle3 = false;
	bool pazzle4 = false;
	DirectX::XMFLOAT3 playerSeedPosition = {};
	DirectX::XMFLOAT3 playerSeedScale = {};

	
};
