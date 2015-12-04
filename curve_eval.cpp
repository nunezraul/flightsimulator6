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

GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts)
{
	GsVec P;
	P.x = 0; P.y = 0;
	int clicks = ctrlpnts.size();
	for (int i = 0; i<clicks; i++)
	{
		P.x = P.x + binomial_coff((float)(clicks - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (clicks - 1 - i)) * ctrlpnts[i].x;
		P.y = P.y + binomial_coff((float)(clicks - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (clicks - 1 - i)) * ctrlpnts[i].y;
	}
	//cout<<P.x<<endl<<P.y;
	//cout<<endl<<endl;
	return P;
}



GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts)
{
	int clicks = ctrlpnts.size();
	float l;
	GsVec P;
	for (float i = 0; i < clicks; i++)
	{
		l = 1;
		for (float j = 0; j < clicks; j++)
		{
			if (j != i)
			{
				l *= (t - (j / (clicks - 1))) / ((i / (clicks - 1)) - (j / (clicks - 1)));
			}
		}
		P += ctrlpnts[(int)i] * (l);
	}

	return P;
}

GsVec crspline(float t, const GsArray<GsVec>& pnts)
{
	GsVec P;
	P = 0.5 * ((2 * pnts[1]) + (-pnts[0] + pnts[2]) * t + (2 * pnts[0] - 5 * pnts[1] + 4 * pnts[2] - pnts[3]) * (t*t) + (-pnts[0] + 3 * pnts[1] - 3 * pnts[2] + pnts[3]) * (t*t*t));
	return P;
}

GsVec bospline(float t, const GsArray<GsVec>& pnts)
{
	GsVec vpl, vmi, vmi2, vi, vi2, pmi, pmi2, ppl, ppl2;
	float dimi, dipl, di2;
	dipl = dist(pnts[2], pnts[1]);
	dimi = dist(pnts[1], pnts[0]);
	di2 = dist(pnts[3], pnts[2]);
	vpl = (pnts[2] - pnts[1]) / (dipl);
	vmi = (pnts[1] - pnts[0]) / (dimi);
	vmi2 = (pnts[3] - pnts[2]) / di2;
	vi = (dipl*vmi + dimi*vpl) / (dimi + dipl);
	vi2 = (di2 * vpl + dipl * vmi2) / (dipl + di2);
	pmi = pnts[1] - ((1.f / 3.f) * dimi * vi);
	ppl = pnts[1] + ((1.f / 3.f) * dipl * vi);
	pmi2 = pnts[2] - ((1.f / 3.f) * dipl * vi2);
	ppl2 = pnts[2] + ((1.f / 3.f) * di2 * vi2);
	GsArray<GsVec> ctrlpnts;
	ctrlpnts.push(pnts[1]), ctrlpnts.push(ppl), ctrlpnts.push(pmi2), ctrlpnts.push(pnts[2]);
	return eval_bezier(t, ctrlpnts);
}

static float N(int i, int k, float u)
{
	float ui = float(i);
	if (k == 1)
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	else
	{
		return ((u - ui) / (k - 1)) * N(i, k - 1, u) + ((ui + k - u) / (k - 1)) * N(i + 1, k - 1, u);
	}
}
GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts)
{
	GsVec P;
	for (int i = 0; i < pnts.size(); i++)
	{
		P += pnts[i] * N(i, k, t);
	}
	return P;
}

