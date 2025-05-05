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

	// Reflectorの高さに来たら反射
	if (ball_->worldTransform_.translation_.y <= reflector_->worldTransform_.translation_.y) {
		ReflectBall();
	}
	
	if (input_->PushKey(DIK_Q)) {
		reflector_->worldTransform_.rotation_.z += 0.1f;
	} else if (input_->PushKey(DIK_E)) {
		reflector_->worldTransform_.rotation_.z -= 0.1f;
	}

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

	ball_->Update();
	reflector_->Update();

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

// ベクトルの正規化
Vector3 GameScene::Normalize(const Vector3& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f)
		return {0, 0, 0};
	return {v.x / length, v.y / length, v.z / length};
}

// ベクトルの反射処理
Vector3 GameScene::Reflect(const Vector3& velocity, const Vector3& normal) {
	Vector3 n = Normalize(normal);
	float dot = velocity.x * n.x + velocity.y * n.y + velocity.z * n.z;
	return {velocity.x - 2.0f * dot * n.x, velocity.y - 2.0f * dot * n.y, velocity.z - 2.0f * dot * n.z};
}

void GameScene::ReflectBall() {
	// ReflectorのY軸回転から法線ベクトルを計算
	float angleY = reflector_->worldTransform_.rotation_.z;
	Vector3 normal = {std::sin(angleY), 0.0f, std::cos(angleY)};

	// 速度を反射
	ball_->velocity = Reflect(ball_->velocity, normal);

	// めり込み防止のため少し上に戻す
	ball_->worldTransform_.translation_.y = reflector_->worldTransform_.translation_.y + 0.5f;
}
