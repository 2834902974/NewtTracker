#pragma once

#include "InteractionPoint.h"
#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include <assert.h>


/// 回调函数如果写在类内 在传递函数指针时 前缀会加上类名 
/// 与tobii Api定义的类型冲突
/// 因此将其写在顶层 解决这个问题
 
/// 这种情况在GazePoint只有一个实例对象时没有问题
/// 如果要考虑多个注视点时 还需要重新修改逻辑
void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void*);

class GazePoint : public InteractionPoint
{
	//Constructor & Destructor
public:
	GazePoint()
	{
		m_pCurrentPoint = new POINT;
	}
	~GazePoint()
	{
		delete m_pCurrentPoint;
	}

	//Fields
public:


	//Methods
public:
	void UpdatePosition();
	static void url_receiver(char const* url, void* user_data);
};
