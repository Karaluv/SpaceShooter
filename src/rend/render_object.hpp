// dimond inheritance
// object -> mesh_object -> const_mesh_object, light_object-> const light object, sin light object -> object2render

// first RenderObject is the base class for all renderable objects
// all of which has position
class RenderObject
{
public:
	RenderObject() : m_position(0.0f, 0.0f, 0.0f) {}
	virtual ~RenderObject() {}

	virtual void setPosition(const glm::vec3& position) = 0;
	// orientation with vec3
	virtual void setOrientation(const glm::vec3& orientation) = 0;
	// rotation
	virtual void setRotation(const glm::vec3& rotation) = 0;
	// scale
	virtual void setScale(const float scale) = 0;
	// type
	virtual void setType(int type) = 0;
	// get Type
	virtual int getType() = 0;
	// scale
	virtual float getScale() = 0;
	
	virtual const glm::vec3& getPosition() const { return m_position; }

	// set type of Lightning
	virtual void setLightningTypeUniformId(int id) = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	
protected:
	glm::vec3 m_position;
};

// second MeshObject is the base class for all mesh objects
// all of which has mesh, type, scale, rotation and orientation
// mesh is stored in Id
// with two varibles of camera with class camera and model matrix id with type id
class MeshObject : public RenderObject
{
// typedef Id ad GLuint
using Id = GLuint;
public:
	MeshObject() :  m_type(0), m_scale(1.0f), m_rotation(0.0f, 0.0f, 0.0f), m_orientation(0.0f, 0.0f, 0.0f) {}
	// non default constructor
	virtual ~MeshObject() {}


	virtual void setType(int type) { m_type = type; }
	virtual int getType() { return m_type; }
	

	virtual void setScale(float scale) { m_scale = scale; }
	virtual float getScale() { return m_scale; }

	virtual void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
	virtual const glm::vec3& getRotation() const { return m_rotation; }

	virtual void setOrientation(const glm::vec3& orientation) { m_orientation = orientation; }
	virtual const glm::vec3& getOrientation() const { return m_orientation; }
	
	// position
	virtual void setPosition(const glm::vec3& position) { m_position = position; }
	virtual const glm::vec3& getPosition() const { return m_position; }

	// set model matrix unofrm id
	virtual void setModelMatrixUniformId(Id id) { m_modelMatrixUniformId = id; }
	// the same for scale
	virtual void setScaleUniformId(Id id) { m_scaleUniformId = id; }

	// set type of lightning
	virtual void setLightningTypeUniformId(int id) { m_lightningTypeUniformId = id; }

	
	// scale id
	// set camera
	void setCamera(camera* cam) { m_camera = cam; }
	
	// set model matrix id

protected:
	int m_type;
	float m_scale;
	glm::vec3 m_rotation;
	glm::vec3 m_orientation;
	
	camera *m_camera;

	int mesh_type;
	
	Id m_meshUniformId;
	Id m_modelMatrixUniformId;
	Id m_scaleUniformId;
	
	Id m_lightningTypeUniformId;
};
// define

// function foo


// third ConstMeshObject is the final class for all objects which will be renderd
// all of which has texture (stored as Id), mesh (stored as Id and number of vertices), type, scale, position, rotation and orientation
// function update() and render() are implemented
class ConstMeshObject : public MeshObject
{
using Id = GLuint;
public:
	ConstMeshObject() : m_numVertices(0) {}
	virtual ~ConstMeshObject() {}

	virtual void setTextureId(Id textureId) { m_textureType = textureId; }
	virtual Id getTextureId() const { return m_textureType; }

	// texture uniform 
	virtual void setTextureUniformId(Id textureUniformId) { m_textureUniformId = textureUniformId; }
	virtual Id getTextureUniformId() const { return m_textureUniformId; }
	

	virtual void setNumVertices(int numVertices) { m_numVertices = numVertices; }
	virtual int getNumVertices() const { return m_numVertices; }
	
	virtual void update() {}
	virtual void render()
	{
		//glUniform1f(m_scaleId, 1);

		//glActiveTexture(GL_TEXTURE0);
	
		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		//glUniform1i(m_textureUniformId, 0);
		glUniform1f(m_scaleUniformId, 1);
			
		glUniform1i(m_lightningTypeUniformId, 1);

		glActiveTexture(GL_TEXTURE0);
		
		glBindTexture(GL_TEXTURE_2D, m_textureType);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 100.0f);
		glUniform1i(m_textureUniformId, 0);



	
		glm::mat4 RotationMatrixX = glm::rotate(glm::mat4(1.0f), m_orientation.x, 
			glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 RotationMatrixY = glm::rotate(glm::mat4(1.0f), m_orientation.y, 
			glm::vec3(cos(m_orientation.x), sin(m_orientation.x), 0.0f));
		glm::mat4 RotationMatrixZ = glm::rotate(glm::mat4(1.0f), m_orientation.z, 
			glm::vec3(sin(m_orientation.x) * sin(m_orientation.y), -sin(m_orientation.y) * cos(m_orientation.x), cos(m_orientation.y)));
		
		glm::mat4 RotationMatrix = RotationMatrixZ * RotationMatrixY * RotationMatrixX;

		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), m_position);
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(), glm::vec3(m_scale, m_scale, m_scale));
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
		
		glm::mat4 MVP = m_camera->get_projection_matrix() * m_camera->get_view_matrix() * ModelMatrix;

		glUniformMatrix4fv(m_camera->get_matrix_id(), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(m_modelMatrixUniformId, 1, GL_FALSE, &ModelMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_numVertices, GL_UNSIGNED_SHORT, (void *)(0));
	
	}
	
