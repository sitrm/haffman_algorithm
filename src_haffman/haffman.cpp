#include "haffman.h"
//--------------------------------------------------------------------


string read_file(const std::filesystem::path& filePath) {
// string read_file(const string & filePath) {   
    string data_file;
    string line;
    std::ifstream inFile;

    inFile.open(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error open file(read file)!\n";
        exit(EXIT_FAILURE);    // обработка исключений при октрытии файла 
    }
    while (getline(inFile, line)) {    // читаем по строчно 

        data_file += line;           // +"\n";
    }
    inFile.close();

    return data_file;
}
//--------------------------------------------------------------------

map<char, int> getFreqSymbols(string file_data) {
    map<char, int> dict_freq;       // словаря { символ : частота}
    for (char ch : file_data) {
        if (dict_freq.count(ch)) {  // если символ уже есть в словере
            dict_freq[ch] += 1;
        }
        else {
            dict_freq[ch] = 1;      // иначе создаем 
        }
    }
    return dict_freq;
}

//--------------------------------------------------------------------
void print_dict(map<char, int> dict_freq) {
    for (auto iter(dict_freq.begin()); iter != dict_freq.end(); iter++) {
        std::cout << iter->first << " : " << iter->second << std::endl;
    }
}
//--------------------------------------------------------------------
void print_codes_symbols(map<char, int> haffman_codes){

    for (auto& pair : haffman_codes) {
    std::cout << pair.first << " : " << pair.second << std::endl;
    }
    std::cout << std::endl;
}

//--------------------------------------------------------------------
Node::Node(char ch, int w) : symbol(ch), weight(w), code(""), left(nullptr), right(nullptr) {
    //здесь можно по другому инициализировать поля
}
//--------------------------------------------------------------------
Node* buildTreeHaffman(map<char, int> symbols_freq) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> heap;   // приоритетная очередь FIFO
    //template <class Type, class Container= vector <Type>, class Compare= less <typename Container ::value_type>>
    //CompareNodes устанавливает критерий сравнения в кучи

    //создаем узлы для каждого символа из словаря 
    for (auto iter(symbols_freq.begin()); iter != symbols_freq.end(); iter++) {
        Node* node = new Node(iter->first, iter->second);   //Node(ch, int freq)
        heap.push(node);
    }
    //алгоритм построения дерева 
    while (heap.size() > 1) {
        Node* left = heap.top();  // Возвращает константную ссылку на наибольший элемент в верхней части priority_queue.
        heap.pop();
        Node* right = heap.top();
        heap.pop();

        Node* combined_nodes = new Node('\0', left->weight + right->weight);

        combined_nodes->left = left;  // указатеть у узла объединенных элементов на левого потомка
        combined_nodes->right = right;  // на правого 

        heap.push(combined_nodes);
    }
    return heap.top();  // указатель на корень дерева
}
//--------------------------------------------------------------------

void generate_haffman_codes(Node* root, map<char, string>& haffman_codes) {

    if (!root) return; // проверяем существование узла

    if (root->left) {
        root->left->code = root->code + "0";        //к существующему коду прибавляем "0"
        generate_haffman_codes(root->left, haffman_codes);
    }
    if (root->right) {
        root->right->code = root->code + "1";        //к существующему коду прибавляем "1"
        generate_haffman_codes(root->right, haffman_codes);
    }
    if (root->symbol != '\0') {    // если root это лист т е конечный символ в поддереве, то заносим его в словрь кодирования 
        haffman_codes[root->symbol] = root->code;
    }

}
//--------------------------------------------------------------------
//name.txt -> name
string getFileNameWithoutExtension(const std::string& fileName) {
    
    size_t lastPos = fileName.find_last_of(".");
    if (lastPos != std::string::npos) {
        return fileName.substr(0, lastPos);
    }
    return fileName;
}


//--------------------------------------------------------------------
void data_compression_haffman(std::filesystem::path& inFilePath){

    std::filesystem::path file_name = inFilePath.filename();
    string filename = getFileNameWithoutExtension(file_name.string());

    string new_name_file = filename + "CompressHaffman.txt";

    map<char, string> haffman_codes; // для хранения закодированных символов 
    std::ofstream outFile;

    string data_file = read_file(inFilePath); //читаем файл

    Node* root_tree = buildTreeHaffman(getFreqSymbols(data_file)); // передаем словрь {символ :  частота } и строим дерево хаффмана

    generate_haffman_codes(root_tree, haffman_codes);               //  обходим дерево и кодируем символы 

    inFilePath.replace_filename(new_name_file);
    // записываем в файл сжатую информацию
    outFile.open(inFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Error open file(compression)!\n";
        exit(EXIT_FAILURE);    // обработка исключений при октрытии файла 
    }
    //сжимаем в соответствии с кодами в словаре
    for( char ch : data_file){
        outFile << haffman_codes[ch];
    }

    outFile.close();
}