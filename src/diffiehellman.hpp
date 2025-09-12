#include <cmath>
#include <iostream>

using namespace std;

#define DH_KEY_LENGTH (32)
typedef unsigned char DH_KEY[DH_KEY_LENGTH];

typedef union _uint256_t {
	struct {
		uint64_t low1;
        uint64_t low2;
		uint64_t high1;
        uint64_t high2;
	};
	unsigned char byte[DH_KEY_LENGTH];
} _uint256_t;

static const _uint256_t P = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF43};

//Power function to return value of a ^ b mod P
long long int power(long long int a, long long int b, long long int P){
    if(b == 1){
        return a;
    } else {
        return ((long long int)pow(a, b) % P);
    }
}



static inline void _u256_make(_uint256_t* dq, const DH_KEY key){

}

static inline int _u256_is_odd(const _uint256_t dq){

}

static inline void _u256_lshift(const _uint256_t* dq){
    uint64_t t = (dq->low2 & 1) << 63; //reconsider this logic because left shift means that  
    uint64_t t = (dq->low1 >> 63) & 1;
}

void DH_generate_key_pair(DH_KEY public_key, DH_KEY private_key){
    _uint256_t private_k;
    _uint256_t public_k;
    for(int i = 0; i < DH_KEY_LENGTH; i++){
        private_key[i] = rand() & 0xFF;
    }

    /* pub_key = G^prv_key mod P*/

}

void DH_generate_key_secret(DH_KEY secret_key, const DH_KEY my_private, const DH_KEY another_public){

}
