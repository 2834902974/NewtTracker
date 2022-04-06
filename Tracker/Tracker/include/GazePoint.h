#pragma once

#include "InteractionPoint.h"
#include "tobii/tobii.h"
#include "tobii/tobii_streams.h"
#include <assert.h>


/// �ص��������д������ �ڴ��ݺ���ָ��ʱ ǰ׺��������� 
/// ��tobii Api��������ͳ�ͻ
/// ��˽���д�ڶ��� ����������
 
/// ���������GazePointֻ��һ��ʵ������ʱû������
/// ���Ҫ���Ƕ��ע�ӵ�ʱ ����Ҫ�����޸��߼�
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
