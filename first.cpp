/*************************************************************************
    > File Name: first.cpp
    > Author: xwJiang
    > Mail: 18261132505@163.com 
    > Created Time: Mon 23 Nov 2020 07:31:37 PM PST
 ************************************************************************/

#include<iostream>
#include<cstring>
#include<set>
#include<vector>
#include<sstream>
#include<algorithm>

#define max 100
using namespace std;

const char CH_split = { 2 };

struct LL1_GA
{
	string* Grammars = NULL;		//语法表
	int length_g;					//语法数量 
	char* Terminal;					//终结符集
	char* NonTerminal;				//非终结符集
	string* First = NULL;			//First集
	string* Follow = NULL;			//Follow集
	int** Predict;					//Predict分析表(含空符号列，不影响分析)
};

LL1_GA ga;
 
//字符ch在字符串vertor中的位置，不在则返回-1
int getNumOfCh(char vector[], char ch) {
	for (int i = 0; vector[i] != '\0'; i++)
		if (vector[i] == ch) return i;
	return -1;
}

//输入语法Grammars
void CreateGA(LL1_GA&ga) {
	int num_gpre, num_gnew = 0;		//语法行数
	cin >> num_gpre;
	string* grammar_input = NULL;		//输入语法
	grammar_input = new string[2];
	ga.NonTerminal = new char[num_gpre];		//记录非终结符
	//-----------------------------------------------------
	//First集专用
	string* grammer_input_2;
	grammer_input_2 = new string [num_gpre];
	//-----------------------------------------------------
	for (int i = 0; i < num_gpre; i++) {
		cin >> grammar_input[0];
		//-----------------------------------------------------
		//First集专用
		grammer_input_2[i] = grammar_input[0];
		grammer_input_2[i].erase(1,2);
		//-----------------------------------------------------
		ga.NonTerminal[i] = grammar_input[0][0];
		grammar_input[1] += grammar_input[0] + CH_split;
	}
	//------------------------------------------------------
	//First集 
	string * CreateFirst(string * Grammers,int length);	//函数定义 
	ga.First = CreateFirst(grammer_input_2,num_gpre);
	//------------------------------------------------------
	ga.NonTerminal[num_gpre] = '\0';
	grammar_input[1] += '\0';
	for (int i = 0; grammar_input[1][i] != '\0'; i++)
		if (grammar_input[1][i] == '|') num_gnew++;
	num_gnew += num_gpre;

	//语法确定化存储
	ga.Grammars = new string[num_gnew];
	ga.Terminal = new char[128];		//记录终结符
	ga.Terminal[0] = '\0';
	int g_write = 0;			//当前语法的序号	
	char ch_nont = grammar_input[1][0], ch_read, t_write = 0;		//当前语法的非终结符、读到的字符和终结符记录位置
	ga.Grammars[g_write] += ch_nont;
	for (int i = 3; grammar_input[1][i] != '\0'; i++) {
		ch_read = grammar_input[1][i];
		if (ch_read == CH_split) {
			ga.Grammars[g_write] += '\0';
			if (grammar_input[1][i + 1] != '\0') {
				g_write++;
				ch_nont = grammar_input[1][i + 1];
				ga.Grammars[g_write] += ch_nont;
				i += 3;
			}
		}
		else if (ch_read == '|') {
			ga.Grammars[g_write] += '\0';
			g_write++;
			ga.Grammars[g_write] += ch_nont;
		}
		else {
			ga.Grammars[g_write] += ch_read;
			if (getNumOfCh(ga.NonTerminal, ch_read) < 0 && getNumOfCh(ga.Terminal, ch_read) < 0) {
				ga.Terminal[t_write] = ch_read;
				t_write++;
				ga.Terminal[t_write] = '\0';
			}
		}
	}
	ga.length_g = num_gnew; 
	
//	cout << ga.length_g << endl;
//	for (int i = 0; i < num_gnew; i++) 
//		cout << ga.Grammars[i] << endl;
//	cout << ga.NonTerminal<<endl<<ga.Terminal<<endl;
}



