//----------------------------------------------------------
// CS260 Assignment Starter Code
// Copyright Andrew Scholer (ascholer@chemeketa.edu)
// Neither this code, nor any works derived from it
//    may not be republished without approval.
//----------------------------------------------------------

#include "IndexMap.h"
#include <string.h>

using namespace std;

void IndexMap::grow(){
    IndexRecord* newRecords = new IndexRecord[numBuckets*2];

    for (int i = 0; i < numBuckets; i++){
        newRecords[i] = buckets[i];
    }
    buckets = newRecords;

    numBuckets = numBuckets*2;

    
}


//IndexMap::IndexMap(const IndexMap& other){
//    numBuckets = other.numBuckets;
//    keyCount = other.keyCount;
//    for (int i = 0; i<other.numBuckets; i++){
//        buckets[i] = other.buckets[i];
//    }
//}


IndexMap& IndexMap::operator=(const IndexMap* other){
    delete this;

   numBuckets = other->numBuckets;
   keyCount = other->keyCount;
   buckets = new IndexRecord[numBuckets];
   for (int i = 0; i<other->numBuckets; i++){
       buckets[i] = other->buckets[i];
   }
   return *this;
    
}


int IndexMap::getLocationFor(  std::string& key) const{
    unsigned int hash = 0;

    for (unsigned int i = 0; i < key.length();i++){
        hash = (hash<<6)^key[i];
    }
    int location = hash % numBuckets ;
    return location;
}


IndexMap::IndexMap(int startingBuckets){
    numBuckets = startingBuckets;

    keyCount = 1;
    buckets = new IndexRecord[startingBuckets];

}


IndexMap::~IndexMap(){
    numBuckets = 0;
    keyCount = 0;
    delete[] buckets;
}





void IndexMap::add(const std::string& key, int pageNumber, int wordNumber){
    IndexLocation loc(pageNumber, wordNumber);
    if (keyCount == numBuckets){
        this->grow();
    }
    unsigned int hash = 0;

    for (unsigned int i = 0; i < key.length();i++){
        hash = (hash<<6)^key[i];
    }
    int location = hash % numBuckets ;
    if (this->contains(key)){
        while(buckets[location].word !=key ){
            location++;

        }
        buckets[location].addLocation(loc);
    }else{
        while(buckets[location].word != "?"){
            location++;
        }
        IndexRecord newIndex(key);
        newIndex.addLocation(loc);
        buckets[location] = newIndex;
        keyCount++;
    }

}

void IndexMap::print() const{
    for (int i =0; i<numBuckets; i++){
        if (buckets[i].word != "?"){
            cout << "word: " << buckets[i].word << endl;
            cout << "locations: " ;
            for(unsigned int j = 0; j < buckets[i].locations.size(); j++){
                cout << buckets[i].locations[j].pageNum << " " << buckets[i].locations[j].wordNum << ", ";
            }
            cout << endl;
        }
    }
    cout << endl;
}

bool IndexMap::contains(const std::string& key) const{
    bool cont = false;
    for (int i = 0; i < numBuckets; i++){
        if (buckets[i].word == key){
            cont = true;
        }
    }
    return cont;
}

IndexRecord IndexMap::get(const std::string& word) const{
    unsigned int hash = 0;
    for (unsigned int i = 0; i < word.length();i++){
        hash = (hash << 6)^word[i];
    }
    int location = hash % numBuckets ;
    while (buckets[location].word != word){
        location++;
        if (buckets[location].word == "?" || location >= numBuckets){
            cout << "\nIt is not part of this IndexMap \n";
            return buckets[numBuckets];
        }
    }
    return buckets[location];
}


