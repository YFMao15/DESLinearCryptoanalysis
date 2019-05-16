// Encypting & Decrypting methods are implemented in this .cpp file
// Every .cpp file needs stdafx, since it is a MFC program

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <math.h>
#include <string>
#include <bitset>
#include "UtilFunction.h"
#include "Process.h"

using namespace std;

// The implementation of single steps in DES
string Padding(string text,long pad_length)
{
	string padding_string;
	char pad_number = char(pad_length + 48);
	for (int count = 0; count < pad_length; count++)
	{
		padding_string += pad_number;
	}
	text.append(padding_string);
	return text;
}

string Erasing(string text)
{
	// The inverse process of padding
	// Get rid of the tailed zeros after deciphering
	string erased_text;
	int pad_number = int(text[text.length() -1])-48;
	if ((pad_number > 0) && (pad_number < 8))
	{
		erased_text = text.substr(0, text.length() - pad_number);
	}
	else
	{
		erased_text = text;
	}
	return erased_text;
}

bitset<64> InitialPermutation(string text)
{
	string permuted_text;
	int IP_table[64] = {58,50,42,34,26,18,10,2,
						60,52,44,36,28,20,12,4,
						62,54,46,38,30,22,14,6,
						64,56,48,40,32,24,16,8,
						57,49,41,33,25,17,9,1,
						59,51,43,35,27,19,11,3,
						61,53,45,37,29,21,13,5,
						63,55,47,39,31,23,15,7};

	// Bitwise operation to detect every bit of text
	// << means bitwise moving to the left direction
	// 64-bit operating system will assign 64 bits to integers
	// Thus, bitwise operation on integers can collect bitwise information of text, which is a 8*8 bits character string
	bitset<64> bit_string;
	for (int digit = 0; digit < 64; digit++)
	{
		int detector = 1;
		int char_count= (64 - IP_table[digit]) / 8;
		for (int move = 0; move < (64 - IP_table[digit]) % 8; move++)
		{
			detector = detector << 1;
		}
		// text.data() is a pointer to 64 bits length character string
		// The same bit length will let character set transfer into integer smoothly.
		const char* pointer = text.data();
		if ((detector & int(*(pointer+char_count))) != 0)
		{
			bit_string.flip(63 - digit);
		}
	}
	return bit_string;
}

string FinalPermutation(bitset<64> bit_string)
{
	// Final permutation acutally is the inverse action of initial permutation
	string final_text;
	bitset<64> final_string;
	int FP_table[64] = {40,8,48,16,56,24,64,32,
						39,7,47,15,55,23,63,31,
						38,6,46,14,54,22,62,30,
						37,5,45,13,53,21,61,29,
						36,4,44,12,52,20,60,28,
						35,3,43,11,51,19,59,27,
						34,2,42,10,50,18,58,26,
						33,1,41,9,49,17,57,25 };
	for (int digit = 0; digit < 64; digit++)
	{
		if (bit_string[64 - FP_table[digit]] != 0)
		{
			final_string.flip(63 - digit);
		}
	}
	for (int char_count = 0; char_count < 8; char_count++)
	{
		bitset<8> temp_char;
		for (int char_digit = 0; char_digit < 8; char_digit++)
		{
			temp_char[char_digit] = final_string[char_count * 8 + char_digit];
		}
		// Use += to append char behind string
		final_text += char(temp_char.to_ulong());
	}
	return final_text;
}

bitset<56> PC1_Permutation(string key)
{
	int PC1_table[56] = {57,49,41,33,25,17,9,1,
						58,50,42,34,26,18,10,2,
						59,51,43,35,27,19,11,3,
						60,52,44,36,63,55,47,39,
						31,23,15,7,62,54,46,38,
						30,22,14,6,61,53,45,37,
						29,21,13,5,28,20,12,4 };
	bitset<56> PC1_key;
	for (int digit = 0; digit < 56; digit++)
	{
		int detector = 1;
		int char_count = (64 - PC1_table[digit]) / 8;
		for (int move = 0; move < (64 - PC1_table[digit]) % 8; move++)
		{
			detector = detector << 1;
		}
		// text.data() is a pointer to 64 bits length character string
		// The same bit length will let character set transfer into integer smoothly.
		const char* pointer = key.data();
		if ((detector & int(*(pointer + char_count))) != 0)
		{
			PC1_key.flip(55 - digit);
		}
	}
	return PC1_key;
}

