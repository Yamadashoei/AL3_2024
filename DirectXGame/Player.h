#pragma once
#include "Model.h"          //モデルヘッダ
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include "WorldTransform.h" //ワールド変換データのヘッダ

class Player {
public:
	/// 初期化
	void Initializa(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	/// 更新
	void Update();

	/// 描画
	void Draw();

private:
	// ワールド変換データ p18
	WorldTransform worldTransform_;
	// モデル p18
	Model* model_ = nullptr;
	// テクスチャハンドル p18
	uint32_t textureHandle_ = 0u;
	// p34
	ViewProjection* viewProjection_ = nullptr;
};

