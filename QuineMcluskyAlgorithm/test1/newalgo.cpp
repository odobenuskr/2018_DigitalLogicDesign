#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

int MIN_BITS = 3;		//minimum bits to print
vector<unsigned> input_values;
bool show_mid = false;		//show middle process

struct B_number{
	unsigned number;
	unsigned dashes;
	bool used;
};

vector<vector<B_number> > table;	//original table
vector<vector<B_number> > p_group;	//mid process table
vector<vector<B_number> > final_group;	//final table
vector<B_number> printed_numbers; //avoid printing the same final numbers

unsigned count_1s(unsigned number); //1의 개수를 센다. count the number of 1s in a number
void print_binary(unsigned number);//2진수 형태로 출력한다. print the number in binary
void create_table();		//1개수로 정렬된 기본 테이블을 생성한다. create original table sorted by the number of 1s
void print_table();		//테이블을 출력한다. print the table
B_number init_B_number(unsigned n,int d, bool u);//B넘버를 생성한다. B넘버는 숫자와 대쉬, 사용여부로 구성. initialize a B_number
void create_p_group();		//중간 결과 테이블을 출력한다. create mid process table
void print_p_group();		//p그룹을 출력한다. print it
void print_p_binary(unsigned n, unsigned d);//print the mid table (with -'s)
void create_final_group();		//최종 결과를 생성한다. create final table
void print_final_group();		//최종 결과를 출력한다. print final table with -'s and unused terms
bool is_printed(B_number n);		//출력 되었는지 여부를 판단한다. dont print terms that were already printed
void init();				//테이블 생성 및 출력의 시작. start the table making and printing
void getinput();			//입력을 받는다. get input from user
unsigned count_bits(unsigned n);	//min bits to represent a number

int main(int argc, char const *argv[]) {
  getinput();
  init();
	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 3; j++) {
			std::cout << table[i][j].number << ' ';
		}
		std::cout << '\n';
	}
  return 0;
}

unsigned count_1s(unsigned number) {
	short bit =0;
	int count = 0;
	while(number>0)	{
		bit = number%2;
		number>>=1;
		if(bit) {
			count++;
		}
	}
	return count;
}
/*get LSB, arrange it in array, the print array in reverse order so MSB is on
the left */
void print_binary(unsigned number) {
	unsigned bits[MIN_BITS];
	int count = 0;

	while(number>0||count<MIN_BITS)	{
		bits[count] = number%2;
		number>>= 1;
		count++;
	}
	for(int i=count-1;i>=0;i--)
		cout<<bits[i];
}
/*creating first table: append current number to the array located in
table[number of 1s f this number]*/
void create_table() {
	short tmp;
	B_number temp_num;
	for(int i=0;i<input_values.size();i++) {
		tmp = count_1s(input_values[i]);
		if(tmp+1>table.size())
			table.resize(tmp+1);

		temp_num = init_B_number(input_values[i],0,false);
		table[tmp].push_back(temp_num);
	}
}

void print_table() {

	cout<<endl<<"COMPUTING:"<<endl;
	for(int i=0;i<table.size();i++)	{
		cout<<i;
		for(int j=0;j<table[i].size();j++) {
			cout<<"\tm"<<table[i][j].number<<"\t";
			print_binary(table[i][j].number);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}
}
/* initialize a B_number variable - like a constructor */
B_number init_B_number(unsigned	n,int d, bool u) {
	B_number num;
	num.number = n;
	num.dashes = d;
	num.used = u;
	return num;
}
/*like the original table, but the paring of numbers from the original table-
dashes are represented by a 1. for example original A=0011 B=1011, new number
is -011 which is represented as C.number=A&B=0011,C.dashes=A^B=1000*/
void create_p_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<table.size()-1;i++) {
		for(int j=0;j<table[i].size();j++) {
			for(int k=0;k<table[i+1].size();k++) {
				temp_number = table[i][j].number & table[i+1][k].number;
				temp_dashes = table[i][j].number ^ table[i+1][k].number;

				if(count_1s(temp_dashes)==1) {
					table[i][j].used = true;
					table[i+1][k].used = true;


					tmp = count_1s(temp_number);

					if(tmp+1>p_group.size())
						p_group.resize(tmp+1);

					temp_num = init_B_number(temp_number, temp_dashes, false);
					p_group[tmp].push_back(temp_num);
				}
			}
		}
	}
}

