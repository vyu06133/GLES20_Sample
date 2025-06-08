//
// Created by vyu06133 on 2025/05/29 木.
//

#pragma once
#include "framework.h"

struct Vertex {
public:
	struct INPUT_ELEMENT_DESC {
		const char *SemanticName;
		GLint index;
		GLint size;
		GLenum type;
	};
};

struct VertexP : public Vertex {
public:
	vec3 pos;

	VertexP() : pos(0.0f) {}
	VertexP(const vec3& P) : pos(P) {}
	VertexP(const float& X, const float& Y, const float& Z) : pos(vec3(X, Y, Z)) {}

	inline static INPUT_ELEMENT_DESC InputLayout[] = {
			{"a_position", 0, 3, GL_FLOAT},
			{}};
};

struct VertexPNC : public VertexP {
public:
	vec3 normal;
	vec4 color;
	VertexPNC() : VertexP(), normal(0.0f), color(1.0f)	{}
	VertexPNC(const vec3& P, const vec3& N, const vec4& C) : VertexP(P), normal(N), color(C)	{}
	VertexPNC(const float& PX, const float& PY, const float& PZ,
	          const float& NX, const float& NY, const float& NZ,
	          const float& R, const float& G, const float& B, const float& A)
		: VertexP(vec3(PX, PY, PZ)), normal(NX, NY, NZ), color(R, G, B, A) {}

	inline static INPUT_ELEMENT_DESC InputLayout[] = {
			{ "a_position", 0, 3, GL_FLOAT },
			{ "a_normal", 1, 3, GL_FLOAT },
			{ "a_color", 2, 4, GL_FLOAT },
			{}
	};
};

struct Geometry
{
	static void GenerateCheckerPlaneZX(std::vector<VertexPNC>* vertices, const vec3& size, const vec3& grid, const vec4& color0, const vec4& color1)
	{
		assert(vertices);
		int32_t gridz = static_cast<int32_t>(grid.z);
		int32_t gridx = static_cast<int32_t>(grid.x);
		float wZ = size.z / grid.z;
		float wX = size.x / grid.x;

		vec4 color[2] = { color0, color1 };

		for (int32_t j = 0; j < gridz; j++)
		{
			float z1 = -size.z * 0.5f + wZ * static_cast<float>(j);
			float z2 = z1 + wZ;
			//float v1 = static_cast<float>(j + 0) / grid.z;
			//float v2 = static_cast<float>(j + 1) / grid.z;
			for (int32_t i = 0; i < gridx; i++)
			{
				float x1 = -size.x * 0.5f + wX * static_cast<float>(i);
				float x2 = x1 + wX;
				//float u1 = static_cast<float>(i + 0) / grid.x;
				//float u2 = static_cast<float>(i + 1) / grid.x;
				auto n = vec3(0.0f, 1.0f, 0.0f);

				//auto v11 = VertexP(vec3(x1, 0.0f, z1));
				//auto v12 = VertexP(vec3(x2, 0.0f, z1));
				//auto v21 = VertexP(vec3(x1, 0.0f, z2));
				//auto v22 = VertexP(vec3(x2, 0.0f, z2));
				auto v11 = VertexPNC(vec3(x1, 0.0f, z1), n, color[(i + j) & 1]);
				auto v12 = VertexPNC(vec3(x2, 0.0f, z1), n, color[(i + j) & 1]);
				auto v21 = VertexPNC(vec3(x1, 0.0f, z2), n, color[(i + j) & 1]);
				auto v22 = VertexPNC(vec3(x2, 0.0f, z2), n, color[(i + j) & 1]);

				vertices->push_back(v11);
				vertices->push_back(v12);
				vertices->push_back(v22);
				vertices->push_back(v11);
				vertices->push_back(v22);
				vertices->push_back(v21);
			}
		}
	}

};