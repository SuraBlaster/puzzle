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

	//ステージ保存
	void SetDifficulty(Stage::Difficulty newDifficulty) { difficulty = newDifficulty; }
	Stage::Difficulty GetDifficulty() const { return difficulty; }

	//キューブが完成されているか
	void SetCubeFlag(bool newCubeFlag) { cubeFlag = newCubeFlag; }
	bool GetCubeFlag()const { return cubeFlag; }

	//時計がすべて揃っているか
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

	// Pazzle1が出現しているか
	void SetDropPazzle1(bool newDropPazzle1) { dropPazzle1 = newDropPazzle1; }
	bool GetDropPazzle1()const { return dropPazzle1; }

	//Pazzle2が出現しているか
	void SetDropPazzle2(bool newDropPazzle2) { dropPazzle2 = newDropPazzle2; }
	bool GetDropPazzle2()const { return dropPazzle2; }

	//Pazzle3が出現しているか
	void SetDropPazzle3(bool newDropPazzle3) { dropPazzle3 = newDropPazzle3; }
	bool GetDropPazzle3()const { return dropPazzle3; }

	//Pazzle4が出現しているか
	void SetDropPazzle4(bool newDropPazzle4) { dropPazzle4 = newDropPazzle4; }
	bool GetDropPazzle4()const { return dropPazzle4; }

	//Pazzle1を持っているか
	void SetPlayerPazzle1(bool newPlayerPazzle1) { playerPazzle1 = newPlayerPazzle1; }
	bool GetPlayerPazzle1()const { return playerPazzle1; }

	//Pazzle2を持っているか
	void SetPlayerPazzle2(bool newPlayerPazzle2) { playerPazzle2 = newPlayerPazzle2; }
	bool GetPlayerPazzle2()const { return playerPazzle2; }

	//Pazzle3を持っているか
	void SetPlayerPazzle3(bool newPlayerPazzle3) { playerPazzle3 = newPlayerPazzle3; }
	bool GetPlayerPazzle3()const { return playerPazzle3; }

	//Pazzle4を持っているか
	void SetPlayerPazzle4(bool newPlayerPazzle4) { playerPazzle4 = newPlayerPazzle4; }
	bool GetPlayerPazzle4()const { return playerPazzle4; }

	//現在選択しているパズルを保存
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

	//Pazzle1がいま設置されているか
	void SetPazzle1(bool newPazzle1) { pazzle1 = newPazzle1; }
	bool GetPazzle1()const { return pazzle1; }

	//Pazzle2がいま設置されているか
	void SetPazzle2(bool newPazzle2) { pazzle2 = newPazzle2;}
	bool GetPazzle2()const { return pazzle2; }

	//Pazzle3がいま設置されているか
	void SetPazzle3(bool newPazzle3) { pazzle3 = newPazzle3;}
	bool GetPazzle3()const { return pazzle3; }

	//Pazzle4がいま設置されているか
	void SetPazzle4(bool newPazzle4) { pazzle4 = newPazzle4;}
	bool GetPazzle4()const { return pazzle4; }

	//コンテナの取得状態を保存
	void SetContainer(bool newContainer) { container = newContainer; }
	bool GetContainer()const { return container; }

	//種の取得状態を保存
	void SetSeed(bool newSeed) { seed = newSeed; }
	bool GetSeed()const { return seed; }

	//プレイヤーの種の取得状態を保存
	void SetPlayerSeed(bool newPlayerSeed) { playerSeed = newPlayerSeed; }
	bool GetPlayerSeed()const { return playerSeed; }

	//プレイヤーが種を植えた位置
	void SetPlayerSeedPosition(DirectX::XMFLOAT3 newPlayerSeedPosition) { playerSeedPosition = newPlayerSeedPosition; }
	DirectX::XMFLOAT3 GetPlayerSeedPosition()const { return playerSeedPosition; }

	void SetPlayerSeedScale(DirectX::XMFLOAT3 newPlayerSeedScale) { playerSeedScale = newPlayerSeedScale; }
	DirectX::XMFLOAT3 GetPlayerSeedScale()const { return playerSeedScale; }

	//プレイヤーがバッテリーを持っているか
	bool GetPlayerHasBattery() const { return playerHasBattery; }
	void SetPlayerHasBattery(bool state) {
		playerHasBattery = state;
		if (playerHasBattery) {
			batteryFlagPast = false; // プレイヤーが持っている場合、両シーンをfalseに
			batteryFlagFuture = false;
		}
	}

	//過去ステージでバッテリーがあるか
	bool GetBatteryPast() const { return batteryFlagPast; }
	void SetBatteryPast(bool state) {
		if (!playerHasBattery) { // プレイヤーが持っている場合は変更しない
			batteryFlagPast = state;
			batteryFlagFuture = !state; // 他のシーンを逆に設定
		}
	}

	//未来ステージでバッテリーがあるか
	bool GetBatteryFuture() const { return batteryFlagFuture; }
	void SetBatteryFuture(bool state) {
		if (!playerHasBattery) { // プレイヤーが持っている場合は変更しない
			batteryFlagFuture = state;
			batteryFlagPast = !state; // 他のシーンを逆に設定
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
