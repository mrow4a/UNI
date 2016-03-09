#include <iostream>
#include <vector>
#include <stdlib.h>     /* strtol */
using namespace std;
#define ERR "Wrong array format passed to commandline ! Please use format e.g. [1,2] as an argument"


int solution(const vector<int> &data) {
	size_t data_size = data.size();
	size_t i,j,k;
	long int lsum = 0;
	long int rsum = 0;
	for(i = 0; i < data_size; i++ ){
		lsum = 0;
		rsum = 0;
		for (j = 0; j < i; j++ ){
			lsum+=data[j];
		}
		for (k = i+1; k < data_size; k++ ){
			rsum+=data[k];
		}
		cout << lsum << " " << rsum << endl;
		if (lsum==rsum) return i;
	}
	return -1;
}

string replaceinString(std::string str, std::string tofind, std::string toreplace)
{
	//used in case of some non-commandline parsing
    size_t position = 0;
    for ( position = str.find(tofind); position != std::string::npos; position = str.find(tofind,position) )
    {
    	str.replace(position ,1, toreplace);
    }
    return(str);
}

bool is_digit(char c){
	if( (c >= '0' && c <= '9') ){
		return true;
	}
	else{
		return false;
	}
}

vector<int> vector_parser(vector < int > &data, string s){
	cout << "Given argument: " << s << endl;
	size_t posOpen = s.find("[");
	size_t posClose = s.find("]");
	if (posOpen!=string::npos && posClose!=string::npos){
		string raw_array = s.substr(posOpen+1, posClose-1);
		raw_array = replaceinString(raw_array, " ", "");
		string number;
		number.clear();
		for (string::iterator it = raw_array.begin(), end = raw_array.end(); it != end; ++it)
		{
			if ((is_digit(*it) && it!=end-1) || (*it=='-' && number.empty())){
			    number += (char) *it;
			}
			else if (is_digit(*it) && it==end-1){
			    number += (char) *it;
				data.push_back(atoi(number.c_str()));
			}
			else if (*it!=','){
				throw ERR;
			}
			else if (*it==',' || it==end-1){
				if (number.empty()){
					throw ERR;
				}
				else{
					data.push_back(atoi(number.c_str()));
					number.clear();
				}
			}
		}
	}

	return data;
}

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i) {
		try{
			vector < int > data;
			data = vector_parser(data, argv[i]);

			cout << endl << "Equilibrium index: " << solution(data) << endl;

		}
		catch (const char* msg) {
			cerr << msg << endl;
		}
	}
	return 0;
}


//0

#include <iostream>
#include <vector>
using namespace std;

int solution1(vector<int> &A) {
    long int suml = 0;
    long int sumr = 0;
    size_t i,j,k,A_size;
    A_size = A.size();
    for (i=0;i<A_size;i++){
        suml = 0;
        sumr = 0;
        for (j=0;j<i;j++){
            suml+=A[j];
        }
        for (k=i+1;k<A_size;k++){
            sumr+=A[k];
        }
        cout << suml << " " << sumr << endl;
        if (suml==sumr) {
            cout << "Result: " << i << endl;
            return i;
        }
    }
    return -1;
}

//https://codility.com/c/run/trainingG6BVPB-VAU
using namespace std;

int solution2(int N) {
    string r;
    char dig;
    char first_dig = '0';
    bool bool_count = false;
    long int count=0;
    long int save_count=0;
    while(N!=0) {
        dig = (N%2==0 ?'0':'1');
        if (dig=='1' && first_dig=='0' && count!=0){
            if (save_count<=count){
                save_count = count;
            }
            count = 0;
            bool_count = false;
        }
        else if (dig=='0' && first_dig=='1'){
            bool_count = true;
        }
        if (bool_count==true && N/2!=0) count+=1;


        r+=dig;
        N/=2;
        first_dig = dig;
    }
    return save_count;
}

