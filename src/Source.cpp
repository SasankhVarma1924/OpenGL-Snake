#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include<iostream>
#include<vector>
#include<map>
#include<Windows.h>

#include "Shader.h"
#include "Buffer.h"
#include "VertexArrays.h"

glm::vec3 vec(0.0f, 0.0f, 0.0f);
std::vector<int> tail;
int n = 0;
int Key = GLFW_KEY_D;
bool flag = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
float rou(float var);

int main()
{
	glfwInit();

	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);*/
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(900, 900, "hello", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew is not initailized" << std::endl;
	}
	float points[6][3]
	{
		-0.5f, 0.2f,0.0f,    
		-0.5f, 0.1f,0.0f,
		-0.4f,0.1f,0.0f,

		-0.5f, 0.2f,0.0f,
		 -0.4f, 0.2f,0.0f,
		 -0.4f,0.1f,0.0f
	};
	float ballpoints[6][2]
	{
		0.0f,0.2f,
		0.0f,0.1f,
		0.1f,0.1f,

		0.0f,0.2f,
		0.1f,0.2f,
		0.1f,0.1f
	};
	unsigned short indices[]
	{
		0,1,2,
		0,2,3
	};
	std::map<int, float>random =
	{
		{0,0.1},{1,0.2},{2,0.3},{3,0.4},{4,0.5},{5,0.6},{6,0.7},{7,0.8}
	};
	
	glm::mat4 ran(1.0f),trans(1.0f);
	ran = glm::translate(ran, glm::vec3(0.0f, 0.0f, 0.0f));
	


	Shader ball("ball", "res/snake.shader", "#shader ballv", "#shader ballf");
	Shader s2("s2", "res/layoutx.shader", "#shader vert2", "#shader frag2");
	Shader s3("s3", "res/layouty.shader", "#shader vert2", "#shader frag2");
	Shader s4("s4", "res/snake.shader", "#shader snakev", "#shader snakef");
	s4.Bind();

	Buffer b4(GL_ARRAY_BUFFER, sizeof(points), points);
	b4.Bind();
	VertexArrays v4(sizeof(float) * 3);
	v4.DataOrder(0, 3, 0);
	v4.UnBind();
	b4.UnBind();
	s4.UnBind();

	ball.Bind();
	Buffer bball(GL_ARRAY_BUFFER, sizeof(ballpoints), ballpoints);
	bball.Bind();
	VertexArrays vball(sizeof(float) * 2);
	vball.DataOrder(0, 2, 0);
	vball.UnBind();
	bball.UnBind();
	ball.UnBind();


	tail.reserve(30);
	glm::mat4 move(1.0f);
	glm::mat4 moved(1.0f);
	int k = 0;

	srand(time(0));
	int bx = rand() % 7;
	int by = rand() % 6;
	float g = 1.0f,f = -1.0f;
	int score = 0;

	double time=0, deltatime=0, lasttime=0;

	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, callback);
		glfwPollEvents();

		glClearColor(0.19f, 0.16f, 0.18f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	    float x = 0.1f;
		float y = 0.1f;

		s2.Bind();
		for (int i = 0; i < 17; i++)
		{
			glUniform1f(glGetUniformLocation(s2.Get_Id(), "x"), x);
			glDrawArrays(GL_LINES, 0, 2);
			x = x + 0.1f;
        }

		s3.Bind();
		for (int i = 0; i < 17; i++)
		{
			glUniform1f(glGetUniformLocation(s3.Get_Id(), "y"), y);
			glDrawArrays(GL_LINES, 0, 2);
			y = y + 0.1f;
		}

		ball.Bind();
		vball.Bind();
		ran = glm::translate(ran, glm::vec3(g * random[bx],f * random[by], 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(ball.Get_Id(), "ran"), 1, GL_FALSE, glm::value_ptr(ran));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ball.UnBind();
		vball.UnBind();

		s4.Bind();
		v4.Bind();
		key_callback(window, Key, 0, GLFW_PRESS, 0);
		
		move = glm::translate(move, vec);
		glUniform3f(glGetUniformLocation(s4.Get_Id(), "col"), 0.5, 0.5, 0.5);
		glUniformMatrix4fv(glGetUniformLocation(s4.Get_Id(), "move"), 1, GL_FALSE, glm::value_ptr(move));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		k = 0;
		for (int i = 3; i < 6; i++)
		{
			int j = 0;
			glm::vec4 vep(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec4 veb(0.0f, 0.0f, 0.0f, 1.0f);

			vep.x = points[i][j];
			vep.y = points[i][j + 1];
			vep.z = 0.0f;
			veb.x = ballpoints[i][j];
			veb.y = ballpoints[i][j + 1];
			veb.z = 0.0f;
			vep = move * vep;
			veb = ran * veb;
			if ((rou(vep.x)==rou(veb.x))&&( rou(vep.y) == rou(veb.y))&& (rou(vep.z) == rou(veb.z)))
			{
				k++;
			}	
			else
			{
				break;
			}
		}
		move = glm::mat4(1.0f);
		if (k==3)
		{
			bx = rand() % 7;
			by = rand() % 6;
			if (rand() % 8 > 4)
				g = g * -1.0f;
			else
				f = f * -1.0f;
			score++;
			flag = true;
		}
		int size = tail.size() - 1;
		glm::vec3 vecd = vec;
		for (int i = 0; i < n; i++)
		{
			glm::mat4 moved(1.0f);
			if (tail[size] == 1)
			{
				vecd.x -= 0.1f;
			}
			if (tail[size] == 3)
			{
				vecd.x += 0.1f;
			}
			if (tail[size] == 0)
			{
				vecd.y -= 0.1f;
			}
			if (tail[size] == 2)
			{
				vecd.y += 0.1f;
			}
			if ((rou(vec.x) == rou(vecd.x)) && (rou(vec.y) == rou(vecd.y)) && (rou(vec.z) == rou(vecd.z)))
			{
				glfwSetWindowShouldClose(window, true);
			}
			glUniform3f(glGetUniformLocation(s4.Get_Id(), "col"), 0.74f, 0.83f, 0.25f);
			moved = glm::translate(moved, vecd);
			glUniformMatrix4fv(glGetUniformLocation(s4.Get_Id(), "move"), 1, GL_FALSE, glm::value_ptr(moved));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			size--;
		}
		if (flag)
		{
			n++;
		}
		flag = false;
		ran = glm::mat4(1.0f);
		s2.UnBind();
		s3.UnBind();
		s4.UnBind();
		v4.UnBind();
		Sleep(200-5 *score);
		if (rou(vec.x) < -0.3 || rou(vec.x) > 1.3 || rou(vec.y) > 0.7 || rou(vec.y) < -1.0)
		{
			glfwSetWindowShouldClose(window, true);
			break;
		}
		glfwSwapBuffers(window);
	}

	glfwSetWindowShouldClose(window, 1);
	glfwTerminate();

	std::cout << "SCORE :: " << score << std::endl;
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_D || key == GLFW_KEY_A) && action == GLFW_PRESS && (Key == GLFW_KEY_W || Key == GLFW_KEY_S))
		Key = key;
	else if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_PRESS && (Key == GLFW_KEY_D || Key == GLFW_KEY_A))
		Key = key;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		tail.push_back(0);
		vec.y += 0.1f;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		tail.push_back(3);
		vec.x -= 0.1f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		tail.push_back(2);
		vec.y -= 0.1f;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		tail.push_back(1);
		vec.x += 0.1f;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
}

float rou(float var)
{
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}
