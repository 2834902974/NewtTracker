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

/// 全局变量
// 全局的UIAutomation接口
IUIAutomation* g_pAutomation;
// UIA接口指针
IUIAutomationElement** ppGazeElement = new IUIAutomationElement*;
IUIAutomationElement** ppCursorElement = new IUIAutomationElement*;
//全局唯一标识符
SAFEARRAY** pGazeID = new SAFEARRAY*;
SAFEARRAY** pCursorID = new SAFEARRAY*;
//全局元素名
BSTR* pCursorName = new BSTR;
BSTR* pGazeName = new BSTR;


/// 函数原型声明
BOOL InitializeUIAutomation();

/// 程序入口点
int main()
{
    //调用一些简单的数学方法
    MathMethod MethodSet;

    RECT* pCursorRect = new RECT;
    RECT* pGazeRect = new RECT;
    // 初始化接口
    while (!InitializeUIAutomation())
    {
        printf("初始化失败!\n");
    }

    POINT LeftTop;
    POINT RightBottom;
    POINT CenterPoint;

    //启动子线程
    CursorPoint newCursorPoint;
    GazePoint newGazePoint;
    std::thread tCursor(&CursorPoint::UpdatePosition, &newCursorPoint);
    std::thread tGaze(&GazePoint::UpdatePosition, &newGazePoint);   

    int FirstEnterWhile = 1;

    //调用PID
    MathMethod HorizontalPID;
    MathMethod VerticalPID;

    CONTROLTYPEID* CtrlID = new CONTROLTYPEID;
    
    while (1)
    {

        //获取注视点元素
        if (g_pAutomation->ElementFromPoint(newGazePoint.m_CurrentPoint, ppGazeElement) != S_OK)
        {
            //cout << "注视点无元素" << endl;
            continue;
        }
        //lcchu
        (*ppGazeElement)->get_CurrentControlType(CtrlID);
        if (*CtrlID != UIA_ButtonControlTypeId && *CtrlID != UIA_ListItemControlTypeId && *CtrlID != UIA_TreeItemControlTypeId)
        {
            continue;
        }

        //获取光标点元素
        g_pAutomation->ElementFromPoint(newCursorPoint.m_CurrentPoint, ppCursorElement);

        //获取注视点boundingrect
        (*ppGazeElement)->get_CurrentBoundingRectangle(pGazeRect);
        //存放boundingrect的左上、右下点
        LeftTop.x = pGazeRect->left;
        LeftTop.y = pGazeRect->top;
        RightBottom.x = pGazeRect->right;
        RightBottom.y = pGazeRect->bottom;
        CenterPoint.x = (LONG)(LeftTop.x + RightBottom.x) / 2;
        CenterPoint.y = (LONG)(LeftTop.y + RightBottom.y) / 2;

        //增量式pid
        HorizontalPID.InitError(CenterPoint.x, newCursorPoint.m_CurrentPoint.x);
        VerticalPID.InitError(CenterPoint.y, newCursorPoint.m_CurrentPoint.y);

        while ((MethodSet.GetDistance(newCursorPoint.m_CurrentPoint, CenterPoint) >100 ) || FirstEnterWhile == 1)
        {    
            newCursorPoint.m_CurrentPoint.x += HorizontalPID.UpdateError(CenterPoint.x, newCursorPoint.m_CurrentPoint.x);
            newCursorPoint.m_CurrentPoint.y += VerticalPID.UpdateError(CenterPoint.y, newCursorPoint.m_CurrentPoint.y);

            if(CONTROLMODE)
                SetCursorPos(newCursorPoint.m_CurrentPoint.x, newCursorPoint.m_CurrentPoint.y);

            //更新注视点元素
            if (g_pAutomation->ElementFromPoint(newGazePoint.m_CurrentPoint, ppGazeElement) == S_OK)
            {
                (*ppGazeElement)->get_CurrentBoundingRectangle(pGazeRect);
                //获取元素名
                (*ppGazeElement)->get_CurrentName(pGazeName);
                //cout << pGazeRect->left << ", " << pGazeRect->top << endl;
                //cout << pGazeRect->right << ", " << pGazeRect->bottom << endl;
                //printf("注视元素名 is : %S", *pGazeName);
            }

            //更新光标点元素
            if (g_pAutomation->ElementFromPoint(newCursorPoint.m_CurrentPoint, ppCursorElement) == S_OK)
            {
                (*ppCursorElement)->get_CurrentBoundingRectangle(pCursorRect);
                //获取元素名
                (*ppCursorElement)->get_CurrentName(pCursorName);
                //printf("光标元素名 is : %S\n", *pCursorName);
            }
            FirstEnterWhile = 0;
        }

        FirstEnterWhile = 1;
        //cout << "一次匹配结束" << endl;
    }

    // 指针释放
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
    if (!CoInitialize(NULL)) // 初始化COM库
        return FALSE;
    HRESULT hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER,
        __uuidof(IUIAutomation), (void**)&g_pAutomation);
    return (SUCCEEDED(hr));
}