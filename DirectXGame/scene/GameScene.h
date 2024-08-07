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

	// 自キャラ
	Player* player_ = nullptr;
	// スカイドーム
	Skydome* skydome_ = nullptr;

	// スカイドーム3Dモデル
	Model* modelSkydome_ = nullptr;

	// 敵キャラモデル
	Model* modelEnemy_ = nullptr;
	// 敵キャラ
	// Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ブロックモデルデータ
	Model* modelBlock_ = nullptr;

	//
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// 02_p27
	DebugCamera* debugCamera_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// カメラコントロール
	CameraController* cameraController_ = nullptr;

	/// <summary>
	/// 行列を計算する・転送する
	/// </summary>
	void UpdateMatrix();
};
