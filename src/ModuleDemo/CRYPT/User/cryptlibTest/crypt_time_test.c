#include <string.h>
#include <stdio.h>
#include "mhscpu_uart.h"
#include "mhscpu_timer.h"
#include "debug.h"
#include "mh_crypt.h"
#include "mh_aes.h"
#include "mh_des.h"
#include "mh_sha.h"
#include "mh_sm2.h"
#include "mh_sm3.h"
#include "mh_sm4.h"
#include "mh_rsa.h"
#include "mh_rand.h"
#include "mh_bignum.h"
#include "mh_bignum_tool.h"

#define AES_TIME_TEST_DATA				1024*4
#define AES_TIME_TEST_COUNT				1
#define DES_TIME_TEST_DATA				1024*4
#define DES_TIME_TEST_COUNT				1
#define SM2_TIME_TEST_DATA				128
#define SM2_TIME_TEST_COUNT				1
#define SM2_TIME_TEST_SIGN_DATA			128
#define SM2_TIME_TEST_SIGN_COUNT		1
#define SM3_TIME_TEST_DATA				1024 * 10
#define SM3_TIME_TEST_COUNT				1
#define SM4_TIME_TEST_DATA				1024*4
#define SM4_TIME_TEST_COUNT				1
#define SHA_TIME_TEST_DATA				1024 * 10
#define RSA_TIME_TEST_COUNT				5

#define MEM_TIME_TEST_DATA				1024
#define MEM_TIME_TEST_COUNT				1

#define RSA_GEN_KEY_TIME_TEST_COUNT 	10

mh_rng_callback f_rng = mh_rand_p;

void mh_aes_time_test(void)
{
	uint32_t i, j, k, time[2] = {0,0};
	mh_pack_mode_def modes[2] = { ECB, CBC};
	char *cmodes[2] = {"ECB", "CBC"};
	mh_aes_key_size_def key_size[3] = {MH_AES_128, MH_AES_192, MH_AES_256};
	char *ckey_size[10] = {"MH_AES_128", "MH_AES_192", "MH_AES_256"};

	
	uint8_t plain[2][AES_TIME_TEST_DATA];
	uint8_t cipher[AES_TIME_TEST_DATA];

	uint8_t key_128[] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
						0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};
	uint8_t key_192[] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E, 0x64, 0x52, 
						0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 
						0X62, 0xF8, 0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};
	uint8_t key_256[] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 
						0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 
						0x1F, 0x35, 0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 
						0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14, 0xDF, 0xF4};
		
	uint8_t iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
					0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

	uint8_t *key[3];
					
	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);

	key[0] = key_128;
	key[1] = key_192;
	key[2] = key_256;
	

	memset(plain, 0, sizeof(plain));
	mh_rand(plain[0], sizeof(plain[0]));
					
	i = 0xfffff;
	while(i--);


	DBG_PRINT("\n%d bytes data size\n", AES_TIME_TEST_DATA);

	//key 128 192 256
	for (i = 0; i < 3; i++)
	{
		//pack mode ECB CBC
		for (j = 0; j < 2; j++)
		{
			memset(time, 0, sizeof(time));
			for (k = 0; k < AES_TIME_TEST_COUNT; k++)
			{
				memset(cipher, 0, sizeof(cipher));
				memset(plain[1], 0, sizeof(plain[1]));
				//AES enc
				
				timer_init_ms(1);
				timer_start(0);
				mh_aes_enc(modes[j], cipher, sizeof(cipher), plain[0], sizeof(plain[0]), key[i], key_size[i], iv, f_rng, NULL);
				time[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
				time[0] += timer_tick(0) * 1000;
				//AES dec
				timer_init_ms(1);
				timer_start(0);
				mh_aes_dec(modes[j], plain[1], sizeof(plain[1]), cipher, sizeof(cipher), key[i], key_size[i], iv, f_rng, NULL);
				time[1] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
				time[1] += timer_tick(0) * 1000;

				if(!memcmp(plain[0], plain[1], sizeof(plain[0])))
				{
//					DBG_PRINT("AES %s %s Test Pass...\n", cmodes[j], ckey_size[i]);
				}
				else
				{
					DBG_PRINT("AES %s %s Test Fail...\n", cmodes[j], ckey_size[i]);
				}
			}
			DBG_PRINT("%dus AES %s %s encrypt time\n", time[0], cmodes[j], ckey_size[i]);
			DBG_PRINT("%dus AES %s %s decrypt time\n", time[1], cmodes[j], ckey_size[i]);
		}
	}
}


void mh_des_time_test(void)
{
	uint32_t i, j, time[2] = {0,0};
	mh_pack_mode_def modes[2] = { ECB, CBC};
	char *cmodes[2] = {"ECB", "CBC"};
	
	uint8_t plain[2][DES_TIME_TEST_DATA];
	uint8_t cipher[DES_TIME_TEST_DATA];
		
	uint8_t iv[] = {0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17};
	uint8_t key[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);

	timer_init_ms(1);

	memset(plain, 0, sizeof(plain));
	mh_rand(plain[0], sizeof(plain[0]));

	i = 0xFFFFF;
	while(i--);

	DBG_PRINT("\n%d bytes data size\n", DES_TIME_TEST_DATA);

	//pack mode ECB CBC
	for (i = 0; i < 2; i++)
	{
		memset(time, 0, sizeof(time));
		for (j = 0; j < DES_TIME_TEST_COUNT; j++)
		{
			memset(cipher, 0, sizeof(cipher));
			memset(plain[1], 0, sizeof(plain[1]));

			timer_init_ms(1);
			timer_start(0);
			mh_des_enc(modes[i], cipher, sizeof(cipher), plain[0], sizeof(plain[0]), key, iv, f_rng, NULL);
			time[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			time[0] += timer_tick(0) * 1000;

			timer_init_ms(1);
			timer_start(0);
			mh_des_dec(modes[i], plain[1], sizeof(plain[1]), cipher, sizeof(cipher), key, iv, f_rng, NULL);
			time[1] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			time[1] += timer_tick(0) * 1000;
			
			if(!memcmp(plain[0], plain[1], sizeof(plain[0])))
			{
//				DBG_PRINT("DES %s Test Pass...\n", cmodes[i]);
			}
			else
			{
				DBG_PRINT("DES %s Test Fail...\n", cmodes[i]);
			}
		}

		DBG_PRINT("%dus DES %s encrypt time\n", time[0], cmodes[i]);
		DBG_PRINT("%dus DES %s decrypt time\n", time[1], cmodes[i]);
	}
}


void mh_tdes_time_test(void)
{
	uint32_t i, j, time[2] = {0,0};
	mh_pack_mode_def modes[2] = { ECB, CBC};
	char *cmodes[2] = {"ECB", "CBC"};
	
	uint8_t plain[2][DES_TIME_TEST_DATA];
	uint8_t cipher[DES_TIME_TEST_DATA];

		
	uint8_t iv[] = {0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F, 0x9B, 0x17};
	mh_tdes_key_def key = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF},
						{0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01},
						{0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23}};

	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);

	timer_init_ms(1);

	memset(plain, 0, sizeof(plain));
	mh_rand(plain[0], sizeof(plain[0]));

	i = 0xFFFF;
	while(i--);

	DBG_PRINT("\n%d bytes data size\n", DES_TIME_TEST_DATA);

	//pack mode ECB CBC
	for (i = 0; i < 2; i++)
	{
		memset(time, 0, sizeof(time));
		for (j = 0; j < DES_TIME_TEST_COUNT; j++)
		{
			memset(cipher, 0, sizeof(cipher));
			memset(plain[1], 0, sizeof(plain[1]));

			timer_init_ms(1);
			timer_start(0);
			mh_tdes_enc(modes[i], cipher, sizeof(cipher), plain[0], sizeof(plain[0]), &key, iv, f_rng, NULL);
			time[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			time[0] += timer_tick(0) * 1000;

			timer_init_ms(1);
			timer_start(0);
			mh_tdes_dec(modes[i], plain[1], sizeof(plain[1]), cipher, sizeof(cipher), &key, iv, f_rng, NULL);
			time[1] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			time[1] += timer_tick(0) * 1000;

			if(!memcmp(plain[0], plain[1], sizeof(plain[0])))
			{
//				DBG_PRINT("TDES %s Test Pass...\n", cmodes[i]);
			}
			else
			{
				DBG_PRINT("TDES %s Test Fail...\n", cmodes[i]);
			}
		}

		DBG_PRINT("%dus TDES %s encrypt time\n", time[0], cmodes[i]);
		DBG_PRINT("%dus TDES %s decrypt time\n", time[1], cmodes[i]);
	}
}


