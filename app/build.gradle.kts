plugins {
	alias(libs.plugins.android.application)
	alias(libs.plugins.kotlin.android)
}

android {
	namespace = "com.example.gles20_sample"
	compileSdk = 33
	
	defaultConfig {
		applicationId = "com.example.gles20_sample"
		minSdk = 21
		targetSdk = 33
		versionCode = 1
		versionName = "1.0"
		
		testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
		externalNativeBuild {
			cmake {
				cppFlags += "-std=c++17"
			}
		}
	}
	
	buildTypes {
		release {
			isMinifyEnabled = false
			proguardFiles(
				getDefaultProguardFile("proguard-android-optimize.txt"),
				"proguard-rules.pro"
			)
		}
		debug {
			// ネイティブデバッグを有効にする
			// debuggable を true に設定することで、NDK ビルドがデバッグシンボルを含んだ共有ライブラリを生成する
			isDebuggable = true
			isJniDebuggable = true // これも追加しておくとより確実
		}
	}
	compileOptions {
		sourceCompatibility = JavaVersion.VERSION_11
		targetCompatibility = JavaVersion.VERSION_11
	}
	kotlinOptions {
		jvmTarget = "11"
	}
	externalNativeBuild {
		cmake {
			path = file("src/main/cpp/CMakeLists.txt")
			version = "3.22.1"
		}
	}
	buildFeatures {
		viewBinding = true
	}
	ndkVersion = "27.0.12077973"
}

dependencies {
	implementation("com.google.android.material:material:1.9.0")
	implementation("androidx.appcompat:appcompat:1.6.1")
	implementation("androidx.core:core-ktx:1.10.0")
	implementation("androidx.constraintlayout:constraintlayout:2.1.4")
	implementation("androidx.transition:transition:1.4.1")
	implementation("androidx.activity:activity:1.7.2")
	implementation("androidx.profileinstaller:profileinstaller:1.3.1")
}


