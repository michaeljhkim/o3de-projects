
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "yurrgoht-o3deSystemComponent.h"

#include <yurrgoht-o3de/yurrgoht-o3deTypeIds.h>

namespace yurrgoht_o3de
{
    class yurrgoht_o3deModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(yurrgoht_o3deModule, yurrgoht_o3deModuleTypeId, AZ::Module);
        AZ_CLASS_ALLOCATOR(yurrgoht_o3deModule, AZ::SystemAllocator);

        yurrgoht_o3deModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                yurrgoht_o3deSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<yurrgoht_o3deSystemComponent>(),
            };
        }
    };
}// namespace yurrgoht_o3de

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), yurrgoht_o3de::yurrgoht_o3deModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_yurrgoht_o3de, yurrgoht_o3de::yurrgoht_o3deModule)
#endif
