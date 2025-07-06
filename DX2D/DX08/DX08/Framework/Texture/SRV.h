#pragma once
class SRV
{
public:
	SRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	SRV(wstring file);
	~SRV();

	void Set(int slot = 0);

	Vector2 GetImageSize();

	const ComPtr<ID3D11ShaderResourceView> GetSRV_COM() { return _srv; }

private:
	Vector2 _size;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv; // SRV -> 판박이 만드는 아저씨
};

