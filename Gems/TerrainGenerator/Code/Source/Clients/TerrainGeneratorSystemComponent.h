
#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <TerrainGenerator/TerrainGeneratorBus.h>

namespace TerrainGenerator
{
    class TerrainGeneratorSystemComponent
        : public AZ::Component
        , protected TerrainGeneratorRequestBus::Handler
        , public AZ::TickBus::Handler
    {
    public:
        //AZ_COMPONENT_DECL(TerrainGeneratorSystemComponent);
        AZ_COMPONENT(TerrainGeneratorSystemComponent, "{baf3bd34-3b40-4b6a-a0be-da719649e1ee}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        TerrainGeneratorSystemComponent();
        ~TerrainGeneratorSystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // TerrainGeneratorRequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZTickBus interface implementation
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
        ////////////////////////////////////////////////////////////////////////
    };

} // namespace TerrainGenerator
