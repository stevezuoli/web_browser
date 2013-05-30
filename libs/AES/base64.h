#ifndef _base64_h_
#define _base64_h_

#ifdef __cplusplus
extern "C" {
#endif

/* 
    Public Base64 conversion tables
*/ 
#define DATA_LOCK  0xdeadbeef
#define SIZEM(array) ( sizeof(array)/sizeof(array[0]) )

/* 
    Initializes the base64->base16 conversion tab. 
    Call this function once when your program starts
    and always after your B64 table has been changed.
*/
extern void initb64(void);
extern void initb64_custom(void* b64table);

/*
   int b64toh(lpBase64String, lpDestinationBuffer);
   Converts base64 string b to hexnumber d.  Returns size of hexnumber in bytes.
*/
extern unsigned int b64toh(void *b64src, void *dest);


/*
   int htob64(lpHexNumber, lpDestinationBuffer);
   Converts hexnumber h (with length l bytes) to base64 string d.
   Returns length of base64 string.
*/
extern unsigned int htob64(void *hascii, void *dest, unsigned int l);

//  - Used Base36 conversion table (supported by MIRACL):
//    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
//  - Used Base60 conversion table (supported by MIRACL):
//    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx
//  - Used Base64 conversion table:
//    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

extern void initb60(void);
extern void initb60_custom(void* b60table);

// check base 2 - base64 valid digit
extern int check_digit(void *src, int base);

// lena n -> 2n lenh
// ascii to hex,  such as: 0 -> 30  1->31, A->41 
// "ABCJKL" -> "4142434A4B4C"  (0x41 0x42 0x43 0x4A 0x4B 0x4C -> 0x3431 0x3432 0x3433 0x3441 0x3442 0x3443)
// zero_pad , add '\0' to tail 
// "ABCJKL" -> "4142434A4B4C" , 0123456789ABCDEF
extern unsigned int atoh(void* tasc, void* thex, unsigned int lena, int zero_pad);
// small letter version (lowercase)
// "ABCJKL" -> "4142434a4b4c" , 0123456789abcdef
extern unsigned int atoh_small(void* tasc, void* thex, unsigned int lena, int zero_pad);

// hex to ascii
// 2n -> n
// hex to ascii: "41" -> "A" (0x3631->0x41)  "414243"->"ABC"  "0536" (30353336) -> 0x0536
extern unsigned int htoa(void* tasc, void* thex,unsigned int lenh, int zero_pad);

// calc base 256 -> base2 bits
extern unsigned int calc_base256_bits(const void* src, unsigned int len);

// calc base 16 -> base2 bits
extern unsigned int calc_base16_bits(const void* src, unsigned int len);

// reverse hex_strings  [1234 -> 3421,  12345 -> 452301]
extern void reverse_hex(void* dest, void* src,unsigned int lenth, int zero_pad);

// fast unsigned int 32 powmod, use LTR (Left-To-Right) method */
extern unsigned int powmod32(unsigned int x, unsigned int e, unsigned int n);

// crc32/ crc16 funs
#define CRC32_NEW   (0xFFFFFFFFUL) 
#define CRC32_POLY  (0xEDB88320UL)
#define CRC16_NEW   (unsigned short)(0xFFFF) 
#define CRC16_POLY  (unsigned short)(0x8408) 
#define CRC16_STD_POLY  (unsigned short)(0xA001) 

extern void init_crc_all_tab(void);

extern void init_crc32_table(unsigned int ploy);
extern unsigned int crc32_update(void *buf, unsigned int lenth, unsigned int crc);
extern unsigned int crc32_pad(void *buf, unsigned int lenth, unsigned int crc);
extern unsigned int crc32_full(void *buf, unsigned int lenth);

extern void init_crc32_rf_table(void);
extern unsigned int iCRC32(unsigned int ABCD,unsigned int abcd);
extern unsigned int rCRC32(unsigned int WXYZ,unsigned int ABCD);

extern void init_crc16_table(unsigned short ploy);
extern unsigned short crc16_update(void *buf, unsigned int lenth, unsigned short crc);
extern unsigned short crc16_pad(void *buf, unsigned int lenth, unsigned short crc);
extern unsigned short crc16_full(void *buf, unsigned int lenth);

extern void init_crc16_rf_table(void);
extern unsigned short CRC16(unsigned short AB, unsigned short ab);
extern unsigned short rCRC16(unsigned short XY, unsigned short AB);

#define MYRAND_A  0x31415929
#define MYRAND_B  0x53589793
extern void mysrand(int seed);
extern int myrand(void);

#ifdef __cplusplus
}
#endif

#endif /* #define _base64_h_ */
