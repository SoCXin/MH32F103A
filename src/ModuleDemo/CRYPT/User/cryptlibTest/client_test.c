/***************************************
    说明：客户需要重新封装几个函数
***************************************/

//#include <string.h>
//#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "mh_crypt.h"
#include "mh_des.h"
#include "mh_rand.h"
#include "mh_bignum.h"
#include "mh_sm2.h"
#include "mh_sm3.h"
#include "mh_sm4.h"
#include "mh_aes.h"
#include "mh_bignum_tool.h"
#include "mh_rsa.h"


typedef uint8_t  uchar;
typedef uint32_t uint;



extern uint32_t bn_s_get_C(uint32_t *r,uint32_t *p,uint32_t len, mh_rng_callback f_rng, void *p_rng);
extern uint32_t bn_s_get_Q(uint32_t *r,uint32_t *p, mh_rng_callback f_rng, void *p_rng);
extern uint32_t mh_rsa_gen_key(mh_rsa_private_key *key, uint32_t exponent, uint32_t nbits,
                        mh_rng_callback f_rng, void *p_rng);

#define MH_RET_RSA_SUCCESS                    (('R'<<24)|('E'<<16)|('R'<<8)|('S'))
#define MH_RET_RSA_FAILURE                    (('R'<<24)|('E'<<16)|('R'<<8)|('E'))

#define MH_RET_RSA_GKEY_SUCCESS             (('R'<<24)|('G'<<16)|('K'<<8)|('S'))
#define MH_RET_RSA_GKEY_FAILURE             (('R'<<24)|('G'<<16)|('K'<<8)|('F'))

#define MH_RET_RSA_CKEY_SUCCESS             (('R'<<24)|('C'<<16)|('K'<<8)|('S'))
#define MH_RET_RSA_CKEY_FAILURE             (('R'<<24)|('C'<<16)|('K'<<8)|('F'))

#define MH_RET_PRIME_NOT_ACCEPTABLE         (('R'<<24)|('P'<<16)|('N'<<8)|('A'))
#define MH_RET_PRIME_IS_PRIME                   (('R'<<24)|('P'<<16)|('I'<<8)|('P'))
#define MH_RET_PRIME_INPUT_PARA_ERR         (('R'<<24)|('P'<<16)|('P'<<8)|('E'))




/****************************  RSA 算法数据结构 ***************************/
#define    MAX_RSA_MODULUS_LEN    ((MH_MAX_RSA_MODULUS_BITS + 7) / 8)
#define    MAX_RSA_PRIME_LEN      ((MH_MAX_RSA_PRIME_BITS + 7) / 8)

#define     RSA_KEY_RET_ERR_BIT      -1                //参数iProtoKeyBit错误
#define     RSA_KEY_RET_ERR_PUKE     -2                //参数iPubEType错误
#define     RSA_KEY_RET_ERR_DATA     -3                //产生数据失败
#define     RSA_KEY_RET_ERR_RANDOM   -4                //产生随机数失败
#define     RSA_KEY_RET_ERR_ENCRY    -5                //加密运算失败
#define     RSA_KEY_RET_ERR_DECRY    -6                //解密运算失败
#define     RSA_KEY_RET_ERR_VERIF    -7                //加解密验证失败

typedef struct
{
    unsigned short int   bits;
    unsigned char        modulus[MAX_RSA_MODULUS_LEN];
    unsigned char        exponent[MAX_RSA_MODULUS_LEN];
}R_RSA_PUBLIC_KEY;

typedef struct
{
    unsigned short int    bits;
    unsigned char         modulus[MAX_RSA_MODULUS_LEN];
    unsigned char         publicExponent[MAX_RSA_MODULUS_LEN];
    unsigned char         exponent[MAX_RSA_MODULUS_LEN];
    unsigned char         prime[2][MAX_RSA_PRIME_LEN];
    unsigned char         primeExponent[2][MAX_RSA_PRIME_LEN];
    unsigned char         coefficient[MAX_RSA_PRIME_LEN];
}R_RSA_PRIVATE_KEY;

void RsaPublicKeyChange(R_RSA_PUBLIC_KEY *publickey,mh_rsa_public_key *key)
{
    key->bytes = (publickey->bits>>3);
    memcpy(key->n,publickey->modulus,MAX_RSA_MODULUS_LEN);
    memcpy(key->e,publickey->exponent,4);
    mh_rsa_get_public_key_C_Q(key,mh_rand_p, NULL);
}

void RsaPrivateKeyChange(R_RSA_PRIVATE_KEY *privatekey,mh_rsa_private_key *key)
{
     key->bytes = (privatekey->bits>>3);
     memcpy(key->n,privatekey->modulus,MAX_RSA_MODULUS_LEN);
     memcpy(key->e,privatekey->publicExponent,4);
     memcpy(key->d,privatekey->exponent ,MAX_RSA_MODULUS_LEN);
     memcpy(key->p,&(privatekey->prime[0][0]) ,MAX_RSA_PRIME_LEN);
     memcpy(key->q,&(privatekey->prime[1][0]) ,MAX_RSA_PRIME_LEN);
     memcpy(key->dp,&(privatekey->primeExponent[0][0]) ,MAX_RSA_PRIME_LEN);
     memcpy(key->dq,&(privatekey->primeExponent[1][0]) ,MAX_RSA_PRIME_LEN);
     memcpy(key->qp,privatekey->coefficient ,MAX_RSA_PRIME_LEN);
     mh_rsa_get_private_key_C_Q(key,mh_rand_p, NULL);
     mh_rsa_complete_key(key, mh_rand_p, NULL);
}

/**
  * @method Public-key decrypt
  * @brief  public key decrypt
  * @param  output      :output data buffer
  * @param  outlenth        :lenth of output data block
  * @param  input           :input data buffer
  * @param  inputlenth  :lenth of input block
    * @param    publickey   :RSA public key
  * @retval                   :RSA_DECRYPT_SUC or RSA_DECRYPT_FAIL
  */
int RSAPublicDecrypt(uint8_t *output, uint32_t *outputLen,uint8_t *input, uint32_t inputlen,R_RSA_PUBLIC_KEY *key)
{   
    int ret = 0;
    mh_rsa_public_key mh_key;
    RsaPublicKeyChange(key,&mh_key);
    
    ret = mh_rsa_public(output, input, &mh_key, mh_rand_p, NULL);
    if (ret == MH_RET_RSA_SUCCESS)
    {
        *outputLen = mh_key.bytes;
		ret = 0;
    }
    else
    {
        *outputLen = 0;
        ret = RSA_KEY_RET_ERR_ENCRY;
    }
    return ret;
}


int RSAPrivateEncrypt(uint8_t *output,uint32_t *outputLen, uint8_t *input,uint32_t inputlen, R_RSA_PRIVATE_KEY *key)
{
    int ret = 0;
    mh_rsa_private_key mh_key;
    RsaPrivateKeyChange(key,&mh_key);
    
    ret = mh_rsa_private_crt(output, input, &mh_key, mh_rand_p, NULL);
    if (ret == MH_RET_RSA_SUCCESS)
    {
        *outputLen = mh_key.bytes;
		ret  = 0;
    }
    else
    {
        *outputLen = 0;
        ret = RSA_KEY_RET_ERR_DECRY;
    }
    return ret;
}
int RSARecover(uchar *pbyModule,uint dwModuleLen,uchar *pbyExp,uint dwExpLen,uchar *pbyDataIn,uchar *pbyDataOut)
{
    int ret = 0;
    R_RSA_PUBLIC_KEY publickey;
    R_RSA_PRIVATE_KEY privatekey;
    uchar tmp[4] = {0x00,0x01,0x00,0x01};
    mh_rsa_private_key  mh_privatekey;
	mh_rsa_public_key  mh_publickey;
    uint outputlen;
	uint inputlen = 128;
    
    if((pbyModule==NULL) || (pbyExp==NULL))
    {
        return -1;
    }
    if((dwModuleLen==0) || (dwExpLen == 0))
    {
        return -2;
    }
    if((dwModuleLen>256) || (dwExpLen >256))
    {
        return -3;
    }
//  if(pbyDataIn>pbyModule)  
//  {
//      return -4;
//  }
    if(dwExpLen > dwModuleLen)
    {
        return -5;
    }
    if(dwModuleLen ==  dwExpLen)    
    {
        mh_privatekey.bytes =  dwModuleLen;
        memcpy(mh_privatekey.n,pbyModule,dwModuleLen);      //n
        memcpy(mh_privatekey.d,pbyExp,dwExpLen);            //d
        //memcpy(mh_privatekey.e,tmp,4);                      //e   采用默认值
        //mh_rsa_get_private_key_C_Q(&mh_privatekey,mh_rand_p, NULL);
        ret = mh_rsa_private_tmp(pbyDataOut, pbyDataIn, &mh_privatekey, mh_rand_p, NULL); 
		if(ret == MH_RET_RSA_SUCCESS)
		{
			ret = 0;
		}
    }
    else                            
    {
    	  memcpy(tmp + sizeof(tmp) - dwExpLen, pbyExp, dwExpLen);
        publickey.bits = dwModuleLen<<3;   
        memcpy(publickey.modulus,pbyModule,dwModuleLen);
        memcpy(publickey.exponent,tmp,sizeof(tmp));
        ret = RSAPublicDecrypt(pbyDataOut,&outputlen,pbyDataIn,inputlen,&publickey);
		if(ret == MH_RET_RSA_SUCCESS)
		{
			ret = 0;
		}
  }
    return ret;
}

