#ifndef LINEARATTACK_H
#define LINEARATTACK_H

#include <string>

using namespace std;

// Define struct for implementing Algorithm 1
// The keybit in the struct is geq to 0 and smaller than 48, which is the result of PC-2 Algorithm
typedef struct Pair
{
	int iteration;
	int keybit;
	struct Pair* next = NULL;
};

int* FindSBoxLinearity(int box_num);

int ApplyAnalysis(bitset<32>plain_high_pair, bitset<32>plain_low_pair, bitset<32>cipher_high_pair, bitset<32>cipher_low_pair, int enc_iteration);

struct Pair* KeyBitSelection(int enc_iteration);

struct Pair* InvKeyBitSelection(struct Pair* head_p_to_c);

float MaxLinearProbabilityEstimation(string plaintext, string ciphertext, int pair_length, int enc_iteration);

#endif
