#include "PlayerBullet.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	// 座標を移動させる（1フレーム分の移動量を足しこむ）
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnColision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
