class camera
{
private:
    glm::vec3 Position = glm::vec3(0, 0, 0);
    glm::vec3 Angel = glm::vec3(0, 0, 0);
    glm::vec3 Rotation = glm::vec3(0, 0, 0);
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

	// mouse_pos
	float mouse_x = 0;
	float mouse_y = 0;

    float roll = 0;

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
    void set_camera_angle(float x, float y)
    {
		mouse_x = x;
		mouse_y = y;
    }
	void set_roll(float r)
	{
		roll = r;
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
		// lookAt vector from mouse angle 
		// convert polar cordinates to decard
		//glm::vec3 orient = glm::vec3(cos(Rotation.x), sin(Rotation.y), sin(Rotation.z));

		// mouse_pos (x,y) are two angles
		// convert them to a vector with such spherical cordinates
		// mouse_y - theta, mouse_x - phi
		glm::vec3 lookAt = glm::vec3(
			cos(mouse_y) * sin(mouse_x),
			sin(mouse_y),
			cos(mouse_y) * cos(mouse_x)
		);
		
		// now orient from roll
        glm::vec3 orient = glm::vec3(0, 1, 0);

		// change orieantation when camera switchs poll
		if (mouse_y > 3.14f)
		{
			orient = glm::vec3(0, -1, 0);
		}
		
		
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