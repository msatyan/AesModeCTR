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
}

#include "AesModeCTR.h"

AesModeCTR::AesModeCTR(const unsigned char *key, AesKeySize ks, const unsigned char *iv)
{
    int i = 0;

    i = sizeof(AESkey);
    i = sizeof(AES_KEY);
    for (i = 0; i < IV_SIZE; ++i)
    {
        BuffIV[i] = *(iv + i);
    }
    
    // AESkey is a data structure holding a transformed version of the key, for efficiency.
    AES_set_encrypt_key((const unsigned char *)key, ks, &AESkey);
}


void AesModeCTR::GetIvCtrMode(size_t c, unsigned char IvCtr[IV_SIZE])
{
    const unsigned char *iv = piv;
    size_t *data = (size_t *)IvCtr;
    size_t d = 0;
    size_t n = 0;

    const union
    {
        long one;
        char little;
    } is_endian = {1};

    if (is_endian.little || ((size_t)iv % sizeof(size_t)) != 0)
    {
        // Little Endian Platforms
        n = IV_SIZE;
        do
        {
            --n;
            c += iv[n];
            IvCtr[n] = (u8)c;

            c >>= 8;
        } while (n);
        return;
    }

    // Big Endian Platforms
    n = IV_SIZE / sizeof(size_t);
    do
    {
        --n;
        d = data[n] += c;

        c = ((d - c) ^ d) >> (sizeof(size_t) * 8 - 1);
    } while (n);

    return;
}

void AesModeCTR::Encrypt(const unsigned char *in, unsigned char *out, size_t len)
{
    // The output buffer should have minimum of the same size as input data (len)

    unsigned char IvCtr[IV_SIZE];
    unsigned char AesCipherOut[BLOCK_SIZE];
    size_t c = 0;
    size_t n = 0;

    while (len >= BLOCK_SIZE) // A full AES block
    {
        // The IvCtr will be different for each block
        // with a deterministic outcome for any specific block
        GetIvCtrMode(c, IvCtr);

        // Encrypt this IvCtr block
        AES_encrypt(IvCtr, AesCipherOut, (const AES_KEY *)&AESkey);
        
        for (n = 0; n < BLOCK_SIZE; n += sizeof(size_t))
        {
            // // The AesCipherOut of IvCtr is being XOR with input data to produce the encriypted data
            *(size_t *)(out + n) = *(size_t *)(in + n) ^ *(size_t *)(AesCipherOut + n);
        }

        len -= BLOCK_SIZE;
        out += BLOCK_SIZE;
        in += BLOCK_SIZE;
        n = 0;
        ++c;
    }

    if (len) // Less than an AES block size of data
    {
        GetIvCtrMode(c, IvCtr);

        AES_encrypt(IvCtr, AesCipherOut, (const AES_KEY *)&AESkey);
        while (len--)
        {
            // // The AesCipherOut of IvCtr is being XOR with input data to produce the encriypted data
            out[n] = in[n] ^ AesCipherOut[n];
            ++n;
        }
    }

    return;
}

void AesModeCTR::Decrypt(const unsigned char *in, unsigned char *out, size_t len)
{
}