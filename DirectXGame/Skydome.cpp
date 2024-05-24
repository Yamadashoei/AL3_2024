#include "Skydome.h"
#include <cassert>  //assert呼び出し


// 02_03 p23の呼び出し
void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック p25
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する↓ p25
	/// ローカル変数は関数が終わると消える
	// モデルをメンバ変数に記録
	model_ = model;
	
	//(ビュープロジェクション)引数の内容をメンバ変数に記録 p34k
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

}

void Skydome::Update() {
	// 自キャラ(天球)呼び出し02_03 p23
	// 行列を定数バッファに転送 
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() {

	// 3Dモデル描画 02_03 p26
	model_->Draw(worldTransform_, *viewProjection_);
}
