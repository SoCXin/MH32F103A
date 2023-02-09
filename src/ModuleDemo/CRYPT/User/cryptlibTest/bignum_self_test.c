#include "stdio.h"
#include "stdint.h"
#ifndef USE_CRYPT_LIB
#include "mh_crypt_ip.h"
#endif
#include "mh_bignum.h"
#include "mh_bignum_tool.h"
#include "mh_misc.h"
#include "mh_rand.h"
#include "debug.h"
#include "mh_crypt_bignum.h"
#include "mh_rsa.h"
#include "mhcard.h"

#define MAX_SIZE   32
#define GEN_DIV_PARA_COUNT 1000
#define DIV_TEST_COUNT     5


#define PARA_BYTE  32
#define PARA_A    "C46841E769D7F6A7599046C029900CB6D47BBD54BE83AF50284584BC1E390BF9"
#define PARA_B    "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"
//#define PARA_A  "0FC9EEFF11BA052C407E7ECC4B6F25E210155DBA7DEE604C2B047D7DC9261DFC29EC7CD491188E68E04EA31CC7A14F6C856CE91E9A379CABDF45DD66C44ADE801B487D61B474F2DF6ACCB480BFD1C27E8BE2070773B193B3A6775A38FE5F045727940D83DCAD2D187A20D68BEFA0D21B8768E3D60EDE1A03F3A319FFA3CBE0C7DDCB1F94B2D151E74421B2402DAC59CE2CD733B7FE5BBF21D9A20BCD0EABCA582C1A3097AADED41664F128055B7C0868A38A27CE4E81B63BB8B35C2295F9C37304C16A7F75D103BC8280A318DF8BD60337A6E84001218BD5B9B6E105F22563FD590E61D02BB9515A503B106E711A992508EB2530B0ECB6A9A1455A871BCC87E9"
//#define PARA_B  "0BD34E32CFBAB12D49293926CE039A5E6A31389649A66A698A967F5B07D534FC7EFC356266FE7FCB4D6058AF452FFD17E728C3855F9A0E8CF33BA287C82178B0328B1E18D9309B0CC0436E48B8459B9268AECF72CF5B008505FA4CDD70FAA30119E799151E377B42EFBA0DFBAF2E8B4BAEED42F96620EDD43DEF46AA22C678BC"
#define MUL_A   "C923E91244FE7E411581E3E8CCFC6651271D1EC0A135B0A63AEF5705F8AEC7D0D93DBE779176C1BA28F6E4C439395EF7D7A7FDC48A3D532CBDA75751BAC568A67560D65A585FB22663F3BA2BE2CDA97FF5E7D58171322FD41146B8FF8CAA9F1938E38795B75FBB5C"
#define MUL_B   "16F5A714D7B2EB6C"


#define PARA_N  "A25AC26D47E943DF81742D6C5DCFB1AC6BD6F9592C441DA4A437CC395C0D2A2473FFDE2B71F889216CABCEC7DD1121F615172F16B0C4D7A9931AE93C581CE6C2AA4CF4F4236A6353"
#define PARA_NC "2527CADF7EC2D097"


#define MOD_P        "75A18418B8EB30CD990303626314215772406FB53C90BC34F8963643CB7C86655174E82AC581EA9D93FBCD8B9B9623E9FE8AD5E2F9311C8FEEFB709D78CC961DC49A3CA5F55733E68A10563695782DE1C8BE5F031E84540E14AA369FA8E03D03CCC7E02B2A030C44AF4B989DBC99640BB4925B9C3AAE287C241C1AF133502AD61593C62FAB42604680FBE1EC77C98B68008C15E52C24035A8F220C1B944C547F1E0FA4C6475395839536FD286F23063E802D657C6D176B46E3B1862616D32EADB0DC1C1AF83EAF33640224658BB46052ACD8D8876A7F35A9232ED4AD2E15FD958F294A5BFFBA11E2E18F7D3377136F6C91B93C9656E289E19FE3E928FDF7FB7F"
#define MOD_B        "65B37280142A48FCED5C720FC0A17DB2A6B6C1901CD1FECA252FEB61E5CF58E4068800EE4F54D2D9E35566BC5CB976B497E65FFA529F398E562BB38AE891A03B212D88E98073DE85A5A00D9B2CA11D0B14AF22CD2C832FD8DCD5E4ADC6BA95FFC7FBEBC80FE9D32ACB4EB6F52C742B11F11D83655E30B5FC786E15CC8C8D1C9F9EF66284D337B8A697E4F8485064D28823FD0DB4E75F5618437D776D6910405AB88B467F2E5DD4C206C670807E25227E61D02EE32B37C731DA1DF141D36CC48B636867C37C6A08C17EE8416A0A33A0791EE2233C3915EC38B2DA1984DE95F00BEAA1575DBD1ACCF4F8DB37500656BE4CD7A874F6247A127C2F32A046197D4D2A"
#define MOD_A        "1AF0D18F357B883EDC58A9FBEE1DA7209BCE173BDA631D69D04836776CE6EDAA8613288505A31501F033B22ACCE8068A987F90CC6469F35201A9E8E4810C1DD812BF96365833E44B82915C0AF25683A02F204DAE8DE93D9EB836B6EADC0E41BA66380EECF797B43EDFFF9714EE314C74F6475E33727768ED85871E1304EB11D505475398875DBE383CEE96C32804444E258547A7BE969185AAA0DBB4E654926B36C77D8AE2D429CDBA4A88A52823526AFD3AFDF16F328304348A528C719BB0CEC0A05ABF4825A184F28B77C304CB456E8C216AEA282FC533D84BC56D61D24DBEFDCAE91A418FD6B8300B47E2E25C66591B149C03BC6CCDC325B55D85AFD2C772"
#define MOD_PC       "813BECFA66F1F46C"