bitset<56> PC1_BitPermutation(bitset<64> key)
{
	int PC1_table[56] = { 57,49,41,33,25,17,9,1,
						58,50,42,34,26,18,10,2,
						59,51,43,35,27,19,11,3,
						60,52,44,36,63,55,47,39,
						31,23,15,7,62,54,46,38,
						30,22,14,6,61,53,45,37,
						29,21,13,5,28,20,12,4 };
	bitset<56> PC1_key;
	for (int digit = 0; digit < 56; digit++)
	{
		if (key[64-PC1_table[digit]] == 1) 
		{
			PC1_key.flip(55 - digit);
		}
	}
	return PC1_key;
}

bitset<56> KeyTransformation(bitset<56> key, int iteration, bool encrypted)
{
	bitset<56> transformed_key;
	bitset<28> left_key, right_key;
	bitset<28> rotated_left, rotated_right;
	// Split the key for rotation
	for (int digit = 0; digit < 28; digit++)
	{
		if (key[digit + 28] != 0)
		{
			left_key.flip(digit);
		}
		if (key[digit] != 0)
		{
			right_key.flip(digit);
		}
	}

	if (!encrypted)
	{
		// Rotation under two circumstances in encryption process
		// In iteration=1,2,9,16 rotate left for 1 bit, while in other iterations rotate left for 2 bits
		if ((iteration == 0) || (iteration == 1) || (iteration == 8) || (iteration == 15))
		{
			for (int digit = 0; digit < 27; digit++)
			{
				if (left_key[digit] != 0)
				{
					rotated_left.flip(digit + 1);
				}
				if (right_key[digit] != 0)
				{
					rotated_right.flip(digit + 1);
				}
			}
			if (left_key[27] != 0)
			{
				rotated_left.flip(0);
			}
			if (right_key[27] != 0)
			{
				rotated_right.flip(0);
			}
		}
		else
		{
			for (int digit = 0; digit < 26; digit++)
			{
				if (left_key[digit] != 0)
				{
					rotated_left.flip(digit + 2);
				}
				if (right_key[digit] != 0)
				{
					rotated_right.flip(digit + 2);
				}
			}
			if (left_key[26] != 0)
			{
				rotated_left.flip(0);
			}
			if (right_key[26] != 0)
			{
				rotated_right.flip(0);
			}
			if (left_key[27] != 0)
			{
				rotated_left.flip(1);
			}
			if (right_key[27] != 0)
			{
				rotated_right.flip(1);
			}
		}
	}
	
	else 
	{
		// Rotation under two circumstances in decryption process
		// Here the input key should be initialized for inverse decrypting operation
		// In iteration=1,2,9,16 rotate right for 1 bit, while in other iterations rotate right for 2 bits
		if ((iteration == 15) || (iteration == 8) || (iteration==1) || (iteration == 0))
		{
			for (int digit = 1; digit < 28; digit++)
			{
				if (left_key[digit] != 0)
				{
					rotated_left.flip(digit - 1);
				}
				if (right_key[digit] != 0)
				{
					rotated_right.flip(digit - 1);
				}
			}
			if (left_key[0] != 0)
			{
				rotated_left.flip(27);
			}
			if (right_key[0] != 0)
			{
				rotated_right.flip(27);
			}
		}
		else
		{
			for (int digit = 2; digit < 28; digit++)
			{
				if (left_key[digit] != 0)
				{
					rotated_left.flip(digit - 2);
				}
				if (right_key[digit] != 0)
				{
					rotated_right.flip(digit - 2);
				}
			}
			if (left_key[1] != 0)
			{
				rotated_left.flip(27);
			}
			if (right_key[1] != 0)
			{
				rotated_right.flip(27);
			}
			if (left_key[0] != 0)
			{
				rotated_left.flip(26);
			}
			if (right_key[0] != 0)
			{
				rotated_right.flip(26);
			}
		}
	}
	

	// Merge the two halves together
	for (int digit = 0; digit < 56; digit++)
	{
		if ((digit < 28) && (rotated_right[digit] != 0))
		{
			transformed_key.flip(digit);
		}
		if ((digit >= 28) && (rotated_left[digit - 28] != 0))
		{
			transformed_key.flip(digit);
		}
	}
	return transformed_key;
}

