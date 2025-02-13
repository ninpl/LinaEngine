/*
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Rendering/Shader.hpp"

#include "Core/RenderEngineBackend.hpp"
#include "Log/Log.hpp"
#include "Utility/UtilityFunctions.hpp"

namespace Lina::Graphics
{
    Shader::~Shader()
    {
        m_engineBoundID = m_renderDevice->ReleaseShaderProgram(m_engineBoundID);
    }

    void* Shader::LoadFromMemory(const std::string& path, unsigned char* data, size_t dataSize)
    {
        LINA_TRACE("[Shader Loader - Memory] -> Loading: {0}", path);
        Resources::IResource::SetSID(path);
        std::string shaderText = std::string(reinterpret_cast<char*>(data), dataSize);
        Graphics::LoadTextWithIncludes(shaderText, "#include");
        const bool usesGeometryShader = shaderText.find("GS_BUILD") != std::string::npos;
        this->Construct(shaderText, usesGeometryShader);
        return static_cast<void*>(this);
    }

    void* Shader::LoadFromFile(const std::string& path)
    {
        LINA_TRACE("[Shader Loader - File] -> Loading: {0}", path);
        Resources::IResource::SetSID(path);
        std::string shaderText;
        Graphics::LoadTextFileWithIncludes(shaderText, path, "#include");
        const bool usesGeometryShader = shaderText.find("GS_BUILD") != std::string::npos;
        this->Construct(shaderText, usesGeometryShader);
        return static_cast<void*>(this);
    }

    Shader& Shader::Construct(const std::string& text, bool usesGeometryShader)
    {
        m_renderDevice  = RenderEngineBackend::Get()->GetRenderDevice();
        m_engineBoundID = m_renderDevice->CreateShaderProgram(text, &m_uniformData, usesGeometryShader);
        return *this;
    }

    void Shader::SetUniformBuffer(const std::string& name, UniformBuffer& buffer)
    {
        m_renderDevice->SetShaderUniformBuffer(m_engineBoundID, name, buffer.GetID());
    }

    void Shader::BindBlockToBuffer(uint32 bindingPoint, std::string blockName)
    {
        m_renderDevice->BindShaderBlockToBufferPoint(m_engineBoundID, bindingPoint, blockName);
    }

} // namespace Lina::Graphics
