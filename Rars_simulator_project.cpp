#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
// #include <bits/stdc++.h>

using namespace std; 

bool flag = false;


int Bin2Decimal(string x)
{
    int sum = 0;
    int power = 0;
    for(int i = x.length() - 1; i >= 0; i--)
    {
        if(x[i] == '1')
        {
            sum += pow(2, power);
        }
        power++;
    }
    return sum;
}

string decToBinary(int n){
    string binary = "";
    while(n > 0){
        if(n % 2 == 0){
            binary = "0" + binary;
        }
        else{
            binary = "1" + binary;
        }
        n = n / 2;
    }
    return binary;
}


map<string, int> registers; //gloabal map that maps the register names to their values

// R-TYPE INSTRUCTIONS
void add(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] + registers[rs2];
}
void sub(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] - registers[rs2];
}
void sll(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] << registers[rs2];
}
void slt(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] < registers[rs2];
}
// COME BACK TO THIS ONE
void sltu(string rd, string rs1, string rs2){
    unsigned int x = registers[rs1];
    unsigned int y = registers[rs2];
    registers[rd] = x < y;
}
void XOR(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] ^ registers[rs2];
}
void srl(string rd, string rs1, string rs2){
    if(registers[rs1] < 0){
        unsigned int x = registers[rs1];
        registers[rd] = x >> registers[rs2];
    }
    else
        registers[rd] = (registers[rs1] >> registers[rs2]) + pow(2, 31);
}
void sra(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] >> registers[rs2];
}
void OR(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] | registers[rs2];
}
void AND(string rd, string rs1, string rs2){
    registers[rd] = registers[rs1] & registers[rs2];
}



// -TYPE INSTRUCTIONS
void slli(string rd, string rs1, int imm){
    if(imm > 31 || imm < 0){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }

    registers[rd] = registers[rs1] << imm;
}

void srli(string rd, string rs1, int imm){
    if(imm > 31 || imm < 0){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    if(registers[rs1] < 0){
        unsigned int x = registers[rs1];
        registers[rd] = x >> imm;
    }
    else
        registers[rd] = (registers[rs1] >> imm) + pow(2, 31);
}
void srai(string rd, string rs1, int imm){
    if(imm > 31 || imm < 0){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] >> imm;
}

// I-TYPE INSTRUCTIONS
void ADDI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] + imm;
}
void SLTI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] < imm;
}
void SLTIU(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    unsigned int x = registers[rs1];
    registers[rd] = x < imm;
}
void XORI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] ^ imm;
}
void ORI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] | imm;
}
void ANDI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    registers[rd] = registers[rs1] & imm;
}




// U-TYPE INSTRUCTIONS
void lui(string rd, int imm){
    registers[rd] = imm << 12;
}

void auipc(string rd, int imm){
    registers[rd] = registers[rd] + (imm << 12);
}


//fadel load store branch jump w break




int main(){
    // INITIALIZING THE REGISTERS
    registers["x0"] = 0; 
    registers["x1"] = 5;
    registers["sp"] = 10;
    registers["t0"] = 10;
    registers["t1"] = 20;
    registers["t2"] = 0;
    registers["t3"] = 0;
    registers["t4"] = 0;
    registers["t5"] = 0;
    registers["t6"] = 0;
    registers["a0"] = 0;
    registers["a1"] = 0;
    registers["a2"] = 0;
    registers["a3"] = 0;
    registers["a4"] = 0;
    registers["a5"] = 0;
    registers["a6"] = 0;
    registers["a7"] = 0;
    registers["s0"] = 0;
    registers["s1"] = 0;
    registers["s2"] = 0;
    registers["s3"] = 0;
    registers["s4"] = 0;
    registers["s5"] = 0;
    registers["s6"] = 0;
    registers["s7"] = 0;
    registers["s8"] = 0;
    registers["s9"] = 0;
    registers["s10"] = 0;
    registers["s11"] = 0;

    string rd, rs1, rs2; 

    ifstream inputStream; 
    inputStream.open("input.txt");
    	if(inputStream.fail()){
		cout << "Problem in opening the file\n";
		return 1;
	}
    string line;
    while(getline(inputStream, line)){
        stringstream ss(line);
        string instruction;
        ss >> instruction;

        if(instruction == "add" || instruction == "sub"){
            vector<string> tokens; 
            stringstream ss2(line);
            string token; 
            while(getline(ss, token, ',')){
                if(!token.empty()){
                    tokens.push_back(token);
                }
            }

            rd = tokens[1]; 
            rs1 = tokens[2];
            rs2 = tokens[3];

            cout << rd << " " << rs1 << " " << rs2 << endl;

            if(instruction == "add")
                add(rd, rs1, rs2);
            else if(instruction == "sub")
                sub(rd, rs1, rs2);
        }
    }
    

    cout << registers["t5"] << endl; 



    lui("t2", 5);
    cout << registers["t2"] << endl;

}