#ifndef USE_CRYPT_LIB

#define     BN_TEST_LEN     (512)

void Add_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigR;
    uint8_t u8A, u8B, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
    }while(u16SLen < 16);
    
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnAdd(&BigR, &BigA, &BigB);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret = MHBN_Add(u8R,u8A,u8B,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum add test \n");
}

void Sub_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigR;
    uint8_t u8A, u8B, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
    }while(u16SLen < 16);
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnSub(&BigR, &BigA, &BigB);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret = MHBN_Sub(u8R,u8A,u8B,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum sub test \n");
}

void ModAdd_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigP, BigR;
    uint8_t u8A, u8B, u8P, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
        MHRAND_Prand(au8P, u16SLen);
    }while(u16SLen < 16);
    //make sure P is bigger than A B
    au8P[0] = 0xff;
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigP.pu8Num = au8P;
    BigP.u32Len = u16SLen;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnModAdd(&BigR, &BigA, &BigB, &BigP);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8P  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8P), au8P, u8DWord<<2, u16SLen);
    u32Ret = MHBN_ModAdd(u8R,u8A,u8B,u8P,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("P.\n", au8P, u16SLen);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum mod add test \n");
}

void ModSub_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigP, BigR;
    uint8_t u8A, u8B, u8P, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
        MHRAND_Prand(au8P, u16SLen);
    }while(u16SLen < 16);
    //make sure P is bigger than A B
    au8P[0] = 0xff;
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigP.pu8Num = au8P;
    BigP.u32Len = u16SLen;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnModSub(&BigR, &BigA, &BigB, &BigP);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8P  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8P), au8P, u8DWord<<2, u16SLen);
    u32Ret = MHBN_ModSub(u8R,u8A,u8B,u8P,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("P.\n", au8P, u16SLen);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum mod sub test \n");
}

void Mul_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8R1[BN_TEST_LEN*2], au8R2[BN_TEST_LEN*2];
    MHBN_BIGNUM BigA, BigB, BigR;
    uint8_t u8A, u8B, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0xff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0xff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
    }while(u16SLen < 16);
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLenA + u16SLenB;
    u32Ret = MHBN_BnMul(&BigR, &BigA, &BigB);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item*2;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret = MHBN_Mul(u8R,u8A,u8B,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLenA + u16SLenB, u8DWord<<3);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("R1.\n", au8R1, u16SLenA + u16SLenB);
//    ouputRes("R2.\n", au8R2, u16SLenA + u16SLenB);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum mul test \n");
}

void ModMul_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigP, BigR;
    MHBN_MOD BnMod;
    uint8_t u8A, u8B, u8P, u8PD, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t au32PC[2], u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
        MHRAND_Prand(au8P, u16SLen);
    }while(u16SLen < 16);
    //make sure P is bigger than A B    and P is odd
    au8P[0] = 0xff;
    au8P[u16SLen - 1] |= 0x01;
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigP.pu8Num = au8P;
    BigP.u32Len = u16SLen;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnModMul(&BigR, &BigA, &BigB, &BigP);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8P  = u8Spare; u8Spare += u8Item;
    u8PD = u8Spare;  u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8P), au8P, u8DWord<<2, u16SLen);
    
    MHBN_GetModMulConst(au32PC, u8P, u8DWord);
    MHBN_GetDomainConst(u8PD, u8P, au32PC, u8DWord, u8Spare);
    BnMod.u8P = u8P;    BnMod.u8PD = u8PD;      BnMod.pu32PC = au32PC;
    u32Ret = MHBN_ModMul(u8R,u8A,u8B,&BnMod,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("P.\n", au8P, u16SLen);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum mod mul test \n");
}

