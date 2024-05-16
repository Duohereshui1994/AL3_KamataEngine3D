#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>
Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 模型旋转二分之Pi。 但是我自己做的模型朝向是向左，所以不需要旋转
	// worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
}

void Player::Update() {
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (_velocity.x < 0.0f) {
				_velocity.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (_velocity.x > 0.0f) {
				_velocity.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		_velocity = Add(_velocity, acceleration);
		_velocity.x = std::clamp(_velocity.x, -kLimitRunSpeed, kLimitRunSpeed);
	} // 非按键时速度衰减
	else {
		_velocity.x *= (1.0f - kAttenuation);
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, _velocity);
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }