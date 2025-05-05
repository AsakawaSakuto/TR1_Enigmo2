#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
#include "vector"
#include "Ball.h"
#include "Reflector.h"
#include <cmath>

// ゲームシーン
class GameScene {
public:
	GameScene();
	~GameScene();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 水滴
	Ball* ball_ = nullptr;

	// 水滴の3Dモデル
	Model* modelBall_ = nullptr;

	// 水滴のテクスチャ
	uint32_t textureBall_ = 0u;

	Reflector* reflector_ = nullptr;

	Model* modelRedlector_ = nullptr;

	uint32_t textureReflector = 0u;

	Vector3 Normalize(const Vector3& v);

	Vector3 Reflect(const Vector3& velocity, const Vector3& normal);

	void ReflectBall();

	// 入力
	Input* input_ = nullptr;

	// カメラ
	Camera camera_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラの切り替え用フラグ
	bool isDegubCameraActive_ = false;
};