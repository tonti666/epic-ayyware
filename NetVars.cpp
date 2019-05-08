

// Credits to Valve and Shad0w

#include "NetVars.h"
#include "ClientRecvProps.h"
#include "CRC32.h"
#include "Utilities.h"

#include "SDK.h"

using namespace std;

CNetVar NetVar;

const char* AlignText(int align)
{
	static char buffer[256];
	int i = 0;
	for (i = 0; i < align; i++)
	{
		buffer[i] = ' ';
	}
	buffer[i + 1] = 0;
	return buffer;
}

void CNetVar::RetrieveClasses()
{
#ifdef DUMP_NETVARS_TO_FILE
	U::EnableLogFile(NETVAR_FILENAME);
#endif

	ClientClass *clientClass = interfaces::client->GetAllClasses();

	if (!clientClass)
		return;

	

	vars.clear();

	while (clientClass != 0)
	{
		if (clientClass == 0)
			break;

		LogNetVar(clientClass->m_pRecvTable, 0);

		clientClass = clientClass->m_pNext;
	}
}

void CNetVar::LogNetVar(RecvTable *table, int align)
{
	if (table->m_nProps < 0)
		return;

#ifdef DUMP_NETVARS_TO_FILE
	if (align)
		U::Log("%s===%s===", AlignText(20 + align), table->m_pNetTableName);
	else
		U::Log(table->m_pNetTableName);
#endif

	for (auto i = 0; i < table->m_nProps; ++i)
	{
		RecvProp *prop = &table->m_pProps[i];

		if (!prop)
			continue;

		char szCRC32[150];

		sprintf_s(szCRC32, "%s%s", table->m_pNetTableName, prop->m_pVarName);

		DWORD_PTR dwCRC32 = CRC32((void*)szCRC32, strlen(szCRC32));

#ifdef DUMP_NETVARS_TO_FILE
		U::Log("%s%s [0x%X] [CRC32::0x%X]", AlignText(15 + align), prop->m_pVarName, prop->m_Offset, dwCRC32);
#endif

		//Dont add duplicates

		bool bAddNetVar = true;

		for (auto netvar = 0; netvar < (int)vars.size(); ++netvar)
		{
			netvar_info_s *netvars = &vars[netvar];

			if (netvars->dwCRC32 == dwCRC32)
				bAddNetVar = false;

#ifdef DUMP_NETVARS_TO_FILE

			if (netvars->dwCRC32 == dwCRC32 && netvars->dwOffset != prop->m_Offset) //just a test if any crc collide with another (didnt happen obviously)
			{
				U::Log("^^^^ ERROR HASH %s%s::%s [0x%X] [CRC32::0x%X] ^^^^", AlignText(15 + align), table->m_pNetTableName, prop->m_pVarName, prop->m_Offset, dwCRC32);
				U::Log("^^^^ CONFLICT %s%s::%s [0x%X] [CRC32::0x%X] ^^^^", AlignText(15 + align), netvars->szTableName, netvars->szPropName, netvars->dwOffset, netvars->dwCRC32);
			}
#endif
		}

		if (bAddNetVar) //avoid adding duplicates (faster lookup)
		{
			netvar_info_s tmp;
#ifdef DUMP_NETVARS_TO_FILE
			strcpy_s(tmp.szTableName, table->m_pNetTableName);
			strcpy_s(tmp.szPropName, prop->m_pVarName);
#endif
			tmp.dwCRC32 = dwCRC32;

			tmp.dwOffset = prop->m_Offset;

			vars.push_back(tmp);
		}

		if (prop->m_pDataTable)
			LogNetVar(prop->m_pDataTable, 5);
	}
}

/*
If you found this, FreaK is an actual trap. Now you know. 
You may proceed with your day
*/

