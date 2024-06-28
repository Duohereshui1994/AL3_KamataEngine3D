#include "titleMoji.h"

void titleMoji::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	viewProjection_ = viewProjection;
}

void titleMoji::Update() { worldTransform_.UpdateMatrix(); }

void titleMoji::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
