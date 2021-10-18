#pragma once

struct Vertex {
	D3DXVECTOR2 pos;
};

class Shader {
private:
	ID3D10InputLayout* _inputLayout;

	ID3D10VertexShader* _vertexShader;
	ID3D10PixelShader* _pixelShader;

	ID3D10Blob* _vertexShaderBuffer;
	ID3D10Blob* _pixelShaderBuffer;

public:
	ID3D10InputLayout* GetInputLayout() const;

	ID3D10VertexShader* GetVertexShader() const;
	ID3D10PixelShader* GetPixelShader() const;

	//Shader file path (.cso)
	bool LoadVertexShaderCSO(std::string);
	bool LoadPixelShaderCSO(std::string);
	
	bool CreateInputLayout();

	void Release();
};