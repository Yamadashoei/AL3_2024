#define NOMINMAX
#include "CameraController.h"
#include "MathUtilityForText.h"
#include "Player.h"

void CameraController::Initialize() {
	// ビュープロジェクションの初期化02_06 p8
	viewProjection_.Initialize();
}

void CameraController::Update() {
	// 追従対象のワールドトランスフォームを参照02_06 p12
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();
	// 追従対象とオフセット 02_06 p8
	cameraPos = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;
	//
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, cameraPos, kInterpolationRate);

	// 02_06 p19
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.top);

	// 02_06 p34
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);

	//
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォーム02_06 p11
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 02_06 p8
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}
