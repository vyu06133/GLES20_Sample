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
	VertexP(const float& X, const float& Y, const float& Z) : pos(X, Y, Z) {}

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
		: VertexP(PX, PY, PZ), normal(NX, NY, NZ), color(R, G, B, A) {}

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

	static void GenerateSphere(std::vector<VertexPNC>* vertices, float radius, int slices, int stacks)
	{
		// 既存のデータをクリア
		vertices->clear();

		if (slices == 0 || stacks < 2)
		{
			LOGE("Warning: slices = 0 or stacks < 2. No sphere generated.\n");
			return;
		}

		// `std::vector` を使用して sin/cos テーブルを安全に管理
		std::vector<float> sint1(slices + 1);
		std::vector<float> cost1(slices + 1);
		std::vector<float> sint2(stacks + 1);
		std::vector<float> cost2(stacks + 1);

		// スライス (全円) の計算
		float angle_step_slices = 2.0f * MyMath::_PAI / (float)slices;
		for (int i = 0; i <= slices; ++i)
		{
			sint1[i] = MyMath::Sin(angle_step_slices * i);
			cost1[i] = MyMath::Cos(angle_step_slices * i);
		}

		// スタック (半円) の計算
		float angle_step_stacks = MyMath::_PAI / (float)stacks;
		for (int i = 0; i <= stacks; ++i)
		{
			sint2[i] = MyMath::Sin(angle_step_stacks * i);
			cost2[i] = MyMath::Cos(angle_step_stacks * i);
		}

		// --- 頂点の生成 ---

		// 最初にすべてのユニークな頂点を生成する (中間ステップ)
		// これにより、計算が簡素化され、後で三角形を形成しやすくなります
		// 実際の頂点データは、三角形を構成するために複数回追加されます
		std::vector<std::vector<VertexPNC>> grid_vertices(stacks + 1, std::vector<VertexPNC>(slices + 1));

		// 北極
		grid_vertices[0][0] = VertexPNC(
				vec3(0.0f, 0.0f, radius),
				vec3(0.0f, 0.0f, 1.0f),
				vec4(0.0f, 0.0f, 1.0f, 1.0f) // 青色
		);

		// 中間の緯度帯の頂点
		for (int i = 1; i < stacks; i++)
		{
			for (int j = 0; j <= slices; j++)
			{
				float x = cost1[j] * sint2[i];
				float y = sint1[j] * sint2[i];
				float z = cost2[i];

				grid_vertices[i][j] = VertexPNC(
						vec3(x * radius, y * radius, z * radius),
						vec3(x, y, z),
						vec4(MyMath::Abs(x), MyMath::Abs(y), MyMath::Abs(z), 1.0f)
				);
			}
		}

		// 南極
		grid_vertices[stacks][0] = VertexPNC(
				vec3(0.0f, 0.0f, -radius),
				vec3(0.0f, 0.0f, -1.0f),
				vec4(0.0f, 0.0f, 1.0f, 1.0f) // 青色
		);


		// --- TRIANGLES として頂点を直接追加 ---

		// 1. 北極周囲の三角形
		// 北極の頂点は grid_vertices[0][0] を使用
		for (int j = 0; j < slices; ++j)
		{
			// v0: 北極
			// v1: 最初の緯度帯の現在の頂点 (i=1, j)
			// v2: 最初の緯度帯の次の頂点 (i=1, j+1)
			vertices->push_back(grid_vertices[0][0]); // 北極

			// 注意: grid_vertices[0][0] は単一の北極頂点です
			// 最初の緯度帯の頂点は grid_vertices[1][j] と grid_vertices[1][j+1] を参照します
			vertices->push_back(grid_vertices[1][j+1]); // 次の頂点 (反時計回りの場合)
			vertices->push_back(grid_vertices[1][j]);   // 現在の頂点
		}

		// 2. 中間の帯の四角形を2つの三角形に分割
		for (int i = 1; i < stacks - 1; ++i) // i=1 から stacks-2 まで
		{
			for (int j = 0; j < slices; ++j)
			{
				// 四角形の4つの頂点
				// v1 -- v2   (grid_vertices[i][j]   -- grid_vertices[i][j+1])
				// |     |
				// v4 -- v3   (grid_vertices[i+1][j] -- grid_vertices[i+1][j+1])

				// 1つ目の三角形 (v1, v4, v3)
				vertices->push_back(grid_vertices[i][j]);         // v1
				vertices->push_back(grid_vertices[i+1][j]);       // v4
				vertices->push_back(grid_vertices[i+1][j+1]);     // v3

				// 2つ目の三角形 (v1, v3, v2)
				vertices->push_back(grid_vertices[i][j]);         // v1
				vertices->push_back(grid_vertices[i+1][j+1]);     // v3
				vertices->push_back(grid_vertices[i][j+1]);       // v2
			}
		}

		// 3. 南極周囲の三角形
		// 南極の頂点は grid_vertices[stacks][0] を使用
		for (int j = 0; j < slices; ++j) {
			// v0: 南極
			// v1: 最後の緯度帯の現在の頂点 (i=stacks-1, j)
			// v2: 最後の緯度帯の次の頂点 (i=stacks-1, j+1)
			vertices->push_back(grid_vertices[stacks][0]); // 南極

			// 最後の緯度帯の頂点は grid_vertices[stacks-1][j] と grid_vertices[stacks-1][j+1] を参照します
			vertices->push_back(grid_vertices[stacks-1][j]);       // 現在の頂点 (反時計回りの場合)
			vertices->push_back(grid_vertices[stacks-1][j+1]);     // 次の頂点
		}

		LOGD("Sphere generated with %zu vertices.\n", vertices->size());
	}
};