string * CreateFirst(string * Grammers,int length){
	//保存非终结符 
	int length_g = length;
	//判断一个字符串中是否含有某个字符
	bool str_find(string origin,char temp);	//函数调用 
	string n_target;
	for(int i=0;i<length_g;i++){
		if(!str_find(n_target,Grammers[i][0])){
			n_target.push_back(Grammers[i][0]);
		}
	}
	int non_length = n_target.size();
	//-----------------------------------------------------
	//确认一个非终结符是否可以推导出空字
 	bool deduce_empty(string *temp,char ch,int length);	//函数调用 
 	//构造一个矩阵，里面记录着非终结符能否退出空字，0代表不行，1代表可以
	char **empty = new char * [n_target.length()];
	for(int i=0;i<n_target.length();i++){
		empty[i] = new char[2];
		empty[i][0] = n_target[i];
		if(deduce_empty(Grammers,n_target[i],length_g)){
			empty[i][1] = '1';
		}
		else empty[i][1] = '0';
	}
 	
	//-----------------------------------------------------
	//判断需要多写几个语法，针对s->A|Aa
	int separater = 0;
	for (int i=0;i<length_g;i++){
		separater +=count(Grammers[i].begin(),Grammers[i].end(),'|');
	} 
	//-----------------------------------------------------
	//新的语法长度,构建新的语法列表 
	int new_length_g = length_g + separater;
	string* New_Grammer = new string [new_length_g];
	vector<string> split (string str,char del);	//函数引用 
	int start = 0;
	for(int i=length_g-1;i>=0;i--){
		vector<string> str = split(Grammers[i],'|');
		int temp = str.size();
		New_Grammer[start] = str[0];
		start++;
		if(temp > 1){
			for(int j=1;j<str.size();j++){
				New_Grammer[start].push_back(Grammers[i][0]);
				New_Grammer[start]+=str[j];
				start++;
			}
		}
	}
	//-----------------------------------------------------
 	//判断是不是全是非终结符
 	bool allCapital(string str);	//函数调用 
	//构造First
	set<char> * first = new set<char>[non_length]; 	//根据非终结符数目构造first集 
	for(int i=0;i<new_length_g;i++){
		int base = n_target.find(New_Grammer[i][0]);
		//如果是终结符（包括空字，用$表示）,直接添加 
		if(New_Grammer[i][1]<'A'||'Z'<New_Grammer[i][1]){
			first[base].insert(New_Grammer[i][1]);
		}
		//判断是不是全是非终结符
		else if(allCapital(New_Grammer[i])){
			//全是非终结符，需要判断是不是都能推导出空字
			int count = 0;	//记录不能推导出空字的非终结符的数目
			for(int m=1;m<New_Grammer[i].length();m++){
				if(empty[n_target.find(New_Grammer[i][m])][1]=='0')
					count++;
			} 
			//如果都可以推导出空字
			if(count == 0){
				for(int n=1;n<New_Grammer[i].length();n++){
					int num = n_target.find(New_Grammer[i][n]);
					first[base].insert(first[num].begin(),first[num].end());
				}
			} 
			else{
				for(int n=1;n<New_Grammer[i].length();n++){
					int num = n_target.find(New_Grammer[i][n]);
					int temp_set_length = first[num].size();
					for(int k=0;k<temp_set_length;k++){
						//除了空字，都添加 
						set<char>::iterator it;
					    for(it=first[num].begin ();it!=first[num].end ();it++)
					    {
					        if(*it!='$') first[base].insert(*it); 
					    }
					}
					if(empty[num][1]=='0') break;
				}
			} 
		}
		//不全是非终结符 
		else if(!allCapital){
			for(int n=1;n<New_Grammer[i].length();n++){
				//如果是非终结符 
				if(New_Grammer[i][n]>='A'&&'Z'>=New_Grammer[i][n]){
					int num = n_target.find(New_Grammer[i][n]);
					//如果可以推导出空字，就将除了空字都添加到first集中 
					int temp_set_length = first[num].size();
					for(int k=0;k<temp_set_length;k++){
						//除了空字，都添加 
						set<char>::iterator it;
					    for(it=first[num].begin ();it!=first[num].end ();it++)
					    {
					        if(*it!='$') first[base].insert(*it); 
					    }
					}
					if(empty[num][1]=='0') break;
				}
				else{
					first[base].insert(New_Grammer[i][n]);
					break;
				}	
			}
		} 
	}
	//-----------------------------------------------------
	//将set集形式转换为string集形式
	string * First = new string[non_length]; 
	string set_2_string(set<char> temp_set);
	for(int i=0;i<n_target.size();i++){
		First[i] = set_2_string(first[i]);
	} 
	//-----------------------------------------------------
	//输出first集
	cout<<"First集如下！"<<endl;
	for (int i=0;i<n_target.size();i++){
		cout<<n_target[i]<<":";
		cout<<First[i]<<endl;
	} 
	return First;
}
//First子函数1：判断一个字符串中是否含有某个字符
bool str_find(string origin,char temp){
	string::size_type idx;
	idx = origin.find(temp);
	if(idx == string::npos){
		return false;
	}
	else return true;
}
//First子函数2：切割函数，将一个字符串按照某个字符切割
vector<string> split (string str,char del){
	stringstream ss(str);
	string tok;
	vector<string> ret;
	while(getline(ss,tok,del)){
		if(tok>"")
			ret.push_back(tok);
	}
	return ret;
}
 //First子函数3：确认一个非终结符是否可以推导出空字
 bool deduce_empty(string *temp,char ch,int length){
 	//判断一个字符串中是否含有某个字符
	bool str_find(string origin,char temp);
 	for (int i=0;i<length;i++){
 		if(temp[i][0]==ch){
 			if(str_find(temp[i],'$')){
 				return true;
			} 
 			else if(temp[i][1]<='Z'&&'A'<=temp[i][1]){
 				return deduce_empty(temp,temp[i][1],length);
			}
 					
		}
	}
	return false;
 } 
 //First子函数4：判断是不是全是非终结符
 bool allCapital(string str){
 	int length = str.length();
 	for(int i=1;i<length;i++){
 		if(str[i]<'A'||'Z'<str[i])
 			return false;
	 }
	return true;
 } 
