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
		mEye = e;
		mCenter = c;
		mUp = u;
	}

	virtual void SetClip(const float& n, const float& f)
	{
		mNear = n;
		mFar = f;
	}

	virtual void SetPers(const float& fov)
	{
		mPers = true;
		mFov = fov;
	}

	virtual void SetOrtho()
	{
		mPers = false;
	}

	virtual void OnPostTick() override
	{
	}

	virtual void OnPostDraw() override
	{
		assert(ts);
		auto s = ts->Shader();
		mEyeWorld = (worldMatrix * vec4(mEye, 1.0f)).xyz();
		auto centerWold = (worldMatrix * vec4(mCenter, 1.0f)).xyz();
		auto upWorld = glm::normalize((worldMatrix * vec4(mUp, 0.0f)).xyz());
		mViewMatrix = glm::lookAt(mEyeWorld, centerWold, upWorld);
		s.UpdateUniform(s.View, mViewMatrix);
		//スペキュラーのためEyeWorldをシェーダに渡す
		if (mPers)
		{
			mProjMatrix = glm::perspective(glm::radians(mFov), s.GetAspect(), mNear, mFar);
			s.UpdateUniform(s.Projection, mProjMatrix);
		}
		else
		{
			mProjMatrix = glm::ortho(0.0f, s.GetWidth(), s.GetHeight(), 0.0f, mNear, mFar);
			s.UpdateUniform(s.Projection, mProjMatrix);
		}
	}

	virtual vec3 Unproject(const vec3& scr) const
	{
		assert(ts);
		vec3 posVec = glm::unProject(
				vec3(scr.x, ts->GetHeight() - scr.y, scr.z),
				mViewMatrix,
				mProjMatrix,
				vec4(0.0f, 0.0f, ts->GetWidth(), ts->GetHeight()));
		return posVec;
	}

	vec3 mEye;
	vec3 mEyeWorld;
	vec3 mCenter;
	vec3 mUp;
	float mNear = 0.1f;
	float mFar = 10000.0f;
	float mFov = 60.0f;//deg
	bool mPers = true;
	mat4 mViewMatrix;
	mat4 mProjMatrix;
};