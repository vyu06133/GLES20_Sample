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

#if 0
	class Camera : public CameraBase
	{
		virtual void OnTick(float) override
		{
			LOGD("Camera::OnTick\n");
			eye_ = vec3(30.f, 40.f, 50.f);
		}
		virtual void OnDraw() override
		{
			LOGD("Camera::OnDraw\n");
			CameraBase::OnDraw();
		}
		virtual void OnPostDraw() override
		{
			LOGD("Camera::OnPostDraw\n");
			CameraBase::OnPostDraw();
		}
	};

	class Stage : public TaskBase
	{
	public:
		std::vector<VertexPNC> m_pnc;
		virtual void OnTick(float deltaTime) override
		{
			LOGD("Stage::OnTick\n");
			worldMatrix = mat4(1.0f);
		}
		virtual void OnPostTick() override
		{
			LOGD("Stage::OnPostTick\n");
		}
		virtual void OnCreate() override
		{
			LOGD("Stage::OnCreate\n");
			Geometry::GenerateCheckerPlaneZX(&m_pnc,vec3(10.0f),vec3(2.0f),vec4(0.1f, 0.1f, 0.1f, 1.0f), vec4(0.9f, 0.9f, 0.9f, 1.0f));
			auto s= ts->Shader();
		}
		virtual void OnPostDraw() override
		{
			LOGD("Stage::OnPostDraw\n");
		}
		virtual void OnDraw() override
		{
			LOGD("Stage::OnDraw\n");
			auto s= ts->Shader();
			s.UpdateUniform(s.Model,worldMatrix);
			s.BindVertexBuffer(m_pnc.data());
			glDrawArrays(GL_TRIANGLES,0,m_pnc.size());

		}
	};
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


//static inline Shader shader;
static inline mat4 model_;
static inline mat4 view_;
static inline mat4 proj_;
static inline std::vector<VertexPNC> vpnc;
	static inline void Init()
	{
		auto& s = ts.Shader();
//		auto& s = shader;
		s.Create(vertex_shader_src,fragment_shader_src);
		glUseProgram(s.Program());
		s.ConnectUniformAndSemantics(s.Model, "u_modelMatrix");
		s.ConnectUniformAndSemantics(s.View, "u_viewMatrix");
		s.ConnectUniformAndSemantics(s.Projection, "u_projectionMatrix");
		model_=mat4(1.0f);
		view_=glm::lookAt(vec3(30.f,40.f,50.f)*2.0f,vec3(0.f),vec3(0.f,1.f,0.f));
		proj_=glm::perspective(glm::radians(45.f),2.0f,.1f,10000.f);
		s.UpdateUniform(s.Model,model_);
		s.UpdateUniform(s.View,view_);
		s.UpdateUniform(s.Projection,proj_);
		Geometry::GenerateCheckerPlaneZX(&vpnc,vec3(10.0f),vec3(2.0f),vec4(0.1f, 0.1f, 0.1f, 1.0f), vec4(0.9f, 0.9f, 0.9f, 1.0f));
	}

	static inline void SurfaceChanged( int width, int height)
	{
		auto& s = ts.Shader();
		s.SetScreenSize(width, height);
		glViewport(0, 0, width, height);
	}

	static inline void DrawFrame()
	{
#if 1
		auto& s = ts.Shader();
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
		proj_=glm::perspective(glm::radians(60.f),s.GetAspect(),.1f,10000.f);
		s.UpdateUniform(s.Projection,proj_);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s.BindVertexBuffer(vpnc.data());
		glDrawArrays(GL_TRIANGLES, 0, vpnc.size());
#else
		auto& s = ts.Shader();
		glUseProgram(s.Program());
		ts.Tick(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ts.Draw();
#endif
	}

	static inline void Setup()
	{
//		auto c= ts.CreateTask<Camera>(nullptr,0,-1);
//		auto s= ts.CreateTask<Stage>(nullptr,0,0);
	}
};
