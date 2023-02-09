#include <string.h>
#include <stdio.h>
#include "mh32f10x.h"
#include "mh_sm4.h"
#include "debug.h"
#include "mh_rand.h"


#if USE_CRYPT
#include "cryptlib.h"
#endif


#if 0
void mh_SM4_test_with_known_data0(void);
void mh_SM4_test_with_known_data1(void);
void mh_SM4_test_with_known_data2(void);

void mh_SM4_test_with_known_data(void)
{
    mh_SM4_test_with_known_data0();
    mh_SM4_test_with_known_data1();
    mh_SM4_test_with_known_data2();
}

void SM4_Test(void)
{   
    uint32_t t;
    uint8_t Key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t IV[16] = {0xA3, 0xB1, 0xBA, 0xC6, 0x56, 0xAA, 0x33, 0x50, 0x67, 0x7D, 0x91, 0x97, 0xB2, 0x70, 0x22, 0xDC};
    uint8_t au8IV[16] = {0xA3, 0xB1, 0xBA, 0xC6, 0x56, 0xAA, 0x33, 0x50, 0x67, 0x7D, 0x91, 0x97, 0xB2, 0x70, 0x22, 0xDC};
    uint8_t Plain[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t Crypt[16] = {0x68, 0x1E, 0xDF, 0x34, 0xD2, 0x06, 0x96, 0x5E, 0x86, 0xB3, 0xE9, 0x4F, 0x53, 0x6E, 0x42, 0x46};
    uint8_t mPlain[16];
    uint8_t mCrypt[16];    
    uint8_t au8IVUse[16];

    uint8_t xPlain[2][32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t xCrypt[32];

    MH_SYM_CRYPT_CALL callSm4;

    DBG_PRINT("\nSM4 Test In\n");
    //ECB
//    memset(mCrypt, 0, sizeof(mCrypt));
//    memset(mPlain, 0, sizeof(mPlain));

//    memcpy(IV, au8IV, sizeof(au8IV));
//    callSm4.pu8In = Plain;
//    callSm4.u32InLen = sizeof(Plain);
//    callSm4.pu8Out = mCrypt;
//    callSm4.u32OutLen = sizeof(mCrypt);
//    callSm4.pu8IV = IV;
//    callSm4.pu8Key = Key;
//    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand), NULL);
//    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_ECB;
//    callSm4.u32Crc = 0;
//        
//    MHSM4_EncDec(&callSm4);
//    t = (!memcmp(mCrypt, Crypt, sizeof(Crypt)));
//    r_printf(t, "SM4 ECB Encrypt Test\n");

//    memset(mCrypt,0,sizeof(mCrypt));
//    memcpy(IV, au8IV, sizeof(au8IV));
//    callSm4.pu8In = Crypt;
//    callSm4.u32InLen = sizeof(Crypt);
//    callSm4.pu8Out = mPlain;
//    callSm4.u32OutLen = sizeof(mPlain);
//    callSm4.pu8IV = IV;
//    callSm4.pu8Key = Key;
//    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand), NULL);
//    callSm4.u16Opt = MH_AES_OPT_MODE_DECRYPT | MH_AES_OPT_BLK_ECB;
//    callSm4.u32Crc = 0;

//    MHSM4_EncDec(&callSm4);
//    t = (!memcmp(mPlain, Plain, sizeof(Plain)));
//    r_printf(t, "SM4 ECB Decrypt Test\n");
//    memset(mPlain,0,sizeof(mPlain));

  
    //CBC   
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = mCrypt;
    callSm4.u32OutLen = sizeof(mCrypt);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);
    //GPIO_ResetBits(GPIO,GPIO_Pin_5); 
    MHSM4_EncDec(&callSm4);
    //GPIO_SetBits(GPIO,GPIO_Pin_5); 
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = mCrypt;
    callSm4.u32InLen = sizeof(mCrypt);
    callSm4.pu8Out = mPlain;
    callSm4.u32OutLen = sizeof(mPlain);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);
    //GPIO_ResetBits(GPIO,GPIO_Pin_5); 
    MHSM4_EncDec(&callSm4);
    //GPIO_SetBits(GPIO,GPIO_Pin_5);
//  mh_sm4_dec(CBC, mPlain, sizeof(mPlain), mCrypt, sizeof(mCrypt), Key, Iv, MHRAND_Prand, NULL);
    
    t = (!memcmp(mPlain, Plain, sizeof(Plain)));
    r_printf(t, "SM4 CBC Test\n");
    
    //xCBC
//    memset(xCrypt, 0, sizeof(xCrypt));
////  memset(xPlain, 0, sizeof(xPlain));
////  
////  mh_rand(xPlain[0], sizeof(xPlain[0]));
//    
//    memcpy(IV, au8IV, sizeof(au8IV));
//    callSm4.pu8In = xPlain[0];
//    callSm4.u32InLen = sizeof(xPlain[0]);
//    callSm4.pu8Out = xCrypt;
//    callSm4.u32OutLen = sizeof(xCrypt);
//    callSm4.pu8IV = IV;
//    callSm4.pu8Key = Key;
//    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand), NULL);
//    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_CBC;
//    callSm4.u32Crc = 0;
//    MHSM4_EncDec(&callSm4);
////    mh_sm4_enc(CBC, xCrypt, sizeof(xCrypt), xPlain[0], sizeof(xPlain[0]), Key, au8IVUse, MHRAND_Prand, NULL);
//    //memcpy(au8IVUse, Iv, sizeof(Iv));
//    memcpy(IV, au8IV, sizeof(au8IV));
//    callSm4.pu8In = xCrypt;
//    callSm4.u32InLen = sizeof(xCrypt);
//    callSm4.pu8Out = xPlain[1];
//    callSm4.u32OutLen = sizeof(xPlain[1]);
//    callSm4.pu8IV = IV;
//    callSm4.pu8Key = Key;
//    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand), NULL);
//    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_CBC;
//    callSm4.u32Crc = 0;

//    MHSM4_EncDec(&callSm4);
//    //mh_sm4_dec(CBC, xPlain[1], sizeof(xPlain[1]), xCrypt, sizeof(xCrypt), Key, au8IVUse, MHRAND_Prand, NULL);
//    t = (!memcmp(xPlain[1], xPlain[0], sizeof(xPlain[1])));
//    r_printf(t, "SM4 CBC xbytes Test\n");
    
//    mh_SM4_test_with_known_data();
}


