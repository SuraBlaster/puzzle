#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Graphics/Model.h"
#include <mutex>

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	// 描画開始
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// 描画
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model) = 0;

	// 描画終了
	virtual void End(ID3D11DeviceContext* context) = 0;

	// 色を設定するメソッド
	//virtual void SetColor(const DirectX::XMFLOAT4& color) = 0;

private:
	ID3D11Buffer* constantBuffer;  // 定数バッファ

};
