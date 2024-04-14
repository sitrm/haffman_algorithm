#include "haffman.hpp"



//---------------------------------
fs::path PATH = "../src_data/data1.txt";                        // путь до главного файла для кодирования 
fs::path path_decode = "../src_data/data1CompressHaffman.txt";  

map<char, string> haffman_codes;

//---------------------------------
int main() {
    
    print_dict(getFreqSymbols(read_file(PATH)));

    Node* root_tree = buildTreeHaffman(getFreqSymbols(read_file(PATH)));

    std :: cout << std::endl;
    
    generate_haffman_codes(root_tree, haffman_codes);
   
    for (auto& pair : haffman_codes) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
    std :: cout << std :: endl;
    

    data_compression_haffman(PATH);
    decoding_data_haffman_algo(path_decode, root_tree);

    return 0;
}

//---------------------------------

