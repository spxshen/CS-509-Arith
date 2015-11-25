//
//  main.cpp
// project_trie
//
//

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <sstream>

using namespace std;

bool isin(char obj, vector<char> valid){
    return find(valid.begin(), valid.end(), obj) != valid.end();
}

bool isin(string obj, vector<string> valid){
    return find(valid.begin(), valid.end(), obj) != valid.end();
}

class trie{
public:
    class node{
    public:
        int cnt;
        bool key;
        unordered_map<char, node*> child;
        
        node(){
            cnt=0;
            key=false;
        }
        
        node(int obj){
            cnt=obj;
            key=false;
        }
        
        node(int cn, bool ke){
            cnt=cn;
            key=ke;
        }
    };
    node* root;
    
    void update(string art){
        node* p =root;
        for(int i=0; i < art.size(); i++){
            if(art[i]==' '){
                if(p->child.find(' ')==p->child.end()){
                    p=root;
                    continue;
                }
            }
            if(p->child.find(art[i]) == p->child.end()){
                while(i<art.size() && art[i]!=' '){
                    i++;
                }
            }
            else{
                p = p->child[art[i]];
                if(p->key == true){
                    p->cnt++;
                }
            }
        }
    }
    
    int check(string obj){
        node *p = root;
        for(int i=0; i < obj.size(); i++){
            if(p->child.find(obj[i]) == p->child.end()){
                return 0;
            }
            else{
                p = p->child[obj[i]];
                if(p->key == true){
                    return p->cnt;
                }
            }
        }
        return 0;
    }
    
    trie(vector<vector<string>> obj){
        root = new node();
        for(int i=0; i < obj.size(); i++){
            for(int j=0; j < obj[i].size(); j++){
                node *p = root;
                for(int h=0; h < obj[i][j].size(); h++){
                    char cur = obj[i][j][h];
                    if(p->child.find(cur) == p->child.end()){
                        p->child.insert(pair<char,node*>(cur, new node()));
                    }
                    p=p->child[cur];
                    if(h==obj[i][j].size()-1){
                        p->cnt=0;
                        p->key=true;
                    }
                }
            }
        }
    }
};

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
    ifstream company_data("./companies.dat");
    string temp;
    vector<string> a_company;
    vector<vector<string>> company_table;
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
    
    trie test(company_table);
    
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
    
    test.update(article);
    
    int num = number_of_words(article);
    
    for(int i=0; i < company_table.size(); i++){
        int count=0;
        for(int j=0; j < company_table[i].size(); j++){
            count+=test.check(company_table[i][j]);
        }
        if(count !=0){
            cout << company_table[i][0] << ":\t" << (float)count / num * 100 << "%" <<endl;
        }
    }
    
    return 0;
}
