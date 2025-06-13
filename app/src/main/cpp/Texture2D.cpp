#include "framework.h"
#include "Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture2D* Texture2D::CreateFromFile(const char* file)
{
	Texture2D* result = nullptr;
	result = new Texture2D;
	if (result == nullptr)
	{
		return nullptr;
	}
	if (!result->LoadFile(file))
	{
		return nullptr;
	}
	return result;
}

bool Texture2D::LoadFile(const char* file)
{
//	AAsset* asset = AAssetManager_open(assetManager, "texture.png", AASSET_MODE_BUFFER);
//	off_t length = AAsset_getLength(asset);
//	unsigned char* buffer = new unsigned char[length];
//	AAsset_read(asset, buffer, length);

	int w, h, bpp;
	//todo:stbi_set_flip_vertically_on_load(true);
	uint8_t* data = stbi_load(file, &w, &h, &bpp, STBI_rgb_alpha);
	LOGD("stbi_load(\"%s\"):%dx%dx%d %p\n", file, w, h, bpp, data);
	if (!data)
	{
		return false;
	}
	if (id_ == 0)
	{
		TexStorage(GL_RGBA, w, h);
	}
	TexSubImage(w, h, GL_RGBA, data);
	stbi_image_free(data);
	return true;
}

bool Texture2D::SaveFile(const char* file)
{
#if 0
	auto ext = MyUtil::FindExtension(file);
	auto texelsize = GetByteCount(format_);
	auto comp = (texelsize == 4) ? STBI_rgb_alpha : ((texelsize == 1) ? STBI_grey : STBI_rgb);
	std::vector<uint8_t> data;
	if (!GetTexImage(&data))
	{
		return false;
	}
	if (ext == ".png")
	{
		auto pitch = width_ * texelsize;
		return stbi_write_png(file, width_, height_, comp, data.data(), pitch) == 0;
	}
	else if (ext == ".tga")
	{
		return stbi_write_tga(file, width_, height_, comp, data.data()) == 0;
	}
	else if (ext == ".jpg")
	{
		return stbi_write_jpg(file, width_, height_, comp, data.data(), 100) == 0;
	}
	else
	{
		return stbi_write_bmp(file, width_, height_, comp, data.data()) == 0;
	}
#endif
	return false;
}

bool Texture2D::SaveFile(const wchar_t* file)
{
#if 0
	auto ext = MyUtil::PathW(file).FindExtension().wstring();
	auto name = MyUtil::U16ToString(file);
	auto texelsize = GetByteCount(format_);
	auto comp = (texelsize == 4) ? STBI_rgb_alpha : ((texelsize == 1) ? STBI_grey : STBI_rgb);
	std::vector<uint8_t> data;
	if (!GetTexImage(&data))
	{
		return false;
	}
	if (ext == L".png")
	{
		auto pitch = width_ * GetByteCount(format_);
		return stbi_write_png(name.c_str(), width_, height_, comp, data.data(), pitch) == 0;
	}
	else if (ext == L".tga")
	{
		return stbi_write_tga(name.c_str(), width_, height_, comp, data.data()) == 0;
	}
	else if (ext == L".jpg")
	{
		return stbi_write_jpg(name.c_str(), width_, height_, comp, data.data(), 100) == 0;
	}
	else
	{
		return stbi_write_bmp(name.c_str(), width_, height_, comp, data.data()) == 0;
	}
#endif
	return false;
}

bool Texture2D::GetTexImage(std::vector<uint8_t>* image)
{
#if 0
	assert(format_ == GL_RED || format_ == GL_RGB || format_ == GL_RGBA || format_ == GL_RGBA8);
	auto texelsize = GetByteCount(format_);
	assert(image);
	image->resize(width_ * height_ * texelsize);
	glBindTexture(GL_TEXTURE_2D, id_);
	glGetTexImage(GL_TEXTURE_2D, 0, format_, GL_UNSIGNED_BYTE, image->data());
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
#endif
	return false;
}

