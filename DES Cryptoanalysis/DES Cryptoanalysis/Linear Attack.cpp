// Linear attack is reconstructed in this file
// Every .cpp file needs stdafx, since it is a MFC program

#include "stdafx.h"
#include <iostream>
#include <malloc.h>
#include <windows.h>
#include <math.h>
#include <string>
#include <bitset>
#include "Linear Attack.h"

using namespace std;

// Number sets in the annex
int alpha_set[4] = { 7,18,24,29 };
int beta_set[4] = { 27,28,30,31 };
int delta_set[3] = { 7,18,24 };

// DES relevant tables
int expansion_table[48] = { 32,1,2,3,4,5,
							4,5,6,7,8,9,
							8,9,10,11,12,13,
							12,13,14,15,16,17,
							16,17,18,19,20,21,
							20,21,22,23,24,25,
							24,25,26,27,28,29,
							28,29,30,31,32,1 };

int S_box_table[8][64] = { 
		// S1 box
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
		// S2 box
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
		// S3 box
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
		// S4 box
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
		// S5 box
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
		// S6 box
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
		// S7 box
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
		// S8 box
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};

int P_table[32] = { 16,7,20,21,29,12,28,17,
					1,15,23,26,5,18,31,10,
					2,8,24,14,32,27,3,9,
					19,13,30,6,22,11,4,25 };

// Substantiate Lemma 1 on one S-box to better realize the full algorithm
int* FindSBoxLinearity(int box_num)
{
	int max_interval = 0;
	int* max_values=(int*)malloc(sizeof(int)*4);
	for (int a = 1; a <= 63; a++)
	{
		for (int b = 1; b <= 15; b++)
		{
			int overlapping_count = 0;
			for (int x = 0; x <= 63; x++)
			{
				// Linear apporximation of S_box transformation
				bitset<6> alpha = a;
				bitset<6> x_binary = x;
				bitset<4> beta = b;
				int row = x_binary[5] * 2 + x_binary[0];
				int column = x_binary[4] * 8 + x_binary[3] * 4 + x_binary[2] * 2 + x_binary[1];
				bitset<4> S_box_result = S_box_table[box_num][row * 16 + column];
				bitset<6> left_equation = alpha & x_binary;
				bitset<4> right_equation = beta & S_box_result;
				int left_result = 0;
				int right_result = 0;
				for (int index = 0; index <= 5; index++)
				{
					left_result ^= left_equation[index];
				}
				for (int index = 0; index <= 3; index++)
				{
					right_result ^= right_equation[index];
				}
				if (left_result == right_result)
				{
					overlapping_count += 1;
				}
			}
			int gap = abs(overlapping_count - 32);
			if (gap > max_interval)
			{
				max_interval = gap;
				max_values[0] = a;
				max_values[1] = b;
				max_values[2] = overlapping_count;
				max_values[3] = max_interval;
			}
		}
	}
	return max_values;
}