void mh_sha_time_test(void)
{
	uint32_t i, time = 0;
	mh_sha_mode_def modes[5] = {SHA_160, SHA_224, SHA_256, SHA_384, SHA_512};
	char *cmodes[5] = {"SHA_160", "SHA_224", "SHA_256", "SHA_384", "SHA_512"};
	
	uint8_t plain[SHA_TIME_TEST_DATA];
	uint8_t cipher[512 /8];

	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);

	memset(plain, 0, sizeof(plain));
	mh_rand(plain, sizeof(plain));

	DBG_PRINT("\n%d bytes data size\n", SHA_TIME_TEST_DATA);

	for (i = 0; i < 5; i++)
	{
		timer_init_ms(1);
		memset(cipher, 0, sizeof(cipher));
		memset(&time, 0, sizeof(time));
		timer_start(0);
		mh_sha(modes[i], cipher, plain, sizeof(plain));
		time = timer_counter() / (clocks.PCLK_Frequency / 1000000);
		time += timer_tick(0) * 1000;
		{
			DBG_PRINT("%dus time %s Test\n", time, cmodes[i]);
		}
	}
}


// #define KEY_LEN_1024		1024 / 32
// #define RSA_N_1024			"CDE42E9228172CBE4C542194BBB112728E0180FF7196B849D74F0630ED92AA5C4E04224E10C43881EBD710151E1F6BE47B2F688EAB880AC3D876C9702378CB524CA85637DE795D4393D129672DB4655C1EE82959ED051A92C1902135751CC4EC3E5B9050D5DF7A40610F61E382D864CC85F49F676E7C58A56E275D32648A65AB"
// #define RSA_E_1024			"00010001"
// #define RSA_D_1024			"B5C1ED76EB5B620D709688BAA362F3351BB5AA2A571D45FD31CCBA5B712F1BA3B6DFA165271A020477AD78AE1BD4834BC58F998EF1507CA85DACA7B18E2EF1D5548723BD59DF8DA9B108DA2B5A455E9F3F12CD2DC0E549FF3EA534E0690AA393042062290931FA2B5AC1E9A1753C4791D74641103AFF067F7DD86B02C768CC59"
// #define RSA_P_1024			"F2DEB75C74A29F4FB17405ED8B584E63839F1822029FADB86AB4AAEA5E76E915503BB6C44BCE3194026FFF47170D3725AEC18B99348DB40B6B5522FAB81CE3A7"
// #define RSA_Q_1024			"D905AFCF9AEEDAE746550927BA6B3C8A32EDEE8C16DC8306027FF54C159CBC847DF4FD4841FFF57E50E8A675C9CAC2428C2BD75B00B7BA74243F3898DBF4FE5D"
// #define RSA_DP_1024			"4A1B14FDDC04E6F16D2706CC0591AB51880EDA513E55B4AF29B7F8D2CC8221FAC3E18491FEC36AB34BCF1AA8796B29E190A233D34A8AD65E09A10FAC4AD35DCF"
// #define RSA_DQ_1024			"06CB8AC9B14C314674894D14155E2E33158D259EF5484A41F1EA17C01E1D1E39F7C7A7F68F3A7871500F38C70F5E6DDEAF8CDFDD55946B1E1FD46AED3E55D971"
// #define RSA_QP_1024			"7E4BDC3E7FA115F1AEFD49CE2FFA474E4777FFA5195A6909919DD093CFACDC98A13C72EFA2476F148E6BE7F330E4143BEDF8B9D965D3A3A2657890049DE98635"
// #define RSA_N_C_1024			"64EFEDD782B6A49AEA1872EDD9275E49931139CCFF5AC92433B07B0A9EA6693FA7C9D1B3149E17C8DD0A1AB8548844EB6E0F50DC894AF0593E4E5B7E27690FB576642F98A1A33C22FF382DAFAEAECDFFDDFD3DD09A207C5B335DEC99C5D137426421E044312602DA6901A218EAB977D6A5C7B5A60191A4BDE0DBC63C407FEF8D"
// #define RSA_P_C_1024			"5DA1CD5FA89C67FA7728E97B67BAAE72D1A9F03E7CC032ADB9EDAC3949667F4B04A1196D73F5D8A375B9E7E13A95D7F934ABB508EC96DE712644621864CB94B8"
// #define RSA_Q_C_1024			"4017B2E1F5500979E663ECE3A049678E49BCD4C822BE2E2C6C6C815A2EC23F5EEE7C153C6A2C2677F2419792F2BA6BE8B4C42943267922BF02B02AAAE7071E23"
// #define RSA_N_Q_1024			"85BA92FD"
// #define RSA_P_Q_1024			"3AC26BE9"
// #define RSA_Q_Q_1024			"98B83A0B"
// #define CIPHER_1024			"4257A706BDB0F1AEF8957E5B00ADB1BB2386AAC79394180049AF03C5AE12642E20D33D725408E361FBB600871E0AD925DAB6AA76CE6324E9189B33C99106BA45317FD8639EAF2F6DCA03DEA33D0DCE37D9493F917A1DB8A47F4FC249D44B22775D12908BE2F8F95A8451A0738A9961BD4E905741AB6A16EA2D600A5BB95AF399"


