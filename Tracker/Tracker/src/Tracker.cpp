#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <uiautomationclient.h>
#include <thread>
#include <cmath>
#include "CursorPoint.h"
#include "GazePoint.h"
#include "MathMethod.h"

#define CONTROLMODE 1


using std::cout;
using std::endl;

/// ȫ�ֱ���
// ȫ�ֵ�UIAutomation�ӿ�
IUIAutomation* g_pAutomation;
// UIA�ӿ�ָ��
IUIAutomationElement** ppGazeElement = new IUIAutomationElement*;
IUIAutomationElement** ppCursorElement = new IUIAutomationElement*;
//ȫ��Ψһ��ʶ��
SAFEARRAY** pGazeID = new SAFEARRAY*;
SAFEARRAY** pCursorID = new SAFEARRAY*;
//ȫ��Ԫ����
BSTR* pCursorName = new BSTR;
BSTR* pGazeName = new BSTR;


/// ����ԭ������
BOOL InitializeUIAutomation();

/// ������ڵ�
int main()
{
    //����һЩ�򵥵���ѧ����
    MathMethod MethodSet;

    RECT* pCursorRect = new RECT;
    RECT* pGazeRect = new RECT;
    // ��ʼ���ӿ�
    while (!InitializeUIAutomation())
    {
        printf("��ʼ��ʧ��!\n");
    }

    POINT LeftTop;
    POINT RightBottom;
    POINT CenterPoint;

    //�������߳�
    CursorPoint newCursorPoint;
    GazePoint newGazePoint;
    std::thread tCursor(&CursorPoint::UpdatePosition, &newCursorPoint);
    std::thread tGaze(&GazePoint::UpdatePosition, &newGazePoint);   

    int FirstEnterWhile = 1;

    //����PID
    MathMethod HorizontalPID;
    MathMethod VerticalPID;

    CONTROLTYPEID* CtrlID = new CONTROLTYPEID;
    
    while (1)
    {

        //��ȡע�ӵ�Ԫ��
        if (g_pAutomation->ElementFromPoint(newGazePoint.m_CurrentPoint, ppGazeElement) != S_OK)
        {
            //cout << "ע�ӵ���Ԫ��" << endl;
            continue;
        }
        //lcchu
        (*ppGazeElement)->get_CurrentControlType(CtrlID);
        if (*CtrlID != UIA_ButtonControlTypeId && *CtrlID != UIA_ListItemControlTypeId && *CtrlID != UIA_TreeItemControlTypeId)
        {
            continue;
        }

        //��ȡ����Ԫ��
        g_pAutomation->ElementFromPoint(newCursorPoint.m_CurrentPoint, ppCursorElement);

        //��ȡע�ӵ�boundingrect
        (*ppGazeElement)->get_CurrentBoundingRectangle(pGazeRect);
        //���boundingrect�����ϡ����µ�
        LeftTop.x = pGazeRect->left;
        LeftTop.y = pGazeRect->top;
        RightBottom.x = pGazeRect->right;
        RightBottom.y = pGazeRect->bottom;
        CenterPoint.x = (LONG)(LeftTop.x + RightBottom.x) / 2;
        CenterPoint.y = (LONG)(LeftTop.y + RightBottom.y) / 2;

        //����ʽpid
        HorizontalPID.InitError(CenterPoint.x, newCursorPoint.m_CurrentPoint.x);
        VerticalPID.InitError(CenterPoint.y, newCursorPoint.m_CurrentPoint.y);

        while ((MethodSet.GetDistance(newCursorPoint.m_CurrentPoint, CenterPoint) >100 ) || FirstEnterWhile == 1)
        {    
            newCursorPoint.m_CurrentPoint.x += HorizontalPID.UpdateError(CenterPoint.x, newCursorPoint.m_CurrentPoint.x);
            newCursorPoint.m_CurrentPoint.y += VerticalPID.UpdateError(CenterPoint.y, newCursorPoint.m_CurrentPoint.y);

            if(CONTROLMODE)
                SetCursorPos(newCursorPoint.m_CurrentPoint.x, newCursorPoint.m_CurrentPoint.y);

            //����ע�ӵ�Ԫ��
            if (g_pAutomation->ElementFromPoint(newGazePoint.m_CurrentPoint, ppGazeElement) == S_OK)
            {
                (*ppGazeElement)->get_CurrentBoundingRectangle(pGazeRect);
                //��ȡԪ����
                (*ppGazeElement)->get_CurrentName(pGazeName);
                //cout << pGazeRect->left << ", " << pGazeRect->top << endl;
                //cout << pGazeRect->right << ", " << pGazeRect->bottom << endl;
                //printf("ע��Ԫ���� is : %S", *pGazeName);
            }

            //���¹���Ԫ��
            if (g_pAutomation->ElementFromPoint(newCursorPoint.m_CurrentPoint, ppCursorElement) == S_OK)
            {
                (*ppCursorElement)->get_CurrentBoundingRectangle(pCursorRect);
                //��ȡԪ����
                (*ppCursorElement)->get_CurrentName(pCursorName);
                //printf("���Ԫ���� is : %S\n", *pCursorName);
            }
            FirstEnterWhile = 0;
        }

        FirstEnterWhile = 1;
        //cout << "һ��ƥ�����" << endl;
    }

    // ָ���ͷ�
    delete pCursorName;
    delete pGazeName;
    delete pGazeRect;
    delete pCursorRect;
    delete ppGazeElement;
    delete ppCursorElement;
    delete pGazeID;
    delete pCursorID;

    delete CtrlID;

    return 0;
}

BOOL InitializeUIAutomation()
{
    if (!CoInitialize(NULL)) // ��ʼ��COM��
        return FALSE;
    HRESULT hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IUIAutomation), (void**)&g_pAutomation);
    return (SUCCEEDED(hr));
}