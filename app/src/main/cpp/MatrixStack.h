//
// Created by vyu06133 on 2025/06/01 日.
//

#pragma once

#include "framework.h"

class MatrixStack {
private:
	std::stack<mat4> matrixStack_;

public:
	// コンストラクタ
	MatrixStack() {
		// スタックのデフォルト基底階層に単位行列をロードしておく
		// GLMの単位行列は mat4(1.0f) で作成可能
		// または identity<mat4>() (glm/gtc/matrix_transform.hpp が必要)
		matrixStack_.push(mat4(1.0f));
	}

	// スタックをリセットし、単位行列をプッシュ
	void Reset() {
		// スタックをクリア
		std::stack<mat4> emptyStack;
		matrixStack_.swap(emptyStack);
		matrixStack_.push(mat4(1.0f)); // 単位行列を再プッシュ
	}

	// 現在のトップ行列を単位行列に置き換える
	void LoadIdentity() {
		LoadMatrix(mat4(1.0f));
	}

	// 現在のトップ行列を新しい行列に置き換える
	void LoadMatrix(const mat4& mat) {
		if (matrixStack_.empty()) {
			// スタックが空の場合はPopできないため、エラーハンドリング
			// これはReset()やコンストラクタの設計上、通常は発生しないはずですが、念のため
			matrixStack_.push(mat); // 単純にプッシュ
		} else {
			matrixStack_.pop();
			matrixStack_.push(mat);
		}
	}

	// 現在のトップ行列と新しい行列の積を計算し、トップ行列を更新する
	void MultMatrix(const mat4& mat) {
		if (matrixStack_.empty()) {
			// スタックが空の場合は乗算できないため、エラーハンドリング
			throw std::runtime_error("MatrixStack is empty, cannot multiply matrix.");
		}
		mat4 top = matrixStack_.top(); // Peek() に相当
		matrixStack_.pop();
		matrixStack_.push(top * mat); // GLMの行列乗算 (C#と同じく top * mat の順序)
	}

	// 現在のトップ行列を複製してスタックにプッシュする
	void PushMatrix() {
		if (matrixStack_.empty()) {
			// スタックが空の場合はPeekできないため、エラーハンドリング
			// これはReset()やコンストラクタの設計上、通常は発生しないはずですが、念のため
			matrixStack_.push(mat4(1.0f)); // 単位行列をプッシュして対応するなどの選択肢
		} else {
			matrixStack_.push(matrixStack_.top()); // Peek() に相当
		}
	}

	// スタックからトップ行列をポップする
	void PopMatrix() {
		if (matrixStack_.size() <= 1) { // 少なくとも1つは単位行列が残るようにする
			throw std::runtime_error("MatrixStack is empty or contains only the base identity matrix. Cannot pop further.");
		}
		matrixStack_.pop();
	}

	// 現在のトップ行列を取得する
	mat4 GetTopMatrix() const {
		if (matrixStack_.empty()) {
			// スタックが空の場合はトップ行列がないため、エラーハンドリング
			throw std::runtime_error("MatrixStack is empty, no top matrix to get.");
		}
		return matrixStack_.top(); // Peek() に相当
	}
};
