//
// Created by vyu06133 on 2025/06/09 月.
//

#pragma once

struct MyMath
{
	static inline const float _PAI = 355.0f / 113.0f;// (3.14159265358979323846264338f);
	static inline const float _E = 1.0e-6f;
	static inline const float F32_MAX = 3.402823466e+38f;
	static inline const float F32_MINP = 1.175494351e-38f;

	static inline float NormalizePAI(const float &x)
	{
		// ループで正規化する
		float d = x;
		// 180以上なら360を減算（180はアリ）
		while (d > _PAI)
		{
			d -= _PAI * 2.0f;
		}
		// -180未満なら360を加算（-180はナシ）
		while (d < -_PAI)
		{
			d += _PAI * 2.0f;
		}
		return d;
	}

	static inline float Normalize180(const float &x)
	{
		// ループで正規化する
		float d = x;
		// 180以上なら360を減算（180はアリ）
		while (d > 180.0f)
		{
			d -= 360.0f;
		}
		// -180未満なら360を加算（-180はナシ）
		while (d < -180.0f)
		{
			d += 360.0f;
		}
		return d;
	}

	static inline float Round(const float& x)
	{
		return std::round(x);
	}

	static inline float Ceil(const float& x)
	{
		return std::ceil(x);
	}

	static inline float Floor(const float& x)
	{
		return std::floor(x);
	}

	static inline float Sqrt(const float& value)
	{
		// 公式
		// X(n+1) = ( X(n) + X(1) / X(n) ) / 2	… n > 1
		assert(value >= 0.0f);
		float s(value), t;
		do {
			t = s;
			s = (t + value / t) * 0.5f;
		} while (s < t);
		return t;
	}

	static inline float Cos(const float& rad)
	{
		// テイラー展開
		//              x²   x⁴   x⁶   x⁸
		// cos(x) = 1 - __ + __ - __ + __ ...
		//              2!   4!   6!   8!
		float x = NormalizePAI(rad);
		float xx = x * x;
		float xxxx = xx * xx;
		return 1.0f - ((1.0f / 2.0f) * xx) + ((1.0f / 24.0f) * xxxx) - ((1.0f / 720.0f) * xxxx * xx) + ((1.0f / 40320.0f) * xxxx * xxxx) - ((1.0f / 3628800.0f) * xxxx * xxxx * xx);
	}

	static inline float Sin(const float& rad)
	{
		// テイラー展開
		//              x³   x⁵   x⁷   x⁹
		// sin(x) = x - __ + __ - __ + __ ...
		//              3!   5!   7!   9!
		float x = NormalizePAI(rad);
		float xx = x * x;
		float xxx = xx * x;
		float xxxx = xxx * x;
		return x - ((1.0f / 6.0f) * xxx) + ((1.0f / 120.0f) * xxxx * x) - ((1.0f / 5040.0f) * xxxx * xxx) + ((1.0f / 362880.0f) * xxx * xxx * xxx) - ((1.0f / 39916800.0f) * xxxx * xxxx * xxx);
	}

	static inline float Tan(const float& rad)
	{
		// tan = sin / cos
		auto c = Cos(rad);
		return (Abs(c) < _E) ? F32_MAX : (Sin(rad) / c);
	}

	static inline float ArcTan(const float& tann)
	{
		bool neg(tann < 0.0f);
		float t(Abs(tann));
		bool inv(t > 1.0f);
		if (inv)
		{
			t = 1.0f / t;	// (π/4)以上はひっくり返して計算
		}
		float a = t * (12.0f * t * t + 45.0f) / (27.0f * t * t + 45.0f);
		if (inv)	a = _PAI * 0.5f - a;
		if (neg)	a = -a;
		return a;
	}

	static inline float ArcTan2(const float& y, const float& x)
	{
		if (x > 0.0f)
		{
			return ArcTan(y / x);
		}
		else
		{
			if (Abs(x)<_E)
			{
				return (y > 0.0f) ? _PAI * 0.5f : -_PAI * 0.5f;
			}
			if (y > 0.0f)
			{
				return ArcTan(y / x) + _PAI;
			}
			else
			{
				return ArcTan(y / x) - _PAI;
			}
		}
	}

