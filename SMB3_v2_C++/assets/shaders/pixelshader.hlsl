float4 PS(PS_INPUT input) : SV_Target
{
	float4 result = float4(1, 1, 1, 0);
	if (input.Col) {
		clip(result);
	}
	return result;
}