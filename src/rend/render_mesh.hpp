class MeshControl
{
protected:
    GLuint vertexPosition_modelspaceID;
    GLuint vertexUVID;
    GLuint vertexNormal_modelspaceID;

    std::vector<GLuint> ElementBuffers;
    std::vector<GLuint> NormalBuffers;
    std::vector<GLuint> UvBuffers;
    std::vector<GLuint> VertexBuffers;
    std::vector<GLuint> IndexBuffer;

    // std::map<int, std::string> PathDict = {{0, "res/scube.obj"}, {1, "res/suzanne.obj"}};
    // std::map<std::string, int> NamesDict = {{"cube", 0}, {"monkey", 1}};

    bool RenderTaskisRunning_ = false;
    bool RenderTaskisTerminated_ = true;

public:
    MeshControl() {}
    ~MeshControl() 
    {
       
		// delete buffers
		glDeleteBuffers(ElementBuffers.size(), &ElementBuffers[0]);
		glDeleteBuffers(NormalBuffers.size(), &NormalBuffers[0]);
		glDeleteBuffers(UvBuffers.size(), &UvBuffers[0]);
		glDeleteBuffers(VertexBuffers.size(), &VertexBuffers[0]);
		glDeleteBuffers(IndexBuffer.size(), &IndexBuffer[0]);


    }

    void DeleteMesh(int index, GLuint &vertexbuffer, GLuint &uvbuffer, GLuint &normalbuffer,
                    GLuint &elementbuffer)
    {
        glDeleteBuffers(index, &vertexbuffer);
        glDeleteBuffers(index, &uvbuffer);
        glDeleteBuffers(index, &normalbuffer);
        glDeleteBuffers(index, &elementbuffer);
        // glDeleteProgram(programID);
    }
    void LoadAllCustomMesh(std::vector<GLuint> &ElementBuff, std::vector<GLuint> &NormalBuff,
                           std::vector<GLuint> &UvBuff, std::vector<GLuint> &VertexBuff,
                           std::vector<GLuint> &IndexBuff, std::map<int, std::string> PathDict)
    {
        for (int i = 0; i < PathDict.size(); ++i)
        {
            ElementBuff.push_back(GLuint(0));
            NormalBuff.push_back(GLuint(0));
            UvBuff.push_back(GLuint(0));
            VertexBuff.push_back(GLuint(0));
            IndexBuff.push_back(GLuint(0));
            IndexBuff.back() = load_mesh("res/obj/" + PathDict[i], i + 1, &ElementBuff.back(), &NormalBuff.back(),
                                         &UvBuff.back(), &VertexBuff.back());
        }
    }

    void UpdateMesh()
    {
    }


    void LoadCustomMesh(std::string Path, int index, std::vector<GLuint> &ElementBuff,
                        std::vector<GLuint> &NormalBuff, std::vector<GLuint> &UvBuff,
                        std::vector<GLuint> &VertexBuff, std::vector<GLuint> &IndexBuff)
    {
        ElementBuff.push_back(GLuint(0));
        NormalBuff.push_back(GLuint(0));
        UvBuff.push_back(GLuint(0));
        VertexBuff.push_back(GLuint(0));
        IndexBuff.push_back(GLuint(0));
        IndexBuff.back() = load_mesh(Path, index, &ElementBuff.back(), &NormalBuff.back(),
                                     &UvBuff.back(), &VertexBuff.back());
    }

    GLuint load_mesh(std::string name, int index_of_model, GLuint *elementbuffer, GLuint *normalbuffer,
                     GLuint *uvbuffer, GLuint *vertexbuffer)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<unsigned short> indices;
        std::vector<glm::vec3> indexed_vertices;
        std::vector<glm::vec2> indexed_uvs;
        std::vector<glm::vec3> indexed_normals;

        bool res = loadOBJ(name.c_str(), vertices, uvs, normals);

        indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

        // Load it into a VBO
		
        index_of_model = 1;

		// assigne 0 on temp
		

		glGenBuffers(index_of_model, vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0],
                     GL_STATIC_DRAW);

        glGenBuffers(index_of_model, uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0],
                     GL_STATIC_DRAW);

        glGenBuffers(index_of_model, normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, *normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0],
                     GL_STATIC_DRAW);

        // Generate a buffer for the indices as well

        glGenBuffers(index_of_model, elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0],
                     GL_STATIC_DRAW);

        size_t indices_size = indices.size();

        vertices.clear();
        uvs.clear();
        normals.clear();

        indices.clear();
        indexed_vertices.clear();
        indexed_uvs.clear();

        return GLuint(indices_size);
    }

    void switch_render_mesh(GLuint VertexBuffer, GLuint UvBuffer, GLuint NormalBuffer, GLuint ElementsBuffer,
                            GLuint vertexPosition_modelspaceID, GLuint vertexUVID,
                            GLuint vertexNormal_modelspaceID)
    {
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
        glVertexAttribPointer(
            vertexPosition_modelspaceID, // The attribute we want to configure
            3,                           // size
            GL_FLOAT,                    // type
            GL_FALSE,                    // normalized?
            0,                           // stride
            (void *)0                    // array buffer offset
        );

        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
        glVertexAttribPointer(
            vertexUVID, // The attribute we want to configure
            2,          //   size : U+V => 2
            GL_FLOAT,   // type
            GL_FALSE,   // normalized?
            0,          // stride
            (void *)0   // array buffer offset
        );

        glEnableVertexAttribArray(vertexNormal_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
        glVertexAttribPointer(
            vertexNormal_modelspaceID, // The attribute we want to configure
            3,                         // size
            GL_FLOAT,                  // type
            GL_FALSE,                  // normalized?
            0,                         // stride
            (void *)0                  // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementsBuffer);
    }
};

class CustomMesh : MeshControl
{
public:
private:
};

class VoxelMesh : MeshControl
{
public:
private:
};