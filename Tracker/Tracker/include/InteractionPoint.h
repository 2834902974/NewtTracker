#pragma once

#include <iostream>
#include <windows.h>

//����һ��������࣬����ΪCursorPoint��GazePoint
class InteractionPoint
{
public:
	//û��˽�г�Ա���� ����Ҫд���캯��
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
	/// ������ѭ������m_CurrentPoint��ֵ
	/// </summary>
	virtual void UpdatePosition() = 0;	
};
