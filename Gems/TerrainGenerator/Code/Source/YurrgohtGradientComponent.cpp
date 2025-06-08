
#include <YurrgohtGradientComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace TerrainGenerator
{
    //AZ_COMPONENT_IMPL(YurrgohtGradientComponent, "YurrgohtGradientComponent", "{862772FD-B012-4F54-923F-B05960A88B1B}");

    void YurrgohtGradientComponent::Activate()
    {
        YurrgohtGradientRequestBus::Handler::BusConnect(GetEntityId());
    }

    void YurrgohtGradientComponent::Deactivate()
    {
        YurrgohtGradientRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    void YurrgohtGradientComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<YurrgohtGradientComponent, AZ::Component>()
                ->Version(1)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<YurrgohtGradientComponent>("YurrgohtGradientComponent", "[Description of functionality provided by this component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ;
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<YurrgohtGradientComponent>("YurrgohtGradient Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "TerrainGenerator Gem Group")
                ;
        }
    }

    void YurrgohtGradientComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("YurrgohtGradientComponentService"));
    }

    void YurrgohtGradientComponent::GetIncompatibleServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
    }

    void YurrgohtGradientComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void YurrgohtGradientComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }
} // namespace TerrainGenerator
