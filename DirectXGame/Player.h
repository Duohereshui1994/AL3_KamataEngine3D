#pragma once
#define NOMINMAX
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "DebugText.h"


enum class LRDirection {
	kRight,
	kLeft,
};

class MapChipField;

class Player {
private:
	//===================加减速移动===================
	Vector3 velocity_ = {};

	// 加速度
	static inline const float kAcceleration = 0.01f;
	// 速度衰减系数
	static inline const float kAttenuation = 0.05f;
	// 最大速度
	static inline const float kLimitRunSpeed = 0.5f;

	//===================圆滑转向===================
	// 方向
	LRDirection lrDirection_ = LRDirection::kRight;
	// 角度補間
	float turnFirstRotation_ = 0.0f;
	float turnTimer_ = 0.0f;
	// 旋回时间（秒）
	static inline const float kTimeTurn = 0.3f;

	//===================跳跃===================
	// 是否在地上
	bool onGround_ = true;

	// 重力加速度（下）
	static inline const float kGravityAcceleration = 0.01f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.5f;
	// 跳跃初速度（上）
	static inline const float kJumpAcceleration = 0.5f; // 方法1 std::max 的对应参数
	// static inline const float kJumpAcceleration = 2.0f;//方法2 std::clamp 的对应参数

	//===================当たり判定===================

	MapChipField* mapChipField_ = nullptr;

	// character 当たり判定 size （可能要调整）
	static inline const float kWidth = 1.99f;
	static inline const float kHeight = 1.99f;

	static inline const float kBlank = 0.01f;//微小余白

	//着地时速度衰减率
	static inline const float kAttenuationLanding = 0.1f;
	//撞墙减速率
	static inline const float kAttenuationWall = 0.05f;
	//===================Others===================

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:
	struct CollisionMapInfo {
		bool ceiling = false; // 天井
		bool landing = false; // 着地
		bool hitWall = false;    // 壁
		Vector3 move;         // 移動量
	};

	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorners // 要素数
	};


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

	void Move();


	/// <summary>
	/// 旋回制御
	/// </summary>
	void ConvolutionalControl();


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

	/// <summary>
	/// 外部setter
	/// </summary>
	/// <param name="mapChipField"></param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void isMapChipCollision(CollisionMapInfo& info);

	void isMapChipUPCollision(CollisionMapInfo& info);

	void isMapChipDownCollision(CollisionMapInfo& info);

	void isMapChipRightCollision(CollisionMapInfo& info);

	void isMapChipLeftCollision(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void collisionResultMove(CollisionMapInfo& info);

	void CeilingCollision(Player::CollisionMapInfo& info);
	void WallCollision(Player::CollisionMapInfo& info);

	void landingSwitch(CollisionMapInfo& info);
};