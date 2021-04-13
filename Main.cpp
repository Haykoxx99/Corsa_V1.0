#include "Functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// chmoranal stexcel help menu

struct VarVero
{
	std::string name;
	double value;
};
struct VarIntero
{
	std::string name;
	int value;
};
struct VarCorda
{
	std::string name;
	std::string value;
};
struct VarVerum
{
	std::string name;
	bool value;
};


// Function of Condition and variable finder
template <typename Type>
int IsType(const std::vector<Type>& type, std::string name)
{
	int size = type.size();
	for (int i = 0; i < size; i++)
	{
		if (name == type[i].name)
		{
			return i;
		}
	}
	return -1;
}
//
double ValueFinder(std::string Line,const std::vector<VarIntero>& Int,const std::vector<VarVero>& Double,bool& bug)
{
	double result = NULL;
	int size = Line.size();
	//if number
	if (IfNumber(Line[0]))
	{
		if (size == 1 && Line[0] == '0')
		{
			return 0;
		}
		else if (size > 1 && Line[0] == '0')
		{
			bug = true;
			return NULL;
		}
		for (int i = 0; i < size; i++)
		{
			if (!(IfNumber(Line[i])))
			{
				bug = true;
				return NULL;
			}
		}
		result = StringToDouble(Line, size);
		return result;
	}
	//if variable
	int index = IsType<VarIntero>(Int, Line);
	if (index != -1)
	{
		result = Int[index].value;
	}
	else
	{
		index = IsType<VarVero>(Double, Line);
		if (index != -1)
		{
			result = Double[index].value;
		}
	}
	if (index == -1)
	{
		bug = true;
	}
	return result;
}
void Condition(std::string Line, const std::vector<VarIntero>& Int, const std::vector<VarVero>& Double, const std::vector<VarVerum>& Bool, bool& bug, bool& result)
{
	std::vector<std::string> temp;
	int size = Line.size();
	std::string tempLine;
	for (int i = 1; i < size; i++)
	{
		if (Line[i] == ' ' || Line[i] == ')')
		{
			temp.push_back(tempLine);
			tempLine = "";
		}
		else
		{
			tempLine += Line[i];
		}
	}
	size = temp.size();
	if (size < 1 || size > 3 || size == 2)
	{
		bug = true;
		return;
	}
	if (size == 1)
	{
		if (IfNumber(temp[0][0]))
		{
			int smallSize = temp[0].size();
			if (smallSize == 1 && temp[0][0] == '0')
			{
				result = false;
				return;
			}
			else if (smallSize > 1 && temp[0][0] == '0')
			{
				bug = true;
				return;
			}
			for (int i = 0; i < size; i++)
			{
				if (!(IfNumber(temp[0][i])))
				{
					bug = true;
					return;
				}
			}
			result = true;
			return;
		}
		int index = IsType<VarVerum>(Bool, temp[0]);
		if (index == -1)
		{
			index = IsType<VarIntero>(Int, temp[0]);
			if (index == -1)
			{
				index = IsType<VarVero>(Double, temp[0]);
				if (index != -1)
				{
					if (Double[index].value != 0)
					{
						result = true;
						return;
					}
					else
					{
						result = false;
						return;
					}
				}
			}
			if (index != -1)
			{
				if (Int[index].value != 0)
				{
					result = true;
					return;
				}
				else
				{
					result = false;
					return;
				}
			}
		}
		if (index != -1)
		{
			if (Bool[index].value == true)
			{
				result = true;
				return;
			}
			else
			{
				result = false;
				return;
			}
		}
		bug = true;
		return;
	}
	if (temp[1].size() != 1)
	{
		bug = true;
		return;
	}
	if (size == 3)
	{
		if (temp[1][0] == '>')
		{
			double value1 = ValueFinder(temp[0], Int, Double, bug);
			if (bug)
			{
				return;
			}
			double value2 = ValueFinder(temp[2], Int, Double, bug);
			if (bug)
			{
				return;
			}
			result = value1 > value2;
			return;
		}
		if (temp[1][0] == '<')
		{
			double value1 = ValueFinder(temp[0], Int, Double, bug);
			if (bug)
			{
				return;
			}
			double value2 = ValueFinder(temp[2], Int, Double, bug);
			if (bug)
			{
				return;
			}
			result = value1 < value2;
			return;
		}
		if (temp[1][0] == '=')
		{
			double value1 = ValueFinder(temp[0], Int, Double, bug);
			if (bug)
			{
				return;
			}
			double value2 = ValueFinder(temp[2], Int, Double, bug);
			if (bug)
			{
				return;
			}
			result = value1 == value2;
			return;
		}
		if (temp[1][0] == '!')
		{
			double value1 = ValueFinder(temp[0], Int, Double, bug);
			if (bug)
			{
				return;
			}
			double value2 = ValueFinder(temp[2], Int, Double, bug);
			if (bug)
			{
				return;
			}
			result = value1 != value2;
			return;
		}
	}
	bug = true;
	return;
}

