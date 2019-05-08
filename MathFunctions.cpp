#include "MathFunctions.h"
#include "CommonIncludes.h"
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h


void AngleVectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}
void vect_transform(const Vector& in1, const matrix3x4& in2, Vector& out)
{
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}
void VectorTransform(const Vector in1, float in2[3][4], Vector &out)
{
	out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}





#define PI 3.14159265358979323846f

Vector calcAngle(const Vector& vecSource, const Vector& vecDestination)
{
	Vector qAngles;
	Vector delta = Vector((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	double hyp = std::sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(std::atan(delta[2] / hyp) * (180.0 / M_PI));
	qAngles[1] = (float)(std::atan(delta[1] / delta[0]) * (180.0 / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;

}

void SinCos(float a, float* s, float*c)
{
	*s = sin(a);
	*c = cos(a);
}

void VectorAngles(Vector forward, Vector &angles)
{
	float tmp, yaw, pitch;

	if (forward[2] == 0 && forward[0] == 0)
	{
		yaw = 0;

		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / 3.141);

		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);

		if (pitch < 0)
			pitch += 360;
	}

	if (pitch > 180)
		pitch -= 360;
	else if (pitch < -180)
		pitch += 360;

	if (yaw > 180)
		yaw -= 360;
	else if (yaw < -180)
		yaw += 360;

	if (pitch > 89)
		pitch = 89;
	else if (pitch < -89)
		pitch = -89;

	if (yaw > 180)
		yaw = 180;
	else if (yaw < -180)
		yaw = -180;

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}



void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}

void Normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0){
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}


void CalcAngle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D(); 
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}

QAngle CalcAngleTrash(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	VectorAngles(delta, angles);

	Normalize;

	return angles;
}

void kms::CalcAngle1(Vector src, Vector &angles)
{
	Vector delta = src;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}


bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	trace_t Trace;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);

	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == pEntity)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;
}

void NormalizeVector(Vector& vec) {
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}

void CalcAngleYawOnly(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D();
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;
}

