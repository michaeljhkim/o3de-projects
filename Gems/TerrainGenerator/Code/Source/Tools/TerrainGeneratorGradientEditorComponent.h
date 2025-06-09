/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GradientSignal/Editor/EditorGradientComponentBase.h>
#include <Clients/TerrainGeneratorGradientComponent.h>

namespace TerrainGenerator
{
    class TerrainGeneratorGradientEditorComponent
        : public GradientSignal::EditorGradientComponentBase<TerrainGeneratorGradientComponent, TerrainGeneratorGradientConfig>
    {
    public:
        using BaseClassType = GradientSignal::EditorGradientComponentBase<TerrainGeneratorGradientComponent, TerrainGeneratorGradientConfig>;
        AZ_EDITOR_COMPONENT(TerrainGeneratorGradientEditorComponent, "{b1b7b33c-5ee2-4ea7-b34e-99205c32e5db}", BaseClassType);
        static void Reflect(AZ::ReflectContext* context);

        AZ::u32 ConfigurationChanged() override;

        static constexpr const char* const s_categoryName = "Gradients";
        static constexpr const char* const s_componentName = "TerrainGenerator Gradient";
        static constexpr const char* const s_componentDescription = "Generates gradient values using FastNoise a noise generation library with a collection of realtime noise algorithms";
        static constexpr const char* const s_icon = "Editor/Icons/Components/Gradient.svg";
        static constexpr const char* const s_viewportIcon = "Editor/Icons/Components/Viewport/Gradient.svg";
        static constexpr const char* const s_helpUrl = "";

    private:
        AZ::Crc32 OnGenerateRandomSeed();
    };
}