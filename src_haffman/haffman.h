#include <iostream>
#include <string>
#include <fstream> 
#include <cstdlib>            // для поддержки exit
#include <map>      //для создания словаря { символ : частота} и хранения кодов
#include <vector>   //для хранения кучи 
#include <queue>
#include <filesystem> // для работы с файловой системой 
using std::string;
using std::map;
//---------------------------------


map<char, int> getFreqSymbols(string);     // получаем словарь {символ : частота}

string read_file(const std::filesystem::path& filePath);                  // чтение файла....Может нужно использовать буфер??? 
//string read_file(const string& filePath);                                                      // передаем константную ссылку на путь используя filesystem


void print_dict(map<char, int>);            // вывод словаря {символ : частота}

void print_codes_symbols(map<char, int>);


//реализация узла для дерева
struct Node {
    char symbol;
    int weight;  // freq
    string code;
    Node* left;   // указатель на левого потомка
    Node* right;  // указатель на правого потомка
    Node(char ch, int w);
};


//струкутра для сравнения узлов по их весу
struct CompareNodes {
    bool operator()(Node* const& a, Node* const& b) {
        return a->weight > b->weight;
    }
};

//построение дерева хаффмана 
Node* buildTreeHaffman(std :: map<char, int> symbols_freq);

//кодирование по построенному дереву. Результат в haffman_codes
void generate_haffman_codes(Node* root, map<char, string>& haffman_codes); // передаем ссылку, чтобы словарь изменялся глобально во время работы фукнкции



void data_compression_haffman(std :: filesystem::path& filePath);