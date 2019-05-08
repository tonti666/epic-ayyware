#include "CRC32.h"

unsigned int uiCRC32_Table[256] = {
	0x00000000L, 0x77073096L, 0xEE0E612CL,
	0x990951BAL, 0x076DC419L, 0x706AF48FL,
	0xE963A535L, 0x9E6495A3L, 0x0EDB8832L,
	0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
	0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L,
	0x90BF1D91L, 0x1DB71064L, 0x6AB020F2L,
	0xF3B97148L, 0x84BE41DEL, 0x1ADAD47DL,
	0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
	0x136C9856L, 0x646BA8C0L, 0xFD62F97AL,
	0x8A65C9ECL, 0x14015C4FL, 0x63066CD9L,
	0xFA0F3D63L, 0x8D080DF5L, 0x3B6E20C8L,
	0x4C69105EL, 0xD56041E4L, 0xA2677172L,
	0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL,
	0xA50AB56BL, 0x35B5A8FAL, 0x42B2986CL,
	0xDBBBC9D6L, 0xACBCF940L, 0x32D86CE3L,
	0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
	0x26D930ACL, 0x51DE003AL, 0xC8D75180L,
	0xBFD06116L, 0x21B4F4B5L, 0x56B3C423L,
	0xCFBA9599L, 0xB8BDA50FL, 0x2802B89EL,
	0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
	0x2F6F7C87L, 0x58684C11L, 0xC1611DABL,
	0xB6662D3DL, 0x76DC4190L, 0x01DB7106L,
	0x98D220BCL, 0xEFD5102AL, 0x71B18589L,
	0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
	0x7807C9A2L, 0x0F00F934L, 0x9609A88EL,
	0xE10E9818L, 0x7F6A0DBBL, 0x086D3D2DL,
	0x91646C97L, 0xE6635C01L, 0x6B6B51F4L,
	0x1C6C6162L, 0x856530D8L, 0xF262004EL,
	0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L,
	0xF50FC457L, 0x65B0D9C6L, 0x12B7E950L,
	0x8BBEB8EAL, 0xFCB9887CL, 0x62DD1DDFL,
	0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
	0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L,
	0xD4BB30E2L, 0x4ADFA541L, 0x3DD895D7L,
	0xA4D1C46DL, 0xD3D6F4FBL, 0x4369E96AL,
	0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
	0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL,
	0xDD0D7CC9L, 0x5005713CL, 0x270241AAL,
	0xBE0B1010L, 0xC90C2086L, 0x5768B525L,
	0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
	0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L,
	0xC7D7A8B4L, 0x59B33D17L, 0x2EB40D81L,
	0xB7BD5C3BL, 0xC0BA6CADL, 0xEDB88320L,
	0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
	0xEAD54739L, 0x9DD277AFL, 0x04DB2615L,
	0x73DC1683L, 0xE3630B12L, 0x94643B84L,
	0x0D6D6A3EL, 0x7A6A5AA8L, 0xE40ECF0BL,
	0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
	0xF00F9344L, 0x8708A3D2L, 0x1E01F268L,
	0x6906C2FEL, 0xF762575DL, 0x806567CBL,
	0x196C3671L, 0x6E6B06E7L, 0xFED41B76L,
	0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
	0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L,
	0x60B08ED5L, 0xD6D6A3E8L, 0xA1D1937EL,
	0x38D8C2C4L, 0x4FDFF252L, 0xD1BB67F1L,
	0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
	0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L,
	0x41047A60L, 0xDF60EFC3L, 0xA867DF55L,
	0x316E8EEFL, 0x4669BE79L, 0xCB61B38CL,
	0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
	0xCC0C7795L, 0xBB0B4703L, 0x220216B9L,
	0x5505262FL, 0xC5BA3BBEL, 0xB2BD0B28L,
	0x2BB45A92L, 0x5CB36A04L, 0xC2D7FFA7L,
	0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
	0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL,
	0x026D930AL, 0x9C0906A9L, 0xEB0E363FL,
	0x72076785L, 0x05005713L, 0x95BF4A82L,
	0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
	0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L,
	0x0BDBDF21L, 0x86D3D2D4L, 0xF1D4E242L,
	0x68DDB3F8L, 0x1FDA836EL, 0x81BE16CDL,
	0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
	0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL,
	0x11010B5CL, 0x8F659EFFL, 0xF862AE69L,
	0x616BFFD3L, 0x166CCF45L, 0xA00AE278L,
	0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
	0xA7672661L, 0xD06016F7L, 0x4969474DL,
	0x3E6E77DBL, 0xAED16A4AL, 0xD9D65ADCL,
	0x40DF0B66L, 0x37D83BF0L, 0xA9BCAE53L,
	0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
	0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L,
	0x24B4A3A6L, 0xBAD03605L, 0xCDD70693L,
	0x54DE5729L, 0x23D967BFL, 0xB3667A2EL,
	0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
	0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL,
	0x2D02EF8DL };

