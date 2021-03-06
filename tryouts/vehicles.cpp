#include <iostream>
#include <string>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <chrono>

#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
using namespace std;

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;
void createentry(mongocxx::database db, string name, int price)
{
    
    bsoncxx::document::value vehicles_doc =
        document {} << "name" << name 
        << "price" << price
        << finalize;
    auto res = db["vehicles"].insert_one(std::move(vehicles_doc));
}
void findentry(mongocxx::database db, string findname, string tofind)
{
    auto cursor = db["vehicles"].find(document{}  << findname << tofind
                                                    << finalize);
    document data_builder{};
    auto array_builder = data_builder << "results" << open_array;
    for (auto&& docView : cursor) {
            array_builder <<  bsoncxx::types::b_document{docView};
    }
    array_builder << close_array;
    bsoncxx::document::value doc = data_builder << finalize;
    std::cout << bsoncxx::to_json(doc) << std::endl;
}
int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
    auto db = conn["crossover"];
    
    
    findentry(db, "model","beta");
    
}   