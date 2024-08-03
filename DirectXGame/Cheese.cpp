#include "Cheese.h"
#include <ImGuiManager.h>

void Cheese::Initialize() {
}

void Cheese::Update() {
	// 敵を倒したらチーズを1つ追加
	for (Enemy* enemy_ : enemys_) {
		if (enemy_->isDead()) {
			score++;
		}
	}
	// 障害物を倒したら5つ追加
	for (Obstacle* obstacle : obstacle_) {
		if (obstacle->isDead()) {
			score += bigScore;
		}
	}

#ifdef _DEBUG
	ImGui::Begin("Cheese");
	ImGui::Text("Score : %f", score);
	ImGui::End();
#endif
}

void Cheese::Draw() {}
