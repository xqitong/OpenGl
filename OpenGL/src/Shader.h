#pragma once
#include <string>
#include <unordered_map>
enum class ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string,int> m_UniformLocationCache ;
    //caching for uniforms
public:
    Shader(const std::string& filepath);
    ~Shader();
    void Bind() const;
    void Unbind() const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);

                                      
 private:
     ShaderProgramSource ParseShader(const std::string& filepath);
     unsigned int CompileShader(unsigned int type, const std::string& source);
     int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
     int GetUniformLocation(const std::string& name);

};

