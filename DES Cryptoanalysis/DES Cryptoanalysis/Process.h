#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <bitset>
using namespace std;

string Padding(string text, long pad_length);

string Erasing(string text);

bitset<64> InitialPermutation(string text);

string FinalPermutation(bitset<64> text);

bitset<56> PC1_Permutation(string key);

bitset<56> PC1_BitPermutation(bitset<64> key);

bitset<56> KeyTransformation(bitset<56> key, int iteration, bool encrypted);

bitset<48> PC2_Transformation(bitset<56> transformed_key);

bitset<32> SplitLeft(bitset<64> text);

bitset<32> SplitRight(bitset<64> text);

bitset<32> F_Transformation(bitset<32> right, bitset<48> PC2_key);

bitset<32> XorTransformation(bitset<32> left, bitset<32> right);

bitset<64> SwapLeftAndRight(bitset<32> left, bitset<32> right);

bitset<56> InitializeKey(bitset<56> key, int max_iteration);

string MessageProcess(string text, string key, int max_iteration, bool verbose_flag, bool encrypted);

#endif