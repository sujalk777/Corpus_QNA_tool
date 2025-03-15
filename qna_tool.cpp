
#include <sstream>
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

QNA_tool::QNA_tool(){
    // Implement your function here  
    // word_matches.resize(100);  
    // for(size_t i=0; i<word_matches.size(); i++){
    //     word_matches[i].matches = nullptr;
    //     word_matches[i].frequency = 0;
    // }
    // word_in_para.resize(100);
    // para_score.resize(100);
    size = 0;
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

bool QNA_tool::separator(char c){
    string separators = ". , - : ! \" \' ( ) ? [ ] ; @";
    return separators.find(c) != string::npos;
}

void QNA_tool::sortpara(vector<new_node*> vec, int k){
    int n= vec.size();
    for(int i=0;i<n-1;++i){
        for(int j=0;j<n-i-1;++j){
            if(vec[j]->word_count < vec[j+1]->word_count){
                new_node* temp=vec[j];
                vec[j]=vec[j+1];
                vec[j+1]=temp;
            }
            else if (vec[j]->word_count == vec[j + 1]->word_count){
                if (vec[j]->diff_words > vec[j+1]->diff_words ){
                        new_node* temp = vec[j];
                        vec[j] = vec[j + 1];
                        vec[j + 1] = temp;
                }
            }
        }
    }
}

long long QNA_tool::gen_freq(string wordFreq){
    ifstream file("unigram_freq.csv");

    if(!file.is_open()){
        return -1;
    }

    string line;
    getline(file, line);

    while(getline(file, line)){
        istringstream ss(line);
        string words;
        int count;

        if(getline(ss, words, ',')){
            if(ss >> count){
                if(words == wordFreq ){
                    return count;
                }
            }
        }
    }
    return 0;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){

    // Implement your function here  
    search.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    //dict.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
    
    return;
}

bool QNA_tool::in_para_score(vector<new_node*> vec, new_node* Word){
    for(const auto& para: vec){
        if(para->book_no == Word->book_no && para->page_no == Word->page_no && para->para_no == Word->para_no){
            return true;
        }
    }
    return false;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    //convert question into words
    string word = "";
    vector<string> words;
    size_t j = 0;
    for(size_t i=0; i<question.length(); i++){
        char c = question[i];
        if(separator(c) || question[i] == ' '){
            if(!word.empty()){
                words.push_back(word);
                j++;
                word = "";
            }
        }
        else{
            word.push_back(tolower(c));
        }
    }
    if(!word.empty()){
        words.push_back(word);
        j++;
        word = "";
    }

    //search for every word of the query; frequency = total word count of the word in copus
    cout << "BOEING\n";
    vector<match> word_matches(j);
    for(size_t i=0; i<j; i++){
        int n = 0;
        Node* new_match = search.search(words[i],n);
        if(new_match != nullptr){
            if(size > j){
                cout << "OHNO\n";
            }
            word_matches[size].matches = new_match;
            word_matches[size].frequency = n;
            size++;
        }
    }
    cout << "TOEING\n";

    //counting word count of each word for each paragraph
    vector<new_node*> word_in_para(size);
    for(size_t i=0; i<size; i++){
        Node* current = word_matches[i].matches;
        if(current != nullptr){
            new_node* node = new new_node(current->book_code, current->page, current->paragraph);
            while(current->right != nullptr){
                if(current->book_code == current->right->book_code && current->page == current->right->page && current->paragraph == current->right->paragraph){
                    current = current->right;
                    node->word_count++;
                }
                else{
                    current = current->right;
                    node->next = new new_node(current->book_code, current->page, current->paragraph);
                }
            }
            word_in_para.push_back(node);
        }
    }
    cout << "FROM\n";

    //score the word
    double* wordScore = new double[size];
    for(size_t i=0; i<size; i++){
        if(word_matches[i].matches != nullptr){
            long long freq = gen_freq(words[i]);
            double num = word_matches[i].frequency + 1;
            long den = freq + 1;
            wordScore[i] = num/den;
        }
    }
    cout << "THE\n";
    //score the para
    vector<new_node*> para_score;
    for(size_t i=0; i<size; i++){
        new_node* current = word_in_para[i];
        if(!in_para_score(para_score,current)){
            new_node* node = new new_node(current->book_no,current->page_no,current->para_no);
            node->word_count = wordScore[i]*current->word_count;
            for(size_t j=0; j<size; j++){
                new_node* nodes = word_in_para[j];
                if(nodes != current){
                    while(nodes->book_no == current->book_no && nodes->page_no == current->page_no && nodes->para_no == current->para_no && nodes->next != nullptr){
                        nodes = nodes->next;
                    }
                    if(nodes->book_no == current->book_no && nodes->page_no == current->page_no && nodes->para_no == current->para_no){
                        node->word_count += wordScore[j]*nodes->word_count;
                    }
                }
            }
            para_score.push_back(node);
        }
    }
    cout << "CHILLAX\n";

    sortpara(para_score,k);
    int total_para = para_score.size();
    if(k<total_para){
        para_score.resize(k);
    }

    Node* head = new Node(para_score[0]->book_no,para_score[0]->page_no,para_score[0]->para_no,0,0);
    head->left = nullptr;
    Node* current_node = head;

    for(int i=1; i<k; i++){
        Node* temp = new Node(para_score[i]->book_no,para_score[i]->page_no,para_score[i]->para_no,0,0);
        current_node->right = temp;
        temp->left = current_node;
        current_node = temp;
    }
    cout << "BABAB\n";

    return head;
}

void QNA_tool::query(string question, string filename){
    // Implement your function here

    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY){

    // first write the 3 paragraphs into different files
    Node* traverse = root;
    int num_paragraph = 0;
    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        ofstream outfile(p_file);
        string paragraph;
        get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "Hey GPT, What is the purpose of life?";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // python3 <filename> API_KEY paragraph_0.txt paragraph_1.txt paragraph_2.txt query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += "paragraph_0.txt";
    command += " ";
    command += "paragraph_1.txt";
    command += " ";
    command += "paragraph_2.txt";
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}
