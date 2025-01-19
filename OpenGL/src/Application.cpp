#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Render.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream,line))
    {
        if (line.find("#shader")!=std::string::npos)
        {
            if (line.find("vertex")!=std::string::npos)
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
     return {ss[0].str(),ss[1].str()};
}
//
static unsigned int CompileShader( unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char*src = source.c_str();// make sure source string is alive.
    glShaderSource(id,1,&src,nullptr);
    glCompileShader(id);
    //TODO:: Error handling

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message =(char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" 
        << (type ==GL_VERTEX_SHADER ? "vertex" : "fragment")
        <<" shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;

    }

    return id;
}
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
   unsigned int program =  glCreateProgram();
   unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
   unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

   glAttachShader(program, vs);
   glAttachShader(program, fs);
   glLinkProgram(program);       
   glValidateProgram(program);

   glDeleteShader(vs);
   glDeleteShader(fs);

   return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // now have a valid OpenGL rendering context
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    } 
    std::cout << glGetString(GL_VERSION) <<std::endl;
    //
    {
        float positions[] = {
            -0.5f, -0.5f,
            0.5f,  -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,

        };
        unsigned int indices[]=
        {
            0,1,2,
            2,3,0
        };


        VertexArray va;

        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        unsigned int ibo;
        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        std::cout <<"VERTEX:\n" << source.VertexSource << "\n"
            << "FRAGMENT:\n" << source.FragmentSource << "\n";
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    
        glUseProgram(shader);
        //glUniform
        GLCall( int loc_u_Color = glGetUniformLocation(shader, "u_Color"));
        ASSERT(loc_u_Color != -1)   // -1 means not found
        float r = 0.0f;
        float increment = 0.05f;    

        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            /* Render here */                          
            glUseProgram(shader);

            float timeValue = glfwGetTime();
            float r = sin(timeValue) / 2.0f + 0.5f;
            GLCall(glUniform4f(loc_u_Color,r, 1.0f - r, 0.0f, 0.0f));
            va.Bind();
            ib.Bind();
            GLfloat attrib_offset[] = { (float)sin(timeValue) * 0.5f, (float)cos(timeValue) * 0.5f, 0.0f, 0.0f };
            glVertexAttrib4fv(1, attrib_offset);

            GLCall(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr));


   
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    
        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}