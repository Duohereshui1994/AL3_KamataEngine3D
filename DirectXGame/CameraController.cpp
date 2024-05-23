#include "CameraController.h"
#include "Player.h"

void CameraController::Initalize() { viewProjection_->Initialize(); }

void CameraController::Update() {}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	
	viewProjection_->translation_ = Add(targetWorldTransform.translation_ , targetOffset_);
}