void Div_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8Q1[BN_TEST_LEN], au8Q2[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigR, BigQ;
    uint8_t u8A, u8B, u8R, u8Q, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
    }while((u16SLen < 16) || (u16SLenA < u16SLenB));
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigQ.pu8Num = au8Q1;
    BigQ.u32Len = u16SLen;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLenB;
    u32Ret = MHBN_BnDiv(&BigQ, &BigR, &BigA, &BigB);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8Q  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret = MHBN_Div(u8Q,u8R,u8A,u8B,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLenB, u8DWord<<2);
    MHMEM_CopyBig(au8Q2, MH_ITEM_TO_MEM(u8Q), u16SLen, u8DWord<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("Q1.\n", au8Q1, u16SLen);
//    ouputRes("Q2.\n", au8Q2, u16SLen);
//    ouputRes("R1.\n", au8R1, u16SLenB);
//    ouputRes("R2.\n", au8R2, u16SLenB);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLenB)), "bignum Div test \n");
    r_printf((0 == memcmp(au8Q1, au8Q2, u16SLen)), "bignum Div test \n");
}

void ModExp_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigP, BigR;
    MHBN_MOD BnMod;
    uint8_t u8A, u8B, u8P, u8PD, u8R, u8WordB, u8WordP, u8DWordP, u8DWordB, u8ItemP, u8ItemB, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t au32PC[2], u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x3f) + 1;
        u16SLenA = 17;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x3f) + 1;
        u16SLenB = 5;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
        MHRAND_Prand(au8P, u16SLen);
    }while(u16SLen < 16);
    //make sure P is bigger than A B    and P is odd
    au8P[0] = 0xff;
    au8P[u16SLen - 1] |= 0x01;
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    BigP.pu8Num = au8P;
    BigP.u32Len = u16SLen;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLen;
    u32Ret = MHBN_BnModExp(&BigR, &BigA, &BigB, &BigP);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8WordB = MH_ROUND_UP(u16SLenB, 16)>>2;
    u8DWordB = MH_ROUND_UP(u8WordB, 2);
    u8ItemB = MH_ROUND_UP(u8WordB, 8)>>3;
    u8WordP = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWordP = MH_ROUND_UP(u8WordP, 2);
    u8ItemP = MH_ROUND_UP(u8WordP, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ItemP;
    u8B  = u8Spare; u8Spare += u8ItemB;
    u8P  = u8Spare; u8Spare += u8ItemP;
    u8PD = u8Spare;  u8Spare += u8ItemP;
    u8R  = u8Spare; u8Spare += u8ItemP;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWordP<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWordB<<2, u16SLenB);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8P), au8P, u8DWordP<<2, u16SLen);
    
    MHBN_GetModMulConst(au32PC, u8P, u8DWordP);
    MHBN_GetDomainConst(u8PD, u8P, au32PC, u8DWordP, u8Spare);
    BnMod.u8P = u8P;    BnMod.u8PD = u8PD;      BnMod.pu32PC = au32PC;
    u32Ret = MHBN_ModExp(u8R,u8A,u8B,&BnMod,u8WordB,u8WordP);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWordP<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
//    ouputRes("P.\n", au8P, u16SLen);
//    ouputRes("R1.\n", au8R1, u16SLen);
//    ouputRes("R2.\n", au8R2, u16SLen);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLen)), "bignum mod Exp test \n");
}

#define RSA_E   "00010001"

void ModInv_Test()
{
    uint8_t au8E[4],au8A[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigP, BigR;
    MH_RSA_KEY_SPACE InBase;
    MH_RSA_KEY KeySrc;
    uint8_t u8A, u8P, u8R, u8WordP, u8DWordP,  u8ItemP, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenP;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0xff);
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenP, 2);
        u16SLenP = (u16SLenP & 0xff);
    }while((u16SLenA < 16) || (u16SLenP <= u16SLenA) || (u16SLenP < 0x20));
    //get Prime P
//    DBG_PRINT("%x\n", u16SLenP);
    bn_read_string_from_head((uint8_t *)au8E, sizeof(au8E), RSA_E);
    memset((uint8_t *)&KeySrc,0,sizeof(KeySrc));
    memset((uint8_t *)&InBase,0,sizeof(InBase));
    KeySrc.u16NBits = u16SLenP<<3;
    KeySrc.u16EBits = 32;
    KeySrc.pu8E = au8E;
    KeySrc.pu8N = au8P;
    KeySrc.u32Crc = MHCRC_CalcBuff(0xffff, &KeySrc, sizeof(MH_RSA_KEY)-4);
    MHRSA_GenKey(&KeySrc, &InBase, NULL, 1);
