#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h" //敵キャラヘッダ
#include "Input.h"
#include "MapChipField.h"   //マップチップヘッダ
#include "Model.h"          //モデルヘッダ
#include "Player.h"         //自キャラヘッダ
#include "Skydome.h"        //スカイドームヘッダ
#include "Sprite.h"         //スプライトヘッダ
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include "ViewProjection.h" //ビュープロジェクションヘッダ
#include "WorldTransform.h" //ワールドトランスフォームヘッダ
#include <vector>
#include "DeathParticles.h"//パーティクルヘッダ

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

	void CheckAllCollisions();

	void GenerateBlocks();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticles = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// 自キャラ
	Player* player_ = nullptr;
	// 敵キャラ
	// Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;

	// 縦横ブロック配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;
};
