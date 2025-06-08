
#pragma once

#include <TerrainGenerator/TerrainGeneratorTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace TerrainGenerator
{
    class TerrainGeneratorRequests
    {
    public:
        AZ_RTTI(TerrainGeneratorRequests, TerrainGeneratorRequestsTypeId);
        virtual ~TerrainGeneratorRequests() = default;
        // Put your public methods here
    };

    class TerrainGeneratorBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using TerrainGeneratorRequestBus = AZ::EBus<TerrainGeneratorRequests, TerrainGeneratorBusTraits>;
    using TerrainGeneratorInterface = AZ::Interface<TerrainGeneratorRequests>;

} // namespace TerrainGenerator