//int RSARecover(uchar *pbyModule,uint dwModuleLen,uchar *pbyExp,uint dwExpLen,uchar *pbyDataIn,uchar *pbyDataOut)
//{
//    int ret = 0;
//    R_RSA_PUBLIC_KEY publickey;
//    R_RSA_PRIVATE_KEY privatekey;
//    uchar tmp[4] = {0x00,0x01,0x00,0x01};
//    mh_rsa_private_key  mh_privatekey;
//	mh_rsa_public_key   mh_publickey;
//    uint outputlen;
//	uint inputlen = 128;
//    
//    if((pbyModule==NULL) || (pbyExp==NULL))
//    {
//        return -1;
//    }
//    if((dwModuleLen==0) || (dwExpLen == 0))
//    {
//        return -2;
//    }
//    if((dwModuleLen>256) || (dwExpLen >256))
//    {
//        return -3;
//    }
//  if(pbyDataIn>pbyModule)  
//  {
//      return -4;
//  }
//    if(dwExpLen > dwModuleLen)
//    {
//        return -5;
//    }
//    if(dwModuleLen ==  dwExpLen)    
//    {
//        mh_privatekey.bytes =  dwModuleLen;
//        memcpy(mh_privatekey.n,pbyModule,dwModuleLen);      //n
//        memcpy(mh_privatekey.d,pbyExp,dwExpLen);            //d
//        //memcpy(mh_privatekey.e,tmp,4);                      //e   采用默认值
//        //mh_rsa_get_private_key_C_Q(&mh_privatekey,mh_rand_p, NULL);
//        ret = mh_rsa_private_tmp(pbyDataOut, pbyDataIn, &mh_privatekey, mh_rand_p, NULL); 
//		if(ret == MH_RET_RSA_SUCCESS)
//		{
//			ret = 0;
//		}
//    }
//    else                            
//    {
//        memcpy(tmp + sizeof(tmp) - dwExpLen, pbyExp, dwExpLen);
//        publickey.bits = dwModuleLen<<3;   
//        memcpy(publickey.modulus,pbyModule,dwModuleLen);
//        memcpy(publickey.exponent,tmp,sizeof(tmp));
//        ret = RSAPublicDecrypt(pbyDataOut,&outputlen,pbyDataIn,inputlen,&publickey);
//		if(ret == MH_RET_RSA_SUCCESS)
//		{
//			ret = 0;
//		}
//  }
//    return ret;
//}


//功能：随机产生1024，2048 位的RSA公私钥对
int RSAKeyPairGen(R_RSA_PUBLIC_KEY *PublicKey,R_RSA_PRIVATE_KEY *PrivateKey,int ProtoKeyBit,int PubEType)
{
     int ret = 0;
     uint i;
     mh_rsa_private_key  mh_privatekey;
     mh_rsa_public_key   mh_publickey;
     switch (PubEType)
    {
        case 0:
             if(ProtoKeyBit == 1024)
             {
                 ret = mh_rsa_gen_key(&mh_privatekey, 0x03, 1024, mh_rand_p, NULL);
             }
             else if(ProtoKeyBit == 2048)
             {
                 ret = mh_rsa_gen_key(&mh_privatekey, 0x03, 2048, mh_rand_p, NULL);
             }
             else
             {
                 ret = RSA_KEY_RET_ERR_BIT;
             }
            break;
        case 1:
             if(ProtoKeyBit == 1024)
             {
                    ret = mh_rsa_gen_key(&mh_privatekey, 0x10001, 1024, mh_rand_p, NULL);   //客户给的参数是0xFF  
             }
             else if(ProtoKeyBit == 2048)
             {
                    ret = mh_rsa_gen_key(&mh_privatekey, 0x10001, 2048, mh_rand_p, NULL);
             }
             else
             {
                  ret = RSA_KEY_RET_ERR_BIT;
             }
            break;
        default:
            return RSA_KEY_RET_ERR_PUKE;
            break;
    }
     if(MH_RET_RSA_CKEY_SUCCESS == mh_rsa_get_private_key_C_Q(&mh_privatekey,mh_rand_p, NULL))
     {
         mh_rsa_complete_key(&mh_privatekey, mh_rand_p, NULL);
        
         PublicKey->bits = (mh_privatekey.bytes<<3);
         memcpy(PublicKey->modulus,mh_privatekey.n,MAX_RSA_MODULUS_LEN);
         memcpy(PublicKey->exponent,mh_privatekey.e ,4);
        
         PrivateKey->bits = (mh_privatekey.bytes<<3);
         memcpy(PrivateKey->modulus,mh_privatekey.n,MAX_RSA_MODULUS_LEN);
         memcpy(PrivateKey->publicExponent,mh_privatekey.e,4);
         memcpy(PrivateKey->exponent ,mh_privatekey.d,MAX_RSA_MODULUS_LEN);
         memcpy(&(PrivateKey->prime[0][0]) ,mh_privatekey.p,MAX_RSA_PRIME_LEN);
         memcpy(&(PrivateKey->prime[1][0]) ,mh_privatekey.q,MAX_RSA_PRIME_LEN);
        
         memcpy(&(PrivateKey->primeExponent[0][0]) ,mh_privatekey.dp,MAX_RSA_PRIME_LEN);
         memcpy(&(PrivateKey->primeExponent[1][0]) ,mh_privatekey.dq,MAX_RSA_PRIME_LEN);
         memcpy(PrivateKey->coefficient ,mh_privatekey.qp,MAX_RSA_PRIME_LEN);
		 ret = 0;
     }
     else
     {
         ret = RSA_KEY_RET_ERR_DATA;
     }
    return ret;
    
}
//功能：验证1024,2048位的RSA公私钥是否是一对
int RSAKayPairVerify(R_RSA_PUBLIC_KEY PublicKey,R_RSA_PRIVATE_KEY PrivateKey)
{
    const char PLAINTXT[10]= "1234567890";
    uint i;
    uchar plain[2048 / 8];
    uchar cipher[2048 / 8];
    uchar tmp0[2048 / 8];
    uint output = 0;
	if(PublicKey.bits != PrivateKey.bits)
	{
		 return RSA_KEY_RET_ERR_VERIF;
	}
    memset(&plain, 0, sizeof(plain));
    memset(&cipher, 0, sizeof(cipher));
    memset(&tmp0, 0, sizeof(cipher));
    bn_read_string_to_bytes(plain, PublicKey.bits>>3, PLAINTXT);  
    RSAPublicDecrypt(cipher,&output,plain,PublicKey.bits>>3,&PublicKey);
    RSAPrivateEncrypt(tmp0,&output,cipher,PublicKey.bits>>3,&PrivateKey);
    if(0 == memcmp(tmp0, plain, 256))
    {
        return 0;
    }
    else
    {
        return RSA_KEY_RET_ERR_VERIF;
    }
}


/*********************************  SM2 算法数据结构 *******************************/
#define   SM_RET_OK                   0
#define   SM_RET_INVALID_PARAM       -1 
#define   SM_RET_ERROR               -2 
#define   SM_RET_TIMEOUT             -3

typedef struct
{
  uchar privkey[32];
  uchar pubkey[64];
}ST_SM2_KEY;

typedef struct
{
  ST_SM2_KEY key;     
  uchar *ida;         // 输入数据
  uint idalen;        // 表示的是输入数据的bit长度
}ST_SM2_INFO;


static void SM2KeyChange(ST_SM2_KEY *key,mh_sm2_private_key *mh_key)
{
  memcpy(mh_key->d,key->privkey,32);
  memcpy(mh_key->e.x,key->pubkey,32);
  memcpy(mh_key->e.y,&(key->pubkey[32]),32);
}

// 椭圆曲线参数
#define tp        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF"
#define ta        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"
#define tb        "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"
#define tn        "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123"
#define tGx       "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"
#define tGy       "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0"

#define PRIKEY     "CA564D125B2BB005F1B610DDED5E0118F9575459A584924B59ADBA30CB447C00"
#define PUBKEY     "ABF1E274B8B02B3041AD006D1DC61F208721DBE38B8AAD6F5048DF9484177CD7536373A9D9761FAFCF9DC018CEAA6CA05651E95DDBC0A772BDEA203C15437B23"

#define PUBKEY_x   "ABF1E274B8B02B3041AD006D1DC61F208721DBE38B8AAD6F5048DF9484177CD7"                   
#define PUBKEY_y   "536373A9D9761FAFCF9DC018CEAA6CA05651E95DDBC0A772BDEA203C15437B23"