// Calculate the XOR result of plain-cipher text pairs
int ApplyAnalysis(bitset<32>plain_high_pair, bitset<32>plain_low_pair, bitset<32>cipher_high_pair, bitset<32>cipher_low_pair, int enc_iteration)
{
	int result = 0;
	switch (enc_iteration)
	{
		case 3:
		{
			result = plain_high_pair[alpha_set[0]] ^ plain_high_pair[alpha_set[1]] ^ plain_high_pair[alpha_set[2]] ^ plain_high_pair[alpha_set[3]] \
				^ plain_low_pair[15]\
				^cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]] \
				^ cipher_low_pair[15];
			break;
		}
		case 4:
		{
			result = plain_high_pair[alpha_set[0]] ^ plain_high_pair[alpha_set[1]] ^ plain_high_pair[alpha_set[2]] ^ plain_high_pair[alpha_set[3]]\
				^ plain_low_pair[15]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 5:
		{
			result = plain_high_pair[15]\
				^ plain_low_pair[alpha_set[0]] ^ plain_low_pair[alpha_set[1]] ^ plain_low_pair[alpha_set[2]] ^ plain_low_pair[alpha_set[3]]\
				^ plain_low_pair[beta_set[0]] ^ plain_low_pair[beta_set[1]] ^ plain_low_pair[beta_set[2]] ^ plain_low_pair[beta_set[3]]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 6:
		{
			result = plain_low_pair[delta_set[0]] ^ plain_low_pair[delta_set[1]] ^ plain_low_pair[delta_set[2]]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]]\
				^ cipher_low_pair[15];
			break;
		}
		case 7:
		{
			result = plain_high_pair[delta_set[0]] ^ plain_high_pair[delta_set[1]] ^ plain_high_pair[delta_set[2]]\
				^ plain_low_pair[12] ^ plain_low_pair[16]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]]\
				^ cipher_low_pair[15];
			break;
		}
		case 8:
		{
			result = plain_high_pair[delta_set[0]] ^ plain_high_pair[delta_set[1]] ^ plain_high_pair[delta_set[2]]\
				^ plain_low_pair[12] ^ plain_low_pair[16]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 9:
		{
			result = plain_high_pair[15]\
				^ plain_low_pair[beta_set[0]] ^ plain_low_pair[beta_set[1]] ^ plain_low_pair[beta_set[2]] ^ plain_low_pair[beta_set[3]]\
				^ plain_low_pair[delta_set[0]] ^ plain_low_pair[delta_set[1]] ^ plain_low_pair[delta_set[2]]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 10:
		{
			result = plain_low_pair[alpha_set[0]] ^ plain_low_pair[alpha_set[1]] ^ plain_low_pair[alpha_set[2]] ^ plain_low_pair[alpha_set[3]]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]]\
				^ cipher_low_pair[15];
			break;
		}
		case 11:
		{
			result = plain_high_pair[alpha_set[0]] ^ plain_high_pair[alpha_set[1]] ^ plain_high_pair[alpha_set[2]] ^ plain_high_pair[alpha_set[3]] ^ plain_low_pair[15]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]] ^ cipher_low_pair[15];
			break;
		}
		case 12:
		{
			result = plain_high_pair[alpha_set[0]] ^ plain_high_pair[alpha_set[1]] ^ plain_high_pair[alpha_set[2]] ^ plain_high_pair[alpha_set[3]]\
				^ plain_low_pair[15]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 13:
		{
			result = plain_high_pair[15]\
				^ plain_low_pair[alpha_set[0]] ^ plain_low_pair[alpha_set[1]] ^ plain_low_pair[alpha_set[2]] ^ plain_low_pair[alpha_set[3]]\
				^ plain_low_pair[beta_set[0]] ^ plain_low_pair[beta_set[1]] ^ plain_low_pair[beta_set[2]] ^ plain_low_pair[beta_set[3]]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
		case 14:
		{
			result = plain_low_pair[delta_set[0]] ^ plain_low_pair[delta_set[1]] ^ plain_low_pair[delta_set[2]]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]]\
				^ cipher_low_pair[15];
			break;
		}
		case 15:
		{
			result = plain_high_pair[delta_set[0]] ^ plain_high_pair[delta_set[1]] ^ plain_high_pair[delta_set[2]]\
				^ plain_low_pair[12] ^ plain_low_pair[16]\
				^ cipher_high_pair[alpha_set[0]] ^ cipher_high_pair[alpha_set[1]] ^ cipher_high_pair[alpha_set[2]] ^ cipher_high_pair[alpha_set[3]]\
				^ cipher_low_pair[15];
			break;
		}
		case 16:
		{
			result = plain_high_pair[delta_set[0]] ^ plain_high_pair[delta_set[1]] ^ plain_high_pair[delta_set[2]]\
				^ plain_low_pair[12] ^ plain_low_pair[16]\
				^ cipher_high_pair[15]\
				^ cipher_low_pair[alpha_set[0]] ^ cipher_low_pair[alpha_set[1]] ^ cipher_low_pair[alpha_set[2]] ^ cipher_low_pair[alpha_set[3]]\
				^ cipher_low_pair[beta_set[0]] ^ cipher_low_pair[beta_set[1]] ^ cipher_low_pair[beta_set[2]] ^ cipher_low_pair[beta_set[3]];
			break;
		}
	}
	return result;
}