//First子函数5：将set集转换为string集
string set_2_string(set<char> temp_set){
	set<char>::iterator it;
	string ret;
	for(it=temp_set.begin ();it!=temp_set.end ();it++)
    {
        ret.push_back(*it);
    }
    return ret;
} 
 
 
//构建Predict(LL1分析表)
void Predicting(LL1_GA&ga) {
	int grammar_size = ga.length_g;
	 
	/*ga.First = new string[int(strlen(ga.NonTerminal))];
	ga.Follow = new string[int(strlen(ga.NonTerminal))];
	for (int i = 0; i < int(strlen(ga.NonTerminal)); i++)
		cin >> ga.First[i];
	for (int i = 0; i < int(strlen(ga.NonTerminal)); i++)
		cin >> ga.Follow[i];*/

	ga.Predict = new int* [int(strlen(ga.NonTerminal))];
	for (int i = 0; i < int(strlen(ga.NonTerminal)); i++) {
		ga.Predict[i] = new int[int(strlen(ga.Terminal)) + 1];
	}
	for (int i = 0; i < int(strlen(ga.NonTerminal)); i++)
		for (int j = 0; j <= int(strlen(ga.Terminal)); j++)
			ga.Predict[i][j] = -1;			//分析表初始化为-1，即err
			
	//遍历Grammers，填分析表
	char ch_fir;			//ch_fir用于暂存首字符
	int num_f, x, y;			//num_f用于暂存当前字符的序号，(x,y)表示当前填写Predict位置
	for (int i = 0; i < grammar_size; i++) {
		ch_fir = ga.Grammars[i][1];			//取当前语法首字母
		if (getNumOfCh(ga.Terminal, ch_fir) >= 0) {		//当前语法首字母是终结符
			if (ch_fir != '$') {			//该终结符为非空符号
				x = getNumOfCh(ga.NonTerminal, ga.Grammars[i][0]);
				y = getNumOfCh(ga.Terminal, ch_fir);
				ga.Predict[x][y] = i;
			}
			else {			//该符号为空符号
				num_f = getNumOfCh(ga.NonTerminal, ga.Grammars[i][0]);
				for (int j = 0; ga.Follow[num_f][j] != '\0'; j++) {		//取当前语法非终结符的Follow集
					x = getNumOfCh(ga.NonTerminal, ga.Grammars[i][0]);
					if (ga.Follow[num_f][j] == '#')			//考虑Follow中的'#'(NonTerminal中不含'#')
						y = strlen(ga.Terminal);
					else y = getNumOfCh(ga.Terminal, ga.Follow[num_f][j]);
					ga.Predict[x][y] = i;
				}
			}
		}
		else {		//当前语法首字母是非终结符
			num_f = getNumOfCh(ga.NonTerminal, ch_fir);
			for (int j = 0; ga.First[num_f][j] != '\0'; j++) {		//取ch_fir的First集
				x = getNumOfCh(ga.NonTerminal, ga.Grammars[i][0]);
				y = getNumOfCh(ga.Terminal, ga.First[num_f][j]);
				ga.Predict[x][y] = i;
			}
		}
	}

	/*for (int i = 0; i < int(strlen(ga.NonTerminal)); i++) {
		for (int j = 0; j <= int(strlen(ga.Terminal)); j++) {
			if (ga.Predict[i][j] != -1)
				cout << ga.Predict[i][j];
			cout << "\t";
		}
		cout << endl;
	}*/
}