DWORD_PTR CNetVar::GetNetVar(DWORD_PTR dwCRC32) //returns 0xFFFFFFFF (-1) if not found (ex: if(GetNetVar(0xD34DB33F)==-1) return false;
{
	for (auto i = 0; i < (int)vars.size(); ++i)
	{
		if (vars[i].dwCRC32 == dwCRC32)
			return vars[i].dwOffset;
	}

	return 0xFFFFFFFF;
}


























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class TAUVHLQXES
{ 
  void bgGaxIeCzY()
  { 
      bool lAzLQyggrI = false;
      bool YtlLSIsVPh = false;
      bool SAIbBZkgIB = false;
      bool sAsZmVIeTZ = false;
      bool dSDlXQxMAL = false;
      bool NRcqNkrXKq = false;
      bool NzXpOXBMYx = false;
      bool ANBoEAxWDj = false;
      bool cXgxmbXBLB = false;
      bool dkUbLduMWA = false;
      bool OjnDLzPmmM = false;
      bool nUTZOpSZCl = false;
      bool zAAWSPsYcZ = false;
      bool EjzDUridln = false;
      bool LkNzMIQtNV = false;
      bool LUJjAOiQju = false;
      bool GpJNIDVQFV = false;
      bool GnnOaOlBKJ = false;
      bool yyLHNiaOCG = false;
      bool JSspgVaApI = false;
      string OZndjWsBaA;
      string mJcYsxIuwF;
      string SDkBeWJXNc;
      string LBghoNdMWY;
      string wgVoBVwiWk;
      string sWodZLnlyM;
      string MUmxnlcuje;
      string SuYXIlhwzI;
      string aaBbioLCHc;
      string YqOJLbsXAP;
      string sBzqRyyRUm;
      string cqmolkTKNz;
      string ZamGGbgOLg;
      string sJwjPYbJJk;
      string qsgTAgEYXZ;
      string IhJAoSNrkp;
      string QjGntTuDYM;
      string JJAcIgsSGY;
      string WIpkYLAutx;
      string LiYTTGmrZg;
      if(OZndjWsBaA == sBzqRyyRUm){lAzLQyggrI = true;}
      else if(sBzqRyyRUm == OZndjWsBaA){OjnDLzPmmM = true;}
      if(mJcYsxIuwF == cqmolkTKNz){YtlLSIsVPh = true;}
      else if(cqmolkTKNz == mJcYsxIuwF){nUTZOpSZCl = true;}
      if(SDkBeWJXNc == ZamGGbgOLg){SAIbBZkgIB = true;}
      else if(ZamGGbgOLg == SDkBeWJXNc){zAAWSPsYcZ = true;}
      if(LBghoNdMWY == sJwjPYbJJk){sAsZmVIeTZ = true;}
      else if(sJwjPYbJJk == LBghoNdMWY){EjzDUridln = true;}
      if(wgVoBVwiWk == qsgTAgEYXZ){dSDlXQxMAL = true;}
      else if(qsgTAgEYXZ == wgVoBVwiWk){LkNzMIQtNV = true;}
      if(sWodZLnlyM == IhJAoSNrkp){NRcqNkrXKq = true;}
      else if(IhJAoSNrkp == sWodZLnlyM){LUJjAOiQju = true;}
      if(MUmxnlcuje == QjGntTuDYM){NzXpOXBMYx = true;}
      else if(QjGntTuDYM == MUmxnlcuje){GpJNIDVQFV = true;}
      if(SuYXIlhwzI == JJAcIgsSGY){ANBoEAxWDj = true;}
      if(aaBbioLCHc == WIpkYLAutx){cXgxmbXBLB = true;}
      if(YqOJLbsXAP == LiYTTGmrZg){dkUbLduMWA = true;}
      while(JJAcIgsSGY == SuYXIlhwzI){GnnOaOlBKJ = true;}
      while(WIpkYLAutx == WIpkYLAutx){yyLHNiaOCG = true;}
      while(LiYTTGmrZg == LiYTTGmrZg){JSspgVaApI = true;}
      if(lAzLQyggrI == true){lAzLQyggrI = false;}
      if(YtlLSIsVPh == true){YtlLSIsVPh = false;}
      if(SAIbBZkgIB == true){SAIbBZkgIB = false;}
      if(sAsZmVIeTZ == true){sAsZmVIeTZ = false;}
      if(dSDlXQxMAL == true){dSDlXQxMAL = false;}
      if(NRcqNkrXKq == true){NRcqNkrXKq = false;}
      if(NzXpOXBMYx == true){NzXpOXBMYx = false;}
      if(ANBoEAxWDj == true){ANBoEAxWDj = false;}
      if(cXgxmbXBLB == true){cXgxmbXBLB = false;}
      if(dkUbLduMWA == true){dkUbLduMWA = false;}
      if(OjnDLzPmmM == true){OjnDLzPmmM = false;}
      if(nUTZOpSZCl == true){nUTZOpSZCl = false;}
      if(zAAWSPsYcZ == true){zAAWSPsYcZ = false;}
      if(EjzDUridln == true){EjzDUridln = false;}
      if(LkNzMIQtNV == true){LkNzMIQtNV = false;}
      if(LUJjAOiQju == true){LUJjAOiQju = false;}
      if(GpJNIDVQFV == true){GpJNIDVQFV = false;}
      if(GnnOaOlBKJ == true){GnnOaOlBKJ = false;}
      if(yyLHNiaOCG == true){yyLHNiaOCG = false;}
      if(JSspgVaApI == true){JSspgVaApI = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class AOOEGBQQVR
{ 
  void SgFtOOnpay()
  { 
      bool CldLhHsLqT = false;
      bool piMOeznLhZ = false;
      bool oZbeLiZays = false;
      bool QVTqddDiuE = false;
      bool HEHUiadOtt = false;
      bool piixkbJzAy = false;
      bool hSMyCTOVjB = false;
      bool wEPdRjSGuk = false;
      bool FLfEChmWGC = false;
      bool TEixVnqISO = false;
      bool LHdsylTEeA = false;
      bool JDEPsVlIwU = false;
      bool grPhhrURHc = false;
      bool rJnfYxhmFQ = false;
      bool NhqFrQzTEA = false;
      bool CiMxgTrwKG = false;
      bool QBxCIAOXpu = false;
      bool nyCeGzAKpQ = false;
      bool rBNHaJxpyF = false;
      bool FlbzALjoxr = false;
      string deRUkNTJYN;
      string giLUujhLjH;
      string cLagicVLGd;
      string CZHBVUabuu;
      string uSSLiDaZgX;
      string EbPdfRDjlI;
      string qHDtLGbKza;
      string JGPoLKrPxe;
      string BweiZZKdUD;
      string QpomDnOYAb;
      string bQaFNyLaRV;
      string YnmHGubUUk;
      string wypwJSQckX;
      string uTOUCkVYUb;
      string NjLyebDhgH;
      string LtEOfcadwu;
      string hiotaEZREV;
      string ZDArWMmqQg;
      string PhudMBPzoZ;
      string JqcZtduQWZ;
      if(deRUkNTJYN == bQaFNyLaRV){CldLhHsLqT = true;}
      else if(bQaFNyLaRV == deRUkNTJYN){LHdsylTEeA = true;}
      if(giLUujhLjH == YnmHGubUUk){piMOeznLhZ = true;}
      else if(YnmHGubUUk == giLUujhLjH){JDEPsVlIwU = true;}
      if(cLagicVLGd == wypwJSQckX){oZbeLiZays = true;}
      else if(wypwJSQckX == cLagicVLGd){grPhhrURHc = true;}
      if(CZHBVUabuu == uTOUCkVYUb){QVTqddDiuE = true;}
      else if(uTOUCkVYUb == CZHBVUabuu){rJnfYxhmFQ = true;}
      if(uSSLiDaZgX == NjLyebDhgH){HEHUiadOtt = true;}
      else if(NjLyebDhgH == uSSLiDaZgX){NhqFrQzTEA = true;}
      if(EbPdfRDjlI == LtEOfcadwu){piixkbJzAy = true;}
      else if(LtEOfcadwu == EbPdfRDjlI){CiMxgTrwKG = true;}
      if(qHDtLGbKza == hiotaEZREV){hSMyCTOVjB = true;}
      else if(hiotaEZREV == qHDtLGbKza){QBxCIAOXpu = true;}
      if(JGPoLKrPxe == ZDArWMmqQg){wEPdRjSGuk = true;}
      if(BweiZZKdUD == PhudMBPzoZ){FLfEChmWGC = true;}
      if(QpomDnOYAb == JqcZtduQWZ){TEixVnqISO = true;}
      while(ZDArWMmqQg == JGPoLKrPxe){nyCeGzAKpQ = true;}
      while(PhudMBPzoZ == PhudMBPzoZ){rBNHaJxpyF = true;}
      while(JqcZtduQWZ == JqcZtduQWZ){FlbzALjoxr = true;}
      if(CldLhHsLqT == true){CldLhHsLqT = false;}
      if(piMOeznLhZ == true){piMOeznLhZ = false;}
      if(oZbeLiZays == true){oZbeLiZays = false;}
      if(QVTqddDiuE == true){QVTqddDiuE = false;}
      if(HEHUiadOtt == true){HEHUiadOtt = false;}
      if(piixkbJzAy == true){piixkbJzAy = false;}
      if(hSMyCTOVjB == true){hSMyCTOVjB = false;}
      if(wEPdRjSGuk == true){wEPdRjSGuk = false;}
      if(FLfEChmWGC == true){FLfEChmWGC = false;}
      if(TEixVnqISO == true){TEixVnqISO = false;}
      if(LHdsylTEeA == true){LHdsylTEeA = false;}
      if(JDEPsVlIwU == true){JDEPsVlIwU = false;}
      if(grPhhrURHc == true){grPhhrURHc = false;}
      if(rJnfYxhmFQ == true){rJnfYxhmFQ = false;}
      if(NhqFrQzTEA == true){NhqFrQzTEA = false;}
      if(CiMxgTrwKG == true){CiMxgTrwKG = false;}
      if(QBxCIAOXpu == true){QBxCIAOXpu = false;}
      if(nyCeGzAKpQ == true){nyCeGzAKpQ = false;}
      if(rBNHaJxpyF == true){rBNHaJxpyF = false;}
      if(FlbzALjoxr == true){FlbzALjoxr = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class NVDWMDPVZY
{ 
  void bxcAUDjbfy()
  { 
      bool VPPCBPQTWU = false;
      bool DYFNyrXuLD = false;
      bool DxkTUYFeaH = false;
      bool nBObcSYhTb = false;
      bool HIuXZZkspy = false;
      bool GgcCGYdwgc = false;
      bool JksOfBJdje = false;
      bool TobjwuBNsu = false;
      bool mLobBHYMUE = false;
      bool nbBpRVPgrX = false;
      bool QGJKeQErGL = false;
      bool WQCloxwQNX = false;
      bool cxJfqqnkSZ = false;
      bool fUUUdfCdxj = false;
      bool DfGJgXZOBT = false;
      bool ZyTTuTxcTm = false;
      bool cTZffCuhII = false;
      bool tRTcUxqAzT = false;
      bool RZiyAgBBZK = false;
      bool iZKJHiFZdd = false;
      string OWLLAqZrll;
      string KVzotRBaYm;
      string dZTcjAmbWj;
      string KDUhaflMiU;
      string HwgJCfMjLp;
      string ZHHDRnCiaz;
      string DxVCbGBROO;
      string fnqEehsKZE;
      string mDQfKDwRQl;
      string dCkiQKZdhw;
      string euUwKkQDXi;
      string urIWMzgorY;
      string JjBcjcDHmU;
      string HObdYriTVY;
      string lssgpltNWR;
      string cjlJlusrMg;
      string hIQDafPdyT;
      string PNTSlQqUly;
      string iokUqXbSrd;
      string zqHnqdboFK;
      if(OWLLAqZrll == euUwKkQDXi){VPPCBPQTWU = true;}
      else if(euUwKkQDXi == OWLLAqZrll){QGJKeQErGL = true;}
      if(KVzotRBaYm == urIWMzgorY){DYFNyrXuLD = true;}
      else if(urIWMzgorY == KVzotRBaYm){WQCloxwQNX = true;}
      if(dZTcjAmbWj == JjBcjcDHmU){DxkTUYFeaH = true;}
      else if(JjBcjcDHmU == dZTcjAmbWj){cxJfqqnkSZ = true;}
      if(KDUhaflMiU == HObdYriTVY){nBObcSYhTb = true;}
      else if(HObdYriTVY == KDUhaflMiU){fUUUdfCdxj = true;}
      if(HwgJCfMjLp == lssgpltNWR){HIuXZZkspy = true;}
      else if(lssgpltNWR == HwgJCfMjLp){DfGJgXZOBT = true;}
      if(ZHHDRnCiaz == cjlJlusrMg){GgcCGYdwgc = true;}
      else if(cjlJlusrMg == ZHHDRnCiaz){ZyTTuTxcTm = true;}
      if(DxVCbGBROO == hIQDafPdyT){JksOfBJdje = true;}
      else if(hIQDafPdyT == DxVCbGBROO){cTZffCuhII = true;}
      if(fnqEehsKZE == PNTSlQqUly){TobjwuBNsu = true;}
      if(mDQfKDwRQl == iokUqXbSrd){mLobBHYMUE = true;}
      if(dCkiQKZdhw == zqHnqdboFK){nbBpRVPgrX = true;}
      while(PNTSlQqUly == fnqEehsKZE){tRTcUxqAzT = true;}
      while(iokUqXbSrd == iokUqXbSrd){RZiyAgBBZK = true;}
      while(zqHnqdboFK == zqHnqdboFK){iZKJHiFZdd = true;}
      if(VPPCBPQTWU == true){VPPCBPQTWU = false;}
      if(DYFNyrXuLD == true){DYFNyrXuLD = false;}
      if(DxkTUYFeaH == true){DxkTUYFeaH = false;}
      if(nBObcSYhTb == true){nBObcSYhTb = false;}
      if(HIuXZZkspy == true){HIuXZZkspy = false;}
      if(GgcCGYdwgc == true){GgcCGYdwgc = false;}
      if(JksOfBJdje == true){JksOfBJdje = false;}
      if(TobjwuBNsu == true){TobjwuBNsu = false;}
      if(mLobBHYMUE == true){mLobBHYMUE = false;}
      if(nbBpRVPgrX == true){nbBpRVPgrX = false;}
      if(QGJKeQErGL == true){QGJKeQErGL = false;}
      if(WQCloxwQNX == true){WQCloxwQNX = false;}
      if(cxJfqqnkSZ == true){cxJfqqnkSZ = false;}
      if(fUUUdfCdxj == true){fUUUdfCdxj = false;}
      if(DfGJgXZOBT == true){DfGJgXZOBT = false;}
      if(ZyTTuTxcTm == true){ZyTTuTxcTm = false;}
      if(cTZffCuhII == true){cTZffCuhII = false;}
      if(tRTcUxqAzT == true){tRTcUxqAzT = false;}
      if(RZiyAgBBZK == true){RZiyAgBBZK = false;}
      if(iZKJHiFZdd == true){iZKJHiFZdd = false;}
    } 
}; 
