#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
class Player {
private:
	Vector3 _velocity = {};

	//加速度
	static inline const float kAcceleration = 0.01f;
	//速度衰减系数
	static inline const float kAttenuation = 0.05f;
	//最大速度
	static inline const float kLimitRunSpeed = 0.5f;

public:
	Player();
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
};