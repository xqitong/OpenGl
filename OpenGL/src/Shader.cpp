#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "Render.h"


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(m_FilePath);
    std::cout << "VERTEX:\n" << source.VertexSource << "\n"
        << "FRAGMENT:\n" << source.FragmentSource << "\n";
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
 
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}
//
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();// make sure source string is alive.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    //TODO:: Error handling

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile"
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;

    }

    return id;
}
int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int Shader::GetUniformLocation(const std::string& name)
{
    auto it = m_UniformLocationCache.find(name);
    if (it == m_UniformLocationCache.end())
    {
        GLCall(int loc = glGetUniformLocation(m_RendererID, name.c_str()));
        if (loc == -1) {
            std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
        }
        m_UniformLocationCache[name]= loc;
        return loc;
    }
    return it->second;
}
