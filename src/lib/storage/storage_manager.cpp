#include "storage_manager.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "utils/assert.hpp"

namespace opossum {

StorageManager& StorageManager::get() {
  throw std::runtime_error("Implement StorageManager::get");
}

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
  // Implementation goes here
}

void StorageManager::reset() {
  // StorageManager::instance = StorageManager{};
  // Implementation goes here
}

}  // namespace opossum
