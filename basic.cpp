
// #include <assert.h>
// #include <sstream>
// #include "qna_tool.h"
// using namespace std;

// bool CheckPunctuation2(char c)
// {
//     switch (c)
//     {
//     case ' ':
//     case '.':
//     case ',':
//     case '-':
//     case ':':
//     case '!':
//     case '\"':
//     case '\'':
//     case '(':
//     case ')':
//     case '?':
//     case '[':
//     case ']':
//     case ';':
//     case '@':
//         return true;
//     default:
//         return false;
//     }
// }

// long long StoLL(string s)
// {
//     long long ans = 0;
//     int n = s.size();
//     for (int i = 0; i < n; i++)
//     {
//         ans += (s[i] - '0');
//         ans *= 10;
//     }
//     ans /= 10;
//     return ans;
// }

// QNA_tool::QNA_tool()
// {
//     TotalUniv = new Dict();
//     TotalCorpus = new Dict();

//     ifstream file("unigram_freq.csv");
//     string line;
//     bool checker = 0;
//     while (getline(file, line))
//     {
//         stringstream ss(line);
//         vector<string> Row;
//         string C;
//         while (getline(ss, C, ','))
//             Row.push_back(C);
//         if (checker)
//         {
//             TotalUniv->Storage->root = TotalUniv->Storage->insert(TotalUniv->Storage->root, Row[0]);
//             TotalUniv->Storage->search(TotalUniv->Storage->root, Row[0])->weight = StoLL(Row[1]);
//         }
//         checker = 1;
//         if (line == "")
//             break;
//     }
//     file.close();
// }

// QNA_tool::~QNA_tool()
// {
//     delete TotalUniv;
//     delete TotalCorpus;
//     ParagraphWords.clear();
// }

// void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
// {
//     TotalCorpus->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
//     vector<int> Info = {book_code, page, paragraph};
//     if (ParagraphWords.empty() || ParagraphWords.back().first != Info)
//     {
//         Dict *D = new Dict();
//         D->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
//         ParagraphWords.push_back({Info, D});
//     }
//     else
//         ParagraphWords.back().second->insert_sentence(book_code, page, paragraph, sentence_no, sentence);
//     return;
// }

// void Lowercase2(string &word)
// {
//     for (char &c : word)
//         if (c <= 'Z' && c >= 'A')
//             c = c - ('A' - 'a');
// }

// Node *QNA_tool::get_top_k_para(string question, int k)
// {
//     vector<string> words;
//     string word = "";
//     for (char c : question)
//     {
//         if (CheckPunctuation2(c) == true && word != "")
//         {
//             Lowercase2(word);
//             words.push_back(word);
//             word = "";
//         }
//         else if (CheckPunctuation2(c) == false)
//             word = word + c;
//     }
//     if (word != "")
//     {
//         Lowercase2(word);
//         words.push_back(word);
//         word = "";
//     }
//     MaxHeap M = MaxHeap();
//     for (pair<vector<int>, Dict *> P : ParagraphWords)
//     {
//         long double score = 0;
//         Dict *D = P.second;
//         for (string word : words)
//         {
//             long double num = D->get_word_count(word) * (TotalCorpus->get_word_count(word) + 1);
//             long double den = TotalUniv->get_word_count(word) + 1;
//             score += (num / den);
//         }
//         Node *node = new Node(P.first[0], P.first[1], P.first[2], 0, 0);
//         M.push({node, score});
//     }
//     LL *Answer = new LL();
//     for (int i = 0; i < k; i++)
//     {
//         Node *node = M.top();
//         M.pop();
//         Answer->insert(node->book_code, node->page, node->paragraph, node->sentence_no, node->offset);
//     }
//     return Answer->head;
// }

// string QNA_tool::get_paragraph(int book_code, int page, int paragraph)
// {
//     string filename = "mahatma-gandhi-collected-works-volume-";
//     filename += to_string(book_code);
//     filename += ".txt";

//     ifstream inputFile(filename);

//     string tuple;
//     string sentence;

//     if (!inputFile.is_open())
//     {
//         cerr << "Error: Unable to open the input file " << filename << "." << endl;
//         exit(1);
//     }

//     string res = "";

//     while (getline(inputFile, tuple, ')') && getline(inputFile, sentence))
//     {
//         tuple += ')';
//         int metadata[5];
//         istringstream iss(tuple);
//         string token;
//         iss.ignore(1);
//         int idx = 0;
//         while (getline(iss, token, ','))
//         {
//             size_t start = token.find_first_not_of(" ");
//             size_t end = token.find_last_not_of(" ");
//             if (start != string::npos && end != string::npos)
//                 token = token.substr(start, end - start + 1);
//             if (token[0] == '\'')
//             {
//                 int num = stoi(token.substr(1, token.length() - 2));
//                 metadata[idx] = num;
//             }
//             else
//             {
//                 int num = stoi(token);
//                 metadata[idx] = num;
//             }
//             idx++;
//         }
//         if (metadata[0] == book_code && metadata[1] == page && metadata[2] == paragraph)
//             res += sentence;
//     }
//     inputFile.close();
//     return res;
// }

// void QNA_tool::query(string question, string filename)
// {
//     string temp = "";
//     vector<string> unnecessary_words={"or","a","and","the","this","on","for"};
//     string word = "";
//     for (char c : question)
//     {
//         if (CheckPunctuation2(c) == true && word != "")
//         {
//             Lowercase2(word);
//             if(find(unnecessary_words.begin(),unnecessary_words.end(),word)==unnecessary_words.end()){
//                 temp+=(word); 
//             }
//             word = "";
//         }
//         else if (CheckPunctuation2(c) == false)
//             word = word + c;
//     }
//     if (word != "")
//     {
//         Lowercase2(word);
//         if(find(unnecessary_words.begin(),unnecessary_words.end(),word)==unnecessary_words.end()){
//             temp+=(word); 
//         }
//         word = "";
//     }
//     question=temp;
//     Node *node = get_top_k_para(question, 8);
//     query_llm(filename, node, 8, "COL106-Assignment7", question);
//     return;
// }

// void QNA_tool::query_llm(string filename, Node *root, int k, string API_KEY, string question)
// {
//     Node *traverse = root;
//     int num_paragraph = 0;

//     while (num_paragraph < k)
//     {
//         assert(traverse != nullptr);
//         string p_file = "paragraph_";
//         p_file += to_string(num_paragraph);
//         p_file += ".txt";
//         remove(p_file.c_str());
//         ofstream outfile(p_file);
//         string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
//         assert(paragraph != "$I$N$V$A$L$I$D$");
//         outfile << paragraph;
//         outfile.close();
//         traverse = traverse->right;
//         num_paragraph++;
//     }

//     ofstream outfile("query.txt");
//     outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
//     outfile << question;
//     outfile << " Give me a detailed answer, and add some details too that you think are related to this, but is not included here.\n";
//     outfile.close();

//     string command = "python3 ";
//     command += filename;
//     command += " ";
//     command += API_KEY;
//     command += " ";
//     command += to_string(k);
//     command += " ";
//     command += "query.txt";
//     system(command.c_str());
//     return;
// }#include <assert.h>
