#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <numbers>

class Player;
class Goal {
private:
	//=====================碰撞size=====================

	static inline const float kWidth = 1.99f;
	static inline const float kHeight = 1.99f;

	//===================Others===================

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();
	// ワールドトランスフォームを取得する
	WorldTransform& GetWorldTransform() { return worldTransform_; }

	//===================冲突判定===================

	/// <summary>
	/// 获得世界位置坐标
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 获得AABB
	/// </summary>
	AABB GetAABB();

	/// <summary>
	/// 冲突结果
	/// </summary>
	void OnCollision(const Player* player);

	//==============================================


};
