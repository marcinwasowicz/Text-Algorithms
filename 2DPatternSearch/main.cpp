#include "PatternSearch.h"
#include "FileParser.h"

string ALPHABET = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM \n.,1234567890(){}[]|;':-\u0022";

vector<string> transpose(vector<string>& matrix){
    vector<string> result(matrix[0].length(), string());
    for(int i = 0; i<matrix.size(); i++){
        for(int j = 0; j<matrix[0].length(); j++){
            result[j] += matrix[i][j];
        }
    }
    return result;
}

void raportPatternSearch(vector<pair<int, vector<int>>>& searchResult){
    cout<<"lower right corners of pattern found at: "<<endl;
    for(auto pair : searchResult){
        cout<<"line: "<< pair.first<<" positions: ";
        for(auto position : pair.second){
            cout<<position<<" ";
        }
        cout<<endl;
    }
}

int main(){

    FileParser fileParser;
    clock_t start;
    clock_t end;

    char* path = "haystack.txt";
    vector<string> text = fileParser.parseFile(path);

    cout<<"Część pierwsza: Znajdź wszystkie wystąpienia: th w dwóch liniach pod rząd, t h w dwóch liniach pod rząd i wzorca p a t t e r n"<<endl;
    vector<string> pattern1 = {
            "th",
            "th"
    };
    pattern1 = transpose(pattern1);

    vector<string> pattern2 = {
            "t h",
            "t h"
    };
    pattern2 = transpose(pattern2);

    vector<string> pattern3 = {
            "p a t t e r n"
    };
    pattern3 = transpose(pattern3);

    PatternSearch patternSearch1(pattern1, ALPHABET);
    PatternSearch patternSearch2(pattern2, ALPHABET);
    PatternSearch patternSearch3(pattern3, ALPHABET);

    vector<pair<int, vector<int>>> result1 = patternSearch1.findPattern(text);
    vector<pair<int, vector<int>>> result2 = patternSearch2.findPattern(text);
    vector<pair<int, vector<int>>> result3 = patternSearch3.findPattern(text);

    cout<<"th w dwóch liniach pod rząd"<<endl;
    raportPatternSearch(result1);
    cout<<"t h w dwóch liniach pod rząd"<<endl;
    raportPatternSearch(result2);
    cout<<"p a t t e r n"<<endl;
    raportPatternSearch(result3);

    cout<<"Część druga: Znajdź wszystkie sytuacje, gdy taka sama literka występuje w dwóch liniach po rząd na tej samej pozycji"<<endl;
    string letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    for(auto letter : letters){
        vector<string> pattern = {
                string(1, letter),
                string(1, letter)
        };
        pattern = transpose(pattern);
        PatternSearch patternSearch(pattern, ALPHABET);
        auto result = patternSearch.findPattern(text);
        if(result.size()!=0){
            cout<<"literka: "<<letter<<endl;
            raportPatternSearch(result);
        }
    }

    cout<<"Część trzecia: Wybierz 4 małe literki i znajdź wystąpienia każdej z nich w załączonym tekście"<<endl;
    letters = "sxop";
    for(auto letter : letters){
        vector<string> pattern = {
                string(1, letter)
        };
        pattern = transpose(pattern);
        PatternSearch patternSearch(pattern, ALPHABET);
        auto result = patternSearch.findPattern(text);
        if(result.size()!=0){
            cout<<"literka: "<<letter<<endl;
            raportPatternSearch(result);
        }
    }

    cout<<"Część czwarta: porównaj czas boduwania automatu i wyszukiwania dla różnych rozmiarów wzorca"<<endl;

    char* patternPath4 = "wzorzec4.txt";
    char* patternPath5 = "wzorzec5.txt";
    char* patternPath6 = "wzorzec6.txt";

    vector<string> pattern4 = fileParser.parseFile(patternPath4);
    pattern4 = transpose(pattern4);
    vector<string> pattern5 = fileParser.parseFile(patternPath5);
    pattern4 = transpose(pattern4);
    vector<string> pattern6 = fileParser.parseFile(patternPath6);
    pattern4 = transpose(pattern4);

    start = clock();
    PatternSearch patternSearch4(pattern4, ALPHABET);
    end = clock();
    cout<<"Tworzenie automatu na wzorcu 4 zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    start = clock();
    PatternSearch patternSearch5(pattern5, ALPHABET);
    end = clock();
    cout<<"Tworzenie automatu na wzorcu 5 zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    start = clock();
    PatternSearch patternSearch6(pattern6, ALPHABET);
    end = clock();
    cout<<"Tworzenie automatu na wzorcu 6 zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    start = clock();
    auto result4 = patternSearch4.findPattern(text);
    end = clock();
    cout<<"Wyszukiwanie wzorca 4 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    start = clock();
    auto result5 = patternSearch5.findPattern(text);
    end = clock();
    cout<<"Wyszukiwanie wzorca 5 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    start = clock();
    auto result6 = patternSearch6.findPattern(text);
    end = clock();
    cout<<"Wyszukiwanie wzorca 6 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;

    cout<<"Część piąta: podziel podany tekst na 2, 4, 8 fragmentów w poziomie i teraz porównaj czas wyszukiwania wzorców z poprzedniej części"<<endl;
    cout<<"Dla każdego wzorca (4 - 6), dla każdego typu podziału pliku (na 2, 4, 8) wypisane będą po kolei czasy poszukiwania danego wzorca w każdym fragmencie pliku,"
          "z danego typu podziału"<<endl;

    int len = text.size();

    vector<vector<string>> twoSplit = {
            vector<string> (text.begin(), text.begin()+len/2), vector<string>(text.begin()+len/2+1, text.end())
    };

    vector<vector<string>> fourSplit = {
            vector<string> (text.begin(), text.begin()+len/4), vector<string>(text.begin()+len/4+1, text.begin()+len/2),
            vector<string> (text.begin()+len/2+1, text.begin()+3*len/4), vector<string>(text.begin()+3*len/4+1, text.end())
    };

    vector<vector<string>> eightSplit ={
            vector<string> (text.begin(), text.begin()+len/8), vector<string>(text.begin()+len/8+1, text.begin()+len/4),
            vector<string> (text.begin()+len/4+1, text.begin()+3*len/8), vector<string>(text.begin()+3*len/8+1, text.begin()+len/2),
            vector<string> (text.begin()+len/2+1, text.begin()+5*len/8), vector<string>(text.begin()+5*len/8+1, text.begin()+3*len/4),
            vector<string> (text.begin()+3*len/4+1, text.begin()+7*len/8), vector<string>(text.begin()+7*len/8+1, text.end())
    };

    cout<<"wzorzec 4"<<endl;
    cout<<"Podział na 2: "<<endl;
    for(auto it : twoSplit){
        start = clock();
        patternSearch4.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 4 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 4: "<<endl;
    for(auto it : fourSplit){
        start = clock();
        patternSearch4.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 4 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 8: "<<endl;
    for(auto it : eightSplit){
        start = clock();
        patternSearch4.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 4 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"wzorzec 5"<<endl;
    cout<<"Podział na 2: "<<endl;
    for(auto it : twoSplit){
        start = clock();
        patternSearch5.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 5 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 4: "<<endl;
    for(auto it : fourSplit){
        start = clock();
        patternSearch5.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 5 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 8: "<<endl;
    for(auto it : eightSplit){
        start = clock();
        patternSearch5.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 5 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"wzorzec 6"<<endl;
    cout<<"Podział na 2: "<<endl;
    for(auto it : twoSplit){
        start = clock();
        patternSearch6.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 6 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 4: "<<endl;
    for(auto it : fourSplit){
        start = clock();
        patternSearch6.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 6 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    cout<<"Podział na 8: "<<endl;
    for(auto it : eightSplit){
        start = clock();
        patternSearch6.findPattern(it);
        end = clock();
        cout<<"Wyszukiwanie wzorca 6 w tekście zajęło: "<<(end-start)*1000000/CLOCKS_PER_SEC<<" mikrosekund"<<endl;
    }

    return 0;
}