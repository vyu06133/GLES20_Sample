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
	VertexPNC(const VertexP& P) : VertexP(P), normal(0.0f), color(1.0f)	{}
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

struct VertexPNCT : public VertexPNC
{
public:
	vec2 texcoord;
	VertexPNCT() : VertexPNC(), texcoord(0.0f)	{}
	VertexPNCT(const VertexPNC& PNC) : VertexPNC(PNC), texcoord(0.0f)	{}
	VertexPNCT(const vec3& P, const vec3& N, const vec4& C, const vec2& T) : VertexPNC(P, N, C), texcoord(T)	{}
	VertexPNCT(const float& PX, const float& PY, const float& PZ,
	          const float& NX, const float& NY, const float& NZ,
	          const float& R, const float& G, const float& B, const float& A,
	          const float& U, const float& V)
		: VertexPNC(PX, PY, PZ, NX, NY, NZ, R, G, B, A), texcoord(U, V) {}

	inline static INPUT_ELEMENT_DESC InputLayout[] = {
			{ "a_position", 0, 3, GL_FLOAT },
			{ "a_normal", 1, 3, GL_FLOAT },
			{ "a_color", 2, 4, GL_FLOAT },
			{ "a_texcoord", 3, 2, GL_FLOAT },
			{}
	};
};

struct Geometry
{
	static inline void GenerateCheckerPlaneZX(std::vector<VertexPNCT>* vertices, const vec3& size, const vec3& grid, const vec4& color0, const vec4& color1)
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
			float v1 = static_cast<float>(j + 0) / grid.z;
			float v2 = static_cast<float>(j + 1) / grid.z;
			for (int32_t i = 0; i < gridx; i++)
			{
				float x1 = -size.x * 0.5f + wX * static_cast<float>(i);
				float x2 = x1 + wX;
				float u1 = static_cast<float>(i + 0) / grid.x;
				float u2 = static_cast<float>(i + 1) / grid.x;
				auto n = vec3(0.0f, 1.0f, 0.0f);

				auto v11 = VertexPNCT(vec3(x1, 0.0f, z1), n, color[(i + j) & 1], vec2(u1, v1));
				auto v12 = VertexPNCT(vec3(x2, 0.0f, z1), n, color[(i + j) & 1], vec2(u2, v1));
				auto v21 = VertexPNCT(vec3(x1, 0.0f, z2), n, color[(i + j) & 1], vec2(u1, v2));
				auto v22 = VertexPNCT(vec3(x2, 0.0f, z2), n, color[(i + j) & 1], vec2(u2, v2));

				vertices->push_back(v11);
				vertices->push_back(v12);
				vertices->push_back(v22);
				vertices->push_back(v11);
				vertices->push_back(v22);
				vertices->push_back(v21);
			}
		}
	}

	static inline void GenarateCube(std::vector<VertexPNCT>* vertices, const vec3& size)
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

		static const vec2 texCoords[4] =
				{
						{ 1.0f, 0.0f },
						{ 1.0f, 1.0f },
						{ 0.0f, 1.0f },
						{ 0.0f, 0.0f },
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
			auto v0 = VertexPNCT((normal - side1 - side2) * tsize, normal, col, texCoords[0]);
			auto v1 = VertexPNCT((normal - side1 + side2) * tsize, normal, col, texCoords[1]);
			auto v2 = VertexPNCT((normal + side1 + side2) * tsize, normal, col, texCoords[2]);
			auto v3 = VertexPNCT((normal + side1 - side2) * tsize, normal, col, texCoords[3]);

			(*vertices).push_back(v0);
			(*vertices).push_back(v1);
			(*vertices).push_back(v2);
			(*vertices).push_back(v0);
			(*vertices).push_back(v2);
			(*vertices).push_back(v3);
		}
	}

