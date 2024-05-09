#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_worldTransform.Initialize();
	_viewProjection = viewProjection;
	_worldTransform.scale_ = {100.0f, 100.0f, 100.0f};
}

void Skydome::Update() { _worldTransform.UpdateMatrix(); }

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