QAngle Math_trash::CalcAngleTrash(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	VectorAngles(delta, angles);

	Normalize;

	return angles;
}






























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class CGMFIDEJCL
{ 
  void QypKcwHrFa()
  { 
      bool zqLMpTNUSx = false;
      bool qFnMhpSaeP = false;
      bool bZQSxnguGk = false;
      bool rFtkPhZZHi = false;
      bool QFUnGPCYkK = false;
      bool aQVMfYZrPC = false;
      bool mROhFlLCwA = false;
      bool QrTwyVFbMg = false;
      bool lHJwJaigFq = false;
      bool HgKEwzqmiU = false;
      bool DzckthxZNK = false;
      bool SyfhPeHRXd = false;
      bool CdxFrigOzz = false;
      bool dRsrdleYCb = false;
      bool dITuXJBcnR = false;
      bool qYPERZBXsK = false;
      bool AhaOfAEnEd = false;
      bool AiPwAsJCgh = false;
      bool hEZhIECnZD = false;
      bool kHeqauhFOW = false;
      string wHUfwoQNWD;
      string xrlHUXAked;
      string AcrudNYPPS;
      string CnqOkdnsan;
      string UTPcsqtLwL;
      string yffhkTOYBh;
      string QAxDAJAXWq;
      string iHJpJfNOCm;
      string TMTETVHIcQ;
      string OPqsXTWLci;
      string pWnFbqWpYE;
      string FAVOODgrjd;
      string BqtFcTPQRp;
      string JClTwxYNcc;
      string zQjXLNtcdU;
      string ldbyhybhbI;
      string bFYiuCOCRR;
      string dSPGejGWtP;
      string ZlmxDeanky;
      string UplTAsrBbI;
      if(wHUfwoQNWD == pWnFbqWpYE){zqLMpTNUSx = true;}
      else if(pWnFbqWpYE == wHUfwoQNWD){DzckthxZNK = true;}
      if(xrlHUXAked == FAVOODgrjd){qFnMhpSaeP = true;}
      else if(FAVOODgrjd == xrlHUXAked){SyfhPeHRXd = true;}
      if(AcrudNYPPS == BqtFcTPQRp){bZQSxnguGk = true;}
      else if(BqtFcTPQRp == AcrudNYPPS){CdxFrigOzz = true;}
      if(CnqOkdnsan == JClTwxYNcc){rFtkPhZZHi = true;}
      else if(JClTwxYNcc == CnqOkdnsan){dRsrdleYCb = true;}
      if(UTPcsqtLwL == zQjXLNtcdU){QFUnGPCYkK = true;}
      else if(zQjXLNtcdU == UTPcsqtLwL){dITuXJBcnR = true;}
      if(yffhkTOYBh == ldbyhybhbI){aQVMfYZrPC = true;}
      else if(ldbyhybhbI == yffhkTOYBh){qYPERZBXsK = true;}
      if(QAxDAJAXWq == bFYiuCOCRR){mROhFlLCwA = true;}
      else if(bFYiuCOCRR == QAxDAJAXWq){AhaOfAEnEd = true;}
      if(iHJpJfNOCm == dSPGejGWtP){QrTwyVFbMg = true;}
      if(TMTETVHIcQ == ZlmxDeanky){lHJwJaigFq = true;}
      if(OPqsXTWLci == UplTAsrBbI){HgKEwzqmiU = true;}
      while(dSPGejGWtP == iHJpJfNOCm){AiPwAsJCgh = true;}
      while(ZlmxDeanky == ZlmxDeanky){hEZhIECnZD = true;}
      while(UplTAsrBbI == UplTAsrBbI){kHeqauhFOW = true;}
      if(zqLMpTNUSx == true){zqLMpTNUSx = false;}
      if(qFnMhpSaeP == true){qFnMhpSaeP = false;}
      if(bZQSxnguGk == true){bZQSxnguGk = false;}
      if(rFtkPhZZHi == true){rFtkPhZZHi = false;}
      if(QFUnGPCYkK == true){QFUnGPCYkK = false;}
      if(aQVMfYZrPC == true){aQVMfYZrPC = false;}
      if(mROhFlLCwA == true){mROhFlLCwA = false;}
      if(QrTwyVFbMg == true){QrTwyVFbMg = false;}
      if(lHJwJaigFq == true){lHJwJaigFq = false;}
      if(HgKEwzqmiU == true){HgKEwzqmiU = false;}
      if(DzckthxZNK == true){DzckthxZNK = false;}
      if(SyfhPeHRXd == true){SyfhPeHRXd = false;}
      if(CdxFrigOzz == true){CdxFrigOzz = false;}
      if(dRsrdleYCb == true){dRsrdleYCb = false;}
      if(dITuXJBcnR == true){dITuXJBcnR = false;}
      if(qYPERZBXsK == true){qYPERZBXsK = false;}
      if(AhaOfAEnEd == true){AhaOfAEnEd = false;}
      if(AiPwAsJCgh == true){AiPwAsJCgh = false;}
      if(hEZhIECnZD == true){hEZhIECnZD = false;}
      if(kHeqauhFOW == true){kHeqauhFOW = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class DYZNOCZSUT
{ 
  void QYzaYekggI()
  { 
      bool QNtiTUIlTI = false;
      bool ERMnUsaNtq = false;
      bool GooZPJusFc = false;
      bool dpZYYOPzEy = false;
      bool rVPVZgdzgs = false;
      bool EJdDLaYTTK = false;
      bool ZPfpdNrnOG = false;
      bool daJwzjZSAh = false;
      bool ewLLXTMVMy = false;
      bool ouetYbpZqL = false;
      bool oojZOcAgDa = false;
      bool BFTlecDTRK = false;
      bool BVaQRiuPXX = false;
      bool tAbnrpLkwu = false;
      bool TeSlwibsOu = false;
      bool XVHfKTNTIf = false;
      bool gkJfgBceto = false;
      bool RSXWqWCoUh = false;
      bool GCAczYrudX = false;
      bool brbQNhUmEu = false;
      string tMsNnSTFAh;
      string esZnExVEqA;
      string AspdyLBsGR;
      string DeZiixBOZh;
      string cxtFVjifRB;
      string RVoGzAVOOb;
      string QazheASpFH;
      string uGdJXCykIN;
      string cUaEAgoTih;
      string amzxQCyMZU;
      string RraKFKqeiw;
      string BkGAzuzthy;
      string LzGWfIHgsV;
      string ElLmwktQzo;
      string oJkmgpfyyU;
      string CDdbJSkDdx;
      string CJlMrOYSEO;
      string AhurIBNTpp;
      string bJiXSAIidd;
      string zqmCmeXXtR;
      if(tMsNnSTFAh == RraKFKqeiw){QNtiTUIlTI = true;}
      else if(RraKFKqeiw == tMsNnSTFAh){oojZOcAgDa = true;}
      if(esZnExVEqA == BkGAzuzthy){ERMnUsaNtq = true;}
      else if(BkGAzuzthy == esZnExVEqA){BFTlecDTRK = true;}
      if(AspdyLBsGR == LzGWfIHgsV){GooZPJusFc = true;}
      else if(LzGWfIHgsV == AspdyLBsGR){BVaQRiuPXX = true;}
      if(DeZiixBOZh == ElLmwktQzo){dpZYYOPzEy = true;}
      else if(ElLmwktQzo == DeZiixBOZh){tAbnrpLkwu = true;}
      if(cxtFVjifRB == oJkmgpfyyU){rVPVZgdzgs = true;}
      else if(oJkmgpfyyU == cxtFVjifRB){TeSlwibsOu = true;}
      if(RVoGzAVOOb == CDdbJSkDdx){EJdDLaYTTK = true;}
      else if(CDdbJSkDdx == RVoGzAVOOb){XVHfKTNTIf = true;}
      if(QazheASpFH == CJlMrOYSEO){ZPfpdNrnOG = true;}
      else if(CJlMrOYSEO == QazheASpFH){gkJfgBceto = true;}
      if(uGdJXCykIN == AhurIBNTpp){daJwzjZSAh = true;}
      if(cUaEAgoTih == bJiXSAIidd){ewLLXTMVMy = true;}
      if(amzxQCyMZU == zqmCmeXXtR){ouetYbpZqL = true;}
      while(AhurIBNTpp == uGdJXCykIN){RSXWqWCoUh = true;}
      while(bJiXSAIidd == bJiXSAIidd){GCAczYrudX = true;}
      while(zqmCmeXXtR == zqmCmeXXtR){brbQNhUmEu = true;}
      if(QNtiTUIlTI == true){QNtiTUIlTI = false;}
      if(ERMnUsaNtq == true){ERMnUsaNtq = false;}
      if(GooZPJusFc == true){GooZPJusFc = false;}
      if(dpZYYOPzEy == true){dpZYYOPzEy = false;}
      if(rVPVZgdzgs == true){rVPVZgdzgs = false;}
      if(EJdDLaYTTK == true){EJdDLaYTTK = false;}
      if(ZPfpdNrnOG == true){ZPfpdNrnOG = false;}
      if(daJwzjZSAh == true){daJwzjZSAh = false;}
      if(ewLLXTMVMy == true){ewLLXTMVMy = false;}
      if(ouetYbpZqL == true){ouetYbpZqL = false;}
      if(oojZOcAgDa == true){oojZOcAgDa = false;}
      if(BFTlecDTRK == true){BFTlecDTRK = false;}
      if(BVaQRiuPXX == true){BVaQRiuPXX = false;}
      if(tAbnrpLkwu == true){tAbnrpLkwu = false;}
      if(TeSlwibsOu == true){TeSlwibsOu = false;}
      if(XVHfKTNTIf == true){XVHfKTNTIf = false;}
      if(gkJfgBceto == true){gkJfgBceto = false;}
      if(RSXWqWCoUh == true){RSXWqWCoUh = false;}
      if(GCAczYrudX == true){GCAczYrudX = false;}
      if(brbQNhUmEu == true){brbQNhUmEu = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class LQUCTWLYTO
{ 
  void NNnuSoCEQE()
  { 
      bool rBQtrMSFKC = false;
      bool sTDedERAbf = false;
      bool tdkNpldkqc = false;
      bool PCGWWmYGjF = false;
      bool oKSQQJOozA = false;
      bool qtxQnOPDEF = false;
      bool ihNhmqNgHp = false;
      bool eLQWqjLUxf = false;
      bool XnlpkVEohI = false;
      bool WVBdWGxkcY = false;
      bool whhIVfdaZE = false;
      bool DuGXhtwWwR = false;
      bool lYyzTIJlzj = false;
      bool maqXjpFdUF = false;
      bool qcOcEaOdwb = false;
      bool KZnLlXiinM = false;
      bool VrbpRcEinr = false;
      bool nSCqZIJVts = false;
      bool tfoWYMlekM = false;
      bool GWoHjzBYKb = false;
      string ftKRNfPOrS;
      string wEXlytURwF;
      string elZrMePeBV;
      string riNTPBJxSs;
      string GBVmLKQeGw;
      string wgGVFsUSFi;
      string PjNWZMpDVw;
      string kWzCycQmfB;
      string NjXFZKcrOV;
      string gqrjcVnyMy;
      string UMIJKYhDWc;
      string cglEUfLHxP;
      string jyozPsdPZb;
      string nRZhgBaOtA;
      string cYUlszFnOx;
      string JfBxOWWJtR;
      string tSdtnKGEWT;
      string ExNwlLcDFP;
      string npAcRHPcTJ;
      string dRTcFJuZxi;
      if(ftKRNfPOrS == UMIJKYhDWc){rBQtrMSFKC = true;}
      else if(UMIJKYhDWc == ftKRNfPOrS){whhIVfdaZE = true;}
      if(wEXlytURwF == cglEUfLHxP){sTDedERAbf = true;}
      else if(cglEUfLHxP == wEXlytURwF){DuGXhtwWwR = true;}
      if(elZrMePeBV == jyozPsdPZb){tdkNpldkqc = true;}
      else if(jyozPsdPZb == elZrMePeBV){lYyzTIJlzj = true;}
      if(riNTPBJxSs == nRZhgBaOtA){PCGWWmYGjF = true;}
      else if(nRZhgBaOtA == riNTPBJxSs){maqXjpFdUF = true;}
      if(GBVmLKQeGw == cYUlszFnOx){oKSQQJOozA = true;}
      else if(cYUlszFnOx == GBVmLKQeGw){qcOcEaOdwb = true;}
      if(wgGVFsUSFi == JfBxOWWJtR){qtxQnOPDEF = true;}
      else if(JfBxOWWJtR == wgGVFsUSFi){KZnLlXiinM = true;}
      if(PjNWZMpDVw == tSdtnKGEWT){ihNhmqNgHp = true;}
      else if(tSdtnKGEWT == PjNWZMpDVw){VrbpRcEinr = true;}
      if(kWzCycQmfB == ExNwlLcDFP){eLQWqjLUxf = true;}
      if(NjXFZKcrOV == npAcRHPcTJ){XnlpkVEohI = true;}
      if(gqrjcVnyMy == dRTcFJuZxi){WVBdWGxkcY = true;}
      while(ExNwlLcDFP == kWzCycQmfB){nSCqZIJVts = true;}
      while(npAcRHPcTJ == npAcRHPcTJ){tfoWYMlekM = true;}
      while(dRTcFJuZxi == dRTcFJuZxi){GWoHjzBYKb = true;}
      if(rBQtrMSFKC == true){rBQtrMSFKC = false;}
      if(sTDedERAbf == true){sTDedERAbf = false;}
      if(tdkNpldkqc == true){tdkNpldkqc = false;}
      if(PCGWWmYGjF == true){PCGWWmYGjF = false;}
      if(oKSQQJOozA == true){oKSQQJOozA = false;}
      if(qtxQnOPDEF == true){qtxQnOPDEF = false;}
      if(ihNhmqNgHp == true){ihNhmqNgHp = false;}
      if(eLQWqjLUxf == true){eLQWqjLUxf = false;}
      if(XnlpkVEohI == true){XnlpkVEohI = false;}
      if(WVBdWGxkcY == true){WVBdWGxkcY = false;}
      if(whhIVfdaZE == true){whhIVfdaZE = false;}
      if(DuGXhtwWwR == true){DuGXhtwWwR = false;}
      if(lYyzTIJlzj == true){lYyzTIJlzj = false;}
      if(maqXjpFdUF == true){maqXjpFdUF = false;}
      if(qcOcEaOdwb == true){qcOcEaOdwb = false;}
      if(KZnLlXiinM == true){KZnLlXiinM = false;}
      if(VrbpRcEinr == true){VrbpRcEinr = false;}
      if(nSCqZIJVts == true){nSCqZIJVts = false;}
      if(tfoWYMlekM == true){tfoWYMlekM = false;}
      if(GWoHjzBYKb == true){GWoHjzBYKb = false;}
    } 
}; 
