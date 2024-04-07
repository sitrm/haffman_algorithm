#include "haffman.h"

using namespace std;

//---------------------------------
filesystem::path PATH = "../src_data/data1.txt";

map<char, string> haffman_codes;

//---------------------------------
int main() {
    print_dict(getFreqSymbols(read_file(PATH)));

    Node* root_tree = buildTreeHaffman(getFreqSymbols(read_file(PATH)));

    cout << endl;
    
    generate_haffman_codes(root_tree, haffman_codes);
   
    for (auto& pair : haffman_codes) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
    cout << endl;
    

    data_compression_haffman(PATH);

    return 0;
}

//---------------------------------

