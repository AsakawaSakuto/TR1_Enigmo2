#include"Ball.h"

#include <numbers>
#include <algorithm>
#include <math.h>

#define _USE_MATH_DEFINES
#define NOMIMAX

using namespace std;

#pragma region M

namespace Matrix {
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result{};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;
	return result;
}
// 拡縮
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;
	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float angle) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[1][1] = std::cos(angle);
	result.m[1][2] = std::sin(angle);
	result.m[2][1] = -std::sin(angle);
	result.m[2][2] = std::cos(angle);
	result.m[3][3] = 1;
	return result;
}
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float angle) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(angle);
	result.m[0][2] = -std::sin(angle);
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(angle);
	result.m[2][2] = std::cos(angle);
	result.m[3][3] = 1;
	return result;
}
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float angle) {
	Matrix4x4 result{};
	result.m[0][0] = std::cos(angle);
	result.m[0][1] = std::sin(angle);
	result.m[1][0] = -std::sin(angle);
	result.m[1][1] = std::cos(angle);
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 worldMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return worldMatrix;
}
} // namespace Matrix

#pragma endregion

void Ball::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	// NULLポインタチェック
#ifdef DEBUG_
	assert(model);
#endif // DEBUG_
	// 引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	//
	camera_ = camera;

	worldTransform_.translation_.y = 20.f;
	velocity = {0.f, 0.f, 0.f};
}

void Ball::Update() { 

	velocity.y += gravity;

	worldTransform_.translation_.y += velocity.y;

	// 行列更新
	Matrix4x4 affinMatrix = Matrix::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.matWorld_ = affinMatrix;
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Ball::Draw() { 
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
}