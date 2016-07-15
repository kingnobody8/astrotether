#include "shader_program.h"
#include <fstream>
#include "utility/helper/macro.h"

namespace engine
{
	namespace render
	{
		STATIC std::map<std::string, ShaderFunctor>& IShaderProgram::GetNameMap()
		{
			static std::map<std::string, ShaderFunctor> impl;
			return impl;
		}
		STATIC std::map<ShaderTypeKey, ShaderFunctor>& IShaderProgram::GetKeyMap()
		{
			static std::map<ShaderTypeKey, ShaderFunctor> impl;
			return impl;
		}
		STATIC std::map<ShaderTypeKey, IShaderProgram*>& IShaderProgram::GetShaderMap()
		{
			static std::map<ShaderTypeKey, IShaderProgram*> impl;
			return impl;
		}
	
		STATIC int IShaderProgram::s_nextShaderTypeId = 0;

		STATIC IShaderProgram* IShaderProgram::CreateShader(const std::string& type)
		{
			auto find = IShaderProgram::GetNameMap().find(type);
			assert(find != IShaderProgram::GetNameMap().end() && "This shader type doesn't exist!");
			IShaderProgram* tmp = find->second();
			tmp->LoadProgram();
			return tmp;
		}
		STATIC IShaderProgram* IShaderProgram::CreateShader(const int type)
		{
			auto find = IShaderProgram::GetKeyMap().find(type);
			assert(find != IShaderProgram::GetKeyMap().end());
			IShaderProgram* tmp = find->second();
			tmp->LoadProgram();
			return tmp;
		}
		STATIC void IShaderProgram::RegisterShader(const std::string& typeName, const ShaderTypeKey& key, const ShaderFunctor func)
		{
			std::map<std::string, ShaderFunctor>& name_map = IShaderProgram::GetNameMap();
			std::map<ShaderTypeKey, ShaderFunctor>& key_map = IShaderProgram::GetKeyMap();

			//Each shader should be unique
			assert(name_map.find(typeName) == name_map.end() && "You have a duplicate part!");
			assert(key_map.find(key) == key_map.end() && "You have a duplicate part!");

			//int key = ++IPart::s_nextPartTypeId;
			IShaderProgram::GetNameMap()[typeName] = func;
			IShaderProgram::GetKeyMap()[key] = func;
		}

		//------------------------------------------------------------------------------

		IShaderProgram::IShaderProgram()
			: m_programID(0)
		{
		}

		VIRTUAL IShaderProgram::~IShaderProgram()
		{
			FreeProgram();
		}

		void IShaderProgram::FreeProgram()
		{
			glDeleteProgram(m_programID);
			__todo() //check that this also resets the m_programID to 0
		}

		const bool IShaderProgram::Bind()
		{
			//Use shader
			glUseProgram(m_programID);

			//Check for error
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				__todo() //figure out this compiler error
//				SDL_Log((char*)gluErrorString(error));
				PrintProgramLog(m_programID);
				return false;
			}

			return true;
		}

		void IShaderProgram::Unbind()
		{
			//Use default program
			glUseProgram(NULL);
		}

		void IShaderProgram::PrintProgramLog(GLuint program)
		{
			//Make sure name is shader
			if (glIsProgram(program))
			{
				//Program log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;

				//Get info string length
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

				//Allocate string
				char* infoLog = new char[maxLength];

				//Get info log
				glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
				if (infoLogLength > 0)
				{
					//Print Log
					SDL_Log("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else
			{
				SDL_Log("Name %d is not a program\n", program);
			}
		}

		void IShaderProgram::PrintShaderLog(GLuint shader)
		{
			//Make sure name is shader
			if (glIsShader(shader))
			{
				//Shader log length
				int infoLogLength = 0;
				int maxLength = infoLogLength;

				//Get info string length
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				//Allocate string
				char* infoLog = new char[maxLength];

				//Get info log
				glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
				if (infoLogLength > 0)
				{
					//Print Log
					SDL_Log("%s\n", infoLog);
				}

				//Deallocate string
				delete[] infoLog;
			}
			else
			{
				SDL_Log("Name %d is not a shader\n", shader);
			}
		}

		const GLuint IShaderProgram::LoadShaderFromFile(const std::string& path, const GLenum& shaderType)
		{
			//Open file
			GLuint shaderID = 0;
			std::string shaderString;
			std::ifstream sourceFile(path.c_str());

			//Source file loaded
			if (sourceFile)
			{
				//Get shader source
				shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

				//Load the shader source
				shaderID = LoadShaderFromString(shaderString, shaderType);
			}
			else
			{
				SDL_Log("Unable to open file %s\n", path.c_str());
			}

			return shaderID;
		}

		const GLuint IShaderProgram::LoadShaderFromString(const std::string& shaderString, const GLenum& shaderType)
		{
			GLuint shaderID = 0;

			//Create shader ID
			shaderID = glCreateShader(shaderType);

			//Set shader source
			const GLchar* shaderSource = shaderString.c_str();
			glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

			//Compile shader source
			glCompileShader(shaderID);

			//Check shader for errors
			GLint shaderCompiled = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
			if (shaderCompiled != GL_TRUE)
			{
				SDL_Log("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
				PrintShaderLog(shaderID);
				glDeleteShader(shaderID);
				shaderID = 0;
			}

			return shaderID;
		}
	}
}