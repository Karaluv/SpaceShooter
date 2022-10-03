
// Include render object
#include <render_object.h>
#include <render_mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

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

        RendObjs.push_back(new_obj);
        access_object.unlock();
    }
    void delete_obj(int index)
    {
        access_object.lock();
        delete &RendObjs[index];
        RendObjs.erase(RendObjs.begin() + index);
        access_object.unlock();
    }

    void update_obj(int index, double x, double y, double z, double ax, double ay, double az, double aw)
    {
        access_object.lock();
        if (index < RendObjs.size())
        {
            RendObjs[index]->set_orientation(ax, ay, az, aw);
            RendObjs[index]->set_position(x, y, z);
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

        // glm::vec3 lightPos = glm::vec3(4, 4, 4);
        //  glUniform3f(3, lightPos.x, lightPos.y, lightPos.z);
        //  glUniform3f(3, 0, 0, 0);
        // glUniform3f(LightID, 4, 4, 4);
        glm::vec3 pointLightsPosition[] = {
            glm::vec3(-10, -20, -10),
            glm::vec3(10, -10, 10),
            glm::vec3(4, 4, 4)};
        glm::vec3 pointLightsColor[] = {
            glm::vec3(1, 1, 1),
            glm::vec3(0.5, 0.5, 0.5),
            glm::vec3(0, 0, 0)};
        GLfloat pointLightsPower[3] = {50, 50, 50};
        int pointLightsCount = 3;

        glUniform3fv(LightsColorID, 3,
                     glm::value_ptr(pointLightsColor[0]));

        glUniform3fv(LightsPositionID, 3,
                     glm::value_ptr(pointLightsPosition[0]));

        glUniform1fv(LightsPowerID, 3, pointLightsPower);
        // glUniform3fv(LightsColorsID, 3,
        //              glm::value_ptr(pointLightsColors[0]));
        //  glUniform3fv(LightsCountID, pointLightsCount);
        // glUniform1d(LightsCountID, pointLightsCount);

        glUniform3f(3, 2.0, 2.0, 2.0);
        //   glUniform3f(5, -2.0, 2.0, 2.0);

        int index;
        access_object.lock();
        for (int i = 0; i < RendObjs.size(); ++i)
        {
            index = RendObjs[i]->get_index();
            MeshSpace->switch_render_mesh(VertexBuffers[index], UvBuffers[index], NormalBuffers[index],
                                          ElementBuffers[index], vertexPosition_modelspaceID, vertexUVID,
                                          vertexNormal_modelspaceID);
            RendObjs[i]->update_matrix(ProjectionMatrix, ViewMatrix, scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
                                       MatrixID, ModelMatrixID, ViewMatrixID);
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

        // glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); r
        // glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);
        // glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);
        // glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);
        // glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);

        // glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        ////glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glfwPollEvents();
        //  glfwSetCursorPos(window, 1024 / 2, 768 / 2);

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glEnable(GL_DEPTH_TEST);

        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        programID = LoadShaders("res/StandardShadingVert.GLSL", "res/StandardShading.GLSL");

        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
        ModelMatrixID = glGetUniformLocation(programID, "M");

        vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
        vertexUVID = glGetAttribLocation(programID, "vertexUV");
        vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

        Texture = loadDDS("res/uvmap.DDS");
        TextureID = glGetUniformLocation(programID, "myTextureSampler");

        MeshSpace->LoadAllCustomMesh(ElementBuffers, NormalBuffers, UvBuffers, VertexBuffers, IndexBuffer);

        glUseProgram(programID);

        LightsPositionID = glGetUniformLocation(programID, "LightsPosition");
        LightsColorID = glGetUniformLocation(programID, "LightsColor");
        LightsPowerID = glGetUniformLocation(programID, "LightsPower");
        LightsCountID = glGetUniformLocation(programID, "LightsCount");

        ViewCam.Position = glm::vec3(0, 0, 7);
        ViewCam.Angel = glm::vec3(0, 0, 0);
        ViewCam.Rotation = glm::vec3(0, 1, 0);
        initialized = true;
        RenderTaskisTerminated_ = false;
    }

    void update_matrixes()
    {
        ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        ViewMatrix = glm::lookAt(ViewCam.Position, ViewCam.Angel, ViewCam.Rotation);
    }

private:
    GLFWwindow *window;

    bool gLookAtOther = true;
    bool initialized = false;

    GLuint programID;

    GLuint LightsPositionID;
    GLuint LightsPowerID;
    GLuint LightsColorID;
    GLuint LightsCountID;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    GLuint vertexPosition_modelspaceID;
    GLuint vertexUVID;
    GLuint vertexNormal_modelspaceID;

    GLuint Texture;
    GLuint TextureID;

    glm::mat4 ProjectionMatrix;
    glm::mat4 ViewMatrix;

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
    std::thread RenderTask_;

    struct camera
    {
        vec3 Position = glm::vec3(0, 0, 7);
        vec3 Angel = glm::vec3(0, 0, 0);
        vec3 Rotation = glm::vec3(0, 1, 0);
    };

    camera ViewCam;

    MeshControl *MeshSpace = new MeshControl;

    std::vector<render_object *> RendObjs;
};
