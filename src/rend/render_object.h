using namespace glm;

class render_object
{
public:
    render_object() = default;
    ~render_object() = default;

    void update_model(int index, GLuint indexsize)
    {
        model_index = index;
        index_size = indexsize;
    }
    void update_camera(camera *cam)
    {
        camera_ = cam;
    }
    void update_matrix(glm::mat4 ScalingMatrixV, GLuint ModelMatrixIDV)
    {
        ScalingMatrix = ScalingMatrixV;
        ModelMatrixID = ModelMatrixIDV;
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
        glm::mat4 MVP = camera_->get_projection_matrix() * camera_->get_view_matrix() * ModelMatrix;

        glUniformMatrix4fv(camera_->get_matrix_id(), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        // glUniformMatrix4fv(camera_, 1, GL_FALSE, &ViewMatrix[0][0]);

        glDrawElements(GL_TRIANGLES, 2904, GL_UNSIGNED_SHORT, (void *)(0));
    }

protected:
    camera *camera_;
    vec3 position;
    quat orientation;
    int model_index;
    GLuint index_size;
    glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

    int ModelMatrixID;
};

class light
{
private:
    vec3 Position = glm::vec3(0, 0, 0);
    vec3 Color = glm::vec3(1, 1, 1);
    float Power = 1.0f;
    vec3 Direction = glm::vec3(0, 0, 0);

public:
    light() = default;
    light(vec3 pos, vec3 color, float power, vec3 direction)
    {
        Position = pos;
        Color = color;
        Power = power;
        Direction = direction;
    }
    ~light() = default;
    // get light position
    vec3 get_position()
    {
        return Position;
    }
    // get light color
    vec3 get_color()
    {
        return Color;
    }
    // get light power
    float get_power()
    {
        return Power;
    }
    void set_position(vec3 pos)
    {
        Position = pos;
    }
    void set_position(float x, float y, float z)
    {
        Position = vec3(x, y, z);
    }
    void set_color(vec3 color)
    {
        Color = color;
    }
    void set_color(float r, float g, float b)
    {
        Color = vec3(r, g, b);
    }
    void set_power(float power)
    {
        Power = power;
    }
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