// #define KEY_LEN_2048		2048 / 32
// #define RSA_N_2048			"75A18418B8EB30CD990303626314215772406FB53C90BC34F8963643CB7C86655174E82AC581EA9D93FBCD8B9B9623E9FE8AD5E2F9311C8FEEFB709D78CC961DC49A3CA5F55733E68A10563695782DE1C8BE5F031E84540E14AA369FA8E03D03CCC7E02B2A030C44AF4B989DBC99640BB4925B9C3AAE287C241C1AF133502AD61593C62FAB42604680FBE1EC77C98B68008C15E52C24035A8F220C1B944C547F1E0FA4C6475395839536FD286F23063E802D657C6D176B46E3B1862616D32EADB0DC1C1AF83EAF33640224658BB46052ACD8D8876A7F35A9232ED4AD2E15FD958F294A5BFFBA11E2E18F7D3377136F6C91B93C9656E289E19FE3E928FDF7FB7F"
// #define RSA_E_2048			"10001"
// #define RSA_D_2048			"562B23211914F0832B7B5197590CFDFF98EA2AEA909B55C357F67179E82E2825402E6B84297AA9EB14E47805B4CB9F33620B41F32AA8174C38BC681A72CB2B8C52E0C95CA9046038991AAE8CFDA133BB19DA9AAF2643B96F3724192F8BFDF7B438E692080C967EFE318B27192577181A9B043BA78E34D5E35A705C2C2472AD6D762273180C7DA3107658259BC5BF554B3CC2CF33F264DDB3C45406A047A1B3720A655CAFF677FA7DCFDB9F125A8C1D333E07F93C884AFC7E9444D6033AAB3975DED23BC71BCA3EC83CCF000D6586020B1BF483762BFE6F1C64CE021CB85AD538A18B7539D42619C6901364ED6B14D07EC9EFECA16AC59D5BAB0F8840A5D86281"
// #define RSA_P_2048			"9AA3212A0A6E3158E399844D2F44C9CC3D25D5DDF1C18B2F49E70BB2326F1E738C33DFB87A4DAC5EED914F72724C670D393D1B7368D2006DA1A649E5D03930C239F0A304C04F3222787436A271D544F8BD3DD3A3C2032B84C68B9080B600AE57395146C6F46C18DA1DB9129544D8B9C65F9DD624164A64AB500EB83AFDBDF279"
// #define RSA_Q_2048			"C2BC8CFDC7F5C23DE47E95D39BD0EE16B27957280655ED4F0D0C8793AACD279B374BA0C3CFF78781DA7D5EF8DE83FDDB83A04386E599FC5A20E113571852FBFE88275A684C3BF6A1F187E3497DD8EC6B7ACE419F471C615DA1FD48A36EF4DBC9CBC692A258BB67D6C213FD7D8FC513FE307E2A2943D7739227D7FD2EC4AB1FB7"
// #define RSA_DP_2048			"598E6E4F8DB8E1A56C32E5775A0FC99B1A32C6588281A79D1C7CCDBE470D29A29BA9134120BBC93F34F0FA04FB031A8F82FDCBDF1BEEBB46C2DEAE5F85270A01025345E62385FA301278A38792CB83E5569BEEED21187AE4ABA3FA6D407AC67D4779B5B6D5A9A0FEF945CCE85BF5F4295A08EC12FC41DB6987B4EEA7637AD569"
// #define RSA_DQ_2048			"9B8767BBD096317AA921889C271828EFABBF49A2C7DC9CD9C3FC3BD3EE5847C7A9D417240CD70F3CE0B9BCBE5B4963D88C39D574C9AFC062635C1FCC31BB2553D67E47B081226B0A50134DAE067AD7E7C77F2FDDFC37687415CA6AD74CE8F719264D0413F1764BAAB3FAAF3C37351990AC3E37821992DBD4E1499A065E40A5E5"
// #define RSA_QP_2048			"906598444A1664A4F3FD0E67DA92E0BCBB445101DC09ED567ACE2D49DBEC61B2E06ECD64544B0C35D26E68DBE3048B83A601285245AF4FFC94E3EF5F7602E3EC9B8AD6D9FBC518F95D3A638D7A3FC34E72307F67A48C0696753664D589BE2EC861EB659C7AB06C5A9095E4D8794D49504E651EAEE26C76091175C5B7082B4016"
// #define RSA_N_C_2048			"65B37280142A48FCED5C720FC0A17DB2A6B6C1901CD1FECA252FEB61E5CF58E4068800EE4F54D2D9E35566BC5CB976B497E65FFA529F398E562BB38AE891A03B212D88E98073DE85A5A00D9B2CA11D0B14AF22CD2C832FD8DCD5E4ADC6BA95FFC7FBEBC80FE9D32ACB4EB6F52C742B11F11D83655E30B5FC786E15CC8C8D1C9F9EF66284D337B8A697E4F8485064D28823FD0DB4E75F5618437D776D6910405AB88B467F2E5DD4C206C670807E25227E61D02EE32B37C731DA1DF141D36CC48B636867C37C6A08C17EE8416A0A33A0791EE2233C3915EC38B2DA1984DE95F00BEAA1575DBD1ACCF4F8DB37500656BE4CD7A874F6247A127C2F32A046197D4D2A"
// #define RSA_P_C_2048			"0224D929A1397374039926557BCA9FC74DD9E7AB3DBAF1A216DC1E57ED4F57B1A7D6E922F3CB5FE825FA14264BBFA864FD6129885EABC7C21680C2390511AC7189EF4F7F7BDEA690843353C18235E9E56BA985F712D44B377AE1578321A61DFAB7240E12597A5513B055E455BBAA66BCD25AAB72129D830B6EFAFFBBA5D30132"
// #define RSA_Q_C_2048			"5AD49E385E01DBA437ED91437ED4C9C17E1EE4F5EACABF9624C3C8AF4B4712D35900C84522FADC41D5723AA652D33F2F25DB60112F8DE2D9F907E8EE7B4A84129AA9FFCEF7FA1FFB4E4E879C5D55CFD45757AAC16739C495D5C756BFA4C3DEF9A591962919C298495A9727252B345D9470C4BFA33F5121AA861952B0D4A3A3D9"
// #define RSA_N_Q_2048			"FAEC3B81"
// #define RSA_P_Q_2048			"7D3E2837"
// #define RSA_Q_Q_2048			"552511F9"
// #define CIPHER_2048			"1A489B964931E96ECBED9DB0A357B0912C203B3020BA1427353AD06CAD229A77F9E864A11BF0AD6A4FE2CB88D3C0F4AE0B964C6122DA581E2E509D9B7828E41BDDC6A179F43EF2B1DAB09C4D49ACCE3CD55FBFA256E264189C365A2E8BE9640B98A73267E9A5ACBD9C93838AD76ED57DCD8C680F248CAC97CDA034CC348A5754EBFAAA3A855FF618CE6BCA51BCBAC33D105D0F6BF58FE1C0E7F8DF4CB293D48A4EC5FEA617DFA00F1E6688893FCF5DC7B45DE3C511E14DF2D6D072511664BF397D322DD61952882CE88E8F1D770B14340B52AAA2D2A388BF2A1F6A5D7A13EF149FF9AEDFE0A2657DCB9568A06FBFFB800D1CFB7CA625630D8DA39174008F93F9"


