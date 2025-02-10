#pragma once
#include "Test.h"
#include "glm/glm.hpp"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Texture.h"
#include <memory>

namespace test{
    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();
       ~TestTexture2D();
       virtual void OnUpdate(float deltaTime) override ;
       virtual void OnRender() override ;
       virtual void OnImGuiRender() override ;
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr < Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        glm::mat4 m_View,m_Proj;
        glm::vec3 m_TranslationA;//(200.0f, 200.0f, 0.0f)
        glm::vec3 m_TranslationB; //(400.0f, 200.0f, 0.0f)
    };
}

