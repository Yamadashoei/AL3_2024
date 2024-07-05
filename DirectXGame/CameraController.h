#pragma once
#include "ViewProjection.h" //ビュープロジェクションヘッダ

// 前方宣言 02_06 p10
class Player;

class CameraController {
public:
	void Initialize();

	void Update();

	// 02_06 p10
	void SetTarget(Player* target) { target_ = target; }
	// 02_06 p11
	void Reset();

	// 矩形 02_06 p17
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	// ビュープロジェクション 02_06 p6
	ViewProjection viewProjection_;

	// 02_06 p10
	Player* target_ = nullptr;

	// 02_06 p11
	Vector3 targetOffset_ = {0, 0, -15.0f};

	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	Rect movableArea_ = {0, 100, 0, 100};

	// カメラの目標座標 02_06 p23
	Vector3 cameraPos;

	// 座標補間割合
	static inline const float kInterpolationRate = 0.5f;

	// 速度掛け算 02_06 p28
	static inline const float kVelocityBias = 5.0f;

	// 追従対象の各方向ほカメラ移動範囲 02_06 p33
	static inline const Rect margin = {-50, 50, -50, 50};

};