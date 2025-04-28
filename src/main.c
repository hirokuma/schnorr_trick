#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <secp256k1.h>
#include <secp256k1_schnorrsig.h>

#define TAG_CHALLENGE   "BIP0340/challenge"
#define TAG_CHALLENGE_LEN   (sizeof(TAG_CHALLENGE) - 1)

const uint8_t GX[32] = {
    0x79, 0xbe, 0x66, 0x7e, 0xf9, 0xdc, 0xbb, 0xac,
    0x55, 0xa0, 0x62, 0x95, 0xce, 0x87, 0x0b, 0x07,
    0x02, 0x9b, 0xfc, 0xdb, 0x2d, 0xce, 0x28, 0xd9,
    0x59, 0xf2, 0x81, 0x5b, 0x16, 0xf8, 0x17, 0x98,
};
const uint8_t DATA_0001[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};
const uint8_t MSG[] = { 0x01, 0x02, 0x03, 0x04 };

static  int nonce_function(
    unsigned char *nonce32,
    const unsigned char *msg,
    size_t msglen,
    const unsigned char *key32,
    const unsigned char *xonly_pk32,
    const unsigned char *algo,
    size_t algolen,
    void *data)
{
    {
        (void)msg;
        (void)msglen;
        (void)key32;
        (void)xonly_pk32;
        (void)algo;
        (void)algolen;
        (void)data;
    }

    memcpy(nonce32, DATA_0001, sizeof(DATA_0001));
    return 1;
}

void schnorr(const secp256k1_context *ctx)
{
    int ret;

    uint8_t sig[64];
    secp256k1_keypair keypair;
    secp256k1_schnorrsig_extraparams extraparams = SECP256K1_SCHNORRSIG_EXTRAPARAMS_INIT;
    extraparams.noncefp = nonce_function;

    // G
    printf("G.x =\n");
    for (size_t i = 0; i < sizeof(GX); i++) {
        printf("%02x", GX[i]);
    }
    printf("\n\n");

    // keypair
    ret = secp256k1_keypair_create(ctx, &keypair, DATA_0001);
    if (ret != 1) {
        fprintf(stderr, "keypair_create failed\n");
        return;
    }

    // m = R.x || P.x || msg = G.x || G.x || msg
    uint8_t m[sizeof(GX) + sizeof(GX) + sizeof(MSG)];
    memcpy(m, GX, sizeof(GX));
    memcpy(m + sizeof(GX), GX, sizeof(GX));
    memcpy(m + sizeof(GX) + sizeof(GX), MSG, sizeof(MSG));
    printf("m =\n");
    for (size_t i = 0; i < sizeof(m); i++) {
        printf("%02x", m[i]);
    }
    printf("\n\n");

    // tagged hash(BIP340challenge)(m)
    uint8_t tagged_challenge[32];
    ret = secp256k1_tagged_sha256(
        ctx,
        tagged_challenge,
        (const uint8_t *)(TAG_CHALLENGE),
        TAG_CHALLENGE_LEN,
        m,
        sizeof(m)
    );
    if (ret != 1) {
        fprintf(stderr, "tagged_sha256 failed\n");
        return;
    }
    printf("tagged challenge =\n");
    for (size_t i = 0; i < sizeof(tagged_challenge); i++) {
        printf("%02x", tagged_challenge[i]);
    }
    printf("\n\n");

    ret = secp256k1_schnorrsig_sign_custom(
              ctx,
              sig,
              MSG,
              sizeof(MSG),
              &keypair,
              &extraparams
          );
    if (ret != 1) {
        fprintf(stderr, "schnorrsig_sign_custom failed\n");
        return;
    }

    printf("sig =\n");
    for (int i = 0; i < 32; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n");
    for (int i = 32; i < 64; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n\n");
}

int main(void)
{
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    schnorr(ctx);
    secp256k1_context_destroy(ctx);
    return 0;
}