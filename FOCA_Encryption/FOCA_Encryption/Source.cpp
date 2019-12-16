//Alice Chambers, SE1, Encryption Assignment

char EKey = 'Z';

#define StudentName "Alice Chambers"

#define MAXCHARS 6    // 6 is the minimum

using namespace std;
#include <string>               // for strings
#include <fstream>              // file I/O
#include <iostream>             // for cin >> and cout <<
#include <iomanip>              // for fancy output
#include "TimeUtils.h"          // for GetTime, GetDate, etc.

#define dollarchar '$'          // string terminator

char EncryptionData[3][MAXCHARS];  // Encryption data 2D array

//----------------------------- C++ Functions ----------------------------------------------------------

void get_char(char& a_character)
{

startWhile:											//label for startWhile
	a_character = (char)_getwche();					//get next char

	__asm
	{
	whileContent:										//label for whileContent

		cmp  eax, '\r'								//compare EAX with '\r'
			je endWhile									//jump to endWhile if equal 
			cmp  eax, '\n'								//compare EAX with '\n'
			je endWhile									//jump to endWhile if equal 
			cmp  eax, '$'								//compare EAX to '$' - dollarchar
			je endWhile									//jump to endWhile if equal
			cmp eax, '0'								//compare EAX to '0'
			jl warnUser									//jump to warnUser if less than
			cmp eax, 'z'								//compare EAX to 'z'
			jg warnUser									//jump to warnUser if less than

			endWhile :									//endWhile label
	}
	return;
warnUser:										//warnUser label
	cout << endl << "Alphanumeric characters only, please try again > " << endl;

	__asm
	{
		jmp startWhile									//jump to startWhile (unconditional)
	}
}

void get_original_chars(int& length)
{
	char next_char = ' ';
	length = 0;
	get_char(next_char);


	while ((length < MAXCHARS) && (next_char != '\n') && (next_char != '\r') && (next_char != dollarchar))
	{
		EncryptionData[0][length++] = next_char;
		get_char(next_char);
	}

}

//---------------------------------------------------------------------------------------------------------------
//----------------- ENCRYPTION ROUTINES -------------------------------------------------------------------------

void encrypt_chars(int length, char EKey)
{
	//code to encrypt the characters in the user inputted string, one by one with use of a for loop

	char temp_char;                           // Character temporary store

	for (int i = 0; i < length; i++)          // Encrypt characters one at a time
	{
		temp_char = EncryptionData[0][i];     // copy the char from Original Chars array to temp_char

		__asm
		{

			push   edx                       // Saves the current state of the register to the stack. Able to use the register without issues
			push   eax                       // Saves the current state of the register to the stack. Able to use the register without issues

			lea    eax, EKey                 // Load Effective Address of EKey and stores the address in EAX (stores the location of EKey)

			mov    edx, [eax]	             // [EAX] the brackets suggests eax contains an address, this is the EKey value address stored earlier. Then copy the value of EKey into EDX
											 // The EKey is being manipulated here
			and edx, 0x000000A7		     // ands 0x000000A7 with EDX and stores the result in EDX e.g. b0101 1010 AND b1010 0111 = b0000 0010 
			rol    dl, 1				     // rotates the low 8 bit field in EDX left by 1
			add    edx, 0x03			     // adds EDX to 0x03 and stores the result in EDX   
			mov[eax], edx				 // copies the EKey value stored in EDX earlier back into EAX

			movzx  eax, temp_char            // zeroise the register and copy the value of temp_char
																			  // The char is being manipulated here
			or eax, 0x20				 // OR's 20 to EAX to convert uppercase letter to lowercase
			add    eax, edx			         // adds the manipulated EKey value in EDX to EAX and stores the result in EAX
			ror    al, 2				     // rotates the low 8 bit field in EAX right by 2
			add    eax, 0x10			     // adds EAX to 0x10 and stores the result in EAX

			mov   temp_char, al              // copies the low 8 bits (the first 8 bits) of the EAX register and stores them in temp_char

			pop	  eax						 // Restores the previous state of the register
			pop   edx                        // Restores the previous state of the register
		}
		EncryptionData[1][i] = temp_char;    // Store the encrypted char (in temp_char) in the Encrypted Chars array
	}
	return;
}


