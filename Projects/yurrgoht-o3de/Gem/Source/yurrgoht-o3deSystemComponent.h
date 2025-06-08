
#pragma once

#include <AzCore/Component/Component.h>

#include <yurrgoht-o3de/yurrgoht-o3deBus.h>

namespace yurrgoht_o3de
{
    class yurrgoht_o3deSystemComponent
        : public AZ::Component
        , protected yurrgoht_o3deRequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(yurrgoht_o3deSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        yurrgoht_o3deSystemComponent();
        ~yurrgoht_o3deSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // yurrgoht_o3deRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
