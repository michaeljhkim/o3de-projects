
#include <AzCore/Serialization/SerializeContext.h>

#include "yurrgoht-o3deSystemComponent.h"

#include <yurrgoht-o3de/yurrgoht-o3deTypeIds.h>

namespace yurrgoht_o3de
{
    AZ_COMPONENT_IMPL(yurrgoht_o3deSystemComponent, "yurrgoht_o3deSystemComponent",
        yurrgoht_o3deSystemComponentTypeId);

    void yurrgoht_o3deSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<yurrgoht_o3deSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void yurrgoht_o3deSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("yurrgoht_o3deService"));
    }

    void yurrgoht_o3deSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("yurrgoht_o3deService"));
    }

    void yurrgoht_o3deSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void yurrgoht_o3deSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    yurrgoht_o3deSystemComponent::yurrgoht_o3deSystemComponent()
    {
        if (yurrgoht_o3deInterface::Get() == nullptr)
        {
            yurrgoht_o3deInterface::Register(this);
        }
    }

    yurrgoht_o3deSystemComponent::~yurrgoht_o3deSystemComponent()
    {
        if (yurrgoht_o3deInterface::Get() == this)
        {
            yurrgoht_o3deInterface::Unregister(this);
        }
    }

    void yurrgoht_o3deSystemComponent::Init()
    {
    }

    void yurrgoht_o3deSystemComponent::Activate()
    {
        yurrgoht_o3deRequestBus::Handler::BusConnect();
    }

    void yurrgoht_o3deSystemComponent::Deactivate()
    {
        yurrgoht_o3deRequestBus::Handler::BusDisconnect();
    }
}
