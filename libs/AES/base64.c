#include <stdlib.h>
#include <string.h>
#include "base64.h"

typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef unsigned int       DWORD;

#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKEDWORD(a, b)     ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))

#define MakeWord(a,b)    MAKEWORD(b,a)
#define MakeDWord(a,b)   MAKEDWORD(b,a)

/* 
    Public Base64 conversion tables
*/ 
#define XBAD   0xFF

static unsigned char B16tab[16+16]="0123456789ABCDEF";
static unsigned char B16small[16+16]="0123456789abcdef";
static unsigned char B60t[60+4];
static unsigned char B64t[64+16];
static unsigned char b16[256];
static unsigned char b64[256];
static unsigned char b60[256];

static unsigned char init_b16 = 0;
static unsigned char init_b60 = 0;
static unsigned char init_b64 = 0;

#define  notbase2(x)        ((x) < '0' || (x) > '1')
#define  notbase8(x)        ((x) < '0' || (x) > '7') 
#define  notbase10(x)       ((x) < '0' || (x) > '9')
#define  notAF(x)           ((x) < 'A' || (x) > 'F')
#define  notaf(x)           ((x) < 'a' || (x) > 'f')
#define  notAZ(x)           ((x) < 'A' || (x) > 'Z')
#define  notaz(x)           ((x) < 'a' || (x) > 'z')

#define  notbase36(x)       (notbase10(x) && notAZ(x))
#define  notbase16b(x)      (b16[x] & 0xF0 )
#define  notbase16(x)       (b16[x] == XBAD)
#define  notbase60(x)       (b60[x] == XBAD)
#define  notbase64(x)       (b64[x] == XBAD)
#define  notbase64b(x)      (b64[x] & 0xC0 )
#define  aboveb64(v)        (v & 0xC0)
#define  aboveb16(v)        (v & 0xF0)


void initb16(void)
{
    unsigned int i;
    for (i=0; i<256; i++) 
        b16[i]= XBAD;
    for (i=0; i<16; i++)
        b16[(B16tab[i])]=i;
    for (i=0; i<16; i++)
        b16[(B16small[i])]=i;
    init_b16 = 1;
}

/*
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx"
*/
void initb60_default_tab(unsigned char *tab)
{
    unsigned int i;
    unsigned char c;
    
    c = '0';
    for(i = 0; i < 10; i++, c++) {
        tab[i] = c;
    }
    c = 'A';
    for(i = 10; i < 36; i++, c++) {
        tab[i] = c;
    }
    c = 'a';
    for(i = 36; i < 60; i++, c++) {
        tab[i] = c;
    }
    tab[60] = 0;
    return;
}

void initb60_internal(unsigned char *tab)
{
    unsigned int i;
    for (i=0; i<256; i++) 
        b60[i]= XBAD;
    for (i=0; i<60; i++)
        b60[(tab[i])]=i;
    init_b60 = 1;
}

void initb60(void)
{
    memset(B60t, 0, sizeof(B60t));
    initb60_default_tab(B60t);
    initb60_internal(B60t);
    return;
}

void initb60_custom(void* b60table)
{
    memset(B60t, 0, sizeof(B60t));
    memcpy(B60t, b60table, 60);
    initb60_internal(B60t);
    return;
}

/* 
    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
    void initb64(); 
    Initializes the base64->base16 conversion tab. 
    Call this function once when your program starts
    and always after your B64t table has been changed.
*/

void initb64_default_tab(unsigned char *tab)
{
    unsigned int i;
    unsigned char c;
    
    c = 'A';
    for(i = 0; i < 26; i++, c++) {
        tab[i] = c;
    }
    c = 'a';
    for(i = 26; i < 52; i++, c++) {
        tab[i] = c;
    }
    c = '0';
    for(i = 52; i < 62; i++, c++) {
        tab[i] = c;
    }
    tab[62] = '+';
    tab[63] = '/';
    tab[64] = 0;
    return;
}