#define PLAINTEXT  		"115960C5598A2797C2D02D76D468CDC9"
#define CIPHERTEXT    "91082B6DD0F33A18D9CC970E014377631CEDA7B4A7E8EAFA5733FA535405AB07CEB27A3A239D04A368210854D5B1C98084337FA41423EDC060C9096A3E67698E71B0E31CC772579463607C76F0AF9373323B60CA196F31FB1B71775BCE49319B2FD33DC2A6A144FF1879780EFC7E1EA5"
//#define CIPHERTEXT  "04D6C813A6BD1EF8F5FCCD556F103E209C5981EB3E0FB80912BDBD298DE8AC21AE18BEFFB83EB9F314F0D107779277A83F2B3C0B226A7593980319BCCA39188E204A4A99E61C3401F26A62527878254EEEAAE7C51FDD44820C2BAC563882BFDD813CE47F25B7F07249054D24F845A1C45E"

#define cENTLa    "0090"
#define cIDa      "414C494345313233405941484F4F2E434F4D"
#define MESSAGE   "F4A38489E32B45B6F876E3AC2168CA392362DC8F23459C1D1146FC3DBFB7BC9A6D65737361676520646967657374"
#define MESSAGERESULT ""


static void SM2GenratePara(mh_sm2_ellipse_para *mh_para)
{
  bn_read_string_to_bytes(mh_para->p, MH_SM2_KEY_BYTES, tp);
  bn_read_string_to_bytes(mh_para->a, MH_SM2_KEY_BYTES, ta);
  bn_read_string_to_bytes(mh_para->b, MH_SM2_KEY_BYTES, tb);
  bn_read_string_to_bytes(mh_para->n, MH_SM2_KEY_BYTES, tn);
  bn_read_string_to_bytes(mh_para->g.x, MH_SM2_KEY_BYTES, tGx);
  bn_read_string_to_bytes(mh_para->g.y, MH_SM2_KEY_BYTES, tGy);
}

int SM2GenKeyPair(ST_SM2_KEY *key)
{
  int ret = SM_RET_OK;
  mh_sm2_private_key mh_key;
  mh_sm2_ellipse_para mh_para;
  uchar Za[32] = {0};
  SM2GenratePara(&mh_para);

  if(MH_RET_ECC_KEY_GENERATION_SUCCESS == mh_sm2_genkey(&mh_key, &mh_para, mh_rand_p, NULL) )
  {
		  memcpy(key->pubkey,mh_key.e.x,32);
      memcpy(&(key->pubkey[32]),mh_key.e.y,32);
      memcpy(key->privkey,mh_key.d,32);   
  }
  else
  {
      ret = SM_RET_ERROR;
  }
  return ret;
}

int SM2Sign(ST_SM2_INFO *info,uint8_t *data,uint32_t datalen,uint8_t *sign,uint32_t *signlen)
{
  int ret = SM_RET_OK;
  mh_sm2_private_key mh_key;
  mh_sm2_ellipse_para mh_para;
  mh_sm2_sign mh_sign;
  uchar Za[32] = {0};
  uint e[8];
  SM2KeyChange(&(info->key),&mh_key);
  SM2GenratePara(&mh_para);
  mh_sm2_hash_z(Za, info->ida, info->idalen, &mh_para,&(mh_key.e)); 
  mh_sm2_hash_e(e, Za, data, datalen);
//   mh_sm2_hash_e(Za, info->ida, info->idalen, &mh_para,&(mh_key.e)); 
  
  if(MH_RET_SM2_SIGN_SUCCESS == mh_sm2_digital_sign_with_e(&mh_sign,e,data, datalen,&mh_key, &mh_para,mh_rand_p, NULL) )
  {
      memcpy(sign,mh_sign.r,32);
      memcpy((sign+32),mh_sign.s,32);
      *signlen = 64;
  }
  else
  {
      ret = SM_RET_ERROR;
  }
  return ret;
}

int SM2Verify(ST_SM2_INFO *info,uint8_t *data,uint32_t datalen,uint8_t *sign,uint32_t signlen)
{
  int ret = SM_RET_OK;
  mh_sm2_private_key mh_key;
  mh_sm2_sign mh_sign;
  uchar Za[32] = {0};
	uint e[8];
  mh_sm2_ellipse_para mh_para;

  SM2GenratePara(&mh_para);
  SM2KeyChange(&(info->key),&mh_key);
  memcpy(mh_sign.r,sign,32);
  memcpy(mh_sign.s,(sign+32),32);
  signlen = 64;
  mh_sm2_hash_z(Za, info->ida, info->idalen, &mh_para,&(mh_key.e)); //????Za
  mh_sm2_hash_e(e, Za, data, datalen);

  if(MH_RET_SM2_VERIFY_SUCCESS != mh_sm2_verify_sign_with_e(&mh_sign,e,data, datalen,&(mh_key.e), &mh_para,mh_rand_p, NULL))
  {
      ret = SM_RET_ERROR;
  }
  return ret;
}
int SM2Recover(ST_SM2_KEY *key,uchar *datain,uint datainlen,uchar *dataout,uint *dataoutlen,uint mode)
{
  int ret = SM_RET_OK;
	uchar tmp[2048];
	uint lenth = 0;
  mh_sm2_private_key t_key;
  mh_sm2_ellipse_para mh_para;
  SM2GenratePara(&mh_para);
  SM2KeyChange(key,&t_key);
  if(mode == 1)
  {
      if(MH_RET_SM2_ENC_SUCCESS != mh_sm2_enc(tmp,&lenth,datain, datainlen,&(t_key.e), &mh_para,mh_rand_p, NULL))
      { 
				ret = SM_RET_ERROR;
			}
			else
			{
				memcpy(dataout,&tmp[1],64);
				memcpy(dataout+64,&tmp[lenth-32],32);
				memcpy(dataout+64+32,&tmp[65],lenth-1-32-64);
				*dataoutlen = lenth -1;
			}
  }
  else
  {	  
			tmp[0] = 0x04;
		  memcpy(&tmp[1],datain,64);
		  memcpy(&tmp[65],datain+64+32,datainlen-64-32);
		  memcpy(&tmp[1+datainlen-32],datain+64,32);
		  lenth = datainlen + 1;
		
			if(MH_RET_SM2_DEC_SUCCESS != mh_sm2_dec(dataout,dataoutlen,tmp, lenth,&t_key, &mh_para,mh_rand_p, NULL))
			{
				ret = SM_RET_ERROR;
			}
  }
  return ret;
}

/*********************** SM3 算法 ****************************/

int SM3(uchar *key,uint keylen,uchar *datain,uint datainlen,uchar *dataout)
{
  int ret = 0;
  if(key==NULL)    //采用SM3杂凑
  {
      ret = mh_sm3(dataout, datain,datainlen);
  }
  else            //HMAC 算法
  {   

  }
  return ret;
}


/*********************** SM4 算法 ****************************/
int SM4(uint8_t *datain,uint32_t datainlen,uint8_t iv[16],uchar key[16],uchar *dataout,uint mode)
{
  int ret = 0;
  switch(mode)
  {
      case 0:
          ret = mh_sm4_dec(ECB, dataout, datainlen, datain, datainlen, key, iv,mh_rand_p, NULL);
          break;
      case 1:
          ret = mh_sm4_enc(ECB, dataout, datainlen, datain, datainlen, key, iv,mh_rand_p, NULL);
          break;
      case 2:
          ret = mh_sm4_dec(CBC, dataout, datainlen, datain, datainlen, key, iv,mh_rand_p, NULL);
          break;
      case 3:
          ret = mh_sm4_enc(CBC, dataout, datainlen, datain, datainlen, key, iv,mh_rand_p, NULL);
          break;
      default:
          break;
  }
  return ret;
}
/************************************* AES 算法 **********************************/
unsigned int encryptAES(uint8_t *key,uint16_t mode,uint8_t *input_data,uint8_t *output_data,uint16_t data_length,uint8_t *init_vector)
{
    uint ret = 0;
    ret = mh_aes_enc(ECB,output_data, data_length,input_data, data_length,key, 128, init_vector,mh_rand_p, NULL);
    return ret;
}
unsigned int  decryptAES(uint8_t *key,uint16_t mode,uint8_t *input_data,uint8_t *output_data,uint16_t data_length,uint8_t *init_vector)
{
   uint ret = 0;
   ret = mh_aes_dec(ECB,output_data, data_length,input_data, data_length,key, 128, init_vector,mh_rand_p, NULL);
   return ret;
}
/****************************** DES 算法 *******************************************/
void des_test(uchar *input,uchar *output,uchar *deskey,int mode)
{
  if(mode == 0)
  {
      mh_des_dec(ECB,output,8,input,8,deskey, NULL,mh_rand_p, NULL);
  }
  else
  {
      mh_des_enc(ECB,output,8,input,8,deskey, NULL,mh_rand_p, NULL);
  }
}




