#include "Player.h" //プレイヤーヘッダ
#include <cassert>  //assert呼び出し

void Player::Initializa(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	// NULLポインタチェック p25
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する↓ p25
	/// ローカル変数は関数が終わると消える
	// モデルをメンバ変数に記録
	model_ = model;
	// テクスチャハンドルをメンバ変数に記録
	textureHandle_ = textureHandle;
	//(ビュープロジェクション)引数の内容をメンバ変数に記録 p34k
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

void Player::Update() {
	// 行列を定数バッファに転送 p30
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// 3Dモデル p35
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