void initb64_internal(unsigned char* tab)
{
    unsigned int i;
    for (i=0; i<256; i++) 
        b64[i]= XBAD;
    for (i=0; i<64; i++)
        b64[(tab[i])]=i;    
    init_b64 = 1;
}

void initb64(void)
{
    memset(B64t, 0, sizeof(B64t));
    initb64_default_tab(B64t);
    initb64_internal(B64t);
    return;
}

void initb64_custom(void* b64table)
{
    memset(B64t, 0, sizeof(B64t));
    memcpy(B64t, b64table, 64);
    initb64_internal(B64t);
    return;
}

/*
   unsigned int b64toh(lpBase64String, lpDestinationBuffer);
   Converts base64 string b to hex number d (base256).
   Returns size of hex number in bytes.
*/
unsigned int b64toh(void *b64src, void *dest){
    unsigned char *b = (unsigned char *)b64src;
    unsigned char *d = (unsigned char *)dest;
    register unsigned char ch, cc;
    
    if(!init_b64) {
        initb64();
    }  
    
    /* every 3 bytes ascii (3 * 8-bit), from 4 bytes b64 (4 * 6-bit) */
    while (1) {        
        ch = b64[*b];
        cc = ch << 2;
        if(aboveb64(ch)) 
            break;
        b++;        
        ch = b64[*b];
        cc |= (ch >> 4);
        if(aboveb64(ch)) 
            break;
        *d++ = cc;
        
        cc = ch << 4;
        b++;
        ch = b64[*b];
        cc |= (ch >> 2);
        if(aboveb64(ch)) 
            break;
        *d++ = cc;
        
        cc = (ch<<6);
        b++;
        ch = b64[*b];
        cc |= ch;
        if(aboveb64(ch)) 
            break;
        *d++ = cc;
        b++;
    }
    
    *d = 0x0;
    return (unsigned int)d - (unsigned int)dest;
}

/*
   unsigned int htob64(lpHexNumber, lpDestinationBuffer);
   Converts base256 h (with length l bytes) to base64 string d.
   Returns length of base64 string.
*/
/* raw bytes in quasi-big-endian order to base 64 string (NUL-terminated) */
// 3 * 8 =24 bits: AA-BB-CC   => 4 * 6: aa-bb-cc-dd, 8-8-8 => 6-(2-4)-(4-2)-6
unsigned int htob64(void *hascii, void *dest, unsigned int l)
{
    unsigned char *h = (unsigned char *)hascii;
    unsigned char *d = (unsigned char *)dest;
    register int len;

    if(l == 0)
        return 0;
    len = (int)l - 3;
    while(len >= 0) { /* optimized for Intel little-endian 32 bit */ 
        d[0] = B64t[h[0] >> 2];
        d[1] = B64t[((h[0] << 4) & 0x30) | (h[1] >> 4)];
        d[2] = B64t[((h[1] << 2) & 0x3c) | (h[2] >> 6)];
        d[3] = B64t[h[2] & 0x3f];
        d += 4;
        h += 3;
        len -= 3;        
    }

    len += 3;
    /* so, here, len < 3,  len = 0, 1 , 2 */
    if (len > 0)
    {  
        *d++ = B64t[h[0] >> 2];
        if(len == 1){   
            *d++ = B64t[(h[0] << 4) & 0x30]; 
        }else if(len == 2) {
            *d++ = B64t[((h[0] << 4) & 0x30) | (h[1] >> 4)];
            *d++ = B64t[(h[1] << 2) & 0x3c];
        }
    }

    *d = 0;
    return ((unsigned int)d - (unsigned int)dest);
}


//  - Used Base36 conversion table (supported by MIRACL):
//    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
//  - Used Base60 conversion table (supported by MIRACL):
//    0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx
//  - Used Base64 conversion table:
//    ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

