#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"          //モデルヘッダ
#include "Sprite.h"         //スプライトヘッダ
#include "ViewProjection.h" //ビュープロジェクションヘッダ
#include "WorldTransform.h" //ワールドトランスフォームヘッダ
#include "TextureManager.h" //テクスチャマネージャーのヘッダ

#include "DebugCamera.h"
#include "Player.h"  //自キャラヘッダ
#include "Skydome.h" //スカイドームヘッダ
#include <vector>

#include "DebugCamera.h"

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

	// 自キャラ 01_p20
	Player* player_ = nullptr;

	// 自キャラ 01_p20
	Skydome* skydome_ = nullptr;

	// 3Dモデル //02_03 p24
	Model* modelSkydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル 01_p9
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ 01_p10
	Model* model_ = nullptr;
	// ビュープロジェクション 01_p11
	ViewProjection viewProjection_;

	// ブロックモデルデータ 02_p4
	Model* modelBlock_ = nullptr;

	// 可変個配列 02_p6 & 02_p15
	// std::vector<WorldTransform*> worldTransformBlocks_;

	// 02_p15
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// デバッグカメラ有効02_26
	bool isDebugCameraActive_ = false;

	// 02_p27
	DebugCamera* debugCamera_ = nullptr;

	/// <summary>
	/// 行列を計算する・転送する
	/// </summary>
	void UpdateMatrix();
};
