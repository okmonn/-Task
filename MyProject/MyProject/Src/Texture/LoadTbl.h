#pragma once
#include <DirectXTex.h>
#include <string>
#include <map>
#include <functional>

namespace tex {
	// “Ç‚İ‚İƒe[ƒuƒ‹
	std::map<std::string,
		std::function<long(const std::wstring& fileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img)>>loadTbl = {
			{"bmp", [](const std::wstring& FileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromWICFile(FileName.c_str(), 0, meta, *img); }},
		    {"png", [](const std::wstring& FileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromWICFile(FileName.c_str(), 0, meta, *img); }},
		    {"jpg", [](const std::wstring& FileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromWICFile(FileName.c_str(), 0, meta, *img); }},
			{"sph", [](const std::wstring& FileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromWICFile(FileName.c_str(), 0, meta, *img); }},
			{"spa", [](const std::wstring& FileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromWICFile(FileName.c_str(), 0, meta, *img); }},
			{"tga", [](const std::wstring& fileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromTGAFile(fileName.c_str(), meta, *img); }},
			{"dds", [](const std::wstring& fileName, DirectX::TexMetadata* meta, DirectX::ScratchImage* img) -> HRESULT {
						return DirectX::LoadFromDDSFile(fileName.c_str(), 0, meta, *img); }}
	};
}