void print_p_group() {
	cout<<endl<<"MID PROCESS COMPUTATION:"<<endl;

	for(int i=0;i<p_group.size();i++) {
		cout<<i;
		for(int j=0;j<p_group[i].size();j++) {
			cout<<"\t\t";
			print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
			cout<<endl;
		}
		cout<<"\n-------------------------------------"<<endl;
	}

}
/*print a number such as -001; this allocates bits in an array dash=2 then
prints reverse array */
void print_p_binary(unsigned n, unsigned d) {
	unsigned bits[MIN_BITS];
	int count = 0;

	while(n>0||count<MIN_BITS) {
		if(!(d%2))
			bits[count] = n%2;
		else
			bits[count] = 2;

		n >>= 1;
		d >>= 1;
		count++;
	}
	for(int i=count-1;i>=0;i--) {
		if(bits[i]!=2)
			cout<<bits[i];
		else
			cout<<"-";
	}
}

void create_final_group() {
	short tmp;
	B_number temp_num;
	unsigned temp_number, temp_dashes;
	for(int i=0;i<p_group.size()-1;i++) {
		for(int j=0;j<p_group[i].size();j++) {
			for(int k=0;k<p_group[i+1].size();k++) {
				if(p_group[i][j].dashes == p_group[i+1][k].dashes) {
					temp_number = p_group[i][j].number & p_group[i+1][k].number;
					temp_dashes = p_group[i][j].number ^ p_group[i+1][k].number;
					if(count_1s(temp_dashes)==1) {
						temp_dashes^= p_group[i][j].dashes;

						p_group[i][j].used = true;
						p_group[i+1][k].used = true;

						tmp = count_1s(temp_number);

						if(tmp+1>final_group.size())
							final_group.resize(tmp+1);

						temp_num = init_B_number(temp_number, temp_dashes, true);
						final_group[tmp].push_back(temp_num);
					}
				}
			}
		}
	}
}
/*print all the values from the final table, except for duplicates.
  print all the unused numbers from original table and mid process table*/
void print_final_group() {
	cout<<endl<<"FINAL:\n-------------------------------------"<<endl;
	int i,j;
	for(i=0;i<final_group.size();i++) {
		for(j=0;j<final_group[i].size();j++) {
			if(!is_printed(final_group[i][j])) {
				print_p_binary(final_group[i][j].number,final_group[i][j].dashes);
				cout<<endl;
				printed_numbers.push_back(final_group[i][j]);
			}
		}
	}
	for(i=0;i<p_group.size();i++) {
		for(j=0;j<p_group[i].size();j++) {
			if(!p_group[i][j].used) {
				print_p_binary(p_group[i][j].number,p_group[i][j].dashes);
				cout<<endl;
			}
		}
	}
	for(i=0;i<table.size();i++) {
		for(j=0;j<table[i].size();j++) {
			if(!table[i][j].used) {
				print_p_binary(table[i][j].number,table[i][j].dashes);
				cout<<endl;
			}
		}
	}
	cout<<"-------------------------------------"<<endl;
}
/*used to avoid printing duplicates that can exist in the final table*/
bool is_printed(B_number n) {
	for(int i=0;i<printed_numbers.size();i++)
		if(n.number==printed_numbers[i].number && n.dashes == printed_numbers[i].dashes)
			return true;

	return false;
}
/*initialize all table*/
void init() {
	table.resize(1);
	p_group.resize(1);
	final_group.resize(1);
	create_table();
	print_table();
	create_p_group();
	if(show_mid)
		print_p_group();
	create_final_group();
	print_final_group();
}

void getinput() {
	int numOfCase, temp;
	std::cin >> MIN_BITS >> numOfCase;
	for (size_t i = 0; i < numOfCase; i++) {
		std::cin >> temp;
		input_values.push_back(temp);
	}
}
/*return min number of bits a number is represented by. used for best output*/
unsigned count_bits(unsigned n) {
	short bit =0;
	int count = 0;
	while(n>0) {
		bit = n%2;
		n>>=1;
		count++;
	}
	return count;
}



























//