//    ouputRes("P.\n", au8P, u16SLenP);

    au8P[u16SLenP -1] &= ~0x01; 
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigP.pu8Num = au8P;
    BigP.u32Len = u16SLenP;
    BigR.pu8Num = au8R1;
    BigR.u32Len = u16SLenP;
    u32Ret = MHBN_BnModInv(&BigR, &BigA, &BigP);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    u8WordP = MH_ROUND_UP(u16SLenP, 4)>>2;
    u8DWordP = MH_ROUND_UP(u8WordP, 2);
    u8ItemP = MH_ROUND_UP(u8WordP, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ItemP;
    u8P  = u8Spare; u8Spare += u8ItemP;
    u8R  = u8Spare; u8Spare += u8ItemP;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWordP<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8P), au8P, u8DWordP<<2, u16SLenP);
    
    u32Ret = MHBN_ModInvSoft(u8R,u8A,u8P,u8WordP);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLenP, u8DWordP<<2);
//    DBG_PRINT("u32Ret = %x\n",u32Ret);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("P.\n", au8P, u16SLenP);
//    ouputRes("R1.\n", au8R1, u16SLenP);
//    ouputRes("R2.\n", au8R2, u16SLenP);
    r_printf((0 == memcmp(au8R1, au8R2, u16SLenP)), "bignum mod Inv test \n");
}

void Cmp_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB;
    uint8_t u8A, u8B, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret1 = 0, u32Ret2 = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        u16SLenB = u16SLenA;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
    }while(u16SLen < 16);
    BigA.pu8Num = au8A;
    BigA.u32Len = u16SLenA;
    BigB.pu8Num = au8B;
    BigB.u32Len = u16SLenB;
    u32Ret1 = MHBN_BnCmp(&BigA, &BigB);
//    DBG_PRINT("u32Ret1 = %x\n",u32Ret1);
    
    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret2 = MHBN_ItemCmpEfc(u8A,u8B,u8Word);
//    DBG_PRINT("u32Ret2 = %x\n",u32Ret2);
//    ouputRes("A.\n", au8A, u16SLenA);
//    ouputRes("B.\n", au8B, u16SLenB);
    r_printf((u32Ret1 == u32Ret2), "bignum cmp test \n");
}

void Xor_Test()
{
    uint8_t au8A[BN_TEST_LEN], au8B[BN_TEST_LEN], au8P[BN_TEST_LEN], au8R1[BN_TEST_LEN], au8R2[BN_TEST_LEN];
    MHBN_BIGNUM BigA, BigB, BigP, BigR;
    uint8_t u8A, u8B, u8P, u8R, u8Word, u8DWord, u8Item, u8Spare = 0; 
    uint16_t u16SLenA, u16SLenB, u16SLen;
    uint32_t u32Ret = 0;
    do
    {
        MHRAND_Prand(&u16SLenA, 2);
        u16SLenA = (u16SLenA & 0x1ff) + 1;
        MHRAND_Prand(au8A, u16SLenA);
        MHRAND_Prand(&u16SLenB, 2);
        u16SLenB = (u16SLenB & 0x1ff) + 1;
        MHRAND_Prand(au8B, u16SLenB);
        u16SLen = (u16SLenA > u16SLenB) ? u16SLenA : u16SLenB;
        MHRAND_Prand(au8P, u16SLen);
    }while(u16SLen < 16);
    //make sure P is bigger than A B

    u8Word = MH_ROUND_UP(u16SLen, 4)>>2;
    u8DWord = MH_ROUND_UP(u8Word, 2);
    u8Item = MH_ROUND_UP(u8Word, 8)>>3;
    u8A  = u8Spare; u8Spare += u8Item;
    u8B  = u8Spare; u8Spare += u8Item;
    u8P  = u8Spare; u8Spare += u8Item;
    u8R  = u8Spare; u8Spare += u8Item;
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8A), au8A, u8DWord<<2, u16SLenA);
    MHMEM_CopyBig(MH_ITEM_TO_MEM(u8B), au8B, u8DWord<<2, u16SLenB);
    u32Ret = MHBN_Xor(u8R,u8A,u8B,u8Word);
    MHMEM_CopyBig(au8R2, MH_ITEM_TO_MEM(u8R), u16SLen, u8DWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au8A, u16SLenA);
    ouputRes("B.\n", au8B, u16SLenB);
    ouputRes("R2.\n", au8R2, u16SLen);
}