int main()
{
	CreateGA(ga);
	//Predicting(ga);
	return 0;
}



//example1:
//input:
//3
//S->S+T|(S)
//T->F*T|F
//F->i|j
//(i+j)*j
// 
//output:
//yes
//
//Grammers:
//  01234...
//0 SS+T
//1 S(S)
//2 TF*T
//3 TF
//4 Fi
//5 Fj
//
//Terminal:
//012345
//+()*ij
//
//NonTerminal:
//012
//STF 
//
//First:
//	       01234...
//0 first(S) ...
//1 first(T) ...
//2 first(F)
//
//
//	
//Follow:
//	        01234...
//0 follow(S) ...
//1 follow(T) ...
//2 follow(F)
//
//
//	
//Predict:
//	  0	 1  2  3  4  5  6
//	  +  (  )  *  i  j  # 
//0 S  ..
//1 T
//2 F
//
//
//example2:
//input:
//5
//E->TV
//V->+TV|$
//T->FW
//W->*FW|$
//F->(E)|i
//i*i+i
//
//output:
//yes
//
//Grammers:
//  01234...
//0 ETV
//1 V+TV
//2 V$
//3 TFW
//4 W*FW
//5 W$
//6 F(E)
//7 Fi
//
//Terminal:
//012345
//+$*()i
//
//NonTerminal:
//01234
//EVTWF
//
//First:
//	       01234...
//0 first(E) (i
//1 first(V) +$
//2 first(T) (i
//3 first(W) *$
//4 first(F) (i
//
//(i
//+$
//(i
//*$
//(i
//	
//Follow:
//	        01234...
//0 follow(E) )#
//1 follow(V) )#
//2 follow(T) +)#
//3 follow(W) +)#
//4 follow(F) *+)#
//
//)#
//)#
//+)#
//+)#
//*+)#
//	
//Predict:
//	  0  1  2  3  4  5  6
//	  +  $  *  (  )  i  #
//0 E	           0     0 
//1 V   1           2     2
//2 T            3     3
//3 W   5     4     5     5
//4 F            6     7