	static inline float ArcCos(const float& cosn)
	{
		float x = Abs(cosn);
		float result = -0.0187293f;
		result = result * x;
		result = result + 0.0742610f;
		result = result * x;
		result = result - 0.2121144f;
		result = result * x;
		result = result + 1.5707288f;
		result = result * Sqrt(1.0f - x);
		if (cosn < 0)
		{
			result = result - 2.0f * result;
			return result + _PAI;
		}
		else
		{
			return result;
		}
	}

	static inline float ArcSin(const float& sinn)
	{
		return ArcTan2(sinn, Sqrt(1.0f - sinn * sinn));
	}

	static inline float Pow(const float& x, const float& n)
	{
		assert(n >= 0.0f);
		float	X(x);
		float	v = 1.0f;
		float	n_int = Floor(n);
		float	n_flt = n - n_int;

		// 整数部
		int32_t in = static_cast<int32_t>(n_int);
		while (in > 0)
		{
			v *= X;
			in -= 1;
		}

		// 小数部…0.5ずつ処理（ ∵ n^0.5 == sqrt(n) ）
		const static uint32_t fbit = 12;	// ←精度
		const static uint32_t fbias = (1 << fbit);
		const static uint32_t fmask = fbias - 1;
		uint32_t fn = static_cast<uint32_t>(n_flt * static_cast<float>(fbias));
		if (fn)
		{
			do {
				X = Sqrt(X);
				fn = (fn & (fmask - 1)) << 1;
				if ((fn & fbias) != 0)
					v *= X;
			} while (fn);
		}
		return v;
	}

	template<typename T = float>
	static inline T Interp(const T& s, const T& e, const float Alpha)
	{
		return s + (e - s) * Alpha;
	}
	template<typename T = float>
	static inline T EaseIn(const T& A, const T& B, float Alpha, float Exp)
	{
		float const PowAlpha = Pow(Alpha, Exp);
		return Interp<T>(A, B, PowAlpha);
	}

	template<typename T = float>
	static inline T EaseOut(const T& A, const T& B, float Alpha, float Exp)
	{
		float const PowAlpha = 1.0f - Pow(1.0f - Alpha, Exp);
		return Interp<T>(A, B, PowAlpha);
	}

	template<typename T = float>
	static inline T EaseInOut(const T& A, const T& B, float Alpha, float Exp)
	{
		float easedAlpha;
		if (Alpha < 0.5f)
		{
			easedAlpha = EaseIn(0.0f, 1.0f, Alpha * 2.0f, Exp) * 0.5f;
		}
		else
		{
			easedAlpha = EaseOut(0.0f, 1.0f, Alpha * 2.0f - 1.0f, Exp) * 0.5f + 0.5f;
		}
		return Interp<T>(A, B, easedAlpha);
	}

	static inline bool IsNAN(const float& value)
	{
		// IEEE754ではNANなら値比較が出来なくなる
		return value != value;
	}

	static inline float RadToDeg(const float &x)
	{
		// ラジアン→度数法
		return x * (180.0f / _PAI);
	}

	static inline float DegToRad(const float &x)
	{
		// 度数法→ラジアン
		return x * (_PAI / 180.0f);
	}

	template<typename T>
	static inline T Max(const T &s, const T &t)
	{
		return (s > t) ? s : t;
	}

	template<typename T>
	static inline T Min(const T &s, const T &t)
	{
		return (s < t) ? s : t;
	}

	template<typename T>
	static inline T Clamp(const T &value, const T &min, const T &max)
	{
		return Max(Min(value, max), min);
	}

	template<typename T>
	static inline T Abs(const T &value)
	{
		return (value < 0) ? -value : value;
	}

	struct Ray
	{
		vec3 pos;
		vec3 dir;

		Ray() noexcept: pos(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 1.0f) {}

		Ray(const vec3 &Pos, const vec3 &Dir) noexcept: pos(Pos), dir(Dir) {}
	};

