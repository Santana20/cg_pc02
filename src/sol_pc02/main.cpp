#include <iostream>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <queue>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderClass.h>
#include <vao.h>
#include <vbo.h>
#include <ebo.h>
#include <texture.h>
#include <camera.h>

#include <figures.h>
#include <maze.h>

float manhattan_distance(glm::vec3 pos1, glm::vec3 pos2)
{
	float result = abs(abs(pos1.x) - abs(pos2.x)) + abs(abs(pos1.y) - abs(pos2.y)) + abs(abs(pos1.z) - abs(pos2.z));
	return result;
}


bool is_valid(int n, int nx, int ny)
{
	int range = (2 * n) + 1;
	if (nx >= 0 && nx < range && ny >= 0 && ny < range) 
		return true;
	
	return false;
}

std::vector<std::pair<int, int>> bfs(int n, std::vector<std::vector<int>> & maze)
{
	int dx[] = {0, 1, 0, -1};
	int dy[] = {-1, 0, 1, 0};

	std::queue<std::pair<int, int>> q;
	std::vector<std::vector<int>> previousStep((2 * n + 1), std::vector<int>((2 * n + 1), -1));

	std::pair<int, int> begin = std::make_pair(1, 0);
	std::pair<int, int> end = std::make_pair(n*2-1, n*2);

	q.push(begin); previousStep[begin.first][begin.second] = 4;

	while (!q.empty()) {
		std::pair<int, int> u = q.front(); q.pop();

		for (int i = 0; i < 4; i++) {
			std::pair<int, int> v = std::make_pair(u.first + dx[i], u.second + dy[i]);

			if (!is_valid(n, v.first, v.second)) continue;
			if (maze[v.first][v.second] == 0) continue;
			if (previousStep[v.first][v.second] != -1) continue;

			previousStep[v.first][v.second] = i;
			q.push(v);
		}
	}

	// Reconstruir path
	std::vector<std::pair<int, int>> path;
	while (end != begin) 
	{
		int p = previousStep[end.first][end.second];
		path.push_back(end);

		end = std::make_pair(end.first - dx[p], end.second - dy[p]);
	}

	reverse(path.begin(), path.end());

	return path;
}



void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

int width = 1000, height = 1000;

int main()
{
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	GLFWwindow* window = glfwCreateWindow(width, height, "Minecraftt", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);

	// To change the viewport when change the size of windows
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// MAZE
	// Generate maze
	int n_maze = 20;
	std::vector<std::vector<int>> maze = mkMaze(n_maze);

	// Generate vector of positions for drawn cubes, based in mkMaze.
	float shapeCube = 1.0f;
	std::vector<glm::vec3> positions;

	bool noPosPlayer = false;
	glm::vec3 playerPos = glm::vec3(0.0f);
	for (int x = 0; x < int(maze.size()); ++x) {
		for (int z = 0; z < int(maze.size()); ++z) {

			std::cout << (maze[x][z] == 1? "  " : (maze[x][z] == 3? "==" : "██"));
			if (maze[x][z] == 0)
				positions.push_back(glm::vec3(z * shapeCube, 0.0f, x * shapeCube));
			if (!noPosPlayer && maze[x][z] == 3)
			{
				noPosPlayer = true;
				playerPos = glm::vec3(z * shapeCube, 0.0f, x * shapeCube);
			}
		}
		std::cout << '\n';
	}

	// Apply BFS to find the solution of maze
	std::vector<std::pair<int, int>> path = bfs(n_maze, maze);



	// Instance a cube
	Cube* cube = new Cube(glm::vec3(shapeCube));

	// Shader for cube
	Shader shader("shader.vert", "shader.frag");
	// Generates Vertex Array Object and binds it
	VAO vao;
	vao.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO vbo(cube->getVertices(), cube->getVSize() * sizeof(float));
	// Generates Element Buffer Object and links it to indices
	EBO ebo(cube->getIndices(), cube->getISize() * sizeof(int));
	// Links VBO attributes such as coordinates, colors and textures to VAO

	// positions
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, cube->len(), cube->skip(0));
	// colors
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, cube->len(), cube->skip(3));
	// tetures
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, cube->len(), cube->skip(9));
	// normals
	vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, cube->len(), cube->skip(6));

	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();


	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(cube->getVertices(), cube->getVSize() * sizeof(float));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(cube->getIndices(), cube->getISize() * sizeof(int));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, cube->len(), cube->skip(0));
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	// proj, view and model of lightCube
	glm::vec4 lightColor_dark = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	glm::vec4 lightColor_red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 lightColor = lightColor_red;

	glm::vec3 lightPos = glm::vec3(playerPos.x, playerPos.y + 3.0f, playerPos.z);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);



	// proj, view and model of CUBE
	glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	Model = glm::translate(Model, Pos);



	// change uniform for lightshader program
	lightShader.Activate();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);



	// change uniform for shader program
	shader.Activate();

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	// TEXTURES
	// Stone Texture for cube
	Texture horrorTexture("horror_walls.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	horrorTexture.texUnit(shader, "tex0", 0);



	// camera
	Camera camera(width, height, playerPos);


	// Variables for animation light Cube
	float speed = 0.025f;
	int idx_path = 0;
	bool reach_x=false, reach_z =false;


	// Variables to change colors of light cube, when we reach the end of maze.
	float prev_time = float(glfwGetTime());
	float angle = 0.0f;

	glEnable(GL_DEPTH_TEST);
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Change color and move light Cube
		if (int(manhattan_distance(camera.getPosition(), lightPos) > int(8 * shapeCube)))
		{
			lightColor = lightColor_dark;
		}
		else
		{
			if (idx_path < int(path.size()))
			{
				glm::vec3 current_target = glm::vec3(path[idx_path].second * shapeCube, lightPos.y, path[idx_path].first * shapeCube);

				// for x
				if (!reach_x)
				{
					float diff_x = (lightPos.x - current_target.x);
					if ((diff_x < 0.0f) && (abs(diff_x) >= speed)) lightPos.x += speed;
					else if ((diff_x > 0.0f) && (abs(diff_x) >= speed)) lightPos.x -= speed;
					else reach_x = true;
				}
				// for z
				if (!reach_z)
				{
					float diff_z = (lightPos.z - current_target.z);
					if ((diff_z < 0.0f) && (abs(diff_z) > speed)) lightPos.z += speed;
					else if ((diff_z > 0.0f) && (abs(diff_z) > speed)) lightPos.z -= speed;
					else reach_z = true;
				}

				if (reach_x && reach_z)
				{
					reach_x = false, reach_z = false;
					++idx_path;
				}
				lightColor = lightColor_red;
			}
			else // reach the end of maze.
			{
				float time = float(glfwGetTime());
				if ((time - prev_time) >= 0.25f)
				{
					angle += 0.1f;
					prev_time = time;
					lightColor = glm::vec4(float(sin(angle)), float(cos(angle)), float(sin(angle)), 1.0f);
				}
			}
		}


		// background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		// Manage inputs for camera
		camera.Inputs(window);
		
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shader.Activate();

		glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		camera.Matrix(shader, "camMatrix");

		vao.Bind();

		// Draw all cubes
		for (int i = 0; i < int(positions.size()); ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);

			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

			horrorTexture.Bind();

			glDrawElements(GL_TRIANGLES, cube->getISize(), GL_UNSIGNED_INT, 0);
		}
		

		// LIGHT CUBE
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

		// Drawn light Cube
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, cube->getISize(), GL_UNSIGNED_INT, 0);

		// Swap the window buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deletes

	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shader.Delete();
	horrorTexture.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
    delete cube;

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}