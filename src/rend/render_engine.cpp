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

        delete MeshSpace;

        for (int i = 0; i < VertexBuffers.size(); i++)
        {
            MeshSpace->DeleteMesh(i + 1, VertexBuffers[i], UvBuffers[i], NormalBuffers[i], ElementBuffers[i]);
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
            render_frame();
            end = std::chrono::steady_clock::now();
            auto render_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
            if (render_time > target_fps)
                std::cout << "Your PC is too slow, render fps is less than 60fps.\n So render auto shuts down\n";
            else
                std::this_thread::sleep_for(std::chrono::nanoseconds(render_time));
            begin = std::chrono::steady_clock::now();
        }
        stop();
    }

    bool GetInitialize()
    {
        return Is_Initialized_;
    }

    void create_object(std::string name, float x, float y, float z, float ax, float ay, float az, float aw)
    {
        access_object_.lock();
        int index = NamesId[name];

        render_object *new_obj = new render_object;

        new_obj->setPosition(glm::vec3(x, y, z));
        new_obj->setOrientation(glm::quat(ax, ay, az, aw));
        new_obj->update_model(index, IndexBuffer[index]);

        new_obj->update_camera(ViewCamera);
        glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        new_obj->update_matrix(ScaleMatrix, ModelMatrixID);

        RendObjs.push_back(new_obj);
        access_object_.unlock();
    }
    void create_light(std::string name, float x, float y, float z, float r, float g, float b, float power)
    {
        access_light_.lock();

        light *new_light = new light;

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
            std::cout << "Error: index out of range\n";
        }
        access_light_.unlock();
    }
    void update_object(int index, float x, float y, float z, float ax, float ay, float az, float aw)
    {
        access_object_.lock();
        if (index < RendObjs.size())
        {
            RendObjs[index]->setPosition(glm::vec3(x, y, z));
            RendObjs[index]->setOrientation(glm::quat(ax, ay, az, aw));
            glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            RendObjs[index]->update_matrix(ScaleMatrix, ModelMatrixID);
        }
        else
            std::cout << "NO such object exicts!";
        access_object_.unlock();
    }

    void update_camera(float x, float y, float z, float ax, float ay, float az, float aw)
    {
        access_camera_.lock();
        ViewCamera->set_position(glm::vec3(x, y, z));
        ViewCamera->set_angel(glm::vec3(ax, ay, az));
        ViewCamera->set_rotation(glm::vec3(aw, 0, 0));
        access_camera_.unlock();
    }

private:
    void render_frame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update_matrixes();

        glUseProgram(programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);

        glm::vec3 *LightPositions = new glm::vec3[Lights.size()];
        glm::vec3 *LightColors = new glm::vec3[Lights.size()];
        GLfloat *LightPowers = new GLfloat[Lights.size()];

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

        access_object_.lock();
        std::cout << "All right";
        for (int i = 0; i < RendObjs.size(); ++i)
        {
            int index = RendObjs[i]->getID();
            MeshSpace->switch_render_mesh(VertexBuffers[index], UvBuffers[index], NormalBuffers[index],
                                          ElementBuffers[index], vertexPosition_modelspaceID, vertexUVID,
                                          vertexNormal_modelspaceID);
            RendObjs[i]->draw_object();
        }
        access_object_.unlock();

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        glDisableVertexAttribArray(vertexNormal_modelspaceID);

        glfwSwapBuffers(window);
        glfwPollEvents();
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

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        programID = LoadShaders("res/StandardShadingVert.GLSL", "res/StandardShading.GLSL");

        ViewCamera->AutoSetID(programID);
        ModelMatrixID = glGetUniformLocation(programID, "M");

        vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
        vertexUVID = glGetAttribLocation(programID, "vertexUV");
        vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

        Texture = loadDDS("res/uvmap.DDS");
        TextureID = glGetUniformLocation(programID, "myTextureSampler");

        Load_All();
        MeshSpace->LoadAllCustomMesh(ElementBuffers, NormalBuffers, UvBuffers, VertexBuffers, IndexBuffer, IdMeshes);

        glUseProgram(programID);
        LightPositsID = glGetUniformLocation(programID, "LightPosits");
        LightColorsID = glGetUniformLocation(programID, "LightColors");
        LightPowersID = glGetUniformLocation(programID, "LightPowers");
        LightCountID = glGetUniformLocation(programID, "LightCount");

        ViewCamera->set_position(glm::vec3(0, 0, 7));
        ViewCamera->set_angel(glm::vec3(0, 0, 0));
        ViewCamera->set_rotation(glm::vec3(0, 1, 0));
        Is_Initialized_ = true;
        RenderTaskisTerminated_ = false;
    }
    void Load_All()
    {
        std::string FatherFile = "res/render_objects.txt";
        int number_of_objects = 0;
        number_of_objects = read_obj_list(FatherFile, IdNames, IdMeshes, IdTextures);

        for (auto it = IdNames.begin(); it != IdNames.end(); ++it)
        {
            NamesId[it->second] = it->first;
        }
    }
    void update_matrixes()
    {
        ViewCamera->update_matrixes();
        ViewCamera->PassMatrixesToShader();
    }

private:
    GLFWwindow *window;

    GLuint programID;

    GLuint LightPositsID;
    GLuint LightPowersID;
    GLuint LightColorsID;
    GLuint LightCountID;

    GLuint ModelMatrixID;

    GLuint vertexPosition_modelspaceID;
    GLuint vertexUVID;
    GLuint vertexNormal_modelspaceID;

    GLuint Texture;
    GLuint TextureID;

    std::vector<GLuint> ElementBuffers;
    std::vector<GLuint> NormalBuffers;
    std::vector<GLuint> UvBuffers;
    std::vector<GLuint> VertexBuffers;
    std::vector<GLuint> IndexBuffer;

    std::map<int, std::string> IdMeshes;
    std::map<int, std::string> IdTextures;
    std::map<int, std::string> IdNames;
    std::map<std::string, int> NamesId;

    bool Is_Initialized_ = false;

    bool RenderTaskisRunning_ = false;
    bool RenderTaskisTerminated_ = true;

    std::mutex access_object_;
    std::mutex access_light_;
    // add mutex for camera
    std::mutex access_camera_;
    std::thread RenderTask_;

    MeshControl *MeshSpace = new MeshControl;
    camera *ViewCamera = new camera;
    std::vector<render_object *> RendObjs;
    std::vector<light *> Lights;
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
