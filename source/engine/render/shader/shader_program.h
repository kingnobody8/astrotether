#pragma once
#include "../render_core.h"
#include "utility/math/math.h"
#include "utility/types/types.h"
#include <string>
#include <list>
#include "utility/helper/macro.h"
#include "utility/helper/func.h"
#include <map>

namespace engine
{
	namespace render
	{
		//forward declare
		class IShaderProgram;

		typedef ulonglong									ShaderTypeKey;
		typedef std::function<IShaderProgram*(void)>		ShaderFunctor;

		class IShaderProgram
		{
			//Internal
		private:
			static std::map<std::string, ShaderFunctor>& GetNameMap();
			static std::map<ShaderTypeKey, ShaderFunctor>& GetKeyMap();
			static std::map<ShaderTypeKey, IShaderProgram*>& GetShaderMap();

		protected:
			static int s_nextShaderTypeId;

			//External
		public:
			static IShaderProgram* CreateShader(const std::string& type);
			static IShaderProgram* CreateShader(const int type);
			static void RegisterShader(const std::string& typeName, const ShaderTypeKey& key, const ShaderFunctor func);

			static const int Type = 0;
			virtual int GetType() const { return Type; }
			virtual const char* GetTypeName() const { return "IShader"; }

		private:
			u16 m_usPartIndex;

			//------------------------------------------------------------------------------

		protected:
			GLuint m_programID;

		public:
			IShaderProgram();
			virtual ~IShaderProgram();
			virtual const bool LoadProgram() = 0;
			virtual void FreeProgram();
			const bool Bind();
			void Unbind();
			const GLuint GetProgramID() { return m_programID; }

		protected:
			void PrintProgramLog(GLuint program);
			void PrintShaderLog(GLuint shader);
			const GLuint LoadShaderFromFile(const std::string& path, const GLenum& shaderType);
			const GLuint LoadShaderFromString(const std::string& shaderString, const GLenum& shaderType);

#define DECLARE_SHADER_TYPE_INFO(CLASS)								\
	typedef CLASS class_t;											\
	static const int Type;											\
	virtual inline int GetType() const { return Type; }				\
	virtual const char* GetTypeName() const;
#define DEFINE_SHADER_TYPE_INFO(CLASS)								\
	const int CLASS::Type = ++IShaderProgram::s_nextShaderTypeId;	\
	const char* CLASS::GetTypeName() const { return #CLASS; }
		};
	}
}