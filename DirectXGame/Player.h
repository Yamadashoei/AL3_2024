#pragma once
#include "MathUtilityForText.h"
#include "Model.h"          //モデルヘッダ
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include "WorldTransform.h" //ワールド変換データのヘッダ

// const Vector3 &position
class Player {
public:
	/// 初期化
	void Initializa(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// 更新
	void Update();

	/// 描画
	void Draw();

private:
	// ワールド変換データ p18
	WorldTransform worldTransform_;
	// p34
	ViewProjection* viewProjection_ = nullptr;
	// モデル p18
	Model* model_ = nullptr;

	// テクスチャハンドル p18
	uint32_t textureHandle_ = 0u;

	// 02_05 p9
	Vector3 velocity_ = {};

	// 02_05 p10
	static inline const float kAcceleration = 0.01f;
	// 02_05 p12
	static inline const float kAttenuation = 1.0f;
	// 02_05 p14
	static inline const float kLimitRunSpeed = 1.0f;

	// 02_05 p18
	enum class LRDirection { kRight, kLeft };
	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度 02_05 p22
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>02_05 p22
	static inline const float kTimeTurn = 0.3f;

	// 02_05 p27
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 1.0f;
	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 1.0f;
	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 1.0f;
};

/*// 初期化 02_05 p4
void Initializa(Model* model, ViewProjection* viewProjection,const Vector3& position);

*/