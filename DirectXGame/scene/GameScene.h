#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "DebugCamera.h"
#include "EnemyBullet.h"
#include "sKydome.h"
#include "RailCamera.h"
#include <sstream>
#include "Obstacle.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllColisions();

	// 敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	// 敵発生データの読み込み
	void LoadEnemyPopData();
	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();
	// 敵発生関数
	void EnemyOccurrence(Vector3 positipn);

	// 障害物発生データの読み込み
	void LoadObstaclePopData();
	// 障害物発生コマンドの更新
	void UpdateObstaclePopCommands();
	// 障害物発生関数
	void ObstacleOccurrence(Vector3 positipn);
	

	// 終了フラグのゲッター
	bool IsFinished() const { return finished_; }
	// クリアフラグのゲッター
	bool IsClear() const { return clear_; }

private:
	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// 終了フラグ
	bool finished_ = false;
	// クリアフラグ
	bool clear_ = false;
	
	// テクスチャハンドル
	uint32_t textureHankdle_ = 0;
	// 3Dモデルデータ
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	
	// 自キャラ
	Player* player_ = nullptr;
	float playerRadius_ = 1.0f;

	// 敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;
	Vector3 position_ = {5.0, 0, 30};
	Vector3 velocity_ = {0, 0, -0.1f};
	float enemyRadius_ = 1.0f;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	// 待機中フラグ
	bool isWait = false;
	// 待機タイマー
	int32_t waitTimer;

	// 障害物
	std::list<Obstacle*> obstacle_;
	Vector3 obstaclePosition_ = {5.0, 0, 30};
	float obstacleRadius_ = 1.0f;
	// 障害物発生コマンド
	std::stringstream obstaclePopCommands;
	// 待機中フラグ
	bool obstacleIsWait = false;
	// 待機タイマー
	int32_t obstacleWaitTimer;

	// 天球のテクスチャハンドル
	uint32_t skydomeTh_ = 0;
	// 3Dモデル
	Model* modelSkydome_ = nullptr;
	// 天球
	Skydome* skydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;
	Vector3 railPosition_ = {0, 0, -100.0f};
	Vector3 railRotation_ = {0, 0, 0};

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;
};
