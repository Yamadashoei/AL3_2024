#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include <cassert>          //assert呼び出し

#include "MapChipField.h" //マップチップヘッダ
#include "MathUtilityForText.h"
#include "Player.h"  //プレイヤーヘッダ
#include "Skydome.h" //スカイドームヘッダ
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
	// ファイル名を指定してテクスチャハンドルを読み込む 01_p9
	textureHandle_ = TextureManager::Load("ressa-panda.jpg"); //Resources/player.png
	// 3Dモデルデータの生成 01_p10
	model_ = Model::CreateFromOBJ("player", true);
	// ビュープロジェクトションの初期化 01_p11　//02_03 p32
	// viewProjection_.farZ = 1000; // 遠くなら描画しない
	viewProjection_.Initialize();

	// デバッグカメラの生成 02_p27
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// ブロックモデルデータの生成 02_p4
	modelBlock_ = Model::CreateFromOBJ("block");

	// 3Dモデルの生成//02_03 p24
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 自キャラ(天球)呼び出し02_03 p23
	skydome_ = new Skydome();
	// 自キャラ(天球)の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// マップチップ呼び出し02_04 p21
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks(); // 02_04 p23

	// 座標をマップチップ番号で指定 02_05 p7
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	// 自キャラ作成 01_p21
	player_ = new Player();
	// 自キャラの初期化 01_p21
	player_->Initializa(model_, &viewProjection_, playerPosition); // playerPosition 追加
	                                                               // 座標をマップチップで指定 02_05 p7 仮
}

void GameScene::Update() {

	// 自キャラの更新 01_p21
	player_->Update();
	// デバッグカメラの更新 02_p27
	debugCamera_->Update();

	// 自キャラの更新 02_p21
	skydome_->Update();

	// 02_p28
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG

	// カメラの処理 02_p29
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// ブロックの更新 02_p9
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// アフィン変換行列の作成 02_p9
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

			//// スケーリング行列
			// worldTransformBlock->scale_ = result;
			//// 回転行列
			// worldTransformBlock->rotation_ = result;
			// worldTransformBlock->translation_ = result;
			//  定数バッファに転送する
		}
	}
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

	// 自キャラの描画 01_p21
	player_->Draw();
	// 自キャラ(天球)の描画 02_p21
	skydome_->Draw();

	// ブロック描画 02_p11
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

void GameScene::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	// matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	// 定数バッファに転送する
	// TransferMatrix();
}
