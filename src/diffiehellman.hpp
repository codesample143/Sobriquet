#include <cmath>
#include <iostream>

using namespace std;

#define DH_KEY_LENGTH	(16)

typedef union _uint256_t {
	struct {
		uint64_t low1;
        uint64_t low2;
		uint64_t high1;
        uint64_t high2;
	};
	unsigned char byte[DH_KEY_LENGTH];
} _uint256_t;

//Power function to return value of a ^ b mod P
long long int power(long long int a, long long int b, long long int P){
    if(b == 1){
        return a;
    } else {
        return ((long long int)pow(a, b) % P);
    }
}