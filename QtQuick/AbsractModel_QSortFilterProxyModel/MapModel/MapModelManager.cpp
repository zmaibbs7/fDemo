#include "MapModelManager.h"

namespace dm
{

    MapModelManager::MapModelManager(QObject *parent):QObject(parent)
    {
        // mapModel.push_back(MapModel());
    }
    
    MapModelManager::~MapModelManager()
    {

    }

    MapModel *MapModelManager::getMap(int floorIndex, bool isCurrentFloor)
    {
        return nullptr;
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
