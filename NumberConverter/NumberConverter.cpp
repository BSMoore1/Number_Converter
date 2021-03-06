//Group G
//Brady Moore T00605919
//
//
//

//There might be a couple of obsolete lines of code left over
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <vector>

using namespace std;

struct Converter {				//Converts number of orginal_base to a decimal internally, then converts that decimal into a number of a target_base
								//Sequence of function calls is as follows: prompt_user > validate_response > convert_number_to_decimal > convert_decimal_to_new_base

	int original_base;		//The base of the number you're converting
	int target_base;			//The base you want your number converted to
	int decimal_result;		//The decimal equivalent of the number given
	string user_input;			//Users input, to convert to vector<char> inside each function that uses it

	void convert_number_to_decimal(int number_base, vector<char> input_number);	//Converts user_input into decimal
	int convert_digit_to_decimal(char alphanumeral);							//Converts char into an int (ex: A = 10)
	char convert_digit_to_letter(int numeral);									//Converts in into char (ex: 10 = A)
	string	convert_decimal_to_new_base(int number_base);						//Converts decimal_result into number of target_base
	void prompt_user();															//UI- get original_base, user_input, target_base from user. Should probably be in a seperate class for modularity
private:
	void validate_response();													//Gives an error if user_input doesn't match original_base (ex: original_base = 10, user_input = 1af4) Should probably be in a seperate class for modularity
};

char Converter::convert_digit_to_letter(int alphanumeral) {
	char number;
	alphanumeral = 'A' + alphanumeral - 10;
	number = alphanumeral;
	return number;
};

int Converter::convert_digit_to_decimal(char alphanumeral) {
	int decimal = alphanumeral - '0';

	 if (isupper(alphanumeral))					//Because 'a' and 'A' have different ASCII values
			decimal = alphanumeral - 55;

	else if (islower(alphanumeral))
			decimal = alphanumeral - 87;

	return decimal;
};

void Converter::convert_number_to_decimal(int number_base, vector<char> input_number) {
	decimal_result = 0;
	vector<char> array_of_digits = input_number;

		for (int i = array_of_digits.size()-1; i >= 0; i--) {						//Parse through vector<char> version of user_input, converting each digit into decimal with convert_digit_to_decimal
			array_of_digits[i] = convert_digit_to_decimal(input_number[i]);
			decimal_result += (array_of_digits[i] * pow(number_base, (array_of_digits.size() - i - 1)));
		}
	return;
};

string Converter::convert_decimal_to_new_base(int number_base) {
	string number_result;
	int exponent = 0;
	int new_number = decimal_result; 

	for (int i = 0; pow(target_base, i) <= decimal_result; i++) {							//Determines highest exponent in decimal_result (ex: in 4A03, highest exponent is 3)
		exponent = i;
	}
	for (int i = exponent, j = 1; exponent >= 0; i--) {										//Parse through each digit of decimal_result by reducing exponent by 1 each interation
		exponent = i;
		while (j * pow(number_base, exponent) <= new_number && j <= target_base)			//Determine highest number each digit should be, without it being over the target_base or it being higher than the user's number (ex: Converting decimal 158 to a base of 3 is 12212, so no digit should be over 2. Highest exponent in 12212 converted to base 3 is 4, but 3 * (3^4) = 243 which is higher than 158. So decrement 3(j) until you get 1(3^4) which is lower than 158.)
			j++;
		new_number -= ((j-1) * pow(target_base, exponent));									//The remainder of decimal_result that needs to be represented (ex: Decimal 158 in base of 3 = 12212, 1 * (3^4) = 81. 158-81 = 77, which should be represented by the digits of exponents less than 4.)
		if ((j - 1) >= 10) {																//If the number is 10 or over, it should be a letter (ex: 11 should be represented as 'B'). Adds that digit to the end of number_result.
			number_result += convert_digit_to_letter(j - 1);								
		}
		else if ((j - 1) == 0 && exponent == -1)											//This prevents program putting a 0 at the end of every number
			break;
		else if ((j - 1) < 10) {															//Adds the determined digit to the end of number_result
 			number_result += to_string(j - 1);
		}

		j = 1;
	}

	return number_result;
};

void Converter::validate_response() {
	vector<char> input_number(user_input.begin(), user_input.end());

	for (int i = input_number.size() - 1; i >= 0; i--) {
		if (original_base < convert_digit_to_decimal(input_number[i])) {
			cout << "The number you had entered contains digits that don't match the base of " << original_base << ", try again.\n\n";
			prompt_user();			//Reprompt user if their number given doesnt match the base they gave
		}
	}
};

void Converter::prompt_user() {

		cout << "Type in the base (in decimal) for your number." << endl;
		cin >> original_base;
		cout << "Type in your number to be converted." << endl;
		cin >> user_input;
		cout << "Type in the base (in decimal) you would like it converted to." << endl;
		cin >> target_base;
		validate_response();

		vector<char> input_number(user_input.begin(), user_input.end());
		convert_number_to_decimal(original_base, input_number);

	return;
};


int main()
{
	string inputs;
	string output;
	Converter Convert;

	cout << "This program converts an integer of any positive base to another integer of any positive base.\n\n";
	do {
		Convert.prompt_user();
		vector<char> input_number(Convert.user_input.begin(), Convert.user_input.end());
		output = Convert.convert_decimal_to_new_base(Convert.target_base);
		cout << endl << Convert.user_input << " in base " << Convert.original_base << " is equivalent to " << output << " in base " << Convert.target_base << ".\n";
		cout << "\n\n Another conversion? (y)\n";
		getline(cin, inputs);
		getline(cin, inputs);
	} while (inputs == "y" || inputs == "Y");

    return 0;
}