#define KEY_LEN_1024		1024 / 32
#define RSA_N_1024			"C4095627747FB2F209852AC8A4A2019235A5CD7D6D963821AE61662A9986B389F1A1200260DDBBF41D60ACB6C04FBF95A527A2F9A14457D13E2C483A183F20A21B382CDE01FB83DA416FC0B598D7495699D6E2A130194274A18B26912F1CC0D780950CD507B6777577D276345B6EA25E56B932815466AE5E923B23DEBBC4AC6D"
#define RSA_E_1024			"00000003"
#define RSA_D_1024			"82B0E41A4DAA774C0658C7306DC15661791933A8F3B97AC11EEB9971BBAF225BF66B6AAC4093D2A2BE40732480352A63C36FC1FBC0D83A8B7EC83026BAD4C06AE66E0D32ED6052596550F6E5D60435A201D1CD7C76368503A9C8C6EF0C7C7D61A1522D5A3F11693D8D0144B064EEE85FE0FCF20E874328BA3937416CA26B56D3"
#define RSA_P_1024			"F35599A27FF31C8BE479D00BD485340DAC384E57B8F457525E6EF25FF2F1F6766151AA4BEB29BC727EDC3BDA685519C24FFCB3CEA4108BFAF682CCA10159D917"
#define RSA_Q_1024			"CE3D7F6F1DF7EBC844FC7E51034BC4D5EAE3E00EC5D3239CC46F09CAA9700E4EAD481E81BDF29D26A57453515BB32C0C3541139CE571654C45E5751AC6C9D11B"
#define RSA_DP_1024			"A2391116FFF7685D42FBE007E30378091D7ADEE525F83A36E99F4C3FF74BF9A440E11C329CC67DA1A9E827E6F038BBD6DFFDCD346D605D51F9AC886B563BE60F"
#define RSA_DQ_1024			"897E54F4BEA547DAD8A85436023283394742955F2E8CC26882F4B131C64AB434738569ABD3F7136F18F8378B927772B2CE2B626898F643882E98F8BC84868B67"
#define RSA_QP_1024			"8AF45F66D106E972BF455A06BFE502EF2726E952AA507DFD74529CCF26A0B12EE657CAC37F42ACC66C16D6DF5FA60005DA3F09FA9DEDD9DC66A6978AEDD291D1"
#define RSA_N_C_1024		"162AD3CFBF650A75DA1892BFBC874B69EF3E133567392C095257174A261F7DBCA7A171B29BD846CAB62C9720A862D201E2307DF989934D10AE2917A7F6A566C311CF31DFEA1873DAF8A9052FFDB62479172CCC589C31B1F457046D77A4D6D7A248C9FDC8BA69B327C436F0E3F1F06CE07264A0C821BF407F68374E1C05811145"
#define RSA_P_C_1024		"4662C73BB0196E3286CEF95D25CDA124CC3602D34DE7B20FCF649CD18D3FBC51C4E2C704AD06B1435FF4DAEBE970B62BFD0E3D1AB10258D83B2F99BD8E07B60D"
#define RSA_Q_C_1024		"79533CFE6A973E477CE898AE4DD709DD680CB5901690CAD140979F13CA5B4583A79B749B4741FC58ABA28F7F7C97591B28E98E8E3204F13EA38988E289E6A710"
#define RSA_N_Q_1024		"CFA5C29B"
#define RSA_P_Q_1024		"E80F1159"
#define RSA_Q_Q_1024		"6CBCDEED"
#define CIPHER_1024			"4257A706BDB0F1AEF8957E5B00ADB1BB2386AAC79394180049AF03C5AE12642E20D33D725408E361FBB600871E0AD925DAB6AA76CE6324E9189B33C99106BA45317FD8639EAF2F6DCA03DEA33D0DCE37D9493F917A1DB8A47F4FC249D44B22775D12908BE2F8F95A8451A0738A9961BD4E905741AB6A16EA2D600A5BB95AF399"


