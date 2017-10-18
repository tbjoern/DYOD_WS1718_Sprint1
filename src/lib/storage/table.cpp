#include "table.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "value_column.hpp"

#include "resolve_type.hpp"
#include "types.hpp"
#include "utils/assert.hpp"

namespace opossum {

Table::Table(const uint32_t chunk_size) : m_chunks(), m_column_names(), m_column_types(), m_chunk_size(chunk_size) {
  this->create_new_chunk();
}

void Table::add_column_definition(const std::string& name, const std::string& type) {
  this->m_column_names.push_back(name);
  this->m_column_types.push_back(type);
}

void Table::add_column(const std::string& name, const std::string& type) {
  this->add_column_definition(name, type);
  for (auto& chunk : this->m_chunks) {
    auto column = make_shared_by_column_type<BaseColumn, ValueColumn>(type);
    chunk->add_column(column);
  }
}

void Table::append(std::vector<AllTypeVariant> values) {
  if (this->m_chunks.back()->size() >= this->m_chunk_size) {
    this->create_new_chunk();
  }
  this->m_chunks.back()->append(values);
}

void Table::create_new_chunk() {
  auto chunk = std::make_unique<Chunk>();
  for (auto type : this->m_column_types) {
    auto column = make_shared_by_column_type<BaseColumn, ValueColumn>(type);
    chunk->add_column(column);
  }
  this->m_chunks.push_back(std::move(chunk));
}

uint16_t Table::col_count() const { return this->m_column_names.size(); }

uint64_t Table::row_count() const {
  uint64_t rows = 0;
  for (auto& chunk : this->m_chunks) {
    rows += chunk->size();
  }
  return rows;
}

ChunkID Table::chunk_count() const { return (ChunkID) this->m_chunks.size(); }

ColumnID Table::column_id_by_name(const std::string& column_name) const {
  for (ColumnID i{0}; i < this->m_column_names.size(); ++i) {
    if (this->m_column_names[i] == column_name) {
      return i;
    }
  }
  throw std::invalid_argument("No such column: " + column_name);
}

uint32_t Table::chunk_size() const { return this->m_chunk_size; }

const std::vector<std::string>& Table::column_names() const { return this->m_column_names; }

const std::string& Table::column_name(ColumnID column_id) const { return this->m_column_names[column_id]; }

const std::string& Table::column_type(ColumnID column_id) const { return this->m_column_types[column_id]; }

Chunk& Table::get_chunk(ChunkID chunk_id) { return *(this->m_chunks[chunk_id]); }

const Chunk& Table::get_chunk(ChunkID chunk_id) const { return *(this->m_chunks[chunk_id]); }

}  // namespace opossum