bitset<48> PC2_Transformation(bitset<56> transformed_key)
{
	int PC2_table[48] ={14,17,11,24,1,5,3,28,
						15,6,21,10,23,19,12,4,
						26,8,16,7,27,20,13,2,
						41,52,31,37,47,55,30,40,
						51,45,33,48,44,49,39,56,
						34,53,46,42,50,36,29,32};
	bitset<48> PC2_key;
	for (int digit = 0; digit < 48; digit++)
	{
		if (transformed_key[56 - PC2_table[digit]] != 0)
		{
			PC2_key.flip(47 - digit);
		}
	}
	return PC2_key;
}

bitset<32> SplitLeft(bitset<64> text)
{
	// Split 63-32 bit from the 64-bit string
	bitset<32> left;
	for (int digit = 32; digit < 64; digit++)
	{
		if (text[digit] != 0)
		{
			left.flip(digit -32);
		}
	}
	return left;
}

bitset<32> SplitRight(bitset<64> text)
{
	// Split 31-0 bit from the 64-bit string
	bitset<32> right;
	for (int digit = 0; digit < 32; digit++)
	{
		if (text[digit] != 0)
		{
			right.flip(digit);
		}
	}
	return right;
}

bitset<32> F_Transformation(bitset<32> right, bitset<48> PC2_key)
{
	bitset<48> expanded_text;
	bitset<48> xor_text;
	bitset<32> merged_text;
	bitset<32> permuted_text;

	// Expansion step, expand 32 bits into 48 bits
	int expansion_table[48] = { 32,1,2,3,4,5,
								4,5,6,7,8,9,
								8,9,10,11,12,13,
								12,13,14,15,16,17,
								16,17,18,19,20,21,
								20,21,22,23,24,25,
								24,25,26,27,28,29,
								28,29,30,31,32,1 };
	for (int digit = 0; digit < 48; digit++)
	{
		if (right[32 - expansion_table[digit]] != 0)
		{
			expanded_text.flip(47 - digit);
		}
	}

	// XOR step, xor the expanded text with key
	xor_text = expanded_text ^= PC2_key;

	// S box step, shrink 48 bits to 32 bits
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

	for (int box_count = 0; box_count < 8; box_count++)
	{
		bitset<6> S_box;
		int row, column, number, output;
		for (int digit = (7 - box_count) * 6; digit < (7 - box_count) * 6 + 6; digit++) 
		{
			if (xor_text[digit] != 0)
			{
				S_box.flip(digit % 6);
			}
		}
		
		row = S_box[5] * 2 + S_box[0];
		column = S_box[4] * 8 + S_box[3] * 4 + S_box[2] * 2 + S_box[1];
		number = row * 16 + column;
		output = S_box_table[box_count][number];
		bitset<4> out_box(output);
		for (int digit = (7 - box_count) * 4; digit < (7 - box_count) * 4 + 4; digit++)
		{
			if (out_box[digit % 4] != 0) 
			{
				merged_text.flip(digit);
			}
		}
	}

	// Permutation step
	int P_table[32] = { 16,7,20,21,29,12,28,17,
						1,15,23,26,5,18,31,10,
						2,8,24,14,32,27,3,9,
						19,13,30,6,22,11,4,25 };
	for (int digit = 0; digit < 32; digit++)
	{
		if (merged_text[32 - P_table[digit]] != 0)
		{
			permuted_text.flip(31 - digit);
		}
	}
	return permuted_text;
}

