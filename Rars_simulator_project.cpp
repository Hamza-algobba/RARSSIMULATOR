#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
// #include <bits/stdc++.h>

using namespace std; 

//the program will terminate at any instance where the flag is set to true
bool flag = false;

vector<string> riscv_registers = {
    "zero", "ra", "sp", "gp", "tp",
    "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
}; //assumption: x0 and x1 will not be acceptable inputs, instead use zero and ra.


string decimal2hexadecimal(int n)
{
    string hexaDeciNum = "";
    char hexaDeciNums[100];
    int i = 0;
    while(n!=0)
    {
        int temp  = 0;
        temp = n % 16;
        if(temp < 10)
        {
            hexaDeciNums[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNums[i] = temp + 55;
            i++;
        }
        n = n/16;
    }
    for(int j = i - 1; j >= 0; j--)
    {
        hexaDeciNum += hexaDeciNums[j];
    }
    return hexaDeciNum;
}

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

//makes sure that the register names are valid and that the rd register is not zero
void validate_3input_registers(string rd, string rs1, string rs2){
    bool rd_present = find(riscv_registers.begin(), riscv_registers.end(), rd) != riscv_registers.end();
    bool rs1_present = find(riscv_registers.begin(), riscv_registers.end(), rs1) != riscv_registers.end();
    bool rs2_present = find(riscv_registers.begin(), riscv_registers.end(), rs2) != riscv_registers.end();

    if (!rd_present || !rs1_present || !rs2_present) {
        cout << "ERROR: Invalid register name" << endl;
        flag = true;
        return;
    }

    if (rd == "zero") {
        cout << "ERROR: Cannot write to zero register" << endl;
        flag = true;
        return;
    }
}

//note to self: talama betchek 3ala asamy elregisters, lazem ttcheck a5er register law immediate instrucrtion lazem ykon rakam
void validate_2input_registers(string rd, string rs1){
    bool rd_present = find(riscv_registers.begin(), riscv_registers.end(), rd) != riscv_registers.end();
    bool rs1_present = find(riscv_registers.begin(), riscv_registers.end(), rs1) != riscv_registers.end();

    if (!rd_present || !rs1_present) {
        cout << "ERROR: Invalid register name" << endl;
        flag = true;
        return;
    }

    if (rd == "zero") {
        cout << "ERROR: Cannot write to zero register" << endl;
        flag = true;
        return;
    }
}

void validate_1input_registers(string rd){
    bool rd_present = find(riscv_registers.begin(), riscv_registers.end(), rd) != riscv_registers.end();

    if (!rd_present) {
        cout << "ERROR: Invalid register name" << endl;
        flag = true;
        return;
    }

    if (rd == "zero") {
        cout << "ERROR: Cannot write to zero register" << endl;
        flag = true;
        return;
    }
}

// R-TYPE INSTRUCTIONS
void add(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] + registers[rs2];
}
void sub(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] - registers[rs2];
}
void sll(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] << registers[rs2];
}
void slt(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] < registers[rs2];
}
// COME BACK TO THIS ONE // rege3na walla lessa?
void sltu(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    unsigned int x = registers[rs1];
    unsigned int y = registers[rs2];
    registers[rd] = x < y;
}
void XOR(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] ^ registers[rs2];
}
void srl(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    if(registers[rs1] < 0){
        unsigned int x = registers[rs1];
        registers[rd] = x >> registers[rs2];
    }
    else
        registers[rd] = (registers[rs1] >> registers[rs2]) + pow(2, 31);
}
void sra(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] >> registers[rs2];
}
void OR(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] | registers[rs2];
}
void AND(string rd, string rs1, string rs2){
    validate_3input_registers(rd, rs1, rs2);
    registers[rd] = registers[rs1] & registers[rs2];
}



// -TYPE INSTRUCTIONS
void slli(string rd, string rs1, int imm){
    //check if immediate value is out of range
    if(imm > 31 || imm < 0){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }

    validate_2input_registers(rd, rs1);

    registers[rd] = registers[rs1] << imm;
}

