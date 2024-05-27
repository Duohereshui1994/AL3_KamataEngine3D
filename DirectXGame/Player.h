#pragma once
#define NOMINMAX
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
private:
	//===================加减速移动===================
	Vector3 velocity_ = {};

	//加速度
	static inline const float kAcceleration = 0.01f;
	//速度衰减系数
	static inline const float kAttenuation = 0.05f;
	//最大速度
	static inline const float kLimitRunSpeed = 0.5f;

	//===================圆滑转向===================
	//方向
	LRDirection lrDirection_ = LRDirection::kRight;
	//角度補間
	float turnFirstRotation_ = 0.0f;
	float turnTimer_ = 0.0f;
	//旋回时间（秒）
	static inline const float kTimeTurn = 0.3f;

	//===================跳跃===================
	//是否在地上
	bool onGround_ = true;

	//重力加速度（下）
	static inline const float kGravityAcceleration = 0.01f;
	//最大落下速度
	static inline const float kLimitFallSpeed = 0.5f;
	//跳跃初速度（上）
	static inline const float kJumpAcceleration = 0.15f;//方法1 std::max 的对应参数
	//static inline const float kJumpAcceleration = 2.0f;//方法2 std::clamp 的对应参数

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

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

	/// <summary>
	/// 获得玩家的速度
	/// </summary>
	/// <returns></returns>
	const Vector3& GetVelocity() { return velocity_; }

	/// <summary>
	/// 获得玩家的世界变换
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform();
};