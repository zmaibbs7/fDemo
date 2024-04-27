#ifndef MAPELEMENT_H_
#define MAPELEMENT_H_

#include <QObject>

namespace dm {
    class MapElement
    {
    public:
        QString         uuid;
        QString         name;
        int             index;
        int             typeID;
    protected:
        bool            isSelected;
    };

    class tfPoint
    {
    private:
        QPointF         m_pixelCoord;
        double          m_yaw;
    };
}//namespace dm

#endif // MAPELEMENT_H_