// Correspond the key bits in the right side of the equation in Algorithm 1
struct Pair* KeyBitSelection(int enc_iteration)
{
	struct Pair* head = (struct Pair*)malloc(sizeof(Pair));
	switch (enc_iteration)
	{
		case 3:
		{
			struct Pair* kv = (struct Pair*)malloc(sizeof(Pair));
			head->iteration = 1;
			head->keybit = 22;
			head->next = kv;
			kv->iteration = 3;
			kv->keybit = 22;
			kv->next = NULL;
			break;
		}
		case 4:
		{
			head->iteration = 1;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 22;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 42;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 4;
			kv_3->keybit = 43;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 4;
			kv_4->keybit = 45;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 4;
			kv_5->keybit = 46;
			kv_4->next = kv_5;
			kv_5->next = NULL;
			break;
		}
		case 5:
		{
			head->iteration = 1;
			head->keybit = 42;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 43;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 1;
			kv_2->keybit = 45;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 1;
			kv_3->keybit = 46;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 2;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 4;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 5;
			kv_6->keybit = 42;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 5;
			kv_7->keybit = 43;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 5;
			kv_8->keybit = 45;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 5;
			kv_9->keybit = 46;
			kv_8->next = kv_9;
			kv_9->next = NULL;
			break;
		}
		case 6:
		{
			head->iteration = 2;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 44;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 6;
			kv_3->keybit = 22;
			kv_2->next = kv_3;
			kv_3 = NULL;
			break;
		}
		case 7:
		{
			head->iteration = 1;
			head->keybit = 19;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 23;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 3;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 4;
			kv_3->keybit = 44;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 5;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 7;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			kv_5->next = NULL;
			break;
		}
		case 8:
		{
			head->iteration = 1;
			head->keybit = 19;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 23;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 3;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 4;
			kv_3->keybit = 44;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 5;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 7;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 8;
			kv_6->keybit = 42;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 8;
			kv_7->keybit = 43;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 8;
			kv_8->keybit = 45;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 8;
			kv_9->keybit = 46;
			kv_8->next = kv_9;
			kv_9->next = NULL;
			break;
		}
		case 9:
		{
			head->iteration = 1;
			head->keybit = 42;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 43;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 1;
			kv_2->keybit = 45;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 1;
			kv_3->keybit = 46;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 2;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 4;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 5;
			kv_6->keybit = 44;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 6;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 8;
			kv_8->keybit = 22;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 9;
			kv_9->keybit = 42;
			kv_8->next = kv_9;
			struct Pair* kv_10 = (struct Pair*)malloc(sizeof(Pair));
			kv_10->iteration = 9;
			kv_10->keybit = 43;
			kv_9->next = kv_10;
			struct Pair* kv_11 = (struct Pair*)malloc(sizeof(Pair));
			kv_11->iteration = 9;
			kv_11->keybit = 45;
			kv_10->next = kv_11;
			struct Pair* kv_12 = (struct Pair*)malloc(sizeof(Pair));
			kv_12->iteration = 9;
			kv_12->keybit = 46;
			kv_11->next = kv_12;
			kv_12->next = NULL;
			break;
		}
		case 10:
		{
			head->iteration = 2;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 44;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 6;
			kv_3->keybit = 22;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 7;
			kv_4->keybit = 44;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 8;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 10;
			kv_6->keybit = 22;
			kv_5->next = kv_6;
			kv_6->next = NULL;
			break;
		}
		case 11:
		{
			head->iteration = 1;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 22;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 44;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 5;
			kv_3->keybit = 22;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 7;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 8;
			kv_5->keybit = 44;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 9;
			kv_6->keybit = 22;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 11;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			kv_7->next = NULL;
		}
		case 12:
		{
			head->iteration = 1;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 22;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 44;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 5;
			kv_3->keybit = 22;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 7;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 8;
			kv_5->keybit = 44;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 9;
			kv_6->keybit = 22;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 11;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 12;
			kv_8->keybit = 42;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 12;
			kv_9->keybit = 43;
			kv_8->next = kv_9;
			struct Pair* kv_10 = (struct Pair*)malloc(sizeof(Pair));
			kv_10->iteration = 12;
			kv_10->keybit = 45;
			kv_9->next = kv_10;
			struct Pair* kv_11 = (struct Pair*)malloc(sizeof(Pair));
			kv_11->iteration = 12;
			kv_11->keybit = 46;
			kv_10->next = kv_11;
			kv_11->next = NULL;
			break;
		}
		case 13:
		{
			head->iteration = 1;
			head->keybit = 42;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 43;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 1;
			kv_2->keybit = 45;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 1;
			kv_3->keybit = 46;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 2;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 4;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 5;
			kv_6->keybit = 44;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 6;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 8;
			kv_8->keybit = 22;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 9;
			kv_9->keybit = 44;
			kv_8->next = kv_9;
			struct Pair* kv_10 = (struct Pair*)malloc(sizeof(Pair));
			kv_10->iteration = 10;
			kv_10->keybit = 22;
			kv_9->next = kv_10;
			struct Pair* kv_11 = (struct Pair*)malloc(sizeof(Pair));
			kv_11->iteration = 12;
			kv_11->keybit = 22;
			kv_10->next = kv_11;
			struct Pair* kv_12 = (struct Pair*)malloc(sizeof(Pair));
			kv_12->iteration = 13;
			kv_12->keybit = 42;
			kv_11->next = kv_12;
			struct Pair* kv_13 = (struct Pair*)malloc(sizeof(Pair));
			kv_13->iteration = 13;
			kv_13->keybit = 43;
			kv_12->next = kv_12;
			struct Pair* kv_14 = (struct Pair*)malloc(sizeof(Pair));
			kv_14->iteration = 13;
			kv_14->keybit = 45;
			kv_13->next = kv_14;
			struct Pair* kv_15 = (struct Pair*)malloc(sizeof(Pair));
			kv_15->iteration = 13;
			kv_15->keybit = 46;
			kv_14->next = kv_15;
			kv_15->next = NULL;
			break;
		}
		case 14:
		{
			head->iteration = 2;
			head->keybit = 22;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 3;
			kv_1->keybit = 44;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 4;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 6;
			kv_3->keybit = 22;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 7;
			kv_4->keybit = 44;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 8;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 10;
			kv_6->keybit = 22;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 11;
			kv_7->keybit = 44;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 12;
			kv_8->keybit = 22;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 14;
			kv_9->keybit = 22;
			kv_8->next = kv_9;
			kv_9->next = NULL;
			break;
		}
		case 15:
		{
			head->iteration = 1;
			head->keybit = 19;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 23;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 3;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 4;
			kv_3->keybit = 44;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 5;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 7;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 8;
			kv_6->keybit = 44;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 9;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 11;
			kv_8->keybit = 22;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 12;
			kv_9->keybit = 44;
			kv_8->next = kv_9;
			struct Pair* kv_10 = (struct Pair*)malloc(sizeof(Pair));
			kv_10->iteration = 13;
			kv_10->keybit = 22;
			kv_9->next = kv_10;
			struct Pair* kv_11 = (struct Pair*)malloc(sizeof(Pair));
			kv_11->iteration = 15;
			kv_11->keybit = 22;
			kv_10->next = kv_11;
			kv_11->next = NULL;
			break;
		}
		case 16:
		{
			head->iteration = 1;
			head->keybit = 19;
			struct Pair* kv_1 = (struct Pair*)malloc(sizeof(Pair));
			kv_1->iteration = 1;
			kv_1->keybit = 23;
			head->next = kv_1;
			struct Pair* kv_2 = (struct Pair*)malloc(sizeof(Pair));
			kv_2->iteration = 3;
			kv_2->keybit = 22;
			kv_1->next = kv_2;
			struct Pair* kv_3 = (struct Pair*)malloc(sizeof(Pair));
			kv_3->iteration = 4;
			kv_3->keybit = 44;
			kv_2->next = kv_3;
			struct Pair* kv_4 = (struct Pair*)malloc(sizeof(Pair));
			kv_4->iteration = 5;
			kv_4->keybit = 22;
			kv_3->next = kv_4;
			struct Pair* kv_5 = (struct Pair*)malloc(sizeof(Pair));
			kv_5->iteration = 7;
			kv_5->keybit = 22;
			kv_4->next = kv_5;
			struct Pair* kv_6 = (struct Pair*)malloc(sizeof(Pair));
			kv_6->iteration = 8;
			kv_6->keybit = 44;
			kv_5->next = kv_6;
			struct Pair* kv_7 = (struct Pair*)malloc(sizeof(Pair));
			kv_7->iteration = 9;
			kv_7->keybit = 22;
			kv_6->next = kv_7;
			struct Pair* kv_8 = (struct Pair*)malloc(sizeof(Pair));
			kv_8->iteration = 11;
			kv_8->keybit = 22;
			kv_7->next = kv_8;
			struct Pair* kv_9 = (struct Pair*)malloc(sizeof(Pair));
			kv_9->iteration = 12;
			kv_9->keybit = 44;
			kv_8->next = kv_9;
			struct Pair* kv_10 = (struct Pair*)malloc(sizeof(Pair));
			kv_10->iteration = 13;
			kv_10->keybit = 22;
			kv_9->next = kv_10;
			struct Pair* kv_11 = (struct Pair*)malloc(sizeof(Pair));
			kv_11->iteration = 15;
			kv_11->keybit = 22;
			kv_10->next = kv_11;
			struct Pair* kv_12 = (struct Pair*)malloc(sizeof(Pair));
			kv_12->iteration = 16;
			kv_12->keybit = 42;
			kv_11->next = kv_12;
			struct Pair* kv_13 = (struct Pair*)malloc(sizeof(Pair));
			kv_13->iteration = 16;
			kv_13->keybit = 43;
			kv_12->next = kv_13;
			struct Pair* kv_14 = (struct Pair*)malloc(sizeof(Pair));
			kv_14->iteration = 16;
			kv_14->keybit = 45;
			kv_13->next = kv_14;
			struct Pair* kv_15 = (struct Pair*)malloc(sizeof(Pair));
			kv_15->iteration = 16;
			kv_15->keybit = 46;
			kv_14->next = kv_15;
			kv_15->next = NULL;
			break;
		}
	}
	return head;
}

