using namespace glm;
class camera
{
private:
    vec3 Position = glm::vec3(0, 0, 7);
    vec3 Angel = glm::vec3(0, 0, 0);
    vec3 Rotation = glm::vec3(0, 1, 0);
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    int MatrixID;
    int ViewMatrixID;

public:
    camera() = default;
    camera(vec3 pos, vec3 angel, vec3 rotation)
    {
        Position = pos;
        Angel = angel;
        Rotation = rotation;
    }
    ~camera() = default;

    void SetID(int matrixID, int viewMatrixID)
    {
        MatrixID = matrixID;
        ViewMatrixID = viewMatrixID;
    }
    void AutoSetID(int programID)
    {
        MatrixID = glGetUniformLocation(programID, "MVP");
        ViewMatrixID = glGetUniformLocation(programID, "V");
    }
    void set_position(vec3 pos)
    {
        Position = pos;
    }
    void set_angel(vec3 angel)
    {
        Angel = angel;
    }
    void set_rotation(vec3 rotation)
    {
        Rotation = rotation;
    }
    vec3 get_position()
    {
        return Position;
    }
    vec3 get_angel()
    {
        return Angel;
    }
    vec3 get_rotation()
    {
        return Rotation;
    }
    void update_matrixes()
    {
        ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        ViewMatrix = glm::lookAt(Position, Angel, Rotation);
    }
    glm::mat4 get_view_matrix()
    {
        return ViewMatrix;
    }
    glm::mat4 get_projection_matrix()
    {
        return ProjectionMatrix;
    }
    int get_matrix_id()
    {
        return MatrixID;
    }
    int get_view_matrix_id()
    {
        return ViewMatrixID;
    }
    void PassMatrixesToShader()
    {
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    }
};