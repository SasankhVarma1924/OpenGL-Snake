#pragma once

#include<GL/glew.h>

class Buffer
{
private:

	unsigned int b_Id;
	GLenum b_Target;

public:

	Buffer(GLenum target,unsigned int size,const void * add)
	{
		b_Target = target;
		glGenBuffers(1, &b_Id);
		Bind();
		glBufferData(b_Target, size, add , GL_STATIC_DRAW);
	}
	void Bind()
	{
		glBindBuffer(b_Target,b_Id);
	}
	void UnBind()
	{
		glBindBuffer(b_Target, 0);
	}

};
