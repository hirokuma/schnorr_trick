#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <secp256k1.h>
#include <secp256k1_schnorrsig.h>

const uint8_t MSG[] = {};
const uint8_t SECKEY[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};

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

    memcpy(nonce32, SECKEY, sizeof(SECKEY));
    return 1;
}

void schnorr(const secp256k1_context *ctx)
{
    int ret;

    uint8_t sig[64];
    secp256k1_keypair keypair;
    secp256k1_schnorrsig_extraparams extraparams = SECP256K1_SCHNORRSIG_EXTRAPARAMS_INIT;
    extraparams.noncefp = nonce_function;

    // keypair
    ret = secp256k1_keypair_create(ctx, &keypair, SECKEY);
    if (ret != 1) {
        fprintf(stderr, "keypair_create failed\n");
        return;
    }

    // extraparams

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

    printf("sig = ");
    for (int i = 0; i < 64; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n");
}

int main(void)
{
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    schnorr(ctx);
    secp256k1_context_destroy(ctx);
    return 0;
}