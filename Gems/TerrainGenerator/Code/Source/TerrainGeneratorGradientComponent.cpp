
#include <TerrainGeneratorGradientComponent.h>

#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>

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


    //AZ_COMPONENT_IMPL(TerrainGeneratorGradientComponent, "TerrainGeneratorGradientComponent", "{862772FD-B012-4F54-923F-B05960A88B1B}");

    void TerrainGeneratorGradientComponent::Activate()
    {
        TerrainGeneratorGradientRequestBus::Handler::BusConnect(GetEntityId());
    }

    void TerrainGeneratorGradientComponent::Deactivate()
    {
        TerrainGeneratorGradientRequestBus::Handler::BusDisconnect(GetEntityId());
    }

    void TerrainGeneratorGradientComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TerrainGeneratorGradientComponent, AZ::Component>()
                ->Version(1)
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
            behaviorContext->Class<TerrainGeneratorGradientComponent>("TerrainGeneratorGradient Component Group")
                ->Attribute(AZ::Script::Attributes::Category, "TerrainGenerator Gem Group")
                ;
        }
    }

    void TerrainGeneratorGradientComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TerrainGeneratorGradientComponentService"));
    }

    void TerrainGeneratorGradientComponent::GetIncompatibleServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
    }

    void TerrainGeneratorGradientComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void TerrainGeneratorGradientComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }
} // namespace TerrainGenerator
