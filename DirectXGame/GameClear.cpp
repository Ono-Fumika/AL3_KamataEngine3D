#include "GameClear.h"

void GameClear::Initialize() {}

void GameClear::Update() {
	// スペースキーでシーン終了
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void GameClear::Draw() {}