void CRAM_Test()
{
    uint8_t u8Test = 0;
    *(uint8_t *)PMEMITEM[40] = 0xaa;
    u8Test = *(uint8_t *)PMEMITEM[40];
    DBG_PRINT("u8Test = %x\n",u8Test);
}

void SRAM_Test()
{
    uint8_t u8Test = 0;
    *(uint8_t *)0x20003800 = 0xaa;
    u8Test = *(uint8_t *)0x20003800;
    DBG_PRINT("u8Test = %x\n",u8Test);
}

void BigNum_Test()
{
    Add_Test();
    Sub_Test();
    ModAdd_Test();
    ModSub_Test();
    Mul_Test();
    ModMul_Test();
    Div_Test();
    ModExp_Test();
    ModInv_Test();
    Cmp_Test();
    Xor_Test();
}


#endif


#if 0
void mul_Test()
{
    uint32_t au32A[8];
    uint32_t au32B[8];
    uint32_t au32R[16];
    uint16_t u16LenByteA, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8R, u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), PARA_A);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), PARA_B);
    u16LenWord  =  MH_ROUND_UP(u16LenByteA,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    
    MHBN_Mul(u8R, u8A, u8B, u16LenWord);
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord << 3);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 3);
//    while(1);
}

void div_Test()
{
    uint32_t au32A[64];
    uint32_t au32B[64];
    uint32_t au32R[64];
    uint32_t au32Q[64];
    uint16_t u16LenByteA, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8R, u8Q, u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), PARA_A);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), PARA_B);
    u16LenWord  =  MH_ROUND_UP(u16LenByteA,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8Q = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare + u8ItemSize*3;
   
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    MHBN_Div(u8Q, u8R, u8A, u8B, u16LenWord); 
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    memcpy( au32Q,MH_ITEM_TO_MEM(u8Q), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("Q.\n", au32Q, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}

void modinv_Test()
{
    uint32_t au32A[18];
    uint32_t au32B[18];
    uint32_t au32R[18];
    uint16_t u16LenByteA, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8R,  u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), ECC_P);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), ECC_GX);
    u16LenWord  =  MH_ROUND_UP(u16LenByteA,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    
    if(MHBN_ModInv(u8R, u8B, u8A, u16LenWord) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The ModInv calc is error!\n");
        while(1);
    }
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
    while(1);
}

void modInvSoft_Test()
{
    uint32_t au32A[8];
    uint32_t au32B[8];
    uint32_t au32R[8];
    uint16_t u16LenByteA, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8R, u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), PARA_A);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), PARA_B);
    u16LenWord  =  MH_ROUND_UP(u16LenByteA,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    
    if(MHBN_ModInvSoft(u8R, u8B, u8A, u16LenWord) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The ModInv calc is error!\n");
        while(1);
    }
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}
void getMontConst_Test()
{
    uint32_t au32P[18];
    uint32_t au32R[2] = {0};
    uint16_t u16LenByte, u16LenWord;
    uint8_t u8ItemSize, u8P, u8Spare = 0;
    u16LenByte = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), ECC_P);
    u16LenWord  =  MH_ROUND_UP(u16LenByte,4) >> 2; 
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    u8P = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    MHBN_GetModMulConst(au32R, u8P, u16LenWord);
    ouputRes("R.\n", au32R, 8);
//    while(1);
}

void montModMul_Test()
{
    uint32_t au32A[12];
    uint32_t au32B[12];
    uint32_t au32P[12];
    uint32_t au32R[12];
    uint32_t au32PC[2];
    uint16_t u16LenByteP, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8P, u8R, u8Spare = 0;
    bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), MOD_A);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), MOD_B);
    u16LenByteP = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), MOD_P);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), MOD_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByteP,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8P = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare; u8Spare += u8ItemSize;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    
    if(MHBN_MontModMul(u8R, u8A, u8B, u8P, au32PC, u16LenWord, u8Spare) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The MontModMul calc is error!\n");
        while(1);
    }
    
    memcpy(au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}

void getDomainConst_Test()
{
    uint32_t au32P[18];
    uint32_t au32R[18];
    uint32_t au32PC[2];
    uint16_t u16LenByte, u16LenWord;
    uint8_t  u8ItemSize, u8P, u8R, u8Spare = 0;
    u16LenByte = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), ECC_P);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), ECC_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByte,4) >> 2; 
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize >>= MH_ASYMC_MEM_ITEM_WORD_BIT;
    u8P = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare; u8Spare += u8ItemSize;
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    MHBN_GetDomainConst(u8R, u8P, au32PC, u16LenWord, u8Spare);
    memcpy(au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
    while(1);   
}

