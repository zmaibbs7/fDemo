#include "MapModelManager.h"
#include <QQmlEngine>
namespace dm
{

    MapModelManager::MapModelManager(QObject *parent):QObject(parent)
    {

    }
    
    MapModelManager::~MapModelManager()
    {

    }

    QObject* MapModelManager::mapModel(int floorIndex, bool isCurrentFloor)
    {
        QQmlEngine::setObjectOwnership(&listModel, QQmlEngine::CppOwnership);
        return &listModel;
    }

    bool MapModelManager::switchMap(int floor)
    {
        return false;
    }

    bool MapModelManager::loadMap(const std::vector<std::string> &pathList)
    {
        return false;
    }

} // namespace dm