#define KEY_LEN_2048		2048 / 32
#define RSA_N_2048			"A38BFB61ACB56C8559356E46B1519E9D20C25284BC05D28E3FFB1A867C8F3F42F64E653D9C18CBA0DBD489357D700049185AEA3114B58FF70A99EB678A9E6A38F9DCDA3595535C15EFC064BE10C1B9C8C2768C9873E775D7ED1A7850DD03ABEB7792F11D4721D9AD72EA471F83CFA51410443AD8F703BE026E1C0D06EBD38E8622B88A035466FC9BA1552B4AAE683E84D58EA0A82821EDCA43F58D8D805D303064B87072DC27D276E877FE64C12498CE9CA50525943203D8EFAD2D90F0E1D737B53D6A1BF6DC829E88CD1EF506025E37625D6D70F55BFFE66AC66D81C32F25DC8CEFC8152C8527C2850645F461306DDE871583A5B39A66C501810EEADE373D89"
#define RSA_E_2048			"00000003"
#define RSA_D_2048			"6D07FCEBC878F30390CE4984763669BE15D6E1ADD2AE8C5ED55211AEFDB4D4D74EDEEE2912BB326B3D385B78FE4AAADB6591F1760DCE5FFA0711479A5C699C25FBE891790E3792B94A80432960812685D6F9B3104D44F93A9E11A58B3E02729CFA61F61384C13BC8F746DA15028A6E0D602D7C90A4AD2956F412B359F28D09ADB0C4455EEFB5CE3608C12BC536A520864319F6EBDEC3D89B8D1F89B466F98F97C756BB84F86BEF2F02517AA516B4747DB211BD5C33832BF220A68E6E99D5EC2ECDFC33AA8A130A956612EB4EAAED1901956867FA57353E1C14858924CAA69E8CE018CDF5F0E7611F72DCF1B03A48536E682C18D10348033B0C42B10080B2AD83"
#define RSA_P_2048			"D518DC9847F40846436574A639C2CD289B1788049B872D5B1CAD4D491088269AAD3286357C9D03C8CA54C2D31908E84E05741B03DB69FFDEF37360E7B52F14482E8959469BB81AB3718EA3E2333D448FBA2E5FD71D572622C36DA3766BB7A7E44AC03DBDC70BE6454675667304F9BFD28D616F6381A007F5E0EC2F96C289AD8B"
#define RSA_Q_2048			"C479455CA4E23F0450CDF4FCA2ADC092D5D02641BE74FB85D398F1B5D55EB2320C83D0F5EAE8E7E79AA9039A060D01C40C164E176B83420ECB3FF70354F1E0A951B9C3558C07D80AFE221A1CD2617425481271A25534FC9988907C54277D9024F20A55667C1E2FCE124574F904CA30E65D71EF08AD0E59F68E30D5D35AA18BBB"
#define RSA_DP_2048			"8E1093102FF805842CEE4DC42681DE1B120FB003125A1E3CBDC8DE30B5B019BC73770423A868AD3086E32C8CBB5B458958F812029246AA94A24CEB452374B8301F063B8467D011CCF65F17EC2228D85FD1743FE4BE3A196C824917A447CFC542DC8029292F5D442E2EF8EEF758A67FE1B3964A42566AAFF9409D750F2C5BC907"
#define RSA_DQ_2048			"82FB83931896D4AD8B33F8A86C73D5B739356ED67EF8A7AE8D10A123E39476CC0857E0A3F1F09A9A671B57BC0408ABD8080EDEBA47ACD6B4877FFA0238A14070E126823908053AB1FEC166BDE1964D6E300C4BC18E23531105B5A8381A53B56DF6B18E4452BECA896183A350ADDC209993A14A05C8B43BF9B4208E8CE7165D27"
#define RSA_QP_2048			"5677636C9BD10D3E051BB5EA45FE17CC4081446130DC1D28A785F2DA12DF1558AE05E4445141C3FBAEDCCA1944DC70B66C248DFE836E1751E745888E6550D4BF5E227BE794E6BD4CB6339392BBF4650D2C847E935E684068D00FC69587054689DC340E3590DB85C43454DDBBD4A8C5333DA566DBFDAF6BD95A413177148639E9"
#define RSA_N_C_2048			"8EDD73DAE686DB8FE8F8CC0A76210CC4C0B5B508E6E32EE7A48C61FA10E24CB40E0494680916A2C9C69F4534615CF05FAF28BBE3EFD1B221B56004FF24875AAD8221F7BDD932CAB0632882230426BBC7E7F2AB79541680144DEBA3F58140572490DF1449410B7D321D0D629F445D16F6482A6F5CC322F35F93A29181E059C310895EA249225DFF8436959913CBF4B03C15838C3AB4B653DD4789F6BB7D5C051865A491B9F4BC3762C71860D5A604C451FC877D0EE32D012E2A44B5A5B6F2BD86F022A4A2805BAD57DC686BC14ADD50B8C5FE79FDF1DCFBABFE74D5085D02D047A0CFA1565AE407DD5359060E790D766B5F1E731C98AF7946E311DE835A282171"
#define RSA_P_C_2048			"92514EC628D533D5DC57D76523E5548D29D709BAAE15DFA5DA55F2C24B8469BEFBA2E6BDD1AE26EA6CDF21A3DF0AC45002263BBCEDF174ED804C0614EB6C4AB52DF1FACF2F12E641D81F4DD6F68F1A2812F9EA78C3FDBEFA7BA81BC2B49A70E5F7C0162B7B8FF9289CBA50572CFC9BB7199568B2BD4C20D990BE101B0EDFFC9B"
#define RSA_Q_C_2048			"5C3DC95D6D25CDB77EE901201E70D3C231DD043036206743D0CAB1E59EB60E4E29FA187E0518D89EEFF9792094FD7500979E57A5F002DB3DC85006554F35F376145DAD69F75108B67E8771744459870C89F719D6348D1E1BD26309DC904F30FF855DD68E9128FC7CFE5B957B09E0AAF593AF3391E4E89AAEA9EDE53FDAB73678"
#define RSA_N_Q_2048			"E0E9B747"
#define RSA_P_Q_2048			"6AE36DDD"
#define RSA_Q_Q_2048			"F2C07E8D"
#define CIPHER_2048			"1A489B964931E96ECBED9DB0A357B0912C203B3020BA1427353AD06CAD229A77F9E864A11BF0AD6A4FE2CB88D3C0F4AE0B964C6122DA581E2E509D9B7828E41BDDC6A179F43EF2B1DAB09C4D49ACCE3CD55FBFA256E264189C365A2E8BE9640B98A73267E9A5ACBD9C93838AD76ED57DCD8C680F248CAC97CDA034CC348A5754EBFAAA3A855FF618CE6BCA51BCBAC33D105D0F6BF58FE1C0E7F8DF4CB293D48A4EC5FEA617DFA00F1E6688893FCF5DC7B45DE3C511E14DF2D6D072511664BF397D322DD61952882CE88E8F1D770B14340B52AAA2D2A388BF2A1F6A5D7A13EF149FF9AEDFE0A2657DCB9568A06FBFFB800D1CFB7CA625630D8DA39174008F93F9"


