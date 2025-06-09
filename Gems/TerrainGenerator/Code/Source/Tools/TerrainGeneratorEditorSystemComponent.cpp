
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include "TerrainGeneratorEditorSystemComponent.h"

#include <TerrainGenerator/TerrainGeneratorTypeIds.h>

namespace TerrainGenerator
{
    //AZ_COMPONENT_IMPL(TerrainGeneratorEditorSystemComponent, "TerrainGeneratorEditorSystemComponent", TerrainGeneratorEditorSystemComponentTypeId, BaseSystemComponent);

    void TerrainGeneratorEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorEditorSystemComponent, TerrainGeneratorSystemComponent>()
                ->Version(0);

            if (AZ::EditContext* ec = serializeContext->GetEditContext())
            {
                ec->Class<TerrainGeneratorEditorSystemComponent>("TerrainGenerator Gradient", "System component for TerrainGenerator gradient gem")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    TerrainGeneratorEditorSystemComponent::TerrainGeneratorEditorSystemComponent() = default;
    TerrainGeneratorEditorSystemComponent::~TerrainGeneratorEditorSystemComponent() = default;

    void TerrainGeneratorEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("TerrainGeneratorEditorService"));
    }

    void TerrainGeneratorEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("TerrainGeneratorEditorService"));
    }

    void TerrainGeneratorEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void TerrainGeneratorEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void TerrainGeneratorEditorSystemComponent::Activate()
    {
        TerrainGeneratorSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void TerrainGeneratorEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        TerrainGeneratorSystemComponent::Deactivate();
    }

} // namespace TerrainGenerator
