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

	//�L���[�u����������Ă��邩
	void SetCubeFlag(bool newCubeFlag) { cubeFlag = newCubeFlag; }
	bool GetCubeFlag()const { return cubeFlag; }

	//���v�����ׂđ����Ă��邩
	void SetClockFlag(bool newClockFlag) { clockFlag = newClockFlag; }
	bool GetClockFlag()const { return clockFlag; }


	void SetDropPazzle1Position(DirectX::XMFLOAT3 newDropPosition1) { dropPosition1 = newDropPosition1; }
	DirectX::XMFLOAT3 GetDropPazzle1Position()const { return dropPosition1; }

	void SetDropPazzle2Position(DirectX::XMFLOAT3 newDropPosition2) { dropPosition2 = newDropPosition2; }
	DirectX::XMFLOAT3 GetDropPazzle2Position()const { return dropPosition2; }

	void SetDropPazzle3Position(DirectX::XMFLOAT3 newDropPosition3) { dropPosition3 = newDropPosition3; }
	DirectX::XMFLOAT3 GetDropPazzle3Position()const { return dropPosition3; }

	void SetDropPazzle4Position(DirectX::XMFLOAT3 newDropPosition4) { dropPosition4 = newDropPosition4; }
	DirectX::XMFLOAT3 GetDropPazzle4Position()const { return dropPosition4; }

	// Pazzle1���o�����Ă��邩
	void SetDropPazzle1(bool newDropPazzle1) { dropPazzle1 = newDropPazzle1; }
	bool GetDropPazzle1()const { return dropPazzle1; }

	//Pazzle2���o�����Ă��邩
	void SetDropPazzle2(bool newDropPazzle2) { dropPazzle2 = newDropPazzle2; }
	bool GetDropPazzle2()const { return dropPazzle2; }

	//Pazzle3���o�����Ă��邩
	void SetDropPazzle3(bool newDropPazzle3) { dropPazzle3 = newDropPazzle3; }
	bool GetDropPazzle3()const { return dropPazzle3; }

	//Pazzle4���o�����Ă��邩
	void SetDropPazzle4(bool newDropPazzle4) { dropPazzle4 = newDropPazzle4; }
	bool GetDropPazzle4()const { return dropPazzle4; }

	//Pazzle1�������Ă��邩
	void SetPlayerPazzle1(bool newPlayerPazzle1) { playerPazzle1 = newPlayerPazzle1; }
	bool GetPlayerPazzle1()const { return playerPazzle1; }

	//Pazzle2�������Ă��邩
	void SetPlayerPazzle2(bool newPlayerPazzle2) { playerPazzle2 = newPlayerPazzle2; }
	bool GetPlayerPazzle2()const { return playerPazzle2; }

	//Pazzle3�������Ă��邩
	void SetPlayerPazzle3(bool newPlayerPazzle3) { playerPazzle3 = newPlayerPazzle3; }
	bool GetPlayerPazzle3()const { return playerPazzle3; }

	//Pazzle4�������Ă��邩
	void SetPlayerPazzle4(bool newPlayerPazzle4) { playerPazzle4 = newPlayerPazzle4; }
	bool GetPlayerPazzle4()const { return playerPazzle4; }

	//���ݑI�����Ă���p�Y����ۑ�
	void SetPazzle(Stage::Pazzle newPazzle) { pazzle = newPazzle; }
	Stage::Pazzle GetPazzle() const { return pazzle; }

	void SetPazzle1Position(DirectX::XMFLOAT3 newPosition1) { position1 = newPosition1; }
	DirectX::XMFLOAT3 GetPazzle1Position()const { return position1; }

	void SetPazzle2Position(DirectX::XMFLOAT3 newPosition2) { position2 = newPosition2; }
	DirectX::XMFLOAT3 GetPazzle2Position()const { return position2; }

	void SetPazzle3Position(DirectX::XMFLOAT3 newPosition3) { position3 = newPosition3; }
	DirectX::XMFLOAT3 GetPazzle3Position()const { return position3; }

	void SetPazzle4Position(DirectX::XMFLOAT3 newPosition4) { position4 = newPosition4; }
	DirectX::XMFLOAT3 GetPazzle4Position()const { return position4; }

	//Pazzle1�����ܐݒu����Ă��邩
	void SetPazzle1(bool newPazzle1) { pazzle1 = newPazzle1; }
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
	bool playerPazzle1 = false;
	bool playerPazzle2 = false;
	bool playerPazzle3 = false;
	bool playerPazzle4 = false;
	bool dropPazzle1 = false;
	bool dropPazzle2 = false;
	bool dropPazzle3 = false;
	bool dropPazzle4 = false;
	bool cubeFlag = false;
	bool clockFlag = false;
	DirectX::XMFLOAT3 playerSeedPosition = {};
	DirectX::XMFLOAT3 playerSeedScale = {};
	DirectX::XMFLOAT3 position1 = {};
	DirectX::XMFLOAT3 position2 = {};
	DirectX::XMFLOAT3 position3 = {};
	DirectX::XMFLOAT3 position4 = {};
	DirectX::XMFLOAT3 dropPosition1 = {};
	DirectX::XMFLOAT3 dropPosition2 = {};
	DirectX::XMFLOAT3 dropPosition3 = {};
	DirectX::XMFLOAT3 dropPosition4 = {};
	
};
