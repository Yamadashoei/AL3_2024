#pragma once
#include "Model.h"          //モデルヘッダ
#include "WorldTransform.h" //ワールドトランスフォームヘッダ
#include "ViewProjection.h" //ビュープロジェクションヘッダ

class Skydome {

public:
	void Initialize(Model* model,ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション 01_p11
	ViewProjection* viewProjection_ ;
	// モデル
	Model* model_ = nullptr;
	
};
