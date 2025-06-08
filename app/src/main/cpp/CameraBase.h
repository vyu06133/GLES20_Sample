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
		view_ = glm::lookAt(eyeWorld_, centerWorld_, upWorld_);
		assert(ts);
		auto w = (float)ts->Width();
		auto h = (float)ts->Height();
		assert(h > 0.0f);
		if (pers_)
		{
			auto aspect = w / h;
			proj_ = glm::perspective(glm::radians(fov_), aspect, near_, far_);
		}
		else
		{
			proj_ = glm::ortho(0.0f, w, h, 0.0f, near_, far_);
		}
	}

	vec3 eye_;
	vec3 center_;
	vec3 up_;
	vec3 eyeWorld_;
	vec3 centerWorld_;
	vec3 upWorld_;
	float near_ = 0.1f;
	float far_ = 20.0f;
	float fov_ = 30.0f;//deg
	bool pers_ = true;
	mat4 view_;
	mat4 proj_;
};