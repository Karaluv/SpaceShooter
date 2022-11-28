class camera
{
private:
    glm::vec3 Position = glm::vec3(0, 0, 0);
    glm::vec3 Angel = glm::vec3(0, 0, 0);
    glm::vec3 Rotation = glm::vec3(0, 0, 0);
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    int MatrixID;
    int ViewMatrixID;

public:
    camera()
    {
		Position = glm::vec3(0, 0, 0);
		Angel = glm::vec3(0, 0, 0);
		Rotation = glm::vec3(0, 0, 0);
		ViewMatrix = glm::mat4(1.0f);
		ProjectionMatrix = glm::mat4(1.0f);

		MatrixID = 0;
		ViewMatrixID = 0;
    }
    camera(glm::vec3 pos, glm::vec3 angel, glm::vec3 rotation)
    {
        Position = pos;
        Angel = angel;
        Rotation = rotation;

		ViewMatrix = glm::mat4(1.0f);
		ProjectionMatrix = glm::mat4(1.0f);
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
    void set_position(glm::vec3 pos)
    {
        Position = pos;
    }
    void set_angel(glm::vec3 angel)
    {
        Angel = angel;
    }
    void set_rotation(glm::vec3 rotation)
    {
        Rotation = rotation;
    }
    glm::vec3 get_position()
    {
        return Position;
    }
    glm::vec3 get_angel()
    {
        return Angel;
    }
    glm::vec3 get_rotation()
    {
        return Rotation;
    }
    void update_matrixes()
    {
	    // we have angle of view and camera position set ViewMatrix not to look at the same point

		// i would like to look at Angle
		// convert Angle into vector with cos, sin functions
		glm::vec3 lookAt = glm::vec3(cos(Angel.x), sin(Angel.y), sin(Angel.z));
		// add vector for roll pitch and yaw
		glm::vec3 orient = glm::vec3(cos(Rotation.x), sin(Rotation.y), sin(Rotation.z));
		
		// create ViewMatrix
		ViewMatrix = glm::lookAt(
			Position, // Camera is here
			Position + lookAt, // and looks here : at the same position, plus "lookAt" vector
			// orientation
			orient
		);
        ProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        //ViewMatrix = glm::lookAt(Position, Angel, Rotation);
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