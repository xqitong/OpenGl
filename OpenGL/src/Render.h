#pragma once
#include <gl/glew.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"


#define ASSERT(x) if (!(x)) __debugbreak();
#define  GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
class Render
{ 
public:
    void Draw(const VertexArray& va,const IndexBuffer& ib, const Shader& shader ) const;
    void Clear() const;
};

