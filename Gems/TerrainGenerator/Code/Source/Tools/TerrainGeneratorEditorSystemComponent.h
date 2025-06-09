
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>
#include <Clients/TerrainGeneratorSystemComponent.h>

namespace TerrainGenerator
{
    /// System component for TerrainGenerator editor
    class TerrainGeneratorEditorSystemComponent
        : public TerrainGeneratorSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = TerrainGeneratorSystemComponent;
    public:
        //AZ_COMPONENT_DECL(TerrainGeneratorEditorSystemComponent);
        AZ_COMPONENT(TerrainGeneratorEditorSystemComponent, TerrainGeneratorEditorSystemComponentTypeId);

        static void Reflect(AZ::ReflectContext* context);

        TerrainGeneratorEditorSystemComponent();
        ~TerrainGeneratorEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace TerrainGenerator
