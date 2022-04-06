#include "CursorPoint.h"

void CursorPoint::UpdatePosition()
{
    while (1)
    {
        GetCursorPos(m_pCurrentPoint);
        m_CurrentPoint.x = m_pCurrentPoint->x;
        m_CurrentPoint.y = m_pCurrentPoint->y;
    }
    
}
