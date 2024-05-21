#include "Enemy.h"
#include "TextureManager.h"
#include <cassert>
#include <ImGuiManager.h>
#include "Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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

	// 接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update() {

		bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	ApproachUpdate();

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

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("enemy");
	ImGui::Text("enemy %f.%f.%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 0.05f;
	//Vector3 bulletVelocity(0, 0, kBulletSpeed);
	// 自キャラのワールド座標を取得する
	player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	GetWorldPosition();
	// 差分ベクトルを求める
	Vector3 difference = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	// ベクトルの正規化
	Normalize(difference);
	// ベクトルの長さを、早さに合わせる
	difference = Multiply(kBulletSpeed, difference);

	//// 速度ベクトルを自機の向きに合わせて回転させる
	//bulletVelocity = TransformNormal(bulletVelocity, worldTransform_.matWorld_);
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, difference);
	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	fireTimer = 60;
}

void Enemy::ApproachUpdate() { 
	fireTimer--;
	if (fireTimer == 0) {
		Fire();
		fireTimer = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


