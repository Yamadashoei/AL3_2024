
#include <Input.h>
#include <MathUtilityForText.h>
#include <Model.h>
#include <Vector3.h>
#include <WorldTransform.h>
#include <algorithm>
#include <numbers>

class MapChipField;

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// 3Dモデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// 歩行の速さ
	static inline const float kWalkSpeed = 0.02f;
	// 速度
	Vector3 velocity_ = {};
	// 最初の角度 [度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度 [度]
	static inline const float kWalkMotionAngleEnd = 0.4f;
	// アニメーションの周期となる時間 [秒]
	static inline const float kWalkMotionTime = 1.0f;
	// 経過時間
	float walkTimer_ = 0.0f;
};
