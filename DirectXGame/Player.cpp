#include "Player.h"


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

	// move
	Move();
	// 旋回制御
	// 因为我自己的模型的原因 所以旋转角这样设置
	ConvolutionalControl();

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;

	isMapChipCollision(collisionMapInfo);

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//====================================================================
	// 因为目前没有atari判定。所以暂时加一个移动限制，来看追踪目标在画面内的补正 后续有判定后删除
	if (worldTransform_.translation_.x > 52) {
		worldTransform_.translation_.x = 52;
	}
	//====================================================================

	worldTransform_.UpdateMatrix();
}

void Player::Move() {
	// 着陆flag
	bool landing = false;
	// 落地判定
	LandingJudgment(landing);
	// 左右移动
	// 接地
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

		// 移动
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
			onGround_ = false;
		}

	}
	// 不在地面
	else {
		velocity_ = Add(velocity_, {0.0f, -kGravityAcceleration, 0.0f});
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed); // 方法1
		// velocity_.y = std::clamp(velocity_.y, -kLimitFallSpeed, 0.0f);	//方法2
		if (landing) {
			worldTransform_.translation_.y = 2.0f; // 我的mapchip size好像是2
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

/// <summary>
/// 获得世界变换
/// </summary>
/// <returns></returns>
WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

// void Player::isMapChipDownCollision(CollisionMapInfo& info) {}
//
// void Player::isMapChipRightCollision(CollisionMapInfo& info) {}
//
// void Player::isMapChipLeftCollision(CollisionMapInfo& info) {}

/// <summary>
/// 落地判定
/// </summary>
/// <param name="landing"></param>
void Player::LandingJudgment(bool& landing) {
	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 2.0f) { // 我的mapchip size好像是2
			landing = true;
		}
	}
}

/// <summary>
/// 旋回制御
/// </summary>
void Player::ConvolutionalControl() {
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
}

/// <summary>
/// 获得角点位置
/// </summary>
/// <param name="center">中心</param>
/// <param name="corner">角点</param>
/// <returns></returns>
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorners] = {
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0.0f}, // 右下
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // 左下
	    {kWidth / 2.0f,  kHeight / 2.0f,  0.0f}, // 右上
	    {-kWidth / 2.0f, kHeight / 2.0f,  0.0f}  // 左上
	};

	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

/// <summary>
/// map冲突判定
/// </summary>
/// <param name="info"></param>
void Player::isMapChipCollision(CollisionMapInfo& info) {
	isMapChipUPCollision(info);
	isMapChipDownCollision(info);
	/*isMapChipRightCollision(info);
	isMapChipLeftCollision(info);*/
}

void Player::isMapChipUPCollision(CollisionMapInfo& info) {

	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	// 上升？ 早期return
	if (info.move.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(worldTransform_.translation_, info.move));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, info.move.y);
		info.ceiling = true;
	}

	collisionResult(info);

	CeilingCollision(info);
}

void Player::isMapChipDownCollision(CollisionMapInfo& info) {
	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	// 下降？
	if (info.move.y >= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	//==================================这里不对劲 Y移动量 还有移动后自character的下端=======================
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move);
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, info.move.y);
		info.landing = true;
	}

	if (info.landing) {
		velocity_.y = 0.0f;
	}

}

// void Player::isMapChipRightCollision(CollisionMapInfo& info) {}

// void Player::isMapChipLeftCollision(CollisionMapInfo& info) {}

void Player::collisionResult(CollisionMapInfo& info) { worldTransform_.translation_ = Add(worldTransform_.translation_, info.move); }

void Player::CeilingCollision(Player::CollisionMapInfo& info) {
	if (info.ceiling) {
		velocity_.y = 0.0f;
	}
}

void Player::landingSwitch(CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			// 移动后四角坐标计算
			std::array<Vector3, kNumCorners> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
			}

			// 上升？
			if (info.move.y <= 0) {
				return;
			}

			// 下落判定和切换
			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (!hit) {
				onGround_ = false;
			}
		}
	} else {
		if (info.landing) {
			onGround_ = true;
			velocity_.x += (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}
