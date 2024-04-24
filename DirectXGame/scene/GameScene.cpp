#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h" //テクスチャマネージャーのヘッダ
#include <cassert>          //assert呼び出し

#include "Player.h" //プレイヤーヘッダ

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {

	// 3Dモデルデータの解放p10
	delete model_;
	// 自キャラの解放p21
	delete player_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャハンドルを読み込むp9
	textureHandle_ = TextureManager::Load("ressa-panda.jpg");
	// 3Dモデルデータの生成p10
	model_ = Model::Create();
	// ビュープロジェクトションの初期化 p11
	viewProjection_.Initialize();

	// 自キャラ作成 p21
	player_ = new Player();
	// 自キャラの初期化 p21
	player_->Initializa(model_, textureHandle_, &viewProjection_);
}

void GameScene::Update() {

	// 自キャラの更新 p21
	player_->Update();
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

	// 自キャラの描画 p21
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