/************************************ AES 测试函数 *************************************/
void TestAes()
{
  uint t;
  mh_pack_mode_def modes[2] = { ECB, CBC};

  uchar plain[] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A, 
                                       0xAE, 0x2D, 0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF, 0x8E, 0x51};
       
  uchar key128[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
  uchar key192[] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 
                                          0X62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
  uchar key256[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 
                                          0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};
      
  uchar iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  uchar eCipher[3][32] = {{0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60, 0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97, 
                                              0xF5, 0xD3, 0xD5, 0x85, 0x03, 0xB9, 0x69, 0x9D, 0xE7, 0x85, 0x89, 0x5A, 0x96, 0xFD, 0xBA, 0xAF},
                                           {0xBD, 0x33, 0x4F, 0x1D, 0x6E, 0x45, 0xF2, 0x5F, 0xF7, 0x12, 0xA2, 0x14, 0x57, 0x1F, 0xA5, 0xCC,
                                              0x97, 0x41, 0x04, 0x84, 0x6D, 0x0A, 0xD3, 0xAD, 0x77, 0x34, 0xEC, 0xB3, 0xEC, 0xEE, 0x4E, 0xEF},
                                           {0xF3, 0xEE, 0xD1, 0xBD, 0xB5, 0xD2, 0xA0, 0x3C, 0x06, 0x4B, 0x5A, 0x7E, 0x3D, 0xB1, 0x81, 0xF8,                                                
                                              0x59, 0x1C, 0xCB, 0x10, 0xD4, 0x10, 0xED, 0x26, 0xDC, 0x5B, 0xA7, 0x4A, 0x31, 0x36, 0x28, 0x70}};   

  uchar cCipher[3][32] = {{0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46, 0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D, 
                                                       0x50, 0x86, 0xCB, 0x9B, 0x50, 0x72, 0x19, 0xEE, 0x95, 0xDB, 0x11, 0x3A, 0x91, 0x76, 0x78, 0xB2},
                                                      {0x4F, 0x02, 0x1D, 0xB2, 0x43, 0xBC, 0x63, 0x3D, 0x71, 0x78, 0x18, 0x3A, 0x9F, 0xA0, 0x71, 0xE8, 
                                                       0xB4, 0xD9, 0xAD, 0xA9, 0xAD, 0x7D, 0xED, 0xF4, 0xE5, 0xE7, 0x38, 0x76, 0x3F, 0x69, 0x14, 0x5A},
                                                      {0xF5, 0x8C, 0x4C, 0x04, 0xD6, 0xE5, 0xF1, 0xBA, 0x77, 0x9E, 0xAB, 0xFB, 0x5F, 0x7B, 0xFB, 0xD6, 
                                                       0x9C, 0xFC, 0x4E, 0x96, 0x7E, 0xDB, 0x80, 0x8D, 0x67, 0x9F, 0x77, 0x7B, 0xC6, 0x70, 0x2C, 0x7D}}; 
  uchar cipher[32];
  uchar mplain[32];
  memset(cipher, 0, sizeof(cipher));
  memset(mplain, 0, sizeof(mplain));
//    mh_aes_enc(modes[0], cipher, sizeof(cipher), plain, sizeof(plain), key128, MH_AES_128, iv);
//    mh_aes_dec(modes[0], mplain, sizeof(mplain), cipher, sizeof(cipher), key128, MH_AES_128, iv);                                                    
  encryptAES(key128,0,plain,cipher,sizeof(plain),iv);
  decryptAES(key128,0,cipher,mplain,sizeof(cipher),iv);                                                        
}


/*****************************************************************************
 Prototype    : TestSm2
 Description  : Test the function of SM2
 Input        : None
 Output       : None
 Return Value : 
 
  History        :
  1.Date         : 2016/7/6
    Author       : david
    Modification : Created function

*****************************************************************************/
void TestSm2()
{
  uint ret = 0,i=0;
  ST_SM2_INFO info;
   mh_sm2_private_key mh_key;
  //签名，验签用数据
  uchar message[100];      
  uchar output[64] = {0};
  uint outputlen = 0;
 
  // 加密用变量
  uchar datain[16] = {0x11,0x59,0x60,0xC5,0x59,0x8A,0x27,0x97,0xC2,0xD0,0x2D,0x76,0xD4,0x68,0xCD,0xC9};
  uchar dataout[300] = {0};
  uint dataoutlen = 0;

  // 解密用变量
  uchar DecData[20] = {0};
  uint DecLen = 0;
  
  info.idalen = 0x0090;                                          // 注意，长度是info.ida 的bit长度
  bn_read_string_to_bytes(info.ida, sizeof(cIDa)/2, cIDa);
  bn_read_string_to_bytes(info.key.privkey, sizeof(PRIKEY)/2, PRIKEY);
  bn_read_string_to_bytes(info.key.pubkey, sizeof(PUBKEY)/2, PUBKEY);
  bn_read_string_to_bytes(datain, sizeof(PLAINTEXT)/2, PLAINTEXT);

  ret = SM2GenKeyPair(&(info.key));
  printf("SM2GenKeyPair() return value is %d\t\r\n",ret);
 
  ret = SM2Sign(&info,message,sizeof(message),output,&outputlen);       //签名测试
  printf("SM2Sign() return value is %d\t\r\n",ret);
 
  ret = SM2Verify(&info,message, sizeof(message),output,outputlen);     //验签测试
  printf("SM2Verify() return value is %d\t\r\n",ret);

  ret = SM2Recover(&(info.key), datain, sizeof(datain), dataout, &dataoutlen, 1); 

  ret = SM2Recover(&(info.key), dataout, dataoutlen, DecData, &DecLen, 0); 
  printf("\r\n打印解密数据!\r\n");
	for(i=0;i<DecLen;i++)
	{
		printf("%X\t",DecData[i]);
	}

	printf("\r\n解密数据打印完成!\r\n");
 

}

/******************************* RSA 测试函数 **********************************/
// 测试使用的变量
#define PLAIN "1234567890"
#define KEY_LEN_1024        1024 / 32
#define RSA_N_1024          "CDE42E9228172CBE4C542194BBB112728E0180FF7196B849D74F0630ED92AA5C4E04224E10C43881EBD710151E1F6BE47B2F688EAB880AC3D876C9702378CB524CA85637DE795D4393D129672DB4655C1EE82959ED051A92C1902135751CC4EC3E5B9050D5DF7A40610F61E382D864CC85F49F676E7C58A56E275D32648A65AB"
#define RSA_E_1024          "00010001"
#define RSA_D_1024          "B5C1ED76EB5B620D709688BAA362F3351BB5AA2A571D45FD31CCBA5B712F1BA3B6DFA165271A020477AD78AE1BD4834BC58F998EF1507CA85DACA7B18E2EF1D5548723BD59DF8DA9B108DA2B5A455E9F3F12CD2DC0E549FF3EA534E0690AA393042062290931FA2B5AC1E9A1753C4791D74641103AFF067F7DD86B02C768CC59"
#define RSA_P_1024          "F2DEB75C74A29F4FB17405ED8B584E63839F1822029FADB86AB4AAEA5E76E915503BB6C44BCE3194026FFF47170D3725AEC18B99348DB40B6B5522FAB81CE3A7"
#define RSA_Q_1024          "D905AFCF9AEEDAE746550927BA6B3C8A32EDEE8C16DC8306027FF54C159CBC847DF4FD4841FFF57E50E8A675C9CAC2428C2BD75B00B7BA74243F3898DBF4FE5D"
#define RSA_DP_1024         "4A1B14FDDC04E6F16D2706CC0591AB51880EDA513E55B4AF29B7F8D2CC8221FAC3E18491FEC36AB34BCF1AA8796B29E190A233D34A8AD65E09A10FAC4AD35DCF"
#define RSA_DQ_1024         "06CB8AC9B14C314674894D14155E2E33158D259EF5484A41F1EA17C01E1D1E39F7C7A7F68F3A7871500F38C70F5E6DDEAF8CDFDD55946B1E1FD46AED3E55D971"
#define RSA_QP_1024         "7E4BDC3E7FA115F1AEFD49CE2FFA474E4777FFA5195A6909919DD093CFACDC98A13C72EFA2476F148E6BE7F330E4143BEDF8B9D965D3A3A2657890049DE98635"