bitset<32> XorTransformation(bitset<32> left,bitset<32> right)
{
	bitset<32> xor_left;
	xor_left = left ^ right;
	return xor_left;
}

bitset<64> SwapLeftAndRight(bitset<32> left,bitset<32> right)
{
	bitset<64> text;
	for (int digit = 0; digit < 64; digit++)
	{
		if ((digit < 32) && (left[digit] != 0))
		{
			text.flip(digit);
		}
		else if ((digit >= 32) && (right[digit - 32] != 0))
		{
			text.flip(digit);
		}
	}
	return text;
}

bitset<56> InitializeKey(bitset<56> transformed_key, int max_iteration)
{
	for (int iteration = 0; iteration < max_iteration; iteration++)
	{
		transformed_key = KeyTransformation(transformed_key, iteration, false);
	}
	return transformed_key;
}

// The macro-structure of DES algorithm
string MessageProcess(string text, string key, int max_iteration, bool verbose_flag, bool encrypted)
{
	string pad_text, temp_text, temp_result, result;
	bitset<64> bit_string;
	bitset<56> transformed_key;
	bitset<48> PC2_key;
	bitset<32> left, right, temp_right;
	long pad_length;
	// The encrypting process referring to the key steps in the textbook
	// 1 byte = 8 bits, thus every 8 byte is regarded as 1 block
	if (!encrypted)
	{
		pad_length = (text.length() / 8 + 1) * 8 - text.length();
		pad_text = Padding(text, pad_length);
	}
	else
	{
		pad_text = text;
	}
	for (int block_count = 0; block_count < pad_text.length() / 8; block_count++)
	{
		if (!verbose_flag)
		{
			temp_text.assign(pad_text.begin() + block_count * 8, pad_text.begin() + (block_count + 1) * 8);
			bit_string = InitialPermutation(temp_text);
			transformed_key = PC1_Permutation(key);
			if (!encrypted)
			{
				for (int iteration = 0; iteration < max_iteration; iteration++)
				{
					transformed_key = KeyTransformation(transformed_key, iteration, encrypted);
					PC2_key = PC2_Transformation(transformed_key);
					left = SplitLeft(bit_string);
					right = SplitRight(bit_string);
					temp_right = F_Transformation(right, PC2_key);
					left = XorTransformation(left, temp_right);
					bit_string = SwapLeftAndRight(left, right);
				}
				left = SplitLeft(bit_string);
				right = SplitRight(bit_string);
				bit_string = SwapLeftAndRight(left, right);
				temp_text = FinalPermutation(bit_string);
				temp_result.append(temp_text);
			}
			else
			{
				// Key initialization is required since max_iteration is not fixed to 16
				transformed_key = InitializeKey(transformed_key, max_iteration);
				// inverse the iteration in KeyTransformation input
				for (int iteration = max_iteration-1; iteration >=0; iteration--)
				{
					PC2_key = PC2_Transformation(transformed_key);
					left = SplitLeft(bit_string);
					right = SplitRight(bit_string);
					temp_right = F_Transformation(right, PC2_key);
					left = XorTransformation(left, temp_right);
					bit_string = SwapLeftAndRight(left, right);
					transformed_key = KeyTransformation(transformed_key, iteration, encrypted);
				}
				left = SplitLeft(bit_string);
				right = SplitRight(bit_string);
				bit_string = SwapLeftAndRight(left, right);
				temp_text = FinalPermutation(bit_string);
				temp_result.append(temp_text);
			}
		}
	}
	if (encrypted)
	{
		result = Erasing(temp_result);
		return result;
	}
	else 
	{
		return temp_result;
	}
	
}