void modMul_Test()
{
    uint32_t au32A[18];
    uint32_t au32B[18];
    uint32_t au32P[18];
    uint32_t au32PD[18];
    uint32_t au32R[18];
    uint32_t au32PC[2];
    MHBN_MOD BnMod;
    uint16_t u16LenByteP, u16LenWord;
    uint8_t u8ItemSize, u8A, u8B, u8P,  u8PD, u8R, u8Spare = 0;
    bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), ECC_GX);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), ECC_GY);
    u16LenByteP = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), ECC_P);
    bn_read_string_from_head((uint8_t *)au32PD, sizeof(au32PD), ECC_PD);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), ECC_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByteP,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A  = u8Spare; u8Spare += u8ItemSize;
    u8B  = u8Spare; u8Spare += u8ItemSize;
    u8P  = u8Spare; u8Spare += u8ItemSize;
    u8PD = u8Spare; u8Spare += u8ItemSize;
    u8R  = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8PD), au32PD, u16LenWord<<2);
    BnMod.u8P = u8P;
    BnMod.u8PD = u8PD;
    BnMod.pu32PC = au32PC; 
    
    if(MHBN_ModMul(u8R, u8A, u8B, &BnMod, u16LenWord) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The ModMul calc is error!\n");
        while(1);
    }
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenWord << 2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}

void MontModExp_Test()
{
    uint32_t au32A[64];
    uint32_t au32B[64];
    uint32_t au32P[64];
    uint32_t au32R[64];
    uint32_t au32PC[2];
    uint16_t u16LenByteB, u16LenByteP, u16LenWord, u16LenB;
    uint8_t u8ItemSize, u8A, u8B, u8P, u8R, u8Spare = 0;
    bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), MOD_A);
    u16LenByteB = bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), MOD_B);
    u16LenByteP = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), MOD_P);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), MOD_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByteP,4) >> 2;
    u16LenB     =  MH_ROUND_UP(u16LenByteB,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A = u8Spare; u8Spare += u8ItemSize;
    u8B = u8Spare; u8Spare += u8ItemSize;
    u8P = u8Spare; u8Spare += u8ItemSize;
    u8R = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    
    if(MHBN_MontModExp(u8R, u8A, u8B, u8P, au32PC, u16LenB, u16LenWord) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The MontModExp calc is error!\n");
        while(1);
    }
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenB << 2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
    
//    while(1);
}

void ModExp_Test()
{
    uint32_t au32A[64];
    uint32_t au32B[64];
    uint32_t au32P[64];
    uint32_t au32PD[64];
    uint32_t au32R[64];
    uint32_t au32PC[2];
    MHBN_MOD BnMod;
    uint16_t u16LenByteB, u16LenByteP, u16LenWord, u16LenB ;
    uint8_t u8ItemSize, u8A, u8B, u8P,  u8PD, u8R, u8Spare = 0;
    bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), MOD_A);
    u16LenByteB = bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), MOD_B);
    u16LenByteP = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), MOD_P);
//    bn_read_string_from_head((uint8_t *)au32PD, sizeof(au32PD), MOD_PD);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), MOD_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByteP,4) >> 2;
    u16LenB     =  MH_ROUND_UP(u16LenByteB,4) >> 2;
//    u16LenB     =  MH_ROUND_UP(u16LenByteB,16) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A  = u8Spare; u8Spare += u8ItemSize;
    u8B  = u8Spare; u8Spare += u8ItemSize;
    u8P  = u8Spare; u8Spare += u8ItemSize;
    u8PD = u8Spare; u8Spare += u8ItemSize;
    u8R  = u8Spare;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8PD), au32PD, u16LenWord<<2);
    BnMod.u8P = u8P;
    BnMod.u8PD = u8PD;
    BnMod.pu32PC = au32PC; 
    
    if(MHBN_ModExp(u8R, u8A, u8B, &BnMod, u16LenB, u16LenWord) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The ModExp calc is error!\n");
        while(1);
    }
    
    ouputRes("A.\n", au32A, u16LenWord << 2);
    ouputRes("B.\n", au32B, u16LenB << 2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}