#define RSA_N_C_1024        "64EFEDD782B6A49AEA1872EDD9275E49931139CCFF5AC92433B07B0A9EA6693FA7C9D1B3149E17C8DD0A1AB8548844EB6E0F50DC894AF0593E4E5B7E27690FB576642F98A1A33C22FF382DAFAEAECDFFDDFD3DD09A207C5B335DEC99C5D137426421E044312602DA6901A218EAB977D6A5C7B5A60191A4BDE0DBC63C407FEF8D"
#define RSA_P_C_1024        "5DA1CD5FA89C67FA7728E97B67BAAE72D1A9F03E7CC032ADB9EDAC3949667F4B04A1196D73F5D8A375B9E7E13A95D7F934ABB508EC96DE712644621864CB94B8"
#define RSA_Q_C_1024        "4017B2E1F5500979E663ECE3A049678E49BCD4C822BE2E2C6C6C815A2EC23F5EEE7C153C6A2C2677F2419792F2BA6BE8B4C42943267922BF02B02AAAE7071E23"
#define RSA_N_Q_1024        "85BA92FD"
#define RSA_P_Q_1024        "3AC26BE9"
#define RSA_Q_Q_1024        "98B83A0B"
#define CIPHER_1024         "4257A706BDB0F1AEF8957E5B00ADB1BB2386AAC79394180049AF03C5AE12642E20D33D725408E361FBB600871E0AD925DAB6AA76CE6324E9189B33C99106BA45317FD8639EAF2F6DCA03DEA33D0DCE37D9493F917A1DB8A47F4FC249D44B22775D12908BE2F8F95A8451A0738A9961BD4E905741AB6A16EA2D600A5BB95AF399"

 #define KEY_LEN_2048       2048 / 32
 #define RSA_N_2048         "75A18418B8EB30CD990303626314215772406FB53C90BC34F8963643CB7C86655174E82AC581EA9D93FBCD8B9B9623E9FE8AD5E2F9311C8FEEFB709D78CC961DC49A3CA5F55733E68A10563695782DE1C8BE5F031E84540E14AA369FA8E03D03CCC7E02B2A030C44AF4B989DBC99640BB4925B9C3AAE287C241C1AF133502AD61593C62FAB42604680FBE1EC77C98B68008C15E52C24035A8F220C1B944C547F1E0FA4C6475395839536FD286F23063E802D657C6D176B46E3B1862616D32EADB0DC1C1AF83EAF33640224658BB46052ACD8D8876A7F35A9232ED4AD2E15FD958F294A5BFFBA11E2E18F7D3377136F6C91B93C9656E289E19FE3E928FDF7FB7F"
 #define RSA_E_2048         "10001"
 #define RSA_D_2048         "562B23211914F0832B7B5197590CFDFF98EA2AEA909B55C357F67179E82E2825402E6B84297AA9EB14E47805B4CB9F33620B41F32AA8174C38BC681A72CB2B8C52E0C95CA9046038991AAE8CFDA133BB19DA9AAF2643B96F3724192F8BFDF7B438E692080C967EFE318B27192577181A9B043BA78E34D5E35A705C2C2472AD6D762273180C7DA3107658259BC5BF554B3CC2CF33F264DDB3C45406A047A1B3720A655CAFF677FA7DCFDB9F125A8C1D333E07F93C884AFC7E9444D6033AAB3975DED23BC71BCA3EC83CCF000D6586020B1BF483762BFE6F1C64CE021CB85AD538A18B7539D42619C6901364ED6B14D07EC9EFECA16AC59D5BAB0F8840A5D86281"
 #define RSA_P_2048         "9AA3212A0A6E3158E399844D2F44C9CC3D25D5DDF1C18B2F49E70BB2326F1E738C33DFB87A4DAC5EED914F72724C670D393D1B7368D2006DA1A649E5D03930C239F0A304C04F3222787436A271D544F8BD3DD3A3C2032B84C68B9080B600AE57395146C6F46C18DA1DB9129544D8B9C65F9DD624164A64AB500EB83AFDBDF279"
 #define RSA_Q_2048         "C2BC8CFDC7F5C23DE47E95D39BD0EE16B27957280655ED4F0D0C8793AACD279B374BA0C3CFF78781DA7D5EF8DE83FDDB83A04386E599FC5A20E113571852FBFE88275A684C3BF6A1F187E3497DD8EC6B7ACE419F471C615DA1FD48A36EF4DBC9CBC692A258BB67D6C213FD7D8FC513FE307E2A2943D7739227D7FD2EC4AB1FB7"
 #define RSA_DP_2048        "598E6E4F8DB8E1A56C32E5775A0FC99B1A32C6588281A79D1C7CCDBE470D29A29BA9134120BBC93F34F0FA04FB031A8F82FDCBDF1BEEBB46C2DEAE5F85270A01025345E62385FA301278A38792CB83E5569BEEED21187AE4ABA3FA6D407AC67D4779B5B6D5A9A0FEF945CCE85BF5F4295A08EC12FC41DB6987B4EEA7637AD569"
 #define RSA_DQ_2048        "9B8767BBD096317AA921889C271828EFABBF49A2C7DC9CD9C3FC3BD3EE5847C7A9D417240CD70F3CE0B9BCBE5B4963D88C39D574C9AFC062635C1FCC31BB2553D67E47B081226B0A50134DAE067AD7E7C77F2FDDFC37687415CA6AD74CE8F719264D0413F1764BAAB3FAAF3C37351990AC3E37821992DBD4E1499A065E40A5E5"
 #define RSA_QP_2048        "906598444A1664A4F3FD0E67DA92E0BCBB445101DC09ED567ACE2D49DBEC61B2E06ECD64544B0C35D26E68DBE3048B83A601285245AF4FFC94E3EF5F7602E3EC9B8AD6D9FBC518F95D3A638D7A3FC34E72307F67A48C0696753664D589BE2EC861EB659C7AB06C5A9095E4D8794D49504E651EAEE26C76091175C5B7082B4016"
 #define RSA_N_C_2048       "65B37280142A48FCED5C720FC0A17DB2A6B6C1901CD1FECA252FEB61E5CF58E4068800EE4F54D2D9E35566BC5CB976B497E65FFA529F398E562BB38AE891A03B212D88E98073DE85A5A00D9B2CA11D0B14AF22CD2C832FD8DCD5E4ADC6BA95FFC7FBEBC80FE9D32ACB4EB6F52C742B11F11D83655E30B5FC786E15CC8C8D1C9F9EF66284D337B8A697E4F8485064D28823FD0DB4E75F5618437D776D6910405AB88B467F2E5DD4C206C670807E25227E61D02EE32B37C731DA1DF141D36CC48B636867C37C6A08C17EE8416A0A33A0791EE2233C3915EC38B2DA1984DE95F00BEAA1575DBD1ACCF4F8DB37500656BE4CD7A874F6247A127C2F32A046197D4D2A"
 #define RSA_P_C_2048       "0224D929A1397374039926557BCA9FC74DD9E7AB3DBAF1A216DC1E57ED4F57B1A7D6E922F3CB5FE825FA14264BBFA864FD6129885EABC7C21680C2390511AC7189EF4F7F7BDEA690843353C18235E9E56BA985F712D44B377AE1578321A61DFAB7240E12597A5513B055E455BBAA66BCD25AAB72129D830B6EFAFFBBA5D30132"
 #define RSA_Q_C_2048       "5AD49E385E01DBA437ED91437ED4C9C17E1EE4F5EACABF9624C3C8AF4B4712D35900C84522FADC41D5723AA652D33F2F25DB60112F8DE2D9F907E8EE7B4A84129AA9FFCEF7FA1FFB4E4E879C5D55CFD45757AAC16739C495D5C756BFA4C3DEF9A591962919C298495A9727252B345D9470C4BFA33F5121AA861952B0D4A3A3D9"
 #define RSA_N_Q_2048       "FAEC3B81"
 #define RSA_P_Q_2048       "7D3E2837"
 #define RSA_Q_Q_2048       "552511F9"
 #define CIPHER_2048        "1A489B964931E96ECBED9DB0A357B0912C203B3020BA1427353AD06CAD229A77F9E864A11BF0AD6A4FE2CB88D3C0F4AE0B964C6122DA581E2E509D9B7828E41BDDC6A179F43EF2B1DAB09C4D49ACCE3CD55FBFA256E264189C365A2E8BE9640B98A73267E9A5ACBD9C93838AD76ED57DCD8C680F248CAC97CDA034CC348A5754EBFAAA3A855FF618CE6BCA51BCBAC33D105D0F6BF58FE1C0E7F8DF4CB293D48A4EC5FEA617DFA00F1E6688893FCF5DC7B45DE3C511E14DF2D6D072511664BF397D322DD61952882CE88E8F1D770B14340B52AAA2D2A388BF2A1F6A5D7A13EF149FF9AEDFE0A2657DCB9568A06FBFFB800D1CFB7CA625630D8DA39174008F93F9"
 
 R_RSA_PUBLIC_KEY TestPublicKey;
 R_RSA_PRIVATE_KEY TestPrivateKey;
 mh_rsa_public_key Test_mh_key;
 mh_rsa_private_key Test_mh_prikey;
 int TestReturn = 0;
 
