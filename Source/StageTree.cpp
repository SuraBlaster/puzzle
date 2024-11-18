#include "StageTree.h"

//コンストラクタ
StageTree::StageTree()
{
    model = new Model("Data/Model/Cube/Cube.mdl");
}

//デストラクタ
StageTree::~StageTree()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageTree::Update(float elapsedTime)
{
    //やること名塩
}

void StageTree::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーにモデルを描画してもらう
    shader->Draw(dc, model);
}

bool StageTree::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}