//https://codility.com/programmers/task/stone_wall/
int solution3(vector<int> &H) {
    double temp_stone_height = 0;
    double stone = 0;
    vector<int> stack;
    for(size_t i=0; i<H.size();i++){
        if (H[i]>temp_stone_height){
            stone+=1;
            stack.push_back(H[i]);
        }
        else if (H[i]<temp_stone_height){
            if (stack[stack.size()-2]!=H[i]){
                int temp_i=stack.size();
                while (stack[temp_i-2]>=H[i]){
                    temp_i-=1;
                    stack.pop_back();
                }
                if (stack[temp_i-1] != H[i]) stone+=1;
                stack[stack.size()-1]=H[i];
            }
            else if (stack[stack.size()-2]==H[i]){
                stack.pop_back();
            }
        }
        temp_stone_height = H[i];
        /*out << "SIZE: " << stack.size() << " |";
        for (size_t j=0;j<stack.size();j++){
            cout << " " << stack[j] ;
        }
        cout << endl;*/
    }
    return stone;
}

//https://codility.com/programmers/task/equi_leader/

int get_equileader(vector<int> &A, size_t start, size_t end){
    vector<int> stack;
    double EQUIL = -1;
    double CANDIDATE;
    for (size_t i=start;i<end;i++){
        //cout << endl << "A:" << A[i] << " |";
        if (!stack.empty() && stack[stack.size()-1]!=A[i]) stack.pop_back();
        else stack.push_back(A[i]);
        //for (size_t j=0;j<stack.size();j++){
            //cout << " " << stack[j];
        //}
    }
    CANDIDATE = stack[0];
    double count = 0;
    for (size_t i=start;i<end;i++){
        if (A[i]==CANDIDATE) {
            //cout << "count!";
            count++;
        }
    }
    if (count > (end-start)/2) EQUIL=CANDIDATE;
    //cout << endl << count << " " << (end-start)/2 << " EQUIL: " << EQUIL;
    return EQUIL;
}
int solution4(vector<int> &A) {
    double a_size = A.size();
    double count =0;
    double eq_1,eq_2;
    for (size_t i=1;i<A.size();i++){
        eq_1 = get_equileader(A,0,i);
        eq_2 = get_equileader(A,i,a_size);
        if (eq_1!=-1 && eq_2!=-1 && eq_1==eq_2) count++;
        //cout << endl;
    }
    return count;
}


//https://codility.com/programmers/task/max_profit/

double maxcalc(double f, double s){
    if (f>=s) return f;
    else return s;
}
int solution(vector<int> &A) {
    double max_profit = 0;
    double max_ending = 0;

    for (size_t i=1;i<A.size();i++){
        max_ending = maxcalc(0,max_ending+A[i] - A[i-1]);
        max_profit = maxcalc(max_profit, max_ending);
    }
    return max_profit;
}





//exam1
// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

bool evalP(vector< vector<int> > &A, size_t j_exc){
    long int sumr=0;
    long int suml=0;
    size_t size_P = A.size();
    size_t size_Q = A[0].size();
    for (size_t i=0;i<size_P;i++){
        for (size_t j=j_exc+1;j<size_Q;j++){
            sumr+=A[i][j];
        }
    }

    for (size_t i=0;i<size_P;i++){
        for (size_t j=0;j<j_exc;j++){
            suml+=A[i][j];
        }
    }
    return (sumr==suml);
}

bool evalQ(vector< vector<int> > &A, size_t i_exc){
    long int sumu=0;
    long int sumd=0;
    size_t size_P = A.size();
    size_t size_Q = A[0].size();
    for (size_t i=0;i<i_exc;i++){
        for (size_t j=0;j<size_Q;j++){
            sumu+=A[i][j];
        }
    }

    for (size_t i=i_exc+1;i<size_P;i++){
        for (size_t j=0;j<size_Q;j++){
            sumd+=A[i][j];
        }
    }
    return (sumu==sumd);
}

int solution(vector< vector<int> > &A) {
    size_t size_P = A.size();
    size_t size_Q = A[0].size();
    bool eval_P_b;
    bool eval_Q_b;
    long int count=0;
    for (size_t i=0;i<size_P;i++){
        for (size_t j=0;j<size_Q;j++){
            eval_P_b = evalP(A,j);
            eval_Q_b =evalQ(A,i);
            if (eval_P_b==true && eval_P_b==eval_Q_b) count++;
            //cout << i << "-" << j << "=" << eval_P_b << "&" << eval_Q_b << " ";
        }
        //cout << endl;
    }
    return count;
}
//exam2
