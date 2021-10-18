#pragma once

class Pipeline {
private:
	ID3D10Effect* _effect;
	ID3D10EffectTechnique* _effectTechnique;
	ID3D10EffectShaderResourceVariable* _effectSRV;
	IDXGISwapChain* _swapChain;
	ID3D10InputLayout* _inputLayout;
	ID3D10RenderTargetView* _renderTargetView;
	ID3D10RasterizerState* _rasterizerState;
	ID3D10BlendState* _blendState;

public:
	ID3D10EffectShaderResourceVariable* GetEffectSRV() const;
	ID3D10EffectTechnique* GetEffectTechnique() const;
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

	void Release();
};

//It's so nice to learn that the framework I built upon is all legacy code
//Really helps when I'm stuck with a problem that only a website from 200X could fix
//Oh wait, that website is gone. Perfect