void srli(string rd, string rs1, int imm){
    if(imm > 31 || imm < 0){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }

    validate_2input_registers(rd, rs1);

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
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] >> imm;
}

// I-TYPE INSTRUCTIONS
void ADDI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] + imm;
}

void SLTI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] < imm;
}
void SLTIU(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    unsigned int x = registers[rs1];
    registers[rd] = x < imm;
}
void XORI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] ^ imm;
}
void ORI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] | imm;
}
void ANDI(string rd, string rs1, int imm){
    if(imm > 2047 || imm < -2048){
        cout << "ERROR: IMMEDIATE VALUE OUT OF RANGE" << endl;
        flag = true;
        return;
    }
    validate_2input_registers(rd, rs1);
    registers[rd] = registers[rs1] & imm;
}




// U-TYPE INSTRUCTIONS
void lui(string rd, int imm){
    validate_1input_registers(rd);
    registers[rd] = imm << 12;
}

void auipc(string rd, int imm){   
    validate_1input_registers(rd);
    registers[rd] = registers[rd] + (imm << 12);
} //WE DID NOT TEST THIS FUNCTION


//fadel load store branch jump w break

//function that parses each instruction into its components
void parseInputString_3operands(string input, string & instruction, string& rd, string& rs1, string& rs2) {
    size_t pos1 = input.find(',');
    size_t pos2 = input.find(',', pos1 + 1);

    instruction = input.substr(0, input.find(' '));
    rd = input.substr(input.find(' ') + 1, pos1 - input.find(' ') - 1);
    rs1 = input.substr(pos1 + 2, pos2 - pos1 - 2);
    rs2 = input.substr(pos2 + 2);
}

void parseInputString_2operands(const std::string& input, std::string& instruction, std::string& rd, std::string& rs) {
    size_t pos1 = input.find(' ');
    size_t pos2 = input.find(',', pos1 + 1);
    instruction = input.substr(0, pos1);
    rd = input.substr(pos1 + 1, pos2 - pos1 - 1);
    rs = input.substr(pos2 + 2);

}


