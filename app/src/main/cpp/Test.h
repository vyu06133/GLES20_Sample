//
// Created by vyu06133 on 2025/05/26 月.
//

#pragma once
#include "TaskSystem.h"
#include "TaskBase.h"
#include "Vertex.h"
#include "CameraBase.h"

struct Test
{
	static inline TaskSystem ts;


	class Camera : public CameraBase
	{
		virtual void OnTick(float) override {
			eye_=vec3(30.f,40.f,50.f);
		}
		virtual void OnDraw() override {
			auto s = ts->Shader();
			s.UpdateUniform(s.View, view_);
			s.UpdateUniform(s.Projection, proj_);
		}
	};

	class Stage : public TaskBase
	{
	public:
		std::vector<VertexPNC> m_pnc;
		virtual void OnTick(float deltaTime) override
		{
			worldMatrix = mat4(1.0f);
		}
		virtual void OnCreate() override
		{
//			Geometry::GenerateCheckerPlaneZX(&m_pnc,vec3(10.0f),vec3(2.0f),vec4(0.1f, 0.1f, 0.1f, 1.0f), vec4(0.9f, 0.9f, 0.9f, 1.0f));
			auto s= ts->Shader();
		}
		virtual void OnDraw() override
		{
			auto s= ts->Shader();
			s.UpdateUniform(s.Model,localMatrix);
			s.VertexAttribPointer<VertexPNC>(m_pnc.data());
			glDrawArrays(GL_TRIANGLES,0,50);
			//glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
			//glEnableVertexAttribArray(positionHandle);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			//glDisableVertexAttribArray(positionHandle);

		}
	};

#if 0
		static inline const char* vert =
			"attribute vec3 a_position;   // 頂点位置\n"
			"attribute vec3 a_normal;     // 頂点の法線\n"
			"attribute vec4 a_color;      // 頂点の色\n"
			"\n"
			"uniform mat4 u_modelMatrix;  // モデル行列 (ワールド空間への変換用)\n"
			"uniform mat4 u_viewMatrix;  // ビュー行列 (ビュー空間への変換用)\n"
			"uniform mat4 u_projectionMatrix;  // プロジェクション行列 (プロジェクション空間への変換用)\n"
			"uniform vec4 u_lightPosition; // ライトの位置または方向 (W=1でポイントライト、W=0でディレクショナルライト)\n"
			"\n"
			"varying vec4 v_color;        // フラグメントシェーダーに渡す色\n"
			"varying vec3 v_normal;       // フラグメントシェーダーに渡す法線 (ワールド空間)\n"
			"varying vec3 v_lightVector;  // フラグメントシェーダーに渡すライトベクトル (ワールド空間)\n"
			"\n"
			"void main() {\n"
			"    // 頂点位置をワールド空間に変換\n"
			"    vec4 worldPos = u_modelMatrix * vec4(a_position,1.0);\n"
			"    vec4 viewPos = u_viewMatrix * worldPos;\n"
			"\n"
			"    // 法線をワールド空間に変換（回転・スケールのみ影響）\n"
			"    v_normal = normalize(mat3(u_modelMatrix) * a_normal);\n"
			"\n"
			"    // ライトベクトルの計算\n"
			"    if (u_lightPosition.w == 1.0) {\n"
			"        // ポイントライト: 頂点からライトへの方向\n"
			"        v_lightVector = normalize(u_lightPosition.xyz - worldPos.xyz);\n"
			"    } else {\n"
			"        // ディレクショナルライト: ライトの方向\n"
			"        v_lightVector = normalize(u_lightPosition.xyz);\n"
			"    }\n"
			"\n"
			"    // 頂点色をフラグメントシェーダーにそのまま渡す\n"
			"    v_color = a_color;\n"
			"\n"
			"    // 最終的な頂点位置をクリップ空間に変換\n"
			"    gl_Position = u_projectionMatrix * viewPos;\n"
			"}\n";
	static inline const char* frag  =
			"precision highp float;\n" // この行を追加
			"varying vec4 v_color;        // 頂点シェーダーから受け取った色\n"
			"varying vec3 v_normal;       // 頂点シェーダーから受け取った法線\n"
			"varying vec3 v_lightVector;  // 頂点シェーダーから受け取ったライトベクトル\n"
			"\n"
			"uniform vec3 u_lightColor;    // ライトの色\n"
			"uniform vec3 u_ambientColor;  // 環境光の色（シンプル化のためuniformで直接受け取る）\n"
			"\n"
			"void main() {\n"
			"    // 法線とライトベクトルを正規化（補間により長さが変わるため）\n"
			"    vec3 N = normalize(v_normal);\n"
			"    vec3 L = normalize(v_lightVector);\n"
			"\n"
			"    // 環境光成分\n"
			"    vec3 ambient = u_ambientColor * v_color.rgb; // 頂点色にも影響させる\n"
			"\n"
			"    // 拡散反射成分 (Diffuse)\n"
			"    // NとLの内積を計算し、光が当たる角度によって強度を調整\n"
			"    float diff = max(0.0, dot(N, L));\n"
			"    vec3 diffuse = u_lightColor * diff * v_color.rgb; // 頂点色にも影響させる\n"
			"\n"
			"    // 環境光と拡散反射を合計して最終的な色を決定\n"
			"    gl_FragColor = vec4(ambient + diffuse, v_color.a); // アルファ値は頂点色からそのまま使用\n"
			"}\n";
#endif

