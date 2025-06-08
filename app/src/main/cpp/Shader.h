//
// Created by vyu06133 on 2025/05/27 火.
//

#pragma once
#include "framework.h"
#include "MatrixStack.h"
#include "Vertex.h"

class Shader
{
public:
	GLint FindUniformLoc(const char* Name)
	{
		GLint loc = -1;
		if (uniformLocMap_.count(Name) != 0)
		{
			loc = uniformLocMap_[Name];
			if (loc >= 0)
			{
				return loc;
			}
		}
		loc = glGetUniformLocation(program_, Name);
		uniformLocMap_[Name] = loc;
		return loc;
	}

	bool Create(const char* vertexShaderCode, const char* fragmentShaderCode)
	{
		vertexShader_ = loadShader(GL_VERTEX_SHADER, vertexShaderCode);
		if (!vertexShader_)
		{
			LOGE("vertexShader fail.\n");
			return false;
		}
		fragmentShader_ = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
		if (!fragmentShader_)
		{
			LOGE("fragmentShader fail.\n");
			return false;
		}
		program_ = glCreateProgram();
		if (!program_)
		{
			LOGE("shaderProgram fail.\n");
			return false;
		}
		glAttachShader(program_, vertexShader_);
		glAttachShader(program_, fragmentShader_);
		glLinkProgram(program_);

		return true;
	}
	enum UniformSemantics
	{
		Model,
		View,
		Projection,
		MVP,
		LightPos,
		LightColor,
		Ambient,
		UniformSemanticsMax
	};
	GLint UniformLocs_[UniformSemanticsMax] = {};
	bool UpdateUniform(const char* Name, const GlmVariant& variant) {
		auto loc = FindUniformLoc(Name);
		if (loc < 0) {
			LOGE("UniformSemantics(%s) missing.\n", Name);
			return false;
		}
		return UpdateUniform(loc, variant);
	}
	bool UpdateUniform(UniformSemantics semantics, const GlmVariant& variant) {
		if (0 > semantics || semantics >= UniformSemanticsMax) {
			LOGE("UniformSemantics(%d) out of range.\n", semantics);
			return false;
		}
		return UpdateUniform(UniformLocs_[semantics], variant);
	}
	bool UpdateUniform(GLint loc, const GlmVariant& variant)
	{
		if (loc < 0)
		{
			return false;
		}
		switch (variant.index())
		{
			default:
			{
				LOGE("UpdateUniform unkwoun variant.\n");
				return false;
			}
			case 0:// int
			{
				glUniform1i(loc, std::get<int>(variant));
				break;
			}
			case 1:// float
			{
				glUniform1f(loc, std::get<float>(variant));
				break;
			}
			case 2:// vec2
			{
				auto v = std::get<vec2>(variant);
				glUniform2f(loc, v.x, v.y);
				break;
			}
			case 3:// vec3
			{
				auto v = std::get<vec3>(variant);
				glUniform3f(loc, v.x, v.y, v.z);
				break;
			}
			case 4:// vec4
			{
				auto v = std::get<vec4>(variant);
				glUniform4f(loc, v.x, v.y, v.z, v.w);
				break;
			}
			case 5:// quat
			{
				auto v = std::get<quat>(variant);
				glUniform4f(loc, v.x, v.y, v.z, v.w);
				break;
			}
			case 6:// mat3
			{
				auto m = std::get<mat3>(variant);
				glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(m));
				break;
			}
			case 7:// mat4
			{
				auto m = std::get<mat4>(variant);
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
				break;
			}
		}
		return true;

	}
	auto GetSemanticsLoc(UniformSemantics semantics) const
	{
		if (semantics < 0 || semantics >= UniformSemanticsMax)
		{
			LOGE("UniformSemantics(%d) out of range.\n",semantics);
			return -1;
		}
		return UniformLocs_[semantics];
	}
	GLint ConnectUniformAndSemantics(UniformSemantics semantics, const char* Name)
	{
		if (semantics < 0 || semantics >= UniformSemanticsMax)
		{
			LOGE("UniformSemantics(%d) out of range.\n",semantics);
			return 0;
		}
		auto loc = FindUniformLoc(Name);
		UniformLocs_[semantics] = loc;
		return loc;
	}
	GLint  ConnectAttrib(const char* SenainticsName, const char* AttribName)
	{
		return 0;
	}
	template<typename T>
	inline void VertexAttribPointer(void* top) {
		Vertex::INPUT_ELEMENT_DESC *elm = (Vertex::INPUT_ELEMENT_DESC *) T::InputLayout;
		char* ptr = (char*)top;
		while (elm && elm->SemanticName != nullptr) {
			auto handle = glGetAttribLocation(program_, elm->SemanticName);
			//assert(handle >= 0):
			//glVertexAttribPointer(handle, 3, GL_FLOAT, GL_FALSE, 0, triangleVertices);
			//glEnableVertexAttribArray(positionHandle);
			//glEnableVertexAttribArray(elm->index);
			glEnableVertexAttribArray(handle);
			glVertexAttribPointer(handle, elm->size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)ptr);
			ptr += sizeof(float) * elm->size;
			elm++;
		}
	}

