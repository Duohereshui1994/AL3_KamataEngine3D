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
	// 着陆flag
	bool landing = false;
	// 落地判定
	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 2.0f) {//我的mapchip size好像是2
			landing = true;
		}
	}
	// 左右移动
	// 接地
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 旋转
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotation_ = std::numbers::pi_v<float>;
					turnTimer_ = 1.0f;
				}
				// 减速
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 旋转
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotation_ = 0.0f;
					turnTimer_ = 1.0f;
				}
				// 减速
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}
			velocity_ = Add(velocity_, acceleration);
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} // 非按键时速度衰减
		else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		// 跳跃
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ = Add(velocity_, {0.0f, kJumpAcceleration, 0.0f});
		}

	} 
	// 不在地面
	else {
		velocity_ = Add(velocity_, {0.0f, -kGravityAcceleration, 0.0f});
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);		//方法1
		//velocity_.y = std::clamp(velocity_.y, -kLimitFallSpeed, 0.0f);	//方法2
		if (landing) {
			worldTransform_.translation_.y = 2.0f;//我的mapchip size好像是2
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	// 旋回制御
	// 因为我自己的模型的原因 所以旋转角这样设置
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::clamp(turnTimer_ - 1 / 30.0f, 0.0f, turnTimer_);
		float destinationRotationYTable[] = {
		    0, std::numbers::pi_v<float>,
		    // std::numbers::pi_v<float> / 2.0f,
		    // std::numbers::pi_v<float> * 3.0f / 2.0f,
		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// worldTransform_.rotation_.y = destinationRotationY;
		worldTransform_.rotation_.y = std::lerp(destinationRotationY, turnFirstRotation_, turnTimer_);
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//====================================================================
	//因为目前没有atari判定。所以暂时加一个移动限制，来看追踪目标在画面内的补正 后续有判定后删除
	if (worldTransform_.translation_.x > 52) {
		worldTransform_.translation_.x = 52;
	}
	//====================================================================

	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }
