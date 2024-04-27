#ifndef MAPPOINT_H
#define MAPPOINT_H

#include <QObject>
#include <QString>
#include "../MapElement.h"

namespace dm {
    class MapPoint : public MapElement
    {
    public:
        //属性
        QString            partition               = "";      // 用户自定义分组名称
        tfPoint            pos;                               // 坐标
        int                isError                 = 0;       // 位置点错误| 0 无错误
        //归属
        QString            parentPathUuid          = "";      // 所属路径的uuid
        tfPoint            projectionPoint;                   // 送餐点 - 在路径上的投影点
        bool               projectionToDraw        = false;   // 是否绘制投影路径
        //调度
        bool               dispatch_isWait         = false;   // [调度]是否等待
        int                dispatch_dockMode       = 0;       // [调度]临停模式
        double             dispatch_dockDistance   = 0.0;     // [调度]临停距离
        bool               dispatch_isBinded       = false;   // [调度]绑定的出餐口 uuid
        QString            dispatch_dockOutlet     = "";      // [调度]临停点绑定的出餐口uuid
        bool               dispatch_isWelcomePoint = false;   // [调度]是否是门迎点 机器配对
        //图片资源
        QPixmap*           imgIcon                 = nullptr; // 位置点图标类型
        QPixmap*           imgArrow                = nullptr; // 箭头图标
    };
}//namespace dm

#endif // MAPPOINT_H
