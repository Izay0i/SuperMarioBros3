#include "../GlobalUtil.h"
#include "Shader.h"

#include <d3dcompiler.h>

ID3D10InputLayout* Shader::GetInputLayout() const {
	return _inputLayout;
}

ID3D10VertexShader* Shader::GetVertexShader() const {
	return _vertexShader;
}

ID3D10PixelShader* Shader::GetPixelShader() const {
	return _pixelShader;
}

bool Shader::LoadVertexShaderCSO(std::string filePath) {
	HRESULT hResult = D3DReadFileToBlob(
		GlobalUtil::ToLPCWSTR(filePath), 
		&_vertexShaderBuffer
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SHADER] Failed to read file to blob\n");
		return false;
	}

	hResult = GlobalUtil::directDevice->CreateVertexShader(
		_vertexShaderBuffer->GetBufferPointer(), 
		_vertexShaderBuffer->GetBufferSize(), 
		&_vertexShader
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SHADER] Failed to create vertex shader\n");
		return false;
	}

	return true;
}

bool Shader::LoadPixelShaderCSO(std::string filePath) {
	HRESULT hResult = D3DReadFileToBlob(
		GlobalUtil::ToLPCWSTR(filePath),
		&_pixelShaderBuffer
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SHADER] Failed to read file to blob\n");
		return false;
	}

	hResult = GlobalUtil::directDevice->CreatePixelShader(
		_pixelShaderBuffer->GetBufferPointer(),
		_pixelShaderBuffer->GetBufferSize(),
		&_pixelShader
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SHADER] Failed to create pixel shader\n");
		return false;
	}

	return true;
}

bool Shader::CreateInputLayout() {
	D3D10_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	const unsigned int numElements = sizeof(layout) / sizeof(layout[0]);

	HRESULT hResult = GlobalUtil::directDevice->CreateInputLayout(
		layout, 
		numElements, 
		_vertexShaderBuffer->GetBufferPointer(), 
		_vertexShaderBuffer->GetBufferSize(), 
		&_inputLayout
	);
	if (FAILED(hResult)) {
		OutputDebugStringA("[SHADER] Failed to create input layout\n");
		return false;
	}

	return true;
}

void Shader::Release() {
	_vertexShaderBuffer->Release();
	_vertexShader->Release();
	_pixelShaderBuffer->Release();
	_pixelShader->Release();
	_inputLayout->Release();
}