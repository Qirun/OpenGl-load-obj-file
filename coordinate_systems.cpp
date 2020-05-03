#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>// settings
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <vector>
GLFWwindow* window;
#include"controls.hpp"
#include "shader_m.h"
#include "objloader.hpp"


//bool firstMouse = true;
//float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//float pitch = 0.0f;
//float lastX = 800.0f / 2.0;
//float lastY = 600.0 / 2.0;
//float fov = 45.0f;

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//��һ����������ѡ������ƣ��ڶ�������ѡ���ֵ

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "load_obj", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD��������OpenGl�ĺ���ָ�룬��ʼ��GLAD
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }//���Ǹ�GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("6.1.coordinate_systems.vs", "6.1.coordinate_systems.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ("Charmander.obj", vertices, uvs, normals);
   
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO_vertex;

    glGenBuffers(1, &VBO_vertex);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);//���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���û���������ݸ��Ƶ���ǰ�󶨻���ĺ���
    unsigned int VBO_uv;
    glGenBuffers(1, &VBO_uv);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
 


    //GLuint vertexbuffer;
    //glGenBuffers(1, &vertexbuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    //
    //GLuint uvbuffer;
    //glGenBuffers(1, &uvbuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    //glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // position attribute
    /*������������
    ��һ������ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
�¸��������������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������5��float֮�����ǰѲ�������Ϊ5 * sizeof(float)��Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶��
����Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ�����������
����ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������
    */
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    //ÿ���������Դ�һ��VBO������ڴ��л���������ݣ��������Ǵ��ĸ�VBO�������п����ж��VBO����ȡ����ͨ���ڵ���glVertexAttribPointerʱ�󶨵�GL_ARRAY_BUFFER��VBO������
    // texture coord attribute
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );



    glBindBuffer(GL_ARRAY_BUFFER, 0);//���
    glBindVertexArray(0);//��󣬺���Ҫʹ��ʱ���ٰ󶨡�





    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("r.bmp", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
              
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
    
        // activate shader
        ourShader.use();

        computeMatricesFromInputs();
        // create transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        
      
        glm::mat4 MVP = projection * view * model;

        
        // retrieve the matrix uniform locations
        unsigned int MatrixID = glGetUniformLocation(ourShader.ID, "MVP");
       
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // pass them to the shaders (3 different ways)
                
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
       
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window); //������ɫ����
        glfwPollEvents();//�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_vertex);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    return 0;
}

