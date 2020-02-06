#pragma once
#include <glad/glad.h>
#include <cassert>
#include <fstream>
#include <memory>
#include <string>

namespace Starter
{
    class Shader
    {
    public:
        Shader() { 
            program = glCreateProgram(); 
        }

        void activate()   { 
            glUseProgram(program); 
        }
        void deactivate() { 
            glUseProgram(0); 
        }
        GLuint get() { 
            return program; 
        }
        void destroy() { 
            glDeleteProgram(program); 
        }
        void attach(std::string const &filename)
        {
            std::ifstream fd(filename.c_str());
            if (fd.fail())
            {
                fprintf(stderr,"Error when loading shader",filename.c_str());
                return;
            }
            auto src = std::string(std::istreambuf_iterator<char>(fd),(std::istreambuf_iterator<char>()));
            const char * source = src.c_str();
            auto shader = create(filename);

            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            // Display errors
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buffer(new char[length]);
                glGetShaderInfoLog(shader, length, nullptr, buffer.get());
                fprintf(stderr, "%s\n%s", filename.c_str(), buffer.get());
            }
            assert(status);

            glAttachShader(program, shader);
            glDeleteShader(shader);
        }


        void link()
        {
            glLinkProgram(program);
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (!status)
            {
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buf(new char[length]);
                glGetProgramInfoLog(program, length, nullptr, buf.get());
                fprintf(stderr, "%s", buf.get());
            }
            assert(status);
        }

        void createShader(std::string const &vertexShaderPath,std::string const &fragmentShaderPath)
        {
            attach(vertexShaderPath);
            attach(fragmentShaderPath);
            link();
        }

        bool isValid()
        {
            glValidateProgram(program);
            glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
            if (!status)
            {
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<char[]> buf(new char[length]);
                glGetProgramInfoLog(program, length, nullptr, buf.get());
                fprintf(stderr, "%s", buf.get());
                return false;
            }

            return true;
        }
        GLuint create(std::string const &filename)
        {
            auto idx = filename.rfind(".");
            auto ext = filename.substr(idx + 1);
                 if (ext == "comp") return glCreateShader(GL_COMPUTE_SHADER);
            else if (ext == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
            else if (ext == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
            else if (ext == "tcs")  return glCreateShader(GL_TESS_CONTROL_SHADER);
            else if (ext == "tes")  return glCreateShader(GL_TESS_EVALUATION_SHADER);
            else if (ext == "vert") return glCreateShader(GL_VERTEX_SHADER);
            else                    return false;
        }

    private:
        GLint  status, length;
        GLuint program;
        Shader(Shader const &) = delete;
        Shader & operator =(Shader const &) = delete;
    };
}
