#include <Clients/TerrainGeneratorGradientComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

// Headers that was in the FastNoise gem
#include <AzCore/Math/MathUtils.h>
#include <LmbrCentral/Dependency/DependencyNotificationBus.h>
//#include <External/FastNoise/FastNoise.h>
//#include <GradientSignal/Ebuses/GradientTransformRequestBus.h>

/*
- From what I can tell, the GetValues(positions, outValues) function is where the noise is calculated and applied 
- 'Terrain Height Gradient List' provides a list of world positions that it needs to calculate noise for
- GetValues() takes the positions, pops them into the fast noise algorithm, then returns the values as outputs

- This is most ideal for me since I would not even need to determine the positions that need to be calculated
- I just need to create the algorithm that is to be applied to each position
*/

namespace TerrainGenerator
{
    AZ::u32 TerrainGeneratorGradientConfig::GetCellularParameterVisibility() const
    {
        return m_noiseType == FastNoiseLite::NoiseType::NoiseType_Cellular ? AZ::Edit::PropertyVisibility::Show : AZ::Edit::PropertyVisibility::Hide;
    }

    AZ::u32 TerrainGeneratorGradientConfig::GetFractalParameterVisbility() const
    {
        switch (m_fractalType)
        {
        case FastNoiseLite::FractalType::FractalType_FBm:
        case FastNoiseLite::FractalType::FractalType_Ridged:
        case FastNoiseLite::FractalType::FractalType_PingPong:
        case FastNoiseLite::FractalType::FractalType_DomainWarpProgressive:
        case FastNoiseLite::FractalType::FractalType_DomainWarpIndependent:
        /*
        case FastNoiseLite::NoiseType::NoiseType_Perlin:
        case FastNoiseLite::NoiseType::NoiseType_OpenSimplex2:
        case FastNoiseLite::NoiseType::NoiseType_OpenSimplex2S:
        case FastNoiseLite::NoiseType::NoiseType_Value:
        */
            return AZ::Edit::PropertyVisibility::Show;
        }
        return AZ::Edit::PropertyVisibility::Hide;
    }

    AZ::u32 TerrainGeneratorGradientConfig::GetFrequencyParameterVisbility() const
    {
        //return m_noiseType != FastNoiseLite::NoiseType::WhiteNoise ? AZ::Edit::PropertyVisibility::Show : AZ::Edit::PropertyVisibility::Hide;
        return AZ::Edit::PropertyVisibility::Show;
    }

