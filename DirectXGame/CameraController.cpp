#include "CameraController.h"
#include "Player.h"

void CameraController::Initalize(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	viewProjection_->translation_ = Add(targetWorldTransform.translation_, targetOffset_);
	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_ = Add(targetWorldTransform.translation_, targetOffset_);
}
