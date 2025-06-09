#include "TaskSystem.h"
#include "Shader.h"

TaskSystem::TaskSystem()
{
}


TaskSystem::~TaskSystem()
{
	DestroyAll();
}

void TaskSystem::Tick(float Delta)
{
	for (auto e : ticks)
	{
		if (e->IsValid())
		{
			e->tickCount++;
			e->OnTick(Delta);
			e->elapsed += Delta;
		}
	}

	// world_���N���A����
	for (auto e : ticks)
	{
		if (e->IsValid())
		{
			e->worldDirty = true;
			e->EnsureWorld();//worldMatrix = mat4(1.0f);
		}
	}

	// ���ꂼ���OnPostTick���Ă�
	for (auto e : ticks)
	{
		if (e->IsValid())
		{
			e->OnPostTick();
		}
	}

	// ���ꂼ��̐e�q�֌W��world_���\�z����
	for (auto e : ticks)
	{
		if (e->IsValid())
		{
			e->EnsureWorld();
		}
	}
}

void TaskSystem::Draw() const
{
	for (auto e : draws)
	{
		if (e->IsValid())
		{
			e->EnsureWorld();
//			shader_.UpdateUniform(Shader::Model, e->worldMatrix);
			e->OnDraw();
			e->OnPostDraw();
		}
	}
}


void TaskSystem::Destroy(TaskBase* task)
{
	task->OnDestroy();
	auto ticksItr = std::remove_if(ticks.begin(), ticks.end(), [task](TaskBase* e) { return e == task; });
	ticks.erase(ticksItr, ticks.end());
	auto drawsItr = std::remove_if(draws.begin(), draws.end(), [task](TaskBase* e) { return e == task; });
	draws.erase(drawsItr, draws.end());
	auto taskItr = std::remove_if(tasks.begin(), tasks.end(), [task](TaskBase* e) { return e == task; });
	draws.erase(taskItr, tasks.end());
	auto mapItr = taskMap.find(task->id);
	if (mapItr != taskMap.end())
	{
		taskMap.erase(mapItr);
	}
	delete task;
}

void TaskSystem::DestroyAll()
{
	for (auto& task : tasks)
	{
		if (task)
		{
			auto ticksItr = std::remove_if(ticks.begin(), ticks.end(), [task](TaskBase* e) { return e == task; });
			ticks.erase(ticksItr, ticks.end());
			auto drawsItr = std::remove_if(draws.begin(), draws.end(), [task](TaskBase* e) { return e == task; });
			draws.erase(drawsItr, draws.end());
			task->OnDestroy();
			delete task;
		}
	}
	taskMap.clear();
	tasks.clear();
}

