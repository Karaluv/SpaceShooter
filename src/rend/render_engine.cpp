// Include standard headers
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
// Include AntTweakBar
#include <AntTweakBar.h>
// Include copied libraries
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp>

#include <objloader.cpp>
#include <load.hpp>
#include <render_camera.hpp>
#include <render_object.hpp>
#include <cub_map.hpp>
#include <render_mesh.hpp>

class render_engine
{
	// pattern Singletone
private:
	render_engine();
	static render_engine* instance;


public:

	static render_engine* GetInstance();



	~render_engine()
	{
		stop();
	}

	void start()
	{
		this->RenderTask_ = std::thread(&render_engine::render_process, this);
	}

	void stop()
	{
		this->RenderTaskisRunning_ = false;
		RenderTask_.join();


		for (int i = 0; i < VertexBuffers.size(); i++)
		{
			MeshSpace.DeleteMesh(i + 1, VertexBuffers[i], UvBuffers[i], NormalBuffers[i], ElementBuffers[i]);
		}
		glDeleteTextures(1, &TextureID);
		TwTerminate();
		glfwTerminate();
		Is_Initialized_ = false;
	}

	void render_process()
	{

		const int target_fps = (1000000000 / 144); // 144 fps
		RenderTaskisRunning_ = true;               // set this to false to stop the render process

		initialize();

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		while (RenderTaskisRunning_)
		{
			// keys
			update_input();
			render_frame();
			end = std::chrono::steady_clock::now();
			auto render_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
			//if (render_time > target_fps)
				//std::cout << "Your PC is too slow, render fps is less than 60fps.\n So render auto shuts down\n";
			//else
				std::this_thread::sleep_for(std::chrono::nanoseconds(render_time));
			begin = std::chrono::steady_clock::now();
		}
		stop();
	}

	unsigned get_number_of_objects()
	{
		return RendObjs.size();
	}

	bool GetInitialize()
	{
		return Is_Initialized_;
	}

	template <typename T>
	void add_object(int Type,int texture_index,float x, float y, float z, float ax, float ay, float az, float aw)
	{
		T* new_obj = new T;

		new_obj->setCamera(&ViewCamera);

		new_obj->setPosition(glm::vec3(x, y, z));
		new_obj->setOrientation(glm::vec3(ax, ay, az));

		new_obj->setScale(ModelScale[Type]);
		new_obj->setScaleUniformId(ScaleId);

		new_obj->setType(Type);

		new_obj->setNumVertices(IndexBuffer[Type]);
		new_obj->setModelMatrixUniformId(ModelMatrixID);


		new_obj->setTextureId(texture_index);
		new_obj->setTextureUniformId(TextureID);


		new_obj->setLightningTypeUniformId( TypeOfLightningId);
		auto* new_rendarable = new Renderable;
		new_rendarable->setStrategy(new_obj);
		
		RendObjs.push_back(new_rendarable);
	}

