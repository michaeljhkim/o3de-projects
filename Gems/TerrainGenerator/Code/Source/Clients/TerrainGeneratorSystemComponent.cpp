
#include "TerrainGeneratorSystemComponent.h"
#include <TerrainGenerator/TerrainGeneratorTypeIds.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

namespace TerrainGenerator
{
    //AZ_COMPONENT_IMPL(TerrainGeneratorSystemComponent, "TerrainGeneratorSystemComponent", TerrainGeneratorSystemComponentTypeId);

    void TerrainGeneratorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void TerrainGeneratorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TerrainGeneratorService"));
    }

    void TerrainGeneratorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("TerrainGeneratorService"));
    }

    void TerrainGeneratorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        (void)required;
    }

    void TerrainGeneratorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        (void)dependent;
    }

    TerrainGeneratorSystemComponent::TerrainGeneratorSystemComponent()
    {
        if (TerrainGeneratorInterface::Get() == nullptr)
        {
            TerrainGeneratorInterface::Register(this);
        }
    }

    TerrainGeneratorSystemComponent::~TerrainGeneratorSystemComponent()
    {
        if (TerrainGeneratorInterface::Get() == this)
        {
            TerrainGeneratorInterface::Unregister(this);
        }
    }

    void TerrainGeneratorSystemComponent::Init()
    {
    }

    void TerrainGeneratorSystemComponent::Activate()
    {
        TerrainGeneratorRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void TerrainGeneratorSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        TerrainGeneratorRequestBus::Handler::BusDisconnect();
    }

    void TerrainGeneratorSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace TerrainGenerator