static void rsa_read_key(mh_rsa_private_key* key, uint32_t key_size)
{
	if (key_size == 1024)
	{
		key->bytes = 1024 / 8;
		bn_read_string_to_bytes(key->n, key->bytes, RSA_N_1024);
		bn_read_string_to_bytes(key->e, 4, RSA_E_1024);
		bn_read_string_to_bytes(key->d, key->bytes, RSA_D_1024);
		bn_read_string_to_bytes(key->p, key->bytes/2, RSA_P_1024);
		bn_read_string_to_bytes(key->q, key->bytes/2, RSA_Q_1024);
		bn_read_string_to_bytes(key->dp, key->bytes/2, RSA_DP_1024);
		bn_read_string_to_bytes(key->dq, key->bytes/2, RSA_DQ_1024);
		bn_read_string_to_bytes(key->qp, key->bytes/2, RSA_QP_1024);
		bn_read_string_to_bytes(key->n_c, key->bytes, RSA_N_C_1024);
		bn_read_string_to_bytes(key->p_c, key->bytes/2, RSA_P_C_1024);
		bn_read_string_to_bytes(key->q_c, key->bytes/2, RSA_Q_C_1024);
		bn_read_string_to_bytes(key->n_q, 4, RSA_N_Q_1024);
		bn_read_string_to_bytes(key->p_q, 4, RSA_P_Q_1024);
		bn_read_string_to_bytes(key->q_q, 4, RSA_Q_Q_1024);
	}
	if (key_size == 2048)
	{
		key->bytes = 2048 / 8;
		bn_read_string_to_bytes(key->n, key->bytes, RSA_N_2048);
		bn_read_string_to_bytes(key->e, 4, RSA_E_2048);
		bn_read_string_to_bytes(key->d, key->bytes, RSA_D_2048);
		bn_read_string_to_bytes(key->p, key->bytes/2, RSA_P_2048);
		bn_read_string_to_bytes(key->q, key->bytes/2, RSA_Q_2048);
		bn_read_string_to_bytes(key->dp, key->bytes/2, RSA_DP_2048);
		bn_read_string_to_bytes(key->dq, key->bytes/2, RSA_DQ_2048);
		bn_read_string_to_bytes(key->qp, key->bytes/2, RSA_QP_2048);
		bn_read_string_to_bytes(key->n_c, key->bytes, RSA_N_C_2048);
		bn_read_string_to_bytes(key->p_c, key->bytes/2, RSA_P_C_2048);
		bn_read_string_to_bytes(key->q_c, key->bytes/2, RSA_Q_C_2048);
		bn_read_string_to_bytes(key->n_q, 4, RSA_N_Q_2048);
		bn_read_string_to_bytes(key->p_q, 4, RSA_P_Q_2048);
		bn_read_string_to_bytes(key->q_q, 4, RSA_Q_Q_2048);
	}
}

#define DBG_RSA_TIME		0

void mh_rsa_time_test(void)
{
	uint32_t i, j, t[3], time[3];
	mh_rsa_private_key rsa_pri_key;
	mh_rsa_public_key rsa_pub_key;
	uint32_t key_size[2] = {1024, 2048};

	uint8_t plain0[256];
	uint8_t plain1[256];
 	uint8_t cipher[256];
#if DBG_RSA_TIME
	uint32_t _plain0[64];
	uint32_t _plain1[64];
	uint32_t _cipher[64];
#endif

	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);
	
	for (i = 0; i < 2; i++)
	{
		DBG_PRINT("\n%d rsa key time test\n", key_size[i]);

		memset(&rsa_pri_key, 0, sizeof(rsa_pri_key));
		memset(&rsa_pub_key, 0, sizeof(rsa_pub_key));
		
		rsa_read_key(&rsa_pri_key, key_size[i]);
		rsa_pub_key.bytes = rsa_pri_key.bytes;
		memcpy(rsa_pub_key.e, rsa_pri_key.e, sizeof(rsa_pri_key.e));
		memcpy(rsa_pub_key.n, rsa_pri_key.n, sizeof(rsa_pri_key.n));
		memcpy(rsa_pub_key.n_c, rsa_pri_key.n_c, sizeof(rsa_pri_key.n_c));
		memcpy(rsa_pub_key.n_q, rsa_pri_key.n_q, sizeof(rsa_pri_key.n_q));


		timer_init_ms(1);

		memset(time, 0, sizeof(time));
		for (j = 0; j < RSA_TIME_TEST_COUNT; j++)
		{
			memset(plain0, 0, sizeof(plain0));
			mh_rand(plain0, key_size[i] / 8);
			// 0 < plain0 < key.n
			plain0[0] = 0;

			//encrypt time count
			memset(cipher, 0, sizeof(cipher));

			timer_init_ms(1);
			timer_start(0);
			mh_rsa_public(cipher, plain0, &rsa_pub_key, mh_rand_p, NULL);
// 			t[0] = timer_tick(0);
			t[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			t[0] += timer_tick(0) * 1000;
			time[0] += t[0];

			//crt decrypt time count
			memset(plain1, 0, sizeof(plain1));
			timer_init_ms(1);
			timer_start(0);
			mh_rsa_private_crt(plain1, cipher, &rsa_pri_key, mh_rand_p, NULL);
// 			t[1] = timer_tick(0);
			t[1] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			t[1] += timer_tick(0) * 1000;
			
			time[1] += t[1];
			
			if (memcmp(plain1, plain0, sizeof(plain0)))
			{
				DBG_PRINT("mh_rsa_private_crt test err\n");
				while(1);
			}


//#if DBG_RSA_TIME
//			memswap(_plain0, sizeof(_plain0), plain0, rsa_pri_key.bytes);
//			memswap(_cipher, sizeof(_cipher), cipher, rsa_pri_key.bytes);
//			bn_printf(_plain0, bn_size(_plain0));printf("\n");
//			bn_printf(_cipher, bn_size(_cipher));printf("\n");
//#endif
//			//decrypt time count
			memset(plain1, 0, sizeof(plain1));
			timer_init_ms(1);
			timer_start(0);
			mh_rsa_private(plain1, cipher, &rsa_pri_key, mh_rand_p, NULL);
 			
			t[2] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			t[2] += timer_tick(0) * 1000;
			time[2] += t[2];
			if (memcmp(plain1, plain0, rsa_pri_key.bytes))
			{	
//#if DBG_RSA_TIME
//				memswap(_plain1, sizeof(_plain1), plain1, rsa_pri_key.bytes);
//				bn_printf(_plain1, bn_size(_plain1));printf("\n");
//#endif
//				printf("mh_rsa_private test err\n");
				while(1);
			}

// 			printf("%3dth: pub:%6dms    pri_crt:%6dms    pri:%6dms\n", j, t[0], t[1], t[2]);
			DBG_PRINT("%3dth: pub:%8dus    pri_crt:%8dus    pri:%8dus\n", j, t[0], t[1], t[2]);
		}

// 		printf("\nsum time: pub:%8dms    pri_crt:%8dms    pri:%8dms\n\n", time[0], time[1], time[2]);
		DBG_PRINT("\nsum time: pub:%10dus    pri_crt:%10dus    pri:%10dus\n\n", time[0], time[1], time[2]);
	}
}



