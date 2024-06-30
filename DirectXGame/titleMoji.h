#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <numbers>
#include <algorithm>

class titleMoji {
private:
	float deltaTime;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void Draw();
};
