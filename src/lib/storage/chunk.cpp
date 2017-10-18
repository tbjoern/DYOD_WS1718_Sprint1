#include <iomanip>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "base_column.hpp"
#include "chunk.hpp"

#include "utils/assert.hpp"

namespace opossum {

void Chunk::add_column(std::shared_ptr<BaseColumn> column) { this->columns.push_back(std::move(column)); }

void Chunk::append(std::vector<AllTypeVariant> values) {
  DebugAssert(this->columns.size() == values.size(), "#Values does not match #Columns");
  for (uint i = 0; i < this->columns.size(); ++i) {
    this->columns[i]->append(values[i]);
  }
}

std::shared_ptr<BaseColumn> Chunk::get_column(ColumnID column_id) const {
  return std::shared_ptr<BaseColumn>(this->columns[column_id]);
}

uint16_t Chunk::col_count() const { return this->columns.size(); }

uint32_t Chunk::size() const {
  uint32_t maxRowCount = 0;
  for (auto column : this->columns) {
    maxRowCount = column->size() > maxRowCount ? column->size() : maxRowCount;
  }
  return maxRowCount;
}

}  // namespace opossum
