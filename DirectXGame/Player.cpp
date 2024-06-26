#define NOMINMAX
#include "Player.h" //プレイヤーヘッダ
#include "Input.h"
#include "input.h"
#include <algorithm>
#include <cassert> //assert呼び出し
#include <numbers> //初期回転

// 02_05 p17から
//
void Player::Initializa(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタチェック p25
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する↓ p25
	/// ローカル変数は関数が終わると消える
	// モデルをメンバ変数に記録
	model_ = model;

	//(ビュープロジェクション)引数の内容をメンバ変数に記録 p34k
	viewProjection_ = viewProjection;

	// ワールド変換の初期化 02_05 p5
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 初期回転 p6
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	// 行列を定数バッファに転送 p30 //02_03 p36
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	// 接地状態 02_05 p28
	// 02_05 p32
	if (onGround_) {

		// 移動入力 02_05 p10  p12追加
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			velocity_ += acceleration;
			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 右入力
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				// o2_05 p18
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn; //
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				// o2_05 p18
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn; //
				}
			} else {
				// 非入力時は移動減衰をかける 02_05 p12
				velocity_.x *= (1.0f - kAttenuation);
			}
			// ジャンプ入力 02_05 p30
			if (Input::GetInstance()->PushKey(DIK_UP)) {
				// ジャンプ初速
				velocity_ += Vector3(0, kJumpAcceleration, 0);
			}
			// 空中 02_0 p28
		} else {
			// 落下速度
			velocity_ += Vector3(0, -kGravityAcceleration, 0);
			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}
	}
	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 着地フラグ 02_05 p31
	bool landing = false;
	// 地面との当たり判定
	// 下降中
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 02_05 p32
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に以降
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に遷移
			onGround_ = true;
		}
	}

		// 旋回制御　02_0 p20
		if (turnTimer_ > 0.0f) {
			// 旋回タイマーを1/60秒分カウントダウン
			turnTimer_ -= 1.0f / 60.0f;

			float destinationRotationYTable[]{std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
			// 状態に応じた角度を取得をする 02_05 p20
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
			// 自キャラの角度を設定する 02_05 p20
			worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
		}
	}

	void Player::Draw() {
		// 3Dモデル p35
		model_->Draw(worldTransform_, *viewProjection_);
	}
