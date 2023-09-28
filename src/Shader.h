#pragma once
#include<string>
#include<GL/glew.h>
#include<fstream>
class Shader
{
private:
	unsigned int s_ProgramId;
	std::string s_Name;
	std::string s_Get_Shader(std::string& path,std::string& name)
	{
		int flag = 0;
		std::ifstream file(path.c_str());
		std::string src;
		std::string s;
		while (!file.eof())
		{
			std::getline(file, s);

			if (s.find(name) != std::string::npos)
			{
				while (getline(file, s))
				{
					if (s.find("shader") != std::string::npos)
					{
						break;
					}
					else
					{
						src += s + '\n';

					}
				}
				break;
			}
		}
		return src;
	}

public:

	Shader(std::string name,std::string path, std::string vertex, std::string fragment)
	{
		char info[512];
		int sucess;

		s_Name = name;

		std::string vertexshader = s_Get_Shader(path, vertex);
		std::string fragmentshader = s_Get_Shader(path, fragment);


		const char* vsrc = vertexshader.c_str();
		const char* fsrc = fragmentshader.c_str();

		unsigned int createvshader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int createfshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(createvshader, 1, &vsrc, NULL);
		glShaderSource(createfshader, 1, &fsrc, NULL);
		glCompileShader(createvshader);
		glCompileShader(createfshader);

		glGetShaderiv(createvshader, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetShaderInfoLog(createvshader, 512, NULL, info);
			std::cout << "ShaderClass::coud not compile vertex shader" << std::endl;
			std::cout << info << std::endl;
		}

		glGetShaderiv(createfshader, GL_COMPILE_STATUS, &sucess);
		if (!sucess)
		{
			glGetShaderInfoLog(createfshader, 512, NULL, info);
			std::cout << "ShaderClass::coud not compile vertex shader" << std::endl;
			std::cout << info << std::endl;
		}

		s_ProgramId = glCreateProgram();
		glAttachShader(s_ProgramId, createvshader);
		glAttachShader(s_ProgramId, createfshader);
		glLinkProgram(s_ProgramId);

		glGetProgramiv(s_ProgramId, GL_LINK_STATUS, &sucess);
		if (!sucess)
		{
			glGetProgramInfoLog(s_ProgramId, 512, NULL, info);
			std::cout << "ShaderClass::coud not compile vertex shader" << std::endl;
			std::cout << info << std::endl;
		}

		glDeleteShader(createvshader);
		glDeleteShader(createfshader);

	}

	void Bind()
	{
		glUseProgram(s_ProgramId);
	}

	unsigned int Get_Id()
	{
        return s_ProgramId;
	}

	void UnBind()
	{
		glUseProgram(0);
	}

	//void Print()
	//{
	//	std::cout << VertexShader << '\n' << FragmentShader << std::endl;
	//}

	~Shader()
	{
		glDeleteProgram(s_ProgramId);
		std::cout << "ShaderClass:: "<< s_Name<< " program is deleted" << std::endl;
	}
};