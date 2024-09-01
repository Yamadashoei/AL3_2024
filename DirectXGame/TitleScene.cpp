#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

TitleScene::~TitleScene() { delete model_; }

void TitleScene::Initialize() {
	model_ = Model::CreateFromOBJ("titleFont");
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	finished_ = false;

	const float kPlayerScale =3.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	

	
}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	viewProjection_.TransferMatrix();

	worldTransformPlayer_.UpdateMatrix();
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	model_->Draw(worldTransformPlayer_, viewProjection_);
	Model::PostDraw();
}