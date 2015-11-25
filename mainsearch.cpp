//
//  main.cpp


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>

using namespace std;

class name{
private:
    vector<string> primary_name;
    vector<vector<string>> synonyms;
    
    name(vector<vector<string>> obj){
        for(int i=0; i < obj.size(); i++){
            for(int j=0; j < obj[i].size(); j++){
                stringstream tmp;
                string temp;
                if(j==0){
                    tmp << obj[i][j];
                    while(!tmp.eof()){
                        tmp >> temp;
                        primary_name.push_back(temp);
                        temp.clear();
                    }
                }
                else{
                    vector<string> sub_name;
                    tmp << obj[i][j];
                    while(!tmp.eof()){
                        tmp >> temp;
                        sub_name.push_back(temp);
                        temp.clear();
                    }
                    synonyms.push_back(sub_name);
                }
            }
        }
    }
};

int strStr(string haystack, string needle) {
    int npoint=0;
    if(needle.empty()){
        return 0;
    }
    if(haystack.empty() || haystack.size() < needle.size()){
        return -1;
    }
    for(int i=0; i <= haystack.size()-needle.size(); i++){
        if(haystack[i] == needle[0]){
            int hpoint=i;
            while(npoint < needle.size() && hpoint < haystack.size() && needle[npoint] == haystack[hpoint]){
                hpoint++;
                npoint++;
            }
            if(npoint==needle.size()){
                return hpoint - npoint;
            }
            npoint=0;
        }
    }
    return -1;
}

ifstream company_data("./companies.dat");
vector<vector<string>> company_table;

bool isin(char obj, vector<char> valid){
    return find(valid.begin(), valid.end(), obj) != valid.end();
}

bool isin(string obj, vector<string> valid){
    return find(valid.begin(), valid.end(), obj) != valid.end();
}

int number_of_words(string obj){
    int result = 0;
    for(long int i=obj.size(); i>=0; i--){
        if(obj[i] != ' ') break;
        if(obj[i]==' ') obj.erase(i);
    }
    stringstream tmp;
    tmp << obj;
    string abc;
    while(!tmp.eof()){
        result++;
        tmp>>abc;
    }
    if(abc == " ") {result--;}
    return result;
}

int main(int argc, const char * argv[]) {
    string temp;
    vector<string> a_company;
    unordered_map<string, int> fre;
    char c;
    vector<char> invalid={'!','@','#','.',',','?','*','(',')','-','+','=','/','$','^','&','%'};
    vector<string> ignore={"a","an","the","and","or","but"};
    
    while (company_data.good()) {
        c = company_data.get();
        if(c=='\n'){
            a_company.push_back(temp);
            company_table.push_back(a_company);
            a_company.clear();
            temp.clear();
            continue;
        }
        
        if(c=='\t'){
            a_company.push_back(temp);
            temp.clear();
            continue;
        }
        temp+=c;
    }
    company_data.close();
    
    cout << "input article" << endl;
    string buffer;
    stringstream tmp;
    while(1){
        getline(cin, buffer);
        cin.clear();
        if(buffer == ".") break;
        for(auto i=buffer.begin(); i != buffer.end(); i++){
            if(isin(*i, invalid)){
                buffer.erase(i);
                i--;
            }
            if(buffer.empty()) break;
        }
        buffer+=" ";
        tmp << buffer;
    }
    
    string article;
    
    while(!tmp.eof())
    {
        string temp;
        tmp >> temp;
        if(isin(temp, ignore)) continue;
        temp+=" ";
        article += temp;
        temp.clear();
    }
    
    for(int i=0; i < company_table.size(); i++){
        for(int j=0; j < company_table[i].size(); j++){
            string temp=article;
            int count=0;
            int pos=strStr(temp, company_table[i][j]);
            while( pos != -1 && !company_table[i][j].empty()){
                count++;
                temp=temp.substr(pos+company_table[i][j].size()-1);
                pos=strStr(temp, company_table[i][j]);
            }
            if(fre.find(company_table[i][0])==fre.end()){
                fre.insert(pair<string, int>(company_table[i][0], count));
            }
            else{
                fre[company_table[i][0]] += count;
            }
        }
    }
    
    int words = number_of_words(article);
    cout << words << endl;
    for(auto i=fre.begin(); i!= fre.end(); i++){
        if(i->second != 0){
            cout << i->first << ":\t\t" << ((float)i->second/words)*100 << "%" << endl;
        }
    }
    
    cout << article;
    return 0;
}

