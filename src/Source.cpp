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

glm::vec3 head(0.0f, 0.0f, 0.0f);

// stores the snake tail / body values
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


	float snakePoints[6][3]
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

	Shader layoutx("layoutx", "res/layoutx.shader", "#shader layoutx", "#shader frag2");
	Shader layouty("layouty", "res/layouty.shader", "#shader layouty", "#shader frag2");

	// Everythig related to generation of snake
	Shader snake("snake", "res/snake.shader", "#shader snakev", "#shader snakef");
	snake.Bind();
	Buffer snakeBuffer(GL_ARRAY_BUFFER, sizeof(snakePoints), snakePoints);
	snakeBuffer.Bind();
	VertexArrays snakeVA(sizeof(float) * 3);
	snakeVA.DataOrder(0, 3, 0);
	snakeVA.UnBind();
	snakeBuffer.UnBind();
	snake.UnBind();

	// Everything related to generation of food or ball
	Shader ball("ball", "res/snake.shader", "#shader ballv", "#shader ballf");
	ball.Bind();
	Buffer ballBuffer(GL_ARRAY_BUFFER, sizeof(ballpoints), ballpoints);
	ballBuffer.Bind();
	VertexArrays ballVA(sizeof(float) * 2);
	ballVA.DataOrder(0, 2, 0);
	ballVA.UnBind();
	ballBuffer.UnBind();
	ball.UnBind();

	tail.reserve(30);

	// used to move the snake
	glm::mat4 move(1.0f);
	glm::mat4 moved(1.0f);
	int k = 0;

	// to generater random coordinated for the food or ball
	srand(time(0));
	int bx = rand() % 7;
	int by = rand() % 6;
	float g = 1.0f,f = -1.0f;
	int score = 0;


	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, callback);
		glfwPollEvents();

		glClearColor(0.19f, 0.16f, 0.18f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	    float x = 0.1f;
		float y = 0.1f;

		// display the horizontal lines
		layoutx.Bind();
		for (int i = 0; i < 17; i++)
		{
			glUniform1f(glGetUniformLocation(layoutx.Get_Id(), "x"), x);
			glDrawArrays(GL_LINES, 0, 2);
			x = x + 0.1f;
        }

		// display the vertical lines
		layouty.Bind();
		for (int i = 0; i < 17; i++)
		{
			glUniform1f(glGetUniformLocation(layouty.Get_Id(), "y"), y);
			glDrawArrays(GL_LINES, 0, 2);
			y = y + 0.1f;
		}

		// display the food or ball
		ball.Bind();
		ballVA.Bind();
		ran = glm::translate(ran, glm::vec3(g * random[bx],f * random[by], 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(ball.Get_Id(), "ran"), 1, GL_FALSE, glm::value_ptr(ran));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ball.UnBind();
		ballVA.UnBind();

		// display and move the snake in certain direction
		snake.Bind();
		snakeVA.Bind();
		key_callback(window, Key, 0, GLFW_PRESS, 0);
		
		// displaying only the head of the snake with different color
		move = glm::translate(move, head);
		glUniform3f(glGetUniformLocation(snake.Get_Id(), "col"), 0.5, 0.5, 0.5);
		glUniformMatrix4fv(glGetUniformLocation(snake.Get_Id(), "move"), 1, GL_FALSE, glm::value_ptr(move));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// checking for the colision of food with the head of the snake 
		k = 0;
		for (int i = 3; i < 6; i++)
		{
			int j = 0;
			glm::vec4 vep(0.0f, 0.0f, 0.0f, 1.0f);
			glm::vec4 veb(0.0f, 0.0f, 0.0f, 1.0f);

			vep.x = snakePoints[i][j];
			vep.y = snakePoints[i][j + 1];
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

		// if collision occurs generate a random coordinate for the food or ball
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

		// displaying the body of the snake
		int size = tail.size() - 1;
		glm::vec3 vecd = head;
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
			if ((rou(head.x) == rou(vecd.x)) && (rou(head.y) == rou(vecd.y)) && (rou(head.z) == rou(vecd.z)))
			{
				glfwSetWindowShouldClose(window, true);
			}
			glUniform3f(glGetUniformLocation(snake.Get_Id(), "col"), 0.74f, 0.83f, 0.25f);
			moved = glm::translate(moved, vecd);
			glUniformMatrix4fv(glGetUniformLocation(snake.Get_Id(), "move"), 1, GL_FALSE, glm::value_ptr(moved));
			glDrawArrays(GL_TRIANGLES, 0, 6);
			size--;
		}

		// if collison occurs increase the body of the snake
		if (flag)
		{
			n++;
		}

		flag = false;
		ran = glm::mat4(1.0f);
		layoutx.UnBind();
		layouty.UnBind();
		snake.UnBind();
		snakeVA.UnBind();
		Sleep(200-5 *score);

		// checking if the snake head is out of the screen or grid
		if (rou(head.x) < -0.3f || rou(head.x) >= 1.3f || rou(head.y) > 0.6f || rou(head.y) < -0.9f)
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

// not letting the user to give wrong input
void callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_D || key == GLFW_KEY_A) && action == GLFW_PRESS && (Key == GLFW_KEY_W || Key == GLFW_KEY_S))
		Key = key;
	else if ((key == GLFW_KEY_W || key == GLFW_KEY_S) && action == GLFW_PRESS && (Key == GLFW_KEY_D || Key == GLFW_KEY_A))
		Key = key;
}

// taking the input and changine the position of the head
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		tail.push_back(0);
		head.y += 0.1f;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		tail.push_back(3);
		head.x -= 0.1f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		tail.push_back(2);
		head.y -= 0.1f;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		tail.push_back(1);
		head.x += 0.1f;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
}

// rounding the up the float upto 2 decimals
float rou(float var)
{
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}
