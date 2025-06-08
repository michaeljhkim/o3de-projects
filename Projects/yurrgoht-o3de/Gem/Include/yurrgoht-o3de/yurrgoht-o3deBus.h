
#pragma once

#include <yurrgoht-o3de/yurrgoht-o3deTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace yurrgoht_o3de
{
    class yurrgoht_o3deRequests
    {
    public:
        AZ_RTTI(yurrgoht_o3deRequests, yurrgoht_o3deRequestsTypeId);
        virtual ~yurrgoht_o3deRequests() = default;
        // Put your public methods here
    };

    class yurrgoht_o3deBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using yurrgoht_o3deRequestBus = AZ::EBus<yurrgoht_o3deRequests, yurrgoht_o3deBusTraits>;
    using yurrgoht_o3deInterface = AZ::Interface<yurrgoht_o3deRequests>;

} // namespace yurrgoht_o3de