//---------------------------------------------------------------------------------------------------------------
//----------------- DECRYPTION ROUTINES -------------------------------------------------------------------------
//
void decrypt_chars(int length, char EKey)
{
	char temp_char;							  // Character temporary store

	for (int i = 0; i < length; i++)		  // Encrypt characters one at a time
	{
		temp_char = EncryptionData[1][i];     // Get the next char from Original Chars array

		__asm
		{
			push   eax                       // Saves the current state of the register to the stack. Able to use the register without issues
			push   ecx                       // Saves the current state of the register to the stack. Able to use the register without issues
			push   edx                       // Saves the current state of the register to the stack. Able to use the register without issues

			lea    eax, EKey                 // Load Effective Address of EKey and stores in EAX (stores the location of EKey)

			movzx ecx, temp_char			 // zeroise the register and copy the value of temp_char

			push  ecx					     // Saves the current state of the register to the stack. Able to use the register without issues
			mov   ecx, [eax]	             // [EAX] the brackets suggests eax contains an address, this is the EKey value address stored earlier. Then copy the value of EKey into ECX
			and ecx, 0x000000A7		         // ands the address 0x000000A7 with ECX and stores the result in ECX e.g. b0101 1010 AND b1010 0111 = b0000 0010 
			rol   cl, 1				         // rotates the low 8 bit field of ECX left by 1
			add   ecx, 0x03			         // adds 0x03 to ECX and stores the result in ECX  

			mov   edx, ecx			         // copy whats in ECX and stores it in EDX
			pop   ecx					     // pops the current value off the stack and back into the register
			sub   ecx, 0x10			         // subtracts ECX to 0x10 and stores the result in ECX
			rol   cl, 2				         // rotates the low 8 bit field in ECX right by 2
			mov[eax], edx				     // copies the EKey value stored in the memory address from EDX and moves it to EAX
			sub   ecx, edx			         // subtracts EDX to ECX and stores the result in ECX
			mov   eax, ecx			         // copies the character to be encrypted from ECX into EAX

			mov    temp_char, al             // takes the low 8 bits (the first 8 bits) of the EDX register and cpoies them to temp_char

			pop    edx                       // Restores the previous state of the register
			pop    ecx                       // Restores the previous state of the register
			pop    eax                       // Restores the previous state of the register

		}
		EncryptionData[2][i] = temp_char;    // Store encrypted char in the Encrypted Chars array
	}
	return;

}

int main(void)
{
	int char_count(0);  // The number of actual characters entered (upto MAXCHARS limit).

	cout << "\nPlease enter upto " << MAXCHARS << " alphanumeric characters:  ";
	get_original_chars(char_count);

	ofstream EDump;
	EDump.open("EncryptDump.txt", ios::app);
	EDump << "\n\nFoCA Encryption program results (" << StudentName << ") Encryption key = '" << EKey << "'";
	EDump << "\nDate: " << GetDate() << "  Time: " << GetTime();

	// Display and save initial string
	cout << "\n\nOriginal string =  " << EncryptionData[0] << "\tHex = ";
	EDump << "\n\nOriginal string =  " << EncryptionData[0] << "\tHex = ";
	for (int i = 0; i < char_count; i++)
	{
		cout << hex << setw(2) << setfill('0') << ((int(EncryptionData[0][i])) & 0xFF) << "  ";
		EDump << hex << setw(2) << setfill('0') << ((int(EncryptionData[0][i])) & 0xFF) << "  ";
	};

	// Encrypt the string and display/save the result
	encrypt_chars(char_count, EKey);

	cout << "\n\nEncrypted string = " << EncryptionData[1] << "\tHex = ";
	EDump << "\n\nEncrypted string = " << EncryptionData[1] << "\tHex = ";
	for (int i = 0; i < char_count; i++)
	{
		cout << ((int(EncryptionData[1][i])) & 0xFF) << "  ";
		EDump << ((int(EncryptionData[1][i])) & 0xFF) << "  ";
	}

	// Decrypt the encrypted string and display/save the result
	decrypt_chars(char_count, EKey);

	cout << "\n\nDecrypted string = " << EncryptionData[2] << "\tHex = ";
	EDump << "\n\nDecrypted string = " << EncryptionData[2] << "\tHex = ";
	for (int i = 0; i < char_count; i++)
	{
		cout << ((int(EncryptionData[2][i])) & 0xFF) << "  ";
		EDump << ((int(EncryptionData[2][i])) & 0xFF) << "  ";
	}

	cout << "\n\n\n";
	EDump << "\n\n-------------------------------------------------------------";
	EDump.close();

	system("PAUSE");
	return (0);
} // end of whole encryption/decryption program --------------------------------------------------------------------