/*****************************************************************************
 Prototype    : TestPubulicKey
 Description  : 
 Input        : None
 Output       : None
 Return Value : 
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2016/6/1
    Author       : david
    Modification : Created function

*****************************************************************************/
void TestPubulicKey()
{
    uint outputlen = 0;
    uint i,ret;
    uchar plain[2048 / 8];
    uchar cipher[2048 / 8];
    uchar tmp0[2048 / 8];
    uint  output = 0;
	mh_rsa_private_key pri_key;
	
    memset(&plain, 0, sizeof(plain));
    memset(&cipher, 0, sizeof(cipher));
    memset(&tmp0, 0, sizeof(cipher));


 #if 0  
	//加解密测试
 	 bn_read_string_to_bytes(TestPublicKey.modulus,256,RSA_N_2048);
 	 bn_read_string_to_bytes(TestPublicKey.exponent, 4, RSA_E_2048);
   pri_key.bytes = 256;
	 bn_read_string_to_bytes(pri_key.n, pri_key.bytes, RSA_N_2048);
	 bn_read_string_to_bytes(pri_key.e, 4, RSA_E_2048);
	 bn_read_string_to_bytes(pri_key.d, pri_key.bytes, RSA_D_2048);
	 bn_read_string_to_bytes(pri_key.p, pri_key.bytes/2, RSA_P_2048);
	 bn_read_string_to_bytes(pri_key.q, pri_key.bytes/2, RSA_Q_2048);
	 bn_read_string_to_bytes(pri_key.dp, pri_key.bytes/2, RSA_DP_2048);
	 bn_read_string_to_bytes(pri_key.dq, pri_key.bytes/2, RSA_DQ_2048);
	 bn_read_string_to_bytes(pri_key.qp, pri_key.bytes/2, RSA_QP_2048);
	 bn_read_string_to_bytes(pri_key.n_c, pri_key.bytes, RSA_N_C_2048);
	 bn_read_string_to_bytes(pri_key.p_c, pri_key.bytes/2, RSA_P_C_2048);
	 bn_read_string_to_bytes(pri_key.q_c, pri_key.bytes/2, RSA_Q_C_2048);
	 bn_read_string_to_bytes(pri_key.n_q, 4, RSA_N_Q_2048);
	 bn_read_string_to_bytes(pri_key.p_q, 4, RSA_P_Q_2048);
	 bn_read_string_to_bytes(pri_key.q_q, 4, RSA_Q_Q_2048);

    bn_read_string_to_bytes(plain,256,PLAIN);
    TestPublicKey.bits = 2048;
    TestReturn = RSAPublicDecrypt(tmp0,&outputlen,plain,256,&TestPublicKey);
    bn_read_string_to_bytes(cipher, 256, CIPHER_2048);
	printf("Enter Pullic Decrypt test!\r\n");
	printf("The return value is %d\r\n",TestReturn);
	for(i=0;i<256;i++)
	{
		printf("%X\t",tmp0[i]);
	    //测试加密通过
	}
	
   	printf("\r\n");
	printf("Enter Pullic Encrypt test!\r\n");
    memset(&tmp0, 0, sizeof(cipher));
   // TestReturn = RSAPrivateEncrypt(tmp0,&outputlen,cipher,256,&TestPrivateKey);

	TestReturn = mh_rsa_private(tmp0, cipher, &pri_key, mh_rand_p, NULL);

	
	printf("The return value is %d\r\n",TestReturn);
	for(i=0;i<256;i++)
	{
		printf("%X\t",tmp0[i]);
	   //测试解密通过
	}
#endif
	 
    //__nop();
    //测试Recover函数
    memset(&plain, 0, sizeof(plain));
    memset(&cipher, 0, sizeof(cipher));
    memset(&tmp0, 0, sizeof(cipher));
	    
    Test_mh_prikey.bytes = 256;
	bn_read_string_to_bytes(plain,256,PLAIN);
    memset(&Test_mh_prikey, 0, sizeof(Test_mh_prikey));
    bn_read_string_to_bytes(Test_mh_prikey.n,256,RSA_N_2048);
    bn_read_string_to_bytes(Test_mh_prikey.e, 4, RSA_E_2048);
    bn_read_string_to_bytes(Test_mh_prikey.d, 256,RSA_D_2048);

	printf("\r\n");
    printf("Recover  Encrypt function Test!\r\n");
    TestReturn = RSARecover(Test_mh_prikey.n, 256, Test_mh_prikey.e, 4,plain, tmp0);
	printf("The return value is %d\r\n",TestReturn);
	for(i=0;i<256;i++)  
	{
		printf("%X\t",tmp0[i]);
	   //测试解密通过
	}
	printf("\r\n");
	printf("Recover  Decrypt function Test!\r\n");
    memset(&tmp0, 0, 256);
	 bn_read_string_to_bytes(cipher,256,CIPHER_2048);
    TestReturn = RSARecover(Test_mh_prikey.n, 256, Test_mh_prikey.d, 256, cipher, tmp0);
   	printf("The return value is %d\r\n",TestReturn);
	for(i=0;i<256;i++)
	{
		printf("%X\t",tmp0[i]);
	   //测试解密通过
	}
}
void TestRsa()
{
	TestReturn = RSAKeyPairGen(&TestPublicKey,&TestPrivateKey,2048,1);
	printf("The  RSAKeyPairGen(&TestPublicKey,&TestPrivateKey,2048,1) return value is %d\r\n",TestReturn);
    TestReturn = RSAKayPairVerify(TestPublicKey,TestPrivateKey);
	printf("The RSAKayPairVerify(TestPublicKey,TestPrivateKey) return value is %d\r\n",TestReturn);
    TestPubulicKey();
}
void Test_RsaRecover(void)
{
	unsigned char pbyModule[100];
	unsigned char pbyExp [10];
	unsigned char pbyDataIn [100];
	unsigned char pbyDataOut [100];
	int dwExpLen = 3;
	int dwModuleLen = 96;
	int nRet = 1;
	int i;
	 
	memcpy(pbyModule,"\x94\x2B\x7F\x2B\xA5\xEA\x30\x73\x12\xB6\x3D\xF7\x7C\x52\x43\x61\x8A\xCC\x20\x02\xBD\x7E\xCB\x74\xD8\x21\xFE\x7B\xDC\x78\xBF\x28\xF4\x9F\x74\x19\x0A\xD9\xB2\x3B\x97\x13\xB1\x40\xFF\xEC\x1F\xB4\x29\xD9\x3F\x56\xBD\xC7\xAD\xE4\xAC\x07\x5D\x75\x53\x2C\x1E\x59\x0B\x21\x87\x4C\x79\x52\xF2\x9B\x8C\x0F\x0C\x1C\xE3\xAE\xED\xC8\xDA\x25\x34\x31\x23\xE7\x1D\xCF\x86\xC6\x99\x8E\x15\xF7\x56\xE3", 96);
	memcpy(pbyExp,"\x01\x00\x01", 3);
	memcpy(pbyDataIn,"\x56\x69\x91\x43\x20\xB9\x6C\xB5\x44\x25\xEF\x78\x41\xC8\x75\xC7\x67\xDD\x82\xB3\x5F\xE5\xC1\x35\x2B\xA2\x8D\x15\xC1\xAC\x99\xF7\xA6\x8D\x38\xD9\xDC\x25\xF5\x6E\x4A\x76\xE8\xEF\x79\x65\xC6\x28\xEB\xF0\x83\x45\x5B\x68\x0E\x62\xAE\x98\xED\x24\xA0\x80\x4A\x7A\x76\xC5\x98\x98\x25\x77\x2B\x22\x5B\xB7\xEE\xA5\x7D\x20\x4C\xC6\x70\x39\xD4\x50\x12\xB8\xFB\xA3\x13\xE5\x5A\x48\x7B\xBA\xCC\xC1", 96);
	 
	nRet = RSARecover (pbyModule, dwModuleLen, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);
	printf("\r\n");

	for(i=0;i<96;i++)
	{
		printf("%X\t",pbyDataOut[i]);
	}
		
	printf("\r\n Printf finish!\r\n");


	
 
}


int DEVICE_RSARecover(unsigned char *pbyModule, unsigned int dwModuleLen,
       unsigned char *pbyExp, unsigned int dwExpLen,
       unsigned char *pbyDataIn, unsigned char *pbyDataOut)
{
 int nRet = 0;
 
 if (pbyModule == NULL || pbyExp == NULL || pbyDataIn == NULL || pbyDataOut == NULL)
 {
//  return DEVICE_PARAM_ERROR;
 }
 nRet = RSARecover(pbyModule, dwModuleLen, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);
	return nRet;
}



