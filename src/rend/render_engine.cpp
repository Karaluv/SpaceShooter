// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

// Include AntTweakBar
#include <AntTweakBar.h>


#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp> // See quaternion_utils.cpp for RotationBetweenVectors, LookAt and RotateTowards

#include <map>


void initializeR()
{
	std::cout << "hui";
}

GLuint load_mesh(std::string name, int index_of_model, GLuint* elementbuffer, GLuint* normalbuffer, GLuint* uvbuffer, GLuint* vertexbuffer)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	bool res = loadOBJ(name.c_str(), vertices, uvs, normals);

	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO


	glGenBuffers(index_of_model, vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);


	glGenBuffers(index_of_model, uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);


	glGenBuffers(index_of_model, normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, *normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well

	glGenBuffers(index_of_model, elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


	int indices_size = indices.size();

	vertices.clear();
	uvs.clear();
	normals.clear();

	indices.clear();
	indexed_vertices.clear();
	indexed_uvs.clear();
	indexed_normals.size();

	return GLuint(indices_size);



}

void switch_render_mesh(GLuint VertexBuffer, GLuint UvBuffer, GLuint NormalBuffer, GLuint ElementsBuffer,
	GLuint vertexPosition_modelspaceID, GLuint vertexUVID, GLuint vertexNormal_modelspaceID)
{
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID,  // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
	glVertexAttribPointer(
		vertexUVID,                   // The attribute we want to configure
		2,                            //   size : U+V => 2
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	glEnableVertexAttribArray(vertexNormal_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(
		vertexNormal_modelspaceID,    // The attribute we want to configure
		3,                            // size
		GL_FLOAT,                     // type
		GL_FALSE,                     // normalized?
		0,                            // stride
		(void*)0                      // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementsBuffer);
}

class render_engine
{
public:
	render_engine() {}
	void initialize()
	{
		glfwInit();

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		window = glfwCreateWindow(1024, 768, "render-test", NULL, NULL);

		glfwMakeContextCurrent(window);

		glewInit();

		glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
		glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
		glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
		glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
		glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwPollEvents();
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);

		programID = LoadShaders("res/StandardShading.vertexshader", "res/StandardShading.fragmentshader");

		MatrixID = glGetUniformLocation(programID, "MVP");
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelMatrixID = glGetUniformLocation(programID, "M");

		vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
		vertexUVID = glGetAttribLocation(programID, "vertexUV");
		vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

		Texture = loadDDS("res/uvmap.DDS");
		TextureID = glGetUniformLocation(programID, "myTextureSampler");

		LoadAllMesh(ElementBuffers, NormalBuffers, UvBuffers, VertexBuffers, IndexBuffer);

		glUseProgram(programID);
		LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

		new_object("monkey", 0.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0);
		new_object("cube", 2.0, -1.0, -2.0, 0.0, 0.0, 0.0, 0.0);

		camera_position = glm::vec3(0, 0, 7);
		camera_angel = glm::vec3(0, 0, 0);
		camera_rotation = glm::vec3(0, 1, 0);
		
	}
	void new_object(std::string name, double x, double y, double z, double ax, double ay, double az, double aw)
	{
		int index = NamesDict[name];

		auto new_obj = new render_object;

		new_obj->set_position(x, y, z);
		new_obj->set_orientation(ax, ay, az, aw);
		new_obj->update_model(index, IndexBuffer[index]);

		RendObjs.push_back(new_obj);

	}
	void render_frame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update_matrixes();

		glUseProgram(programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);
		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		double x, y, z, w;
		int index;
		for (int i = 0; i < RendObjs.size(); ++i)
		{
			index = RendObjs[i]->get_index();
			switch_render_mesh(VertexBuffers[index], UvBuffers[index], NormalBuffers[index], ElementBuffers[index], vertexPosition_modelspaceID, vertexUVID, vertexNormal_modelspaceID);

			RendObjs[i]->get_orientation(&x, &y, &z, &w);
			RendObjs[i]->set_orientation(x, y, z + 3.14159f / 2.0f * 0.001, w);
			RendObjs[i]->update_matrix(ProjectionMatrix, ViewMatrix, scale(mat4(), vec3(1.0f, 1.0f, 1.0f)), MatrixID, ModelMatrixID, ViewMatrixID);
			RendObjs[i]->draw_object();
		}

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	void terminate_all()
	{
		for (int i = 0; i < VertexBuffers.size(); i++)
		{
			delete_mesh(i + 1, VertexBuffers[i], UvBuffers[i], NormalBuffers[i], ElementBuffers[i]);
		}
		glDeleteTextures(1, &TextureID);
		TwTerminate();
		glfwTerminate();
	}

	void delete_obj(int index)
	{
		delete &RendObjs[index];
		RendObjs.erase(RendObjs.begin() + index);
	}


private:

	void delete_mesh(int index, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer)
	{
		glDeleteBuffers(index, &vertexbuffer);
		glDeleteBuffers(index, &uvbuffer);
		glDeleteBuffers(index, &normalbuffer);
		glDeleteBuffers(index, &elementbuffer);
		glDeleteProgram(programID);
	}
	void update_matrixes()
	{
		ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		ViewMatrix = glm::lookAt(camera_position, camera_angel, camera_rotation);
	}
	void LoadAllMesh(std::vector<GLuint>& ElementBuff, std::vector<GLuint>& NormalBuff, std::vector<GLuint>& UvBuff, std::vector<GLuint>& VertexBuff, std::vector<GLuint>& IndexBuff)
	{
		for (int i = 0; i < PathDict.size(); ++i)
		{
			ElementBuff.push_back(GLuint(0));
			NormalBuff.push_back(GLuint(0));
			UvBuff.push_back(GLuint(0));
			VertexBuff.push_back(GLuint(0));
			IndexBuff.push_back(GLuint(0));
			IndexBuff.back() = load_mesh(PathDict[i], i + 1, &ElementBuff.back(), &NormalBuff.back(), &UvBuff.back(), &VertexBuff.back());
		}
	}

	GLFWwindow* window;

	bool gLookAtOther = true;

	GLuint programID;

	GLuint LightID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	GLuint vertexPosition_modelspaceID;
	GLuint vertexUVID;
	GLuint vertexNormal_modelspaceID;

	GLuint Texture;
	GLuint TextureID;

	vec3 camera_position;
	vec3 camera_angel;
	vec3 camera_rotation;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	std::vector<GLuint> ElementBuffers;
	std::vector<GLuint> NormalBuffers;
	std::vector<GLuint> UvBuffers;
	std::vector<GLuint> VertexBuffers;
	std::vector<GLuint> IndexBuffer;


	std::map<int, std::string> PathDict = { {0,"res/scube.obj"},{1,"res/suzanne.obj"} };
	std::map<std::string, int>  NamesDict = { {"cube",0},{"monkey",1} };

	class render_object
	{
	public:
		render_object() {}
		~render_object() 
		{
			delete &position;
			delete &orientation;
			delete &model_index;
			delete& index_size;
			delete &ProjectionMatrix;
			delete& ViewMatrix;
			delete& ScalingMatrix;
			delete& MatrixID;
			delete& ModelMatrixID;
			delete& ViewMatrixID;
		}

		void update_model(int index, GLuint indexsize)
		{
			model_index = index;
			index_size = indexsize;
		}

		void update_matrix(glm::mat4 ProjectionMatrixV, glm::mat4 ViewMatrixV, glm::mat4 ScalingMatrixV,
			GLuint MatrixIDV, GLuint ModelMatrixIDV, GLuint ViewMatrixIDV)
		{
			ProjectionMatrix = ProjectionMatrixV;
			ViewMatrix = ViewMatrixV;
			ScalingMatrix = ScalingMatrixV;

			MatrixID = MatrixIDV;
			ModelMatrixID = ModelMatrixIDV;
			ViewMatrixID = ViewMatrixIDV;
		}
		void get_position(double* x, double* y, double* z)
		{
			*x = position.x;
			*y = position.y;
			*z = position.z;
		}
		vec3 get_position()
		{
			return position;
		}
		void get_orientation(double* x, double* y, double* z, double* w)
		{
			*x = orientation.x;
			*y = orientation.y;
			*z = orientation.z;
			*w = orientation.w;
		}
		void set_position(vec3 new_position)
		{
			position = new_position;
		}
		void set_position(double x, double y, double z)
		{
			position.x = x;
			position.y = y;
			position.z = z;
		}
		void set_position()
		{
			position = vec3(0.0, 0.0, 0.0);
		}
		void set_orientation()
		{
			orientation = quat(0.0, 0.0, 0.0, 0.0);
		}
		void set_orientation(quat new_orientation)
		{
			orientation = new_orientation;
		}
		void set_orientation(double x, double y, double z, double w)
		{
			orientation.x = x;
			orientation.y = y;
			orientation.z = z;
			orientation.w = w;
		}
		int get_index()
		{
			return model_index;
		}

	public:
		void draw_object()
		{
			glm::mat4 RotationMatrix = eulerAngleYXZ(orientation.x, orientation.y, orientation.z);
			glm::mat4 TranslationMatrix = translate(mat4(), position); // A bit to the left
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

			glDrawElements(GL_TRIANGLES, 2904, GL_UNSIGNED_SHORT, (void*)(0));
		}

	private:
		vec3 position;
		quat orientation;
		int model_index;
		GLuint index_size;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

		GLuint MatrixID;
		GLuint ModelMatrixID;
		GLuint ViewMatrixID;
	};

	std::vector<render_object*> RendObjs;
};

void run(void)
{
	render_engine RendEng;
	RendEng.initialize();

	do {
		RendEng.render_frame();
	} 
	while (true);

	RendEng.terminate_all();
}

