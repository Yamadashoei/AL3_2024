#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include <cassert>          //assert呼び出し

#include "Player.h" //プレイヤーヘッダ

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {

	// 3Dモデルデータの解放 01_p10
	delete model_;
	// 自キャラの解放 01_p21
	delete player_;
	// 02_p7
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
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

	// 要素数 02_p8
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅 02_p8
	const float kBlockWidth = 2.0f;
	// 要素数を変更する 02_p8
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブ生成 02_p8
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

void GameScene::Update() {

	// 自キャラの更新 01_p21
	player_->Update();

	// ブロックの更新 02_p9
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		// アフィン変換行列の作成

		//worldTransformBlock->matWorld_ = ;
		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
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

void GameScene::UpdateMatrix() {}
