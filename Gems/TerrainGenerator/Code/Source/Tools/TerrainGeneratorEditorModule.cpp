
#include <TerrainGenerator/TerrainGeneratorTypeIds.h>
#include <TerrainGeneratorModuleInterface.h>
#include "TerrainGeneratorEditorSystemComponent.h"

namespace TerrainGenerator
{
    class TerrainGeneratorEditorModule
        : public TerrainGeneratorModuleInterface
    {
    public:
        AZ_RTTI(TerrainGeneratorEditorModule, TerrainGeneratorEditorModuleTypeId, TerrainGeneratorModuleInterface);
        AZ_CLASS_ALLOCATOR(TerrainGeneratorEditorModule, AZ::SystemAllocator);

        TerrainGeneratorEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                TerrainGeneratorEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<TerrainGeneratorEditorSystemComponent>(),
            };
        }
    };
}// namespace TerrainGenerator

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), TerrainGenerator::TerrainGeneratorEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_TerrainGenerator_Editor, TerrainGenerator::TerrainGeneratorEditorModule)
#endif
