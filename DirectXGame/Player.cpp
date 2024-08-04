#include "Player.h"
#include <ImGuiManager.h>
#include "MassFunction.h"
#include <TextureManager.h>
#include <WinApp.h>


Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

Player::~Player(){
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
};

void Player::Intialize(Model* model, uint32_t textureHandle, Vector3 playerPosition, Model* model2) { 
	// NULLポインタチェック
	assert(model);
	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	bulletModel_ = model2;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("point.png");
	// スプライト作成
	sprite2DReticle_ = Sprite::Create(textureReticle, {620.0f,360.0f}, {1,1,1,1}, {0.5f,0.5f});

	// 発射タイマーを初期化
	fireTimer = 60;
}

void Player::Update(const ViewProjection& viewProjection) {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// hpが0なら死ぬ
	if (hp_ <= 0) {
		isDead_ = true;
	}

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

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 移動限界座標
	const float KMoveLimitX = 9.0f;
	const float KMoveLimtY = 5.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -KMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +KMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -KMoveLimtY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +KMoveLimtY);

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	
	worldTransform_.UpdateMatrix();

	// キャラクターの座標を画面表示する処理
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::Text("player %f.%f.%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
	ImGui::End();
#endif

	// キャラクター攻撃処理
	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		// 自機から3Dレティクルへの距離
		//const float kDistancePlayerTo3DReticle = 50.0f;
		//// 自機から3Dレティクルへのオフセット（Z+向き）
		//Vector3 offset = {0, 0, 1.0f};
		//// 自機のワールド行列の回転を反転
		////offset = TransformNormal(offset, worldTransform_.matWorld_);
		//// ベクトルの長さを整える
		//offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
		//// 3Dレティクルの座標を設定
		//worldTransform3DReticle_.translation_ = Add(offset, GetWorldPosition());
		//// worldTransform3DReticle_のワールド行列更新と転送
		//worldTransform3DReticle_.UpdateMatrix();
		//worldTransform3DReticle_.TransferMatrix();
	}

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		//Vector3 positionReticle = GetWorldPosition3DReticle();
		//// ビューポート行列
		//Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
		//Matrix4x4 matViewProjectionViewport = 
		//Multiply(Multiply(viewProjection.matView, viewProjection.matProjection),matViewport);

		//// ワールドスクリーン座標返還（ここで3Dから2Dになる）
		//positionReticle = Transform(positionReticle, matViewProjectionViewport);

		// スプライトのレティクルに座標返還
		//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{ 
		POINT mousePosition;
		// マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		// スプライトの現在位置を取得
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		// クライアントエリア座標に移動する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		// マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x, (float)mousePosition.y));

		// ジョイスティック状態取得
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
			// スプライトの座標変換を反映
			sprite2DReticle_->SetPosition(spritePosition);
		}

		// ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		// ビュープロジェクションビューポート合成行列
		Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
		// 合成行列の逆行列を計算する
		Matrix4x4 matInverseVPV = Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
		Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);

		// スクリーン座標からワールド座標へ
		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		// マウスレイの方向
		Vector3 mouseDirection = Subtract(posFar, posNear);
		mouseDirection = Normalize(mouseDirection);
		// カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 70.0f;
		worldTransform3DReticle_.translation_ = Add( posNear,Multiply(kDistanceTestObject, mouseDirection));

		// worldTransform3DReticle_のワールド行列更新と転送
		worldTransform3DReticle_.UpdateMatrix();
		worldTransform3DReticle_.TransferMatrix();

		// デバッグ文字
#ifdef _DEBUG
		ImGui::Begin("Player");
		ImGui::Text("(float)joyState : %f", (float)joyState.Gamepad.sThumbRX);
		ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
		ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
		ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
		ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
		ImGui::Text("HP : %f", hp_);
		ImGui::End();
#endif
	}
}

void Player::Attack() { 

	XINPUT_STATE joyState;
	
	// ゲームパッド未接続なら何もせず続ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (input_->TriggerKey(DIK_SPACE) || (joyState.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		fireTimer--;
		if (fireTimer <= 0) {

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 自機から標準オブジェクトへのベクトル
			velocity = Subtract(GetWorldPosition3DReticle(), GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalize(velocity));

			// 弾の生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);


			fireTimer = 30;
		}
		
		
	} else {
		fireTimer = 0;
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
	model_->Draw(worldTransform_, viewProjection); 

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	// 3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::OnColision() { hp_--; }

void Player::SetParent(const WorldTransform* parent){
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }
