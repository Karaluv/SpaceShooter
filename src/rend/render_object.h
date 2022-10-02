// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>
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
// Include AntTweakBar
#include <AntTweakBar.h>
// Include copied libraries
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp>

using namespace glm;

class render_object
{
public:
    render_object() {}
    ~render_object()
    {
        delete &position;
        delete &orientation;
        delete &model_index;
        delete &index_size;
        delete &ProjectionMatrix;
        delete &ViewMatrix;
        delete &ScalingMatrix;
        delete &MatrixID;
        delete &ModelMatrixID;
        delete &ViewMatrixID;
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
    void get_position(double *x, double *y, double *z)
    {
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }
    vec3 get_position()
    {
        return position;
    }
    void get_orientation(double *x, double *y, double *z, double *w)
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

    void draw_object()
    {
        glm::mat4 RotationMatrix = eulerAngleYXZ(orientation.x, orientation.y, orientation.z);
        glm::mat4 TranslationMatrix = translate(mat4(), position); // A bit to the left
        glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glDrawElements(GL_TRIANGLES, 2904, GL_UNSIGNED_SHORT, (void *)(0));
    }

protected:
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

class RenderVoxel : render_object
{
public:
private:
};

class RenderCockpit : render_object
{
public:
private:
};

class RenderCustomObj : render_object
{
public:
private:
};