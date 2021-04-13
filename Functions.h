#include <iostream>
#include <vector>
#include <string>
//
bool IfNumber(char str)
{
	if (str - '0' < 10 && str - '0' > -1)
	{
		return true;
	}
	return false;
}
//
double Std(char symbol)
{
	return (symbol - '0');
}
//
double StringToDouble(std::string number, int size)
{
	if (size == 1)
	{
		return Std(number[0]);
	}
	int sizeik = size - 1;
	double final = 0;
	double afterDot = 0;
	int count = 1;
	final = Std(number[0]);
	for (int i = 1; i < size; i++)
	{
		if (number[i] == '.')
		{
			sizeik--;
			count++;
			break;
		}
		if (IfNumber(number[i]))
		{
			final = final * 10 + Std(number[i]);
			sizeik--;
			count++;
		}
		else
		{
			std::cout << "Invalid value" << std::endl;
			return 0;
		}

	}
	if (sizeik > 0)
	{
		for (int i = count; i < size; i++)
		{
			afterDot = afterDot * 10 + Std(number[i]);
		}
	}
	while (sizeik != 0)
	{
		afterDot /= 10;
		sizeik--;
	}
	final += afterDot;
	return final;
}
//
int StringToInt(std::string number, int size)
{
	if (size == 1)
	{
		return Std(number[0]);
	}
	int final = 0;
	final = Std(number[0]);
	for (int i = 1; i < size; i++)
	{
		if (number[i] == '.')
		{
			return final;
		}
		if (IfNumber(number[i]))
		{
			final = final * 10 + Std(number[i]);

		}
		else
		{
			std::cout << "Invalid value" << std::endl;
			return NULL;
		}
	}
	return final;
}
//
bool IfLess0(int number, int size)
{
	return number == (size - 1);

}



void HelpMenu()
{
	std::cout << " Corsa V 1.0" << std::endl << std::endl;
	std::cout << " Variable Declaration`" << std::endl;
	std::cout << " - Intero  for integers." << std::endl;
	std::cout << " - Vero    for real numbers." << std::endl;
	std::cout << " - Corda   for text." << std::endl;
	std::cout << " - Verum   for boolean values.\n" << std::endl;
	std::cout << " To change variable value write 'Ingresso" << std::endl;
	std::cout << " To print something write 'Tampa'" << std::endl;
	std::cout << " To print endl write 'Riga'" << std::endl;
	std::cout << " For using if statement write 'Caso'\n" << std::endl << std::endl;
	std::cout << " Thank you for choosing our services.\n We are working on a new update of the program." << std::endl;
	return;
}
