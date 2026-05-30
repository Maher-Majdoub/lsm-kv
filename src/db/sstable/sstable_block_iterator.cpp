#include "lsm/db/sstable/sstable_block_iterator.h"
#include "lsm/db/sstable/format.h"

namespace lsm {
  SSTableBlockIterator::SSTableBlockIterator(const std::vector<char>& buffer): buffer_(buffer) {}

  void SSTableBlockIterator::first() { 
    pos_ = 0; 
    update_(); 
  }
  
  void SSTableBlockIterator::next()  { 
    pos_ += current_size_;
    update_();
  }
  
  bool SSTableBlockIterator::is_done() const {
    return pos_ >= buffer_.size();
  }

  const std::string& SSTableBlockIterator::key() const {
    return current_key_;
  }

  const std::string& SSTableBlockIterator::value() const {
    return current_value_;
  }

  void SSTableBlockIterator::update_() {
    current_size_ = 0;
    sstable::RecordHeader record_header;

    std::memcpy(&record_header, buffer_.data() + pos_, sizeof(record_header));
    current_size_ += sizeof(record_header);

    current_key_.assign(buffer_.data() + pos_ + current_size_, record_header.key_size);
    current_size_ += record_header.key_size;

    current_value_.assign(buffer_.data() + pos_ + current_size_, record_header.value_size);
    current_size_ += record_header.value_size;;
  }
}