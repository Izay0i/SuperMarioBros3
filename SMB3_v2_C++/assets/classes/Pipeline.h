#pragma once

//Render pipeline or graphics pipeline, AIO
class Pipeline {
private:
	static Pipeline* _pipelineInstance;

	ID3D10Effect* _effect;
	ID3D10EffectTechnique* _effectTechnique;
	ID3D10EffectShaderResourceVariable* _effectSRV;
	IDXGISwapChain* _swapChain;
	ID3D10InputLayout* _inputLayout;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10RasterizerState* _rasterizerState;
	ID3D10BlendState* _blendState;

	Pipeline();
	~Pipeline();

public:
	static Pipeline* GetInstance();

	ID3D10EffectTechnique* GetEffectTechnique() const;
	ID3D10EffectShaderResourceVariable* GetEffectSRV() const;
	IDXGISwapChain* GetSwapChain() const;
	ID3D10InputLayout* GetInputLayout() const;
	ID3D10RenderTargetView* GetRenderTargetView() const;
	ID3D10RasterizerState* GetRasterizerState() const;
	ID3D10BlendState* GetBlendState() const;

	//.fx file path
	//Expects main as technique name
	bool LoadEffect(std::string);

	//Content window, back buffer width, height, frame rate (default is 60)
	bool CreateDeviceAndSwapChain(HWND, unsigned int, unsigned int, unsigned int = 60);
	//Input layout, number of elements
	bool CreateInputLayout(D3D10_INPUT_ELEMENT_DESC*, unsigned int);
	bool CreateRenderTagetView();
	//Back buffer width, height
	bool CreateViewport(unsigned int, unsigned int);
	bool CreateRasterizerState();
	bool CreateBlendState();
	//CreateFont() clashes with CreateFont macro
	//Font name, font size, bold style, italic style
	bool PipCreateFont(std::string, int, bool = false, bool = false);

	void Release();
};