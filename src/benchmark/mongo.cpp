// Copyright (c) 2015 Francois Doray <francois.pierre-doray@polymtl.ca>
//
// This file is part of trace-kit.
//
// trace-kit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// trace-kit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with trace-kit.  If not, see <http://www.gnu.org/licenses/>.
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <iostream>
#include <mongo/bson/bson.h>
#include <mongo/client/dbclient.h>
#include <mongo/client/dbclientinterface.h>
#include <unordered_set>
#include <vector>

#include "utils/attributes.h"
#include "utils/time.h"

const char kMongoHost[] = "localhost:27017";
const char kMongoCollection[] = "tracekit.mongowrite";
const size_t kNumStrings = 10;
const size_t kStringSize = 15;

const int kNumTests = 100;
const int kNumInsertions = 200000;

class RandomStringGenerator
{
public:
    RandomStringGenerator()
        : chars("abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"),
          index_dist(0, chars.size() - 1)
    {
    }

    std::string GenerateRandomString(size_t length)
    {
        std::string str;
        for (size_t i = 0; i < length; ++i)
            str += chars[index_dist(rng)];
        return str;
    }

private:
    std::string chars;
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist;
};

void NOINLINE FillBson(const std::vector<std::string>& strings,
                       mongo::BSONObjBuilder& bson)
{
    std::unordered_set<std::string> keys;
    for (const auto& str : strings)
        bson.append(str, str);
}

void NOINLINE InsertInDatabase(mongo::BSONObjBuilder& bson,
                               mongo::DBClientConnection& connection)
{
    connection.insert(kMongoCollection, bson.obj());
}

void NOINLINE Action(const std::vector<std::string>& strings,
                     mongo::DBClientConnection& connection)
{
    for (size_t i = 0; i < kNumInsertions; ++i)
    {
        mongo::BSONObjBuilder bson;
        FillBson(strings, bson);
        InsertInDatabase(bson, connection);
    }
}

int main()
{
    // Generate some random strings.
    RandomStringGenerator generator;
    std::vector<std::string> randomStrings;
    for (size_t i = 0; i < kNumStrings; ++i)
        randomStrings.push_back(generator.GenerateRandomString(kStringSize));

    // Initialize MongoDB.
    mongo::client::initialize();
    mongo::DBClientConnection connection(true);
    std::string errMsg;
    if (!connection.connect(kMongoHost, errMsg))
    {
        std::cerr << "Error while connecting to mongodb: " << errMsg
                  << std::endl;
        return 1;
    }

    // Warm-up.
    Action(randomStrings, connection);
    usleep(200000);

    // Run the benchmark.
    std::cout << "duration," << std::endl;
    for (size_t i = 0; i < kNumTests; ++i)
    {
        // Clean the database.
        mongo::BSONObjBuilder b;
        b.append("dropDatabase", 1);
        mongo::BSONObj r;
        if (!connection.runCommand("tracekit", b.obj(), r))
        {
            std::cerr << "Failed to clean database." << std::endl;
            std::cerr << r.toString() << std::endl;
            return 1;
        }

        // Wait a little bit.
        usleep(200000);

        // Insert data in the database.
        auto start = GetMonotonicTime();
        Action(randomStrings, connection);
        auto end = GetMonotonicTime();
        std::cout << (end - start) << "," << std::endl;
    }

    return 0;
}
