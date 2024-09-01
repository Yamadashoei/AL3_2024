#include <AABB.h>
#include <Input.h>
#include <MathUtilityForText.h>
#include <Model.h>
#include <Vector3.h>
#include <WorldTransform.h>
#include <algorithm>
#include <numbers>

class MapChipField;
class Player;

class Enemy {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// 角
	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数
	};

	/// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// 毎フレーム処理
	void Update();

	/// 描画
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector3& GetVelocity() const { return velocity_; }
	Vector3 GetWorldPosition();

	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Player* player);

private:
	// 歩行の速さ
	static inline const float kWalkSpeed = 0.02f;
	// 最初の角度 [度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度 [度]
	static inline const float kWalkMotionAngleEnd = 0.4f;
	// アニメーションの周期となる時間 [秒]
	static inline const float kWalkMotionTime = 1.0f;
	// 経過時間
	float walkTimer_ = 0.0f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;

	struct CollisionMapInfo {
		bool hitWall = false; // 壁
		Vector3 move;
	};
	//
	bool moveLeft = true;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	// 速度
	Vector3 velocity_ = {};
	LRDirection lrDirection_ = LRDirection::kLeft;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	
	void ChecMapCollision(CollisionMapInfo& info);
	void ChecMapCollisionRight(CollisionMapInfo& info);
	void ChecMapCollisionLeft(CollisionMapInfo& info);
	void AnimateTurn();
	Vector3 CornerPosition(const Vector3& center, Corner corner);


};