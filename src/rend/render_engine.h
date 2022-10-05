#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <render_camera.h>
#include <render_object.h>
#include <render_mesh.h>

using namespace glm;

// point light source
class render_engine
{
public:
    render_engine() {}
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
        initialized = false;
    }

    void render_process()
    {

        const int target_fps = (1000000000 / 144);
        RenderTaskisRunning_ = true;

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
        return initialized;
    }

    void new_object(std::string name, double x, double y, double z, double ax, double ay, double az, double aw)
    {
        access_object.lock();
        int index = NamesDict[name];

        render_object *new_obj = new render_object;

        new_obj->set_position(x, y, z);
        new_obj->set_orientation(ax, ay, az, aw);
        new_obj->update_model(index, IndexBuffer[index]);

        new_obj->update_camera(ViewCamera);
        glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        new_obj->update_matrix(ScaleMatrix, ModelMatrixID);

        RendObjs.push_back(new_obj);
        access_object.unlock();
    }

    void new_light(std::string name, double x, double y, double z, double r, double g, double b, double power)
    {
        access_light.lock();

        light *new_light = new light;

        new_light->set_position(x, y, z);
        new_light->set_color(r, g, b);
        new_light->set_power(power);

        Lights.push_back(new_light);
        access_light.unlock();
    }
    void delete_light(int index)
    {
        access_light.lock();
        Lights.erase(Lights.begin() + index);
        access_light.unlock();
    }
    void delete_obj(int index)
    {
        access_object.lock();
        RendObjs.erase(RendObjs.begin() + index);
        access_object.unlock();
    }
    void update_light(int index, double x, double y, double z, double r, double g, double b, double power)
    {
        access_light.lock();
        // check if index is valid
        if (index < Lights.size())
        {
            Lights[index]->set_position(x, y, z);
            Lights[index]->set_color(r, g, b);
            Lights[index]->set_power(power);
        }
        // print error message
        else
        {
            std::cout << "Error: index out of range\n";
        }
        access_light.unlock();
    }
    void update_obj(int index, double x, double y, double z, double ax, double ay, double az, double aw)
    {
        access_object.lock();
        if (index < RendObjs.size())
        {
            RendObjs[index]->set_orientation(ax, ay, az, aw);
            RendObjs[index]->set_position(x, y, z);
            glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            RendObjs[index]->update_matrix(ScaleMatrix, ModelMatrixID);
        }
        else
            std::cout << "NO such object exicts!";
        access_object.unlock();
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

        // create arrays of Lights
        glm::vec3 *LightPositions = new glm::vec3[Lights.size()];
        glm::vec3 *LightColors = new glm::vec3[Lights.size()];
        GLfloat *LightPowers = new GLfloat[Lights.size()];
        access_light.lock();
        for (int i = 0; i < Lights.size(); i++)
        {
            LightPositions[i] = Lights[i]->get_position();
            LightColors[i] = Lights[i]->get_color();
            LightPowers[i] = Lights[i]->get_power();
        }

        glUniform1i(LightCountID, Lights.size());
        // pass to shader all lights positions
        glUniform3fv(LightPositsID, 3, glm::value_ptr(LightPositions[0]));
        // pass to shader all lights colors
        glUniform3fv(LightColorsID, 3, glm::value_ptr(LightColors[0]));
        // pass to shader all lights power
        glUniform1fv(LightPowersID, 3, LightPowers);
        access_light.unlock();

        access_object.lock();
        for (int i = 0; i < RendObjs.size(); ++i)
        {
            int index = RendObjs[i]->get_index();
            MeshSpace->switch_render_mesh(VertexBuffers[index], UvBuffers[index], NormalBuffers[index],
                                          ElementBuffers[index], vertexPosition_modelspaceID, vertexUVID,
                                          vertexNormal_modelspaceID);
            RendObjs[i]->draw_object();
        }
        access_object.unlock();

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

        window = glfwCreateWindow(1024, 768, "render-test", NULL, NULL);

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

        MeshSpace->LoadAllCustomMesh(ElementBuffers, NormalBuffers, UvBuffers, VertexBuffers, IndexBuffer);

        glUseProgram(programID);
        LightPositsID = glGetUniformLocation(programID, "LightPosits");
        LightColorsID = glGetUniformLocation(programID, "LightColors");
        LightPowersID = glGetUniformLocation(programID, "LightPowers");
        LightCountID = glGetUniformLocation(programID, "LightCount");

        ViewCamera->set_position(glm::vec3(0, 0, 7));
        ViewCamera->set_angel(glm::vec3(0, 0, 0));
        ViewCamera->set_rotation(glm::vec3(0, 1, 0));
        initialized = true;
        RenderTaskisTerminated_ = false;
    }

    void update_matrixes()
    {
        ViewCamera->update_matrixes();
        ViewCamera->PassMatrixesToShader();
    }

private:
    GLFWwindow *window;

    bool gLookAtOther = true;
    bool initialized = false;

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

    std::map<int, std::string> PathDict = {{0, "res/scube.obj"}, {1, "res/suzanne.obj"}};
    std::map<std::string, int> NamesDict = {{"cube", 0}, {"monkey", 1}};

    bool RenderTaskisRunning_ = false;
    bool RenderTaskisTerminated_ = true;

    std::mutex access_object;
    std::mutex access_light;
    std::thread RenderTask_;

    MeshControl *MeshSpace = new MeshControl;
    camera *ViewCamera = new camera;
    std::vector<render_object *> RendObjs;
    std::vector<light *> Lights;
};
