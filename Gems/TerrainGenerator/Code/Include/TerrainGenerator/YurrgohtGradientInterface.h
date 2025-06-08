
#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace TerrainGenerator
{
    class YurrgohtGradientRequests
        : public AZ::ComponentBus
    {
    public:
        //AZ_RTTI(TerrainGenerator::YurrgohtGradientRequests, "{8E73FF49-B88A-4816-AEBE-51C14E0F3BCE}");
        AZ_RTTI(TerrainGenerator::YurrgohtGradientRequests, "{2a74a3d4-0ba3-4f3c-a12a-ded002e9e2a8}");

        // Put your public request methods here.
        
        // Put notification events here. Examples:
        // void RegisterEvent(AZ::EventHandler<...> notifyHandler);
        // AZ::Event<...> m_notifyEvent1;
        
    };

    using YurrgohtGradientRequestBus = AZ::EBus<YurrgohtGradientRequests>;

} // namespace TerrainGenerator
