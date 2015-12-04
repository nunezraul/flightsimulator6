# include "curve_eval.h"

int factorial(int n)
{
	if (n <= 1)
		return(1);
	else
		n = n*factorial(n - 1);
	return n;
}

float binomial_coff(float n, float k)
{
	float ans;
	ans = factorial(n) / (factorial(k)*factorial(n - k));
	return ans;
}

GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts) {
	GsVec P;
	P.x = 0; P.y = 0;
	int size = ctrlpnts.size();
	for (int i = 0; i<size; i++)
	{
		P.x = P.x + binomial_coff((float)(size-1), (float)i) * pow(t, (double)i) * pow((1 - t), (size-1 - i)) * ctrlpnts[i].x;
		P.y = P.y + binomial_coff((float)(size-1), (float)i) * pow(t, (double)i) * pow((1 - t), (size-1 - i)) * ctrlpnts[i].y;
	}
	return P;
}

GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpntsn) {
	GsVec P(0.0f, 0.0f, 0.0f);
	float L;
	float _s= ctrlpntsn.size();
	GsArray<float> arr;
	for (int i = 0; i < ctrlpntsn.size(); i++){
		arr.push((i*1.f) / (float)(ctrlpntsn.size() - 1));
	}
	for (int i = 0; i < _s; i++) {	//Epsilon
		L = 1;
		for (int j = 0; j < _s; j++) {	//Pi
			if (j != i)
				L *= ((t - arr[j]) / (arr[i] - arr[j]));
		}
		P += ctrlpntsn[i] * L;
	}
	return P;
}

GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts) {
	GsVec P;
	for (int i = 0; i < pnts.size(); i++) {
		P += pnts[i] * N(i, k, t);
	}
	return P;
}

float N(int i, int k, float u) {
	float ui = float(i);
	if (k == 1)
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	else
		return ((u - ui) / (k - 1))*N(i, k - 1, u) + ((ui + k - u) / (k - 1)) * N(i + 1, k - 1, u);
}

GsVec crspline(float t, const const GsArray<GsVec>& pnts) {
	GsVec P = 0.5*((2 * pnts[1]) + (-pnts[0] + pnts[2])*t + (2 * pnts[0] - 5 * pnts[1] + 4 * pnts[2] - pnts[3])*t*t + (-pnts[0] + 3 * pnts[1] - 3 * pnts[2] + pnts[3])*t*t*t);
	return P;
}

GsVec bospline(float t, const GsArray<GsVec>& pnts) {
	//GsVec P;
	//Calculate two points for point 1
	float d1 = dist(pnts[1], pnts[0]);
	float d2 = dist(pnts[2], pnts[1]);
	GsVec v1 = (pnts[1] - pnts[0]) / d1;
	GsVec v2 = (pnts[2] - pnts[1]) / d2;
	GsVec vi = (d2*v1 + d1*v2) / (d1 + d2);

	//GsVec P1 = pnts[1] - (1.0f / 3.0f)*d1*vi;
	GsVec P2 = pnts[1] + (1.0f / 3.0f)*d2*vi;

	//Calculate two points for point 2
	float d3 = dist(pnts[3], pnts[2]);
	GsVec v3 = (pnts[3] - pnts[2]) / d3;
	GsVec vi2 = (d3*v2 + d2*v3) / (d2 + d3);
	GsVec P3 = pnts[2] - (1.0f / 3.0f)*d2*vi2;
	//GsVec P4 = pnts[2] + (1.0f / 3.0f)*d3*vi2;

	GsArray<GsVec> bez; bez.push() = pnts[1]; bez.push() = P2; bez.push() = P3; bez.push() = pnts[2];
	return eval_bezier(t, bez);
}