void  Test_RsaRecover1(void)
{
  unsigned char pbyModule[256];
  unsigned char pbyExp [10];
  unsigned char pbyDataIn [256];
  unsigned char pbyDataOut [256];
    int dwExpLen=3;
    int dwModuleLen=247;
    int nRet = 1;
	int i;
  
 memcpy(pbyModule,"\x9C\x60\x60\x93\x03\xF8\xD2\x26\x80\x9F\x26\xBD\xB2\x7C\xC2\x1D\xBD\x47\x48\x0D\x3A\x44\xA8\xE8\xDD\x59\xD5\xAA\x8A\xEB\x85\x24\x2C\x2C\x80\xAA\xFA\x2C\x8E\x26\xE3\xF4\x3F\x79\xEE\xCF\xE9\xC0\x3E\x8B\x1B\x44\x7B\x57\x03\x7F\xCB\x8B\x90\xE1\xB4\x35\x5B\x58\x54\x41\x83\xAB\x5B\x63\xC4\x60\x54\x84\x20\xAD\xD8\x31\x10\x22\x3F\x49\x7E\x45\x6E\xBA\xC5\x85\x0D\x4E\xBC\x71\x03\x04\x20\xCC\x03\xF1\xD6\x7E\xC2\xAC\x66\x83\x32\xFF\x61\x20\x62\x96\x92\x70\x7B\xC5\x2F\x17\x4E\x24\x66\x22\x81\x41\x5D\xFB\x2C\xBB\x53\xEC\x93\x88\x12\xCD\x76\xA3\x3D\xA6\xFA\xF3\x2F\xBA\x17\x4D\x06\xC9\xB9\xF1\xE0\xFC\x2A\xE0\x49\xA3\xA3\x0B\xD3\xC5\x87\xC4\x89\x55\xA9\xCE\x33\xA4\x36\x95\x09\x79\xD5\x6C\x04\xE3\x0B\x78\xB5\x97\x7B\x00\xEA\x8D\x85\x4E\x64\x5C\x8D\x6C\x8E\x4B\xD7\x47\x33\x4D\x97\x1F\x0B\x11\x22\x98\x6E\x2E\x72\x7D\x01\xC2\xA1\xA9\xDB\xEF\x40\x30\xE1\x7B\x34\xA1\x89\xDA\xAA\x3F\x36\x28\x2E\x13\x7E\xC1\xB2\x30\x10\xB5\xCA\x56\xDF\x15\xCE\x37\xB5\xE3\x10\x96\xF1\xAA\xC9\xDB\x03\x5D\x04\x0E\xA1", 247);
 memcpy(pbyExp,"\x01\x00\x01", 3);
 memcpy(pbyDataIn,"\x1A\x04\x79\xEF\x7C\x0E\x24\x56\x9B\x0A\x68\x38\x24\x18\x5F\x8C\x6F\x8A\x87\xD6\x48\xA7\x7B\x05\x87\xC3\x32\xC0\x9C\xD9\x15\x0B\xD7\x0F\x7D\x7D\x35\x30\xEB\x7B\x8D\xCB\x1E\xB7\x86\x82\xEE\x23\x0D\xB7\x7D\x05\x5E\x2D\xFF\x5A\x08\xF4\xA9\xA8\xCE\xCD\x7A\x84\x82\xE8\x55\x1B\x55\xC0\x17\xB9\x5F\x76\xF3\xB5\x20\x3A\xB1\x08\xE7\x1A\x1F\x55\x0E\xC3\x48\xBF\x1D\x41\xB6\xFC\x74\x03\x04\x95\xC6\xD7\x35\xC9\x57\xB6\x4F\x43\x05\x6E\x9E\x09\x14\x8A\x31\x3F\x9C\xB9\xE5\x84\xFB\x80\x03\x88\x03\x61\x81\x6C\xD5\xA1\xA0\x04\x7C\x42\x41\x27\xE0\xE0\xEF\x16\x1D\x9D\x54\x98\x75\x1F\x01\x58\xD5\xE0\xDC\x5A\xF0\x92\x34\x7F\x49\xA7\x3A\x25\x80\xAD\x21\x8B\xD2\x53\xF5\x00\x9D\xB5\x59\x27\xF9\x6D\xC6\xD0\x95\x4F\xC9\xA4\x98\x65\x21\x20\x25\xE8\xA8\x78\x0F\x0A\x2B\x2F\xEF\x4B\x4C\xBB\x83\xE5\x41\x58\x13\x06\x56\xF8\xF8\x0B\x4F\xA9\x18\xD5\x66\xFD\x4B\x8A\x79\xC6\x04\x07\xEC\x29\xC2\x13\x96\x26\x88\x43\x13\xF2\xEC\x81\xFE\x36\x5E\xDA\x50\x28\x1C\xEE\xAB\x42\x9C\x3D\x8D\x68\x38\x38\x38\x23\xF8\x5B\xDF", 247);
  
 nRet = DEVICE_RSARecover(pbyModule, dwModuleLen, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);


	printf("\r\n pbyDataOut is:\r\n");
	for(i = 0; i < 100; i++)
	{
	   printf("%x\t", pbyDataOut[i]);
	}
	printf("\r\n Printf finish!\r\n");
//	APP_DEVICE_ScrCls();
//	APP_DEVICE_Lcdprintf("nRet = %d %02x %02x", nRet,pbyDataOut[0],pbyDataOut[95]);
//	APP_DEVICE_kbflush();
//	APP_DEVICE_getkey();
  
}




/*****************************************************************************
 Prototype    : SM2RecoverTest
 Description  : test the function of the SM2RecoverTest()
 Input        : void  
 Output       : None
 Return Value : 
  History        :
  1.Date         : 2016/7/7
    Author       : david
    Modification : Created function

*****************************************************************************/
void SM2RecoverTest( void )
{
        uint ret = 0;
	ST_SM2_INFO info;
	uint outputlen = 0;
	uint inputlen = 0;
	uchar i = 0;
	uchar tmp[20] = {0};
	uint tmplen = 0;

	uchar datain[16] = {
	0x39, 0x12, 0x39, 0x52, 0x08, 0x0b, 0x17, 0x5c,
	0x39, 0x1e, 0x04, 0x22, 0x12, 0x49, 0x18, 0x59
	};
	uchar Value[] = {
	        0x45, 0xea, 0x7a, 0xec, 0x89, 0xaf, 0xcb, 0xee, 
	        0x69, 0x14, 0xa2, 0xc8, 0x67, 0xe6, 0x9f, 0xa7, 
	        0xde, 0xf6, 0xa4, 0xd9, 0x51, 0xa0, 0xf7, 0x08, 
	        0xad, 0x9f, 0xeb, 0x67, 0x6e, 0x57, 0x95, 0xd4, 
	        0xc9, 0xa1, 0x61, 0xc3, 0xb0, 0xe8, 0xc9, 0xab, 
	        0xf0, 0x80, 0x6d, 0xd4, 0xca, 0x4e, 0x86, 0x1c, 
	        0x98, 0x61, 0xa9, 0xbc, 0x2d, 0x73, 0x25, 0x33, 
	        0x82, 0x97, 0xad, 0x54, 0x1c, 0x96, 0x35, 0x4d, 
	        0xab, 0x56, 0x83, 0x8d, 0xc2, 0x20, 0x19, 0xda, 
	        0x13, 0xa1, 0x96, 0xe1, 0xd4, 0x7f, 0xe6, 0xfe, 
	        0x1f, 0x48, 0x34, 0x65, 0x28, 0xfb, 0xc2, 0x8e, 
	        0xf1, 0xe6, 0xcc, 0x0b, 0x21, 0xd6, 0x43, 0xb7, 
	        0x04, 0xdc, 0x6e, 0x4e, 0xcb, 0xf6, 0x4b, 0x22, 
	        0xf1, 0xcf, 0xaa, 0x6a, 0x63, 0xe2, 0x3f, 0xba

	};
	uchar dataout[150] = {0};
        uchar PubKey[64] = {
	0xd6, 0x21, 0x46, 0x9f, 0x03, 0xf9, 0xe5, 0x23, 
	0x1a, 0x83, 0x28, 0x99, 0x41, 0xa4, 0x24, 0x1c, 
	0x08, 0x5e, 0x6c, 0x7c, 0x87, 0xf2, 0xf3, 0xf4, 
	0x65, 0x2e, 0x96, 0xef, 0xaa, 0x6e, 0xee, 0x5b, 
	0x51, 0x03, 0xdd, 0xe9, 0x71, 0xd6, 0x7d, 0xd2,
	0xbd, 0x68, 0xc6, 0x13, 0xde, 0x76, 0xf7, 0x02, 
	0xcb, 0x44, 0x92, 0x3b, 0x46, 0x72, 0x62, 0x96, 
	0x64, 0x12, 0x80, 0x09, 0xcd, 0x9a, 0x44, 0x6d
	};
	uchar PriKey[32] = {
	0xa7, 0x3e, 0x39, 0x88, 0x4a, 0xec, 0x94, 0xe4, 
	0xa0, 0x74, 0x3d, 0x88, 0x81, 0x81, 0x90, 0x0d, 
	0x10, 0x9d, 0x65, 0x14, 0xaa, 0x8f, 0x54, 0xd9, 
	0xbf, 0x7a, 0x21, 0xfb, 0x23, 0xf1, 0x1c, 0xe8
	};
    
	const uchar Ida[] = "0514134858024b5b01272b5059270253"; 

	info.idalen = 128;		
	bn_read_string_to_bytes(info.ida, sizeof(info.ida), Ida);
	memcpy(info.key.pubkey, PubKey, 64);
	memcpy(info.key.privkey, PriKey, 32);
	
	//注意，每次加密，得到的数据不一样
	ret = SM2Recover(&(info.key), datain, sizeof(datain), dataout, &outputlen, 1); 
	printf("\r\n打印加密数据!\r\n");
	for(i=0;i<outputlen;i++)
	{
		printf("%X\t",dataout[i]);
	}
	printf("\r\n加密数据打印完成!\r\n");

	ret = SM2Recover(&(info.key), dataout, outputlen, tmp, &tmplen, 0); 
	if((sizeof(datain) != tmplen)||(0 != memcmp(tmp, datain, tmplen)))
	{
		printf("\r\n解密数据失败!\r\n");
	}
	
	//解密特定的数据测试
	ret = SM2Recover(&(info.key), Value, sizeof(Value), dataout, &outputlen, 0);       //解密测试
	printf("打印解密数据!\r\n");
	for(i=0;i<outputlen;i++)
	{
		printf("%X\t",dataout[i]);
	}
	printf("\r\n");
	printf("解密数据打印完成!\r\n");

	if((sizeof(datain) != outputlen)||(0 != memcmp(dataout, datain, outputlen)))
	{
		printf("datain(dataout)-%d:", outputlen);
	    for(i = 0; i < outputlen; i++)
	    {
	       printf("%x(%x),", datain[i], dataout[i]);
	    }
	    printf("inputlen=%d outputlen=%d \n", sizeof(datain), outputlen);
	    printf("SM2RecoverTest Fail.\n");
	} 
	else
	{
		printf("SM2RecoverTest Succeed.\n");
	}
}