// check base 2 - base64 valid digit
int check_digit(void *src, int base)
{
    unsigned char *s = (unsigned char *)src;
    unsigned char *d = (unsigned char *)src;
    
    if(*s == '-'){
        s++;
        d++;
    }
    
    switch(base) {
    case 2:
        while (*s != 0){
            if (notbase2(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;

    case 8:
        while (*s != 0){
            if (notbase8(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;

    case 10:
        while (*s != 0){
            if (notbase10(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;
    case 16:
        if(!init_b16) {
            initb16();
        }  
        while (*s != 0){
            if (notbase16(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;
    case 36:    /* 0123456789ABCD..XYZ */  
        while (*s != 0){
            if (notbase36(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;  
    case 60:  
        if(!init_b60) {
            initb60();
        } 
        while (*s != 0){
            if (notbase60(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }
        break;
    case 64:  
        if(!init_b64) {
            initb64();
        }  
        while (*s != 0){
            if (notbase64(*s)) {
                s++;
                continue;
            }
            *d++ = *s++;
        }   
        break;
    default:
        break;
    }

    *d = 0;
    return ((unsigned int)d - (unsigned int)src);
}



// lenta n -> 2n lenh
// ascii to hex,  such as: 0 -> 30  1->31, A->41 
// "ABC" -> "414243"  (0x41 0x42 0x43 -> 0x3431 0x3432 0x3433)
// zero_pad , add '\0' to tail 
unsigned int atoh(void* tasc, void* thex, unsigned int lena, int zero_pad)
{ 
    unsigned char *p = (unsigned char *)tasc;
    unsigned char *q = (unsigned char *)thex;
    register unsigned char ch;
    register unsigned int  len;

    len = lena;
    while(len--) {
        ch = *p++;
        *q++ = B16tab[ch >> 4];        // high 4 bit
        *q++ = B16tab[ch & 0xF];       // low 4 bit
    }

    if(zero_pad == 0)
        *q = 0;
    len = (unsigned int)q - (unsigned int)thex;
    return len;
}

unsigned int atoh_small(void* tasc, void* thex, unsigned int lena, int zero_pad)
{ 
    unsigned char *p = (unsigned char *)tasc;
    unsigned char *q = (unsigned char *)thex;
    register unsigned char ch;
    register unsigned int  len;
    
    len = lena;
    while(len--) {
        ch = *p++;
        *q++ = B16small[ch >> 4];        // high 4 bit
        *q++ = B16small[ch & 0xF];       // low 4 bit
    }
    
    if(zero_pad == 0)
        *q = 0;
    len = (unsigned int)q - (unsigned int)thex;
    return len;
}


// hex to ascii
// 2n -> n
// hex to ascii: "41" -> "A" (0x3631->0x41)  "414243"->"ABC"  "0536" (30353336) -> 0x0536
unsigned int htoa(void* tasc, void* thex, unsigned int lenh, int zero_pad)
{ 
    unsigned char *p = (unsigned char *)tasc;
    unsigned char *q = (unsigned char *)thex;
    register unsigned int len;
    register unsigned char ch, cl; /* high, low 4 bits */

    if(!init_b16) {
        initb16();
    }  
    if(lenh == 0)
        return 0;
    // if it is not 2n bytes
    if (lenh & 0x1) {
        ch = b16[*q++];
        if(aboveb16(ch)) 
            return 0;
        *p++ = ch;
    }

    len = lenh >> 1;
    while(len--) {
        ch = b16[*q++]; 
        cl = b16[*q++]; 
        *p++ = (ch << 4) | cl;
        ch |= cl;
        if(aboveb16(ch)) {  /* this is the same as, but fast than: (ch | 0xF0) || (cl | 0xF0) */
            p--;
            break;
        }
    } 
    if (zero_pad == 0)
        *p = 0;
    len = (unsigned int)p - (unsigned int)tasc;
    return len;
}


/* calc base 256 ascii-string  bits */
unsigned int calc_base256_bits(const void* src, unsigned int lenth)
{
    unsigned char *p = (unsigned char *)src;
    unsigned char *pend;
    unsigned int j,k; 
    unsigned char ch;

    if(lenth == 0)
        return 0;

    lenth -= 1;
    pend = p+lenth;
    j = 0;
    while(*p == 0) {
        if(p == pend) 
            return 0;
        p++;
        j++;
    }

    /* ok, *p is not zero, check it's MSB */
    ch = *p;
    k = 0;
    while(ch != 0){
        ch >>= 1;
        k++;
    }  

    if(j < lenth) {
        k += (lenth-j) * 8;   
    }
    return k;
}

/* calc base16 string  bits */
unsigned int calc_base16_bits(const void* src, unsigned int lenth)
{
    unsigned char *p = (unsigned char *)src;
    unsigned char *pend;
    unsigned int j,k; 
    register unsigned char ch;
    
    if(lenth == 0)
        return 0;

    lenth -= 1;
    pend = p+lenth;
    j = 0;
    while(*p == '0') {
        if(p == pend) 
            return 0;
        p++;
        j++;
    }
    
    /* ok,  *p is not zero, check it's MSB */
    ch = *p;
    if(!notbase10(ch)) {
        ch -= '0';
    }else if(!notAF(ch)) {
        ch -= ('A' - 10);
    }else {
        ch -= ('a' - 10);
    }

    k = 0;
    while(ch != 0){
        ch >>= 1;
        k++;
    }
    
    if(j < lenth) {
        k += (lenth-j) * 4;   
    }
    return k;
}

//  ABCD -> CDAB   ABC ->BC0A
void reverse_hex(void* dest, void* src, unsigned int lenth, int zero_pad)
{
    unsigned char *sz_dest = (unsigned char *)dest; 
    unsigned char *sz_src = (unsigned char *)src; 
    register int j, k;
    
    // at least 4 bytes can reverse
    if (lenth <= 2 ) {
        sz_dest[0] = sz_src[0];
        sz_dest[1] = sz_src[1];
        sz_dest[2] = 0;
        return; 
    }
    
    j = 0;
    k = lenth - 2;
    while(k >= 0) {
        sz_dest[j] = sz_src[k];
        sz_dest[j+1] = sz_src[k+1];
        j += 2;
        k -= 2;
    }
    if (lenth & 0x1) {   //奇数，没有补全，高位补0, lenth加1 A12 -> 120A
        sz_dest[lenth - 1] = '0';
        sz_dest[lenth] = sz_src[0]; 
        lenth += 1; 
    }
    
    if(zero_pad == 0)
        sz_dest[lenth] = 0;
    return;
}

static unsigned int crc32_tab[256];
static unsigned short crc16_tab[256];
static unsigned char crc32_rf[256];
static unsigned char crc16_rf[256];

void init_crc32_table(unsigned int poly)
{
    unsigned int i,j;
    unsigned int crc;
    for( i = 0; i < 256; i ++ ) {
        crc = i;
        for( j = 0; j < 8; j ++ ){
            if(crc & 1) crc = (crc >> 1) ^ poly;
            else crc >>= 1;
        }
        crc32_tab[i] = crc;
    }
    return;
}

unsigned int crc32_update(void *buf, unsigned int lenth, unsigned int crc_head)
{
    unsigned char *p = (unsigned char *)buf;
    register unsigned int crc = crc_head;    
    while (lenth--) {
        crc = (crc >> 8) ^ crc32_tab[(unsigned char)(crc ^ *p++)];
    }    
    return crc;
}

unsigned int crc32_pad(void *buf, unsigned int lenth, unsigned int crc)
{
    return crc32_update(buf, lenth, crc) ^ CRC32_NEW;
}

unsigned int crc32_full(void *buf, unsigned int lenth)
{
    unsigned char *p = (unsigned char *)buf;
    register unsigned int crc;
    crc = CRC32_NEW;    
    while (lenth--)
        crc = (crc >> 8) ^ crc32_tab[(unsigned char)(crc ^ *p++)];    
    return crc ^ CRC32_NEW;
}

void init_crc16_table(unsigned short poly)
{
    unsigned int i,j;
    unsigned short crc;
    for( i = 0; i < 256; i ++ ) {
        crc = i;
        for( j = 0; j < 8; j ++ ){
            if(crc & 1) crc = (crc >> 1) ^ poly;
            else crc >>= 1;
        }
        crc16_tab[i] = crc;
    }
    return;
}

unsigned short crc16_update(void *buf, unsigned int lenth, unsigned short crc)
{    
    unsigned char *p = (unsigned char *)buf;
    while(lenth--) {
        crc = (crc >> 8) ^ crc16_tab[(unsigned char)(crc ^ *p++)];
    }
    return crc;
}

unsigned short crc16_pad(void *buf, unsigned int lenth, unsigned short crc)
{
    return crc16_update(buf, lenth, crc) ^ CRC16_NEW;
}

unsigned short crc16_full(void *buf, unsigned int lenth)
{
    unsigned char *p = (unsigned char *)buf;
    unsigned int crc;
    crc = CRC16_NEW;    
    while (lenth--) {
        crc = (crc >> 8) ^ crc16_tab[(unsigned char)(crc ^ *p++)];
    }
    return crc ^ CRC16_NEW;
}

void init_crc32_rf_table(void)
{
    unsigned int i,j;
    
    for(i=0; i<256; i++) {
        j = HIBYTE(HIWORD(crc32_tab[i]));
        crc32_rf[j] = i;
    }
    return;
}

/* must init_crc32_rf_table() first */ 
#define RF(x)   crc32_rf[(BYTE) x]
#define F(x)    HIBYTE(HIWORD(crc32_tab[(BYTE) x]))
#define G(x)    LOBYTE(HIWORD(crc32_tab[(BYTE) x]))
#define H(x)    HIBYTE(LOWORD(crc32_tab[(BYTE) x]))
#define I(x)    LOBYTE(LOWORD(crc32_tab[(BYTE) x]))

/*
 given crc_head = ABCD
 return WXYZ crc(abcd)
*/
unsigned int iCRC32(unsigned int ABCD,unsigned int abcd)
{
    BYTE p,o,n,m,a,b,c,d,W,X,Y,Z,A,B,C,D;
    
    A=HIBYTE(HIWORD(ABCD));
    B=LOBYTE(HIWORD(ABCD));
    C=HIBYTE(LOWORD(ABCD));
    D=LOBYTE(LOWORD(ABCD));
    
    a=HIBYTE(HIWORD(abcd));
    b=LOBYTE(HIWORD(abcd));
    c=HIBYTE(LOWORD(abcd));
    d=LOBYTE(LOWORD(abcd));
    
    m=d^D;
    n=c^C^I(m);
    o=b^B^H(m)^I(n);
    p=a^A^G(m)^H(n)^I(o);
    
    W=F(p);
    X=F(o)^G(p);
    Y=F(n)^G(o)^H(p);
    Z=F(m)^G(n)^H(o)^I(p);
    
    return MakeDWord(MakeWord(W,X),MakeWord(Y,Z));
}

/*
given crc_head = ABCD
find abcd;
that crc(abcd) = WXYZ
*/
unsigned int rCRC32(unsigned int WXYZ,unsigned int ABCD)
{
    BYTE p,o,n,m,a,b,c,d,W,X,Y,Z,A,B,C,D;
    
    W=HIBYTE(HIWORD(WXYZ));
    X=LOBYTE(HIWORD(WXYZ));
    Y=HIBYTE(LOWORD(WXYZ));
    Z=LOBYTE(LOWORD(WXYZ));
    
    A=HIBYTE(HIWORD(ABCD));
    B=LOBYTE(HIWORD(ABCD));
    C=HIBYTE(LOWORD(ABCD));
    D=LOBYTE(LOWORD(ABCD));
    
    p=crc32_rf[W];
    o=crc32_rf[(BYTE) (X^G(p))];
    n=crc32_rf[(BYTE) (Y^G(o)^H(p))];
    m=crc32_rf[(BYTE) (Z^G(n)^H(o)^I(p))];
    
    d=m^D;
    c=n^C^I(m);
    b=o^B^H(m)^I(n);
    a=p^A^G(m)^H(n)^I(o);    
    return MakeDWord(MakeWord(a,b),MakeWord(c,d));
}

#define FF(x)  HIBYTE(crc16_tab[(unsigned char)x])
#define GG(x)  LOBYTE(crc16_tab[(unsigned char)x])

void init_crc16_rf_table(void)
{
    unsigned int i,j;
    
    for(i=0; i<256; i++) {
        j = HIBYTE(crc16_tab[i]);
        crc16_rf[j] = i;
    }
    return;
}

unsigned short CRC16(unsigned short AB, unsigned short ab)
{
    BYTE X,Y,A,B,a,b,m,n; 
    
    A=HIBYTE(AB);
    B=LOBYTE(AB);
    a=HIBYTE(ab);
    b=LOBYTE(ab);
    
    m = B^b;
    n = A^GG(m)^a;
    
    X = FF(n);
    Y = FF(m) ^ GG(n); 
    return MakeWord(X,Y);
}

unsigned short rCRC16(unsigned short XY, unsigned short AB)
{
    BYTE X,Y,A,B,a,b,m,n; 
    
    X=HIBYTE(XY);
    Y=LOBYTE(XY);
    A=HIBYTE(AB);
    B=LOBYTE(AB);
    
    n=crc16_rf[X];
    m=crc16_rf[(BYTE) (Y^GG(n))];
    
    b=m^B;
    a=n^A^GG(m); 
    return MakeWord(a,b);
}

void init_crc_all_tab(void)
{
    init_crc32_table(CRC32_POLY);
    init_crc32_rf_table();
    init_crc16_table(CRC16_POLY);
    init_crc16_rf_table();
    return;
}

/* x ^ e % n */
unsigned int powmod32(unsigned int x, unsigned int e, unsigned int n)
{
    /* EDX : EAX = High32 , Low 32 */
    __asm{
        mov eax, x;    // tmp = x
        mov ebx, 1;    // r = 1 
        mov ecx, e;    // e   
        mov esi, n;          
        test ecx, ecx;  // x^0 = 1
        jz  __done;
        cmp eax, esi;  // x  < n
        jb  __loop;  
        xor edx, edx; // High32 = Zero
        div esi;
        mov eax, edx; //  x = x % n
__loop:
        mov edi, eax; // edi saved tmp
        test ecx, 1;
        jz __square;
        mul ebx ;     //  r *= tmp
        div esi ;     //  r %= n, edx is 32 bit reminder, eax is quotient
        mov ebx, edx; // saved r
        mov eax, edi;  
__square:
        mul edi;      //  as eax = edi, tmp *= tmp
        div esi;      //  tmp %= n
        mov eax, edx; // saved tmp 
        shr	ecx, 1 ;
        jnz  __loop;
__done:
        mov eax, ebx; // return r, in eax
    }

}

/*

/* x ^ e % n  , C source code */ 
unsigned int powmod32_c(unsigned int x, unsigned int e, unsigned int n)
{
    unsigned int ee;
    unsigned __int64 r, tmp;
    
    tmp = x%n;
    r = 1;
    ee = e;
    for(; ee != 0; ee >>= 1) {
        if(ee & 0x1) { // e is odd
            r *= tmp;
            r %= n;
        }

        tmp *= tmp;
        tmp %= n;
    }
    
    return ((unsigned int)r);
}

static int myseed;
void mysrand(int seed)
{
    myseed = seed;
    return;
}

/*
y = seed * P , eax is low32 bit
seed = y + b
seed = seed * P + b , this  Y = A*X + B
*/

int myrand(void)
{
    __asm
    {
        mov eax, myseed; 
        mov edx, MYRAND_A; /* this is a prime number */
        mul edx; 
        add eax, MYRAND_B;
        mov myseed, eax;
        shr eax, 1;
    }
}