protected:
	Id m_textureUniformId;
	int m_textureType;
	GLsizei m_numVertices;
};

// skybox
class Skybox : public ConstMeshObject
{
// change update function
public:
	Skybox() {}
	virtual ~Skybox() {}

	void update() override final
	{
		m_position = m_camera->get_position();
	}
	
	void render() override final
	{
		glUniform1f(m_scaleUniformId, 1);

		//glActiveTexture(GL_TEXTURE0);
	
		//glBindTexture(GL_TEXTURE_2D, m_textureId);
		//glUniform1i(m_textureUniformId, 0);
			glActiveTexture(GL_TEXTURE0);
		
			glBindTexture(GL_TEXTURE_2D, m_textureType);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 100.0f);
			glUniform1i(m_textureUniformId, 0);


		

		
		glUniform1i(m_lightningTypeUniformId, 0);
		
		glm::mat4 RotationMatrixX = glm::rotate(glm::mat4(1.0f), m_orientation.x, 
			glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 RotationMatrixY = glm::rotate(glm::mat4(1.0f), m_orientation.y, 
			glm::vec3(cos(m_orientation.x), sin(m_orientation.x), 0.0f));
		glm::mat4 RotationMatrixZ = glm::rotate(glm::mat4(1.0f), m_orientation.z, 
			glm::vec3(sin(m_orientation.x) * sin(m_orientation.y), -sin(m_orientation.y) * cos(m_orientation.x), cos(m_orientation.y)));
		
		glm::mat4 RotationMatrix = RotationMatrixZ * RotationMatrixY * RotationMatrixX;

		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), m_position);
		glm::mat4 ScalingMatrix = glm::scale(glm::mat4(), glm::vec3(m_scale, m_scale, m_scale));
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
		
		glm::mat4 MVP = m_camera->get_projection_matrix() * m_camera->get_view_matrix() * ModelMatrix;

		glUniformMatrix4fv(m_camera->get_matrix_id(), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(m_modelMatrixUniformId, 1, GL_FALSE, &ModelMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, m_numVertices, GL_UNSIGNED_SHORT, (void *)(0));
	}
};

// pattern strategy
// so it can choose between skybox and constant mesh
class Renderable
{
	using Id = GLuint;
public:
	// it can be skybox or constant mesh
	Renderable() {}
	// set strategy
	void setStrategy(RenderObject* strategy) 
	{ m_strategy = strategy; }
	~Renderable() 
	{
		// delete stategy
		if (m_strategy != nullptr)
		{
			delete m_strategy;
			m_strategy = nullptr;
		}
	}
	// render and uodate functions
	void render()
	{
		m_strategy->render();
	}
	void update()
	{
		m_strategy->update();
	}

	// set postion, type, scale, orientation and rotation
	void setPosition(glm::vec3 position) { m_strategy->setPosition(position); }
	void setType(int type) { m_strategy->setType(type); }
	void setScale(float scale) { m_strategy->setScale(scale); }
	void setOrientation(glm::vec3 orientation) { m_strategy->setOrientation(orientation); }
	void setRotation(glm::vec3 rotation) { m_strategy->setRotation(rotation); }

	// getters
	glm::vec3 getPosition() { return m_strategy->getPosition(); }
	int getType() { return m_strategy->getType(); }
	float getScale() { return m_strategy->getScale(); }
	//glm::vec3 getOrientation() { return m_strategy->getOrientation(); }
	//glm::vec3 getRotation() { return m_strategy->getRotation(); }
	
	
	RenderObject* getStrategy() const { return m_strategy; }
private:
	// strategy
	RenderObject* m_strategy = nullptr;
};
	

class Object2Render
{
public:
	Object2Render()
	{
	}

	// default constructor for postion and orientation
	Object2Render(glm::vec3 pos, glm::vec3 orient)
	{
		position = pos;
		orientation = orient;
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

	float getScale()
	{
		return scale;
	}

	void setScale(float newScale) { scale = newScale;
}

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	//glm::quat orientation;
	glm::vec3 orientation;
	unsigned int MyID;
	unsigned int TypeID;
	float scale = 1;
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

	void update_matrix(GLuint ModelMatrixIDV)
	{
		
		ScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
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

		if (ModelMatrixID != 6)
		std::cout << ModelMatrixID << std::endl;

		glDrawElements(GL_TRIANGLES, number_of_vertices, GL_UNSIGNED_SHORT, (void *)(0));
	}

protected:
	camera *camera_;
	GLuint number_of_vertices;

	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(), glm::vec3(scale, scale, scale));
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


