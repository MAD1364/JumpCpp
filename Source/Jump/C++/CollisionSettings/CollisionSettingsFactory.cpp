
#include "CollisionSettingsFactory.h"
#include "Engine/EngineTypes.h"

FCollisionResponseContainer CreateCollisionResponseContainer()
{
    return FCollisionResponseContainer(ECollisionResponse::ECR_Overlap);
}
