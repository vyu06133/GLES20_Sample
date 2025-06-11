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
	static inline void GenerateCheckerPlaneZX(std::vector<VertexPNC>* vertices, const vec3& size, const vec3& grid, const vec4& color0, const vec4& color1)
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

	static inline void GenarateCube(std::vector<VertexPNC>* vertices, const vec3& size)
	{
		vertices->clear();

		// A box has six faces, each one pointing in a different direction.
		constexpr int FaceCount = 6;

		static const vec3 faceNormals[FaceCount] =
				{
						{  0.0f,  0.0f, -1.0f },
						{  0.0f,  0.0f,  1.0f },
						{ -1.0f,  0.0f,  0.0f },
						{  1.0f,  0.0f,  0.0f },
						{  0.0f,  1.0f,  0.0f },
						{  0.0f, -1.0f,  0.0f },
				};

		auto tsize = size * 0.5f;

		// Create each face in turn.
		for (int i = 0; i < FaceCount; i++)
		{
			auto normal = faceNormals[i];

			// Get two vectors perpendicular both to the face normal and to each other.
			const auto basis = (i >= 4) ? vec3(0.0f, 0.0f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);

			const auto side1 = glm::cross(normal, basis);
			const auto side2 = glm::cross(normal, side1);

			vec4 col(MyMath::Abs(normal.x), MyMath::Abs(normal.y), MyMath::Abs(normal.z), 1.0f);
			auto v0 = VertexPNC((normal - side1 - side2) * tsize, normal, col);
			auto v1 = VertexPNC((normal - side1 + side2) * tsize, normal, col);
			auto v2 = VertexPNC((normal + side1 + side2) * tsize, normal, col);
			auto v3 = VertexPNC((normal + side1 - side2) * tsize, normal, col);

			(*vertices).push_back(v0);
			(*vertices).push_back(v1);
			(*vertices).push_back(v2);
			(*vertices).push_back(v0);
			(*vertices).push_back(v2);
			(*vertices).push_back(v3);
		}
	}

	static inline void GenarateSphere(std::vector<VertexPNC>* vertices, float diameter, size_t tessellation)
	{
		assert(tessellation >= 3);

		std::vector<VertexPNC> vtx;
		const size_t verticalSegments = tessellation;
		const size_t horizontalSegments = tessellation * 2;

		const float radius = diameter * 0.5f;

		// Create rings of vertices at progressively higher latitudes.
		for (size_t i = 0; i <= verticalSegments; i++)
		{
			const float v = 1 - float(i) / float(verticalSegments);

			const float latitude = (float(i) * 180.0f / float(verticalSegments)) - 90.0f;
			float dy = MyMath::Sin(latitude);
			float dxz = MyMath::Cos(latitude);

			for (size_t j = 0; j <= horizontalSegments; j++)
			{
				const float u = float(j) / float(horizontalSegments);

				const float longitude = float(j) * 360.0f / float(horizontalSegments);
				float dx = MyMath::Sin(longitude);
				float dz = MyMath::Cos(longitude);

				dx *= dxz;
				dz *= dxz;

				const vec3 normal(dx, dy, dz);
				const vec4 col(dx, dy, dz, 1.0f);

				vtx.push_back(VertexPNC(normal * radius, normal, col));
			}
		}
		std::vector<uint16_t> idx;
		idx.reserve(verticalSegments * (verticalSegments+1) * 6);
		const size_t stride = horizontalSegments + 1;

		for (size_t i = 0; i < verticalSegments; i++)
		{
			for (size_t j = 0; j <= horizontalSegments; j++)
			{
				const size_t nextI = i + 1;
				const size_t nextJ = (j + 1) % stride;

				idx.push_back(uint16_t(i * stride + j));
				idx.push_back(uint16_t(nextI * stride + j));
				idx.push_back(uint16_t(i * stride + nextJ));

				idx.push_back(uint16_t(i * stride + nextJ));
				idx.push_back(uint16_t(nextI * stride + j));
				idx.push_back(uint16_t(nextI * stride + nextJ));
			}
		}

		vertices->clear();
		vertices->reserve(idx.size());
		for (const auto& i: idx)
		{
			vertices->push_back(vtx.at(i));
		}
	}
};