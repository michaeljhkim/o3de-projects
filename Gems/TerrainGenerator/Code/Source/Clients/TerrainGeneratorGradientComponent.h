#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/parallel/shared_mutex.h>
#include <GradientSignal/Ebuses/GradientRequestBus.h>
#include <GradientSignal/Ebuses/GradientTransformRequestBus.h>
#include <TerrainGenerator/TerrainGeneratorGradientInterface.h>

//#include <External/FastNoise/FastNoise.h>
#include <External/FastNoiseLite.h>

namespace AZ
{
    //AZ_TYPE_INFO_SPECIALIZE(FastNoiseLite::Interp, "{9bb11a1f-2d8b-40c5-8bd9-ecb8bee70dcf}");
    AZ_TYPE_INFO_SPECIALIZE(FastNoiseLite::NoiseType, "{215ce177-1817-446e-a9b1-bd176505052e}");
    AZ_TYPE_INFO_SPECIALIZE(FastNoiseLite::FractalType, "{ea959923-ed2e-4ccd-8c99-331c0c13cbd4}");
    AZ_TYPE_INFO_SPECIALIZE(FastNoiseLite::CellularDistanceFunction, "{e0c14b5e-416a-44a7-a8d0-9319f4fd41b4}");
    AZ_TYPE_INFO_SPECIALIZE(FastNoiseLite::CellularReturnType, "{1e007e6a-8ee0-4154-85fb-6f0855720730}");
}

namespace LmbrCentral
{
    template<typename, typename>
    class EditorWrappedComponentBase;
}

namespace TerrainGenerator
{
    class TerrainGeneratorGradientConfig
        : public AZ::ComponentConfig
    {
    public:
        AZ_CLASS_ALLOCATOR(TerrainGeneratorGradientConfig, AZ::SystemAllocator);
        AZ_RTTI(TerrainGeneratorGradientConfig, "{93dc5b0b-5777-4a88-a088-944311ac1d5a}", AZ::ComponentConfig);
        static void Reflect(AZ::ReflectContext* context);

        AZ::u32 GetCellularParameterVisibility() const;
        AZ::u32 GetFractalParameterVisbility() const;
        AZ::u32 GetFrequencyParameterVisbility() const;
        AZ::u32 GetInterpParameterVisibility() const;

        bool operator==(const TerrainGeneratorGradientConfig& rhs) const;

        int m_seed = 1;
        float m_frequency = 1.f;
        //FastNoiseLite::Interp m_interp = FastNoiseLite::Interp::Quintic;
        FastNoiseLite::NoiseType m_noiseType = FastNoiseLite::NoiseType::NoiseType_Perlin;

        int m_octaves = 4;
        float m_lacunarity = 2.f;
        float m_gain = 0.5;
        FastNoiseLite::FractalType m_fractalType = FastNoiseLite::FractalType::FractalType_None;

        FastNoiseLite::CellularDistanceFunction m_cellularDistanceFunction = FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean;
        FastNoiseLite::CellularReturnType m_cellularReturnType = FastNoiseLite::CellularReturnType::CellularReturnType_CellValue;
        float m_cellularJitter = 0.45f;
    };

    inline constexpr AZ::TypeId TerrainGeneratorGradientComponentTypeId{ "{fe33d2b2-4843-457a-8eff-1b612fffbc79}" };

    class TerrainGeneratorGradientComponent
        : public AZ::Component
        , public TerrainGeneratorGradientRequestBus::Handler
        , private GradientSignal::GradientRequestBus::Handler
        , private GradientSignal::GradientTransformNotificationBus::Handler
    {
    public:
        friend class TerrainGeneratorGradientEditorComponent;
        template<typename, typename> friend class LmbrCentral::EditorWrappedComponentBase;

        //AZ_COMPONENT_DECL(TerrainGeneratorGradientComponent);
        AZ_COMPONENT(TerrainGenerator::TerrainGeneratorGradientComponent, "{2a74a3d4-0ba3-4f3c-a12a-ded002e9e2a8}", AZ::Component);

        /*
        * Reflects component data into the reflection contexts, including the serialization, edit, and behavior contexts.
        */
        static void Reflect(AZ::ReflectContext* context);

        /*
        * Specifies the services that this component provides.
        * Other components can declare a dependency on these services. The system uses this
        * information to determine the order of component activation.
        */
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);

