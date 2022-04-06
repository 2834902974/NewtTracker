#pragma once
#include "InteractionPoint.h"


class CursorPoint : public InteractionPoint
{
	//Constructor & Destructor
public:
	CursorPoint()
	{
		m_pCurrentPoint = new POINT;
	}
	~CursorPoint()
	{
		delete m_pCurrentPoint;
	}

	//Methods
public:
	void UpdatePosition();

};

