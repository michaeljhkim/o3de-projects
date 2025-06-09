
#include <TerrainGeneratorGradientComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace TerrainGenerator
{
    //AZ_COMPONENT_IMPL(TerrainGeneratorGradientComponent, "TerrainGeneratorGradientComponent", "{862772FD-B012-4F54-923F-B05960A88B1B}");

    void TerrainGeneratorGradientComponent::Activate()
    {
        TerrainGeneratorGradientRequestBus::Handler::BusConnect(GetEntityId());
    }

    void TerrainGeneratorGradientComponent::Deactivate()
    {
        TerrainGeneratorGradientRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    void TerrainGeneratorGradientComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorGradientComponent, AZ::Component>()
                ->Version(1)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<TerrainGeneratorGradientComponent>("TerrainGeneratorGradientComponent", "[Description of functionality provided by this component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ;
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<TerrainGeneratorGradientComponent>("TerrainGeneratorGradient Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "TerrainGenerator Gem Group")
                ;
        }
    }

    void TerrainGeneratorGradientComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TerrainGeneratorGradientComponentService"));
    }

    void TerrainGeneratorGradientComponent::GetIncompatibleServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
    }

    void TerrainGeneratorGradientComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void TerrainGeneratorGradientComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }
} // namespace TerrainGenerator