        /*
        * Specifies the services that this component cannot operate with.
        * For example, if two components provide a similar service and the system cannot use the
        * services simultaneously, each of those components would specify the other component as
        * an incompatible service.
        * If an entity cannot have multiple instances of this component, include this component's
        * provided service in the list of incompatible services.
        */
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        /*
        * Specifies the services that this component requires.
        * The system activates the required services before it activates this component.
        * It also deactivates the required services after it deactivates this component.
        * If a required service is missing before this component is activated, the system
        * returns an error and does not activate this component.
        */
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        /*
        * Specifies the services that this component depends on, but does not require.
        * The system activates the dependent services before it activates this component.
        * It also deactivates the dependent services after it deactivates this component.
        * If a dependent service is missing before this component is activated, the system
        * does not return an error and still activates this component.
        */
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        TerrainGeneratorGradientComponent(const TerrainGeneratorGradientConfig& configuration);
        TerrainGeneratorGradientComponent() = default;

        // AZ::Component overrides...
        bool ReadInConfig(const AZ::ComponentConfig* baseConfig) override;
        bool WriteOutConfig(AZ::ComponentConfig* outBaseConfig) const override;

        // GradientRequestBus overrides...
        float GetValue(const GradientSignal::GradientSampleParams& sampleParams) const override;
        void GetValues(AZStd::span<const AZ::Vector3> positions, AZStd::span<float> outValues) const override;

    protected:
        /*
        * Puts this component into an active state.
        * The system calls this function once during activation of each entity that owns the
        * component. You must override this function. The system calls a component's Activate()
        * function only if all services and components that the component depends on are present
        * and active.
        */
        void Activate() override;

        /*
        * Deactivates this component.
        * The system calls this function when the owning entity is being deactivated. You must
        * override this function. As a best practice, ensure that this function returns this component
        * to a minimal footprint. The order of deactivation is the reverse of activation, so your
        * component is deactivated before the components it depends on.
        *
        * The system always calls a component's Deactivate() function before destroying the component.
        * However, deactivation is not always followed by the destruction of the component. An entity and
        * its components can be deactivated and reactivated without being destroyed. Ensure that your
        * Deactivate() implementation can handle this scenario.
        */
        void Deactivate() override;

        // Copied from the FastNoise
        TerrainGeneratorGradientConfig m_configuration;
        FastNoiseLite m_generator;
        GradientSignal::GradientTransform m_gradientTransform;
        mutable AZStd::shared_mutex m_queryMutex;

        // GradientTransformNotificationBus overrides...
        void OnGradientTransformChanged(const GradientSignal::GradientTransform& newTransform) override;

        // FastNoiseGradientRequest overrides...
        // Get functions - moved protected definitions to header for simplicity 
        int GetRandomSeed() const { return m_configuration.m_seed; }
        float GetFrequency() const { return m_configuration.m_frequency; }
        //FastNoiseLite::Interp GetInterpolation() const { return m_configuration.m_interp; }
        FastNoiseLite::NoiseType GetNoiseType() const { return m_configuration.m_noiseType; }
        int GetOctaves() const { return m_configuration.m_octaves; }
        float GetLacunarity() const { return m_configuration.m_lacunarity; }
        float GetGain() const { return m_configuration.m_gain; }
        FastNoiseLite::FractalType GetFractalType() const { return m_configuration.m_fractalType; }

        // Set functions - defined in cpp
        void SetRandomSeed(int seed) override;
        void SetFrequency(float freq) override;
        //void SetInterpolation(FastNoiseLite::Interp interp) override;
        void SetNoiseType(FastNoiseLite::NoiseType type) override;
        void SetOctaves(int octaves) override;
        void SetLacunarity(float lacunarity) override;
        void SetGain(float gain) override;
        void SetFractalType(FastNoiseLite::FractalType type) override;

        template <typename TValueType, TValueType TerrainGeneratorGradientConfig::*TConfigMember, void (FastNoiseLite::*TMethod)(TValueType)>
        void SetConfigValue(TValueType value);
    };
} // namespace TerrainGenerator
