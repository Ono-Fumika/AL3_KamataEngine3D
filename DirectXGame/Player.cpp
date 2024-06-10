#include "Player.h"
#include <ImGuiManager.h>
#include "MassFunction.h"

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Player::~Player(){
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
};

void Player::Intialize(Model* model, uint32_t textureHandle,Vector3 playerPosition) { 
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// キャラクター旋回処理
	Rotate();

	// 押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 移動限界座標
	const float KMoveLimitX = 19.0f;
	const float KMoveLimtY = 35.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -KMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +KMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -KMoveLimtY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +KMoveLimitX);

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
	/*ImGui::Begin("player");
	ImGui::Text("player %f.%f.%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();*/

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾の生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::OnColision() {}

void Player::SetParent(const WorldTransform* parent){
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
};
