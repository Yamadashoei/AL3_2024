#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include <cassert>          //assert呼び出し

#include "Player.h"  //プレイヤーヘッダ
#include "Skydome.h" //スカイドームヘッダ

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
	textureHandle_ = TextureManager::Load("ressa-panda.jpg");
	// 3Dモデルデータの生成 01_p10
	model_ = Model::Create();
	// ビュープロジェクトションの初期化 01_p11
	viewProjection_.Initialize();
	// 自キャラ作成 01_p21
	player_ = new Player();
	// 自キャラの初期化 01_p21
	player_->Initializa(model_, textureHandle_, &viewProjection_);
	// デバッグカメラの生成 02_p27
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// ブロックモデルデータの生成 02_p4
	modelBlock_ = Model::CreateFromOBJ("cube");

	// 3Dモデルの生成//02_03 p24
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 自キャラ(天球)呼び出し02_03 p23
	skydome_ = new Skydome();
	// 自キャラ(天球)の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// 要素数 02_p8 & 02_p16
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅 02_p8 & 02_p16
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する 02_p8 & 02_p16
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		//
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// キューブ生成 02_p8 & 02_p16
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if ((i + j) % 2 == 0) //%は割る→今回は2で割ると0
				continue;
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
	// ビュープロジェクション
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

			/*// スケーリング行列
			worldTransformBlock->scale_ = result;
			// 回転行列
			worldTransformBlock->rotation_ = result;
			worldTransformBlock->translation_ = result;*/

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
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

void GameScene::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	// matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	// 定数バッファに転送する
	// TransferMatrix();
}
