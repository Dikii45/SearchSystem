// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
// 271
// Закомитьте изменения и отправьте их в свой репозиторий.

#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <set>
#include <string>
#include <utility>
#include <vector>


using namespace std;


const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}


struct Document {
    int id;
    double relevance;
};


struct Query {
    set<string> plus_words;
    set<string> minus_words;
};


class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }


    void AddDocument(int document_id, const string& document) {
        number_documents_++; // подсчет документов

        vector<string> words_document = SplitIntoWordsNoStop(document);

        // этот коофицент нужен для посчета TF
        double coefficient_word = double(1) / words_document.size();


        for (const auto& word : words_document) {
            word_to_document_freqs_[word].insert({ document_id, coefficient_word });
        }
    }


    // находим топ документов по релевантности (по запросу)  
    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query_words = ParseQuery(raw_query);         // разделение на + и - слова
        auto matched_documents = FindAllDocuments(query_words);  // находим релевантность документов по запросу

        // сортирую по релевантности
        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        return matched_documents;
    }

private:

    // кличество документов
    int number_documents_ = 0;


    map<string, map<int, double>> word_to_document_freqs_;


    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    // разделяем текст на + и - слова
    Query ParseQuery(const string& text) const {
        Query query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (word[0] == '-') {                  // нахожу минус слово
                string word_munis;

                // удаляю первый элемент
                for (int i = 1; i < word.size(); i++) {
                    word_munis.push_back(word[i]);
                }
                query_words.minus_words.insert(word_munis);
            }
            else {
                query_words.plus_words.insert(word);
            }
        }
        return query_words;
    }



    double СountIDF()







        vector<Document> FindAllDocuments(const Query& query_words) const {


        vector<Document> documents;             // для возращения всех id документов и их релевантности
        map<int, double> document_to_relevance;



        for (const auto& plus_word : query_words.plus_words) {
            if (word_to_document_freqs_.count(i) == true) {                          //если +слово найдено среди слов документа тогда считаем окончательную релевантость
                for (const auto& pair_word : word_to_document_freqs_.at(plus_word)) {

                    // IDF -> количество всего документов / на количесво встречаемых в документах
                    double IDF = log(double(number_documents_) / word_to_document_freqs_.at(plus_word).size());

                    document_to_relevance[pair_word.first] += (IDF * pair_word.second);       // переумножение IDF и TF
                }
            }
        }

        // удаление контейнеров с минус словами
        for (const auto& minus_word : query_words.minus_words) {
            if (word_to_document_freqs_.count(minus_word) == true) {
                for (const auto& j : word_to_document_freqs_.at(minus_word)) {
                    document_to_relevance.erase(j.first);
                }
            }
        }


        for (const auto& i : document_to_relevance)
        {
            documents.push_back({ i.first, i.second });
        }

        return documents;
    };


};





 

SearchServer CreateSearchServer() {
    SearchServer search_server;  
    search_server.SetStopWords(ReadLine());            // ввод стоп слов

    const int document_count = ReadLineWithNumber();        // считывание количество документов
    // запись всех документов
    for (int document_id = 0; document_id < document_count; ++document_id) { 
        search_server.AddDocument(document_id, ReadLine());        // записываем TF каждого слово 
    }

    return search_server;
}

int main() { 

    // добавить const
    const SearchServer search_server = CreateSearchServer(); // записываем документы и слова

    const string query = ReadLine(); // записываем слова по которым нужно найти 
    for (const auto& i : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << i.id << ", "
            << "relevance = "s << i.relevance << " }"s << endl;
    }
}