void mh_SM4_test_with_known_data0(void)
{
    uint32_t t;
    uint8_t Plain[16 * 4] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                            0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    uint8_t Key[16] = {0xb1,0x22,0xad,0x0a,0x7a,0x36,0x2e,0xc3,0xab,0xa1,0xdd,0xef,0xb3,0xaf,0x49,0x15};
    uint8_t IV[16] = {0x11,0x11,0x11,0x11,0xd1,0xa1,0x6c,0x20,0x67,0x70,0x8a,0xcb,0xd1,0xa1,0x6c,0x20};
    uint8_t au8IV[16] = {0x11,0x11,0x11,0x11,0xd1,0xa1,0x6c,0x20,0x67,0x70,0x8a,0xcb,0xd1,0xa1,0x6c,0x20};
    uint8_t Crypt_ECB[16 * 4] = {0xab,0x54,0xe1,0x1d,0x8b,0xc0,0xde,0x81,0x35,0x3e,0x41,0x0c,0x45,0x0e,0xd8,0xc1,0x09,0x2f,0xde,0xb9,0x83,0xf4,0xe0,0x0e,0x69,0x54,0x95,0x18,0x7d,0xcc,0xfb,0x93,0xfb,0xbd,0xda,0xa6,0x35,0x4f,0xaa,0xf9,0xe6,0x62,0xab,0x62,0x2b,0x31,0x02,0x78,0x05,0x59,0xcf,0x0d,0xa0,0x3a,0x54,0xbf,0x79,0xb8,0xeb,0xb6,0x08,0xce,0x36,0x30};
    uint8_t Crypt_CBC[16 * 4] = {0x59,0x40,0x8a,0x35,0x1e,0xd4,0xe0,0x0d,0xa6,0xc3,0xf9,0xc2,0x27,0xa9,0x73,0x8d,0x07,0xbf,0x44,0x4d,0x66,0x79,0xbd,0x5c,0x87,0xda,0x21,0xb5,0xac,0xc5,0x4a,0x39,0x40,0x26,0xdc,0x35,0x70,0x57,0x46,0x69,0xb7,0x15,0x55,0xc0,0xf3,0x2a,0xcb,0x71,0x4c,0x15,0x20,0xcd,0x77,0xce,0xeb,0x1a,0x16,0x67,0x0a,0x63,0x4a,0xdf,0x46,0x29};

    uint8_t Crypt2[16 * 4] = {0};
    uint8_t Plain2[16 * 4] = {0};
    uint8_t au8IVUse[16];
    MH_SYM_CRYPT_CALL callSm4;

    DBG_PRINT("\n%s\n", __func__);

    //ECB
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(ECB, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_ECB, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 ECB Encrypt Test\n");

    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_ECB;
    callSm4.u32InLen = sizeof(Crypt_ECB);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;

    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(ECB, Plain2, sizeof(Plain2), Crypt_ECB, sizeof(Crypt_ECB), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 ECB Decrypt Test\n");

    
    //CBC
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
            
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(CBC, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_CBC, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 CBC Encrypt Test\n");
    
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_CBC;
    callSm4.u32InLen = sizeof(Crypt_CBC);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(CBC, Plain2, sizeof(Plain2), Crypt_CBC, sizeof(Crypt_CBC), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 CBC Decrypt Test\n");
}


void mh_SM4_test_with_known_data1(void)
{
    uint32_t t;
    uint8_t Plain[16 * 4] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                            0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    uint8_t Key[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
    uint8_t IV[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
    uint8_t au8IV[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
    uint8_t Crypt_ECB[16 * 4] = {0x6c,0x1d,0xd6,0xaf,0xca,0xa9,0x43,0x1e,0x4c,0x26,0xb3,0xcd,0xde,0x0a,0x73,0x24,0xd7,0xc6,0x4c,0x78,0x75,0x79,0x83,0xf8,0xb8,0xa2,0x92,0x19,0xf0,0x90,0x5c,0x9e,0x31,0xc2,0xe7,0xfb,0xcb,0x22,0x67,0x3f,0xd5,0xa1,0xcb,0x44,0xa7,0x4b,0x04,0x1b,0x2e,0xe8,0xdc,0xe4,0x63,0x41,0xf0,0x08,0xe6,0xfe,0x1c,0x1a,0x7b,0x14,0x2a,0x30};
    uint8_t Crypt_CBC[16 * 4] = {0xcc,0x86,0x86,0x4b,0xb7,0x9f,0x4e,0x58,0xba,0x83,0x3f,0xb4,0x87,0x21,0x39,0xfd,0x5d,0x04,0x2a,0xa3,0x27,0x29,0x72,0x28,0x76,0x72,0x56,0x60,0x2f,0x47,0x1a,0xc8,0xf9,0x38,0xab,0x9e,0x25,0x4a,0xbc,0x74,0x2c,0x68,0xda,0xd8,0x0f,0x7a,0xa5,0x30,0x5a,0x64,0x03,0xd5,0x93,0x87,0x71,0x15,0x8f,0x49,0x0d,0x52,0x80,0xb8,0x18,0xe6};

    uint8_t Crypt2[16 * 4] = {0};
    uint8_t Plain2[16 * 4] = {0};
    uint8_t au8IVUse[16];
    MH_SYM_CRYPT_CALL callSm4;

    DBG_PRINT("\n%s\n", __func__);

    //ECB
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
    
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(ECB, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_ECB, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 ECB Encrypt Test\n");

    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_ECB;
    callSm4.u32InLen = sizeof(Crypt_ECB);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;

    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(ECB, Plain2, sizeof(Plain2), Crypt_ECB, sizeof(Crypt_ECB), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 ECB Decrypt Test\n");

    
    //CBC
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(CBC, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_CBC, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 CBC Encrypt Test\n");
    
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_CBC;
    callSm4.u32InLen = sizeof(Crypt_CBC);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(CBC, Plain2, sizeof(Plain2), Crypt_CBC, sizeof(Crypt_CBC), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 CBC Decrypt Test\n");
}

void mh_SM4_test_with_known_data2(void)
{
    uint32_t t;
    uint8_t Plain[16 * 4] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                            0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
                            0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
                            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
    uint8_t Key[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0xab,0xa1,0xdd,0xef,0xb3,0xaf,0x49,0x15};
    uint8_t IV[16] = {0x11,0x11,0x11,0x11,0xd1,0xa1,0x6c,0x20,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
    uint8_t au8IV[16] = {0x11,0x11,0x11,0x11,0xd1,0xa1,0x6c,0x20,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
    uint8_t Crypt_ECB[16 * 4] = {0x59,0x0b,0xf4,0xfe,0xc7,0x08,0xdb,0x75,0x62,0xca,0x73,0xe3,0xc1,0x5f,0x51,0x35,0x2f,0xb4,0xf3,0x27,0xdd,0xb5,0x54,0xcd,0x75,0xb9,0x9e,0x2c,0x2e,0x45,0x85,0xee,0xc3,0xb7,0xe6,0x45,0xe3,0x94,0x34,0x74,0xbc,0x95,0x72,0x9e,0xe2,0x3b,0xb9,0x19,0xe7,0xd0,0x5b,0x0a,0x57,0x8d,0x8b,0x5b,0x9c,0x0c,0x87,0x5a,0x1a,0x7c,0xa6,0xd2};
    uint8_t Crypt_CBC[16 * 4] = {0x21,0x37,0xe3,0x20,0x95,0xcf,0x11,0x5f,0x4f,0x49,0xf6,0x84,0xfd,0xe2,0x43,0x69,0x9f,0xb5,0xb9,0x4f,0xae,0x41,0xbf,0x39,0x94,0x46,0x8e,0x81,0x0a,0x8e,0xf4,0xb2,0x0f,0xbf,0x6e,0x0c,0x4c,0x89,0x5b,0xe2,0x0c,0x86,0xb5,0x6c,0x89,0x9a,0x10,0x77,0xb9,0x61,0x67,0xb7,0x19,0x50,0x39,0xc0,0x70,0xfb,0xb7,0x33,0xb0,0x1f,0xee,0xdb};

    uint8_t Crypt2[16 * 4] = {0};
    uint8_t Plain2[16 * 4] = {0};
    uint8_t au8IVUse[16];
    MH_SYM_CRYPT_CALL callSm4;

    DBG_PRINT("\n%s\n", __func__);

    //ECB
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(ECB, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_ECB, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 ECB Encrypt Test\n");

    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_ECB;
    callSm4.u32InLen = sizeof(Crypt_ECB);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_ECB;
    callSm4.u32Crc = 0;

    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(ECB, Plain2, sizeof(Plain2), Crypt_ECB, sizeof(Crypt_ECB), Key, NULL, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 ECB Decrypt Test\n");

    
    //CBC
    memset(Plain2, 0, sizeof(Plain2));
    memset(Crypt2, 0, sizeof(Crypt2));
    
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Plain;
    callSm4.u32InLen = sizeof(Plain);
    callSm4.pu8Out = Crypt2;
    callSm4.u32OutLen = sizeof(Crypt2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_enc(CBC, Crypt2, sizeof(Crypt2), Plain, sizeof(Plain), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Crypt_CBC, Crypt2, sizeof(Crypt2)));
    r_printf(t, "SM4 CBC Encrypt Test\n");
    
    memcpy(IV, au8IV, sizeof(au8IV));
    callSm4.pu8In = Crypt_CBC;
    callSm4.u32InLen = sizeof(Crypt_CBC);
    callSm4.pu8Out = Plain2;
    callSm4.u32OutLen = sizeof(Plain2);
    callSm4.pu8IV = IV;
    callSm4.pu8Key = Key;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u16Opt = MH_SM4_OPT_MODE_DECRYPT | MH_SM4_OPT_BLK_CBC;
    callSm4.u32Crc = 0;
    MHSM4_EncDec(&callSm4);
    //mh_sm4_dec(CBC, Plain2, sizeof(Plain2), Crypt_CBC, sizeof(Crypt_CBC), Key, au8IVUse, MHRAND_Prand, NULL);
    t = (!memcmp(Plain2, Plain, sizeof(Plain)));
    r_printf(t, "SM4 CBC Decrypt Test\n");
}
#else

#define SM4_TIME_TEST_DATA  (128)

typedef struct
{
    uint32_t    u32Val;
    char *      pu8Name;
}SM4_MODE_TYP;

void SM4_Rand_Test(void)
{
    uint32_t i, t;
    uint32_t u32MsgLen = 0;
    MH_SYM_CRYPT_CALL callSm4;  
    SM4_MODE_TYP ModeType[2] = {{MH_SM4_OPT_BLK_ECB, "ECB"},
                             {MH_SM4_OPT_BLK_CBC, "CBC"}};
    uint8_t au8Plain[2][SM4_TIME_TEST_DATA];
    uint8_t au8Cipher[SM4_TIME_TEST_DATA]; 
    uint8_t au8Iv[16] = {0};
    uint8_t au8Key[16] = {0};
    uint8_t au8IvTmp[16] = {0};
    DBG_PRINT("\nSM4 Test Start\n");
    memset(&callSm4, 0, sizeof(callSm4));
    for (i = 0; i < 2; i++)
    {
        memset(au8Plain, 0, sizeof(au8Plain));
        memset(au8Cipher, 0, sizeof(au8Cipher));
        memset(au8Iv,0,sizeof(au8Iv));
        memset(au8Key,0,sizeof(au8Key));
        MHRAND_Prand(&u32MsgLen,sizeof(u32MsgLen));
        u32MsgLen = (u32MsgLen & 0x7f) + 1;
//        u32MsgLen = 32;
        MHRAND_Prand(au8Plain,u32MsgLen);
        MHRAND_Prand(au8Iv,sizeof(au8Iv));
        MHRAND_Prand(au8Key,sizeof(au8Key));
        //DES Enc
		memset((uint8_t *)&callSm4, 0, sizeof(callSm4));
        callSm4.pu8In = au8Plain[0];
        callSm4.pu8Out = au8Cipher;
        callSm4.pu8IV = au8Iv;
        callSm4.pu8Key = au8Key;
        callSm4.u32InLen = u32MsgLen;
        callSm4.u32OutLen = ((u32MsgLen + 15)>>4)<<4;
        callSm4.u16Opt = MH_SM4_OPT_MODE_ENCRYPT | ModeType[i].u32Val;
        callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);
        if(MH_RET_SM4_SUCCESS != MHSM4_EncDec(&callSm4))
        {
            r_printf(0, "SM4 enc test\n");
        }
        //DES Dec
        callSm4.pu8In = au8Cipher;
        callSm4.u32InLen = ((u32MsgLen + 15)>>4)<<4;
        callSm4.pu8Out = au8Plain[1];
        callSm4.u32OutLen = ((u32MsgLen + 15)>>4)<<4;
        callSm4.pu8IV = au8Iv;
        callSm4.pu8Key = au8Key;
//        MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
        callSm4.u16Opt = ModeType[i].u32Val | MH_SM4_OPT_MODE_DECRYPT;
        callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);
        if(MH_RET_SM4_SUCCESS != MHSM4_EncDec(&callSm4))
        {
            r_printf(0, "SM4 dec test\n");
        }                    
        t = (!memcmp(au8Plain[0], au8Plain[1], u32MsgLen));
        DBG_PRINT("%s SM4 Test ", ModeType[i].pu8Name);
        r_printf(t, "\n");
    }
}

#define SM4_TEST_LEN    (64)
void SM4_Func_Test()
{
    uint8_t au8Plain[SM4_TEST_LEN] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                            0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
                            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
                            0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40};
    uint8_t au8SM4Cipher_ECB[SM4_TEST_LEN] = {0xA6, 0x38, 0x95, 0x5D, 0x5B, 0xCB, 0x14, 0x94, 0x48, 0x8A, 0x29, 0x49, 0xA1, 0x5F, 0xD1, 0x38,
                                    0x43, 0x29, 0xA6, 0x24, 0x1E, 0x39, 0xAD, 0x7A, 0x9A, 0x40, 0x4A, 0x81, 0x4A, 0x7E, 0xDD, 0x32,
                                    0x7D, 0xCF, 0x36, 0x31, 0xCF, 0x64, 0x07, 0x99, 0x0E, 0x84, 0x4D, 0xE9, 0x85, 0xC3, 0xCA, 0xCA,
                                    0x81, 0x3A, 0xB8, 0xD4, 0xF4, 0xBB, 0x66, 0x72, 0x9F, 0x30, 0x7F, 0x92, 0x30, 0x10, 0x00, 0x43};
    uint8_t au8SM4Cipher_CBC[SM4_TEST_LEN] = {0xDD, 0x60, 0x70, 0x8C, 0xF5, 0xE1, 0xD9, 0x66, 0x40, 0xA5, 0xE1, 0x5D, 0x95, 0x61, 0x3F, 0xAC,
                                    0x52,0xB9, 0x6A, 0xE4, 0xDF, 0xF2, 0x97, 0x57, 0x46, 0x48, 0xDA, 0x86, 0x5B, 0x01, 0xD4, 0xBF,
                                    0x22,0xD5, 0x1F, 0x2E, 0x8E, 0xCC, 0xAE, 0x85, 0xF9, 0x21, 0x42, 0xBE, 0x91, 0xE5, 0x53, 0xB0,
                                    0x9E,0x81, 0x10, 0x13, 0x6F, 0xB6, 0xDC, 0xBF, 0xB2, 0x25, 0x94, 0x93, 0xD5, 0x22, 0x2D, 0x49};
    uint8_t au8Result[SM4_TEST_LEN] = {0};
    uint8_t au8Iv[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};    
    uint8_t au8Key[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    MH_SYM_CRYPT_CALL callSm4; 

    memset(&callSm4, 0, sizeof(callSm4));
    //ECB ENC
    callSm4.pu8In = au8Plain;
    callSm4.u32InLen = SM4_TEST_LEN;
    callSm4.pu8Out = au8Result;
    callSm4.u32OutLen = SM4_TEST_LEN;                
    callSm4.pu8Key = au8Key;
    callSm4.u16Opt = MH_SM4_OPT_BLK_ECB | MH_SM4_OPT_MODE_ENCRYPT;
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);  
    MHSM4_EncDec(&callSm4);
    r_printf((0 == memcmp(au8SM4Cipher_ECB, au8Result, SM4_TEST_LEN)), "mh_sm4_ecb test\n");
    
    //CBC DEC
    callSm4.pu8In = au8SM4Cipher_CBC;
    callSm4.u32InLen = SM4_TEST_LEN;
    callSm4.pu8Out = au8Result;
    callSm4.u32OutLen = SM4_TEST_LEN;                
    callSm4.pu8IV = au8Iv;
    callSm4.u16Opt = MH_SM4_OPT_BLK_CBC | MH_SM4_OPT_MODE_DECRYPT;
    callSm4.pu8Key = au8Key;
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);  
    MHSM4_EncDec(&callSm4);
    r_printf((0 == memcmp(au8Plain, au8Result, SM4_TEST_LEN)), "mh_sm4_cbc test\n");
}

void SM4_Test()
{
    SM4_Func_Test();
    SM4_Rand_Test();
}

#endif

#if 0
void SM4_UpdateTest()
{
    MH_SYM_CRYPT_CALL callSm4; 
    uint32_t u32MsgLen = 0;
    uint8_t au8Plain[2][32];
    uint8_t au8Cipher[2][32];
    uint8_t au8Iv[16] = {0};
    uint8_t au8IvTmp[16] = {0};
    uint8_t au8Key[16] = {0}; 
    u32MsgLen = 32;
    MHRAND_Prand(au8Iv,sizeof(au8Iv));
    memcpy(au8IvTmp, au8Iv, sizeof(au8Iv));
    MHRAND_Prand(au8Key,sizeof(au8Key));
    MHRAND_Prand(au8Plain[0],u32MsgLen);
    ouputRes("au8Plain: ", au8Plain[0], 32);
    ouputRes("au8Iv: ", au8Iv, 16);
    ouputRes("au8Key128: ", au8Key, 16);
    callSm4.pu8In = au8Plain[0];
    callSm4.u32InLen = u32MsgLen;
    callSm4.pu8Out = au8Cipher[0];
    callSm4.u32OutLen = ((u32MsgLen + 15)>>4)<<4;                
    callSm4.pu8IV = au8IvTmp;
    callSm4.pu8Key = au8Key;
    callSm4.u16Opt = MH_SM4_OPT_BLK_ECB| MH_SM4_OPT_MODE_ENCRYPT;
    MHRAND_Prand(&callSm4.u32Rand, sizeof(callSm4.u32Rand));
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);               
    if(MH_RET_SM4_SUCCESS != MHSM4_EncDec(&callSm4))
    {
        r_printf(0, "SM4 enc test\n");
    }
    ouputRes("au8IvTmp: ", au8IvTmp, 16);
    ouputRes("au8Cipher: ", au8Cipher, 32);
    callSm4.pu8Out = au8Cipher[1];
    callSm4.u32Crc = MHCRC_CalcBuff(0xffff, &callSm4, sizeof(MH_SYM_CRYPT_CALL)-4);               
    if(MH_RET_SM4_SUCCESS != MHSM4_EncDec(&callSm4))
    {
        r_printf(0, "SM4 enc test\n");
    }
    ouputRes("au8Cipher: ", au8Cipher, 64);
}
#endif