	void create_object(std::string name, float x, float y, float z, float ax, float ay, float az, float aw)
	{

		// check if stringin NamesId
		if (NamesId.count(name) !=0)
		{
		}
		else
		{
			// throw exception
			//std::cout << "Error: object with name " << name << " not found\n";
			throw std::exception("Error: object with name not found\n");
			return;
		}
		access_object_.lock();
		
		int Type = NamesId[name];


		std::string mesh_name = NamesMesh[Type];
		std::string texture_name = NamesTexture[Type];
			
		int mesh_index = MeshesId[mesh_name];
		int texture_index = TexturesId[texture_name];
		GLuint Gltexture_index = TexturesGl[texture_index];
		
		
		// conditional
		// new_obj has type of Skybox if name skybox
		// new_obj has type of ConstMeshObject if name not skybox
		
		// conditional
		
		if (name == "skybox")
			add_object<Skybox>(Type,texture_index,x,  y,z, ax, ay, az, aw);
		else
			add_object<ConstMeshObject>(Type,texture_index,x, y, z, ax, ay, az, aw);
		access_object_.unlock();
			
	}
	void create_light(std::string name, float x, float y, float z, float r, float g, float b, float power)
	{
		access_light_.lock();

		light* new_light = new light;

		new_light->set_position(glm::vec3(x, y, z));
		new_light->set_color(glm::vec3(r, g, b));
		new_light->set_power(power);

		Lights.push_back(new_light);
		access_light_.unlock();
	}
	void delete_light(int index)
	{
		access_light_.lock();
		Lights.erase(Lights.begin() + index);
		access_light_.unlock();
	}
	void delete_object(int index)
	{
		access_object_.lock();
		RendObjs.erase(RendObjs.begin() + index);
		access_object_.unlock();
	}
	void update_light(int index, float x, float y, float z, float r, float g, float b, float power)
	{
		access_light_.lock();
		// check if index is valid
		if (index < Lights.size())
		{
			Lights[index]->set_position(glm::vec3(x, y, z));
			Lights[index]->set_color(glm::vec3(r, g, b));
			Lights[index]->set_power(power);
		}
		// print error message
		else
		{
		
		// trow "Error: index out of range\n";
		access_light_.unlock();
			throw std::exception("Error: index out of range");
			return;
		 
		}
	}
	void update_object(int index, float x, float y, float z, float ax, float ay, float az, float aw)
	{
		access_object_.lock();
		if (index < RendObjs.size())
		{
			RendObjs[index]->setPosition(glm::vec3(x, y, z));
			RendObjs[index]->setOrientation(glm::vec3(ax, ay, az));
			//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			//RendObjs[index]->update_matrix(ModelMatrixID);
		access_object_.unlock();
		}
		else
		{
		access_object_.unlock();
			// trow "Error: index out of range\n";
		throw std::exception("Error: index out of range");
		return;
		}
			//std::cout << "NO such object exicts!";
	}

	void update_camera(float x, float y, float z, float mouse_x, float mouse_y,float roll)
	{
		access_camera_.lock();
		ViewCamera.set_position(glm::vec3(x, y, z));
		ViewCamera.set_camera_angle(mouse_x, mouse_y);
		//ViewCamera->set_orientation(glm::vec3(ax, ay, az));
		//ViewCamera->set_rotation(glm::vec3(rx, ry, rz));
		// roll
		ViewCamera.set_roll(roll); 
		access_camera_.unlock();
	}

	// get inputs map
	std::map<char, bool> get_inputs()
	{
		return inputs;
	}

	// get mouse move in pair
	std::pair<double, double> get_mouse()
	{
		// return cords in pair
		return std::make_pair(mouse_x, mouse_y);
	}

private:

	

	void render_frame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update_matrixes();

		glUseProgram(programID);

		glm::vec3* LightPositions = new glm::vec3[Lights.size()];
		glm::vec3* LightColors = new glm::vec3[Lights.size()];
		GLfloat* LightPowers = new GLfloat[Lights.size()];

		access_light_.lock();
		for (int i = 0; i < Lights.size(); i++)
		{
			LightPositions[i] = Lights[i]->get_position();
			LightColors[i] = Lights[i]->get_color();
			LightPowers[i] = Lights[i]->get_power();
		}

		glUniform1i(LightCountID, GLint(Lights.size()));
		// pass to shader all lights positions
		glUniform3fv(LightPositsID, 3, glm::value_ptr(LightPositions[0]));
		// pass to shader all lights colors
		glUniform3fv(LightColorsID, 3, glm::value_ptr(LightColors[0]));
		// pass to shader all lights power
		glUniform1fv(LightPowersID, 3, LightPowers);
		access_light_.unlock();

/*
		glBegin(GL_QUADS);
		glColor3d(1, 0, 0);
		glVertex3f(-1, -1, -10);
		glColor3d(1, 1, 0);
		glVertex3f(1, -1, -10);
		glColor3d(1, 1, 1);
		glVertex3f(1, 1, -10);
		glColor3d(0, 1, 1);
		glVertex3f(-1, 1, -10);
		glEnd();
*/
			
