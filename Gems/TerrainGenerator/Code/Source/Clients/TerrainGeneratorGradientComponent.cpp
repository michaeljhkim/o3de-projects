#include <Clients/TerrainGeneratorGradientComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

// Headers that was in the FastNoise gem
#include <AzCore/Math/MathUtils.h>
#include <LmbrCentral/Dependency/DependencyNotificationBus.h>
//#include <External/FastNoise/FastNoise.h>
//#include <GradientSignal/Ebuses/GradientTransformRequestBus.h>

namespace TerrainGenerator
{
    AZ::u32 TerrainGeneratorGradientConfig::GetCellularParameterVisibility() const
    {
        return m_noiseType == FastNoise::NoiseType::Cellular ? AZ::Edit::PropertyVisibility::Show : AZ::Edit::PropertyVisibility::Hide;
    }

    AZ::u32 TerrainGeneratorGradientConfig::GetFractalParameterVisbility() const
    {
        switch (m_noiseType)
        {
        case FastNoise::NoiseType::CubicFractal:
        case FastNoise::NoiseType::PerlinFractal:
        case FastNoise::NoiseType::SimplexFractal:
        case FastNoise::NoiseType::ValueFractal:
            return AZ::Edit::PropertyVisibility::Show;
        }
        return AZ::Edit::PropertyVisibility::Hide;
    }

    AZ::u32 TerrainGeneratorGradientConfig::GetFrequencyParameterVisbility() const
    {
        return m_noiseType != FastNoise::NoiseType::WhiteNoise ? AZ::Edit::PropertyVisibility::Show : AZ::Edit::PropertyVisibility::Hide;
    }

    AZ::u32 TerrainGeneratorGradientConfig::GetInterpParameterVisibility() const
    {
        switch (m_noiseType)
        {
        case FastNoise::NoiseType::Value:
        case FastNoise::NoiseType::ValueFractal:
        case FastNoise::NoiseType::Perlin:
        case FastNoise::NoiseType::PerlinFractal:
            return AZ::Edit::PropertyVisibility::Show;
        }
        return AZ::Edit::PropertyVisibility::Hide;
    }

    bool TerrainGeneratorGradientConfig::operator==(const TerrainGeneratorGradientConfig& rhs) const
    {
        return (m_cellularDistanceFunction == rhs.m_cellularDistanceFunction)
        && (m_cellularJitter == rhs.m_cellularJitter)
        && (m_cellularReturnType == rhs.m_cellularReturnType)
        && (m_fractalType == rhs.m_fractalType)
        && (m_frequency == rhs.m_frequency)
        && (m_gain == rhs.m_gain)
        && (m_interp == rhs.m_interp)
        && (m_lacunarity == rhs.m_lacunarity)
        && (m_noiseType == rhs.m_noiseType)
        && (m_octaves == rhs.m_octaves)
        && (m_seed == rhs.m_seed);
    }

    void TerrainGeneratorGradientConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorGradientConfig, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("NoiseType", &TerrainGeneratorGradientConfig::m_noiseType)
                ->Field("Seed", &TerrainGeneratorGradientConfig::m_seed)
                ->Field("Frequency", &TerrainGeneratorGradientConfig::m_frequency)
                ->Field("Octaves", &TerrainGeneratorGradientConfig::m_octaves)
                ->Field("Lacunarity", &TerrainGeneratorGradientConfig::m_lacunarity)
                ->Field("Gain", &TerrainGeneratorGradientConfig::m_gain)
                ->Field("Interp", &TerrainGeneratorGradientConfig::m_interp)
                ->Field("FractalType", &TerrainGeneratorGradientConfig::m_fractalType)
                ->Field("CellularDistanceFunction", &TerrainGeneratorGradientConfig::m_cellularDistanceFunction)
                ->Field("CellularReturnType", &TerrainGeneratorGradientConfig::m_cellularReturnType)
                ->Field("CellularJitter", &TerrainGeneratorGradientConfig::m_cellularJitter)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<TerrainGeneratorGradientConfig>(
                    "FastNoise Gradient", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_seed, "Random Seed", "Using different seeds will cause the noise output to change")
                    ->Attribute(AZ::Edit::Attributes::Min, 1)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<int>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMin, 1)
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 100)
                    ->Attribute(AZ::Edit::Attributes::Step, 10)
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_noiseType, "Noise Type", "Sets the type of noise generator used")
                    ->Attribute(AZ::Edit::Attributes::Min, std::numeric_limits<int>::min())
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<int>::max())
                    ->EnumAttribute(FastNoise::NoiseType::Value, "Value")
                    ->EnumAttribute(FastNoise::NoiseType::ValueFractal, "Value Fractal")
                    ->EnumAttribute(FastNoise::NoiseType::Perlin, "Perlin")
                    ->EnumAttribute(FastNoise::NoiseType::PerlinFractal, "Perlin Fractal")
                    ->EnumAttribute(FastNoise::NoiseType::Simplex, "Simplex")
                    ->EnumAttribute(FastNoise::NoiseType::SimplexFractal, "Simplex Fractal")
                    ->EnumAttribute(FastNoise::NoiseType::Cellular, "Cellular")
                    ->EnumAttribute(FastNoise::NoiseType::WhiteNoise, "White Noise")
                    ->EnumAttribute(FastNoise::NoiseType::Cubic, "Cubic")
                    ->EnumAttribute(FastNoise::NoiseType::CubicFractal, "Cubic Fractal")
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_frequency, "Frequency", "Higher frequencies are more coarse")
                    ->Attribute(AZ::Edit::Attributes::DisplayDecimals, 4)
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0001f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 8.0f)
                    ->Attribute(AZ::Edit::Attributes::SliderCurveMidpoint, 0.25) // Give the frequency a non-linear scale slider with higher precision at the low end
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFrequencyParameterVisbility)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_octaves, "Octaves", "Number of recursions in the pattern generation, higher octaves refine the pattern")
                    ->Attribute(AZ::Edit::Attributes::Min, 0)
                    ->Attribute(AZ::Edit::Attributes::Max, 20)
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 8)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFractalParameterVisbility)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_lacunarity, "Lacunarity", "The frequency multiplier between each octave")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 5.f)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFractalParameterVisbility)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_gain, "Gain", "The relative strength of noise from each layer when compared to the last")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 5.f)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFractalParameterVisbility)
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_cellularDistanceFunction, "Distance Function", "Sets the distance function used to calculate the cell for a given point")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetCellularParameterVisibility)
                    ->EnumAttribute(FastNoise::CellularDistanceFunction::Euclidean, "Euclidean")
                    ->EnumAttribute(FastNoise::CellularDistanceFunction::Manhattan, "Manhattan")
                    ->EnumAttribute(FastNoise::CellularDistanceFunction::Natural, "Natural")
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_cellularReturnType, "Return Type", "Alters the value type the cellular function returns from its calculation")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetCellularParameterVisibility)
                    ->EnumAttribute(FastNoise::CellularReturnType::CellValue, "CellValue")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance, "Distance")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance2, "Distance2")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance2Add, "Distance2Add")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance2Sub, "Distance2Sub")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance2Mul, "Distance2Mul")
                    ->EnumAttribute(FastNoise::CellularReturnType::Distance2Div, "Distance2Div")
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_cellularJitter, "Jitter", "Sets the maximum distance a cellular point can move from its grid position")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 10.f)
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetCellularParameterVisibility)
                    ->ClassElement(AZ::Edit::ClassElements::Group, "FastNoise Advanced Settings")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_interp, "Interpolation", "Changes the interpolation method used to smooth between noise values")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetInterpParameterVisibility)
                    ->EnumAttribute(FastNoise::Interp::Linear, "Linear")
                    ->EnumAttribute(FastNoise::Interp::Hermite, "Hermite")
                    ->EnumAttribute(FastNoise::Interp::Quintic, "Quintic")
                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_fractalType, "Fractal Type", "Sets how the fractal is combined")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFractalParameterVisbility)
                    ->EnumAttribute(FastNoise::FractalType::FBM, "FBM")
                    ->EnumAttribute(FastNoise::FractalType::Billow, "Billow")
                    ->EnumAttribute(FastNoise::FractalType::RigidMulti, "Rigid Multi")
                    ;
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<TerrainGeneratorGradientConfig>()
                ->Constructor()
                ->Attribute(AZ::Script::Attributes::Category, "Vegetation")
                ->Property("randomSeed", BehaviorValueProperty(&TerrainGeneratorGradientConfig::m_seed))
                ->Property("frequency", BehaviorValueProperty(&TerrainGeneratorGradientConfig::m_frequency))
                ->Property("octaves", BehaviorValueProperty(&TerrainGeneratorGradientConfig::m_octaves))
                ->Property("lacunarity", BehaviorValueProperty(&TerrainGeneratorGradientConfig::m_lacunarity))
                ->Property("gain", BehaviorValueProperty(&TerrainGeneratorGradientConfig::m_gain))
                ->Property("noiseType",
                    [](TerrainGeneratorGradientConfig* config) { return (int&)(config->m_noiseType); },
                    [](TerrainGeneratorGradientConfig* config, const int& i) { config->m_noiseType = (FastNoise::NoiseType)i; })
                ->Property("interpolation",
                    [](TerrainGeneratorGradientConfig* config) { return (int&)(config->m_interp); },
                    [](TerrainGeneratorGradientConfig* config, const int& i) { config->m_interp = (FastNoise::Interp)i; })
                ->Property("fractalType",
                    [](TerrainGeneratorGradientConfig* config) { return (int&)(config->m_fractalType); },
                    [](TerrainGeneratorGradientConfig* config, const int& i) { config->m_fractalType = (FastNoise::FractalType)i; })
                ;
        }
    }

