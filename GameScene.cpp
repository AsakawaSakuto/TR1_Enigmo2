#include "GameScene.h"
using namespace KamataEngine;

#define _USE_MATH_DEFINES
#include <math.h>

GameScene::GameScene() {}

GameScene::~GameScene() { delete modelBall_; }

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

	if (input_->PushKey(DIK_R)) {
		ball_->worldTransform_.translation_.x = 0.f;
		ball_->worldTransform_.translation_.y = 20.f;
		ball_->velocity = {0.f, 0.f, 0.f}; // 横に速度を与える
	}

	ball_->velocity.y += ball_->gravity;
	ball_->worldTransform_.translation_.y += ball_->velocity.y;
	ball_->worldTransform_.translation_.x += ball_->velocity.x;

	// 衝突判定と反射処理
	if (ball_->worldTransform_.translation_.y <= reflector_->worldTransform_.translation_.y + 0.7f && ball_->velocity.y <= 0.0f) {
		ReflectBall(); // 反射処理を呼び出す
		// ボールが反射板に接触した場合
		ball_->worldTransform_.translation_.y = reflector_->worldTransform_.translation_.y + 0.5f;
	}

	if (input_->PushKey(DIK_Q)) {
		reflector_->worldTransform_.rotation_.z += 0.1f;
	} else if (input_->PushKey(DIK_E)) {
		reflector_->worldTransform_.rotation_.z -= 0.1f;
	} else if (input_->TriggerKey(DIK_T)) {
			reflector_->worldTransform_.rotation_.z = 0.f;
	}

	// デバッグカメラ
#ifdef _DEBUG // デバッグモードのみ有効
	if (input_->TriggerKey(DIK_SPACE)) {
		isDegubCameraActive_ = !isDegubCameraActive_;
	}

	if (isDegubCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;             // ビュー行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection; // プロジェクション行列
		camera_.TransferMatrix();
	} else {
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

// ベクトルの正規化
Vector3 GameScene::Normalize(const Vector3& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f)
		return {0, 0, 0};
	return {v.x / length, v.y / length, v.z / length};
}

// 反射処理
Vector3 GameScene::Reflect(const Vector3& velocity, const Vector3& normal) {
	// 法線を正規化
	Vector3 n = Normalize(normal);

	// 反射計算
	float dot = velocity.x * n.x + velocity.y * n.y + velocity.z * n.z;
	Vector3 reflectedVelocity = {velocity.x - 2.0f * dot * n.x, velocity.y - 2.0f * dot * n.y, velocity.z - 2.0f * dot * n.z};

	// 反射後の速度が逆転している場合
	// 反射法線の向きが正しく反転していない可能性がある場合は、反射法線を反転させる
	if (n.x < 0) {
		reflectedVelocity.x = -reflectedVelocity.x; // X方向の反射を逆転
	}

	return reflectedVelocity;
}

// 回転行列が反映されているかを確認する
void GameScene::ReflectBall() {
	// 回転行列の中身を確認
	char buffer[128];
	sprintf_s(buffer, "Rotation Matrix: %.3f %.3f %.3f\n", reflector_->worldTransform_.matWorld_.m[0][1], reflector_->worldTransform_.matWorld_.m[1][1], reflector_->worldTransform_.matWorld_.m[2][1]);
	OutputDebugStringA(buffer);

	// ワールド行列から反射板の法線ベクトルを取得
	Vector3 normal = {
	    reflector_->worldTransform_.matWorld_.m[0][1], // X成分
	    reflector_->worldTransform_.matWorld_.m[1][1], // Y成分
	    reflector_->worldTransform_.matWorld_.m[2][1], // Z成分
	};

	// 正規化
	normal = Normalize(normal);

	// デバッグ出力：法線ベクトルの確認
	sprintf_s(buffer, "Normal: x=%.3f, y=%.3f, z=%.3f\n", normal.x, normal.y, normal.z);
	OutputDebugStringA(buffer);

	// 反射計算
	ball_->velocity = Reflect(ball_->velocity, normal);

	// 反射後の位置調整
	float correctionDistance = 0.5f;
	if (ball_->worldTransform_.translation_.y <= reflector_->worldTransform_.translation_.y + correctionDistance) {
		ball_->worldTransform_.translation_.y = reflector_->worldTransform_.translation_.y + correctionDistance;
	}

	// ダンピング処理
	const float dampingFactor = 0.8f;
	ball_->velocity.x *= dampingFactor; // X方向
	ball_->velocity.y *= dampingFactor; // Y方向
	ball_->velocity.z *= dampingFactor; // Z方向

	// 最小速度を設定
	const float minVelocity = 0.1f;
	if (std::abs(ball_->velocity.x) < minVelocity) {
		ball_->velocity.x = 0.0f;
	}
	if (std::abs(ball_->velocity.y) < minVelocity) {
		ball_->velocity.y = 0.0f;
	}
	if (std::abs(ball_->velocity.z) < minVelocity) {
		ball_->velocity.z = 0.0f;
	}
}
