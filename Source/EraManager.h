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

	//現在選択しているパズルを保存
	void SetPazzle(Stage::Pazzle newPazzle) { pazzle = newPazzle; }
	Stage::Pazzle GetPazzle() const { return pazzle; }

	//Pazzle1がいま設置されているか
	void SetPazzle1(bool newPazzle1) { pazzle1 = newPazzle1;}
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
	DirectX::XMFLOAT3 playerSeedPosition = {};
	DirectX::XMFLOAT3 playerSeedScale = {};

	
};
