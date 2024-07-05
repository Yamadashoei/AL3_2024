#pragma once
#include "MapChipField.h"
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

	// 02_06 補足
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 02_06 p28
	const Vector3& GetVelocity() const { return velocity_; }


	//// 02_07 p4
	// void SetMapChipField(MapChipField* mapChipField) {}

	//// 02_07 p10
	// void InputMove();

	//// 02_07 p13
	// void Player::CheckMapCollision(CollisionMapInfo& info) {
	//	CheckMapCollisionUp(info);
	//	CheckMapCollisionDown(info);
	//	CheckMapCollisionRight(info);
	//	CheckMapCollisionLeft(info);
	// }

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
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.5f;


	//// マップチップによるフィールド 02_07 p4
	// MapChipField* mapChipField_ = nullptr;

	//// キャラクターの当たり判定サイズ 02_07 p4
	// static inline const float kWidth = 0.8f;
	// static inline const float kHeight = 0.8f;

	//// 02_07 p12
	// struct CollisionMapInfo {
	//	bool ceiling = false;
	//	bool landing = false;
	//	bool hitWall = false;
	//	Vector3 move;
	// };
	//// 角 02_07 p16
	// enum Corner {
	//	kRightBottom, //右下
	//	kLeftBottom, //左下
	//	kRightTop, //右上
	//	kLeftTop,//左下

	//	kNumCorner //要素数
	//};

	///// 02_07 p17
	// Vector3 CornerPosition(const Vector3& center, Corner corner);
};

/*// 初期化 02_05 p4
void Initializa(Model* model, ViewProjection* viewProjection,const Vector3& position);

*/