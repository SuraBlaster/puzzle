#pragma once

#include <d3d11.h>
#include <DirectXMath.h>


//平行光源情報
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;//向き
	DirectX::XMFLOAT4 color;//色
};

//点光源情報
struct PointLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
	float range;//範囲
	DirectX::XMFLOAT3 dummy;
};
static constexpr int PointLightMax = 8;

//スポットライト情報
struct SpotLightData
{
	DirectX::XMFLOAT4 position;  //座標
	DirectX::XMFLOAT4 direction;//向き
	DirectX::XMFLOAT4 color;    //色
	float range;     //範囲
	float innerCorn; //インナー角度範囲
	float outerCorn; //アウター角度範囲
	float dummy;
};

//スポットライトの最大数
static constexpr int SpotLightMax = 8;

// レンダーコンテキスト
struct RenderContext
{
	//	カメラ情報
	DirectX::XMFLOAT4       viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//ライト情報
	DirectX::XMFLOAT4    ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData       pointLightData[PointLightMax];//点光源情報
	SpotLightData        spotLightData[SpotLightMax];//スポットライト情報
	int                  pointLightCount = 0;//点光源数
	int                  spotLightCount = 0;        //スポットライト数


};