#define PLAIN "1234567890"
#define KEY_LEN_1024        1024 / 32
#define RSA_N_1024          "CDE42E9228172CBE4C542194BBB112728E0180FF7196B849D74F0630ED92AA5C4E04224E10C43881EBD710151E1F6BE47B2F688EAB880AC3D876C9702378CB524CA85637DE795D4393D129672DB4655C1EE82959ED051A92C1902135751CC4EC3E5B9050D5DF7A40610F61E382D864CC85F49F676E7C58A56E275D32648A65AB"
#define RSA_E_1024          "00010001"
#define RSA_D_1024          "B5C1ED76EB5B620D709688BAA362F3351BB5AA2A571D45FD31CCBA5B712F1BA3B6DFA165271A020477AD78AE1BD4834BC58F998EF1507CA85DACA7B18E2EF1D5548723BD59DF8DA9B108DA2B5A455E9F3F12CD2DC0E549FF3EA534E0690AA393042062290931FA2B5AC1E9A1753C4791D74641103AFF067F7DD86B02C768CC59"
#define RSA_P_1024          "F2DEB75C74A29F4FB17405ED8B584E63839F1822029FADB86AB4AAEA5E76E915503BB6C44BCE3194026FFF47170D3725AEC18B99348DB40B6B5522FAB81CE3A7"
#define RSA_Q_1024          "D905AFCF9AEEDAE746550927BA6B3C8A32EDEE8C16DC8306027FF54C159CBC847DF4FD4841FFF57E50E8A675C9CAC2428C2BD75B00B7BA74243F3898DBF4FE5D"
#define RSA_DP_1024         "4A1B14FDDC04E6F16D2706CC0591AB51880EDA513E55B4AF29B7F8D2CC8221FAC3E18491FEC36AB34BCF1AA8796B29E190A233D34A8AD65E09A10FAC4AD35DCF"
#define RSA_DQ_1024         "06CB8AC9B14C314674894D14155E2E33158D259EF5484A41F1EA17C01E1D1E39F7C7A7F68F3A7871500F38C70F5E6DDEAF8CDFDD55946B1E1FD46AED3E55D971"
#define RSA_QP_1024         "7E4BDC3E7FA115F1AEFD49CE2FFA474E4777FFA5195A6909919DD093CFACDC98A13C72EFA2476F148E6BE7F330E4143BEDF8B9D965D3A3A2657890049DE98635"

#define RSA_N_C_1024        "64EFEDD782B6A49AEA1872EDD9275E49931139CCFF5AC92433B07B0A9EA6693FA7C9D1B3149E17C8DD0A1AB8548844EB6E0F50DC894AF0593E4E5B7E27690FB576642F98A1A33C22FF382DAFAEAECDFFDDFD3DD09A207C5B335DEC99C5D137426421E044312602DA6901A218EAB977D6A5C7B5A60191A4BDE0DBC63C407FEF8D"
#define RSA_P_C_1024        "5DA1CD5FA89C67FA7728E97B67BAAE72D1A9F03E7CC032ADB9EDAC3949667F4B04A1196D73F5D8A375B9E7E13A95D7F934ABB508EC96DE712644621864CB94B8"
#define RSA_Q_C_1024        "4017B2E1F5500979E663ECE3A049678E49BCD4C822BE2E2C6C6C815A2EC23F5EEE7C153C6A2C2677F2419792F2BA6BE8B4C42943267922BF02B02AAAE7071E23"
#define RSA_N_Q_1024        "85BA92FD"
#define RSA_P_Q_1024        "3AC26BE9"
#define RSA_Q_Q_1024        "98B83A0B"
#define CIPHER_1024         "4257A706BDB0F1AEF8957E5B00ADB1BB2386AAC79394180049AF03C5AE12642E20D33D725408E361FBB600871E0AD925DAB6AA76CE6324E9189B33C99106BA45317FD8639EAF2F6DCA03DEA33D0DCE37D9493F917A1DB8A47F4FC249D44B22775D12908BE2F8F95A8451A0738A9961BD4E905741AB6A16EA2D600A5BB95AF399"




///////////// 我的RSA测试程序  ///////////////
#define PLAIN_TEXT "123456789"
#define N_TEXT     "812D98AFF0A009ACD514059DBFCC8DC83E1E072780A206652DE62AC77A366A4C184799DB9A99DFEA44AF0EFD0111A51A77C9E72807D5C50F7F180A2AC20583EE59E95F85DFE91B1FA7E008581227DC89B5880116FDEB93D5ECC45CCDCF02DEDAB0E2D2BCA387D07CEBF584FE2148689759FD50F06911CBC67FAD168E2EAD1D8B"
#define D_TEXT	   "5B4741DE08B4BD41702B3EB0B247B363EC68C17B6B4C92224AA59A52EC0BA95EEDC52275E7AF95666F7417AE3564277B256BF8B7F3995D2ECF0AC89C600F0FA28A8F971AA3274039AF16AC87F77291AC663E9497B1F0620473B4DB24D2B5B5E582B4283791B3A7055EE8C622EC6D3B0FD44490503E90CC251F1ECA044D6A4A81"

#define CIPHER_TEXT  "22AFD1AD909A6317FA4C32C7CBFEFC579209B75BCC2C131F858547C5040F77AD5723F81188F91B48651BD663E5512ABA2823C6CC83E9423E73818021D571C6A176A9AECAC05152F507D55990D23C545CB2868648FCE6CDEC6B557B4233C7791FAC155089C7A81AA54770147598A100023102C29788E5BFF034F3DCE4F2F471EF"

#if 0
void MyRsaRecoverTest()
{

	unsigned char pbyModule[128];
	unsigned char pbyExp [4] = {0x00,0x01,0x00,0x01};
	unsigned char pbyDataIn [9] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
	unsigned char pbyDataOut [128];
	int dwExpLen = 4;
	int dwModuleLen = 128;
	int nRet = 1;
	int i;
	
	bn_read_string_to_bytes(pbyModule, sizeof(N_TEXT)/2, N_TEXT);
	nRet = RSARecover (pbyModule,128, pbyExp, dwExpLen, pbyDataIn, pbyDataOut);
	printf("\r\n");

	for(i=0;i<128;i++)
	{
		printf("%X\t",pbyDataOut[i]);
	}
		
	printf("\r\n Printf finish!\r\n");
}



void RsaTimeTest()
{
		uint32_t  time[2];
		R_RSA_PUBLIC_KEY pubkey;
	
		unsigned char out[256] = {0};
		unsigned int  outlenth;
		unsigned char input[10] = "1234567890";
		unsigned int inputlenth;
		unsigned int i;
    unsigned char tmp[4];

	
		memset(&pubkey,0,sizeof(pubkey));
   	pubkey.bits = 1024;
		bn_read_string_to_bytes(pubkey.modulus, sizeof(RSA_N_1024)/2, RSA_N_1024);
		bn_read_string_to_bytes(pubkey.exponent, sizeof(RSA_E_1024)/2, RSA_E_1024);


		printf("\r\nPrintf the pubkey.modulus!\r\n");
		for(i=0;i<sizeof(RSA_N_1024)/2;i++)
		{
			printf("0x%2x\t",pubkey.modulus[i]);
		}
		printf("\r\nPrintf the pubkey.exponent!\r\n");
		for(i=0;i<sizeof(RSA_E_1024)/2;i++)
		{
			printf("0x%2x\t",pubkey.exponent[i]);
		}
	
	  SYSCTRL_GetClocksFreq(&clocks);
		timer_init_ms(1);
		timer_start(0);
		printf("\r\n Index 1! \r\n");
		
	  RSAPublicDecrypt(out,&outlenth,input,10,&pubkey);

		
	  printf("\r\n Index 2! \r\n");
		printf("\r\nPrintf the outputdata!\r\n");
		for(i=0;i<outlenth;i++)
		{
			printf("0x%2x\t",out[i]);
		}
		
		time[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
		time[0] += timer_tick(0) * 1000;
	  __nop();
	  printf("\r\nThe time is %d\r\n",time[0]);

}









void ClientTest()
{
//	RsaTimeTest();
	MyRsaRecoverTest();
//	Test_RsaRecover1();
//	Test_RsaRecover();
//	TestRsa();
//	TestSm2();
// 	SM2RecoverTest();
    while(1);
}
#endif





















