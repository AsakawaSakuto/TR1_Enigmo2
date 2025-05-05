#include "GameScene.h"
using namespace KamataEngine;

#define _USE_MATH_DEFINES
#include <math.h>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete modelBall_;
}

void GameScene::Initialize() {

	// 入力のインスタンス取得
	input_ = Input::GetInstance();

	// カメラの初期化
	camera_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	ball_ = new Ball();
	modelBall_ = Model::CreateFromOBJ("Ball", true);
	textureBall_ = TextureManager::Load("Ball/Ball.jpg");
	ball_->Initialize(modelBall_, textureBall_, &camera_);

	reflector_ = new Reflector();
	modelRedlector_ = Model::CreateFromOBJ("Reflector", true);
	textureReflector = TextureManager::Load("Reflector/Reflector.jpg");
	reflector_->Initialize(modelRedlector_, textureReflector, &camera_);
}

void GameScene::Update() {

	ball_->Update();
	reflector_->Update();

	if (input_->PushKey(DIK_Q)) {
		reflector_->worldTransform_.rotation_.z += 0.1f;
	} else if (input_->PushKey(DIK_E)) {
		reflector_->worldTransform_.rotation_.z -= 0.1f;
	}

	// 反射処理（関数呼び出し）
	ReflectBallIfHit(ball_, reflector_);

	#ifdef _DEBUG // デバッグモードのみ有効

	// SPACEキーで切り替え
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDegubCameraActive_) {
			isDegubCameraActive_ = false;
		} else {
			isDegubCameraActive_ = true;
		}
	}

	if (isDegubCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		camera_.matView = debugCamera_->GetCamera().matView;             // ビュー行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection; // プロジェクション行列

		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

#endif // _DEBUG
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	ball_->Draw();

	reflector_->Draw();

	// 3Dモデル描画後処理
	Model::PostDraw();
}

// 反射処理関数
void GameScene::ReflectBallIfHit(Ball* ball, Reflector* reflector) {
	float reflectorY = reflector->worldTransform_.translation_.y;
	float ballRadius = 1.0f; // 必要ならBallにメンバ化してもOK

	// 反射条件：地面に接触 & 下向きのとき
	if (ball->worldTransform_.translation_.y - ballRadius <= reflectorY && ball->velocity.y < 0.0f) {

		// 反射処理
		Vector3 normal = {0.0f, 1.0f, 0.0f}; // Reflectorの法線

		float dot = ball->velocity.x * normal.x + ball->velocity.y * normal.y + ball->velocity.z * normal.z;

		// 反射公式 V' = V - 2 * (V・N) * N
		ball->velocity.x = ball->velocity.x - 2.0f * dot * normal.x;
		ball->velocity.y = ball->velocity.y - 2.0f * dot * normal.y;
		ball->velocity.z = ball->velocity.z - 2.0f * dot * normal.z;

		// 減衰
		ball->velocity.x *= 0.8f;
		ball->velocity.y *= 0.8f;
		ball->velocity.z *= 0.8f;

		// めり込み修正
		ball->worldTransform_.translation_.y = reflectorY + ballRadius;
	}
}
