#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);
float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState,pin.texcoord) * pin.color;
	float3 N = normalize(pin.normal);
	float3 L = normalize(directionalLightData.direction.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//マテリアル定数
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	//環境光の計算
	float3 ambient = ka * ambientLightColor;

	//平行光源のライティング計算
	float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
	float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

	//点光源の処理
	float3 pointDiffuse = (float3)0;
	float3 pointSpecular = (float3)0;
	int i;
	for (i = 0; i < pointLightCount; ++i)
	{
		//レイとベクトルを算出
		float3 lightVector = pointLightData[i].position.xyz - pin.world_position.xyz;

		//ライトベクトルの長さを算出
		float lightLength = length(lightVector);

		//ライトの影響範囲外なら後の計算はしない
		if (lightLength > pointLightData[i].range)continue;

		//距離減衰を算出する
		float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);

		lightVector = lightVector / lightLength;

		pointDiffuse += CalcLambertDiffuse(N, -lightVector,
			pointLightData[i].color.rgb, kd.rgb) * attenuate;

		pointSpecular += CalcPhongSpecular(N, lightVector,
			pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
	}

	float4 color = float4(ambient, diffuseColor.a);
	color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse);
	color.rgb += directionalSpecular + pointSpecular;

	return color;
}