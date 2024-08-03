#pragma once
#include "Enemy.h"
#include "Obstacle.h"

/// <summary>
/// チーズクラス
/// </summary>
class Cheese {
public:
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

private:
	// 総数
	float score;
	// 障害物の破壊スコア
	float bigScore = 10;

	// 敵
	std::list<Enemy*> enemys_;
	// 障害物
	std::list<Obstacle*> obstacle_;
};
