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
#include "Shader.h"



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

         Shader shader("res/shaders/Basic.shader");
         shader.Bind();
        shader.SetUniform4f("u_Color",0.8f,0.3f,0.8f,1.0f);
        float r = 0.0f;
        float increment = 0.05f;    

        shader.Unbind();
        va.Unbind();
        vb.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);
            /* Render here */                          

            float timeValue = glfwGetTime();
            float r = sin(timeValue) / 2.0f + 0.5f;
            shader.Bind();
            //GLCall(glUniform4f(loc_u_Color,r, 1.0f - r, 0.0f, 0.0f));
            shader.SetUniform4f("u_Color", r, 1.0f - r, 0.0f, 0.0f);
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
    
    }
    glfwTerminate();
    return 0;
}