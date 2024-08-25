#include "Goal.h"
void Goal::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	//===================常规初始化========================
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 模型旋转二分之三Pi。调整转向
	//worldTransform_.rotation_.y = std::numbers::pi_v<float> * 1.0f / 2.0f;
	viewProjection_ = viewProjection;

}

void Goal::Update() {
	// 更新模型的位置
	worldTransform_.UpdateMatrix();
}

// 描画
void Goal::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

Vector3 Goal::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Goal::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Goal::OnCollision(const Player* player) { (void)player; }
