//
// Created by vyu06133 on 2025/05/26 月.
//

#pragma once
#include "TaskBase.h"

class  CameraBase : public TaskBase
{
public:
	CameraBase()
	{
	}

	virtual void LookAt(const vec3& e, const vec3& c, const vec3& u)
	{
		eye_ = e;
		center_ = c;
		up_ = u;
	}

	virtual void SetClip(const float& n, const float& f)
	{
		near_ = n;
		far_ = f;
	}

	virtual void SetPers(const float& fov)
	{
		pers_ = true;
		fov_ = fov;
	}

	virtual void SetOrtho()
	{
		pers_ = false;
	}

	virtual void OnPostTick() override
	{
		eyeWorld_ = (worldMatrix * vec4(eye_, 1.0f)).xyz();
		centerWorld_ = (worldMatrix * vec4(center_, 1.0f)).xyz();
		upWorld_  = glm::normalize((worldMatrix * vec4(up_, 0.0f)).xyz());
	}

	virtual void OnPostDraw() override
	{
		view_ = glm::lookAt(eyeWorld_, centerWorld_, upWorld_);
		assert(ts);
		auto s = ts->Shader();
		if (pers_)
		{
			proj_ = glm::perspective(glm::radians(fov_), s.GetAspect(), near_, far_);
		}
		else
		{
			proj_ = glm::ortho(0.0f, s.GetWidth(), s.GetHeight(), 0.0f, near_, far_);
		}
		s.UpdateUniform(s.View, view_);
		s.UpdateUniform(s.Projection, proj_);
	}

	virtual vec3 Unproject(const vec3& scr) const
	{
		auto x = static_cast<float>(scr.x);
		auto y = static_cast<float>(scr.y);
		assert(ts);
		auto w = (float)ts->Width();
		auto h = (float)ts->Height();
		vec3 s(scr.x, h - scr.y, scr.z);
		vec3 posVec = glm::unProject(
				s,
				view_,
				proj_,
				vec4(0.0f, 0.0f, w, h));
		return posVec;
	}

	vec3 eye_;
	vec3 center_;
	vec3 up_;
	vec3 eyeWorld_;
	vec3 centerWorld_;
	vec3 upWorld_;
	float near_ = 0.1f;
	float far_ = 20.0f;
	float fov_ = 60.0f;//deg
	bool pers_ = true;
	mat4 view_;
	mat4 proj_;
};