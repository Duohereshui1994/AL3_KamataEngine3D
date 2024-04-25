#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
class Skydome {
public:
	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update();
	void Draw();

private:
	Model* _model = nullptr;
	WorldTransform _worldTransform;
	ViewProjection* _viewProjection = nullptr;

};