void NonMul_Test()
{
    uint32_t au32A[32];
    uint32_t au32B[2];
    uint32_t au32R[32 + 2];
    uint16_t u16LenByteA, u16LenByteB, u16LenWordA, u16LenWordB, u16LenWordR;
    uint8_t u8ASize, u8A, u8B, u8R, u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), MUL_A);
    u16LenByteB = bn_read_string_from_head((uint8_t *)au32B, sizeof(au32B), MUL_B);
    u16LenWordA  =  MH_ROUND_UP(u16LenByteA, 8) >> 2; 
    u16LenWordB  =  MH_ROUND_UP(u16LenByteB, 8) >> 2; 
    u16LenWordR = u16LenWordA + u16LenWordB;
    u8ASize = MH_ROUND_UP(u16LenWordA, 8);
    u8ASize = u8ASize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A  = u8Spare; u8Spare += u8ASize;
    u8B  = u8Spare; u8Spare += 1;
    u8R  = u8Spare; u8Spare += u8ASize + 1;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenWordA<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16LenWordB<<2);
    
    if(MHBN_HalfLenMul(u8R, u8A, u8B, u16LenWordA, u16LenWordB, u8Spare) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The NonMul calc is error!\n");
        while(1);
    }
    
    memcpy( au32R,MH_ITEM_TO_MEM(u8R), u16LenWordR<<2);
    ouputRes("A.\n", au32A, u16LenWordA << 2);
    ouputRes("B.\n", au32B, u16LenWordB << 2);
    ouputRes("R.\n", au32R, u16LenWordR << 2);
    MHRAND_Prand(au32B, u16LenWordA << 2);
    ouputRes("B.\n", au32A, u16LenWordA << 2);
}

void Shift_Test()
{
    uint32_t au32A[32];
    uint32_t au32R[32];
    uint32_t u32Bits = 255;
    uint16_t u16AByte,u16AWord;
    uint8_t u8ASize, u8A, u8R, u8Spare = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), PARA_A);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;

    u8A  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    MHBN_ShiftLH(u8R,u8A,u16AWord,u32Bits);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    MHBN_ShiftRH(u8R,u8A,u16AWord,u32Bits);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}


#define    ADD_SRC_A     "00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
#define    ADD_SRC_B     "00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1"
void Add_Test()
{
    uint8_t u8ASize, u8A, u8R, u8B, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t au32B[32];
    uint32_t au32R[32];
    uint32_t u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(ADD_SRC_A), ADD_SRC_A);
    bn_read_string_from_head((uint8_t *)au32B, sizeof(ADD_SRC_B), ADD_SRC_B);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    u8B  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8B), au32B, u16AWord<<2);
    u32Ret = MHBN_Add(u8R,u8A,u8B,u16AWord-1);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("B.\n", au32B, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}
#define    ADDONE_SRC_DATA     "00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
void AddOne_Test()
{
    uint8_t u8ASize, u8A, u8R, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t au32R[32];
    uint32_t u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(ADDONE_SRC_DATA), ADDONE_SRC_DATA);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    u32Ret = MHBN_AddOne(u8R,u8A,u16AWord - 1);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}


#define    ADDU32_SRC_DATA     "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"

void Addu32_Test()
{
    uint8_t u8ASize, u8A, u8R, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t au32R[32];
    uint32_t u32B, u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(ADDU32_SRC_DATA), ADDU32_SRC_DATA);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    u32B = 0x12345678;
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    u32Ret = MHBN_AddU32(u8R,u8A,u32B,u16AWord);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}

#define    SUBONE_SRC_DATA     "000000000100000000000000000000000000000000000000"
void SubOne_Test()
{
    uint8_t u8ASize, u8A, u8R, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t au32R[32];
    uint32_t u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(SUBONE_SRC_DATA), SUBONE_SRC_DATA);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    u32Ret = MHBN_SubOne(u8R,u8A,u16AWord - 1);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}

#define   SUBU32_SRC_DATA     "000000000000000000000000000000000000000012345678"

void Subu32_Test()
{
    uint8_t u8ASize, u8A, u8R, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t au32R[32];
    uint32_t u32B, u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(SUBU32_SRC_DATA), SUBU32_SRC_DATA);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    u32B = 0x21234567;
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    u8R  = u8Spare; u8Spare += u8ASize;
    memset(MH_ITEM_TO_MEM(u8R), 0, u16AWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    u32Ret = MHBN_SubU32(u8R,u8A,u32B,u16AWord-1);
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16AWord<<2);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
    ouputRes("A.\n", au32A, u16AWord << 2);
    ouputRes("R.\n", au32R, u16AWord << 2);
}