    bool TerrainGeneratorGradientConfig::operator==(const TerrainGeneratorGradientConfig& rhs) const
    {
        return (m_cellularDistanceFunction == rhs.m_cellularDistanceFunction)
        && (m_cellularJitter == rhs.m_cellularJitter)
        && (m_cellularReturnType == rhs.m_cellularReturnType)
        && (m_fractalType == rhs.m_fractalType)
        && (m_frequency == rhs.m_frequency)
        && (m_gain == rhs.m_gain)
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
                ->Field("FractalType", &TerrainGeneratorGradientConfig::m_fractalType)
                ->Field("CellularDistanceFunction", &TerrainGeneratorGradientConfig::m_cellularDistanceFunction)
                ->Field("CellularReturnType", &TerrainGeneratorGradientConfig::m_cellularReturnType)
                ->Field("CellularJitter", &TerrainGeneratorGradientConfig::m_cellularJitter)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<TerrainGeneratorGradientConfig>(
                    "FastNoiseLite Gradient", "")
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
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, "NoiseType_OpenSimplex2")
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2S, "NoiseType_OpenSimplex2S")
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_Cellular, "NoiseType_Cellular")
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_Perlin, "NoiseType_Perlin")
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_ValueCubic, "NoiseType_ValueCubic")
                    ->EnumAttribute(FastNoiseLite::NoiseType::NoiseType_Value, "NoiseType_Value")

                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_frequency, "Frequency", "Higher frequencies are more coarse")
                    ->Attribute(AZ::Edit::Attributes::DisplayDecimals, 4)
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0001f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 8.0f)
                    ->Attribute(AZ::Edit::Attributes::SliderCurveMidpoint, 0.25) // Give the frequency a non-linear scale slider with higher precision at the low end
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetFrequencyParameterVisbility)

                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_fractalType, "Fractal Type", "Sets how the fractal is combined")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_None, "FractalType_None")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_FBm, "FractalType_FBm")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_Ridged, "FractalType_Ridged")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_PingPong, "FractalType_PingPong")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_DomainWarpProgressive, "FractalType_DomainWarpProgressive")
                    ->EnumAttribute(FastNoiseLite::FractalType::FractalType_DomainWarpIndependent, "FractalType_DomainWarpIndependent")

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
                    ->EnumAttribute(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Euclidean, "CellularDistanceFunction_Euclidean")
                    ->EnumAttribute(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_EuclideanSq, "CellularDistanceFunction_EuclideanSq")
                    ->EnumAttribute(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Manhattan, "CellularDistanceFunction_Manhattan")
                    ->EnumAttribute(FastNoiseLite::CellularDistanceFunction::CellularDistanceFunction_Hybrid, "CellularDistanceFunction_Hybrid")

                    ->DataElement(AZ::Edit::UIHandlers::ComboBox, &TerrainGeneratorGradientConfig::m_cellularReturnType, "Return Type", "Alters the value type the cellular function returns from its calculation")
                    ->Attribute(AZ::Edit::Attributes::Visibility, &TerrainGeneratorGradientConfig::GetCellularParameterVisibility)
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_CellValue, "CellValue")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance, "CellularReturnType_Distance")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2, "CellularReturnType_Distance2")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2Add, "CellularReturnType_Distance2Add")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2Sub, "CellularReturnType_Distance2Subs")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2Mul, "CellularReturnType_Distance2Mul")
                    ->EnumAttribute(FastNoiseLite::CellularReturnType::CellularReturnType_Distance2Div, "CellularReturnType_Distance2Div")

                    ->DataElement(AZ::Edit::UIHandlers::Slider, &TerrainGeneratorGradientConfig::m_cellularJitter, "Jitter", "Sets the maximum distance a cellular point can move from its grid position")
                    ->Attribute(AZ::Edit::Attributes::Min, 0.f)
                    ->Attribute(AZ::Edit::Attributes::Max, std::numeric_limits<float>::max())
                    ->Attribute(AZ::Edit::Attributes::SoftMax, 10.f)
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
                    [](TerrainGeneratorGradientConfig* config, const int& i) { config->m_noiseType = (FastNoiseLite::NoiseType)i; })
                ->Property("fractalType",
                    [](TerrainGeneratorGradientConfig* config) { return (int&)(config->m_fractalType); },
                    [](TerrainGeneratorGradientConfig* config, const int& i) { config->m_fractalType = (FastNoiseLite::FractalType)i; })
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

    TerrainGeneratorGradientComponent::TerrainGeneratorGradientComponent(const TerrainGeneratorGradientConfig& configuration)
        : m_configuration(configuration)
    {
    }

    void TerrainGeneratorGradientComponent::Activate()
    {
        // This will immediately call OnGradientTransformChanged and initialize m_gradientTransform.
        GradientSignal::GradientTransformNotificationBus::Handler::BusConnect(GetEntityId());

        // Some platforms require random seeds to be > 0.  Clamp to a positive range to ensure we're always safe.
        m_generator.SetSeed(AZ::GetMax(m_configuration.m_seed, 1));
        m_generator.SetFrequency(m_configuration.m_frequency);
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


/*
************************************************************************************************************************************
------------------------------------------------------------------------------------------------------------------------------------
- Fast noise calculations are done through 'GetNoise()'
------------------------------------------------------------------------------------------------------------------------------------
************************************************************************************************************************************
*/
    // Fast noise calculations are done through 'GetNoise()'
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

    // I think this is what the Terrain Gradient list uses to get the new coordinates
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

    template <typename TValueType, TValueType TerrainGeneratorGradientConfig::*TConfigMember, void (FastNoiseLite::*TMethod)(TValueType)>
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
        SetConfigValue<int, &TerrainGeneratorGradientConfig::m_seed, &FastNoiseLite::SetSeed>(AZ::GetMax(seed, 1));
    }

    void TerrainGeneratorGradientComponent::SetFrequency(float freq)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_frequency, &FastNoiseLite::SetFrequency>(freq);
    }

    void TerrainGeneratorGradientComponent::SetNoiseType(FastNoiseLite::NoiseType type)
    {
        SetConfigValue<FastNoiseLite::NoiseType, &TerrainGeneratorGradientConfig::m_noiseType, &FastNoiseLite::SetNoiseType>(type);
    }

    void TerrainGeneratorGradientComponent::SetOctaves(int octaves)
    {
        SetConfigValue<int, &TerrainGeneratorGradientConfig::m_octaves, &FastNoiseLite::SetFractalOctaves>(octaves);
    }

    void TerrainGeneratorGradientComponent::SetLacunarity(float lacunarity)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_lacunarity, &FastNoiseLite::SetFractalLacunarity>(lacunarity);
    }

    void TerrainGeneratorGradientComponent::SetGain(float gain)
    {
        SetConfigValue<float, &TerrainGeneratorGradientConfig::m_gain, &FastNoiseLite::SetFractalGain>(gain);
    }

    void TerrainGeneratorGradientComponent::SetFractalType(FastNoiseLite::FractalType type)
    {
        SetConfigValue<FastNoiseLite::FractalType, &TerrainGeneratorGradientConfig::m_fractalType, &FastNoiseLite::SetFractalType>(type);
    }

    
} // namespace TerrainGenerator
