#pragma once
#include "input.h"

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameOver {
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

	// 終了フラグのゲッター
	bool IsFinished() const { return finished_; }

private:
	// 終了フラグ
	bool finished_ = false;

	// キーボード入力
	Input* input_ = nullptr;
};