void Validate_instructions(string FileName) //validates instructions of the entire program and calls the corresponding function
{
    //opens the file and makes sure it is opened correctly
    ifstream inputStream; 
    inputStream.open(FileName);
    if(inputStream.fail()){
		cout << "Problem in opening the file\n";
		exit(-1);
	}

    //reads the file line by line and stores each line in a vector
    //possible limitation: it will store empty lines as well
    string command;
    vector<string> commands; 
    while(getline(inputStream, command)){
        commands.push_back(command);
    }

    string line;
    while(getline(inputStream, line)){
        //initializes the variables that will hold the instruction and its operands
        string* instruction = new string("Hello");
        string* rd = new string("word");
        string* rs1 = new string("word");
        string* rs2  = new string("word");

        //gets number of commas in the line to determine the number of operands in the instruction
        int numCommas = 0;
        for (char c : line) {
            if (c == ',') {
                numCommas++;
            }
        }

        //parses the instruction into its components according to the number of operands determined by the number of commas
        if (numCommas == 2){
            parseInputString_3operands(line, *instruction, *rd, *rs1, *rs2);
        }
        else if (numCommas == 1){
            parseInputString_2operands(line, *instruction, *rd, *rs1);
        }
        else{
            cout << "ERROR: INVALID INSTRUCTION" << endl;
            flag = true;
            return;
        }

       
        //calls the corresponding function according to the instruction
        if (*instruction == "LUI" || *instruction == "lui") {
            lui(*rd, stoi(*rs1));
        }
        else if (*instruction == "AUIPC" || *instruction == "auipc") {
            auipc(*rd, stoi(*rs1));
        }
        // else if (*instruction == "JAL" || *instruction == "jal") {
        //     JAL(rd, rs1, rs2);
        // }
        // else if (*instruction == "JALR" || *instruction == "jalr") {
        //     JALR(rd, rs1, rs2);
        // }
        // else if (*instruction == "BEQ" || *instruction == "beq") {
        //     BEQ(rd, rs1, rs2);
        // }
        // else if (*instruction == "BNE" || *instruction == "bne") {
        //     BNE(rd, rs1, rs2);
        // }
        // else if (*instruction == "BLT" || *instruction == "blt") {
        //     BLT(rd, rs1, rs2);
        // }
        // else if (*instruction == "BGE" || *instruction == "bge") {
        //     BGE(rd, rs1, rs2);
        // }
        // else if (*instruction == "BLTU" || *instruction == "bltu") {
        //     BLTU(rd, rs1, rs2);
        // }
        // else if (*instruction == "BGEU" || *instruction == "bgeu") {
        //     BGEU(rd, rs1, rs2);
        // }
        // else if (*instruction == "LB" || *instruction == "lb") {
        //     LB(rd, rs1, rs2);
        // }
        // else if (*instruction == "LH" || *instruction == "lh") {
        //     LH(rd, rs1, rs2);
        // }
        // else if (*instruction == "LW" || *instruction == "lw") {
        //     LW(rd, rs1, rs2);
        // }
        // else if (*instruction == "LBU"   || *instruction == "lbu") {
        //     LBU(rd, rs1, rs2);
        // }
        // else if (*instruction == "LHU" || *instruction == "lhu") {
        //     LHU(rd, rs1, rs2);
        // }
        // else if (*instruction == "SB" || *instruction == "sb") {
        //     SB(rd, rs1, rs2);
        // }
        // else if (*instruction == "SH" || *instruction == "sh") {
        //     SH(rd, rs1, rs2);
        // }
        // else if (*instruction == "SW" || *instruction == "sw") {
        //     SW(rd, rs1, rs2);
        // }
        else if (*instruction == "ADDI" || *instruction == "addi") {
            ADDI(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "SLTI" || *instruction == "slti") {
            SLTI(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "SLTIU" || *instruction == "sltiu") {
            SLTIU(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "XORI" || *instruction == "xori") {
            XORI(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "ORI" || *instruction == "ori") {
            ORI(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "ANDI" || *instruction == "andi") {
            ANDI(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "SLLI" || *instruction == "slli") {
            slli(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "SRLI" || *instruction == "srli") {
            srli(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "SRAI" || *instruction == "srai") {
            srai(*rd, *rs1, stoi(*rs2));
        }
        else if (*instruction == "ADD" || *instruction == "add") {
            add(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SUB" || *instruction == "sub") {
            sub(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SLL" || *instruction == "sll") {
            sll(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SLT" || *instruction == "slt") {
            slt(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SLTU" || *instruction == "sltu") {
            sltu(*rd, *rs1, *rs2);
        }
        else if (*instruction == "XOR" || *instruction == "xor") {
            XOR(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SRL" || *instruction == "srl") {
            srl(*rd, *rs1, *rs2);
        }
        else if (*instruction == "SRA" || *instruction == "sra") {
            sra(*rd, *rs1, *rs2);
        }
        else if (*instruction == "OR" || *instruction == "or") {
            OR(*rd, *rs1, *rs2);
        }
        else if (*instruction == "AND" || *instruction == "and") {
            AND(*rd, *rs1, *rs2);
        }
        else if (*instruction == "FENCE" || *instruction == "EcALL" || *instruction == "EBREAK") {
            exit(0);
        }
        else {
            cout << "Invalid Instruction" << endl;
            flag = true;
        }
  
    }
   
}

//note to self: when a procedure is called e.g. L1: it will be followed by a colon. 
//This is how we will detect a procedure call and differentiate it from a normal instruction
//the caviat is that user won't be able to put the procedure call and the instruction on the same line e.g. L1: add x1, x2, x3

int main(){
    // INITIALIZING THE REGISTERS
    registers["x0"] = 0; 
    registers["x1"] = 5;
    registers["sp"] = 10;
    registers["gp"] = 0;
    registers["tp"] = 0;
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

    //string rd, rs1, rs2; 

    //inputting the file name and validating its content.
    string filename;
    cout<< "Enter the name of the file: ";
    cin >> filename;	
    Validate_instructions(filename);
    

    cout << registers["t5"] << endl; 



    lui("t2", 5);
    cout << registers["t2"] << endl;

}
