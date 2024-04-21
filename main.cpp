#include "haffman.hpp"

//---------------------------------
fs::path PATH = "../../src_data/data1.txt";                        // путь до главного файла для кодирования 
fs::path path_decode = "../../src_data/data1CompressHaffman.txt";  //декодирование 


//---------------------------------
int main() {
    //map<char, string> huffman_codes;
    // print_dict(getFreqSymbols(read_file(PATH)));

    // Node* root_tree = buildTreeHaffman(getFreqSymbols(read_file(PATH)));

    // std :: cout << std::endl;
    
    // generate_huffman_codes(root_tree, huffman_codes);
   
    // for (auto& pair : huffman_codes) {
    //     std::cout << pair.first << " : " << pair.second << std::endl;
    // }
    // std :: cout << std :: endl;
    
    map<char, string> dict_huffman_codes;
    dict_huffman_codes = encoding_data_haffman(PATH);
    
    decoding_data_haffman(path_decode, dict_huffman_codes);

    return 0;
}

//---------------------------------

