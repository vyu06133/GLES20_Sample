#include "TaskBase.h"

void TaskBase::OnConstruct()
{
}

void TaskBase::OnDestruct()
{
}

void TaskBase::OnTick(float deltaTime)
{
}

void TaskBase::OnPostTick()
{
}

void TaskBase::OnDraw()
{
}

void TaskBase::OnPostDraw()
{
}

void TaskBase::OnCreate()
{
}

void TaskBase::OnDestroy()
{
}

void TaskBase::OnTouch(int action, int x, int y)
{
}

void TaskBase::OnKeyDown(int vk)
{
}

void TaskBase::OnKeyUp(int vk)
{
}

bool TaskBase::IsValid() const
{
	return true;
}

void TaskBase::EnsureWorld()
{
	if (worldDirty)
	{
		// world_���������Ă��Ȃ��̂Ŋ���������
		if (parent && parent->IsValid())
		{
			if (parent->worldDirty)
			{
				// �e��world_�m���O��������
				parent->EnsureWorld();//�ċA
			}
			worldMatrix = localMatrix * parent->worldMatrix;
		}
		else
		{
			// root	�����ł���
			worldMatrix= localMatrix;//* taskSys_->origin_;
		}
		worldDirty = false;
	}
}