private:
	GLuint loadShader(GLenum type, const char *shaderCode) {
		GLuint shader = glCreateShader(type);
		if (shader == 0) {
			// glCreateShaderが失敗した場合、通常はメモリ不足などの問題
			LOGE("ERROR: Failed to create shader of type 0x%x\n", type);
			return 0;
		}
		glShaderSource(shader, 1, &shaderCode, nullptr);
		glCompileShader(shader);
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); // コンパイルステータスを取得
		if (!compiled) {
			// コンパイルに失敗した場合
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen); // エラーログの長さを取得

			if (infoLen > 1) { // エラーログが存在する場合 (1より大きいのはヌル終端文字のため)
				std::string info(infoLen,'\0');
				glGetShaderInfoLog(shader, infoLen, nullptr, info.data()); // エラーログを取得
				LOGE("ERROR: Could not compile shader 0x%x:\n%s\n", type, info.data());
			} else {
				LOGE("ERROR: Could not compile shader 0x%x: No info log available.\n", type);
			}

			glDeleteShader(shader); // エラーが発生したシェーダーは削除
			return 0; // 失敗を示す0を返す
		}
		return shader;
	}

public:
	GLuint& Program()
	{
		return program_;
	}

	GLint& Width()
	{
		return screenWidth_;
	}

	GLint& Height()
	{
		return screenHeight_;
	}
private:
	GLint screenWidth_ = 0;
	GLint screenHeight_ = 0;
	GLuint program_ = 0;
	GLuint vertexShader_ = 0;
	GLuint fragmentShader_ = 0;
	std::unordered_map<std::string, GLint> uniformLocMap_;
public:
	template<typename V>
	void BindVertexBuffer(const V* bufferTop)
	{
		//敢えてVAOを使わないパターン

		glBindBuffer(GL_ARRAY_BUFFER, 0); // 必須（バッファをバインド解除）
		auto elm = V::InputLayout;
		char* ofs = (char*)bufferTop;
		while (elm && elm->SemanticName != nullptr)
		{
			auto handle = glGetAttribLocation(program_, elm->SemanticName);
			glEnableVertexAttribArray(handle);
			glVertexAttribPointer(handle, elm->size, GL_FLOAT, GL_FALSE, sizeof(V), ofs);
			ofs += 4/*sizeof(float)*/ * elm->size;
			elm++;
		}
	}

#if 0
	struct Attrib
	{
		std::string attribName = "";
		std::string semanticName = "";
		int dim = 0;
		int strride = 0;
		const void* ofs = nullptr;
		GLuint handle = 0;
	};
private:
	std::unordered_map<std::string, Attrib> attribLocMap_;

public:

	GLuint VertexAttribPointer(const char* Name, const char* Semantic, int dim, int strride, const void* ofs)
	{
		Attrib attr;
		if (attribLocMap_.count(Name) != 0)
		{
			LOGE("%s already exisits\n", Name);
			return 0u;
		}
		attr.attribName = Name;
		attr.semanticName = Semantic;
		attr.handle = glGetAttribLocation(program_, Name);
		attr.dim = dim;
		attr.strride = strride;
		attr.ofs = ofs;
		LOGD("VertexAttribPointer(%s %s %d %d 0x%x)=%u\n", Name, Semantic, dim, strride, ofs, attr.handle);
		glVertexAttribPointer(
			attr.handle,
			dim,
			GL_FLOAT,      // データ型を GL_FLOAT にする
			GL_FALSE, // 正規化しない
			strride,
			ofs );
		attribLocMap_[Name] = attr;
		return attr.handle;
	}
#endif
};