		access_object_.lock();
		//std::cout << "All right";
		for (int i = 0; i < RendObjs.size(); ++i)
		{
			
			RendObjs[i]->update();
			
			//if (i == 0)
			//	glUniform1i(TypeOfLightningId, 0);
			//else
			//	glUniform1i(TypeOfLightningId, 1);

			int index = RendObjs[i]->getType();

			// get scale 
			std::string mesh_name = NamesMesh[index];
			std::string texture_name = NamesTexture[index];
			
			int mesh_index = MeshesId[mesh_name];
			int texture_index = TexturesId[texture_name];
			GLuint Gltexture_index = TexturesGl[texture_index];

			glActiveTexture(GL_TEXTURE0);
		
			glBindTexture(GL_TEXTURE_2D, Gltexture_index);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 100.0f);
			glUniform1i(TextureID, 0);

			// scale texture
			
			
			
			MeshSpace.switch_render_mesh(VertexBuffers[mesh_index], UvBuffers[mesh_index], NormalBuffers[mesh_index],
				ElementBuffers[mesh_index], vertexPosition_modelspaceID, vertexUVID,
				vertexNormal_modelspaceID);
			
			

			
			
			RendObjs[i]->render();
		}
		access_object_.unlock();

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		
		
		glFlush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	void Load_All_textures()
	{
		// you have dictionary with all pathes
		
		for (auto it = TexturesId.begin(); it != TexturesId.end(); ++it)
		{
			int index = it->second;
			std::string path = PathTextures[index];
			
			try {
				GLuint texture = loadDDS(("res/textures/" + path).c_str());
			TexturesGl[index] = texture;
			}
			catch(...)
			{
				std::cout << "Error: can't load texture " << path << std::endl;
			}
		}
	}

	void initialize()
	{

		glfwInit();

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		window = glfwCreateWindow(1024, 768, "SpaceShooter", NULL, NULL);

		glfwMakeContextCurrent(window);

		glewInit();

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		

		//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		// black
		glClearColor(65.0f/255, 60.0f/255, 81.0f/255,0.0f);
		

		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);

		programID = LoadShaders("res/StandardShadingVert.GLSL", "res/StandardShading.GLSL");

		ViewCamera.AutoSetID(programID);
		ModelMatrixID = glGetUniformLocation(programID, "M");

		vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
		vertexUVID = glGetAttribLocation(programID, "vertexUV");
		vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

		Texture = loadDDS("res/textures/MyTest.DDS");
		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		
		Load_All();
		Load_All_textures();

		
			
		
		MeshSpace.LoadAllCustomMesh(ElementBuffers, NormalBuffers, UvBuffers, VertexBuffers, IndexBuffer, PathMeshes);
		

		glUseProgram(programID);
		LightPositsID = glGetUniformLocation(programID, "LightPosits");
		LightColorsID = glGetUniformLocation(programID, "LightColors");
		LightPowersID = glGetUniformLocation(programID, "LightPowers");
		LightCountID = glGetUniformLocation(programID, "LightCount");
		ScaleId = glGetUniformLocation(programID, "scale");
		TypeOfLightningId = glGetUniformLocation(programID, "type_light");

		glUniform1i(TypeOfLightningId, 1);

		

		ViewCamera.set_position(glm::vec3(0, 0, 1));
		ViewCamera.set_angel(glm::vec3(0, 0, 0));
		ViewCamera.set_rotation(glm::vec3(0, 1, 0));
		Is_Initialized_ = true;
		RenderTaskisTerminated_ = false;
	}

	void Load_All()
	{
		std::string path = "res/obj/objs.txt";
		int number_of_objects = 0;
		try {
			number_of_objects = read_list_of_n(path, IdMeshes, PathMeshes);
		}
		catch (std::exception e)
		{
			std::cout << e.what();
		}
			
		for (auto it = IdMeshes.begin(); it != IdMeshes.end(); ++it)
		{
			MeshesId[it->second] = it->first;
		}
		// for textures
		std::string path_textures = "res/textures/textures.txt";
		int number_of_textures = 0;
		try{
		number_of_textures = read_list_of_n(path_textures, IdTextures, PathTextures);
		}
		catch (std::exception e)
		{
			std::cout << e.what();
		}
		for (auto it = IdTextures.begin(); it != IdTextures.end(); ++it)
		{
			TexturesId[it->second] = it->first;
		}
		// for names
		std::string path_names = "res/types.txt";
		int number_of_names = 0;
		std::map<int, std::string> temp_scale;
		try {
			number_of_names = read_list_of_n(path_names, IdNames, NamesMesh, NamesTexture, temp_scale);
		}
		catch (std::exception e)
		{
			std::cout << e.what();
		}
		for (auto it = IdNames.begin(); it != IdNames.end(); ++it)
		{
			NamesId[it->second] = it->first;
		}
		// for scale
		for (auto it = IdNames.begin(); it != IdNames.end(); ++it)
		{
			ModelScale[it->first] = stof(temp_scale[it->first]);
		}
		
	}
	void update_matrixes()
	{
		ViewCamera.update_matrixes();
		ViewCamera.PassMatrixesToShader();
	}
	void update_input()
	{
		inputs_.lock();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		// write w,a,s,d, e,q,r,t to a special dictionary, and mouse move
		
		inputs['w'] = false;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			inputs['w'] = true;
		
		inputs['a'] = false;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			inputs['a'] = true;
		
		inputs['s'] = false;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			inputs['s'] = true;

		inputs['d'] = false;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			inputs['d'] = true;

		inputs['e'] = false;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			inputs['e'] = true;

		inputs['q'] = false;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			inputs['q'] = true;

		inputs['r'] = false;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			inputs['r'] = true;

		inputs['t'] = false;
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			inputs['t'] = true;
		
		// space and ctrl
		inputs[' '] = false;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			inputs[' '] = true;

		inputs['c'] = false;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			inputs['c'] = true;

		// mouse move
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		
		inputs_.unlock();
		
	}
	
	

