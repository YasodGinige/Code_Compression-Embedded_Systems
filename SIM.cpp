//180195A-Y.S Ginige
/*On my honor, I have neither given nor received unauthorized aid on this assignment*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <bitset>
#include <sstream>
#include <math.h>
using namespace std;


vector<string> Org_entries;
vector<string> Dicom_string;
vector<string> Dictionary;
vector<string> Org_entries_Compressed;
string Comp_string;
string compressedData;
map<string, string> Decom_Dictionay;
map<string, vector<int>> Sorted_list;
map<string, string> DictionaryDic;


void reverseStr(string& str)     // fuction to reverse a given string
{
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

////////////////////////////////////////////////////////////////////////  

int Dec_convert(string m) {           // convert bin_num numbers to decimal
  int dec=0;
  reverseStr(m);

  for (int j=0;j<m.length();j++){
    dec+=(m[j]-'0')*pow(2,j);
  }
  return dec;
}


string Bin_convert( int n, int len){    // converst decimal to bin_num
int  i;
vector<int> a;

for(i=0; n>0; i++){
    a.insert(a.begin(), n%2);       
    n= n/2;  
  }

  ostringstream os;
  for (int kk: a) {
      os << kk;
  }
  string str(os.str());
  while (i<len){
    str="0"+str;
    i++;
  }

  return str;  
}  

////////////////////////////////////////////////////////////////////////

string XOR(string a, string b, int n){       //get XOR between two strings
  string ans = "";
  for (int i = 0; i < n; i++)
  {
      // If the Character matches
      if (a[i] == b[i])
          ans += "0";
      else
          ans += "1";
  }
  return ans;
}


int bin_numSearch(string arr[], string x,int n)     //Search and element in an string and return its index
    { 
      for (int j=0;j<8;j++){
          if (arr[j]==x){
            return j;
          }
      }
      
      return -1;
    }

///////////////////////////////////////////////////////////////////////

int findSum(string str)        //function to calculate the sumation of a string
{
    int sum = 0;
    for (char ch : str){
        if (isdigit(ch)){
            sum += (ch - '0');
        }
    }
    return sum;
}

bool check_consecutive(string Word){      //check whether two 1's occurs consecutively in a string 
  for (int i=0;i<Word.length()-1;i++) {
    if (Word.substr(i,2)=="11")
      return true;
  }
  return false;
}

bool check_bitmask(string Word){         //check whether a 4 bit bitmask is applicable to a given string
  for (int i=0;i<Word.length()-3;i++) {
    if (findSum(Word.substr(i,4))==2)
      return true;
  }
  return false;
}



///////////////////////////////////////////////////////////////////////////////////////////

void Read_compressed_File(){                    // Function to read the compressed file
                                                // and seperate data
  string myText;
  ifstream MyFile("compressed.txt");

  while (getline (MyFile, myText)) {
    Org_entries_Compressed.push_back(myText);
  }
  MyFile.close();

  bool notBreaked = true;
  int tempIndex = 0;
  for (auto & line : Org_entries_Compressed){
    if (line != "xxxx"){
      compressedData += line;
      tempIndex +=1;
    }else {
      break;
    }
  }
  int index = 0;
  for (int i=tempIndex + 1; i< Org_entries_Compressed.size(); ++i){
    string bin_num = Bin_convert(index,3);
    Decom_Dictionay[bin_num] = Org_entries_Compressed[i];
    index +=1;
  }
}




void Creat_Dictionary(){                      // Function to create the dictionary to a given file

  string myText;
  ifstream MyReadFile("original.txt");

  while (getline (MyReadFile, myText)) {
    Org_entries.push_back(myText);
  }
  MyReadFile.close();

  int count = 1;
  for (auto & Entry : Org_entries){
    if (Sorted_list.find(Entry) != Sorted_list.end()){
      Sorted_list[Entry][0] ++ ;
    }
    else Sorted_list[Entry] = {1,count};
    count += 1;
  }
  
  map<int, map<int, string>> Temp_list;
  for (auto  & element : Sorted_list) {
    Temp_list[element.second[0]].insert({element.second[1], element.first} );
  }

  map<int, map<int, string>>::reverse_iterator it =  Temp_list.rbegin();
  for (it = Temp_list.rbegin() ; it != Temp_list.rend(); ++it) {
    for (auto & data : it -> second){
      Dictionary.push_back(data.second);
    }
  }

  for (int i=0; i<8; i++){
    string bin_num = Bin_convert(i,3);
    DictionaryDic[Dictionary[i]] = bin_num;
  }
}




////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////    Decompression  ////////////////////////////////////////////////////////




void Decode_file(){                             //Function that decompress the file
  int strIndex = 0;
  string New_bits;
  bool Completed = true;

  while (Completed){
    string Code = compressedData.substr(strIndex,3);

    if (Code == "000"){
      string runLength = compressedData.substr(strIndex + 3,2);
      for (int i=0; i< ((runLength == "00") ? 1 : (runLength == "01") ? 2 : (runLength == "10") ? 3 : 4); ++i){
        Dicom_string.push_back(New_bits);

      }
      strIndex += 5;

    }else if (Code == "001"){
                          
      int startLocation = Dec_convert(compressedData.substr(strIndex + 3,5));

      string Dictionary_entry = Decom_Dictionay[compressedData.substr(strIndex + 12,3)];
      bitset Dictionary_entryBits = bitset<4>(Dictionary_entry.substr(startLocation,4));
      bitset bitmask = bitset<4>(compressedData.substr(strIndex + 8, 4));
      string xorBits = (bitmask ^ Dictionary_entryBits).to_string();

      New_bits = Dictionary_entry.substr(0,startLocation) + xorBits + Dictionary_entry.substr(startLocation + 4);
      Dicom_string.push_back(New_bits);
      strIndex += 15;

    }else if (Code == "010"){
      int mismatchLocation = bitset<5>(compressedData.substr(strIndex + 3,5)).to_ullong();

      string Dictionary_entry = Decom_Dictionay[compressedData.substr(strIndex + 8,3)];
      string mismatchBit = (Dictionary_entry[mismatchLocation] == *"0") ? "1" : "0";

      New_bits = Dictionary_entry.substr(0,mismatchLocation) + mismatchBit + Dictionary_entry.substr(mismatchLocation + 1);
      Dicom_string.push_back(New_bits);
      strIndex += 11;

    }else if (Code == "011"){
      int startLocation = Dec_convert(compressedData.substr(strIndex + 3,5));

      string Dictionary_entry = Decom_Dictionay[compressedData.substr(strIndex + 8,3)];
      string mismatchBits = Dictionary_entry.substr(startLocation, 2);
      string rightBits = (mismatchBits == "00") ? "11" : ((mismatchBits == "01") ? "10" : ((mismatchBits == "10") ? "01" : "00"));

      New_bits = Dictionary_entry.substr(0,startLocation) + rightBits + Dictionary_entry.substr(startLocation + 2);
      Dicom_string.push_back(New_bits);
      strIndex += 11;

    }else if (Code == "100"){
      int Loc_1 = Dec_convert(compressedData.substr(strIndex + 3,5));
      int Loc_2 = Dec_convert(compressedData.substr(strIndex + 8,5));

      string Dictionary_entry = Decom_Dictionay[compressedData.substr(strIndex + 13,3)];
      string Bit_1_error = (Dictionary_entry[Loc_1] == *"0") ? "1" : "0";
      string Bit_2_error = (Dictionary_entry[Loc_2] == *"0") ? "1" : "0";

      New_bits = Dictionary_entry.substr(0,Loc_1) + Bit_1_error + Dictionary_entry.substr(Loc_1 + 1, Loc_2-Loc_1-1) + Bit_2_error + Dictionary_entry.substr(Loc_2 + 1);
      Dicom_string.push_back(New_bits);
      strIndex += 16;

    }else if (Code == "101"){
      string Dictionary_entry = Decom_Dictionay[compressedData.substr(strIndex + 3,3)];
      New_bits = Dictionary_entry;
      Dicom_string.push_back(New_bits);
      strIndex += 6;

    }else if (Code == "110"){
      New_bits = compressedData.substr(strIndex + 3, 32);
      Dicom_string.push_back(New_bits);
      strIndex += 35;

    }else {
      Completed = false;
    }
  }

  ofstream MyFile("dout.txt");
  int tempCount = 0;
  for (auto & line : Dicom_string){
    MyFile << line;
    tempCount +=1;
    if (tempCount < Dicom_string.size()){
      MyFile << "\n";
    }
  }
  MyFile.close();


}



//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////   Compression  /////////////////////////////////////////////


void Encode_file(){                                   // Functionn to encode a file
  string Prev_bits;
  string New_bits;
  int repeateCount = 0;
  for (auto & Entry : Org_entries){
    string Encoded_bits;
    if (DictionaryDic.find(Entry) != DictionaryDic.end()){
      Encoded_bits = "101" + DictionaryDic[Entry];
    }else {
      bool One_bit_error = false;
      for(string::size_type i = 0; i < Entry.size(); ++i) {
        string tempEntry = Entry.substr(0,i) + ((Entry[i]==*"0") ? "1":"0") + Entry.substr(i+1);
        // cout << tempEntry << "\n";
        if (DictionaryDic.find(tempEntry) != DictionaryDic.end()){
          Encoded_bits = "010" + Bin_convert(i,5) + DictionaryDic[tempEntry];
          One_bit_error = true;
        }
        if (One_bit_error){
          break;
        }
      }
      if (!One_bit_error){
        bool Two_bit_cons_error = false;
        for(string::size_type j = 0; j < (Entry.size()-1); ++j) {
          //string temp__String="";
          string tempEntry1;
          if (Entry.substr(j,2)=="00")
            tempEntry1 = Entry.substr(0,j) + "11" + Entry.substr(j+2);
          else if((Entry.substr(j,2)=="01"))
            tempEntry1 = Entry.substr(0,j) + "10" + Entry.substr(j+2);
          else if((Entry.substr(j,2)=="10"))
            tempEntry1 = Entry.substr(0,j) + "01" + Entry.substr(j+2);
          else
            tempEntry1 = Entry.substr(0,j) + "00" + Entry.substr(j+2);;
          
          //string tempEntry1 = Entry.substr(0,j) + temp__String + Entry.substr(j+2);
          // cout << tempEntry1 << "   " << Entry.substr(j,2) << "\n";
          if (DictionaryDic.find(tempEntry1) != DictionaryDic.end()){
            Encoded_bits = "011" + Bin_convert(j,5) + DictionaryDic[tempEntry1];
            Two_bit_cons_error = true;
          }
          if (Two_bit_cons_error){
            break;
          }
        }
        if (!Two_bit_cons_error){
          bool bitmask = false;
          map<int, string> tempBitmasks;
          string Dictionary_Index;
          for(auto & Dictionary_entry : DictionaryDic) {

            for (string::size_type k = 0; k < (Dictionary_entry.first.size()-3); ++k){
              string tempDictionary_entry = Dictionary_entry.first.substr(0,k) + Dictionary_entry.first.substr(k+4);
              string tempEntry = Entry.substr(0,k) + Entry.substr(k+4);

              if (tempDictionary_entry == tempEntry){
                bitset EntryBits = bitset<4>(Entry.substr(k,4));
                bitset Dictionary_entryBits = bitset<4>(Dictionary_entry.first.substr(k,4));
                string xorBits = (EntryBits ^ Dictionary_entryBits).to_string();
                bitmask = true;
                tempBitmasks[k] = xorBits;}}
            if (bitmask){
              Dictionary_Index = Dictionary_entry.second;
              break;
            }
          }

          if (bitmask){
            bool select = false;
            int key;
            string val;
            for (auto & bm : tempBitmasks){
              if (bm.second[0] == *"1"){
                select = true;
                key = bm.first;
                val = bm.second;
              }
              if (select){
                break;
              }
            }


            if (!select){
              for (auto & h : tempBitmasks){
                key = h.first;
                val = h.second;
                break;
              }
            }

            Encoded_bits = "001" + Bin_convert(key,5) + val + Dictionary_Index;
          }else {

            bool double_error = false;
            for (string::size_type i = 0; i < Entry.size(); ++i){
              string firstChange = Entry.substr(0,i) + ((Entry[i]==*"0") ? "1":"0") + Entry.substr(i+1);
              for (string::size_type j = 0; j < firstChange.size(); ++j){
                string secondChange = firstChange.substr(0,j) + ((firstChange[j]==*"0") ? "1":"0") + firstChange.substr(j+1);
                if (DictionaryDic.find(secondChange) != DictionaryDic.end()){
                  double_error = true;
                  Encoded_bits = "100" + Bin_convert(i,5) + Bin_convert(j,5) + DictionaryDic[secondChange];
                  break;}}
              if (double_error)
                break;}
            if (!double_error)
              Encoded_bits = "110" + Entry;
            }}}
    }

    if (Encoded_bits == Prev_bits){
      if (repeateCount < 4){
        repeateCount += 1;
      } else {                                        
        New_bits = Encoded_bits + "000" + Bin_convert(repeateCount-1,2);
        repeateCount = 0 ;
      }
    }else if (repeateCount != 0){
      New_bits = Prev_bits + "000" + Bin_convert(repeateCount-1,2);
      repeateCount = 0;
      Prev_bits = Encoded_bits;
    }else if (repeateCount == 0){
      New_bits = Prev_bits;
      Prev_bits = Encoded_bits;
    }
    if (repeateCount == 0){
      Comp_string = Comp_string + New_bits;
    } 
  }

  if (Prev_bits != New_bits){
    Comp_string = Comp_string + Prev_bits;
  }

  if (repeateCount != 0) {
    Comp_string = Comp_string + "000" + Bin_convert(repeateCount,2);
  }

  if (Comp_string.size() % 32 != 0){
    for (int i=0; i<(Comp_string.size() % 32); ++i){
      Comp_string += "1";
    }  
  }

  ofstream MyFile("cout.txt");
  for (int j=0; j<(Comp_string.size() / 32); ++j){
    MyFile << Comp_string.substr(j*32,32) << "\n";
  }
  
  MyFile << "xxxx\n";
  for (int i = 0; i< DictionaryDic.size(); ++i){
    MyFile << Dictionary[i];
    if (i < DictionaryDic.size()-1){
      MyFile << "\n";
    }
  }
  MyFile.close();
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////  Main Function  //////////////////////////////////////



int main (int argc, char* argv[]){
  
  if (*argv[1] == *"1") {
    Creat_Dictionary();
    Encode_file();

  }else if (*argv[1] == *"2"){
    Read_compressed_File();
    Decode_file();
  }
}

