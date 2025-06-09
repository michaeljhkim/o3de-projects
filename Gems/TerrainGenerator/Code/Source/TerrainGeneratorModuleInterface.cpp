
#include "TerrainGeneratorModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <TerrainGenerator/TerrainGeneratorTypeIds.h>
#include <Clients/TerrainGeneratorSystemComponent.h>

namespace TerrainGenerator
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(TerrainGeneratorModuleInterface,
        "TerrainGeneratorModuleInterface", TerrainGeneratorModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(TerrainGeneratorModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(TerrainGeneratorModuleInterface, AZ::SystemAllocator);

    TerrainGeneratorModuleInterface::TerrainGeneratorModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            TerrainGeneratorSystemComponent::CreateDescriptor(),
            TerrainGeneratorGradientComponent::CreateDescriptor()
            });
    }

    AZ::ComponentTypeList TerrainGeneratorModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<TerrainGeneratorSystemComponent>(),
        };
    }
} // namespace TerrainGenerator
