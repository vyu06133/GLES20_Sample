#pragma once
#include "TaskBase.h"
#include "Shader.h"
#include "MyMath.h"

class TaskSystem
{
public:
	static const int32_t defaultTickPrio = 0;
	static const int32_t defaultDrawPrio = 0;
	TaskSystem();
	~TaskSystem();
	void Destroy(TaskBase* task);
	void DestroyAll();
	void SortPrio()
	{
		std::sort(
			ticks.begin(),
			ticks.end(),
			[](TaskBase* a, TaskBase* b)->bool {
				return(a->tickPriority > b->tickPriority);
			}
		);
		std::sort(
			draws.begin(),
			draws.end(),
			[](TaskBase* a, TaskBase* b)->bool {
				return(a->drawPriority > b->drawPriority);
			}
		);
	}
	void Tick(float Delta);
	void Draw() const;

	template<class T> T* CreateTask(TaskBase* const parent, const char* name = nullptr, int32_t tickPrio = defaultTickPrio, int32_t drawPrio = defaultDrawPrio)
	{
		do {
			taskId++;
		} while (taskMap.count(taskId));
		T* newtask = new T();
		if (newtask)
		{
			newtask->id = taskId;
			taskMap[taskId] = newtask;
			newtask->ts = this;
			newtask->name = name ? name : "no name";
			newtask->parent = parent;
			newtask->tickPriority = tickPrio;
			newtask->drawPriority = drawPrio;

			newtask->OnConstruct();
			tasks.push_back(newtask);
			ticks.push_back(newtask);
			draws.push_back(newtask);
			SortPrio();
			taskMap[taskId] = newtask;
			newtask->OnCreate();
		}
		return newtask;
	}

	std::vector<TaskBase*> FindTaskByName(const std::string& name)
	{
		std::vector<TaskBase*> result;
		for (auto& t : tasks)
		{
			if (t->name == name)
			{
				result.push_back(t);
			}
		}
		return result;
	}

	void BroadcastTouch(int action, int x, int y)
	{
		for (auto& t: tasks)
		{
			if (t && t->IsValid())
			{
				t->OnTouch(action, x, y);
			}
		}
	}

public:
	int32_t taskId = 0;
	std::vector<TaskBase*> tasks;//�쐬��
	std::unordered_map<int32_t, TaskBase*> taskMap;
	std::vector<TaskBase*> ticks;//tick��
	std::vector<TaskBase*> draws;//draw��
//	int32_t width_;
	float GetWidth() const {return shader_.GetWidth();}
//	int32_t height_;
	float GetHeight() const {return shader_.GetHeight();}
	Shader shader_;
	Shader& Shader() {return shader_;}
};

