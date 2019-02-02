#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "leveldb/db.h"
#include "leveldb/comparator.h"

using namespace std;

// Assumes default comparator is used
void list_entries_with_key_prefix(leveldb::DB & db, const leveldb::Slice & key_prefix) {
    cout << "list entries with starting with key prefix [" << key_prefix.ToString() << "]" << endl;
    leveldb::ReadOptions options;
    leveldb::Iterator* it = db.NewIterator(options);
    for (it->Seek(leveldb::Slice(key_prefix)); it->Valid() && it->key().starts_with(key_prefix); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }
    
    if (!it->status().ok())
    {
        cerr << "Iterator error " << it->status().ToString() << endl; 
    }
    
    delete it;
}

void list_entries_within_range(leveldb::DB & db, leveldb::Options & options, const leveldb::Slice & start, const leveldb::Slice & end) {
    cout << "list entries within range[" << start.ToString() << ":" << end.ToString() << "]" << endl;
    leveldb::Iterator* it = db.NewIterator(leveldb::ReadOptions());
    for (it->Seek(leveldb::Slice(start)); it->Valid() && options.comparator->Compare(it->key(), end)<=0; it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }
    
    if (!it->status().ok())
    {
        cerr << "Iterator error " << it->status().ToString() << endl; 
    }
    
    delete it;
}

void list_all_entries(leveldb::DB & db) {
    cout << "list all entries" << endl;
    leveldb::Iterator* it = db.NewIterator(leveldb::ReadOptions());
    
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }
    
    if (!it->status().ok())
    {
        cerr << "Iterator error " << it->status().ToString() << endl; 
    }
    
    delete it;
}

void create_entries(leveldb::DB & db, unsigned int count) {
    leveldb::WriteOptions writeOptions;
    for (unsigned int i = 0; i < count; ++i)
    {
        ostringstream keyStream;
        keyStream << "Key" << setfill ('0') << setw (6) << i;
        
        ostringstream valueStream;
        valueStream << "Test data value: " << i;
        
        db.Put(writeOptions, keyStream.str(), valueStream.str());
    }

}


int main(int argc, char** argv)
{
    // Set up database connection information and open database
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);

    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './testdb'" << endl;
        cerr << status.ToString() << endl;
        return -1;
    }
    
    // Add 256 values to the database
    create_entries(*db, 25600);

    // Iterate over each item in the database and print them
    // list_all_entries(*db);

    // Iterate over specific item whose key starts with a prefix in the database
    // and print them
    list_entries_with_key_prefix(*db, leveldb::Slice("Key00508"));

    // Iterate over specific item within the specified range in the database
    // and print them
    list_entries_within_range(*db, options, leveldb::Slice("Key015083"), leveldb::Slice("Key015097"));
    

    std::string memory_usage;
    if (db->GetProperty(leveldb::Slice("leveldb.approximate-memory-usage"), &memory_usage)) {
        cout << "Memory usage : " << memory_usage << " bytes " << endl;
    } 

    // Close the database
    delete db;
}

