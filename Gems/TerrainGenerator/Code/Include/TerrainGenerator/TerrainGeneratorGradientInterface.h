#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <External/FastNoiseLite.h>

/*
- This is the request bus interface
- What allows the Terrain Spawner to see this as a valid gradient component
*/

namespace TerrainGenerator
{
    class TerrainGeneratorGradientRequests
        : public AZ::ComponentBus
    {
    public:
        //AZ_RTTI(TerrainGenerator::TerrainGeneratorGradientRequests, "{8E73FF49-B88A-4816-AEBE-51C14E0F3BCE}");
        AZ_RTTI(TerrainGenerator::TerrainGeneratorGradientRequests, "{2a74a3d4-0ba3-4f3c-a12a-ded002e9e2a8}");

        // Put your public request methods here.
        
        // Put notification events here. Examples:
        // void RegisterEvent(AZ::EventHandler<...> notifyHandler);
        // AZ::Event<...> m_notifyEvent1;
        
        /**
         * Overrides the default AZ::EBusTraits handler policy to allow one
         * listener only.
         */
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;

        virtual int GetRandomSeed() const = 0;
        virtual void SetRandomSeed(int seed) = 0;

        virtual float GetFrequency() const = 0;
        virtual void SetFrequency(float freq) = 0;

        virtual FastNoiseLite::NoiseType GetNoiseType() const = 0;
        virtual void SetNoiseType(FastNoiseLite::NoiseType type) = 0;

        virtual int GetOctaves() const = 0;
        virtual void SetOctaves(int octaves) = 0;

        virtual float GetLacunarity() const = 0;
        virtual void SetLacunarity(float lacunarity) = 0;

        virtual float GetGain() const = 0;
        virtual void SetGain(float gain) = 0;

        virtual FastNoiseLite::FractalType GetFractalType() const = 0;
        virtual void SetFractalType(FastNoiseLite::FractalType type) = 0;
    };

    using TerrainGeneratorGradientRequestBus = AZ::EBus<TerrainGeneratorGradientRequests>;

} // namespace TerrainGenerator