struct Pair* InvKeyBitSelection(struct Pair* head_p_to_c)
{
	struct Pair* temp1 = head_p_to_c;
	struct Pair* temp2 = NULL;
	struct Pair* head_c_to_p = NULL;
	int count = 0;
	for (temp1; temp1 != NULL; temp1 = temp1->next)
	{
		struct Pair* temp3 = (struct Pair*)malloc(sizeof(Pair));
		temp3->iteration = 15 - (temp1->iteration);
		temp3->keybit = temp1->keybit;
		if (count == 0)
		{
			head_c_to_p = temp3;
			temp2 = temp3;
			temp3->next = NULL;
		}
		else
		{
			temp2->next = temp3;
			temp2 = temp3;
			temp3->next = NULL;
		}
		count++;
	}
	return head_c_to_p;
}

// Implementation of Mitsuru Algorithm 1
// The annex provides the best expression and the best probability of DES cipher
// Here we follow the annex 
float MaxLinearProbabilityEstimation(string plaintext, string ciphertext, int pair_length, int enc_iteration)
{
	int zero_count = 0;
	for (int index = 0; index < pair_length / 8; index++) 
	{
		string plain_left_info = plaintext.substr(index * 8, 4);
		string plain_right_info = plaintext.substr(index * 8 + 4, 4);
		string cipher_left_info = ciphertext.substr(index * 8, 4);
		string cipher_right_info = ciphertext.substr(index * 8 + 4, 4);
		bitset<32> plain_high_pair;
		bitset<32> plain_low_pair;
		bitset<32> cipher_high_pair;
		bitset<32> cipher_low_pair;
		for (int digit = 0; digit < 32; digit++)
		{
			int detector = 1;
			int char_count = digit / 8;
			for (int move = 0; move < digit % 8; move++)
			{
				detector = detector << 1;
			}
			const char* pointer1 = plain_left_info.data();
			const char* pointer2 = plain_right_info.data();
			const char* pointer3 = cipher_left_info.data();
			const char* pointer4 = cipher_right_info.data();
			if ((detector & int(*(pointer1 + char_count))) != 0)
			{
				plain_high_pair.flip(digit);
			}
			if ((detector & int(*(pointer2 + char_count))) != 0)
			{
				plain_low_pair.flip(digit);
			}
			if ((detector & int(*(pointer3 + char_count))) != 0)
			{
				cipher_high_pair.flip(digit);
			}
			if ((detector & int(*(pointer4 + char_count))) != 0)
			{
				cipher_low_pair.flip(digit);
			}
		}
		int result = ApplyAnalysis(plain_high_pair, plain_low_pair, cipher_high_pair, cipher_low_pair, enc_iteration);
		if (result == 0)
		{
			zero_count++;
		}
	}
	float percentage = float(zero_count) / float(pair_length) * 8;
	return percentage;
}