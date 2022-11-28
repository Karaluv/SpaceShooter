class Object2Render
{
public:
	Object2Render()
	{
		
	}
	~Object2Render() = default;

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getRotation() const { return rotation; }
	//glm::quat getOrientation() const { return orientation; }
	glm::vec3 getOrientation() const { return orientation; }
	
	void setPosition(glm::vec3 pos) { position = pos; }
	void setRotation(glm::vec3 rot) { rotation = rot; }
	//void setOrientation(glm::quat ori) { orientation = ori; }
	void setOrientation(glm::vec3 ori) { orientation = ori; }

	int getID() const { return MyID; }
	void setID(int id) { MyID = id; }

	int getTypeId() const { return TypeID; }
	void setTypeId(int id) { TypeID = id; }

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	//glm::quat orientation;
	glm::vec3 orientation;
	unsigned int MyID;
	unsigned int TypeID;
};

class render_object : public Object2Render
{
public:
	render_object()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		//orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		orientation = glm::vec3(0.0f, 0.0f, 0.0f);
		MyID = 0;
		TypeID = 0;
		// camera
		
	}
	~render_object() = default;

	void update_number_of_vertices(int num) { number_of_vertices = num; }
	void update_camera(camera *cam) { camera_ = cam; }

	void update_matrix(glm::mat4 ScalingMatrixV, GLuint ModelMatrixIDV)
	{
		ScalingMatrix = ScalingMatrixV;
		ModelMatrixID = ModelMatrixIDV;
	}
	void update_model(int index, GLuint IndexBufferV)
	{
		number_of_vertices = IndexBufferV;
		MyID = index;
	}
	void draw_object()
	{
		// euler angles matrixes 
		glm::mat4 RotationMatrixX = glm::rotate(glm::mat4(1.0f), orientation.x, 
			glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 RotationMatrixY = glm::rotate(glm::mat4(1.0f), orientation.y, 
			glm::vec3(cos(orientation.x), sin(orientation.x), 0.0f));
		glm::mat4 RotationMatrixZ = glm::rotate(glm::mat4(1.0f), orientation.z, 
			glm::vec3(sin(orientation.x) * sin(orientation.y), -sin(orientation.y) * cos(orientation.x), cos(orientation.y)));
		
		glm::mat4 RotationMatrix = RotationMatrixZ * RotationMatrixY * RotationMatrixX;



		
		//glm::mat4 RotationMatrix = glm::eulerAngleYXZ(orientation.x, orientation.y, orientation.z);
		// Rotate quaternions
		//glm::mat4 RotationMatrix = glm::toMat4(orientation);
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), position); // A bit to the left
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
		glm::mat4 MVP = camera_->get_projection_matrix() * camera_->get_view_matrix() * ModelMatrix;

		glUniformMatrix4fv(camera_->get_matrix_id(), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, number_of_vertices, GL_UNSIGNED_SHORT, (void *)(0));
	}

protected:
	camera *camera_;
	GLuint number_of_vertices;

	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	int ModelMatrixID;
};

class light : public Object2Render
{
private:
	glm::vec3 Color = glm::vec3(1, 1, 1);
	float Power = 1.0f;

public:
	light() = default;
	light(glm::vec3 pos, glm::vec3 color, float power, glm::vec3 direction)
	{
		Color = color;
		Power = power;
		position = pos;
		rotation = direction;
	}
	~light() = default;
	glm::vec3 get_position() { return position; }
	glm::vec3 get_color() { return Color; }
	float get_power() { return Power; }
	void set_position(glm::vec3 pos) { position = pos; }
	void set_color(glm::vec3 color) { Color = color; }
	void set_power(float power) { Power = power; }
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
/*
class RenderCustomObj : render_object
{
public:
	RenderCustomObj() = default;
	RenderCustomObj(int id, int type_id, glm::vec3 pos, glm::vec3 rot, glm::quat ori, int num_vertices)
	{
		MyID = id;
		TypeID = type_id;
		position = pos;
		rotation = rot;
		orientation = ori;
		ScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
		number_of_vertices = num_vertices;
	}
	~RenderCustomObj() = default;

private:
};
*/
