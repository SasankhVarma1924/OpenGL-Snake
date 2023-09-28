#pragma once
#include<vector>
#include<GL/glew.h>
class VertexArrays
{
private:
	unsigned int v_Id;
	unsigned int v_DataSize;
public:
	VertexArrays(unsigned int datasize)
	{
		glCreateVertexArrays(1, &v_Id);
		Bind();
		v_DataSize = datasize;
	}
	void DataOrder(unsigned int location,unsigned int number,unsigned int stride)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, number, GL_FLOAT, GL_FALSE, v_DataSize, (const void*) (stride * sizeof(float)));
	}
	void Bind()
	{
		glBindVertexArray(v_Id);
	}
	void UnBind()
	{
		glBindVertexArray(0);
	}
	~VertexArrays()
	{
		glDeleteVertexArrays(1, &v_Id);
	}
};
