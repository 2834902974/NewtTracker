#pragma once

#include <iostream>
#include <windows.h>

//这是一个抽象基类，子类为CursorPoint和GazePoint
class InteractionPoint
{
public:
	//没有私有成员变量 不需要写构造函数
	//InteractionPoint();
	//~InteractionPoint();


//Fields
protected:
	LPPOINT m_pCurrentPoint;

public:
	POINT m_CurrentPoint;

//Methods
public:
	/// <summary>
	/// 启动死循环更新m_CurrentPoint的值
	/// </summary>
	virtual void UpdatePosition() = 0;	
};
