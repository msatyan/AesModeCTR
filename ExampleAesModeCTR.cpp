/*
Copyright 2017 Sathyanesh Krishnan

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


extern "C" {
#include "openssl/aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

#include "AesModeCTR.h"

void MyMemSet(unsigned char *p, unsigned int t);
void MyPrintBinData(const unsigned char *BinData, size_t len);


int main()
{
    unsigned char Buff[1024];
    unsigned char key[] = "01234567890123456789012345678901";  // Key is of 256 bit 
    unsigned char iv[] = "01234567890123456"; // IV is 128 bit 

    AesModeCTR aesctr(key, AesKeySize::AES256, iv);


    unsigned char *plaintext = (unsigned char *)"We will send this message for encryption and then try decrypt too!.";
    size_t len = 0;

    len = strlen((const char *)plaintext);

    MyMemSet(Buff, sizeof(Buff));
    aesctr.Encrypt(plaintext, Buff, len);

    MyPrintBinData(Buff, len);

    return(0);
}

void MyMemSet(unsigned char *p, unsigned int t)
{
    unsigned int i = 0;
    for (i = 0; i < t; ++i)
    {
        *p++ = 0;
    }
}


void MyPrintBinData(const unsigned char *BinData, size_t len)
{
    size_t i;
    int DisplayBlockSeparation = 0;

    for (i = 0; i < len; i++)
    {
        printf("%X", BinData[i] / 16);
        printf("%X", BinData[i] % 16);

        ++DisplayBlockSeparation;
        if (DisplayBlockSeparation == 4)
        {
            DisplayBlockSeparation = 0;
            printf(" ");
        }
    }
    printf("\n");
}

