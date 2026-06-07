#include "lsm/db/wal/wal_manager.h"
#include "lsm/db/wal/wal_builder.h"
#include "lsm/db/wal/wal_iterator.h"
#include "lsm/db/wal/wal.h"
#include "lsm/utils/time.h"

#include <cassert>
#include <filesystem>
#include <optional>

namespace lsm {
  WALManager::WALManager(const std::filesystem::path& work_dir): work_dir_(work_dir) {
    std::filesystem::create_directories(work_dir);
  }

  std::optional<WALIterator> WALManager::recover() {
    std::optional<std::string> current_wal = read_current_file_();
    if (!current_wal) return std::nullopt; // empty

    WAL wal(work_dir_ / *current_wal);
    WALIterator it(wal);
    
    it.first();
    // TODO: move the iterator to the first not considered entry (seq > last_seq)

    return it;
  }

  void WALManager::add_entry(const std::string& key, const std::string& value) {
    if (!wal_builder_) rotate();
    assert(wal_builder_);

    wal_builder_->set(key, value);
  }

  void WALManager::rotate() {
    // TODO: delete the previous wal
    std::string wal_name = "WALL-" + current_timestamp();

    wal_builder_ = WALBuilder(work_dir_ / wal_name);
    update_current_file(wal_name);
  }

  std::optional<std::string> WALManager::read_current_file_() {
    std::filesystem::path file_path = work_dir_ / WALManager::CURRENT_FILE_NAME;

    if (!std::filesystem::exists(file_path)) return std::nullopt;

    std::ifstream current_file(file_path);
    std::string wall_name;
    std::getline(current_file, wall_name);

    if (wall_name.empty()) return std::nullopt;
    return wall_name;
  } 

  void WALManager::update_current_file(const std::string& wal_name) {
    std::filesystem::path path = work_dir_ / WALManager::CURRENT_FILE_NAME;
    std::ofstream current_file(path);
    
    current_file << wal_name << "\n";
    current_file.close();
  }
}