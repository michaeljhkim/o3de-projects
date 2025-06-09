
#pragma once

namespace TerrainGenerator
{
    // System Component TypeIds
    inline constexpr const char* TerrainGeneratorSystemComponentTypeId = "{66342265-cf2b-4819-9d55-338cbbeccda1}";
    inline constexpr const char* TerrainGeneratorEditorSystemComponentTypeId = "{ca7498eb-898c-4111-8367-44797b9f012d}";

    // Module derived classes TypeIds
    inline constexpr const char* TerrainGeneratorModuleInterfaceTypeId = "{a514b27b-bd09-4fcd-bf4c-ddbc01241faa}";
    inline constexpr const char* TerrainGeneratorModuleTypeId = "{7f902000-b4c8-44b9-9ff2-3fb2bf312f23}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* TerrainGeneratorEditorModuleTypeId = TerrainGeneratorModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* TerrainGeneratorRequestsTypeId = "{ccfe281c-5b5d-4a05-b188-df27c8d01779}";
} // namespace TerrainGenerator