/*
************************************************************************************************************************************
------------------------------------------------------------------------------------------------------------------------------------
- THE CONFIG ENDS HERE
- THE ACTUAL COMPONENT STARTS HERE
------------------------------------------------------------------------------------------------------------------------------------
************************************************************************************************************************************
*/

    //AZ_COMPONENT_IMPL(TerrainGeneratorGradientComponent, "TerrainGeneratorGradientComponent", "{862772FD-B012-4F54-923F-B05960A88B1B}");

    void TerrainGeneratorGradientComponent::Activate()
    {
        // This will immediately call OnGradientTransformChanged and initialize m_gradientTransform.
        GradientSignal::GradientTransformNotificationBus::Handler::BusConnect(GetEntityId());

        // Some platforms require random seeds to be > 0.  Clamp to a positive range to ensure we're always safe.
        m_generator.SetSeed(AZ::GetMax(m_configuration.m_seed, 1));
        m_generator.SetFrequency(m_configuration.m_frequency);
        m_generator.SetInterp(m_configuration.m_interp);
        m_generator.SetNoiseType(m_configuration.m_noiseType);

        m_generator.SetFractalOctaves(m_configuration.m_octaves);
        m_generator.SetFractalLacunarity(m_configuration.m_lacunarity);
        m_generator.SetFractalGain(m_configuration.m_gain);
        m_generator.SetFractalType(m_configuration.m_fractalType);

        m_generator.SetCellularDistanceFunction(m_configuration.m_cellularDistanceFunction);
        m_generator.SetCellularReturnType(m_configuration.m_cellularReturnType);
        m_generator.SetCellularJitter(m_configuration.m_cellularJitter);

        TerrainGeneratorGradientRequestBus::Handler::BusConnect(GetEntityId());

        // Connect to GradientRequestBus last so that everything is initialized before listening for gradient queries.
        GradientSignal::GradientRequestBus::Handler::BusConnect(GetEntityId());
    }

    void TerrainGeneratorGradientComponent::Deactivate()
    {
        // Disconnect from GradientRequestBus first to ensure no queries are in process when deactivating.
        GradientSignal::GradientRequestBus::Handler::BusDisconnect();

        TerrainGeneratorGradientRequestBus::Handler::BusDisconnect(GetEntityId());
        GradientSignal::GradientTransformNotificationBus::Handler::BusDisconnect();
    }

    void TerrainGeneratorGradientComponent::Reflect(AZ::ReflectContext* context)
    {
        TerrainGeneratorGradientConfig::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorGradientComponent, AZ::Component>()
                ->Version(1)
                ->Field("Configuration", &TerrainGeneratorGradientComponent::m_configuration)
                ;

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<TerrainGeneratorGradientComponent>("TerrainGeneratorGradientComponent", "[Description of functionality provided by this component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "ComponentCategory")
                    ->Attribute(AZ::Edit::Attributes::Icon, "Icons/Components/Component_Placeholder.svg")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ;
            }
        }

        if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            /*
            behaviorContext->Class<TerrainGeneratorGradientComponent>("TerrainGeneratorGradient Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "TerrainGenerator Gem Group")
                ;
            */

            behaviorContext->Constant("TerrainGeneratorGradientComponentTypeId", BehaviorConstant(TerrainGeneratorGradientComponentTypeId));

            behaviorContext->Class<TerrainGeneratorGradientComponent>()->RequestBus("TerrainGeneratorGradientRequestBus");

            behaviorContext->EBus<TerrainGeneratorGradientRequestBus>("TerrainGeneratorGradientRequestBus")
                ->Attribute(AZ::Script::Attributes::Category, "Vegetation")
                ->Event("GetRandomSeed", &TerrainGeneratorGradientRequestBus::Events::GetRandomSeed)
                ->Event("SetRandomSeed", &TerrainGeneratorGradientRequestBus::Events::SetRandomSeed)
                ->VirtualProperty("RandomSeed", "GetRandomSeed", "SetRandomSeed")
                ->Event("GetFrequency", &TerrainGeneratorGradientRequestBus::Events::GetFrequency)
                ->Event("SetFrequency", &TerrainGeneratorGradientRequestBus::Events::SetFrequency)
                ->VirtualProperty("Frequency", "GetFrequency", "SetFrequency")
                ->Event("GetInterpolation", &TerrainGeneratorGradientRequestBus::Events::GetInterpolation)
                ->Event("SetInterpolation", &TerrainGeneratorGradientRequestBus::Events::SetInterpolation)
                ->VirtualProperty("Interpolation", "GetInterpolation", "SetInterpolation")
                ->Event("GetNoiseType", &TerrainGeneratorGradientRequestBus::Events::GetNoiseType)
                ->Event("SetNoiseType", &TerrainGeneratorGradientRequestBus::Events::SetNoiseType)
                ->VirtualProperty("NoiseType", "GetNoiseType", "SetNoiseType")
                ->Event("GetOctaves", &TerrainGeneratorGradientRequestBus::Events::GetOctaves)
                ->Event("SetOctaves", &TerrainGeneratorGradientRequestBus::Events::SetOctaves)
                ->VirtualProperty("Octaves", "GetOctaves", "SetOctaves")
                ->Event("GetLacunarity", &TerrainGeneratorGradientRequestBus::Events::GetLacunarity)
                ->Event("SetLacunarity", &TerrainGeneratorGradientRequestBus::Events::SetLacunarity)
                ->VirtualProperty("Lacunarity", "GetLacunarity", "SetLacunarity")
                ->Event("GetGain", &TerrainGeneratorGradientRequestBus::Events::GetGain)
                ->Event("SetGain", &TerrainGeneratorGradientRequestBus::Events::SetGain)
                ->VirtualProperty("Gain", "GetGain", "SetGain")
                ->Event("GetFractalType", &TerrainGeneratorGradientRequestBus::Events::GetFractalType)
                ->Event("SetFractalType", &TerrainGeneratorGradientRequestBus::Events::SetFractalType)
                ->VirtualProperty("FractalType", "GetFractalType", "SetFractalType")
                ;
        }
    }

    void TerrainGeneratorGradientComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        //provided.push_back(AZ_CRC_CE("TerrainGeneratorGradientComponentService"));
        provided.push_back(AZ_CRC_CE("GradientService"));
    }

    void TerrainGeneratorGradientComponent::GetIncompatibleServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GradientService"));
    }

    void TerrainGeneratorGradientComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("GradientTransformService"));
    }

    void TerrainGeneratorGradientComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent) 
    { 
    }

    TerrainGeneratorGradientComponent::TerrainGeneratorGradientComponent(const TerrainGeneratorGradientConfig& configuration)
        : m_configuration(configuration)
    {
    }



    bool TerrainGeneratorGradientComponent::ReadInConfig(const AZ::ComponentConfig* baseConfig)
    {
        if (auto config = azrtti_cast<const TerrainGeneratorGradientConfig*>(baseConfig))
        {
            m_configuration = *config;
            return true;
        }
        return false;
    }

    bool TerrainGeneratorGradientComponent::WriteOutConfig(AZ::ComponentConfig* outBaseConfig) const
    {
        if (auto config = azrtti_cast<TerrainGeneratorGradientConfig*>(outBaseConfig))
        {
            *config = m_configuration;
            return true;
        }
        return false;
    }

    void TerrainGeneratorGradientComponent::OnGradientTransformChanged(const GradientSignal::GradientTransform& newTransform)
    {
        AZStd::unique_lock lock(m_queryMutex);
        m_gradientTransform = newTransform;
    }

    float TerrainGeneratorGradientComponent::GetValue(const GradientSignal::GradientSampleParams& sampleParams) const
    {
        AZ::Vector3 uvw;
        bool wasPointRejected = false;

        AZStd::shared_lock lock(m_queryMutex);

        m_gradientTransform.TransformPositionToUVW(sampleParams.m_position, uvw, wasPointRejected);

        // Generator returns a range between [-1, 1], map that to [0, 1]
        return wasPointRejected ?
            0.0f :
            AZ::GetClamp((m_generator.GetNoise(uvw.GetX(), uvw.GetY(), uvw.GetZ()) + 1.0f) / 2.0f, 0.0f, 1.0f);
    }

    void TerrainGeneratorGradientComponent::GetValues(AZStd::span<const AZ::Vector3> positions, AZStd::span<float> outValues) const
    {
        if (positions.size() != outValues.size())
        {
            AZ_Assert(false, "input and output lists are different sizes (%zu vs %zu).", positions.size(), outValues.size());
            return;
        }

        AZStd::shared_lock lock(m_queryMutex);
        AZ::Vector3 uvw;

        for (size_t index = 0; index < positions.size(); index++)
        {
            bool wasPointRejected = false;

            m_gradientTransform.TransformPositionToUVW(positions[index], uvw, wasPointRejected);

            // Generator returns a range between [-1, 1], map that to [0, 1]
            outValues[index] = wasPointRejected ?
                0.0f :
                AZ::GetClamp((m_generator.GetNoise(uvw.GetX(), uvw.GetY(), uvw.GetZ()) + 1.0f) / 2.0f, 0.0f, 1.0f);
        }
    }

    template <typename TValueType, TValueType TerrainGeneratorGradientConfig::*TConfigMember, void (FastNoise::*TMethod)(TValueType)>
    void TerrainGeneratorGradientComponent::SetConfigValue(TValueType value)
    {
        // Only hold the lock while we're changing the data. Don't hold onto it during the OnCompositionChanged call, because that can
        // execute an arbitrary amount of logic, including calls back to this component.
        {
            AZStd::unique_lock lock(m_queryMutex);

            m_configuration.*TConfigMember = value;
            ((&m_generator)->*TMethod)(value);
        }

        LmbrCentral::DependencyNotificationBus::Event(GetEntityId(), &LmbrCentral::DependencyNotificationBus::Events::OnCompositionChanged);
    }

    void TerrainGeneratorGradientComponent::SetRandomSeed(int seed)
    {
        // Some platforms require random seeds to be > 0.  Clamp to a positive range to ensure we're always safe.
        SetConfigValue<int, &TerrainGeneratorGradientConfig::m_seed, &FastNoise::SetSeed>(AZ::GetMax(seed, 1));
    }

    void TerrainGeneratorGradientComponent::SetFrequency(float freq)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_frequency, &FastNoise::SetFrequency>(freq);
    }

    void TerrainGeneratorGradientComponent::SetInterpolation(FastNoise::Interp interp)
    {
        SetConfigValue<FastNoise::Interp, &TerrainGeneratorGradientConfig::m_interp, &FastNoise::SetInterp>(interp);
    }

    void TerrainGeneratorGradientComponent::SetNoiseType(FastNoise::NoiseType type)
    {
        SetConfigValue<FastNoise::NoiseType, &TerrainGeneratorGradientConfig::m_noiseType, &FastNoise::SetNoiseType>(type);
    }

    void TerrainGeneratorGradientComponent::SetOctaves(int octaves)
    {
        SetConfigValue<int, &TerrainGeneratorGradientConfig::m_octaves, &FastNoise::SetFractalOctaves>(octaves);
    }

    void TerrainGeneratorGradientComponent::SetLacunarity(float lacunarity)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_lacunarity, &FastNoise::SetFractalLacunarity>(lacunarity);
    }

    void TerrainGeneratorGradientComponent::SetGain(float gain)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_gain, &FastNoise::SetFractalGain>(gain);
    }

    void TerrainGeneratorGradientComponent::SetFractalType(FastNoise::FractalType type)
    {
        SetConfigValue<FastNoise::FractalType, &TerrainGeneratorGradientConfig::m_fractalType, &FastNoise::SetFractalType>(type);
    }

    
} // namespace TerrainGenerator
