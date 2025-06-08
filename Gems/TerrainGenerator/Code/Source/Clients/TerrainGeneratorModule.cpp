
#include <TerrainGenerator/TerrainGeneratorTypeIds.h>
#include <TerrainGeneratorModuleInterface.h>
#include "TerrainGeneratorSystemComponent.h"

namespace TerrainGenerator
{
    class TerrainGeneratorModule
        : public TerrainGeneratorModuleInterface
    {
    public:
        AZ_RTTI(TerrainGeneratorModule, TerrainGeneratorModuleTypeId, TerrainGeneratorModuleInterface);
        AZ_CLASS_ALLOCATOR(TerrainGeneratorModule, AZ::SystemAllocator);
    };
}// namespace TerrainGenerator

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), TerrainGenerator::TerrainGeneratorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_TerrainGenerator, TerrainGenerator::TerrainGeneratorModule)
#endif
