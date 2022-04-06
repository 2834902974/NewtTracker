#include "GazePoint.h"

POINT g_CurrentPoint;
#define PREVIEWMODE 1


void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void*)
{
    //cout << "进入注视捕捉程序" << endl;
    if (gaze_point->validity == TOBII_VALIDITY_VALID)
    {
        g_CurrentPoint.x = 1920 * gaze_point->position_xy[0];
        g_CurrentPoint.y = 1080 * gaze_point->position_xy[1];
    }
    //cout << g_GazePoint.x << ", " << g_GazePoint.y << endl;

}

void GazePoint::UpdatePosition()
{
    // Create API
    tobii_api_t* api = NULL;
    tobii_error_t result = tobii_api_create(&api, NULL, NULL);
    assert(result == TOBII_ERROR_NO_ERROR);

    // Enumerate devices to find connected eye trackers, keep the first
    char url[256] = { 0 };
    result = tobii_enumerate_local_device_urls(api, url_receiver, url);
    assert(result == TOBII_ERROR_NO_ERROR);
    if (*url == '\0')
    {
        printf("Error: No device found\n");
        return;
    }

    // Connect to the first tracker found
    tobii_device_t* device = NULL;
    result = tobii_device_create(api, url, TOBII_FIELD_OF_USE_INTERACTIVE, &device);
    assert(result == TOBII_ERROR_NO_ERROR);

    // Subscribe to gaze data
    result = tobii_gaze_point_subscribe(device, gaze_point_callback, 0);
    assert(result == TOBII_ERROR_NO_ERROR);

    std::cout << "眼动仪初始化成功" << std::endl;

    HDC screenDC = GetDC(0);    //获取当前屏幕的DC

    HGDIOBJ original = NULL;
    original = SelectObject(screenDC, GetStockObject(DC_PEN));
    //SetDCBrushColor(screenDC, RGB(0, 47, 167)); //克莱因蓝
    SetDCPenColor(screenDC, RGB(127, 0, 32)); //勃艮第红
    SelectObject(screenDC, GetStockObject(NULL_BRUSH));   //透明填充
    SetROP2(screenDC, R2_NOTXORPEN);
    
    while (1)
    {
        //清除之前的可视化，只显示最新注视点
        Rectangle(screenDC, m_CurrentPoint.x - 50, m_CurrentPoint.y - 50, m_CurrentPoint.x + 50, m_CurrentPoint.y + 50);

        // Optionally block this thread until data is available. Especially useful if running in a separate thread.
        result = tobii_wait_for_callbacks(1, &device);
        assert(result == TOBII_ERROR_NO_ERROR || result == TOBII_ERROR_TIMED_OUT);

        // Process callbacks on this thread if data is available
        result = tobii_device_process_callbacks(device);
        assert(result == TOBII_ERROR_NO_ERROR);

        m_CurrentPoint.x = g_CurrentPoint.x;
        m_CurrentPoint.y = g_CurrentPoint.y;

        //注视点可视化
        if (PREVIEWMODE)
            Rectangle(screenDC, m_CurrentPoint.x - 50, m_CurrentPoint.y - 50, m_CurrentPoint.x + 50, m_CurrentPoint.y + 50);
        
    }
    SelectObject(screenDC, original);    //复原

    // Cleanup
    result = tobii_gaze_point_unsubscribe(device);
    assert(result == TOBII_ERROR_NO_ERROR);
    result = tobii_device_destroy(device);
    assert(result == TOBII_ERROR_NO_ERROR);
    result = tobii_api_destroy(api);
    assert(result == TOBII_ERROR_NO_ERROR);
}

void GazePoint::url_receiver(char const* url, void* user_data)
{
    char* buffer = (char*)user_data;
    if (*buffer != '\0') return; // only keep first value

    if (strlen(url) < 256)
        strcpy(buffer, url);
}




