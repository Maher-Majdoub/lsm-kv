#include "lsm/sstable/base_sstable.h"
#include "lsm/services/config_service.h"

#include <string>
#include <fstream>


BaseSStable::BaseSStable(const std::string& file_name) {
  auto dataFolderPath = ConfigService::get("DATA_FOLDER_PATH").value_or("./data");
  std::filesystem::create_directories(dataFolderPath);
  
  file_path_ = dataFolderPath + "/"+ file_name;
}