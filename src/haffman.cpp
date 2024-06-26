#include "haffman.hpp"
//--------------------------------------------------------------------


string read_file(const fs::path& filePath) {
// string read_file(const string & filePath) {   
    string data_file;
    string line;
    std::ifstream inFile;

    inFile.open(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error open file(read file)!\n";
        exit(EXIT_FAILURE);    // обработка исключений при октрытии файла 
    }
    
    std::stringstream buffer; // класс для работы с  потоками строк (быстрее)
    // rdbuf(), который возвращает указатель на буфер файла, и записываем его в объект buffer
    buffer << inFile.rdbuf();
    inFile.close();
    data_file = buffer.str();

    return data_file;
}
//--------------------------------------------------------------------


void writeFile(const fs::path& filePath, const string& data){
    std::ofstream outFile;
    outFile.open(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Unable to open file for writing: " << filePath << std::endl;
        exit(EXIT_FAILURE);
        return;
    }
    outFile << data;

    // Проверка успешности записи
    if (!outFile.good()) {
        std::cerr << "Error occurred while writing to file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    outFile.close();
}
//--------------------------------------------------------------------------


map<char, int> getFreqSymbols(const string file_data) {
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
void print_codes_symbols(map<char, int> huffman_codes){

    for (auto& pair : huffman_codes) {
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

void generate_huffman_codes(Node* root, map<char, string>& huffman_codes) {

    if (!root) return; // проверяем существование узла

    if (root->left) {
        root->left->code = root->code + "0";        //к существующему коду прибавляем "0"
        generate_huffman_codes(root->left, huffman_codes);
    }
    if (root->right) {
        root->right->code = root->code + "1";        //к существующему коду прибавляем "1"
        generate_huffman_codes(root->right, huffman_codes);
    }
    if (root->symbol != '\0') {    // если root это лист т е конечный символ в поддереве, то заносим его в словрь кодирования 
        huffman_codes[root->symbol] = root->code;
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
//кодирование данных переданных по пути в новый созданный файл; return: словарь кодирования/декодирования
map<char, string> encoding_data_haffman(fs::path& inFilePath){

    std::filesystem::path file_name = inFilePath.filename();            //  получаем имя файла из пути
    string filename = getFileNameWithoutExtension(file_name.string());    // обрезаем до рассширения 

    string new_name_file = filename + "CompressHaffman.txt";

    map<char, string> huffman_codes; // для хранения закодированных символов 
    std::ofstream outFile;

    string data_file = read_file(inFilePath); //читаем файл

    Node* root_tree = buildTreeHaffman(getFreqSymbols(data_file)); // передаем словрь {символ :  частота } и строим дерево хаффмана

    generate_huffman_codes(root_tree, huffman_codes);               //  обходим дерево и кодируем символы 

    inFilePath.replace_filename(new_name_file);       //переименовали в пути до файла имя файла на новое 
    // записываем в файл сжатую информацию
    outFile.open(inFilePath);
    if (!outFile.is_open()){
        std::cerr << "Error open file(compression)!\n";
        exit(EXIT_FAILURE);    // обработка исключений при октрытии файла 
    }
    //сжимаем в соответствии с кодами в словаре
    for( char ch : data_file){
        outFile << huffman_codes[ch];
    }

    outFile.close();
    std :: cout << "The digital stream has been successfully written to a file" << inFilePath << std::endl;
    return huffman_codes;
}

//-------------------------------------------------------------------------------------
// декодирование данных. Принимает путь до файла закодированных даннных и указатель на корень построенного дерева хаффмана
void decoding_data_haffman(fs::path& filePath,  map<char, string> huffman_codes){
    
    std::filesystem::path file_name = filePath.filename();            //  получаем имя файла из пути
    string filename = getFileNameWithoutExtension(file_name.string());    // обрезаем до рассширения 

    string data_file = read_file(filePath); 
    string new_name_file = filename + "decodeHaffman.txt";
    
    std::ofstream outFile;  
    filePath.replace_filename(new_name_file);

    outFile.open(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Error open file(compression)!\n";
        exit(EXIT_FAILURE);    // обработка исключений при октрытии файла 
    }

    string current_code;
    for (char bit : data_file){
        current_code +=bit;
        for (auto& code : huffman_codes){
            if (code.second == current_code){
                outFile << code.first;
                current_code.clear();
                break;
            }
        }
    }
    outFile.close();  // не забываем закрыть файл
    std :: cout << "The digital stream has been successfully decoded and written to a file" << filePath << std::endl;
}

////так как в словре кодирования "1010" это строки, то необходимо перевести в вектор bool.Но пока не понятно зачем???....
std::vector<bool> stringToVectorBool(const std::string &str){
    std::vector<bool> vectorBool;

    for( char ch : str){
        if(ch == '0'){
            vectorBool.push_back(false);
        }else if( ch == '1'){
            vectorBool.push_back(true);
        }else{
            std::cerr << "Error symbol(not 1/0)"<< std::endl;
            return{};
        }

    }
    return vectorBool;
}