void mh_rsa_gen_key_time_test(void)
{
	uint32_t i, j, t, time[4] = {0,0,0,0};
	mh_rsa_private_key rsa_pri_key;
	uint32_t key_type[4][2] = {{0x03, 1024}, {0x03, 2048}, {0x10001, 1024}, {0x10001, 2048}};

    SYSCTRL_ClocksTypeDef clocks;
    SYSCTRL_GetClocksFreq(&clocks);
    
    for(i = 0; i < 4; i++)
    {
        DBG_PRINT("\ne = %d bits = %d rsa key time test\n", key_type[i][0], key_type[i][1]);
        for (j = 0; j < RSA_GEN_KEY_TIME_TEST_COUNT; j++)
        {
            memset(&rsa_pri_key, 0, sizeof(rsa_pri_key));
            timer_init_ms(1);
            timer_start(0);
            mh_rsa_efc_gen_key(&rsa_pri_key, key_type[i][0], key_type[i][1],
                            mh_rand_p, NULL);
            mh_rsa_complete_key_e(&rsa_pri_key, mh_rand_p, NULL);
			t = timer_counter() / (clocks.PCLK_Frequency / 1000000);
			t += timer_tick(0) * 1000;
//			t = timer_tick(0);
            time[i] += t;
            DBG_PRINT("\ne = %d bits = %d rsa key time %8dms", key_type[i][0], key_type[i][1], t);
        }
        DBG_PRINT("\ne = %d bits = %d %dsum rsa key time %8dms", key_type[i][0], key_type[i][1],RSA_GEN_KEY_TIME_TEST_COUNT, time[i]);
        DBG_PRINT("\ne = %d bits = %d %dave rsa key time %8dms\n", key_type[i][0], key_type[i][1],RSA_GEN_KEY_TIME_TEST_COUNT, time[i] / RSA_GEN_KEY_TIME_TEST_COUNT);
    }


}



#define KEY_WORDS 256 / 32


#define cp		"8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3"
#define ca		"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498"
#define cb		"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A"
#define cGx		"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
#define cGy		"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
#define cn		"8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7"

#define crand	"4C62EEFD6ECFC2B95B92FD6C3D9575148AFA17425546D49018E5388D49DD7B4F"

#define ckeyd	"1649AB77A00637BD5E2EFE283FBF353534AA7F7CB89463F208DDBC2920BB0DA0"
#define cex		"435B39CCA8F3B508C1488AFC67BE491A0F7BA07E581A0E4849A5CF70628A7E0A"
#define cey		"75DDBA78F15FEECB4C7895E2C1CDF5FE01DEBB2CDBADF45399CCF77BBA076A42"

void mh_sm2_time_test_read_para(mh_sm2_ellipse_para *para)
{
	bn_read_string_to_bytes(para->p, MH_SM2_KEY_BYTES, cp);
	bn_read_string_to_bytes(para->a, MH_SM2_KEY_BYTES, ca);
	bn_read_string_to_bytes(para->b, MH_SM2_KEY_BYTES, cb);
	bn_read_string_to_bytes(para->n, MH_SM2_KEY_BYTES, cn);
	bn_read_string_to_bytes(para->g.x, MH_SM2_KEY_BYTES, cGx);
	bn_read_string_to_bytes(para->g.y, MH_SM2_KEY_BYTES, cGy);
}
void mh_sm2_time_test_read_key(mh_sm2_private_key *key)
{
	bn_read_string_to_bytes(key->d, MH_SM2_KEY_BYTES, ckeyd);
	bn_read_string_to_bytes(key->e.x, MH_SM2_KEY_BYTES, cex);
	bn_read_string_to_bytes(key->e.y, MH_SM2_KEY_BYTES, cey);
}


void mh_sm2_time_test(void)
{
	mh_sm2_ellipse_para para;
	mh_sm2_private_key key;
	uint32_t i, t, time[2]={0,0};

	unsigned char plain0[SM2_TIME_TEST_DATA];
	unsigned char plain1[SM2_TIME_TEST_DATA];
	unsigned char cipher[SM2_TIME_TEST_DATA + 65 + 32];
	
	uint32_t mlen;
	uint32_t clen;
	uint8_t rand[32];
	
	mh_sm2_time_test_read_para(&para);
	mh_sm2_time_test_read_key(&key);
	
	DBG_PRINT("\n%d bytes data size\n", SM2_TIME_TEST_DATA);
	
	timer_init_ms(1);

	memset(time, 0, sizeof(time));
	for (i = 0; i < SM2_TIME_TEST_COUNT; i++)
	{
		mh_rand_p(rand, sizeof(rand),NULL);

		memset(plain0, 0, sizeof(plain0));
		mh_rand(plain0, sizeof(plain0));
		memset(plain1, 0, sizeof(plain1));
		memset(cipher, 0, sizeof(cipher));

		timer_start(0);
		t = mh_sm2_enc(cipher, &clen, plain0, sizeof(plain0), &key.e, &para, mh_rand_p, NULL);
		time[0] += timer_tick(0);

		if(MH_RET_SM2_ENC_SUCCESS == t)
		{
			timer_start(0);
			mh_sm2_dec(plain1, &mlen, cipher, clen, &key, &para, mh_rand_p, NULL);
			time[1] += timer_tick(0);
			if(0 == memcmp(plain1, plain0, sizeof(plain0)))
			{
//				printf("sm2 encrypt decrypt pass\n");
			}
			else
			{
				DBG_PRINT("sm2 encrypt decrypt err\n");
			}
		}
		else
			DBG_PRINT("encrypt err\n");
	}

	DBG_PRINT("%dms sm2 encrypt time\n", time[0]);
	DBG_PRINT("%dms sm2 decrypt time\n", time[1]);
	
}

#define cZa		"F4A38489E32B45B6F876E3AC2168CA392362DC8F23459C1D1146FC3DBFB7BC9A"

void mh_sm2_sign_time_test(void)
{
	uint32_t t, i, time[2];
	mh_sm2_ellipse_para para;
	mh_sm2_private_key key;
	mh_sm2_sign sign;
	uint32_t t1 = 0;

	uint8_t msg[SM2_TIME_TEST_SIGN_DATA];

	uint8_t Za[32];
	uint8_t rand[32];
	uint32_t e[8] = {0};

	bn_read_string_to_bytes(Za, sizeof(Za), cZa);
	
	mh_sm2_time_test_read_para(&para);
	mh_sm2_time_test_read_key(&key);
	
	DBG_PRINT("\n%d bytes data size\n", SM2_TIME_TEST_SIGN_DATA);
	
	timer_init_ms(1);

	memset(time, 0, sizeof(time));
	for (i = 0; i < SM2_TIME_TEST_SIGN_COUNT; i++)
	{
		f_rng(rand, sizeof(rand), NULL);
		f_rng(msg, sizeof(msg), NULL);

		memset(&sign, 0, sizeof(sign));
		timer_start(0);
		mh_sm2_hash_e(e, Za, msg, sizeof(msg));
	//	t1 = mh_sm2_digital_sign(&sign, Za, msg, sizeof(msg), &key, &para, mh_rand_p, NULL);
		t1 = mh_sm2_digital_sign_with_e(&sign,e, msg, sizeof(msg), &key, &para, mh_rand_p, NULL);
		time[0] += timer_tick(0);
		
//		printf("\r\n The Sign Ret value is %08x",t1);

		timer_start(0);
		mh_sm2_hash_e(e, Za, msg, sizeof(msg));
		t = mh_sm2_verify_sign_with_e(&sign, e, msg, sizeof(msg), &key.e ,&para, mh_rand_p, NULL);
//		t = mh_sm2_verify_sign(&sign, Za, msg, sizeof(msg), &key.e ,&para, mh_rand_p, NULL);
		time[1] += timer_tick(0);
	 
//		printf("\r\n The verify_sign Ret value is %08x",MH_RET_SM2_VERIFY_FAILURE);
//		printf("\r\n The verify_sign Ret value is %08x",MH_RET_SM2_VERIFY_SUCCESS	);
//		printf("\r\n The verify_sign Ret value is %08x",t);
		if(MH_RET_SM2_VERIFY_SUCCESS == t)
		{
			DBG_PRINT("\r\nSign test pass\n");
		}
		else
		{
			DBG_PRINT("Sign test err\n");
		}
	}

	DBG_PRINT("%dms sm2 sign time\n", time[0]);
	DBG_PRINT("%dms sm2 verify time\n", time[1]);
	
}



