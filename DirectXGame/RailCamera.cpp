#include "RailCamera.h"
#include "MassFunction.h"
#include <ImGuiManager.h>


void RailCamera::Initialize(Vector3& translation_, Vector3& rotation_) {
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = translation_;
	worldTransform_.rotation_ = rotation_;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate_);
	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	if (worldTransform_.translation_.z >= -10.0f) {
		isGoal_ = true;
	}

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
#endif
 }
