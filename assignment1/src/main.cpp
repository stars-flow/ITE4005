#include <exception>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include "Item.h"
#include "AssociationRule.h"

using namespace std;

/**
 * Read transactions from input file stream.
 *
 * @param ifs Input file stream.
 * @return Vector of items.
 */
vector<Item> read_items(ifstream &ifs) {
    unordered_map<item_id_t, Item *> map_item;
    string txn_str;
    num_txns = 0;
    while (getline(ifs, txn_str)) {
        istringstream iss(txn_str);
        item_id_t item_id;
        while (iss >> item_id) {
            if (!map_item[item_id]) {
                map_item[item_id] = new Item(item_id);
            }
            map_item[item_id]->add_transaction(num_txns);
        }
        num_txns++;
    }
    vector<Item> vec_item;
    for (auto &item : map_item) {
        vec_item.push_back(*item.second);
    }
    return vec_item;
}

/**
 * Find frequent item sets using Apriori algorithm.
 *
 * @param txns Vector of transactions.
 * @param min_support Minimum support count.
 * @return Vector of frequent item sets.
 */
vector<ItemSet> find_frequent_item_sets(const vector<txn_t> &txns, const int min_support) {
    vector<ItemSet> freq_item_sets;

    auto candidate_sets = transactions_to_itemsets(txns);
    unordered_set<item_id_t> freq_item_set;
    for (const auto &item : item_cnt) {
        if (item.second >= min_support) {
            freq_item_set.insert(item.first);
        }
    }

    // TODO

    return freq_item_sets;
}

/**
 * Find association rules.
 * @param items Vector of items.
 * @param min_support Minimum support value.
 * @return Vector of association rule.
 */
vector<AssociationRule> find_association_rules(const vector<Item> &items, const int min_support) {
    vector<AssociationRule> assc_rules;

    auto freq_item_sets = find_frequent_item_sets(txns, min_support);
    for (const auto &freq_item_set : freq_item_sets) {

    }
    // TODO

    return assc_rules;
}

/**
 * Overloads operator<< to write association rules on file stream.
 * @param ofs Output file stream.
 * @param rules Vector of association rule.
 * @return `ofs`.
 */
ofstream &operator<<(ofstream &ofs, const vector<AssociationRule> &rules) {
    for (const auto &rule : rules) {
        ofs << "{" << rule.item_set.items.cbegin()->id;
        for (auto it = ++rule.item_set.items.cbegin(); it != rule.item_set.items.cend(); it++) {
            ofs << "," << it->id;
        }
        ofs << "}\t";
        ofs << "{" << rule.assc_item_set.items.cbegin()->id;
        for (auto it = ++rule.assc_item_set.items.cbegin(); it != rule.assc_item_set.items.cend(); it++) {
            ofs << "," << it->id;
        }
        ofs << "}\t";
        auto ori_precision = ofs.precision();
        ofs.precision(2);
        ofs << rule.num_support / num_txns << "\t";
        ofs << rule.num_confidence << "\n";
        ofs.precision(ori_precision);
    }
    return ofs;
}

/**
 * Solves assignment1, finding association rules of given transactions.
 * @param argc Argument count, which must be 4.
 * @param argv[1] Minimum support value in percentage.
 * @param argv[2] Input file path.
 * @param argv[3] Output file path.
 * @return Returns 0 if no error occurred, non-zero value otherwise.
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "usage: apriori [0-100] input_file output_file" << endl;
        return 1;
    }

    char *tmp_ptr = argv[1];
    const int min_support_percent = (int) strtol(argv[1], &tmp_ptr, 0);
    ifstream ifs(argv[2]);
    ofstream ofs(argv[3]);
    if (min_support_percent < 0 || min_support_percent > 100 || argv[1] == tmp_ptr) {
        cerr << "minimum support must be integer [0-100]. (%)";
        return EINVAL;
    } else if (!ifs.good()) {
        cerr << "cannot open input file: " << argv[2];
        return 1;
    } else if (!ofs.good()) {
        cerr << "cannot open output file: " << argv[3];
        return 1;
    }

    size_t num_txn;
    auto items = read_items(ifs);
    int min_support = ceil(min_support_percent * num_txns / 100.);
    auto assc_rules = find_association_rules(items, min_support);
    ofs << assc_rules;

    ifs.close();
    ofs.close();
    return 0;
}