void mh_sm3_time_test(void)
{
	uint32_t time;
	
	uint8_t plain[SM3_TIME_TEST_DATA];
	uint8_t cipher[32];
	
	SYSCTRL_ClocksTypeDef clocks;
	SYSCTRL_GetClocksFreq(&clocks);


	memset(plain, 0, sizeof(plain));
	mh_rand(plain, sizeof(plain));

	DBG_PRINT("\n%d bytes data size\n", SM3_TIME_TEST_DATA);

	timer_init_ms(1);
	memset(cipher, 0, sizeof(cipher));
	memset(&time, 0, sizeof(time));
	timer_start(0);
	mh_sm3(cipher, plain, sizeof(plain));
	time = timer_counter() / (clocks.PCLK_Frequency / 1000000);
	time += timer_tick(0) * 1000;
	{
		DBG_PRINT("%dus time SM3 Test\n", time);
	}
}


void mh_sm4_test1()
{
	uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
	uint8_t iv[16] = {0xA3, 0xB1, 0xBA, 0xC6, 0x56, 0xAA, 0x33, 0x50, 0x67, 0x7D, 0x91, 0x97, 0xB2, 0x70, 0x22, 0xDC};

	uint8_t MyPlain[1024] = "0123456789";
	uint8_t MyCipher[1024];
	uint8_t tmp[1024];
	uint32_t MyLenth;
	uint32_t i;
	for(i=0;i<1024;i++)
	{
		MyPlain[i] = 0x30+ (i%10);
	}

    mh_sm4_enc(ECB, MyCipher, sizeof(MyCipher), MyPlain, sizeof(MyPlain), key, iv, mh_rand_p, NULL);
	mh_sm4_dec(ECB, tmp, sizeof(MyCipher), MyCipher, sizeof(MyCipher), key, iv, mh_rand_p, NULL);
	
}
void mh_sm4_time_test(void)
{
	uint32_t i, j, time[2];
	mh_pack_mode_def modes[2] = { ECB, CBC};
	char *cmodes[2] = {"ECB", "CBC"};
	
	uint8_t plain[2][SM4_TIME_TEST_DATA];
	uint8_t cipher[SM4_TIME_TEST_DATA];


    uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t iv[16] = {0xA3, 0xB1, 0xBA, 0xC6, 0x56, 0xAA, 0x33, 0x50, 0x67, 0x7D, 0x91, 0x97, 0xB2, 0x70, 0x22, 0xDC};

	uint8_t MyPlain[64] = "1234567890123456789012345678901234567890123456789012345678901234";
	uint8_t MyCipher[64];
	uint8_t tmp[64];
	uint32_t MyLenth;
	uint32_t k;
    SYSCTRL_ClocksTypeDef clocks;
    SYSCTRL_GetClocksFreq(&clocks);

	memset(plain, 0, sizeof(plain));
	mh_rand(plain[0], sizeof(plain[0]));

	DBG_PRINT("\n%d bytes data size\n", SM4_TIME_TEST_DATA);

    //pack mode ECB CBC
    for (i = 0; i < 2; i++)
    {
        memset(time, 0, sizeof(time));
        for (j = 0; j < SM4_TIME_TEST_COUNT; j++)
        {
            memset(cipher, 0, sizeof(cipher));
            memset(plain[1], 0, sizeof(plain[1]));
            
            timer_init_ms(1);
            timer_start(0);
            mh_sm4_enc(modes[i], cipher, sizeof(cipher), plain[0], sizeof(plain[0]), key, iv, mh_rand_p, NULL);
            time[0] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
            time[0] += timer_tick(0) * 1000;
            
            timer_init_ms(1);
            timer_start(0);
            mh_sm4_dec(modes[i], plain[1], sizeof(plain[1]), cipher, sizeof(cipher), key, iv, mh_rand_p, NULL);
            time[1] = timer_counter() / (clocks.PCLK_Frequency / 1000000);
            time[1] += timer_tick(0) * 1000;
			for(k=0;k<sizeof(plain[0]);k++)
			{
				if(plain[0][k] != plain[1][k])
				{
					break;
				}
			}
            if(!memcmp(plain[0], plain[1], sizeof(plain[0])))
            {
//              DBG_PRINT("SM4 %s Test Pass...\n", cmodes[i]);
            }
            else
            {
                DBG_PRINT("SM4 %s Test Fail...\n", cmodes[i]);
            }
        }

		DBG_PRINT("%dus SM4 %s encrypt time\n", time[0], cmodes[i]);
		DBG_PRINT("%dus SM4 %s decrypt time\n", time[1], cmodes[i]);
	}
}


void mh_mem_time_test(void)
{
	uint32_t j, time[2];
	uint8_t data[2][MEM_TIME_TEST_DATA];

	timer_init_ms(1);

	memset(data, 0, sizeof(data));
	mh_rand(data[0], sizeof(data[0]));

	DBG_PRINT("\n%d bytes data size\n", MEM_TIME_TEST_DATA);

	memset(time, 0, sizeof(time));
	for (j = 0; j < MEM_TIME_TEST_COUNT; j++)
	{
		memset(data, 0, sizeof(data));
		mh_dbg_time_begin();
		mh_memcpy(data[1], data[0], sizeof(data[0]), f_rng, NULL);
		time[0] += mh_dbg_time_get();
		
		memset(data, 0, sizeof(data));
		mh_dbg_time_begin();
		memcpy(data[1], data[0], sizeof(data[0]));
		time[1] += mh_dbg_time_get();
	}
	DBG_PRINT("%dtick mh_memcpy time\n", time[0]);
	DBG_PRINT("%dtick memcpy time\n", time[1]);
}




