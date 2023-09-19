#include <cstdlib>
#include <iostream>

#include "leveldb/db.h"
#include "leveldb/write_batch.h"

inline void quit_if_err(const leveldb::Status& status, bool quit = true) {
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        if (quit) {
            exit(1);
        }
    }
}

int main() {
    /* system("rm -rf test.db"); */

    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    /* options.error_if_exists = true; */
    leveldb::Status status = leveldb::DB::Open(options, "test.db", &db);
    quit_if_err(status);

    std::string key = "abc";
    std::string value = "1234";
    auto s = db->Put(leveldb::WriteOptions(), key, value);
    quit_if_err(s);
    {
        std::string value;
        auto s = db->Get(leveldb::ReadOptions(), key, &value);
        quit_if_err(s);
        std::cout << "value=" << value << std::endl;
    }
    s = db->Delete(leveldb::WriteOptions(), key);
    quit_if_err(s);

    // Atomic Updates
    {
        std::string key1 = "abc", key2 = "abc";
        std::string value = "1122";
        leveldb::WriteBatch batch;
        batch.Put(key1, value);
        batch.Put(key2, value);
        auto s = db->Write(leveldb::WriteOptions(), &batch);
        quit_if_err(s);

        std::string tmp;
        s = db->Get(leveldb::ReadOptions(), key1, &tmp);
        quit_if_err(s);
        std::cout << "value=" << tmp << std::endl;
    }

    // Synchronous Writes
    {
        std::string key = "abc2";
        std::string value = "1123";
        leveldb::WriteOptions write_options;
        write_options.sync = true;
        auto s = db->Put(write_options, key, value);
        quit_if_err(s);
        std::string tmp;
        s = db->Get(leveldb::ReadOptions(), key, &tmp);
        quit_if_err(s);
        std::cout << "value=" << tmp << std::endl;
    }

    // Iteration
    {
        std::cout << "Iteration" << std::endl;
        leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        }
        quit_if_err(it->status());
        delete it;

        // ranges
        std::cout << "ranges" << std::endl;
        it = db->NewIterator(leveldb::ReadOptions());
        std::string start = "a";
        std::string limit = "z";
        for (it->Seek(start); it->Valid() && it->key().ToString() < limit; it->Next()) {
            std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        }
        quit_if_err(it->status());
        delete it;

        // reverse
        std::cout << "reverse" << std::endl;
        it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToLast(); it->Valid(); it->Prev()) {
            std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        }
        quit_if_err(it->status());
        delete it;
    }

    // Snapshots
    {
        std::cout << "Snapshots" << std::endl;
        leveldb::ReadOptions options;
        options.snapshot = db->GetSnapshot();

        std::string key = "abc";
        std::string value = "000";
        auto s = db->Put(leveldb::WriteOptions(), key, value);
        quit_if_err(s);

        leveldb::Iterator *it = db->NewIterator(options);
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        }
        quit_if_err(it->status());
        delete it;

        db->ReleaseSnapshot(options.snapshot);

        it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
        }
        quit_if_err(it->status());
        delete it;
    }

    // Approximate Sizes
    {
        auto range = leveldb::Range("a", "z");
        uint64_t size;
        db->GetApproximateSizes(&range, 1, &size);
        std::cout << "Approximate Size: " << size << std::endl;
    }

    delete db;

    system("rm -rf test.db");
}