	static inline const char* vertex_shader_src =
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec4 a_color;\n"
			"\n"
			"uniform mat4 u_modelMatrix;\n"
			"uniform mat4 u_viewMatrix;\n"
			"uniform mat4 u_projectionMatrix;\n"
			"\n"
			"varying vec4 v_color;\n"
			"varying vec3 v_normal;\n"
			"\n"
			"void main() {\n"
			"    vec4 worldPos = u_modelMatrix * vec4(a_position, 1.0);\n"
			"    v_normal = mat3(u_modelMatrix) * a_normal;\n"
			"    v_color = a_color;\n"
			"    gl_Position = u_projectionMatrix * u_viewMatrix * worldPos;\n"
			"}\n";

	static inline const char* fragment_shader_src =
			"precision mediump float;\n"
			"\n"
			"varying vec4 v_color;\n"
			"varying vec3 v_normal;\n"
			"\n"
			"void main() {\n"
			"    gl_FragColor = v_color;\n"
			"}\n";


static inline Shader shader;
static inline mat4 model_;
static inline mat4 view_;
static inline mat4 proj_;
static inline std::vector<VertexPNC> vpnc;
	static inline void Init()
	{
//		auto& s = ts.Shader();
		auto& s = shader;
//		s.Create(vert, frag);
		s.Create(vertex_shader_src,fragment_shader_src);
		s.ConnectUniformAndSemantics(s.Model, "u_modelMatrix");
		s.ConnectUniformAndSemantics(s.View, "u_viewMatrix");
		s.ConnectUniformAndSemantics(s.Projection, "u_projectionMatrix");
		model_=mat4(1.0f);
		view_=glm::lookAt(vec3(30.f,40.f,50.f)*2.0f,vec3(0.f),vec3(0.f,1.f,0.f));
		proj_=glm::perspective(glm::radians(45.f),2.0f,.1f,10000.f);
		s.UpdateUniform(s.Model,model_);
		s.UpdateUniform(s.View,view_);
		s.UpdateUniform(s.Projection,proj_);
		Geometry::GenerateCheckerPlaneZX(&vpnc,vec3(10.0f),vec3(10.0f),vec4(0.0f, 0.0f, 0.0f, 1.0f),vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	static inline void SurfaceChanged( int width, int height)
	{
		LOGD("wxh=%dx%d\n",width,height);
		shader.Width() = width;
		shader.Height() = height;
	//	ts.Width() = width;
	//	ts.Height() = height;
		glViewport(0, 0, width, height);
	}

	static inline void DrawFrame()
	{
		auto& s = shader;
		glUseProgram(s.Program());
// CPUメモリ上の配列から直接描画する（VBOをバインドしない）
	model_=mat4(1.0f);
		s.UpdateUniform(s.Model,model_);
		static float rot = 0.0f;
		auto ang = glm::radians(rot);
		rot += 0.5f;
		view_ = glm::lookAt(
				glm::vec3(sinf(ang) * 15.0f, 10.0f, cosf(ang) * 15.0f),
				glm::vec3(0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
		s.UpdateUniform(s.View,view_);
		proj_=glm::perspective(glm::radians(60.f),(float)s.Width()/(float)s.Height(),.1f,10000.f);
		s.UpdateUniform(s.Projection,proj_);
// 描画コール（例：三角形リストとして）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s.BindVertexBuffer(vpnc.data());
		glDrawArrays(GL_TRIANGLES, 0, vpnc.size());
//		s.VertexAttribPointer<VertexPNC>(vp.data());
//		glDrawArrays(GL_TRIANGLES,0,20);
	//	ts.Tick(1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	ts.Draw();
	}

	static inline void Setup()
	{
	//	auto c= ts.CreateTask<Camera>(nullptr,0,-1);
	//	ts.CreateTask<Stage>(c);
	}
};