// --- 修正された球体生成関数 (VertexPNCT を使用) ---
	static inline void GenerateSphere(std::vector<VertexPNCT>* vertices, float radius, int slices, int stacks)
	{
		// 既存のデータをクリア
		vertices->clear();

		if (slices == 0 || stacks < 2)
		{
			LOGE("Warning: slices = 0 or stacks < 2. No sphere generated.\n");
			return;
		}

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
		// grid_vertices を VertexPNCT 型に変更
		std::vector<std::vector<VertexPNCT>> grid_vertices(stacks + 1, std::vector<VertexPNCT>(slices + 1));

		// 北極と南極は、テクスチャUVが特殊なケース
		// 北極のテクスチャV座標は1.0、南極のテクスチャV座標は0.0
		// U座標は、極点に集約されるため、複数の値を持つことがありますが、
		// ここでは便宜上、0.0としておきます。実際の描画では極点のテクスチャは歪む可能性があります。

		// 北極の計算 (i=0 のスタック)
		// 北極は単一の点ですが、各スライスに対応するUVを生成して格納しておくと、
		// 下のループで使いやすくなります。
		// 極点ではU座標は意味を持ちませんが、Vは1.0です。
		// ただし、後で三角形を生成する際、grid_vertices[0][0] のように単一点を参照するため、
		// ここでループして複数の頂点を作る必要はありません。
		// 単一の北極点を作成し、テクスチャ座標も便宜上設定します。
		// U座標は通常、経度に比例しますが、極点ではすべての経度が一点に収束するため、
		// テクスチャをスムーズに貼るには工夫が必要です。
		// 簡単のため、北極点自体のUは0.0とし、周囲の三角形で補正します。
		grid_vertices[0][0] = VertexPNCT(
				vec3(0.0f, 0.0f, radius),
				vec3(0.0f, 0.0f, 1.0f),
				vec4(0.0f, 0.0f, 1.0f, 1.0f), // 青色
				vec2(0.0f, 1.0f) // Uは0.0 (便宜上), Vは1.0 (北極)
		);

		// 中間の緯度帯の頂点 (i=1 から stacks-1 まで)
		for (int i = 1; i < stacks; i++)
		{
			float v_coord = 1.0f - ((float)i / stacks); // V座標: 1.0 (北極側) から 0.0 (南極側) へ
			for (int j = 0; j <= slices; j++) // j=0 から slices まで (一周分の重複)
			{
				float x = cost1[j] * sint2[i];
				float y = sint1[j] * sint2[i];
				float z = cost2[i];

				float u_coord = (float)j / slices; // U座標: 0.0 から 1.0 へ (経度に比例)

				grid_vertices[i][j] = VertexPNCT(
						vec3(x * radius, y * radius, z * radius), // Position
						vec3(x, y, z),                           // Normal (単位球の座標がそのまま法線になる)
						vec4(MyMath::Abs(x), MyMath::Abs(y), MyMath::Abs(z), 1.0f), // Color
						vec2(u_coord, v_coord) // テクスチャ座標
				);
			}
		}

		// 南極の計算 (i=stacks のスタック)
		// 南極点自体のUは0.0 (便宜上), Vは0.0 (南極)
		grid_vertices[stacks][0] = VertexPNCT(
				vec3(0.0f, 0.0f, -radius),
				vec3(0.0f, 0.0f, -1.0f),
				vec4(0.0f, 0.0f, 1.0f, 1.0f), // 青色
				vec2(0.0f, 0.0f) // Uは0.0 (便宜上), Vは0.0 (南極)
		);


		// --- TRIANGLES として頂点を直接追加 ---

		// 1. 北極周囲の三角形
		// 北極の頂点 (grid_vertices[0][0]) を各三角形で使用
		for (int j = 0; j < slices; ++j)
		{
			// 北極点のデータは grid_vertices[0][0]
			// しかし、テクスチャ座標のU値は、次の2点に合わせて個別に設定する必要があります
			// ここでは、各三角形のために新しいVertexPNCTオブジェクトを作成し、UVを調整します。

			// 最初の頂点 (北極)
			// U座標は、この三角形の2つの隣接頂点の中間点や平均値にすると良いかもしれません。
			// または、極点ではUがどこでも良いという性質を利用し、テクスチャ座標の「縫い目」を目立たなくする工夫が必要です。
			// シンプルなアプローチとして、各Uスライスに合わせた北極点を作ります。
			// 複雑さを避けるため、今回は一旦単純にV=1.0でUを隣接頂点と合わせます。
			vertices->push_back(VertexPNCT(
					grid_vertices[0][0].pos,
					grid_vertices[0][0].normal,
					grid_vertices[0][0].color,
					vec2((grid_vertices[1][j].texcoord.x + grid_vertices[1][j+1].texcoord.x) / 2.0f, // U座標は周囲の頂点の中間
					1.0f) // V座標は1.0 (北極)
			));

			// 2番目の頂点 (最初の緯度帯の次の頂点)
			vertices->push_back(grid_vertices[1][j+1]);

			// 3番目の頂点 (最初の緯度帯の現在の頂点)
			vertices->push_back(grid_vertices[1][j]);
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
		// 南極の頂点 (grid_vertices[stacks][0]) を各三角形で使用
		for (int j = 0; j < slices; ++j) {
			// 南極点のデータは grid_vertices[stacks][0]
			// 北極と同様に、テクスチャUVを調整した南極点を作成します。
			vertices->push_back(VertexPNCT(
					grid_vertices[stacks][0].pos,
					grid_vertices[stacks][0].normal,
					grid_vertices[stacks][0].color,
					vec2((grid_vertices[stacks-1][j].texcoord.x + grid_vertices[stacks-1][j+1].texcoord.x) / 2.0f, // U座標は周囲の頂点の中間
					0.0f) // V座標は0.0 (南極)
			));

			// 2番目の頂点 (最後の緯度帯の現在の頂点)
			vertices->push_back(grid_vertices[stacks-1][j]);

			// 3番目の頂点 (最後の緯度帯の次の頂点)
			vertices->push_back(grid_vertices[stacks-1][j+1]);
		}

		LOGD("Sphere generated with %zu vertices.\n", vertices->size());
	}
};