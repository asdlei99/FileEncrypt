#include "AES256.h"

void Decryption::AES256::InverseCipher(BYTE srcBytes[16], const UINT32 srcExpandedKey[60]) {
    *((UINT32*)srcBytes) ^= srcExpandedKey[56];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[57];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[58];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[59];

    for(BYTE i = 13; i > 0; i--) {
        //Inverse Shift rows starts;
        //Inverse shift the second row;
        srcBytes[13] ^= srcBytes[9] ^= srcBytes[13] ^= srcBytes[9];
        srcBytes[9] ^= srcBytes[5] ^= srcBytes[9] ^= srcBytes[5];
        srcBytes[5] ^= srcBytes[1] ^= srcBytes[5] ^= srcBytes[1];
        //Inverse shift the third row;
        srcBytes[14] ^= srcBytes[6] ^= srcBytes[14] ^= srcBytes[6];
        srcBytes[10] ^= srcBytes[2] ^= srcBytes[10] ^= srcBytes[2];
        //Inverse shift the fourth row;
        srcBytes[3] ^= srcBytes[7] ^= srcBytes[3] ^= srcBytes[7];
        srcBytes[7] ^= srcBytes[11] ^= srcBytes[7] ^= srcBytes[11];
        srcBytes[11] ^= srcBytes[15] ^= srcBytes[11] ^= srcBytes[15];

        for(BYTE j = 0; j < 16; j++) srcBytes[j] = Decryption::InverseSBox[srcBytes[j]];

        *((UINT32*)srcBytes) ^= srcExpandedKey[i << 2];
        *((UINT32*)srcBytes + 1) ^= srcExpandedKey[(i << 2) + 1];
        *((UINT32*)srcBytes + 2) ^= srcExpandedKey[(i << 2) + 2];
        *((UINT32*)srcBytes + 3) ^= srcExpandedKey[(i << 2) + 3];

        for(BYTE j = 0; j < 16; j += 4) {
            BYTE tmp[4];
            *(UINT32*)tmp = *((UINT32*)srcBytes + (j >> 2));
            srcBytes[j] = Decryption::Multiply0x0E[tmp[0]] ^ Decryption::Multiply0x0B[tmp[1]] ^ Decryption::Multiply0x0D[tmp[2]] ^ Decryption::Multiply0x09[tmp[3]];
            srcBytes[j + 1] = Decryption::Multiply0x09[tmp[0]] ^ Decryption::Multiply0x0E[tmp[1]] ^ Decryption::Multiply0x0B[tmp[2]] ^ Decryption::Multiply0x0D[tmp[3]];
            srcBytes[j + 2] = Decryption::Multiply0x0D[tmp[0]] ^ Decryption::Multiply0x09[tmp[1]] ^ Decryption::Multiply0x0E[tmp[2]] ^ Decryption::Multiply0x0B[tmp[3]];
            srcBytes[j + 3] = Decryption::Multiply0x0B[tmp[0]] ^ Decryption::Multiply0x0D[tmp[1]] ^ Decryption::Multiply0x09[tmp[2]] ^ Decryption::Multiply0x0E[tmp[3]];
        }
    }

    //Inverse Shift rows starts;
    //Inverse shift the second row;
    srcBytes[13] ^= srcBytes[9] ^= srcBytes[13] ^= srcBytes[9];
    srcBytes[9] ^= srcBytes[5] ^= srcBytes[9] ^= srcBytes[5];
    srcBytes[5] ^= srcBytes[1] ^= srcBytes[5] ^= srcBytes[1];
    //Inverse shift the third row;
    srcBytes[14] ^= srcBytes[6] ^= srcBytes[14] ^= srcBytes[6];
    srcBytes[10] ^= srcBytes[2] ^= srcBytes[10] ^= srcBytes[2];
    //Inverse shift the fourth row;
    srcBytes[3] ^= srcBytes[7] ^= srcBytes[3] ^= srcBytes[7];
    srcBytes[7] ^= srcBytes[11] ^= srcBytes[7] ^= srcBytes[11];
    srcBytes[11] ^= srcBytes[15] ^= srcBytes[11] ^= srcBytes[15];

    for(BYTE j = 0; j < 16; j++) srcBytes[j] = Decryption::InverseSBox[srcBytes[j]];

    *((UINT32*)srcBytes) ^= srcExpandedKey[0];
    *((UINT32*)srcBytes + 1) ^= srcExpandedKey[1];
    *((UINT32*)srcBytes + 2) ^= srcExpandedKey[2];
    *((UINT32*)srcBytes + 3) ^= srcExpandedKey[3];
}

void Decryption::AES256::KeyExpansion(const BYTE srcKey[32], UINT32 dstExpandedKey[60]) {
    for(BYTE i = 0; i < 8; i++) dstExpandedKey[i] = *((UINT32*)srcKey + i);
    for(BYTE i = 8; i < 60; i++) {
        UINT32 tmp = dstExpandedKey[i - 1];
        if(i % 8 == 0) {
            tmp = tmp >> 8 | tmp << 24;
            *((BYTE*)&tmp) = Decryption::SBox[*((BYTE*)&tmp)];
            *((BYTE*)&tmp + 1) = Decryption::SBox[*((BYTE*)&tmp + 1)];
            *((BYTE*)&tmp + 2) = Decryption::SBox[*((BYTE*)&tmp + 2)];
            *((BYTE*)&tmp + 3) = Decryption::SBox[*((BYTE*)&tmp + 3)];
            tmp ^= Decryption::Rcon[i >> 3];
        }
        if(i % 8 == 4) {
            *((BYTE*)&tmp) = Decryption::SBox[*((BYTE*)&tmp)];
            *((BYTE*)&tmp + 1) = Decryption::SBox[*((BYTE*)&tmp + 1)];
            *((BYTE*)&tmp + 2) = Decryption::SBox[*((BYTE*)&tmp + 2)];
            *((BYTE*)&tmp + 3) = Decryption::SBox[*((BYTE*)&tmp + 3)];
        }
        dstExpandedKey[i] = dstExpandedKey[i - 8] ^ tmp;
    }
}
