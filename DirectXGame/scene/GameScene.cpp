#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene(){};

GameScene::~GameScene() {
	delete model_; 
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy_ : enemys_) {
		delete enemy_;
	}
	delete modelSkydome_;
	delete railCamera_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	LoadEnemyPopData();

	// ファイルを指定してテクスチャを読み込む
	textureHankdle_ = TextureManager::Load("player.PNG");
	// 3Dモデルデータの生成
	model_ = Model::Create();
	// ビュープロダクション
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player;
	// 自キャラの初期化
	Vector3 playerPosition(0, 0, 10.0f);
	player_->Intialize(model_,textureHankdle_,playerPosition);

	// 敵の生成
	//enemy_ = new Enemy;
	
	// 天球
	// 3Dモデルの生成
	skydome_ = new Skydome;
	modelSkydome_ = Model::CreateFromOBJ("AL3", true);
	skydome_->Initialize(modelSkydome_, textureHankdle_);

	// レールカメラ
	railCamera_ = new RailCamera;
	railCamera_->Initialize(railPosition_, railRotation_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロダクションを指定する（アドレスなし）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// レティクルのテクスチャ
	TextureManager::Load("point.png");
}

void GameScene::Update() {
	UpdateEnemyPopCommands();
	player_->GetWorldPosition();

	// 自キャラの更新
	player_->Update(viewProjection_);

	CheckAllColisions();

	// 敵の更新
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	for (Enemy* enemy_ : enemys_) {
		if (enemy_ != nullptr) {
			enemy_->Update();
		}
	}

	// 弾更新
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

#ifdef _DEBUG
	/*if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}*/
#endif
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		/*debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;*/

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();
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

	skydome_->Draw(viewProjection_);

	// 自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	for (Enemy* enemy_ : enemys_) {
		if (enemy_ != nullptr) {
			enemy_->Draw(viewProjection_);
			// 弾描画
			for (EnemyBullet* bullet : enemyBullets_) {
				bullet->Draw(viewProjection_);
			}
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	player_->DrawUI();

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
	const std::list<EnemyBullet*>& enemyBullets = GetBullets();

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
	// 自弾と敵キャラの当たり判定
	for (Enemy* enemy_ : enemys_) {
		for (PlayerBullet* bullet : playerBullets) {
			// 敵のワールド座標
			posA = enemy_->GetWorldPosition();
			// 敵弾の座標
			posB = bullet->GetWorldPosition();

			// 弾と弾の交差判定
			if ((((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z))) <=
			    ((playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_))) {
				enemy_->OnColision();
				bullet->OnColision();
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			// 待機完了
			isWait = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		
		
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);
		std::string word;
		
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyOccurrence(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait = true;
			waitTimer = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyOccurrence(Vector3 positipn) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, positipn, velocity_);
	enemys_.push_back(newEnemy);
	// 敵キャラにデーむシーンを渡す
		newEnemy->SetGameScene(this);
		// 敵キャラに自キャラのアドレスを渡す
		newEnemy->SetPlayer(player_);
}

//void GameScene::AddEnemy(Enemy* enemy) {
//	// リストに登録する
//	enemys_.push_back(enemy);
//}
