#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_worldTransform.Initialize();
	_viewProjection = viewProjection;
	_worldTransform.scale_ = {150.0f, 150.0f, 150.0f};
}

void Skydome::Update() { _worldTransform.UpdateMatrix(); }

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
