#include "GameOver.h"

void GameOver::Initialize() {}

void GameOver::Update() {
	// スペースキーでシーン終了
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void GameOver::Draw() {}