	struct Plane : public glm::vec4
	{
		inline Plane() noexcept {}
		inline Plane(const glm::vec4 &V) noexcept: glm::vec4(V) {}
		inline Plane(const float &X, const float &Y, const float &Z, const float &W) : glm::vec4(X, Y, Z, W) {}
		inline Plane(const vec3 &Normal, const float &W) : vec4(Normal, W) {}
		inline Plane(const vec3 &Base, const vec3 &Normal)
		{
			x = Normal.x;
			y = Normal.y;
			z = Normal.z;
			w = -(Base.x * Normal.x + Base.y * Normal.y + Base.z * Normal.z);
		}
		inline Plane(const vec3 &A, const vec3 &B, const vec3 &C)
		{
			vec3 N = glm::normalize(glm::cross(A - B, A - C));
			w = -(N.x * A.x + N.y * A.y + N.z * A.z);
		}

		inline glm::vec3 GetNormal() const
		{
			return glm::vec3(x, y, z);
		}

		inline glm::vec3 GetBase() const
		{
			return glm::vec3(x * w, y * w, z * w);
		}
	};

	static inline bool RayPlaneIntersection(const Ray &ray, const MyMath::Plane &plane, vec3 *result)
	{
		const auto planeNormal = plane.GetNormal();
		const auto planeOrigin = plane.GetBase();

		const float denom = glm::dot(ray.dir, planeNormal);
		if (Abs(denom) < _E)
		{
			return false;
		}
		const float distance = glm::dot((planeOrigin - ray.pos), planeNormal) / denom;
		*result = ray.pos + ray.dir * distance;
		return true;
	}

	static inline float CalcDistPointAndPlane(const vec3 &pt, const Plane &plane)
	{
		vec3 n = plane.GetNormal();
		vec3 b = plane.GetBase();
		return Abs(glm::dot(n, pt - b));
	}

	static inline float CalcDistPointAndPlane(const vec3 &pt, const vec3 &v0, const vec3 &v1, const vec3 &v2)
	{
		vec3 n = glm::normalize(glm::cross(v1 - v0, v2 - v1));
		vec3 g = (v0 + v1 + v2) / 3.0f;
		return Abs(glm::dot(n, pt - g));
	}

	struct MersenneTwister
	{
		/* Period parameters */
		static const int32_t MT_N_ = 624;
		static const int32_t MT_M_ = 397;
		static const uint32_t MATRIX_A_ = 0x9908b0dful;
		static const uint32_t UPPER_MASK_ = 0x80000000ul;
		static const uint32_t LOWER_MASK_ = 0x7ffffffful;

		uint32_t mt_[MT_N_] = { 0 }; /* the array for the state vector  */
		int32_t mti_ = MT_N_ + 1; /* mti_==MT_N_+1 means mt_[MT_N_] is not initialized */

		MersenneTwister() {}
		MersenneTwister(uint32_t seed) { Init(seed); }
		MersenneTwister(const MersenneTwister& rhs)
		{
			std::copy(std::begin(rhs.mt_), std::end(rhs.mt_), std::begin(mt_));
			mti_ = rhs.mti_;
		}

		void Save(std::vector<uint32_t>* info, int32_t* index) const
		{
			assert(info);
			assert(index);
			info->clear();
			info->reserve(sizeof(mt_)/sizeof(mt_[0]));
			for (auto i : mt_)
			{
				info->push_back(i);
			}
			*index = mti_;
		}

		bool Restore(const std::vector<uint32_t> info, int32_t index)
		{
			if (info.size() != MT_N_)
			{
				return false;
			}
			for (int32_t i = 0; i < MT_N_; i++)
			{
				mt_[i] = info[i];
			}
			mti_ = index;
			return true;
		}

		/* initializes mt_[MT_N_] with a seed */
		void Init(uint32_t seed)
		{
			mt_[0] = seed & 0xfffffffful;
			for (mti_ = 1; mti_ < MT_N_; mti_++)
			{
				mt_[mti_] = (1812433253ul * (mt_[mti_ - 1] ^ (mt_[mti_ - 1] >> 30)) + mti_);
				mt_[mti_] &= 0xfffffffful;
			}
		}

