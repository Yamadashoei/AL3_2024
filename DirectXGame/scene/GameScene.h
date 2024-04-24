#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"          //モデルヘッダ
#include "Sprite.h"         //スプライトヘッダ
#include "ViewProjection.h" //ビュープロジェクションヘッダ
#include "WorldTransform.h" //ワールドトランスフォームヘッダ

#include "DebugCamera.h"
#include "Player.h" //自キャラヘッダ

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 自キャラp20
	Player* player_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドルp9
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータp10
	Model* model_ = nullptr;
	// ビュープロジェクションp11
	ViewProjection viewProjection_;
};
