#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AES/aes.h"
#include "AES/base64.h"

int main()
{
	AES_KEY key;

	char *password="s87PfD3FczE5z01XaB6YacbG9lQc20A3";
	char *token=\
		"cOyQFIgOM0W7kcSx1kg5tOri+nRCvD1URH0nK5B+8kXW3MitKbjNKS1cjX2A3BJ+O+9Gx6uC6cICbEs5M7YwlNEMZmSrXALHbKtEh+NKgIsu/29WPy7mwHc2lkLB/0tN3XW7YeVE+MPRmMWDkScKXCETWjZslCyNVcpGQ/3juh8=";

	// char *token="0123456789abcdef0123456789abcdef0123456789abcdef0123456789";

	char in[1024]={0};
	char cipher[1024]={0};
	char text[1024]={0};
	char cipher64[1024] = {0};

	int len = strlen(token);
	int rem = 16 - len % 16;
	int bits = strlen(password) * 8;
	
	memcpy(in, token, len);
	memset(in+len, ' ', rem);
	len += rem;


	AES_set_encrypt_key(password, bits, &key);
	AES_ecb_encrypt(in, cipher, len, &key, 1);   // enc = 1, encrypt

	initb64();
	htob64(cipher, cipher64, len);
	

    char* output_cipher = "G74B8eIxtd1UQ8sEDZEReQuVmO/UrkE9rtTjAnimUZR+ZWJEbh3RjGVbUqlQGdO4X7UFV9s0hxSpnXEdU+eOoQnCJfbS/iQ+ZyRn0p82RWRVtDJuJq+/+3EgJ23YMJCRxKGDTePOXNob/ETr6z1VdIUo2PWFhKdqMeW57V+3tSbTbsK0rsVp2nkqWQBUWHbmY9WIltvVrUFvfvLsJUEr2K3CoK6rXmbaXGIQPBh1Yd0";
	b64toh(output_cipher, cipher);
    
    
    AES_set_decrypt_key(password, bits, &key);
	AES_ecb_encrypt(cipher, text, len, &key, 0); // enc = 0, decrypt
	
	printf(" in=\n%s,len=%d\n cipher=\n%s\n text=\n%s,len=%d\n", in, len, cipher, text, strlen(text));

	printf(" cipher64=\n%s\n,b64len=%d\n", cipher64, strlen(cipher64));

}


