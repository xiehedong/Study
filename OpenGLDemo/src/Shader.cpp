#include "Shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    // 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ��֤ifstream��������׳��쳣��
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // ���ļ�
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;
        // ��ȡ�ļ��Ļ������ݵ���������
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // �ر��ļ�������
        vShaderFile.close();
        fShaderFile.close();
        // ת����������string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. ������ɫ��
    unsigned int vertex, fragment;

    //----------------���붥����ɫ��--------------------start
    //����һ����ɫ������ע�⻹����ID�����õġ��������Ǵ������������ɫ��Ϊunsigned int��
    //Ȼ����glCreateShader���������ɫ����
    //��ɫ������:GL_VERTEX_SHADER��ʾ������ɫ��
    vertex = glCreateShader(GL_VERTEX_SHADER);
    //�������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ���������
    //glShaderSource������Ҫ�������ɫ��������Ϊ��һ��������
    //�ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ����
    //�����������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��
    glShaderSource(vertex, 1, &vShaderCode, NULL);//��Դ�븽�ӵ���ɫ������
    glCompileShader(vertex);//������ɫ��
    
    
    // ��ӡ�����������еĻ���
    //����ܻ�ϣ������ڵ���glCompileShader������Ƿ�ɹ��ˣ����û�ɹ��Ļ���
    //�㻹��ϣ��֪��������ʲô������������޸����ǡ�������ʱ�������ͨ�����´�����ʵ�֣�
    CheckCompileErrors(ID, "VERTEX");
    //�������Ƕ���һ�����ͱ�������ʾ�Ƿ�ɹ����룬��������һ�����������Ϣ������еĻ�����������
    //Ȼ��������glGetShaderiv����Ƿ����ɹ����������ʧ�ܣ�
    //���ǻ���glGetShaderInfoLog��ȡ������Ϣ��Ȼ���ӡ����
    //��������ʱ��û�м�⵽�κδ��󣬶�����ɫ���ͱ�����ɹ��ˡ�

    //----------------���붥����ɫ��--------------------end

    // Ƭ����ɫ��Ҳ����
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // ��ӡ�����������еĻ���
    CheckCompileErrors(ID, "FRAGMENT");

    //----------------������ɫ��--------------------start
    //��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
    //���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������
    //Ȼ������Ⱦ�����ʱ�򼤻������ɫ�������Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�
    // ��ɫ������:glCreateProgram��������һ�����򣬲������´�����������ID���á�
    ID = glCreateProgram();
    //����������Ҫ��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram�������ǣ�
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // ��ӡ���Ӵ�������еĻ���
    //������ɫ���ı���һ��������Ҳ���Լ��������ɫ�������Ƿ�ʧ�ܣ�����ȡ��Ӧ����־��
    CheckCompileErrors(ID, "PROGRAM");

    //�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ�����ˣ�
    // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    //----------------������ɫ��--------------------end
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::Use()
{
    //����glUseProgram����������������
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);//??
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3f(const std::string& name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}

void Shader::SetVec4f(const std::string& name, float r, float g, float b, float a) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), r, g, b, a);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
