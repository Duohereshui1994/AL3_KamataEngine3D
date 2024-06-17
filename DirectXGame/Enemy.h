#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <numbers>

class Enemy {

private:
	//===================Move===================

	static inline const float kWalkSpeed = 0.05f;
	Vector3 velocity_ = {-kWalkSpeed, 0.0f, 0.0f};

	//===================Animation===============

	static inline const float kWalkMotionAngleStart = 0.0f;          // 最初的角度
	static inline const float kWalkMotionAngleEnd = 2 * std::numbers::pi_v<float>; // 最后的角度
	static inline const float kWalkMotionTime = 2.0f;                // 动画周期时间（秒）

	float walkTimer_ = 0.0f; // 动画计时器经过时间

	//===================Others===================

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; }
};