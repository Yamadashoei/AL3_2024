#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include <cassert> //assert呼び出し

#include "MathUtilityForText.h"
#include "input.h"

// 02_p27からデバッグカメラの追加

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {

	// 3Dモデルデータの解放 01_p10
	delete model_;
	// 自キャラの解放 01_p21
	delete player_;
	// 3Dモデルデータの解放 02_p4
	delete modelBlock_;
	// デバッグカメラの解放 02_p27
	delete debugCamera_;

	// 自キャラの解放 02_03_p23
	delete skydome_;

	// 自キャラの解放 02_03_p24
	delete modelSkydome_;

	// マップチップフィールドの解放 02_04 p21
	delete mapChipField_;

	// 敵キャラ
	// delete enemy_;
	// 敵キャラモデル

	for (Enemy* newEnemy : enemies_) {
		delete newEnemy;
	}
	enemies_.clear();

	delete modelEnemy_;

	// 02_p7 & 02_p16
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャハンドルを読み込む
	textureHandle_ = TextureManager::Load("ressa-panda.jpg"); // Resources/player.png
	// 3Dモデルデータの生成
	model_ = Model::CreateFromOBJ("player", true);
	// ビュープロジェクトションの初期化
	viewProjection_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// ブロックモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("block");

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 自キャラ(天球)呼び出し
	skydome_ = new Skydome();
	// 自キャラ(天球)の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// マップチップ呼び出し
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	// 自キャラ作成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, &viewProjection_, playerPosition); // playerPosition 追加

	// 自キャラの生成と初期化
	player_->SetMapChipField(mapChipField_);

	// 座標をマップチップ番号で指定
	// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10, 18);

	// 敵キャラの生成
	// enemy_ = new Enemy();
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	for (int32_t i = 0; i < 2; ++i) {
		modelEnemy_ = Model::CreateFromOBJ("enemy", true);
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10, 18);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// 敵キャラの初期化
	// enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
	// enemy_->SetMapChipField(mapChipField_);

	// カメラコントロールの初期化 02_06 p13||02_06 p7
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();
	// スカイドームの更新
	skydome_->Update();
	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// デバッグカメラの更新
	debugCamera_->Update();
	// カメラコントローラーの更新
	cameraController_->Update();

	// 全ての当たり判定を行う
	CheckAllCollisions();

	// 02_p28
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		// viewProjection_.UpdateMatrix();
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// アフィン変換行列の作成
			// 存在しなかったら次へ
			if (!worldTransformBlock)
				continue;
			// 平行移動
			Matrix4x4 result{
			    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, worldTransformBlock->translation_.x, worldTransformBlock->translation_.y, worldTransformBlock->translation_.z,
			    1.0f};

			// 平行移動だけ代入
			worldTransformBlock->matWorld_ = result;

			Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->rotation_);
			// アフィン変換と転送
			worldTransformBlock->TransferMatrix();
		}
	}

	//
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw();
	//スカイドームの描画
	skydome_->Draw();
	// 敵キャラの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// ブロック描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// 存在しなかったら次へ
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾すべての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び起こす
			player_->OnCollision(enemy);
			// 敵弾の衝突時コールバックを呼び起こす
			enemy->OnCollision(player_);
		}
	}
}

void GameScene::GenerateBlocks() {
	// 要素数 02_p8 & 02_p16
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する 02_p8 & 02_p16
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		//
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// キューブ生成 02_p8 & 02_p16
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) { //%は割る→今回は2で割ると0
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