int main()
{
	//Opening file
	std::string path = "Instructions.txt";
	std::ifstream file;
	while (true)
	{
		file.open(path, std::fstream::in);
		static short count = 0;
		if (count == 3)
		{
			std::cout << "Bye!" << std::endl;
			return 0;
		}
		if (!file.is_open())
		{
			std::cout << "Please put true path!" << std::endl;
			std::cin >> path;
			count++;
		}
		else
		{
			break;
		}
	}

	//Declarations

	std::string Line = "";
	std::vector<VarVero> VarDouble;
	std::vector<VarIntero> VarInt;
	std::vector<VarCorda> VarString;
	std::vector<VarVerum> VarBool;
	std::vector<std::string> ActiveLine;
	std::vector<std::string> AllLines;

	//reading file
	while (!file.eof())
	{
		std::getline(file, Line);
		if (Line == "")
		{
			continue;
		}
		AllLines.push_back(Line);
	}
	file.close();

	for (int i = 0; i < AllLines.size(); i++)
	{
		//reading line
		std::string thisLine = AllLines[i];
		Line = "";

		// Creating ActiveLine
		int bigSize = thisLine.size();
		if (thisLine[0] == '{' || thisLine[0] == '}')
		{
			if (bigSize != 1)
			{
				system("CLA");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			else
			{
				continue;
			}
		}
		for (int j = 0; j < bigSize; j++)
		{
			if (thisLine[bigSize - 1] != ';')
			{
				system("CLS");
				std::cout << "Expected ; in line " << i << std::endl;
				return 0;
			}
			if (thisLine[j] == '(' && thisLine[bigSize - 2] == ')')
			{
				for (; j < bigSize; j++)
				{
					if (thisLine[j] == ';')
					{
						ActiveLine.push_back(Line);
						Line = "";
					}
					else
					{
						Line += thisLine[j];
					}
				}
				continue;
			}
			if (thisLine[j] == ' ' || thisLine[j] == ';')
			{
				ActiveLine.push_back(Line);
				Line = "";
			}
			else
			{
				Line += thisLine[j];
			}
		}
		
		//Analizing ActiveLine

		if (i == 0 && ActiveLine[0] == "Help")
		{
			HelpMenu();
			return 0;
		}
		//Variable declaration

		//If variable is Double
		if (ActiveLine[0] == "Vero")
		{
			if (ActiveLine.size() < 2)
			{
				system("CLS");
				std::cout << "In line " << i << " Your declared variable don't have name!" << std::endl;
				return 0;
			}
			VarVero variable;
			for (int j = 1; j < ActiveLine.size(); j++)
			{
				//Name declaration
				if (j == 1)
				{
					//if variable starts with number
					if (IfNumber(ActiveLine[j][0]))
					{
						system("CLS");
						std::cout << "Variable can't start with number!" << std::endl;
						return 0;
					}
					//if there is another variable by that name
					for (int s = 0; s < VarDouble.size(); s++)
					{
						if (VarDouble[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarInt.size(); s++)
					{
						if (VarInt[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarBool.size(); s++)
					{
						if (VarBool[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarString.size(); s++)
					{
						if (VarString[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					//if all goes well it will be declared variable
					variable.name = ActiveLine[j];
					if (ActiveLine.size() <= 2)
					{
						break;
					}
					continue;
				}
				//check =
				if (ActiveLine[j] == "=")
				{
					if (ActiveLine.size() == j + 1)
					{
						system("CLS");
						std::cout << "In line " << i << " Syntax error!" << std::endl;
						return 0;
					}
					if (!(IfNumber(ActiveLine[j+1][0])))
					{
						int l = 0;
						for (int s = 0; s < VarDouble.size(); s++)
						{
							if (VarDouble[s].name == ActiveLine[j + 1])
							{
								variable.value = VarDouble[s].value;
								j++;
								break;
							}
							l++;
						}
						if (l == VarDouble.size())
						{
							system("CLS");
							std::cout << "Undeclared variable name in line " << i << std::endl;
							return 0;
						}
					}
					else
					{
						variable.value = StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size());
						j++;
					}
					continue;
				}

				// + - * /
				if (ActiveLine.size() > j + 1)
				{
					// +
					if (ActiveLine[j] == "+")
					{
						if (!(IfNumber(ActiveLine[j + 1][0])))
						{
							int l = 0;
							for (int s = 0; s < VarDouble.size(); s++)
							{
								if (VarDouble[s].name == ActiveLine[j + 1])
								{
									variable.value += VarDouble[s].value;
									j++;
									break;
								}
							}
							if (l == VarDouble.size())
							{
								system("CLS");
								std::cout << "Undeclared variable name in line " << i << std::endl;
								return 0;
							}
						}
						else
						{
							variable.value += StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size());
							j++;
						}
						continue;
					}
					// -
					if (ActiveLine[j] == "-")
					{
						if (!(IfNumber(ActiveLine[j + 1][0])))
						{
							int l = 0;
							for (int s = 0; s < VarDouble.size(); s++)
							{
								if (VarDouble[s].name == ActiveLine[j + 1])
								{
									variable.value -= VarDouble[s].value;
									j++;
									break;
								}
							}
							if (l == VarDouble.size())
							{
								system("CLS");
								std::cout << "Undeclared variable name in line " << i << std::endl;
								return 0;
							}
						}
						else
						{
							variable.value -= StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size());
							j++;
						}
						continue;
					}
					// *
					if (ActiveLine[j] == "*")
					{
						if (!(IfNumber(ActiveLine[j + 1][0])))
						{
							int l = 0;
							for (int s = 0; s < VarDouble.size(); s++)
							{
								if (VarDouble[s].name == ActiveLine[j + 1])
								{
									variable.value *= VarDouble[s].value;
									j++;
									break;
								}
							}
							if (l == VarDouble.size())
							{
								system("CLS");
								std::cout << "Undeclared variable name in line " << i << std::endl;
								return 0;
							}
						}
						else
						{
							variable.value *= StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size());
							j++;
						}
						continue;
					}
					// /
					if (ActiveLine[j] == "/")
					{
						if (!(IfNumber(ActiveLine[j + 1][0])))
						{
							int l = 0;
							for (int s = 0; s < VarDouble.size(); s++)
							{
								if (VarDouble[s].name == ActiveLine[j + 1])
								{
									if (VarDouble[s].value == 0)
									{
										system("CLS");
										std::cout << "You can't divide in 0!" << std::endl;
										return 0;
									}
									variable.value /= VarDouble[s].value;
									j++;
									break;
								}
							}
							if (l == VarDouble.size())
							{
								system("CLS");
								std::cout << "Undeclared variable name in line " << i << std::endl;
								return 0;
							}
						}
						else
						{
							if (StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size()) == 0)
							{
								system("CLS");
								std::cout << "You can't divide in 0!" << std::endl;
								return 0;
							}
							variable.value /= StringToDouble(ActiveLine[j + 1], ActiveLine[j + 1].size());
							j++;
						}
						continue;
					}
				}
				else
				{
					system("CLS");
					std::cout << "In line " << i << " Syntax error! " << "[ " << ActiveLine[j] << " ]" << std::endl;
					return 0;
				}
			}
			VarDouble.push_back(variable);
			ActiveLine.clear();
			continue;
		}

		// If variable is Int
		if (ActiveLine[0] == "Intero")
		{
			if (ActiveLine.size() < 2)
			{
				system("CLS");
				std::cout << "In line " << i << " Your declared variable don't have name!" << std::endl;
				return 0;
			}
			VarIntero variable;
			for (int j = 1; j < ActiveLine.size(); j++)
			{
				//Name declaration
				if (j == 1)
				{
					if (IfNumber(ActiveLine[j][0]))
					{
						system("CLS");
						std::cout << "Variable can't start with number!" << std::endl;
						return 0;
					}
					for (int s = 0; s < VarDouble.size(); s++)
					{
						if (VarDouble[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarInt.size(); s++)
					{
						if (VarInt[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarBool.size(); s++)
					{
						if (VarBool[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					for (int s = 0; s < VarString.size(); s++)
					{
						if (VarString[s].name == ActiveLine[j])
						{
							system("CLS");
							std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
							return 0;
						}
					}
					variable.name = ActiveLine[j];
					if (ActiveLine.size() <= 2)
					{
						break;
					}
					continue;
				}
				//check =
				if (ActiveLine[j] == "=")
				{
					if (ActiveLine.size() == j + 1)
					{
						system("CLS");
						std::cout << "In line " << i << " Syntax error!" << std::endl;
						return 0;
					}
					if (!(IfNumber(ActiveLine[j + 1][0])))
					{
						int l = 0;
						for (int s = 0; s < VarInt.size(); s++)
						{
							if (VarInt[s].name == ActiveLine[j + 1])
							{
								variable.value = VarInt[s].value;
								j++;
								break;
							}
							l++;
						}
						if (l == VarInt.size())
						{
							system("CLS");
							std::cout << "Undeclared variable name in line " << i << std::endl;
							return 0;
						}
					}
					else
					{
						variable.value = StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size());
						j++;
					}
					continue;
				}

				// + - * /
				if (ActiveLine.size() > j + 1)
				{
					// +
					if (ActiveLine[j] == "+")
						{
							if (!(IfNumber(ActiveLine[j + 1][0])))
							{
								int l = 0;
								for (int s = 0; s < VarInt.size(); s++)
								{
									if (VarInt[s].name == ActiveLine[j + 1])
									{
										variable.value += VarInt[s].value;
										j++;
										break;
									}
								}
								if (l == VarInt.size())
								{
									system("CLS");
									std::cout << "Undeclared variable name in line " << i << std::endl;
									return 0;
								}
							}
							else
							{
								variable.value += StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size());
								j++;
							}
							continue;
						}
					// -
					if (ActiveLine[j] == "-")
						{
							if (!(IfNumber(ActiveLine[j + 1][0])))
							{
								int l = 0;
								for (int s = 0; s < VarInt.size(); s++)
								{
									if (VarInt[s].name == ActiveLine[j + 1])
									{
										variable.value -= VarInt[s].value;
										j++;
										break;
									}
								}
								if (l == VarInt.size())
								{
									system("CLS");
									std::cout << "Undeclared variable name in line " << i << std::endl;
									return 0;
								}
							}
							else
							{
								variable.value -= StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size());
								j++;
							}
							continue;
						}
					// *
					if (ActiveLine[j] == "*")
						{
							if (!(IfNumber(ActiveLine[j + 1][0])))
							{
								int l = 0;
								for (int s = 0; s < VarInt.size(); s++)
								{
									if (VarInt[s].name == ActiveLine[j + 1])
									{
										variable.value *= VarInt[s].value;
										j++;
										break;
									}
								}
								if (l == VarInt.size())
								{
									system("CLS");
									std::cout << "Undeclared variable name in line " << i << std::endl;
									return 0;
								}
							}
							else
							{
								variable.value *= StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size());
								j++;
							}
							continue;
						}
					// /
					if (ActiveLine[j] == "/")
						{
							if (!(IfNumber(ActiveLine[j + 1][0])))
							{
								int l = 0;
								for (int s = 0; s < VarInt.size(); s++)
								{
									if (VarInt[s].name == ActiveLine[j + 1])
									{
										if (VarInt[s].value == 0)
										{
											system("CLS");
											std::cout << "You can't divide in 0!" << std::endl;
											return 0;
										}
										variable.value /= VarInt[s].value;
										j++;
										break;
									}
								}
								if (l == VarInt.size())
								{
									system("CLS");
									std::cout << "Undeclared variable name in line " << i << std::endl;
									return 0;
								}
							}
							else
							{
								if (StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size()) == 0)
								{
									system("CLS");
									std::cout << "You can't divide in 0!" << std::endl;
									return 0;
								}
								variable.value /= StringToInt(ActiveLine[j + 1], ActiveLine[j + 1].size());
								j++;
							}
							continue;
						}
				}
				else
				{
					system("CLS");
					std::cout << "In line " << i << " Syntax error! " << "[ " << ActiveLine[j] << " ]" << std::endl;
					return 0;
				}
			}
			VarInt.push_back(variable);
			ActiveLine.clear();
			continue;
		}

		// If variable is String
		if (ActiveLine[0] == "Corda")
		{
			if (ActiveLine.size() < 2)
			{
				system("CLS");
				std::cout << "In line " << i << " Your declared variable don't have name!" << std::endl;
				return 0;
			}
			VarCorda variable;
			for (int s = 0; s < VarDouble.size(); s++)
			{
				if (VarDouble[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarInt.size(); s++)
			{
				if (VarInt[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarBool.size(); s++)
			{
				if (VarBool[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarString.size(); s++)
			{
				if (VarString[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}

			variable.name = ActiveLine[1];
			if (ActiveLine.size() == 3 || ActiveLine.size() > 4)
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			if (ActiveLine[2] == "=")
			{
				variable.value = ActiveLine[3];
			}
			else
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			VarString.push_back(variable);
			ActiveLine.clear();
			continue;
		}

		// If variable is Bool
		if (ActiveLine[0] == "Verum")
		{
			if (ActiveLine.size() < 2)
			{
				system("CLS");
				std::cout << "In line " << i << " Your declared variable don't have name!" << std::endl;
				return 0;
			}
			VarVerum variable;
			for (int s = 0; s < VarDouble.size(); s++)
			{
				if (VarDouble[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarInt.size(); s++)
			{
				if (VarInt[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarBool.size(); s++)
			{
				if (VarBool[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			for (int s = 0; s < VarString.size(); s++)
			{
				if (VarString[s].name == ActiveLine[1])
				{
					system("CLS");
					std::cout << "This name already declared!" << std::endl << "Error line " << i << std::endl;
					return 0;
				}
			}
			variable.name = ActiveLine[1];
			if (ActiveLine.size() == 3 || ActiveLine.size() > 4)
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			if (ActiveLine[2] == "=")
			{
				if (ActiveLine[3] == "" || ActiveLine[3] == "0" || ActiveLine[3] == "Falso")
				{
					variable.value = false;
				}
				else
				{
					variable.value = true;
				}
			}
			else
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			VarBool.push_back(variable);
			ActiveLine.clear();
			continue;
		}

		// std::cout 
		if (ActiveLine[0] == "Tampa")
		{
			bool ifik = true;
			// std::endl
			if (ActiveLine[1] == "Riga" && ActiveLine.size() == 2)
			{
				if (ActiveLine.size() != 2)
				{
					system("CLS");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				std::cout << std::endl;
				ifik = false;
			}
		
			int size = ActiveLine[1].size();
			// if text
			if (ActiveLine[1][0] == '(')
			{
				std::cout << ActiveLine[1].substr(1, size - 2);
				ifik = false;
			}
			// if variables
			if (ActiveLine.size() == 2 && ifik)
			{
				int sizeik = VarInt.size();
				for (int i = 0; i < sizeik; i++)
				{
					if (VarInt[i].name == ActiveLine[1])
					{
						std::cout << VarInt[i].value;
						ifik = false;
						break;
					}
				}
				if (ifik)
				{
					sizeik = VarDouble.size();
					for (int i = 0; i < sizeik; i++)
					{
						if (VarDouble[i].name == ActiveLine[1])
						{
							std::cout << VarDouble[i].value;
							ifik = false;
							break;
						}
					}
				}
				if (ifik)
				{
					sizeik = VarString.size();
					for (int i = 0; i < sizeik; i++)
					{
						if (VarString[i].name == ActiveLine[1])
						{
							std::cout << VarString[i].value;
							ifik = false;
							break;
						}
					}
				}
				if (ifik)
				{
					sizeik = VarBool.size();
					for (int i = 0; i < sizeik; i++)
					{
						if (VarBool[i].name == ActiveLine[1])
						{
							std::cout << VarBool[i].value;
							ifik = false;
							break;
						}
					}
				}
				if (ifik)
				{
					system("ClS");
					std::cout << "You want to print undeclared variable in line " << i << std::endl;
					return 0;
				}
			}
			ActiveLine.clear();
			continue;
		}

		//std::cin
		if (ActiveLine[0] == "Ingresso" && ActiveLine.size() == 2)
		{
			bool ifik = true;
			int sizeik = VarInt.size();
			for (int i = 0; i < sizeik; i++)
			{
				if (VarInt[i].name == ActiveLine[1])
				{
					std::cin >> VarInt[i].value;
					ifik = false;
					break;
				}
			}
			if (ifik)
			{
				sizeik = VarDouble.size();
				for (int i = 0; i < sizeik; i++)
				{
					if (VarDouble[i].name == ActiveLine[1])
					{
						std::cin >> VarDouble[i].value;
						ifik = false;
						break;
					}
				}
			}
			if (ifik)
			{
				sizeik = VarString.size();
				for (int i = 0; i < sizeik; i++)
				{
					if (VarString[i].name == ActiveLine[1])
					{
						std::cin >> VarString[i].value;
						ifik = false;
						break;
					}
				}
			}
			if (ifik)
			{
				sizeik = VarBool.size();
				for (int i = 0; i < sizeik; i++)
				{
					if (VarBool[i].name == ActiveLine[1])
					{
						std::cin >> VarBool[i].value;
						ifik = false;
						break;
					}
				}
			}
			if (ifik)
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			ActiveLine.clear();
			continue;
		}

		// if
		if (ActiveLine[0] == "Caso")
		{
			if (ActiveLine.size() != 2)
			{
				//system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			bool state = true;
			int index;
			if (ActiveLine.size() > 2)
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			int size = AllLines.size();
			if (AllLines[i + 1][0] != '{')
			{
				system("CLS");
				std::cout << "Expected { statment in line " << i << std::endl;
				return 0;
			}
			for (int k = i + 2; k < size; k++)
			{
				if (AllLines[k][0] == '}')
				{
					state = false;
					index = k;
					break;
				}
			}
			if (state)
			{
				system("CLS");
				std::cout << "Expected } statment" << std::endl;
				return 0;
			}
			bool result = true;
			bool bug = false;
			Condition(ActiveLine[1], VarInt, VarDouble, VarBool, bug, result);
			if (bug)
			{
				system("CLS");
				std::cout << "Syntax error in line " << i << std::endl;
				return 0;
			}
			if (result)
			{
				i += 1;
				ActiveLine.clear();
				continue;
			}
			else
			{
				i = index;
				ActiveLine.clear();
				continue;
			}
		}

		// = + - * /
		int size = ActiveLine.size();
		if (size == 3)
		{
			bool bug = false;
			//if =
			if (ActiveLine[1] == "=")
			{
				int index = IsType<VarIntero>(VarInt, ActiveLine[0]);
				if (index != -1)
				{
					VarInt[index].value = ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVero>(VarDouble, ActiveLine[0]);
				if (index != -1)
				{
					VarDouble[index].value = ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVerum>(VarBool, ActiveLine[0]);
				if (index != -1)
				{
					bool value = true;
					if (ActiveLine[2] == "0" || ActiveLine[2] == "Falso")
					{
						value = false;
					}
					VarBool[index].value = value;
					ActiveLine.clear();
					continue;
				}
				index = IsType<VarCorda>(VarString, ActiveLine[0]);
				if (index != -1)
				{
					VarString[index].value = ActiveLine[2];
					ActiveLine.clear();
					continue;
				}
				if (index == -1)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
			}
			//if +
			if (ActiveLine[1] == "+=")
			{
				int index = IsType<VarIntero>(VarInt, ActiveLine[0]);
				if (index != -1)
				{
					VarInt[index].value += ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVero>(VarDouble, ActiveLine[0]);
				if (index != -1)
				{
					VarDouble[index].value += ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarCorda>(VarString, ActiveLine[0]);
				if (index != -1)
				{
					VarString[index].value += ActiveLine[2];
					ActiveLine.clear();
					continue;
				}
				if (index == -1)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
			}
			//if -
			if (ActiveLine[1] == "-=")
			{
				int index = IsType<VarIntero>(VarInt, ActiveLine[0]);
				if (index != -1)
				{
					VarInt[index].value -= ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVero>(VarDouble, ActiveLine[0]);
				if (index != -1)
				{
					VarDouble[index].value -= ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				if (index == -1)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
			}
			// if *
			if (ActiveLine[1] == "*=")
			{
				int index = IsType<VarIntero>(VarInt, ActiveLine[0]);
				if (index != -1)
				{
					VarInt[index].value *= ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVero>(VarDouble, ActiveLine[0]);
				if (index != -1)
				{
					VarDouble[index].value *= ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				if (index == -1)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
			}
			//if /
			if (ActiveLine[1] == "/=")
			{
				int index = IsType<VarIntero>(VarInt, ActiveLine[0]);
				if (index != -1)
				{
					int value = ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					if (value != 0)
					{
						VarInt[index].value /= value;
					}
					else
					{
						system("CLA");
						std::cout << "You can't divide on 0 in line " << i << std::endl;
						return 0;
					}
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				index = IsType<VarVero>(VarDouble, ActiveLine[0]);
				if (index != -1)
				{
					double value = ValueFinder(ActiveLine[2], VarInt, VarDouble, bug);
					if (value != 0)
					{
						VarDouble[index].value /= value;
					}
					else
					{
						system("CLA");
						std::cout << "You can't divide on 0 in line " << i << std::endl;
						return 0;
					}
					ActiveLine.clear();
					continue;
				}
				if (bug)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
				if (index == -1)
				{
					system("CLA");
					std::cout << "Syntax error in line " << i << std::endl;
					return 0;
				}
			}
			system("CLA");
			std::cout << "Syntax error in line " << i << std::endl;
			return 0;
		}
		else
		{
			system("CLA");
			std::cout << "Syntax error in line " << i << std::endl;
			return 0;
		}
		ActiveLine.clear();
	}
	
	return 0;
}
