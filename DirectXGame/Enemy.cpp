#include "Enemy.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include "Player.h"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void Enemy::Update() {

	// フェーズの処理
	switch (phase_) {
	case Phase::Approach:
	default:
		// 座標を移動させる（1フレーム分の移動量を足しこむ）
		worldTransform_.translation_ = Add(worldTransform_.translation_,approachSpeed_);
		// 既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		// 座標を移動させる（1フレーム分の移動量を足しこむ）
		worldTransform_.translation_ = Add(worldTransform_.translation_, leaveSpeed_);
		break;
	}

	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_.z += velocity_.z;

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();


	// キャラクターの座標を画面表示する処理
	ImGui::Begin("enemy");
	ImGui::Text("enemy %f.%f.%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