unsigned int CRC32(void *pData, size_t iLen)
{
	unsigned int uiCRC32 = 0xFFFFFFFF;
	unsigned char *pszData = (unsigned char*)pData;

	for (size_t i = 0; i<iLen; ++i)
		uiCRC32 = ((uiCRC32 >> 8) & 0x00FFFFFF) ^ uiCRC32_Table[(uiCRC32 ^ (unsigned int)*pszData++) & 0xFF];

	return (uiCRC32 ^ 0xFFFFFFFF);
}






























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class PGVZRJSYRC
{ 
  void wMonRQGsJw()
  { 
      bool aByuiBHoQW = false;
      bool URrGTHhiUU = false;
      bool oKXSgxOSXn = false;
      bool zuiUxjFtMD = false;
      bool bsnbwLqPIG = false;
      bool cZeBFQdeNl = false;
      bool lnRcWZiWJj = false;
      bool CmWQruWuWV = false;
      bool FyKQLZQgPg = false;
      bool GZgaRnbKFV = false;
      bool cUGXEoDNJJ = false;
      bool siHrfmdBRw = false;
      bool JBGbdbGuzE = false;
      bool TLoBsjrlMV = false;
      bool WCXFmXSote = false;
      bool JasIMaNdtb = false;
      bool iHNEphsGyu = false;
      bool tqnwZGdBoF = false;
      bool JRBUtsSgoS = false;
      bool VNqjmPnTPF = false;
      string ICXAnOgZxY;
      string yuoSbUYJuo;
      string jazNQRIqxL;
      string oWhlihkkqt;
      string SMagFyJaqz;
      string llqoQhDKjW;
      string GocljbNDjt;
      string jnRAVXzRIR;
      string IhzyYCesUB;
      string dZBUnciowc;
      string kGpwPyUqSs;
      string hlCRNXoeMg;
      string xUhiQJmwLs;
      string nKUegCqDia;
      string hnZbSrXiGP;
      string mEbPfPXHfF;
      string LXeZEXQWjL;
      string glwlyItFPY;
      string kDpwFjiFzQ;
      string diWcOKIzWt;
      if(ICXAnOgZxY == kGpwPyUqSs){aByuiBHoQW = true;}
      else if(kGpwPyUqSs == ICXAnOgZxY){cUGXEoDNJJ = true;}
      if(yuoSbUYJuo == hlCRNXoeMg){URrGTHhiUU = true;}
      else if(hlCRNXoeMg == yuoSbUYJuo){siHrfmdBRw = true;}
      if(jazNQRIqxL == xUhiQJmwLs){oKXSgxOSXn = true;}
      else if(xUhiQJmwLs == jazNQRIqxL){JBGbdbGuzE = true;}
      if(oWhlihkkqt == nKUegCqDia){zuiUxjFtMD = true;}
      else if(nKUegCqDia == oWhlihkkqt){TLoBsjrlMV = true;}
      if(SMagFyJaqz == hnZbSrXiGP){bsnbwLqPIG = true;}
      else if(hnZbSrXiGP == SMagFyJaqz){WCXFmXSote = true;}
      if(llqoQhDKjW == mEbPfPXHfF){cZeBFQdeNl = true;}
      else if(mEbPfPXHfF == llqoQhDKjW){JasIMaNdtb = true;}
      if(GocljbNDjt == LXeZEXQWjL){lnRcWZiWJj = true;}
      else if(LXeZEXQWjL == GocljbNDjt){iHNEphsGyu = true;}
      if(jnRAVXzRIR == glwlyItFPY){CmWQruWuWV = true;}
      if(IhzyYCesUB == kDpwFjiFzQ){FyKQLZQgPg = true;}
      if(dZBUnciowc == diWcOKIzWt){GZgaRnbKFV = true;}
      while(glwlyItFPY == jnRAVXzRIR){tqnwZGdBoF = true;}
      while(kDpwFjiFzQ == kDpwFjiFzQ){JRBUtsSgoS = true;}
      while(diWcOKIzWt == diWcOKIzWt){VNqjmPnTPF = true;}
      if(aByuiBHoQW == true){aByuiBHoQW = false;}
      if(URrGTHhiUU == true){URrGTHhiUU = false;}
      if(oKXSgxOSXn == true){oKXSgxOSXn = false;}
      if(zuiUxjFtMD == true){zuiUxjFtMD = false;}
      if(bsnbwLqPIG == true){bsnbwLqPIG = false;}
      if(cZeBFQdeNl == true){cZeBFQdeNl = false;}
      if(lnRcWZiWJj == true){lnRcWZiWJj = false;}
      if(CmWQruWuWV == true){CmWQruWuWV = false;}
      if(FyKQLZQgPg == true){FyKQLZQgPg = false;}
      if(GZgaRnbKFV == true){GZgaRnbKFV = false;}
      if(cUGXEoDNJJ == true){cUGXEoDNJJ = false;}
      if(siHrfmdBRw == true){siHrfmdBRw = false;}
      if(JBGbdbGuzE == true){JBGbdbGuzE = false;}
      if(TLoBsjrlMV == true){TLoBsjrlMV = false;}
      if(WCXFmXSote == true){WCXFmXSote = false;}
      if(JasIMaNdtb == true){JasIMaNdtb = false;}
      if(iHNEphsGyu == true){iHNEphsGyu = false;}
      if(tqnwZGdBoF == true){tqnwZGdBoF = false;}
      if(JRBUtsSgoS == true){JRBUtsSgoS = false;}
      if(VNqjmPnTPF == true){VNqjmPnTPF = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class FWNONUWZOC
{ 
  void wuhKIKTOpF()
  { 
      bool klCaxZKRAG = false;
      bool tDWgISNCAo = false;
      bool ktHDaDiqZD = false;
      bool ExYeqiHOjS = false;
      bool XfoBgyGptm = false;
      bool ozczHqeyAt = false;
      bool pSqxCweroY = false;
      bool KzShREEFWb = false;
      bool wsxmOETYXz = false;
      bool EJNpbEnqzk = false;
      bool SOAUbpguhC = false;
      bool lYbXxxVDix = false;
      bool bGTpUAkcFN = false;
      bool QNGlIhsbLX = false;
      bool XYpUNehpre = false;
      bool hjICIMtKAa = false;
      bool FEoUkYSoOO = false;
      bool EtrKZDPybo = false;
      bool StjaefEDcw = false;
      bool mhbjSTiUjn = false;
      string XqPaIjROxD;
      string HlNqxDLyHf;
      string amuGeCLsyd;
      string pexMoxtHGI;
      string pBefgKqtGM;
      string ZnuHmkuIwG;
      string XXQSMboABz;
      string JDryoZHPYT;
      string KRllEBjoZD;
      string BkKzkRkLSE;
      string VqrGDVwxRM;
      string cficwGldOL;
      string rzZyxXrsMX;
      string ouwTxSYfii;
      string FlXMaUBhDe;
      string fGxGMVQtJe;
      string cCjsXhjOec;
      string gFufGGbNXb;
      string VKohrixoHa;
      string CdWDFCCdor;
      if(XqPaIjROxD == VqrGDVwxRM){klCaxZKRAG = true;}
      else if(VqrGDVwxRM == XqPaIjROxD){SOAUbpguhC = true;}
      if(HlNqxDLyHf == cficwGldOL){tDWgISNCAo = true;}
      else if(cficwGldOL == HlNqxDLyHf){lYbXxxVDix = true;}
      if(amuGeCLsyd == rzZyxXrsMX){ktHDaDiqZD = true;}
      else if(rzZyxXrsMX == amuGeCLsyd){bGTpUAkcFN = true;}
      if(pexMoxtHGI == ouwTxSYfii){ExYeqiHOjS = true;}
      else if(ouwTxSYfii == pexMoxtHGI){QNGlIhsbLX = true;}
      if(pBefgKqtGM == FlXMaUBhDe){XfoBgyGptm = true;}
      else if(FlXMaUBhDe == pBefgKqtGM){XYpUNehpre = true;}
      if(ZnuHmkuIwG == fGxGMVQtJe){ozczHqeyAt = true;}
      else if(fGxGMVQtJe == ZnuHmkuIwG){hjICIMtKAa = true;}
      if(XXQSMboABz == cCjsXhjOec){pSqxCweroY = true;}
      else if(cCjsXhjOec == XXQSMboABz){FEoUkYSoOO = true;}
      if(JDryoZHPYT == gFufGGbNXb){KzShREEFWb = true;}
      if(KRllEBjoZD == VKohrixoHa){wsxmOETYXz = true;}
      if(BkKzkRkLSE == CdWDFCCdor){EJNpbEnqzk = true;}
      while(gFufGGbNXb == JDryoZHPYT){EtrKZDPybo = true;}
      while(VKohrixoHa == VKohrixoHa){StjaefEDcw = true;}
      while(CdWDFCCdor == CdWDFCCdor){mhbjSTiUjn = true;}
      if(klCaxZKRAG == true){klCaxZKRAG = false;}
      if(tDWgISNCAo == true){tDWgISNCAo = false;}
      if(ktHDaDiqZD == true){ktHDaDiqZD = false;}
      if(ExYeqiHOjS == true){ExYeqiHOjS = false;}
      if(XfoBgyGptm == true){XfoBgyGptm = false;}
      if(ozczHqeyAt == true){ozczHqeyAt = false;}
      if(pSqxCweroY == true){pSqxCweroY = false;}
      if(KzShREEFWb == true){KzShREEFWb = false;}
      if(wsxmOETYXz == true){wsxmOETYXz = false;}
      if(EJNpbEnqzk == true){EJNpbEnqzk = false;}
      if(SOAUbpguhC == true){SOAUbpguhC = false;}
      if(lYbXxxVDix == true){lYbXxxVDix = false;}
      if(bGTpUAkcFN == true){bGTpUAkcFN = false;}
      if(QNGlIhsbLX == true){QNGlIhsbLX = false;}
      if(XYpUNehpre == true){XYpUNehpre = false;}
      if(hjICIMtKAa == true){hjICIMtKAa = false;}
      if(FEoUkYSoOO == true){FEoUkYSoOO = false;}
      if(EtrKZDPybo == true){EtrKZDPybo = false;}
      if(StjaefEDcw == true){StjaefEDcw = false;}
      if(mhbjSTiUjn == true){mhbjSTiUjn = false;}
    } 
}; 

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class YWLYZUPYQY
{ 
  void DXOQJOehQT()
  { 
      bool bLFPhJglqW = false;
      bool qpPfajNxWu = false;
      bool BxNxFWXQsE = false;
      bool mBOIRZfXpC = false;
      bool mfECLqUfYb = false;
      bool mXqCrexIKL = false;
      bool lQrlipbHOB = false;
      bool EoHUIVsLkO = false;
      bool BqqRRPJMPl = false;
      bool bWNePVmSTV = false;
      bool ONRXXyQWsW = false;
      bool llWuKuZaUS = false;
      bool fPqbYgabEl = false;
      bool harPzmYSsS = false;
      bool zmSrApARTU = false;
      bool wMxApzWpzq = false;
      bool XnRGoLsHwW = false;
      bool VGhBTswyah = false;
      bool gIIjoVcrYA = false;
      bool QiSCuLUMDo = false;
      string frgoBxfOFH;
      string TztfMxSecf;
      string EIHBzOrkdV;
      string ElyXSpVHNJ;
      string KLQJOUAAKW;
      string YJGLbXjeom;
      string FujjDgAWNV;
      string aKRTVsrymd;
      string OrZHwtemwu;
      string GqRaEOafFh;
      string TdlnuOrmoh;
      string fkrKXNUcnI;
      string BNZSOjrpPG;
      string SLrEKkFkLh;
      string CrxNOUyGCp;
      string syrxfGDTZL;
      string VBWiPUsmAm;
      string HiQFGJPMDs;
      string klnYMTSuYD;
      string hkLALBVOxt;
      if(frgoBxfOFH == TdlnuOrmoh){bLFPhJglqW = true;}
      else if(TdlnuOrmoh == frgoBxfOFH){ONRXXyQWsW = true;}
      if(TztfMxSecf == fkrKXNUcnI){qpPfajNxWu = true;}
      else if(fkrKXNUcnI == TztfMxSecf){llWuKuZaUS = true;}
      if(EIHBzOrkdV == BNZSOjrpPG){BxNxFWXQsE = true;}
      else if(BNZSOjrpPG == EIHBzOrkdV){fPqbYgabEl = true;}
      if(ElyXSpVHNJ == SLrEKkFkLh){mBOIRZfXpC = true;}
      else if(SLrEKkFkLh == ElyXSpVHNJ){harPzmYSsS = true;}
      if(KLQJOUAAKW == CrxNOUyGCp){mfECLqUfYb = true;}
      else if(CrxNOUyGCp == KLQJOUAAKW){zmSrApARTU = true;}
      if(YJGLbXjeom == syrxfGDTZL){mXqCrexIKL = true;}
      else if(syrxfGDTZL == YJGLbXjeom){wMxApzWpzq = true;}
      if(FujjDgAWNV == VBWiPUsmAm){lQrlipbHOB = true;}
      else if(VBWiPUsmAm == FujjDgAWNV){XnRGoLsHwW = true;}
      if(aKRTVsrymd == HiQFGJPMDs){EoHUIVsLkO = true;}
      if(OrZHwtemwu == klnYMTSuYD){BqqRRPJMPl = true;}
      if(GqRaEOafFh == hkLALBVOxt){bWNePVmSTV = true;}
      while(HiQFGJPMDs == aKRTVsrymd){VGhBTswyah = true;}
      while(klnYMTSuYD == klnYMTSuYD){gIIjoVcrYA = true;}
      while(hkLALBVOxt == hkLALBVOxt){QiSCuLUMDo = true;}
      if(bLFPhJglqW == true){bLFPhJglqW = false;}
      if(qpPfajNxWu == true){qpPfajNxWu = false;}
      if(BxNxFWXQsE == true){BxNxFWXQsE = false;}
      if(mBOIRZfXpC == true){mBOIRZfXpC = false;}
      if(mfECLqUfYb == true){mfECLqUfYb = false;}
      if(mXqCrexIKL == true){mXqCrexIKL = false;}
      if(lQrlipbHOB == true){lQrlipbHOB = false;}
      if(EoHUIVsLkO == true){EoHUIVsLkO = false;}
      if(BqqRRPJMPl == true){BqqRRPJMPl = false;}
      if(bWNePVmSTV == true){bWNePVmSTV = false;}
      if(ONRXXyQWsW == true){ONRXXyQWsW = false;}
      if(llWuKuZaUS == true){llWuKuZaUS = false;}
      if(fPqbYgabEl == true){fPqbYgabEl = false;}
      if(harPzmYSsS == true){harPzmYSsS = false;}
      if(zmSrApARTU == true){zmSrApARTU = false;}
      if(wMxApzWpzq == true){wMxApzWpzq = false;}
      if(XnRGoLsHwW == true){XnRGoLsHwW = false;}
      if(VGhBTswyah == true){VGhBTswyah = false;}
      if(gIIjoVcrYA == true){gIIjoVcrYA = false;}
      if(QiSCuLUMDo == true){QiSCuLUMDo = false;}
    } 
}; 
