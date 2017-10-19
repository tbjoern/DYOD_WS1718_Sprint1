#include "storage_manager.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "utils/assert.hpp"

namespace opossum {

void StorageManager::add_table(const std::string& name, std::shared_ptr<Table> table) {
  this->m_table_map.emplace(name, std::move(table));
}

void StorageManager::drop_table(const std::string& name) {
  if (!this->has_table(name)) {
    throw std::invalid_argument("No such table: " + name);
  }
  this->m_table_map.erase(name);
}

StorageManager& StorageManager::get() {
  static StorageManager instance;
  return instance;
}

std::shared_ptr<Table> StorageManager::get_table(const std::string& name) const {
  auto it = this->m_table_map.find(name);
  if (it == this->m_table_map.end()) {
    throw std::invalid_argument("No such table: " + name);
  }
  return it->second;
}

bool StorageManager::has_table(const std::string& name) const {
  auto it = this->m_table_map.find(name);
  if (it == this->m_table_map.end()) {
    return false;
  }
  return true;
}

std::vector<std::string> StorageManager::table_names() const {
  std::vector<std::string> names{};
  for (auto it = this->m_table_map.begin(); it != this->m_table_map.end(); ++it) {
    names.push_back(it->first);
  }
  return names;
}

void StorageManager::print(std::ostream& out) const {
  for (auto& kv : this->m_table_map) {
    auto& table = *(kv.second);
    out << kv.first << ": " << table.col_count() << "columns, " << table.row_count() << "rows, " << table.chunk_count()
        << "chunks" << std::endl;
  }
}

void StorageManager::reset() { get() = StorageManager(); }

}  // namespace opossum