		void InitByArray(uint32_t init_key[], int key_length)
		{
			int32_t i, j, k;
			Init(19650218ul);
			i = 1; j = 0;
			k = (MT_N_ > key_length ? MT_N_ : key_length);
			for (; k; k--) {
				mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525ul))
				         + init_key[j] + j; /* non linear */
				mt_[i] &= 0xfffffffful; /* for WORDSIZE > 32 machines */
				i++; j++;
				if (i >= MT_N_) { mt_[0] = mt_[MT_N_ - 1]; i = 1; }
				if (j >= key_length) j = 0;
			}
			for (k = MT_N_ - 1; k; k--) {
				mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941ul)) - i; /* non linear */
				mt_[i] &= 0xfffffffful; /* for WORDSIZE > 32 machines */
				i++;
				if (i >= MT_N_) { mt_[0] = mt_[MT_N_ - 1]; i = 1; }
			}

			mt_[0] = 0x80000000ul; /* MSB is 1; assuring non-zero initial array */
		}

		/// <summary>
		/// 0～0xffffffffのuint32_t乱数
		/// </summary>
		/// <returns>値</returns>
		uint32_t randu(void)
		{
			uint32_t y;
			static uint32_t mag01[2] = { 0x0ul, MATRIX_A_ };

			if (mti_ >= MT_N_)
			{ /* generate N words at one time */
				int32_t kk;

				if (mti_ == MT_N_ + 1)   /* if init_genrand() has not been called, */
				{
					Init(5489ul); /* a default initial seed is used */
				}

				for (kk = 0; kk < MT_N_ - MT_M_; kk++)
				{
					y = (mt_[kk] & UPPER_MASK_) | (mt_[kk + 1] & LOWER_MASK_);
					mt_[kk] = mt_[kk + MT_M_] ^ (y >> 1) ^ mag01[y & 0x1u];
				}
				for (; kk < MT_N_ - 1; kk++)
				{
					y = (mt_[kk] & UPPER_MASK_) | (mt_[kk + 1] & LOWER_MASK_);
					mt_[kk] = mt_[kk + (MT_M_ - MT_N_)] ^ (y >> 1) ^ mag01[y & 0x1u];
				}
				y = (mt_[MT_N_ - 1] & UPPER_MASK_) | (mt_[0] & LOWER_MASK_);
				mt_[MT_N_ - 1] = mt_[MT_M_ - 1] ^ (y >> 1) ^ mag01[y & 0x1u];

				mti_ = 0;
			}

			y = mt_[mti_++];

			/* Tempering */
			y ^= (y >> 11);
			y ^= (y << 7) & 0x9d2c5680ul;
			y ^= (y << 15) & 0xefc60000ul;
			y ^= (y >> 18);

			return y;
		}

		/// <summary>
		/// 0～0x7fffffffのint32_t乱数
		/// </summary>
		/// <param name=""></param>
		/// <returns>値</returns>
		int32_t randi(void)
		{
			uint32_t t = randu() & 0x7fffffffu;
			return *reinterpret_cast<int32_t*>(&t);
		}

		/// <summary>
		/// 0～255のBYTE乱数
		/// </summary>
		/// <param name=""></param>
		/// <returns>値</returns>
		uint8_t randb(void)
		{
			return static_cast<uint32_t>(randu() & 0x00ffu);
		}

		/// <summary>
		/// 0.0f～1.0fのfloat乱数
		/// </summary>
		/// <returns>値</returns>
		float randf(void)
		{
			return static_cast<float>(randu() & 0x00ffffffu) / 16777215.0f;	// 24ビット整数の乱数→浮動小数点数へ
		}

		double randd(void)
		{
			uint32_t a = randu() >> 5;
			uint32_t b = randu() >> 6;
			return (static_cast<double>(a) * 67108864.0 + static_cast<double>(b)) * (1.0 / 9007199254740992.0);
		}

		int32_t RandRangei(int32_t low, int32_t high)
		{
			auto range = Abs(high - low);
			assert(range != 0);
			return Clamp((randi() % range) + Min(low, high), low, high);
		}

		float RandRangef(float low, float high)
		{
			return Clamp(randf() * Abs(high - low) + Min(low, high), low, high);
		}

		inline vec3 RandRangeVec(const vec3& low, const vec3& high)
		{
			return vec3(
					RandRangef(low.x, high.x),
					RandRangef(low.y, high.y),
					RandRangef(low.z, high.z));
		}
	};
	static inline MersenneTwister mt;
};
