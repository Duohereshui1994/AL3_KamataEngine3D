#pragma once
#include "ViewProjection.h"

class Player;

class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// カメラの位置を更新
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) {target_ = target;}
	void Reset();

private:
	ViewProjection* viewProjection_ = nullptr;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};
};
