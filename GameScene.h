#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
#include "vector"
#include "Ball.h"
#include "Reflector.h"

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

	// 入力
	Input* input_ = nullptr;

	// カメラ
	Camera camera_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラの切り替え用フラグ
	bool isDegubCameraActive_ = false;
};