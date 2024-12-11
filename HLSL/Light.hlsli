// 平行光源情報
struct DirectionalLightData
{
	float4	direction;	// 向き
	float4	color;		// 色
};

//点光源情報
struct PointLightData
{
	float4	position;	// 座標
	float4	color;		// 色
	float	range;		// 範囲
	float3	dummy;

};

//点光源の最大数
static const int PointLightMax = 8;

//スポットライト情報
struct SpotLightData
{
	float4 position;//座標
	float4 direction;//向き
	float4 color;//範囲
	float range; //範囲
	float innerCorn;
	float outerCorn;
	float dummy;
};
//スポットライトの最大数
static const int SpotLightMax = 8;


//--------------------------------------------
//	ランバート拡散反射計算関数
//--------------------------------------------
// normal		: 法線(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// kd			: 反射率(反射の強さ)
// 返す値		: 拡散反射色
float3	CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// 法線とライト方向へのベクトルでの内積 
	// 内積した結果、ライトから見て裏向きの面だとマイナスの結果が出てしまうため
	// それをそのまま乗算すると色がおかしくなります。
	// 0以下であれば0になるように制限をつけましょう
	float b = max(dot(normal, -lightVector), 0.0);

	// 入射光色と内積の結果、及び反射率をすべて乗算して返却しましょう。
	return b * lightColor * kd;
}
//--------------------------------------------
//	ハーフランバート拡散反射計算関数
//--------------------------------------------
// normal		: 法線(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// kd			: 反射率(反射の強さ)
float3 ClacHalfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = dot(normal, lightVector) * 0.5 + 0.5;
	return lightColor * d * kd;
}


//--------------------------------------------
//	フォンの鏡面反射計算関数
//--------------------------------------------
// normal		: 法線(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// eyeVector		: 視線ベクトル(正規化済み)
// shininess		: 光沢度(質感調整値)
// ks			: 反射率(反射の強さ)
// 返す値		: 鏡面反射色
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
	// 入射ベクトルを法線方向に対して反射させた反射ベクトルを算出
	float3 R = reflect(-lightVector, normal);

	// 反射ベクトルと視線ベクトルとで内積
	// 内積した結果を0以下であれば0になるように制限
	float d = max(dot(R, eyeVector), 0.0);

	// 光沢処理
	// 入射光色と内積の結果を指数的に変化させる
	d = pow(d, shininess);

	// 入射光色と内積の結果、及び反射率をすべて乗算して返却
	return d * lightColor * ks;
}


//--------------------------------------------
// リムライト
//--------------------------------------------
// normal		: 法線(正規化済み)
// eyeVector	: 視線ベクトル(正規化済み)
// lightVector	: 入射ベクトル(正規化済み)
// lightColor	: 入射光色
// rimPower		: リムライトの強さ(初期値はテキトーなので自分で設定するが吉)
float3 CalcRimLight(float3 normal, float3 eyeVector, float3 lightVector, float3 lightColor, float rimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(normal, -eyeVector));

	return  lightColor * pow(rim, rimPower) * saturate(dot(lightVector, -eyeVector));
}

//--------------------------------------------
//	トゥーン拡散反射計算関数
//--------------------------------------------
// toonTexture		: トゥーン用U方向ランプテクスチャ
// toonSamplerState	: トゥーン用サンプラーステート
// normal			: 法線(正規化済み)
// lightVector		: 入射ベクトル(正規化済み)
// lightColor		: 入射光色
// kd				: 反射率(反射の強さ)
// 返す値			: 拡散反射色
float3	CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState, float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	//	法線と入射ベクトルからU座標を求める
	float u = saturate(dot(normal, -lightVector) * 0.5f + 0.5f);

	// トゥーンテクスチャから色を取得する
	float3 c = toonTexture.Sample(toonSamplerState, float2(u, 0.1f));

	return lightColor * c * kd;
}