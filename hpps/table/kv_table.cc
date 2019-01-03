/*!
 * \file kv_table.cc
 * \brief The kv table
 */
#include "hpps/table/kv_table.h"

#include "hpps/common/io/io.h"
#include "hpps/common/log.h"
#include "hpps/updater/updater.h"

namespace hpps {

template <typename Key, typename Val>
KVServerTable<Key, Val>::KVServerTable(const KVTableOption<Key, Val>& kvtable_option) {
  ParamInitializer<Val>::ResetRandomOption(kvtable_option.random_option);
  store_.reset(new KVStore<Key, Val>(kvtable_option.init_capacity, kvtable_option.value_len));
}

template <typename Key, typename Val>
void KVServerTable<Key, Val>::Store(Stream* stream) {

}

template <typename Key, typename Val>
void KVServerTable<Key, Val>::Load(Stream*) {

}

template class KVWorkerTable<uint64_t, float>;
template class KVServerTable<uint64_t, float>;

}  // namespace hpps
