#include "Obstacle.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include "GameScene.h"
#include "MassFunction.h"

void Obstacle::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void Obstacle::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	// デバッグ文字
#ifdef _DEBUG
	ImGui::Begin("Obtacle");
	ImGui::Text("HP : %f", hp);
	ImGui::Text("Radius : %f", radius_);
	ImGui::End();
#endif
}

void Obstacle::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Obstacle::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Obstacle::OnColision() { 
	// プレイヤーの弾に当たったら耐久値を下げる
	hp--;
	// 耐久値がまだあるならサイズを大きくする
	if (hp > 0) {
		worldTransform_.scale_ = Add(worldTransform_.scale_ ,Vector3{5, 5, 0});
		radius_ += 8.0f;
	} else {
		// 耐久値が0なら壊れる
		isDead_ = true;
	}
}