private:
	GLFWwindow* window;

	GLuint programID;

	GLuint LightPositsID;
	GLuint LightPowersID;
	GLuint LightColorsID;
	GLuint LightCountID;
	
	GLuint TypeOfLightningId;

	GLuint ModelMatrixID;

	GLuint vertexPosition_modelspaceID;
	GLuint vertexUVID;
	GLuint vertexNormal_modelspaceID;

	GLuint Texture;
	GLuint TextureID;
	
	GLuint CubeMapTexture;
	GLuint CubeMapTextureID;
	
	GLuint ScaleId;

	std::vector<GLuint> ElementBuffers;
	std::vector<GLuint> NormalBuffers;
	std::vector<GLuint> UvBuffers;
	std::vector<GLuint> VertexBuffers;
	std::vector<GLuint> IndexBuffer;

	std::map<int, std::string> IdMeshes;
	std::map<int, std::string> PathMeshes;
	std::map<std::string, int> MeshesId;
	
	
	std::map<int, std::string> IdTextures;
	std::map<int, std::string> PathTextures;
	std::map<std::string, int> TexturesId;
	std::map<int, GLuint> TexturesGl;
	std::map<int, GLuint> TexturesGlId;
	
	
	std::map<int, std::string> IdNames;
	std::map<std::string, int> NamesId;
	std::map<int, std::string> NamesMesh;
	std::map<int, std::string> NamesTexture;

	std::map<int, float> ModelScale;
	

	bool Is_Initialized_ = false;

	bool RenderTaskisRunning_ = false;
	bool RenderTaskisTerminated_ = true;

	std::mutex access_object_;
	std::mutex access_light_;
	// add mutex for camera
	std::mutex access_camera_;
	std::thread RenderTask_;

	std::mutex inputs_;

	MeshControl MeshSpace;
	camera ViewCamera;

	//camera ConstMeshObject::s_camera = ViewCamera;
	
	std::vector<Renderable*> RendObjs;
	std::vector<light*> Lights;
	
	// create dictionary for char and bool
	// with w a s d e q r t m n c s x y
	std::map<char, bool> inputs{
		{'w', false}, {'a', false}, {'s', false},
		{'d', false}, {'e', false}, {'q', false},
		{'r', false}, {'t', false}, {'m', false},
		{'n', false}, {'c', false}, {'s', false},
		{' ', false} };

	double mouse_x, mouse_y;


	

	
};

// singletone
render_engine* render_engine::instance = nullptr;
render_engine* render_engine::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new render_engine();
	}
	return instance;
}

render_engine::render_engine()
{}
