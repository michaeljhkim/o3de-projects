
#pragma once

namespace TerrainGenerator
{
    // System Component TypeIds
    inline constexpr const char* TerrainGeneratorSystemComponentTypeId = "{07D08598-AF92-41A5-80F3-11830D17619A}";
    inline constexpr const char* TerrainGeneratorEditorSystemComponentTypeId = "{DA5AEA25-9B7F-4C93-A992-9F1C52C1A757}";

    // Module derived classes TypeIds
    inline constexpr const char* TerrainGeneratorModuleInterfaceTypeId = "{407C068B-DF4A-465A-B88D-D695E05CB901}";
    inline constexpr const char* TerrainGeneratorModuleTypeId = "{62CD4CF1-CDDD-4773-B960-EB6052E07BE0}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* TerrainGeneratorEditorModuleTypeId = TerrainGeneratorModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* TerrainGeneratorRequestsTypeId = "{D4561753-B9D8-4D6C-A6C4-3C4F8078A36B}";
} // namespace TerrainGenerator
