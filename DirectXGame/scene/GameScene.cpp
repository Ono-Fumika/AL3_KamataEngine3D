#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_; 
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイルを指定してテクスチャを読み込む
	textureHankdle_ = TextureManager::Load("player.PNG");
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// ビュープロダクション
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player;
	// 自キャラの初期化
	player_->Intialize(model_,textureHankdle_);

	// 敵の生成
	enemy_ = new Enemy;
	// 敵の初期化
	enemy_->Initialize(model_, position_, velocity_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロダクションを指定する（アドレスなし）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->GetWorldPosition();

	// 自キャラの更新
	player_->Update();

	CheckAllColisions();

	// 敵の更新
	if (enemy_ != nullptr) {
		enemy_->Update();
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
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

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}

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

void GameScene::CheckAllColisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region
	// 自キャラのワールド座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾の当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 弾と弾の交差判定
		if ((((posB.x - posA.x) * (posB.x - posA.x)) + 
			((posB.y - posA.y) * (posB.y - posA.y)) + 
			((posB.z - posA.z) * (posB.z - posA.z))) <=
		    ((playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_))) {
			player_->OnColision();
			bullet->OnColision();
		}

	}
	#pragma endregion;

	#pragma region
	// 敵のワールド座標
	posA = enemy_->GetWorldPosition();

	// 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 弾と弾の交差判定
		if ((((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z))) <=
		    ((playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_))) {
			enemy_->OnColision();
			bullet->OnColision();
		}
	}
	#pragma endregion;

	#pragma region
	// 自弾と敵キャラの当たり判定
	for (EnemyBullet* bullet_ : enemyBullets) {
		for (PlayerBullet* bullet : playerBullets) {
			// 敵弾の座標
			posA = bullet_->GetWorldPosition();
			posB = bullet->GetWorldPosition();
			// 弾と弾の交差判定
			if ((((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z))) <=
			    ((playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_))) {
				bullet->OnColision();
				bullet_->OnColision();
			}
		}
	}
#pragma endregion;
}