#define    CMP_SRC_DATA     "000000000000000000000000543210AB"
void CmpInt_Test()
{
    uint8_t u8ASize, u8A, u8Spare = 0;
    uint16_t u16AByte,u16AWord;
    uint32_t au32A[32];
    uint32_t u32B, u32Ret = 0;
    u16AByte = bn_read_string_from_head((uint8_t *)au32A, sizeof(CMP_SRC_DATA), CMP_SRC_DATA);
    u16AWord = MH_ROUND_UP(u16AByte, 4)>>2;
    u32B = 0x543210FB;
    u8ASize = MH_ROUND_UP(u16AWord, 8)>>3;
    u8A  = u8Spare; u8Spare += u8ASize;
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16AWord<<2);
    u32Ret = MHBN_ItemCmpIntEfc(u8A,u32B,u16AWord - 1);
    DBG_PRINT("u32Ret = %x\n",u32Ret);
}

//#define MONT_SRC_A   "B290D62F2D6B64831D47750C1DDAAA4D254FA3F14A069263FD4C4DB2CEE16430FF20BC04997ED7368B7BEFAC157CF5D5"
//#define MONT_SRC_P   "00000000CEE16430FF20BC04997ED7368B7BEFAC157CF5D5"
//#define MONT_SRC_PC  "83646B2D2FD690B2" //B290D62F2D6B6483
//#define MONT_SRC_PD  "000000000A54645BDADB6AA669ADCDA31A635F0DAFC8DFDA"

#define MONT_SRC_A   "39E86CB53BEDFDAAB290D62F2D6B64831D47750C1DDAAA4D254FA3F14A069263FD4C4DB2CEE16430FF20BC04997ED7368B7BEFAC157CF5D5"
#define MONT_SRC_P   "027789F4F8954AF8B5F43A6EC17179F7"
#define MONT_SRC_PC  "39E86CB53BEDFDAA" //AAFDED3BB56CE839
#define MONT_SRC_PD  "026376E6AF46A04C17028F89ABE12FDC"

void MontMod_Test()
{
    uint32_t au32A[32];
    uint32_t au32P[32];
    uint32_t au32PD[32];
    uint32_t au32R[32];
    uint32_t au32PC[2];
    MHBN_MOD BnMod;
    uint16_t u16LenByteA, u16LenByteP, u16LenWord, u16LenA ;
    uint8_t u8ItemSize, u8ItemSizeA, u8A, u8P, u8PD, u8R, u8Spare = 0;
    u16LenByteA = bn_read_string_from_head((uint8_t *)au32A, sizeof(au32A), MONT_SRC_A);
    u16LenByteP = bn_read_string_from_head((uint8_t *)au32P, sizeof(au32P), MONT_SRC_P);
    bn_read_string_from_head((uint8_t *)au32PD, sizeof(au32PD), MONT_SRC_PD);
    bn_read_string_from_head((uint8_t *)au32PC, sizeof(au32PC), MONT_SRC_PC);
    u16LenWord  =  MH_ROUND_UP(u16LenByteP,4) >> 2;
    u16LenA     =  MH_ROUND_UP(u16LenByteA,4) >> 2;
    
    u8ItemSize = MH_ROUND_UP(u16LenWord, 8);
    u8ItemSize = u8ItemSize >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    u8ItemSizeA = MH_ROUND_UP(u16LenA, 8);
    u8ItemSizeA = u8ItemSizeA >> MH_ASYMC_MEM_ITEM_WORD_BIT;
    
    u8A  = u8Spare; u8Spare += u8ItemSizeA;
    u8P  = u8Spare; u8Spare += u8ItemSize;
    u8PD = u8Spare; u8Spare += u8ItemSize;
    u8R  = u8Spare; u8Spare += u8ItemSize;
    
    memcpy(MH_ITEM_TO_MEM(u8A), au32A, u16LenA<<2);
    memcpy(MH_ITEM_TO_MEM(u8P), au32P, u16LenWord<<2);
    memcpy(MH_ITEM_TO_MEM(u8PD), au32PD, u16LenWord<<2);
    BnMod.u8P = u8P;
    BnMod.u8PD = u8PD;
    BnMod.pu32PC = au32PC; 
    
    if(MHBN_MontMod(u8R, u8A, &BnMod, u16LenA, u16LenWord, u8Spare) != MH_RET_CRYPT_SUCCESS)
    {
        DBG_PRINT("The MontMod calc is error!\n");
        while(1);
    }
    memcpy(au32R, MH_ITEM_TO_MEM(u8R), u16LenWord<<2);
    ouputRes("A.\n", au32A, u16LenA << 2);
    ouputRes("P.\n", au32P, u16LenWord << 2);
    ouputRes("R.\n", au32R, u16LenWord << 2);
//    while(1);
}


void BIGNUM_Test()
{
    Add_Test();
    AddOne_Test();
    SubOne_Test();
    Addu32_Test();
    Subu32_Test();
    CmpInt_Test();
    MontMod_Test();
    while(1);
}
#endif
