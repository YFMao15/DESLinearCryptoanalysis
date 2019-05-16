// Functions used in multiple methods
// Be aware of binary input and output, since the file could possibly contain '\0'
// If not using binary operations, then output file could bu distorted while saving
// Every .cpp file needs stdafx, since it is a MFC program

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cassert>
#include <string>
#include "UtilFunction.h"

using namespace std;

string ReadDocument(string file_path)
{
	string doc_message;
	// Read the document from the full path provided
	ifstream infile(file_path.data(),ifstream::binary);
	// Program terminated if target document fails 
	if (infile.is_open())
	{
		stringstream buffer;
		buffer << infile.rdbuf();
		string doc_message(buffer.str());
		return doc_message;
	}
	else
	{
		cout << "File not found." << endl;
		return "";
	}
}
	

bool SaveDocument(string file_path,string message)
{
	ofstream outfile(file_path.data(), ofstream::binary);
	if (outfile.is_open())
	{
		outfile << message;
		outfile.close();
		return true;
	}
	else
	{
		cout << "Output file not created." << endl;
		return false;
	}
}
